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
};

class cartridge
{
public:
    bool load(char* cart);

    const char* get_license_name() const;

    const char* get_type_name() const;

private:
    cart_context ctx_ = {};
};
