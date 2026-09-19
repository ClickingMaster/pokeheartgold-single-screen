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

u32 ov07_0221FA78(void *p) { return U32(p, 0xc8); }
u32 ov07_0221FA80(Outer *p, int i) { return U32(p->inner->ptrB0[i], 8); }
u32 ov07_0221FA90(Outer *p, int i) { return U32(p->inner->ptrB0[i], 4); }
u32 ov07_0221FAA0(Outer *p, int i) { return U32(p->inner->ptrB0[i], 12); }
BOOL ov07_0221FAB0(void *p) { return (U32((void *)U32(p, 0xc0), 0xd4) & 2) != 0; }
