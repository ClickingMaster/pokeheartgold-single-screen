#ifndef TO50_OV80_R26_PRIVATE_H
#define TO50_OV80_R26_PRIVATE_H
#include "global.h"
typedef struct SysTask SysTask;
void *sub_0209680C(void *);
void GF_AssertFail(void);
void SysTask_Destroy(SysTask *);
u16 FrontierScript_ReadVar(void *);
void MI_CpuFill8(void *, u8, u32);
void ov80_0222F4F0(SysTask *, void *);
SysTask *SysTask_CreateOnMainQueue(void (*)(SysTask *, void *), void *, u32);
int FrtCmd_076(void *);
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#endif
