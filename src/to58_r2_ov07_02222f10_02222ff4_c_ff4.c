#include "global.h"
void *ov07_0221FA78(void *);
void *PaletteData_GetFadedBuf(void *, int);
int ov07_0221BFC0(void *);
void ov07_02222F7C(void *, int);
void PaletteData_CopyPalette(void *, int, int, int, int, int);
void ov07_02222FC4(void *);
void ov07_02222FF4(void *);

void ov07_02222FF4(void *state) {
    void *data = ov07_0221FA78(state);
    if (ov07_0221BFC0(state) == 1) {
        PaletteData_CopyPalette(data, 0, 0, 0, 0, 0x60);
        return;
    }
    PaletteData_CopyPalette(data, 0, 0, 0, 0, 0x100);
}
