#include<cstdint>

#include<SDL2/SDL.h>

class PPU
{
    public:
        PPU();
        ~PPU();
        //get_frame();
        void update();
        void refresh_memory(uint8_t* memory);
    private:

};