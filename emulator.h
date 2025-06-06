#pragma once
#include <cstdint>

typedef struct
{
    bool paused;
    bool running;
    uint64_t ticks;
} emu_context;

class emulator
{
public:
    emulator();

    emu_context* get_context() { return &context_; }

    int run(int argc, char** argv);

private:
    emu_context context_;
};

