#pragma once
#include "common.h"

namespace wram
{
    u8 read(u16 address);
    void write(u16 address, u8 value);
}

namespace hram
{
    u8 read(u16 address);
    void write(u16 address, u8 value);
}
