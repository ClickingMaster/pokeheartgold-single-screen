#include "to54_atomic_w01_to54_w01_internal.h"
#define U8_AT(p, o)   (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT2(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT2(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT2(p, o) (*(void **)((u8 *)(p) + (o)))

BOOL ov07_0221C634(void *p) {
    u8 old = U8_AT(p, 4);
    int ret;
    U8_AT(p, 4) = old + 1;
    if (old < U8_AT(p, 3)) {
        return TRUE;
    }
    U8_AT(p, 4) = 0;
    ret = 1;
    U8_AT(p, 0x18)
    --;
    PlaySE(U16_AT2(p, 0x1a));
    SetSeqTrackPan(U16_AT2(p, 0x1a), 0xffff, U32_AT2(p, 0x14));
    if (U8_AT(p, 0x18) == 0) {
        ret = 0;
    }
    return ret;
}

BOOL ov07_0221C674(void *p) {
    u8 old = U8_AT(p, 3);
    int ret = 1;
    U8_AT(p, 3) = old - 1;
    if (old == 0) {
        PlaySE(U16_AT2(p, 0x1a));
        SetSeqTrackPan(U16_AT2(p, 0x1a), 0xffff, U32_AT2(p, 0x14));
        ret = 0;
    }
    return ret;
}

void ov07_0221C69C(void) {
    G2x_SetBlendAlpha_(0x04000050, 0, 0x3f, 8, 8);
}

void ov07_0221C6B4(void *p) {
    U8_AT(p, 0x8d) = 1;
    if ((U32_AT2(gSystem, 0x44) & 0x200) && (U32_AT2(gSystem, 0x44) & 0x100) && (U32_AT2(gSystem, 0x48) & 0x400)) {
        U32_AT2(p, 0x18) += 4;
        U8_AT(p, 0x8d) = 0;
    }
}

void ov07_0221C6EC(void *p) {
    U32_AT2(p, 0x18) += 4;
    U8_AT(p, 0x8d) = U32_AT2(PTR_AT2(p, 0x18), 0);
    U32_AT2(p, 0x18) += 4;
    PTR_AT2(p, 0xbc) = ov07_0221BE20;
}

void ov07_0221C70C(void *p) {
    if (U16_AT2(p, 0x8e) == 0) {
        U32_AT2(p, 0x18) += 4;
        U8_AT(p, 0x8d) = 0;
    } else {
        U8_AT(p, 0x8d) = 1;
    }
}

void ov07_0221C72C(void *p) {
    u32 i, v;
    U32_AT2(p, 0x18) += 4;
    i = U32_AT2(PTR_AT2(p, 0x18), 0);
    U32_AT2(p, 0x18) += 4;
    v = U32_AT2(PTR_AT2(p, 0x18), 0);
    U32_AT2(p, 0x18) += 4;
    if (i < 10) {
        U32_AT2(p, 0x94 + i * 4) = v;
    }
}

void ov07_0221C74C(void *p) {
    int i;
    U32_AT2(p, 0x18) += 4;
    for (i = 0; i < 10; i++) {
        U32_AT2(p, 0x94) = 0;
        p = (u8 *)p + 4;
    }
}
