#include "emulator.h"

#include <iostream>
#include <SDL3/SDL.h>

emulator::emulator()
{
    context_ = { false, false, 0 };
}

void delay(const uint32_t ms)
{
    SDL_Delay(ms);
}

int emulator::run(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: emu <rom_file>\n";
        return -1;
    }

    //if (!cart_load(argv[1]))
    {
        std::cout << "Failed to write ROM file: " << argv[1] << "\n";
        return -2;
    }

    std::cout << "Cart loaded...\n";
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "SDL INIT\n";

    return 0;
}
