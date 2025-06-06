#include "emulator.h"

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "cartridge.h"

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

    cartridge cartridge;

    if (!cartridge.load(argv[1]))
    {
        std::cout << "Failed to write ROM file: " << argv[1] << "\n";
        return -2;
    }

    std::cout << "Cart loaded...\n";
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "SDL INIT\n";
    TTF_Init();
    std::cout << "TTF INIT\n";

    //cpu_init();

    ctx_.running = true;
    ctx_.paused = false;
    ctx_.ticks = 0;

    while (ctx_.running)
    {
        if (ctx_.paused)
        {
            delay(10);
            continue;
        }

        //if (!cpu_step())
        {
            std::cout << "CPU Stopped!\n";
            return -3;
        }

        ctx_.ticks++;
    }

    return 0;
}
