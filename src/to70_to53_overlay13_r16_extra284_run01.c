#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8 ov13_0224DF30[];extern void *ov13_0224DFB0;
int ov13_02223A7C(void),WCM_ConnectAsync(void *,void *,int),WCM_Init(void *,int),WCM_StartupAsync(void *,void (*)(void));void ov13_02223770(void);
int ov13_02223CA0(void *,void *,int),ov13_02223D24(void *,u8 *,int);
#define P(o) (*(void **)(ov13_0224DF30+(o)))
#define W(o) (*(int *)(ov13_0224DF30+(o)))
int ov13_02223CA0(void *network,void *key,int option){OSIntrMode mode=OS_DisableInterrupts();W(88)=option;if(key)MI_CpuCopy8(key,P(40),96);else MI_CpuFill8(P(40),0,96);MIi_CpuCopy32(network,P(76),192);if(ov13_02223A7C()==1){W(44)=8;OS_RestoreInterrupts(mode);return 1;}if(W(44)==3){if(WCM_ConnectAsync(P(76),P(40),W(88))==3){W(44)=8;OS_RestoreInterrupts(mode);return 1;}}OS_RestoreInterrupts(mode);return 0;}
int ov13_02223D24(void *owner,u8 *buffer,int size){OSIntrMode mode=OS_DisableInterrupts();u32 cursor=((u32)buffer+99)&~3;P(40)=buffer;P(8)=(void *)cursor;cursor=(cursor+47)&~31;P(60)=(void *)cursor;cursor=(cursor+8991)&~31;P(76)=(void *)cursor;cursor=(cursor+223)&~31;*(void **)((u8 *)P(8)+4)=(void *)cursor;*(int *)((u8 *)P(8)+8)=(buffer+size)-(u8 *)*(void **)((u8 *)P(8)+4);*(int *)((u8 *)P(8)+12)=0;*(int *)P(8)=3;ov13_0224DFB0=owner;if(W(44)==0){if(WCM_Init(P(60),8960)!=0){OS_RestoreInterrupts(mode);return 0;}W(44)=1;}if(W(44)==1){if(WCM_StartupAsync(P(8),ov13_02223770)!=3){OS_RestoreInterrupts(mode);return 0;}W(44)=4;OS_RestoreInterrupts(mode);return 1;}OS_RestoreInterrupts(mode);return 0;}
