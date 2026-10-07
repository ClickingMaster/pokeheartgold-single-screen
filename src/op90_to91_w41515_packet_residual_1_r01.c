#pragma force_active on
#include "global.h"

typedef struct Ov81Work {
    u8 pad0[9];
    u8 mode;
    u8 padA[0x18 - 0xA];
    u8 count;
    u8 pad19[0x45A - 0x19];
    u16 unk45A[2];
    u16 unk45E[2];
    u8 pad462;
    u8 received;
} Ov81Work;

int ov80_02236DD4(u8 mode);
int sub_0203769C(void);
void ov81_022410C8(int netId, int size, u16 *data, Ov81Work *work);

// @fn ov81_022410C8
void ov81_022410C8(int netId, int size, u16 *data, Ov81Work *work) {
    int i;
    int pos = 0;

    ov80_02236DD4(work->mode);
    if (netId == sub_0203769C()) {
        return;
    }
    work->count = data[0];
    pos++;
    for (i = 0; i < work->count; i++) {
        work->unk45A[i] = data[pos + i];
    }
    pos += work->count;
    for (i = 0; i < work->count; i++) {
        work->unk45E[i] = data[pos + i];
    }
    work->received = TRUE;
}
