#pragma force_active on
#include "global.h"
typedef struct { u8 pad[12]; u16 *data; } Pal;
typedef struct { u8 pad[0x6a0]; Pal *palette; } Work;
void BagApp_LoadPocketObjPalette(Work *w, int i);
// @fn BagApp_LoadPocketObjPalette
void BagApp_LoadPocketObjPalette(Work *w, int i) {
    if (i <= 7) {
        u16 *p = w->palette->data;
        GXS_LoadOBJPltt(&p[0x80], 0, 0x100);
        GXS_LoadOBJPltt(&p[i * 16], i * 32, 32);
    }
}
