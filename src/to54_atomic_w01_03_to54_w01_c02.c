#include "to54_atomic_w01_to54_w01_internal.h"
#define U8_AT(p, o)   (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT2(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT2(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT2(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0221D088(void *p) {
    u32 i, v;
    U32_AT2(p, 0x18) += 4;
    i = U32_AT2(PTR_AT2(p, 0x18), 0);
    U32_AT2(p, 0x18) += 4;
    v = U32_AT2(PTR_AT2(p, 0x18), 0);
    U32_AT2(p, 0x18) += 4;
    if (v == U32_AT2(p, 0x94 + i * 4)) {
        PTR_AT2(p, 0x18) = PTR_AT2(PTR_AT2(p, 0x18), 0);
    } else {
        U32_AT2(p, 0x18) += 4;
    }
}

void ov07_0221D0C0(void *p) {
    U32_AT2(p, 0x18) += 4;
    if (ov07_0221F8C4(PTR_AT2(p, 0xc0))) {
        PTR_AT2(p, 0x18) = PTR_AT2(PTR_AT2(p, 0x18), 0);
    } else {
        U32_AT2(p, 0x18) += 4;
    }
}
