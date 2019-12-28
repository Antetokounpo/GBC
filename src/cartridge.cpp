#include<iostream>

#include<cstring>
#include<stdexcept>

#include "cartridge.hpp"

Cartridge::Cartridge(uint8_t* rom, size_t rom_size)
{
    data = new uint8_t[rom_size];
    memcpy(data, rom, rom_size);
    data_size = rom_size;
}

Cartridge::~Cartridge()
{
    delete[] data;
}

uint8_t& Cartridge::operator[] (size_t idx)
{
    if(idx < data_size)
        return data[idx];
    throw std::out_of_range("Index out of range");
}

bool Cartridge::checksum()
{
    uint8_t nintendo_bitmap[48] = {
        0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
        0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
        0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
    };

    if(memcmp(nintendo_bitmap, &(data[0x0104]), 48))
        return false;

    int x = 0;
    for(int i = 0x134; i<0x14C+1; ++i)
    {
        x = x - data[i] - 1;
    }

    if((uint8_t)x != data[0x014D])
        return false;
    
    return true;
}

char* Cartridge::title()
{
    return (char*)&(data[0x134]);
}
