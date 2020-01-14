#ifndef PPU_H
#define PPU_H

#include<cstdint>

#include<SDL2/SDL.h>

class PPU
{
    public:
        PPU(SDL_Renderer* r, uint8_t* m);
        ~PPU();
        SDL_Texture* get_background();
        void update();
    private:
        SDL_Renderer* renderer;
        SDL_Texture* background;

        uint8_t* memory;
};

#endif