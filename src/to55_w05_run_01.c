#include "to55_w05_private.h"
#define W(p, o)  (*(u32 *)((u8 *)(p) + (o)))
#define SW(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define H(p, o)  (*(s16 *)((u8 *)(p) + (o)))
#define P(p, o)  (*(void **)((u8 *)(p) + (o)))

void ov07_0222F434(void *p) {
    float x, y;
    BattleAnimUtil_InitScaleMotionXY((u8 *)p + 0x28, 10, 10, 10, 20, 10, 8);
    BattleAnimUtil_InitScaleMotion((u8 *)p + 0x4c, 10, 10, 1, 4);
    W(p, 0x94) = 0;
    W(p, 0x98) = 6;
    ManagedSprite_SetDrawFlag(P(p, 0x20), ov07_02236800[BattleAnimUtil_GetBattlerType(P(p, 0), BattleAnimSystem_GetAttacker(P(p, 0)))]);
    ManagedSprite_SetDrawFlag(P(p, 0x18), 1);
    BattleAnimUtil_GetScaleFloat((u8 *)p + 0x28, &x, &y);
    if (W(p, 0xc8) == 1) {
        x = 0.0f - x;
    }
    ManagedSprite_SetAffineScale(P(p, 0x18), x, y);
    ov07_0222F408(p, P(p, 0x18));
}
