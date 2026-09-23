#include "global.h"

extern int ManagedSprite_GetPaletteOverrideOffset(void *);
extern int PaletteData_BeginPaletteFade(void *, int, u16, s8, u8, u8, u16);
extern int PaletteData_GetSelectedBuffersBitmask(void *);
extern void ov07_02232F74(void *, int);
int ov07_022330FC(u8 *);

int ov07_022330FC(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(*(void **)(state + 0x30));
        PaletteData_BeginPaletteFade(*(void **)(state + 0xb0), 4, (u16)(1 << offset), -1, 0, 12, 0x37f);
        *(int *)(state + 8) += 1;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(*(void **)(state + 0xb0)) == 0) {
            int offset = ManagedSprite_GetPaletteOverrideOffset(*(void **)(state + 0x30));
            PaletteData_BeginPaletteFade(*(void **)(state + 0xb0), 4, (u16)(1 << offset), -1, 12, 0, 0x37f);
            *(int *)(state + 8) += 1;
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(*(void **)(state + 0xb0)) == 0) {
            ov07_02232F74(state, 10);
        }
        break;
    }
    return 1;
}
