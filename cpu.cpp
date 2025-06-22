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
        case addr_mode::AM_R:
            ctx.fetched_data = read_reg(ctx.cur_inst->reg_1);
            return;
        case addr_mode::AM_R_R:
            ctx.fetched_data = read_reg(ctx.cur_inst->reg_2);
            return;
        case addr_mode::AM_R_D8:
        case addr_mode::AM_R_A8:
        case addr_mode::AM_HL_SPR:
        case addr_mode::AM_D8:
            ctx.fetched_data = bus::read(ctx.regs.pc);
            emulator::cycle(1);
            ctx.regs.pc++;
            return;
        case addr_mode::AM_R_D16:
        case addr_mode::AM_D16: {
            const u16 lo = bus::read(ctx.regs.pc);
            emulator::cycle(1);

            const u16 hi = bus::read(ctx.regs.pc + 1);
            emulator::cycle(1);

            ctx.fetched_data = lo | (hi << 8);

            ctx.regs.pc += 2;

            return;
        }
        case addr_mode::AM_MR_R:
            ctx.fetched_data = read_reg(ctx.cur_inst->reg_2);
            ctx.mem_dest = read_reg(ctx.cur_inst->reg_1);
            ctx.dest_is_mem = true;

            if (ctx.cur_inst->reg_1 == reg_type::RT_C)
            {
                ctx.mem_dest |= 0xFF00;
            }
            return;
        case addr_mode::AM_R_MR:
        {
            u16 addr = read_reg(ctx.cur_inst->reg_2);

            if (ctx.cur_inst->reg_1 == reg_type::RT_C)
            {
                addr |= 0xFF00;
            }

            ctx.fetched_data = bus::read(addr);
            emulator::cycle(1);
            return;
        }
        case addr_mode::AM_R_HLI:
            ctx.fetched_data = bus::read(read_reg(ctx.cur_inst->reg_2));
            emulator::cycle(1);
            set_reg(reg_type::RT_HL, read_reg(reg_type::RT_HL) + 1);
            return;
        case addr_mode::AM_R_HLD:
            ctx.fetched_data = bus::read(read_reg(ctx.cur_inst->reg_2));
            emulator::cycle(1);
            set_reg(reg_type::RT_HL, read_reg(reg_type::RT_HL) - 1);
            return;
        case addr_mode::AM_HLI_R:
            ctx.fetched_data = read_reg(ctx.cur_inst->reg_2);
            ctx.mem_dest = read_reg(ctx.cur_inst->reg_1);
            ctx.dest_is_mem = true;
            set_reg(reg_type::RT_HL, read_reg(reg_type::RT_HL) + 1);
            return;
        case addr_mode::AM_HLD_R:
            ctx.fetched_data = read_reg(ctx.cur_inst->reg_2);
            ctx.mem_dest = read_reg(ctx.cur_inst->reg_1);
            ctx.dest_is_mem = true;
            set_reg(reg_type::RT_HL, read_reg(reg_type::RT_HL) - 1);
            return;

        case addr_mode::AM_A8_R:
            ctx.mem_dest = bus::read(ctx.regs.pc) | 0xFF00;
            ctx.dest_is_mem = true;
            emulator::cycle(1);
            ctx.regs.pc++;
            return;
        case addr_mode::AM_A16_R:
        case addr_mode::AM_D16_R: {
            const u16 lo = bus::read(ctx.regs.pc);
            emulator::cycle(1);

            const u16 hi = bus::read(ctx.regs.pc + 1);
            emulator::cycle(1);

            ctx.mem_dest = lo | (hi << 8);
            ctx.dest_is_mem = true;

            ctx.regs.pc += 2;
            ctx.fetched_data = read_reg(ctx.cur_inst->reg_2);

            return;
        }
        case addr_mode::AM_MR_D8:
            ctx.fetched_data = bus::read(ctx.regs.pc);
            emulator::cycle(1);
            ctx.regs.pc++;
            ctx.mem_dest = read_reg(ctx.cur_inst->reg_1);
            ctx.dest_is_mem = true;
            return;
        case addr_mode::AM_MR:
            ctx.mem_dest = read_reg(ctx.cur_inst->reg_1);
            ctx.dest_is_mem = true;
            ctx.fetched_data = bus::read(read_reg(ctx.cur_inst->reg_1));
            emulator::cycle(1);
            return;
        case addr_mode::AM_R_A16:
        {
            u16 lo = bus::read(ctx.regs.pc);
            emulator::cycle(1);

            u16 hi = bus::read(ctx.regs.pc + 1);
            emulator::cycle(1);

            u16 addr = lo | (hi << 8);

            ctx.regs.pc += 2;
            ctx.fetched_data = bus::read(addr);
            emulator::cycle(1);

            return;
        }
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

    void set_reg(const reg_type rt, u16 val)
    {
        switch (rt) {
        case reg_type::RT_A: ctx.regs.a = val & 0xFF; break;
        case reg_type::RT_F: ctx.regs.f = val & 0xFF; break;
        case reg_type::RT_B: ctx.regs.b = val & 0xFF; break;
        case reg_type::RT_C: {
            ctx.regs.c = val & 0xFF;
        } break;
        case reg_type::RT_D: ctx.regs.d = val & 0xFF; break;
        case reg_type::RT_E: ctx.regs.e = val & 0xFF; break;
        case reg_type::RT_H: ctx.regs.h = val & 0xFF; break;
        case reg_type::RT_L: ctx.regs.l = val & 0xFF; break;

        case reg_type::RT_AF: *((u16*)&ctx.regs.a) = reverse(val); break;
        case reg_type::RT_BC: *((u16*)&ctx.regs.b) = reverse(val); break;
        case reg_type::RT_DE: *((u16*)&ctx.regs.d) = reverse(val); break;
        case reg_type::RT_HL: {
            *((u16*)&ctx.regs.h) = reverse(val);
            break;
        }

        case reg_type::RT_PC: ctx.regs.pc = val; break;
        case reg_type::RT_SP: ctx.regs.sp = val; break;
        case reg_type::RT_NONE: break;
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
