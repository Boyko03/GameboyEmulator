#include "bus.h"

#include "cartridge.h"
#include "cpu.h"
#include "ram.h"

// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

u8 bus::read(const u16 address)
{
    if (address < 0x8000)
    {
        // ROM Data
        return cart::read(address);
    }
    if (address < 0xA000)
    {
        // Char/Map Data
        printf("UNSUPPORTED bus_read(%04X)\n", address);
        NO_IMPL
    }
    if (address < 0xC000)
    {
        // Cartridge RAM
        return cart::read(address);
    }
    if (address < 0xE000)
    {
        // WRAM (Working RAM)
        return wram::read(address);
    }
    if (address < 0xFE00)
    {
        // reserved echo ram
        return 0;
    }
    if (address < 0xFEA0)
    {
        // OAM
        // TODO
        printf("UNSUPPORTED bus_read(%04X)\n", address);
        NO_IMPL
    }
    if (address < 0xFF00)
    {
        // reserved unusable
        return 0;
    }
    if (address < 0xFF80)
    {
        // IO Registers
        // TODO
        printf("UNSUPPORTED bus_read(%04X)\n", address);
        //NO_IMPL
    }
    if (address == 0xFFFF)
    {
        // CPU Enable Register
        return cpu::get_ie_register();
    }

    return hram::read(address);
}

u16 bus::read16(const u16 address)
{
    const u16 lo = read(address);
    const u16 hi = read(address + 1);

    return lo | (hi << 8);
}

void bus::write(const u16 address, const u8 value)
{
    if (address < 0x8000)
    {
        cart::write(address, value);
    }
    else if (address < 0xA000)
    {
        // Char/Map Data
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        NO_IMPL
    }
    else if (address < 0xC000)
    {
        // EXT-RAM
        cart::write(address, value);
    }
    else if (address < 0xE000)
    {
        // WRAM (Working RAM)
        wram::write(address, value);
    }
    else if (address < 0xFE00)
    {
        // reserved echo ram
    }
    else if (address < 0xFEA0)
    {
        // OAM
        // TODO
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        NO_IMPL
    }
    else if (address < 0xFF00)
    {
        // reserved unusable
    }
    else if (address < 0xFF80)
    {
        // IO Registers
        // TODO
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        //NO_IMPL
    }
    else if (address == 0xFFFF)
    {
        // CPU Enable Register
        cpu::set_ie_register(value);
    }
    else
    {
        hram::write(address, value);
    }
}

void bus::write16(const u16 address, const u16 value)
{
    write(address + 1, (value >> 8) & 0xFF);
    write(address, value & 0xFF);
}
