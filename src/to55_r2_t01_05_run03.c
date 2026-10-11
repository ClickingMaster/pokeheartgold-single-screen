#include "to55_r2_t01_private.h"
void ov07_02223E94(void *task, void *data) {
    void **w = data;
    if (PaletteData_GetSelectedBuffersBitmask(w[1]) == 0) {
        Heap_Free(w);
        BattleAnimSystem_DestroyTask(w[0], task);
    }
}
