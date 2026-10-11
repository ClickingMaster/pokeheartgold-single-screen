#pragma force_active on
#include "global.h"
#include "heap.h"
#include "scanline_effect.h"
#include <string.h>
typedef struct { u8 pad[28]; ScanlineEffect *scanline; } Work;
extern void *ov07_02222D90(int);
extern void ov07_02222CAC(void *, void *);
extern void ov07_02222CC0(void *, void *);
extern void ov07_02222B30(Work *, Work *, void (*)(void *, void *), void (*)(void *, void *));
Work *ov07_02222CCC(u8 first, u8 last, u8 step, fx32 amplitude, s16 speed, int mode, u32 priority, u32 fill, enum HeapID heap);

// @fn ov07_02222CCC
Work *ov07_02222CCC(u8 first, u8 last, u8 step, fx32 amplitude, s16 speed, int mode, u32 priority, u32 fill, enum HeapID heap) {
    Work *w = Heap_Alloc(heap, sizeof(Work));
    void *dest;
    GF_ASSERT(w != NULL);
    memset(w, 0, sizeof(Work));
    dest = ov07_02222D90(mode);
    w->scanline = ScanlineEffect_New(heap);
    ScanlineEffect_StartWave(w->scanline, first, last, step, amplitude, speed, dest, fill, priority);
    ov07_02222B30(w, w, ov07_02222CAC, ov07_02222CC0);
    return w;
}
