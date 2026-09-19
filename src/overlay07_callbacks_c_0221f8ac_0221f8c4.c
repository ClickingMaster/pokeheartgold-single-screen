#include "overlay07_callbacks_w02_private.h"

#define U8(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct Inner {
    u8 pad00[0x1c];
    u32 arr1C[16];
    u8 pad5C[0x54];
    void *ptrB0[3];
    u8 bytesBC[0x2c];
    u32 wordsEC[16];
} Inner;
typedef struct Outer {
    u8 pad00[0xc0];
    Inner *inner;
} Outer;

void ov07_0221F8AC(void) {}
u32 ov07_0221F8B0(u32 i) {
    extern u32 ov07_02234DE8[];
    if (i > 0x58) return 0;
    return ov07_02234DE8[i];
}
u8 ov07_0221F8C4(void *p) { return U8(p, 1); }
