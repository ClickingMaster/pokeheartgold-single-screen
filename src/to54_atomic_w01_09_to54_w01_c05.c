#include "to54_atomic_w01_to54_w01_internal.h"
#define U8_AT(p, o)   (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT2(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT2(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT2(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0221D270(void *p) {
    int a, b;
    U32_AT2(p, 0x18) += 4;
    a = ov07_0223192C(p, U16_AT2(PTR_AT2(p, 0xc0), 0x14));
    b = ov07_0223192C(p, U16_AT2(PTR_AT2(p, 0xc0), 0x16));
    if (a == b) {
        U32_AT2(p, 0x18) += U32_AT2(PTR_AT2(p, 0x18), 0) * 4;
    } else {
        U32_AT2(p, 0x18) += 4;
    }
}

void ov07_0221D2B0(void *p) {
    u32 v;
    U32_AT2(p, 0x18) += 4;
    v = U32_AT2(PTR_AT2(p, 0x18), 0);
    U32_AT2(p, 0x18) += 4;
    if (v == U16_AT2(PTR_AT2(p, 0xc0), 0x10)) {
        U32_AT2(p, 0x18) += U32_AT2(PTR_AT2(p, 0x18), 0) * 4;
    } else {
        U32_AT2(p, 0x18) += 4;
    }
}

void ov07_0221D2E4(void *p) {
    U32_AT2(p, 0x18) += 4;
    U32_AT2(p, 0x18) += U32_AT2(PTR_AT2(p, 0x18), 0) * 4;
}
