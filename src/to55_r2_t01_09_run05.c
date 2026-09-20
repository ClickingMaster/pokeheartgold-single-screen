#include "to55_r2_t01_private.h"
void ov07_0222519C(void *task, void *data) {
    void **w = data;
    *(vu32 *)0x04000000 &= 0xFFFF1FFF;
    ov07_0221EC7C(w[2], 2);
    ov07_0221C448(w[2], task);
    Heap_Free(w);
}
