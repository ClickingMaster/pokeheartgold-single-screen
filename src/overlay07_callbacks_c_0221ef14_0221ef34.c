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

void ov07_0221EF14(void *p) {
    if (U8(p, 0x17c) == 2) {
        U32(p, 0x18) += 4;
        U8(p, 0x8d) = 0;
    } else {
        U8(p, 0x8d) = 1;
    }
}
void ov07_0221EF34(void *p) {
    if (U8(p, 0x17c) == 0) {
        U32(p, 0x18) += 4;
        U8(p, 0x8d) = 0;
    } else {
        U8(p, 0x8d) = 1;
    }
}
