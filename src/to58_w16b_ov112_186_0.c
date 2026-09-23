#include "global.h"
#include "sprite_system.h"
void ov112_021F22D0(void *, int, int, int);
void ov112_021F22D0(void *s, int row, int spriteIndex, int anim) {
    s16 y, x;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)s + 0x70 + spriteIndex * 4), &y, &x);
    x = *(void **)((u8 *)s + 0x98 + row * 0x14) ? 0x18 : 0x28;
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)s + 0x70 + spriteIndex * 4), y, x);
    ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)s + 0x70 + spriteIndex * 4), anim);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)s + 0x70 + spriteIndex * 4), 1);
}
