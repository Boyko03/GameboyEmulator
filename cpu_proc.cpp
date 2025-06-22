#include <unordered_map>

#include "cpu.h"
#include "emulator.h"
#include "instructions.h"

static void proc_none(cpu_context* ctx)
{
    printf("INVALID INSTRUCTION!\n");
    exit(-7);
}

static void proc_nop(cpu_context* ctx)
{
}


static void proc_ld(cpu_context* ctx)
{
    // TODO
}

static void cpu_set_flags(cpu_context* ctx, const char z, const char n, const char h, const char c)
{
    if (z != -1)
    {
        BIT_SET(ctx->regs.f, 7, z);
    }

    if (n != -1)
    {
        BIT_SET(ctx->regs.f, 6, n);
    }

    if (h != -1)
    {
        BIT_SET(ctx->regs.f, 5, h);
    }

    if (c != -1)
    {
        BIT_SET(ctx->regs.f, 4, c);
    }
}

static void proc_xor(cpu_context* ctx)
{
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a, 0, 0, 0);
}

static bool check_cond(const cpu_context* ctx)
{
    const bool z = CPU_FLAG_Z;
    const bool c = CPU_FLAG_C;

    switch (ctx->cur_inst->cond)
    {
    case cond_type::CT_NONE: return true;
    case cond_type::CT_C: return c;
    case cond_type::CT_NC: return !c;
    case cond_type::CT_Z: return z;
    case cond_type::CT_NZ: return !z;
    }

    return false;
}

static void proc_jp(cpu_context* ctx)
{
    if (check_cond(ctx))
    {
        ctx->regs.pc = ctx->fetched_data;
        emulator::cycle(1);
    }
}

static void proc_di(cpu_context* ctx)
{
    ctx->int_master_enabled = false;
}

namespace cpu
{
    static std::unordered_map<in_type, IN_PROC> processors = {
        {in_type::IN_NONE, proc_none},
        {in_type::IN_NOP, proc_nop},
        {in_type::IN_LD, proc_ld},
        {in_type::IN_JP, proc_jp},
        {in_type::IN_DI, proc_di},
        {in_type::IN_XOR, proc_xor}
    };

    IN_PROC inst_get_processor(const in_type type)
    {
        return processors[type];
    }
}
