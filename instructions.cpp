#include "instructions.h"

#include <unordered_map>

static std::unordered_map<u8, instruction> instructions = {
    { 0x00, {in_type::IN_NOP, addr_mode::AM_IMP} },
    {0x05, {in_type::IN_DEC, addr_mode::AM_R, reg_type::RT_B}},
    {0x0E, {in_type::IN_LD, addr_mode::AM_R_D8, reg_type::RT_C}},
    {0xAF, {in_type::IN_XOR, addr_mode::AM_R, reg_type::RT_A}},
    {0xC3, {in_type::IN_JP, addr_mode::AM_D16}},
    {0xF3, {in_type::IN_DI}}
};

instruction* instruction_by_opcode(const u8 opcode)
{
    /*if (instructions.find(opcode) != instructions.end())
    {
        if (instructions[opcode].type == in_type::IN_NONE)
        {
            return nullptr;
        }
        return &instructions[opcode];
    }

    return nullptr;*/

    return &instructions[opcode];
}

const char* inst_lookup[] = {
    "<NONE>",
    "NOP",
    "LD",
    "INC",
    "DEC",
    "RLCA",
    "ADD",
    "RRCA",
    "STOP",
    "RLA",
    "JR",
    "RRA",
    "DAA",
    "CPL",
    "SCF",
    "CCF",
    "HALT",
    "ADC",
    "SUB",
    "SBC",
    "AND",
    "XOR",
    "OR",
    "CP",
    "POP",
    "JP",
    "PUSH",
    "RET",
    "CB",
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "IN_ERR",
    "IN_RLC",
    "IN_RRC",
    "IN_RL",
    "IN_RR",
    "IN_SLA",
    "IN_SRA",
    "IN_SWAP",
    "IN_SRL",
    "IN_BIT",
    "IN_RES",
    "IN_SET"
};

const char* inst_name(in_type t) {
    return inst_lookup[static_cast<int>(t)];
}
