#ifndef GBC_H
#define GBC_H

#include<chrono>

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
    private:
        void execute(uint8_t opcode);
        void interpret_bits(uint8_t opcode);
        void interrupt_handler();
        void timing();

        Cartridge* cartridge;
        PPU* ppu;

        std::chrono::steady_clock::time_point t;
};

#endif
