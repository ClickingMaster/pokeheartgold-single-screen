#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_lifetimes off
extern u8 ov90_0225C2CC[],ov90_0225C2A4[],ov90_0225C39C[];
extern void *Heap_Alloc(int,int),*Save_GameStats_Get(void*),*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int),*SysTask_CreateOnVWaitQueue(void(*)(void*,void*),void*,int);
extern void Sound_SetSceneAndPlayBGM(int,int,int),GfGfx_SetBanks(const void*),ov90_0225C178(void*),ov90_0225A204(void*,void*),ov90_02258FF0(void*,void*,void*,int,int),ov90_022590CC(void*,int,int,int,int),ov90_02258DD0(void*,int,int),ov90_02259184(void*,int),ov90_022596C8(void*,int),ov90_0225938C(void*,void*,void*,int,int),ov90_02259794(void*,void*),ov90_02259B18(void*,void*),ov00_021E69A8(int),GameStats_AddScore(void*,int);
extern int ov90_0225888C(void*,u8),ov90_022588A4(void*,int);
u8 *ov90_02259588(const u8*,u16,int);
u8 *ov90_02259588(const u8 *config,u16 mode,int heap){u8 *s;Sound_SetSceneAndPlayBGM(19,0x480,0);s=Heap_Alloc(heap,0x5f4);memset(s,0,0x5f4);memcpy(s+8,config,16);*(u16*)(s+4)=mode;*(vu16*)0x04000050=0;*(vu16*)0x04001050=0;*(vu32*)0x04000000&=0xffff1fff;*(vu32*)0x04001000&=0xffff1fff;GfGfx_SetBanks(ov90_0225C2CC);*(u16*)(s+2)=heap;ov90_0225C178(s);ov90_0225A204(s+0x18,s+8);ov90_02258FF0(s+0x30,ov90_0225C2A4,ov90_0225C39C,5,heap);ov90_022590CC(s+0x84,16,1,1,heap);ov90_02258DD0(s+0x1b0,1,heap);ov90_02259184(s+0x3c,heap);ov90_022596C8(s,heap);ov90_0225938C(s+0x4c,s+0x30,*(void**)(s+8),ov90_022588A4(s+8,ov90_0225888C(s+8,s[0x11])),heap);*(void**)(s+0x5ec)=SysTask_CreateOnMainQueue(ov90_02259794,s,0);*(void**)(s+0x5f0)=SysTask_CreateOnVWaitQueue(ov90_02259B18,s,0);if(s[0x12])ov00_021E69A8(heap);{void *stats=Save_GameStats_Get(*(void**)(s+8));if(s[0x13])GameStats_AddScore(stats,39);}return s;}
