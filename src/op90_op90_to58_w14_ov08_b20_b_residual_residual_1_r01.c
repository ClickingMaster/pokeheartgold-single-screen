#pragma force_active on
#include "global.h"
#include "heap.h"
#include "bg_window.h"
typedef struct { u8 pad[12]; enum HeapID heapId; } Base;
typedef struct { Base *base; u8 pad4[0x1e0]; BgConfig *bg; } Work;
extern const u8 sBattlePartyButtonRects[], ov08_02225A55[], ov08_02225A56[], ov08_02225A57[];
void BattlePartyMenu_BuildButtonTilemap(Work *w, u16 *dst, u8 index, u8 arg, u8 palette);
void BattlePartyMenu_DrawButton(Work *w, u8 index, u8 arg, u8 palette);
// @fn BattlePartyMenu_DrawButton
void BattlePartyMenu_DrawButton(Work *w, u8 index, u8 arg, u8 palette) {
    int offset = index * 4;
    int height = ov08_02225A57[offset];
    int width = ov08_02225A56[offset];
    u16 *buf = Heap_Alloc(w->base->heapId, width * height * 2);
    BattlePartyMenu_BuildButtonTilemap(w, buf, index, arg, palette);
    LoadRectToBgTilemapRect(w->bg, 6, buf, sBattlePartyButtonRects[offset], ov08_02225A55[offset], width, height);
    ScheduleBgTilemapBufferTransfer(w->bg, 6);
    Heap_Free(buf);
}
