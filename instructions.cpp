#include "instructions.h"

#include <unordered_map>

static std::unordered_map<u8, instruction> instructions = {
    { 0x00, {in_type::IN_NOP, addr_mode::AM_IMP} },
    {0x05, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_B}},
    {0x0E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_C}},
    {0xAF, {in_type::IN_XOR, addr_mode::AM_R, reg_type::RT_A}},
    {0xC3, {in_type::IN_JP, addr_mode::AM_D16}}
};

instruction* instruction_by_opcode(const u8 opcode)
{
    if (instructions.find(opcode) != instructions.end())
    {
        if (instructions[opcode].type == in_type::IN_NONE)
        {
            return nullptr;
        }
        return &instructions[opcode];
    }

    return nullptr;
}
