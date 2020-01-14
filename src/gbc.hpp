#ifndef GBC_H
#define GBC_H

#include <SDL2/SDL.h>

#include "Z80/z80.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"

class GBC : public Z80::Z80
{
    public:
        GBC(SDL_Renderer* r);
        ~GBC();
        bool load(const char* filename);
        uint8_t fetch();
        uint8_t fetch(int offset);
        void get_frame();
    protected:
        void execute(uint8_t opcode);
        void interpret_bits(uint8_t opcode);
        void interrupt_handler();

        Cartridge* cartridge;
        PPU* ppu;
};

#endif
