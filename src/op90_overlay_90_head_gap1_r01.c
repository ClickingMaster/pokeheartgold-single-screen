#pragma force_active on
#include "global.h"

typedef struct Ov90Shared {
    u8 pad0[4];
    u8 ids[8];
    u8 padC[0x30 - 0xC];
    void *unk30;
} Ov90Shared;

typedef struct Ov90Work {
    u8 pad0[7];
    u8 unk7;
    u8 pad8[0x14 - 8];
    u8 count;
    u8 pad15[3];
    Ov90Shared *shared;
    int netIds[4];
    u8 done[8];
} Ov90Work;

int ov90_0225BA14(Ov90Work *work);
void ov45_0222ACB8(void *a0, u8 a1, int count, u32 id0, u32 id1, u32 id2, u32 id3);
void ov90_0225BA38(Ov90Work *work);

// @fn ov90_0225BA38
void ov90_0225BA38(Ov90Work *work) {
    u32 ids[4] = {0};
    int i;
    int n;
    int self;

    n = 0;
    self = ov90_0225BA14(work);
    for (i = 0; i < work->count; i++) {
        if (work->done[i] == 0 && work->netIds[i] != self) {
            ids[n] = work->shared->ids[i];
            n++;
        }
    }
    if (n != 0) {
        ov45_0222ACB8(work->shared->unk30, work->unk7, n, ids[0], ids[1], ids[2], ids[3]);
    }
}
