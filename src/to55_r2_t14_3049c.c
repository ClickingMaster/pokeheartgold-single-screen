#include "to55_r2_t14_private.h"

void ov07_0223049C(void *owner, Ov07FadeWork *work) {
    void *palette;

    switch (work->state) {
    case 0:
        ov07_02222268(&work->transition, 0x7F, 0, 0, 0, 8);
        *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & 0xFFFF1FFF) | 0x2000;
        BattleAnimUtil_SetWindowInsidePlanes(work->owner, 0, 0);
        *(vu16 *)0x04000040 = 0xFF;
        *(vu16 *)0x04000044 = 0xBF;
        work->timer = 7;
        work->state++;
        break;
    case 1:
        if (--work->timer < 0) {
            palette = BattleAnimSystem_GetPaletteData(work->owner);
            PaletteData_BeginPaletteFade(palette, 1, BattleAnimSystem_GetBattleBgPaletteMask(work->owner), 0, 0, 0x10, 0xFFFF);
            work->state++;
        }
        break;
    case 2:
        if (ov07_022222B4(&work->transition)) {
            s16 value = work->transition;
            *(vu16 *)0x04000040 = ((0x7F - value) << 8 & 0xFF00) | (u8)(value + 0x80);
            *(vu16 *)0x04000044 = 0xBF;
        } else {
            work->state++;
        }
        break;
    case 3:
        if (PaletteData_GetSelectedBuffersBitmask(BattleAnimSystem_GetPaletteData(work->owner)) == 0) {
            work->state++;
            *(vu32 *)0x04000000 &= 0xFFFF1FFF;
            PaletteData_BeginPaletteFade(BattleAnimSystem_GetPaletteData(work->owner), 1, BattleAnimSystem_GetBattleBgPaletteMask(work->owner), 0, 0x10, 0, 0xFFFF);
        }
        break;
    case 4:
        palette = BattleAnimSystem_GetPaletteData(work->owner);
        if (PaletteData_GetSelectedBuffersBitmask(palette) == 0) {
            work->state++;
        }
        break;
    case 5:
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->owner, owner);
        break;
    }
}
