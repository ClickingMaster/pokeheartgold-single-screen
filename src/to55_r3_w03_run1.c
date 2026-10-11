#include "to55_r3_w03_private.h"
#define BASE(p) (*(s32 **)(*(void **)((p) + 0x20)))

typedef struct Args {
    u32 a, b, c, d, e;
} Args;
typedef struct Out {
    Args args;
    u32 pad14;
    void *obj;
    s32 count;
    s32 resource;
} Out;

void ov07_02232AC0(void *task, u8 *p) {
    if (*(int *)(p + 4) == 0) {
        **(int **)(p + 0xc) = 1;
        ParticleSystem_CreateEmitterWithParam(*(void **)(p + 0x10), 0, *(void **)(p + 0x18), p);
        ParticleSystem_SetPerspectiveType(*(void **)(p + 0x10), 1);
        SysTask_Destroy(task);
        Heap_Free(p);
    } else {
        (*(int *)(p + 4))--;
    }
}
