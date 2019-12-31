#ifndef GBC_H
#define GBC_H

#include "Z80/z80.hpp"
#include "cartridge.hpp"

class GBC : public Z80::Z80
{
    public:
        GBC();
        ~GBC();
        bool load(const char* filename);
        uint8_t fetch();
        uint8_t fetch(int offset);
    protected:
        void execute(uint8_t opcode);
        void interpret_bits(uint8_t opcode);

        Cartridge* cartridge;        
};

#endif
