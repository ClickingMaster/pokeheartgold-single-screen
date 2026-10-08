#pragma force_active on
#include "global.h"
#include "heap.h"
#include "bg_window.h"
typedef struct { u8 pad[12]; enum HeapID heapId; } Base;
typedef struct { Base *base; BgConfig *bg; } Work;
extern const u8 ov08_02225E9C[], ov08_02225E9D[], ov08_02225E9E[], ov08_02225E9F[];
void ov08_02224768(Work *w, u16 *dst, u8 index, u8 arg, u8 palette);
void ov08_022247E4(Work *w, u8 index, u8 arg, u8 palette);
extern u16 *ov08_022245DC(Work *, int, int);
extern u32 ov08_02224684(Work *, int, int, u8);
extern void ov08_022246F4(Work *, void *, int, int);
// @fn ov08_022247E4
void ov08_022247E4(Work *w, u8 index, u8 arg, u8 palette) {
    int offset = index * 4;
    int height = ov08_02225E9F[offset];
    int width = ov08_02225E9E[offset];
    u16 *buf = Heap_Alloc(w->base->heapId, width * height * 2);
    ov08_02224768(w, buf, index, arg, palette);
    LoadRectToBgTilemapRect(w->bg, 6, buf, ov08_02225E9C[offset], ov08_02225E9D[offset], width, height);
    ScheduleBgTilemapBufferTransfer(w->bg, 6);
    Heap_Free(buf);
}
