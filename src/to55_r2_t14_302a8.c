#include "to55_r2_t14_private.h"

void ov07_022302A8(void *owner, Ov07CallbackWork *work) {
    switch (work->state) {
    case 0:
        ov07_02230170(work);
        ov07_022301C4(work);
        work->state++;
        break;
    case 1:
        work->field28--;
        ov07_022301C4(work);
        if (work->field28 < 0) {
            work->field2C++;
            if (work->field2C < 3) {
                work->state = 0;
            } else {
                work->state++;
            }
        }
        break;
    case 2: {
        u32 bg;
        void *tilemap;

        if (ov07_0221FAC8(work->owner, ov07_0221C470(work->owner)) == 0) {
            Pokepic_SetAttr(work->picture, 6, 0);
        }
        ov07_02222C98(work->resource);
        ov07_02222C60(work->resource);
        bg = ov07_0221FAEC(work->owner, 1);
        SetBgPriority(bg, ov07_0221FB04(work->owner, 1));
        SetBgPriority(0, ov07_0221FAE8(work->owner));
        bg = ov07_0221FAEC(work->owner, 1);
        BG_ClearCharDataRange(bg, 0x4000, 0, ov07_0221BFD0(work->owner));
        tilemap = ov07_0221C4A0(work->owner);
        bg = ov07_0221FAEC(work->owner, 1);
        BgClearTilemapBufferAndCommit(tilemap, bg);
        Heap_Free(work);
        ov07_0221C448(work->owner, owner);
        break;
    }
    }
}
