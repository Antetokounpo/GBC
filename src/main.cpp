#include<iostream>

#include<SDL2/SDL.h>

#include "gbc.hpp"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename]\n";
        return -1;
    }

    GBC cpu;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return -1;
    SDL_Window* window = SDL_CreateWindow("gbemu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 576, NULL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, NULL);

    if(!cpu.load(argv[1]))
        return -1;

    bool quit = false;
    SDL_Event e;

    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
                quit = true;
        }

        cpu.step();
        std::cout << "Frame!" << std::endl;
    }

    return 0;
}