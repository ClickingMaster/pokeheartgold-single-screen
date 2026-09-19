#include "private.h"

extern void GF_AssertFail(void);
extern const Ov96SpriteTemplate ov96_0221D944;
extern void *SpriteSystem_NewSpriteWithYOffset(void *, void *, const void *, s32);
extern void ManagedSprite_SetAnimateFlag(void *, u32);
extern void ManagedSprite_SetPaletteOverride(void *, u32);
extern void *SpriteManager_GetSpriteList(void *);
extern void *ov96_021EA374(void *, void *, u32, u32);
extern void Sprite_SetMatrix(void *, const void *);
extern void Sprite_SetDrawFlag(void *, u32);
extern void BufferString(void *, u32, void *, u32, u32, u32);
extern void ov96_02219BC4(void *, u32, u32, u32, u32);

void *ov96_02219E00(void *sys, void *list, u16 a, u16 b, u16 c, u16 d) {
    Ov96SpriteTemplate t = ov96_0221D944;
    void *sprite;
    if (sys == NULL) GF_AssertFail();
    if (list == NULL) GF_AssertFail();
    *(u16 *)((u8 *)t.raw + 0) = a;
    *(u16 *)((u8 *)t.raw + 2) = b;
    *(u16 *)((u8 *)t.raw + 6) = c;
    t.raw[2] = d;
    sprite = SpriteSystem_NewSpriteWithYOffset(sys, list, &t, 0x1e0000);
    ManagedSprite_SetAnimateFlag(sprite, 1);
    return sprite;
}

void ov96_02219E60(void *raw) {
    u8 *p = raw;
    s32 i;
    u8 *slot;
    s32 x;
    void *sprite;
    sprite = ov96_02219E00(PAT(p, 8), PAT(p, 0xc), 0x72, 0xb4, 0x14, 1);
    PAT(p, 0x64) = sprite;
    ManagedSprite_SetPaletteOverride(sprite, 0xe);
    i = 0;
    slot = p;
    x = 0x86;
    for (; i < 2; i++) {
        s32 matrix[3] = { 0, 0, 0 };
        s32 scaled;
        sprite = ov96_021EA374(PAT(p, 0x10), SpriteManager_GetSpriteList(PAT(p, 0xc)), 1, U32AT(p, 4));
        PAT(slot, 0xa8) = sprite;
        scaled = x;
        scaled <<= 12;
        matrix[0] = scaled;
        matrix[1] = 0x2d << 14;
        matrix[2] = 0;
        Sprite_SetMatrix(PAT(slot, 0xa8), matrix);
        Sprite_SetDrawFlag(PAT(slot, 0xa8), 1);
        slot += 4;
        x += 0x10;
    }
}

void ov96_02219EE0(void *p, s32 value) {
    s32 index;
    index = (value + 1) % 3;
    index = (u8)index;
    BufferString(PAT(p, 0x1c), 0, PAT(p, 0xb0 + index * 4), 2, 1, 2);
    ov96_02219BC4((u8 *)p + 0x40, (u32)PAT(p, 0x1c), (u32)PAT(p, 0x18), 0x9a, U32AT(p, 4));
}
