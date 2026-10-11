#pragma force_active on
#include "global.h"
#include "heap.h"
#include "bg_window.h"
typedef struct { u8 pad[12]; enum HeapID heapId; } Base;
typedef struct { Base *base; BgConfig *bg; } Work;
typedef struct { u8 x, y, w, h; } Rect;
extern const Rect sBattleBagButtonRects[];
extern const u8 ov08_02225E9E[], ov08_02225E9F[];
void BattleBagMenu_BuildButtonTilemap(Work *w, u16 *dst, u8 index, u8 arg, u8 palette);
extern u16 *BattleBagMenu_GetButtonTilemap(Work *, int, int);
extern u32 BattleBagMenu_GetButtonPalette(Work *, int, int, u8);
extern void BattleBagMenu_CopyPocketIconTiles(Work *, void *, int, int);
// @fn BattleBagMenu_BuildButtonTilemap
void BattleBagMenu_BuildButtonTilemap(Work *w, u16 *dst, u8 index, u8 arg, u8 palette) {
    u16 *src = BattleBagMenu_GetButtonTilemap(w, index, arg);
    u16 color = BattleBagMenu_GetButtonPalette(w, index, arg, palette) << 12;
    u16 i;
    for (i = 0; i < sBattleBagButtonRects[index].w * sBattleBagButtonRects[index].h; i++) {
        dst[i] = (src[i] & 0xFFF) | color;
    }
    BattleBagMenu_CopyPocketIconTiles(w, dst, index, arg);
}
