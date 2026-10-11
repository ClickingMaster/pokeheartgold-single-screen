#pragma force_active on
#include "global.h"
#include "heap.h"
#include "bg_window.h"
typedef struct { u8 pad[12]; enum HeapID heapId; } Base;
typedef struct { Base *base; BgConfig *bg; } Work;
extern const u8 sBattleBagButtonRects[], ov08_02225E9D[], ov08_02225E9E[], ov08_02225E9F[];
void BattleBagMenu_BuildButtonTilemap(Work *w, u16 *dst, u8 index, u8 arg, u8 palette);
void BattleBagMenu_DrawButton(Work *w, u8 index, u8 arg, u8 palette);
extern u16 *BattleBagMenu_GetButtonTilemap(Work *, int, int);
extern u32 BattleBagMenu_GetButtonPalette(Work *, int, int, u8);
extern void BattleBagMenu_CopyPocketIconTiles(Work *, void *, int, int);
// @fn BattleBagMenu_DrawButton
void BattleBagMenu_DrawButton(Work *w, u8 index, u8 arg, u8 palette) {
    int offset = index * 4;
    int height = ov08_02225E9F[offset];
    int width = ov08_02225E9E[offset];
    u16 *buf = Heap_Alloc(w->base->heapId, width * height * 2);
    BattleBagMenu_BuildButtonTilemap(w, buf, index, arg, palette);
    LoadRectToBgTilemapRect(w->bg, 6, buf, sBattleBagButtonRects[offset], ov08_02225E9D[offset], width, height);
    ScheduleBgTilemapBufferTransfer(w->bg, 6);
    Heap_Free(buf);
}
