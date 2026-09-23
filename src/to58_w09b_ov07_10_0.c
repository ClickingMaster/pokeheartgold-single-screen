#include "global.h"

typedef struct { s16 x, y; void *a, *b, *c; int d; } EffectInfo;
extern int ManagedSprite_GetPaletteOverrideOffset(void *);
extern int PaletteData_BeginPaletteFade(void *, int, u16, s8, u8, u8, u16);
extern int PaletteData_GetSelectedBuffersBitmask(void *);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void ManagedSprite_SetAnimationFrame(void *, int);
extern void *ov07_02232644(void *, void *);
extern void *ov07_02232D20(EffectInfo *);
extern void ov07_02232D80(void *);
extern int ov07_02232DF4(void *);
extern void ov07_02232E10(void *);
extern void ov07_02232F74(void *, int);
int ov07_022333A8(u8 *);

int ov07_022333A8(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(*(void **)(state + 0x30));
        PaletteData_BeginPaletteFade(*(void **)(state + 0xb0), 4, (u16)(1 << offset), -5, 0, 10, 0);
        *(int *)(state + 8) += 1;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(*(void **)(state + 0xb0)) == 0) {
            EffectInfo info;
            info.a = *(void **)(state + 0xa0);
            info.b = *(void **)(state + 0x94);
            info.c = ov07_02232644(info.a, info.b);
            info.d = 0;
            ManagedSprite_GetPositionXY(*(void **)(state + 0x30), &info.x, &info.y);
            *(void **)(state + 0xd0) = ov07_02232D20(&info);
            *(int *)(state + 8) += 1;
        }
        break;
    case 2:
        ov07_02232D80(*(void **)(state + 0xd0));
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
        if (PaletteData_GetSelectedBuffersBitmask(*(void **)(state + 0xb0)) == 0) {
            ov07_02232F74(state, 22);
        }
        break;
    }
    return 1;
}
