#include "global.h"
int ov07_0221FAE8(void *, int);
void BG_LoadCharTilesData(void *, int, void *, int, int);
void PaletteData_LoadPalette(void *, void *, int, int, int);
int ov07_0221FB04(void *, unsigned);
void ov07_0221FB30(u8 *, int);
void ov07_0221FB58(u8 *);

void ov07_0221FB30(u8 *state, int layer) {
    u8 *data = *(u8 **)(state + 0xC0);
    BG_LoadCharTilesData(*(void **)(state + 0xC4), (u8)layer, *(void **)(data + 0x110), 0x10000, 0);
}

void ov07_0221FB58(u8 *state) {
    u8 *data = *(u8 **)(state + 0xC0);
    PaletteData_LoadPalette(*(void **)(state + 0xC8), *(void **)(data + 0x114), 0, 0, 0x200);
}
