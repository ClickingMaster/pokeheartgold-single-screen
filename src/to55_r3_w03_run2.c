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

void ov07_02232C28(u8 *p) {
    s32 v[3];
    u8 *q = sub_02015504(p);
    ov07_02232BB0(*(int *)(q + 4), v);
    *(s32 *)(p + 0x28) = v[0] + BASE(p)[1];
    *(s32 *)(p + 0x2c) = v[1] + BASE(p)[2];
    *(s32 *)(p + 0x30) = v[2] + BASE(p)[3];
}
