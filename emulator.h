#pragma once
#include "common.h"

struct emu_context
{
    bool paused;
    bool running;
    u64 ticks;
};

namespace emulator
{
    static emu_context ctx = {};

    int run(int argc, char** argv);

    void cycle(int cpu_cycles);

};

