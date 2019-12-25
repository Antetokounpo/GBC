#include "gbc.hpp"

bool GBC::load(const char* filename)
{
    if(!Z80::load(filename)) // function from parent class
        return false;

    std::memmove(memory + 0, rom + 0, 0x3FFF); /* 16KB ROM bank 00 */
    std::memmove(memory + 0x4000, rom + 0x4000, 0x3FFF); /* 16KB ROM bank 01~NN */

    return true;
}

void GBC::step()
{
    uint8_t opcode;
    opcode = memory[pc];
    execute(opcode);
}

void GBC::execute(uint8_t opcode)
{
    switch(opcode)
    {
        case 0x08:
            ld(memory[get_operand(2)], sp);
            pc += 3; break;
        case 0x10:
            // TODO STOP
            break;
        case 0x22: /* ldi (HL), A */
            ld(memory[HL.p], *A);
            HL.p++;
            pc++;break;
        case 0x2A: /* ldi A, (HL) */
            ld(*A, memory[HL.p]);
            HL.p++;
            pc++;break;
        case 0x32: /* ldd (HL), A */
            ld(memory[HL.p], *A);
            HL.p--;
            pc++;break;
        case 0x3A: /* ldd A, (HL) */
            ld(*A, memory[HL.p]);
            HL.p--;
            pc++;break;
        case 0xD3: /* - */
            break;
        case 0xD9: /* reti */
            // TODO RETI
            break;
        case 0xDB: /* - */
            break;
        case 0xDD: /* - */
            break;
        case 0xE0: /* ld (FF00+n), A */
            ld(memory[0xFF00+get_operand(1)], *A);
            pc += 2; break;
        case 0xE2: /* ld (FF00+C), A */
            ld(memory[0xFF00+(*C)], *A);
            pc++; break;
        case 0xE3: /* - */
            break;
        case 0xE4: /* - */
            break;
        case 0xE8: /* add sp, dd */
            add(sp, (int8_t)get_operand(1));
            pc += 2; break;
        case 0xEA: /* ld (nn), A */
            ld(memory[get_operand(2)], *A);
            pc += 3; break;
        case 0xEB: /* - */
            break;
        case 0xEC: /* - */
            break;
        case 0xED: /* - */
            break;
        case 0xF0: /* ld A, (FF00+n) */
            ld(*A, memory[0xFF00+get_operand(1)]);
            pc += 2; break;
        case 0xF2: /* ld A, (FF00+C) */
            ld(*A, memory[0xFF00+(*C)]);
            pc++; break;
        case 0xF4: /* - */
            break;
        case 0xF8: /* ld HL, sp+dd */
            ld(HL.p, sp+(int8_t)get_operand(1));
            pc += 2; break;
        case 0xFA: /* ld A, (nn) */
            ld(*A, memory[get_operand(2)]);
            pc += 3; break;
        case 0xFC:
            break;
        case 0xFD:
            break;
        case 0xCB:
            interpret_bits(opcode);
            pc++; break;
        default:
            Z80::execute(opcode);
            break;
    }
}

void GBC::interpret_bits(uint8_t opcode)
{
    uint8_t* registers[] = {B, C, D, E, H, L, &(memory[HL.p]), A};

    uint8_t high_nibble = opcode >> 4;
    uint8_t low_nibble = opcode & 0xF;

    uint8_t& r = *(registers[low_nibble - 0x8]);

    switch(high_nibble)
    {
        case 0x3:
            r = (r << 4) + (r >> 4);
            break;
        default:
            Z80::interpret_bits(opcode);
            break;
    }
}
