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
            ld(HL.p, *A);
            HL.p++;
            pc++;break;
        case 0x2A: /* ldi A, (HL) */
            ld(*A, HL.p);
            HL.p++;
            pc++;break;
        case 0x32: /* ldd (HL), A */
            ld(HL.p, *A);
            HL.p--;
            pc++;break;
        case 0x3A: /* ldd A, (HL) */
            ld(*A, HL.p);
            HL.p--;
            pc++;break;
        case 0xD3: /* - */
            break;
        case 0xD9: /* reti */
            // TODO RETI
            break;
    }
}
