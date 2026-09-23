#include "global.h"

typedef struct { u32 a, b, c; } Ov41Triplet;
u32 ov41_0224895C(void *work, int index);
void GF_AssertFail(void);

u32 ov41_0224895C(void *work, int index)
{
    Ov41Triplet value;
    if (work == NULL) {
        GF_AssertFail();
    }
    value = ((Ov41Triplet *)((u8 *)work + 0xc))[index];
    return value.c;
}
