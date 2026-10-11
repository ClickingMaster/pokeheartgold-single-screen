#include "sol_r9_ov18_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void PokedexApp_DrawListEntryHeader(void *work, u32 value, u32 other) {
    PokedexApp_DrawInfoLabel(work, 7);
    PokedexApp_DrawDexNumber(work, value, 8);
    PokedexApp_DrawSpeciesName(work, value, 9);
    PokedexApp_DrawCategory(work, value, other, 10);
}

void PokedexApp_DrawSearchEntryHeader(void *work, u32 value, u32 other) {
    PokedexApp_DrawInfoLabel(work, 0x51);
    PokedexApp_DrawDexNumber(work, value, 0x52);
    PokedexApp_DrawSpeciesName(work, value, 0x53);
    PokedexApp_DrawCategory(work, value, other, 0x54);
}

void PokedexApp_DrawInfoLabel(void *work, u32 index) {
    u8 *base = (u8 *)work + 0xc;
    u32 offset = index * 0x10;
    FillWindowPixelBuffer(base + offset, 0);
    PokedexApp_PrintMessage(base + offset, PTR(work, 0x65c), 8, 0, 0, 4, 0x20100, 0);
    ScheduleWindowCopyToVram(base + offset);
}
