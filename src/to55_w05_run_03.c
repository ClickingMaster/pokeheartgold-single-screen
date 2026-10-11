#include "to55_w05_private.h"
#define W(p, o)  (*(u32 *)((u8 *)(p) + (o)))
#define SW(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define H(p, o)  (*(s16 *)((u8 *)(p) + (o)))
#define P(p, o)  (*(void **)((u8 *)(p) + (o)))

void ov07_0222F9B8(void *p) {
    BattleAnimUtil_InitScaleMotion((u8 *)p + 0xf8, 0x19, 10, 10, 10);
    BattleAnimUtil_InitSpriteSequence((u8 *)p + 0x48, (u8 *)p + 0xf8, BattleAnimUtil_UpdateScaleMotion, H(p, 0x38), H(p, 0x3a), 9, 3, 1, W(p, 0x44), W(p, 0x40), W(p, 0x3c), 0);
}

void ov07_0222FA08(void *p) {
    BattleAnimUtil_InitScaleMotion((u8 *)p + 0xf8, 10, 10, 0x19, 6);
    BattleAnimUtil_InitSpriteSequence((u8 *)p + 0x48, (u8 *)p + 0xf8, BattleAnimUtil_UpdateScaleMotion, H(p, 0x38), H(p, 0x3a), 5, 3, 1, W(p, 0x3c), W(p, 0x40), W(p, 0x44), 0);
    ManagedSprite_SetDrawFlag(P(p, 0x3c), 1);
}
