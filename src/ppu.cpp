#include<cstdint>

#include<SDL2/SDL.h>

#include "ppu.hpp"

#include<iostream> // DEBUG

PPU::PPU(SDL_Renderer* r, uint8_t* m) : renderer(r), memory(m)
{
    background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8, 8);
}

PPU::~PPU()
{
    SDL_DestroyTexture(background);
}

void PPU::update()
{
    get_background();
    SDL_RenderCopy(renderer, background, NULL, NULL);
}

SDL_Texture* PPU::get_background()
{
    void* pixels;
    int pitch;
    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    Uint32 color;
    Uint32* mpixels;

    SDL_LockTexture(background, NULL, &pixels, &pitch);
    mpixels = (Uint32*)pixels;

    for(int i = 0; i<16; ++i)
    {
        uint8_t b = memory[0x8000+i];
        std::cout << (uint)b << std::endl;
        for(int j = 0; j<4; ++j)
        {
            switch(b >> (2*j) & 0x3)
            {
                case 0x0:
                    color = SDL_MapRGBA(format, 0xFF, 0xFF, 0xFF, 0xFF); /* White */
                    break;
                case 0x1:
                    color = SDL_MapRGBA(format, 0xD3, 0xD3, 0xD3, 0xFF); /* Light grey */
                    break;
                case 0x2:
                    color = SDL_MapRGBA(format, 0xA9, 0xA9, 0xA9, 0xFF); /* Dark grey */
                    break;
                case 0x3:
                    color = SDL_MapRGBA(format, 0, 0, 0, 0xFF); /* White */
                    break;
            }
            mpixels[i*4+j] = color;
        }

    }

    SDL_UnlockTexture(background);
    SDL_FreeFormat(format);
    return background;
}