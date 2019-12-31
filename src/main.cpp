#include<iostream>

#include "gbc.hpp"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename]\n";
        return -1;
    }

    GBC cpu;

    if(!cpu.load(argv[1]))
        return -1;

    bool quit = false;

    while(!quit)
    {
        cpu.step();
    }


    return 0;
}