#include "emulator.h"

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "cartridge.h"
#include "cpu.h"

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

    if (!cart::load(argv[1]))
    {
        std::cout << "Failed to write ROM file: " << argv[1] << "\n";
        return -2;
    }

    std::cout << "Cart loaded...\n";
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "SDL INIT\n";
    TTF_Init();
    std::cout << "TTF INIT\n";

    cpu::init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while (ctx.running)
    {
        if (ctx.paused)
        {
            delay(10);
            continue;
        }

        if (!cpu::step())
        {
            std::cout << "CPU Stopped!\n";
            return -3;
        }

        ctx.ticks++;
    }

    return 0;
}

void emulator::cycle(int cpu_cycles)
{
    // TODO
}
