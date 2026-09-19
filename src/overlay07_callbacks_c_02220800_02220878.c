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

void ov07_02220800(void *p) { ov07_022207D0(p, 0, 0); }
void ov07_0222080C(void *p) { ov07_022207D0(p, 1, 1); }
void ov07_02220818(void *p) { ov07_022207D0(p, 1, 2); }
void ov07_02220824(void *p) { ov07_022207D0(p, 1, 3); }
void ov07_02220830(void *p) { ov07_022207D0(p, 1, 4); }
void ov07_0222083C(void *p) { ov07_022207D0(p, 1, 5); }
void ov07_02220848(void *p) { ov07_022207D0(p, 2, 1); }
void ov07_02220854(void *p) { ov07_022207D0(p, 2, 2); }
void ov07_02220860(void *p) { ov07_022207D0(p, 2, 3); }
void ov07_0222086C(void *p) { ov07_022207D0(p, 2, 4); }
void ov07_02220878(void *p) { ov07_022207D0(p, 2, 5); }
