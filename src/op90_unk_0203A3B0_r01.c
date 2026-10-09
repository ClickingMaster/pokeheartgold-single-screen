#include "wireless_status_icon_internal.h"
#include <nitro/gx/gx.h>
#include <nitro/gx/gx_vramcnt.h>
#include <nitro/gx/g2_oam.h>
#pragma force_active on
#pragma push
#pragma opt_propagation off
// @fn sub_0203A700
void sub_0203A700(UnkStruct_0203A3B0 *p)
{
    int bank;
    s32 tile;
    s32 screen;
    s32 index;
    volatile UnkOamRegs * oam;
    s32 mode;
    u32 pal;
    volatile UnkOamRegs * old;
    switch (p->unk11)
    {
        case 1:
        if (((*(volatile u16 *)0x4000304 & 0x8000) >> 15) == 1)
        {
            screen = 1;
        }
        else
        {
            screen = 2;
        }
        break;
        case 2:
        if (((*(volatile u16 *)0x4000304 & 0x8000) >> 15) == 1)
        {
            screen = 2;
        }
        else
        {
            screen = 1;
        }
        break;
        default:
        screen = p->unk21;
        break;
    }
    index = p->unk04;
    if (screen == 1)
    {
        mode = GX_GetOBJVRamModeChar();
        bank = GX_GetBankForOBJ();
    }
    else
    {
        mode = GXS_GetOBJVRamModeChar();
        bank = GX_GetBankForSubOBJ();
    }
    switch (mode)
    {
        case 0x10:
        if (bank == 64 || bank == 32)
        {
            tile = 4 * index + 496;
        }
        else
        {
            tile = 4 * index + 1008;
        }
        break;
        case 0x100010:
        if (bank == 64 || bank == 32 || bank == 256)
        {
            tile = 2 * index + 504;
        }
        else
        {
            goto fallback;
        }
        break;
        case 0x200010:
        if (bank == 48 || bank == 80)
        {
            tile = index + 636;
        }
        else
        {
            if (bank == 16)
            {
                tile = index + 508;
            }
            else
            {
                tile = index + 1020;
            }
        }
        break;
        case 0x300010:
        if (bank == 3)
        {
            tile = 1020 + index;
        }
        else
        {
            tile = index + 508;
        }
        break;
        default:
fallback:
        tile = 2 * index + 1016;
        break;
    }
    if (screen == 1)
    {
        oam = (volatile UnkOamRegs *)0x7000000;
    }
    else
    {
        oam = (volatile UnkOamRegs *)0x7000400;
    }
    pal = p->unk20;
    {
        u8 prio = p->unk22;
        oam->unk00 = ((p->unk0C & 511) << 16) | (0x40000000 | ((p->unk0E & 255) | (prio << 10)));
    }
    oam->unk04 = (pal << 12) | tile;
    old = p->unk1C;
    if (oam != old)
    {
        old->unk00 = 0x40000200 | (p->unk22 << 10);
        old->unk04 = 0;
        p->unk1C = (UnkOamRegs *)oam;
    }
}




#pragma pop
