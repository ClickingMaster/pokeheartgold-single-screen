#pragma force_active on
#include "global.h"
typedef struct Ov91Sub {
    u32 a, b, c;
} Ov91Sub;
typedef struct Ov91Data {
    u32 x0[5];
    Ov91Sub sub;
    u32 x20[2];
} Ov91Data;
typedef struct Ov91Entry {
    u8 state;
    u8 pad1;
    u16 x2;
    Ov91Data data;
    Ov91Sub sub;
    u32 x38;
    u32 x3C;
    u32 x40;
} Ov91Entry;
typedef struct Ov91Work {
    u8 pad0[0x10];
    void *x10;
    u32 x14;
    u8 pad18[0x4C - 0x18];
    Ov91Entry entries[0x60];
    u8 pad19CC[0x1AB4 - 0x19CC];
    u8 x1AB4[4];
} Ov91Work;
void ov91_02260728(void *, Ov91Entry *, void *, u32);
void ov91_02260754(void *, Ov91Entry *);
void ov91_0225E400(void *);
void ov91_0225E2E4(Ov91Work *work, Ov91Data *data, u32 kind);
// @fn ov91_0225E2E4
void ov91_0225E2E4(Ov91Work *work, Ov91Data *data, u32 kind) {
    int i;
    for (i = 0; i < 0x60; i++) {
        if (work->entries[i].state == 0) {
            work->entries[i].data = *data;
            ((Ov91Entry *)((u8 *)work + 0x4C))[i].state = kind;
            work->entries[i].sub = data->sub;
            work->entries[i].x2 = 0;
            work->entries[i].x3C = 0;
            ov91_02260728(work->x1AB4, &work->entries[i], work->x10, work->x14);
            return;
        }
    }
    if (kind == 1) {
        for (i = 0; i < 0x60; i++) {
            if (work->entries[i].state == 2 || work->entries[i].state == 5) {
                ov91_02260754(work->x1AB4, &((Ov91Entry *)((u8 *)work + 0x4C))[i]);
                ov91_0225E400(&work->entries[i]);
                work->entries[i].data = *data;
                work->entries[i].state = kind;
                work->entries[i].sub = data->sub;
                work->entries[i].x2 = 0;
                work->entries[i].x3C = 0;
                ov91_02260728(work->x1AB4, &((Ov91Entry *)((u8 *)work + 0x4C))[i], work->x10, work->x14);
                return;
            }
        }
        GF_ASSERT(FALSE);
    }
}
