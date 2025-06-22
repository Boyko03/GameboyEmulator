#include "cpu.h"

#include "emulator.h"
#include "bus.h"
#include "instructions.h"


void cpu::init()
{
    ctx.regs.pc = 0x100;
    ctx.regs.a = 0x01;
}

namespace cpu {
    u16 read_reg(const reg_type rt);

    void fetch_instruction()
    {
        ctx.cur_opcode = bus::read(ctx.regs.pc++);
        ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);
    }

    void fetch_data()
    {
        ctx.mem_dest = 0;
        ctx.dest_is_mem = false;

        if (!ctx.cur_inst) return;

        switch (ctx.cur_inst->mode)
        {
        case addr_mode::AM_IMP: return;
        /*case addr_mode::AM_R_D16:
            break;
        case addr_mode::AM_R_R:
            break;
        case addr_mode::AM_MR_R:
            break;*/
        case addr_mode::AM_R:
            ctx.fetched_data = read_reg(ctx.cur_inst->reg_1);
            return;
        case addr_mode::AM_R_D8:
            ctx.fetched_data = bus::read(ctx.regs.pc);
            emulator::cycle(1);
            ctx.regs.pc++;
            return;
        /*case addr_mode::AM_R_MR:
            break;
        case addr_mode::AM_R_HLI:
            break;
        case addr_mode::AM_R_HLD:
            break;
        case addr_mode::AM_HLI_R:
            break;
        case addr_mode::AM_HLD_R:
            break;
        case addr_mode::AM_R_A8:
            break;
        case addr_mode::AM_A8_R:
            break;
        case addr_mode::AM_HL_SPR:
            break;*/
        case addr_mode::AM_D16: {
            const u16 lo = bus::read(ctx.regs.pc);
            emulator::cycle(1);

            const u16 hi = bus::read(ctx.regs.pc + 1);
            emulator::cycle(1);

            ctx.fetched_data = lo | (hi << 8);

            ctx.regs.pc += 2;

            return;
        }
        /*case addr_mode::AM_D8:
            break;
        case addr_mode::AM_D16_R:
            break;
        case addr_mode::AM_MR_D8:
            break;
        case addr_mode::AM_MR:
            break;
        case addr_mode::AM_A16_R:
            break;
        case addr_mode::AM_R_A16:
            break;*/
        default:
            printf("Unknown Addressing Mode! %d (%02X)\n", ctx.cur_inst->mode, ctx.cur_opcode);
            exit(-7);
            return;
        }
    }

    void execute()
    {
        IN_PROC proc = inst_get_processor(ctx.cur_inst->type);

        if (!proc)
        {
            NO_IMPL
        }

        proc(&ctx);
    }

    u16 reverse(const u16 n) {
        return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
    }

    u16 read_reg(const reg_type rt) {
        switch (rt) {
        case reg_type::RT_A: return ctx.regs.a;
        case reg_type::RT_F: return ctx.regs.f;
        case reg_type::RT_B: return ctx.regs.b;
        case reg_type::RT_C: return ctx.regs.c;
        case reg_type::RT_D: return ctx.regs.d;
        case reg_type::RT_E: return ctx.regs.e;
        case reg_type::RT_H: return ctx.regs.h;
        case reg_type::RT_L: return ctx.regs.l;

        case reg_type::RT_AF: return reverse(*reinterpret_cast<u16*>(&ctx.regs.a));
        case reg_type::RT_BC: return reverse(*reinterpret_cast<u16*>(&ctx.regs.b));
        case reg_type::RT_DE: return reverse(*reinterpret_cast<u16*>(&ctx.regs.d));
        case reg_type::RT_HL: return reverse(*reinterpret_cast<u16*>(&ctx.regs.h));

        case reg_type::RT_PC: return ctx.regs.pc;
        case reg_type::RT_SP: return ctx.regs.sp;
        default: return 0;
        }
    }
}

bool cpu::step()
{
    if (!ctx.halted)
    {
        const u16 pc = ctx.regs.pc;

        fetch_instruction();
        fetch_data();

        printf("%04X: %-7s (%02X, %02X, %02X) A: %02X B: %02X C: %02X\n",
            pc, inst_name(ctx.cur_inst->type), ctx.cur_opcode, bus::read(pc + 1),
            bus::read(pc + 2), ctx.regs.a, ctx.regs.b, ctx.regs.c);

        if (!ctx.cur_inst)
        {
            printf("Unknown instruction! %02X\n", ctx.cur_opcode);
            exit(-7);
        }

        execute();
    }
    return true;
}
