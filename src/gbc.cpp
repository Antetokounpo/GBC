#include<cstring>
#include<iostream>
#include<bitset>
#include<chrono>

#include<SDL2/SDL.h>

#include "gbc.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"

GBC::GBC(SDL_Renderer* r)
{
    ppu = new PPU(r, memory);
    cartridge = 0; /* Empêche de free de la mémoire non-allouée */

    t = std::chrono::steady_clock::now();
}

GBC::~GBC()
{
    delete cartridge;
    delete ppu;
}

bool GBC::load(const char* filename)
{
    if(!Z80::load(filename)) // function from parent class
        return false;

    cartridge = new Cartridge(rom , rom_size);

    if(!cartridge->checksum())
    {
        std::cout << "Checksum failed\n";
        return false;
    }

    std::cout << cartridge->title() << std::endl;

    std::memcpy(memory + 0, rom + 0, 0x3FFF); /* 16KB ROM bank 00 */
    std::memcpy(memory + 0x4000, rom + 0x4000, 0x3FFF); /* 16KB ROM bank 01~NN */

    pc = 0x100;

    return true;
}

uint8_t GBC::fetch(int offset)
{
    return memory[pc+offset];
}

void GBC::get_frame()
{
    ppu->update();
}

void GBC::execute(uint8_t opcode)
{
    #ifdef GBC_DEBUG
	    std::cout << std::hex << "PC: " << (uint)pc << "\tSP: " << (uint)sp << std::endl;
        std::cout << std::hex << "opcode: " << (uint)opcode << std::endl;
        std::bitset<8> x(*F);
        std::cout << "Flags: " << x << std::endl;
        std::cout << std::hex << "A: " << (uint)(*A) << "\tB: " << (uint)(*B) << "\tH: " << uint(*H) << "\tL: " << uint(*L) << std::endl;
        std::cout << std::hex << "args: " << get_operand(2) << std::endl;
        std::cout << std::endl;
        #ifdef GBC_STEP
            if(getchar() == 'q')
                exit(0);
        #endif
    #endif

    switch(opcode)
    {
        case 0x08:
            cycles += 20;
            ld(memory[get_operand(2)], sp);
            pc += 3; break;
        case 0x10:
            // TODO STOP
            break;
        case 0x22: /* ldi (HL), A */
            cycles += 8;
            ld(memory[HL.p], *A);
            HL.p++;
            pc++;break;
        case 0x2A: /* ldi A, (HL) */
            cycles += 8;
            ld(*A, memory[HL.p]);
            HL.p++;
            pc++; break;
        case 0x32: /* ldd (HL), A */
            cycles += 8;
            ld(memory[HL.p], *A);
            HL.p--;
            pc++; break;
        case 0x3A: /* ldd A, (HL) */
            cycles += 8;
            ld(*A, memory[HL.p]);
            HL.p--;
            pc++; break;
        case 0xD9: /* reti */
            cycles += 16;
            ei();
            pop(pc);
            break;
        case 0xE0: /* ld (FF00+n), A */
            cycles += 12;
            ld(memory[0xFF00+get_operand(1)], *A);
            pc += 2; break;
        case 0xE2: /* ld (FF00+C), A */
            cycles += 8;
            ld(memory[0xFF00+(*C)], *A);
            pc++; break;
        case 0xE8: /* add sp, dd */
            cycles += 16;
            add(sp, twoscomp(get_operand(1)));
            pc += 2; break;
        case 0xEA: /* ld (nn), A */
            cycles += 16;
            ld(memory[get_operand(2)], *A);
            pc += 3; break;
        case 0xF0: /* ld A, (FF00+n) */
            cycles += 12;
            ld(*A, memory[0xFF00+get_operand(1)]);
            pc += 2; break;
        case 0xF2: /* ld A, (FF00+C) */
            cycles += 8;
            ld(*A, memory[0xFF00+(*C)]);
            pc++; break;
        case 0xF8: /* ld HL, sp+dd */
            cycles += 12;
            ld(HL.p, sp+twoscomp(get_operand(1)));
            pc += 2; break;
        case 0xFA: /* ld A, (nn) */
            cycles += 16;
            ld(*A, memory[get_operand(2)]);
            pc += 3; break;
        case 0xCB:
            pc++;
            interpret_bits(fetch(0)); /* Cycle incrementation inside of function */
            pc++; break;

        case 0xD3:
        case 0xD8:
        case 0xDD:
        case 0xE3:
        case 0xE4:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xF4:
        case 0xFC:
        case 0xFD:
            std::cout << "Invalid GMB opcode\n" << std::endl;
            exit(EXIT_FAILURE);
            break;

        default:
            Z80::execute(opcode);
            break;
    }

    if(memory[0xFF02] == 0x81)
        std::cout << char(memory[0xFF01]);

    timing();
    interrupt_handler();
}

void GBC::interpret_bits(uint8_t opcode)
{
    uint8_t* registers[] = {B, C, D, E, H, L, &(memory[HL.p]), A};

    uint8_t high_nibble = opcode >> 4;
    uint8_t low_nibble = opcode & 0xF;

    switch(high_nibble)
    {
        case 0x3:
            if(low_nibble < 0x8)
            {
                cycles += (low_nibble == 0x6) ? 15 : 8;
                uint8_t& r = *(registers[low_nibble]);
                r = (r << 4) + (r >> 4);
                break;
            }
        default:
            Z80::interpret_bits(opcode);
            break;
    }
}

void GBC::interrupt_handler()
{
    if(!iff1)
        return;

    for(int i = 0; i<5; ++i)
    {
        if(memory[0xFFFF] & (0x1 << i))
        {
            cycles += 20;
            push(pc);
            iff1 = false;

            switch(i)
            {
                case 0: /* V-Blank */
                    pc = 0x40;
                    break;
                case 1: /* LCD STAT */
                    pc = 0x48;
                    break;
                case 2: /* Timer */
                    pc = 0x50;
                    break;
                case 3: /* Serial */
                    pc = 0x58;
                    break;
                case 4: /* Joypad */
                    pc = 0x60;
                    break;
            }
        }
    }

}

void GBC::timing()
{
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - t);
    
    if(time_span >= std::chrono::microseconds(1000000/16384))
        memory[0xFF04] += 1; /* DIV - Divider Register */
    if(memory[0xFF07] & (0x1 << 2))
    {
        int clock;
        switch(memory[0xFF07] & 0x3)
        {
            case 0x0:
                clock = 4096;
                break;
            case 0x1:
                clock = 262144;
                break;
            case 0x2:
                clock = 65536;
                break;
            case 0x3:
                clock = 16384;
                break;
        }

        if(time_span >= std::chrono::microseconds(1000000/clock))
        {
            if(memory[0xFF05] == 0xFF)
            {
                memory[0xFF05] = memory[0xFF06];
                // request interrupt TODO
            }
            else
                memory[0xFF05] += 1;
        }
    }
    


}
