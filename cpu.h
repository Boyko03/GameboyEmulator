#pragma once
#include "common.h"

struct instruction;

struct cpu_registers
{
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 sp;
    u16 pc;
};

struct cpu_context
{
    cpu_registers regs;

    // current fetch
    u16 fetched_data;
    u16 mem_dest;
    bool dest_is_mem;
    u8 cur_opcode;
    instruction* cur_inst;

    bool halted;
    bool stepping;
};

namespace cpu
{
    void init();
    bool step();

    static cpu_context ctx;
};

