#include "Z80/z80.hpp"

class GBC : public Z80::Z80
{
    public:
        bool load(const char* filename);
        void step();
    protected:
        void execute(uint8_t opcode);
        void interpret_bits(uint8_t opcode);
};
