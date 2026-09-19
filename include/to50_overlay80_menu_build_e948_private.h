#ifndef TO50_OV80_R35H_PRIVATE_H
#define TO50_OV80_R35H_PRIVATE_H
#include "global.h"

#include "sys_task_api.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *FrontierSystem_GetFrontierMap(void *);
extern u32 ov80_0222EB14(void *);
extern void AddWindowParameterized(void *, void *, u8, u8, u8, u8, u8, u8, u16);
extern void DrawFrameAndWindow1(void *, BOOL, u16, u8);
extern void ov80_0222EB54(void *);
extern void *ListMenuInit(void *, u32, u8, u8);
extern void ov80_0222EE7C(void *);
extern void ov80_0222EC90(SysTask *, void *);
void ov80_0222E948(void *);
#endif
