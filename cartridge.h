#pragma once
#include "common.h"

struct rom_header
{
    u8 entry[4];
    u8 logo[0x30];

    char title[16];
    u16 new_lic_code;
    u8 sgb_flag;
    u8 type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 lic_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
};

struct cart_context
{
    char filename[1024];
    u32 rom_size;
    u8* rom_data;
    rom_header* header;

    ~cart_context()
    {
        free(rom_data);
    }
};

namespace cart
{
    bool load(const char* cart);

    u8 read(u16 address);
    void write(u16 address, u8 value);

    const char* get_license_name();

    const char* get_type_name();
};
