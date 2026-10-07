#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct {
    void *x0;
    void *x4;
    u8 x8[4];
    u8 xc[4];
    u8 x10[4][3];
    u8 count;
    u8 x1d;
    u8 x1e;
    u8 x1f;
    u8 start;
    u8 x21;
    u8 pad[2];
} Obj;
u8 *ov96_021E60D8(void *course, int a, int b);
Obj *ov96_021F7684(enum HeapID heapId, u8 count, int unused, void *x4, void *course);
// @fn ov96_021F7684
Obj *ov96_021F7684(enum HeapID heapId, u8 count, int unused, void *x4, void *course) {
    int i;
    int j;
    Obj *obj = Heap_Alloc(heapId, sizeof(Obj));
    MI_CpuFill8(obj, 0, sizeof(Obj));
    obj->count = count;
    obj->x21 = 4 - count;
    obj->start = obj->x21;
    obj->x1f = 3;
    obj->x4 = x4;
    for (i = 0; i < obj->count; i++) {
        u8 idx = obj->start + i;
        for (j = 0; j < 3; j++) {
            u8 *info = ov96_021E60D8(course, idx, j);
            obj->x8[idx] += info[2];
            obj->xc[idx] += info[4];
            obj->x10[idx][j] = obj->x8[idx];
        }
    }
    return obj;
}
