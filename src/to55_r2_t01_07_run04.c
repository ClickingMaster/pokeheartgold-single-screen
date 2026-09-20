#include "to55_r2_t01_private.h"
void ov07_022247C8(void *task, void *data) {
    void **w = data;
    if (ov07_02222AF4((u8 *)w + 8)) {
        Heap_Free(w);
        ov07_0221C448(w[0], task);
    } else {
        SpriteSystem_DrawSprites(w[1]);
    }
}
