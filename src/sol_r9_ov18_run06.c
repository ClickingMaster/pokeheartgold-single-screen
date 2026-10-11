#include "sol_r9_ov18_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define DRAW_FIXED(name, message, y_pos)                                                  \
    void name(void *work, u32 index) {                                                    \
        u8 *base = (u8 *)work + 0xc;                                                      \
        u32 offset = index * 0x10;                                                        \
        FillWindowPixelBuffer(base + offset, 0);                                          \
        PokedexApp_PrintMessage(base + offset, PTR(work, 0x65c), message, y_pos, 0, 0, 0x20100, 2); \
    }

DRAW_FIXED(PokedexApp_DrawSearchOrderLabel, 0x1a, 0x24)
DRAW_FIXED(PokedexApp_DrawSearchNameLabel, 0x1b, 0x14)
DRAW_FIXED(PokedexApp_DrawSearchTypeLabel, 0x1c, 0x14)
DRAW_FIXED(PokedexApp_DrawSearchHeightLabel, 0x1d, 0x14)
DRAW_FIXED(PokedexApp_DrawSearchWeightLabel, 0x1e, 0x14)
DRAW_FIXED(PokedexApp_DrawSearchAreaLabel, 0x1f, 0x14)
DRAW_FIXED(PokedexApp_DrawSearchFormLabel, 0x20, 0x18)
