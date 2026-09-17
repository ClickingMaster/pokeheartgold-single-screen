#include "global.h"

#include "sprite_system.h"

BOOL ov18_021F2AF8(void *p, u32 x, u32 y);

BOOL ov18_021F2AF8(void *p, u32 x, u32 y) {
    s16 sx;
    s16 sy;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)p + 0x670), &sx, &sy);
    if (x >= sx - 16 && x < sx + 16 && y >= sy - 16 && y < sy + 16) {
        return TRUE;
    }
    return FALSE;
}
