#include "overlay18_pokedex_info_windows_internal.h"

void PokedexApp_FreeAreaWindows(void *p) {
    u32 i;
    u8 *window = p;
    i = 0;
    window += 0xC;
    for (; i < 14; i++) {
        ClearWindowTilemapAndScheduleTransfer((Window *)window);
        window += 0x10;
    }
    PokedexApp_RemoveAllWindows(p);
}

void PokedexApp_DrawTimeOfDayLabel(void *p) {
    FillWindowPixelBuffer((Window *)((u8 *)p + 0x1C), 0);
    PokedexApp_PrintMessage((u8 *)p + 0x1C, PTR(p, 0x65C), (s8) * ((u8 *)p + 0x18C9) + 0x81, 0x1C, 0, 0, 0x20100, 2);
    p = (u8 *)p + 0x1C;
    ScheduleWindowCopyToVram(p);
}
