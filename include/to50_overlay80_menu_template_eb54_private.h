#ifndef TO50_OV80_R35T_PRIVATE_H
#define TO50_OV80_R35T_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct ListMenu ListMenu;
extern void ov80_0222EC84(ListMenu *, s32, u8);
extern void ov80_0222EC60(ListMenu *, s32, u8);
void ov80_0222EB54(void *);
#endif
