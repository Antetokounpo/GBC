#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include<cstddef>

class Cartridge
{
    public:
        Cartridge(uint8_t* rom, size_t rom_size);
        ~Cartridge();
        uint8_t& operator[] (size_t idx);
        bool checksum();
        char* title();
        int cartridge_type();
    private:
        uint8_t* data;
        size_t data_size;
};

#endif
