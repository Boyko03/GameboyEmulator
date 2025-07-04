#include "ram.h"

struct ram_context
{
    u8 wram[0x2000];
    u8 hram[0x80];
};

static ram_context ctx;

namespace wram
{
    u8 read(u16 address)
    {
        address -= 0xC000;

        if (address >= 0x2000 || address < 0)
        {
            printf("INVALID WRAM ADDRESS: %08X\n", address + 0xC000);
            exit(-1);
        }

        return ctx.wram[address];
    }

    void write(u16 address, const u8 value)
    {
        address -= 0xC000;

        if (address >= 0x2000 || address < 0)
        {
            printf("INVALID WRAM ADDRESS: %08X\n", address + 0xC000);
            exit(-1);
        }

        ctx.wram[address] = value;
    }

}

namespace hram
{
    u8 read(u16 address)
    {
        address -= 0xFF80;

        if (address >= 0x80 || address < 0)
        {
            printf("INVALID HRAM ADDRESS: %08X\n", address + 0xFF80);
            exit(-1);
        }

        return ctx.hram[address];
    }

    void write(u16 address, u8 value)
    {
        address -= 0xFF80;

        if (address >= 0x80 || address < 0)
        {
            printf("INVALID WRAM ADDRESS: %08X\n", address + 0xFF80);
            exit(-1);
        }

        ctx.hram[address] = value;
    }

}
