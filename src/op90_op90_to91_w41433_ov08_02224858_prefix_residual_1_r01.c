#pragma force_active on
#include "global.h"
#include "heap.h"
#include "bg_window.h"
typedef struct { u8 pad[12]; enum HeapID heapId; } Base;
typedef struct { Base *base; BgConfig *bg; } Work;
typedef struct { u8 x, y, w, h; } Rect;
extern const Rect ov08_02225E9C[];
extern const u8 ov08_02225E9E[], ov08_02225E9F[];
void ov08_02224768(Work *w, u16 *dst, u8 index, u8 arg, u8 palette);
extern u16 *ov08_022245DC(Work *, int, int);
extern u32 ov08_02224684(Work *, int, int, u8);
extern void ov08_022246F4(Work *, void *, int, int);
// @fn ov08_02224768
void ov08_02224768(Work *w, u16 *dst, u8 index, u8 arg, u8 palette) {
    u16 *src = ov08_022245DC(w, index, arg);
    u16 color = ov08_02224684(w, index, arg, palette) << 12;
    u16 i;
    for (i = 0; i < ov08_02225E9C[index].w * ov08_02225E9C[index].h; i++) {
        dst[i] = (src[i] & 0xFFF) | color;
    }
    ov08_022246F4(w, dst, index, arg);
}
