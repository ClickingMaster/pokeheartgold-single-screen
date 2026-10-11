#include "global.h"

typedef struct { s16 x, y; void *a, *b; int c, d; } EffectInfo;
extern void ManagedSprite_SetAnim(void *, int);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern int ManagedSprite_GetAnimationFrame(void *);
extern void ManagedSprite_SetAnimationFrame(void *, int);
extern void *BattleAnimBattlerSprites_NewAll(void *, void *);
extern void *ov07_02232D20(EffectInfo *);
extern void ov07_02232D80(void *);
extern void BattleAnimBattlerSprites_Free(void *);
extern int ov07_02232DF4(void *);
extern void ov07_02232E10(void *);
extern void ov07_02232F74(void *, int);
int ov07_02232FA8(u8 *);

int ov07_02232FA8(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0: {
        ManagedSprite_SetAnim(*(void **)(state + 0x30), 1);
        EffectInfo info;
        info.a = *(void **)(state + 0xa0);
        info.b = *(void **)(state + 0x94);
        info.c = 255;
        info.d = 0;
        ManagedSprite_GetPositionXY(*(void **)(state + 0x30), &info.x, &info.y);
        *(void **)(state + 0xd8) = BattleAnimBattlerSprites_NewAll(*(void **)(state + 0xb4), *(void **)(state + 0x94));
        *(void **)(state + 0xd0) = ov07_02232D20(&info);
        *(int *)(state + 8) += 1;
        break;
    }
    case 1:
        if (ManagedSprite_GetAnimationFrame(*(void **)(state + 0x30)) >= 2) {
            *(int *)(state + 0x24) = 0;
            *(int *)(state + 8) += 1;
        }
        break;
    case 2:
        ov07_02232D80(*(void **)(state + 0xd0));
        BattleAnimBattlerSprites_Free(*(void **)(state + 0xd8));
        *(int *)(state + 8) += 1;
        break;
    case 3:
        if (ov07_02232DF4(*(void **)(state + 0xd0)) == 0) {
            ManagedSprite_SetAnimationFrame(*(void **)(state + 0x30), 0);
            ov07_02232E10(*(void **)(state + 0xd0));
            *(int *)(state + 8) += 1;
        }
        break;
    default:
        ov07_02232F74(state, 4);
        break;
    }
    return 1;
}
