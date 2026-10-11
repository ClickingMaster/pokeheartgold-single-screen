#ifndef TO50_OV80_R15_PRIVATE_H
#define TO50_OV80_R15_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *sub_0209680C(void *);
extern void *FrontierMap_GetSprite(void *, u16);
extern BOOL FrontierMap_IsSpriteAnimActive(void *, u16);
extern BOOL ManagedSprite_IsAnimated(void *);
extern u16 FrontierScript_ReadVar(void *);
extern void *Heap_Alloc(u32, u32);
extern void Heap_Free(void *);
extern void GF_AssertFail(void);
extern void ov80_02239BF0(void *, u16, u32, u16 *, u32);
BOOL ov80_0222CA94(void *);
void ov80_0222CAD0(void *, int, int);
#endif
