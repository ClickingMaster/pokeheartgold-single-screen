#pragma force_active on
#include "global.h"
#include "heap.h"

typedef struct Ov89Work {
    u8 pad0[0x14];
    u32 bitmapOffset;
} Ov89Work;

void ov89_0225AB64(Ov89Work *work, int dir);

// @fn ov89_0225AB64
void ov89_0225AB64(Ov89Work *work, int dir) {
    u32 offset = work->bitmapOffset;
    u32 *tmp = Heap_Alloc(HEAP_ID_125, 0x40);
    int i, j, shift;
    u32 *src;
    u32 *dst;

    MI_CpuCopy16((u8 *)work + offset, tmp, 0x40);
    MI_CpuClear16((u8 *)work + offset, 0x40);
    switch (dir) {
    case 3:
        src = tmp;
        shift = 30;
        i = 0;
        for (; i < 16; i++, shift -= 2, src++) {
            j = 0;
            dst = (u32 *)((u8 *)work + offset);
            for (; j < 32; j += 2) {
                *dst |= ((*src >> j) & 3) << shift;
                dst++;
            }
        }
        break;
    case 4: {
        u32 *in;
        u32 *out;
        in = tmp;
        for (shift = 0, i = 0; i < 16; i++, shift += 2, in++) {
            j = 0;
            out = (u32 *)((u8 *)work + offset) + 15;
            for (; j < 32; j += 2) {
                *out |= ((*in >> j) & 3) << shift;
                out--;
            }
        }
    }
        break;
    }
    Heap_Free(tmp);
}
