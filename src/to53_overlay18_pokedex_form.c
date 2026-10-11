#include "global.h"

#include "bg_window.h"

typedef struct W09Pokedex {
    u8 pad0000[8];
    void *grid;
    Window windows[101];
    void *msgData;
    u8 pad0660[0x18CA - 0x660];
    s8 current;
    u8 pad18CB[0x1900 - 0x18CB];
    int count;
} W09Pokedex;

extern void BgCellManager_GetCellPosition(void *, int, s8 *, s8 *);
extern void BgCellManager_DrawCellAt(void *, int, int, int);
extern u32 PokedexApp_GetAreaListEntryMsg(void *, int);
extern void PokedexApp_PrintFormattedMessage(void *, void *, u32, u32, s32, u32, u32, u32, u32);
void PokedexApp_ScrollAreaList(W09Pokedex *p, int direction);

void PokedexApp_ScrollAreaList(W09Pokedex *p, int direction) {
    u32 i = 0;
    s8 second;
    s8 first;
    W09Pokedex *work = p;

    do {
        BgCellManager_GetCellPosition(work->grid, i + 0x11, &second, &first);
        if (first == 2 || first == 0x14) {
            break;
        }
        i++;
    } while (i < 6);

    FillWindowPixelBuffer(&work->windows[i + 8], 0);
    if (direction < 0) {
        int next;
        BgCellManager_DrawCellAt(work->grid, i + 0x11, 10, 0x14);
        next = work->current + 2;
        if (next < work->count) {
            PokedexApp_PrintFormattedMessage(work, work->msgData, i + 8, PokedexApp_GetAreaListEntryMsg(work, next), 0x48, 0, 4, 0xF0C00, 2);
        }
    } else {
        int previous;
        BgCellManager_DrawCellAt(work->grid, i + 0x11, 10, 2);
        previous = work->current;
        previous -= 2;
        if (previous >= 0) {
            PokedexApp_PrintFormattedMessage(work, work->msgData, i + 8, PokedexApp_GetAreaListEntryMsg(work, previous), 0x48, 0, 4, 0xF0C00, 2);
        }
    }
    CopyWindowPixelsToVram_TextMode(&work->windows[i + 8]);
}
