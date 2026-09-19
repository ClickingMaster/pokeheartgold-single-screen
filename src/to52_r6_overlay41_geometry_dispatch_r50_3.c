#include "to52_r6_overlay41_geometry_dispatch_r50_3_private.h"

#define OBJECT(p) (*(void **)(p))
#define MODE(p)   (*(s32 *)((u8 *)(p) + 4))

BOOL ov41_02249AA8(void *dispatch, s32 x, s32 y, void **table) {
    if (MODE(dispatch) == 0) {
        return ov41_02245F14(OBJECT(dispatch), x, y, table[*(u32 *)OBJECT(dispatch)]);
    }
    if (MODE(dispatch) == 1) {
        void *object = OBJECT(dispatch);
        u32 index = *(u32 *)object;
        index += 100;
        return ov41_02245F14(object, x, y, table[index]);
    }
    if (MODE(dispatch) == 2) {
        void *object = OBJECT(dispatch);
        u32 index = *(u32 *)object;
        index += 100;
        return ov41_02245F14(object, x, y, table[index]);
    }
    return ov41_022497A8(OBJECT(dispatch), x, y, table[0x76]);
}

void ov41_02249AF4(void *dispatch, s32 x, s32 y) {
    s32 width;
    s32 height;

    if (MODE(dispatch) < 3) {
        ov41_02245F9C(OBJECT(dispatch), (s16)x, (s16)y);
        return;
    }
    void *object = OBJECT(dispatch);
    ov41_022497A0(object, &width, &height);
    width /= 2;
    height /= 2;
    ov41_0224971C(object, x + width, y + height);
}

void ov41_02249B44(void *dispatch, s32 *x, s32 *y) {
    s32 width;
    s32 height;

    if (MODE(dispatch) < 3) {
        ov41_02245FA8(OBJECT(dispatch), x, y);
        return;
    }
    void *object = OBJECT(dispatch);
    ov41_02249780(object);
    ov41_022497A0(object, &width, &height);
    width /= 2;
    height /= 2;
    *x -= width;
    *y -= height;
}
