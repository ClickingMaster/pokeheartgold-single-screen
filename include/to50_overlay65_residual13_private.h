#ifndef TO50_OV65_R13_PRIVATE_H
#define TO50_OV65_R13_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct Ov65StateR13 {
    u8 pad0[0x2224];
    void *partyA;
    void *partyB;
    u8 pad222C[0x1488];
    void *field36B4;
} Ov65StateR13;
extern void sub_0200F450(void *);
extern s32 Party_GetCount(void *);
extern void *Party_GetMonByIndex(void *, u32);
extern u32 GetMonData(void *, u32, void *);
void ov65_0221F780(void *);
s32 ov65_0221F79C(void *);
s32 ov65_0221F7E0(void *);
#endif
