#include "global.h"

extern int ManagedSprite_GetPaletteOverrideOffset(void *);
extern int PaletteData_BeginPaletteFade(void *, int, u16, s8, u8, u8, u16);
extern int PaletteData_GetSelectedBuffersBitmask(void *);
extern void ov07_02232F74(void *, int);
int ov07_0223349C(u8 *);

int ov07_0223349C(u8 *state) {
    switch (*(int *)(state + 8)) {
    case 0: {
        int offset = ManagedSprite_GetPaletteOverrideOffset(*(void **)(state + 0x30));
        PaletteData_BeginPaletteFade(*(void **)(state + 0xb0), 4, (u16)(1 << offset), -5, 10, 0, 0);
        *(int *)(state + 8) += 1;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(*(void **)(state + 0xb0)) == 0) {
            *(int *)(state + 8) += 1;
        }
        break;
    default:
        ov07_02232F74(state, 24);
        break;
    }
    return 1;
}
