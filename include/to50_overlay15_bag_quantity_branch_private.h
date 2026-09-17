#ifndef TO50_OV15_R11_PRIVATE_H
#define TO50_OV15_R11_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void ov15_021FFF24(void *);
extern u32 Pocket_GetQuantity(void *, u32, u16, u32);
extern void ov15_021FEEA4(void *);
extern void ov15_021FD788(void *, s32);
extern void ov15_021FD574(void *, s32, s32, s32);
extern void ov15_02200300(void *, s32, u16);
extern void ov15_021FEDEC(void *, s32);
extern void ov15_021FF7C4(void *);
extern void ov15_021FF29C(void *, s32);
s32 ov15_021FBCAC(void *);
#endif
