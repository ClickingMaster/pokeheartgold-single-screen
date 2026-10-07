#ifndef OVERLAY_37_RESIDUAL_7_PRIVATE_H
#define OVERLAY_37_RESIDUAL_7_PRIVATE_H

#include "global.h"

#include "overlay_37_work.h"

typedef struct Ov37Tile {
    u8 data[0x18];
} Ov37Tile;



/* module-local rodata blob, left in the assembly */
extern const Ov37Tile ov37_021E7AC8[][8];

/* external game functions */
void CopyWindowToVram(Ov37Window *window);
void BlitBitmapRectToWindow(Ov37Window *window, const void *src, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, u16 destX, u16 destY, u16 destWidth, u16 destHeight);
void Sprite_SetDrawFlag(void *sprite, u32 flag);
u32 sub_02034818(u32 index);
u32 sub_0203769C(void);
u32 sub_02037454(void);
void BufferPlayersName(void *msgFormat, u32 idx, u32 arg2);
s32 GF_SinDeg(u16 deg);
void GX_LoadOBJPltt(const void *src, u32 offset, u32 size);

/* module functions */
void ov37_021E657C(void *sprite, u32 x, u32 y);
void ov37_021E6848(Ov37SpriteHolder *holder, u32 arg1);
void ov37_021E68AC(Ov37Work *work);
void ov37_021E76C0(Ov37Work *work);
void ov37_021E76D0(Ov37Work *work, u32 arg1);
void ov37_021E7844(Ov37Work *work, u32 state);
void ov37_021E78A4(Ov37Work *work);

u32 ov37_021E6F5C(Ov37Work *work, u32 arg1);
void ov37_021E6FC8(Ov37Work *work, int arg1, u32 arg2);
void ov37_021E70BC(Ov37Window *window, const void *src, s32 srcX, s32 srcY, s32 srcWidth, s32 srcHeight, s32 destX, s32 destY, s32 destWidth, s32 destHeight);
void ov37_021E713C(Ov37Window *window, const void *src, s32 x1, s32 y1, s32 *px, s32 *py, s32 arg6, s32 arg7);
void ov37_021E72B4(const Ov37Entry *entries, Ov37Point *points);
void ov37_021E72E8(Ov37Window *window, Ov37Entry *entries, Ov37Point *points, u32 arg3);
void ov37_021E73B4(Ov37Work *work);
void ov37_021E741C(u16 *deg);

#endif
