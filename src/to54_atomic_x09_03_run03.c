#include "global.h"
#pragma require_prototypes off

#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(u8 **)((u8 *)(p) + (o)))

extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetAnim(void *, int);
extern void ov112_021F1624(void *, int, int);
extern void ov112_021F2204(void *, int, int);
extern void ov112_021F22B0(void *, int, int);
extern void ov112_021F1EFC(void *, int);
extern void GF_AssertFail(void);

void ov112_021F24C4(void *p) {
    U32(p, 0x14C) = (U32(p, 0x14C) & ~1) | 1;
}

void ov112_021F24D8(void *p) {
    U32(p, 0x14C) &= ~1;
    U32(p, 0x14C) = (u8)U32(p, 0x14C);
    U32(p, 0x14C) &= ~0xFE;
}
