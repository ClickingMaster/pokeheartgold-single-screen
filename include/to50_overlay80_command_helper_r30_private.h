#ifndef TO50_OV80_R30_PRIVATE_H
#define TO50_OV80_R30_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern int Comm_CountConnected(void *);
extern int sub_02037B38(u8);
extern void *Frontier_GetData(void *);
extern u32 Comm_GetLocalNetId(void);
extern void *sub_02034818(u32);
extern u32 PlayerProfile_GetVersion(void *);
extern BOOL sub_02096BF8(void *, u16);
BOOL ov80_0222E198(void *);
#endif
