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

void ov07_02232CD8(void *arg) {
    u8 *p = arg;
    s16 *q = sub_02015504(p);
    s32 dy = (s16)(100 - q[1]) * 172;
    s32 dx = (s16)(q[0] - 129) * 172;
    *(s32 *)(p + 0x28) = dx + BASE(p)[1];
    *(s32 *)(p + 0x2c) = dy + BASE(p)[2];
    *(s32 *)(p + 0x30) = BASE(p)[3];
}

void *ov07_02232D20(Args *a);
void *ov07_02232D20(Args *a) {
    Out *p = Heap_Alloc(a->c, 0x24);
    if (!p) {
        GF_AssertFail();
    }
    p->args = *a;
    if (p->args.e) {
        p->resource = ov07_0223261C(0x403);
        p->count = ov07_02232630(0x403);
    } else {
        p->resource = ov07_0223261C(p->args.b);
        p->count = ov07_02232630(p->args.b);
    }
    p->obj = ov07_0221FEB0(p->args.c, 0x5f, p->resource, 0);
    return p;
}

void ov07_02232D80(u8 *p) {
    int i;
    u32 sel = *(u32 *)(p + 0xc);
    if (sel == 0xff) {
        if (*(u32 *)(p + 0x10)) {
            for (i = 0; i < *(int *)(p + 0x1c); i++) {
                sub_02015494(*(void **)(p + 0x18), i, ov07_02232CD8, p);
            }
        } else {
            for (i = 0; i < *(int *)(p + 0x1c); i++) {
                if (i != ov07_02232644(*(int *)(p + 4))) {
                    sub_02015494(*(void **)(p + 0x18), i, ov07_02232CD8, p);
                }
            }
        }
    } else {
        sub_02015494(*(void **)(p + 0x18), sel, ov07_02232CD8, p);
    }
    sub_02015528(*(void **)(p + 0x18), 1);
}
