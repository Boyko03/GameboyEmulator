#pragma once
#include "common.h"

struct emu_context
{
    bool paused;
    bool running;
    u64 ticks;
};

class emulator
{
public:
    emu_context* get_context() { return &ctx_; }

    int run(int argc, char** argv);

private:
    emu_context ctx_ = {};
};

