#include "to55_r2_t01_private.h"
void ov07_0222519C(void *task, void *data) {
    void **w = data;
    *(vu32 *)0x04000000 &= 0xFFFF1FFF;
    BattleAnimBackground_ResetLayer(w[2], 2);
    BattleAnimSystem_DestroyTask(w[2], task);
    Heap_Free(w);
}
