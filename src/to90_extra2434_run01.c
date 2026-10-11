#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

void sub_02039528(void*),WirelessStatusIcon_Create(void),ov75_022494CC(void*,void*,int,int,int),ov75_0224785C(void*),Sys_SetSleepDisableFlag(int),ov00_021EC3F0(void*,int,int,int),ov00_021EC454(int),ov00_021EC4A4(void);int ov75_02249534(void*);int ov75_02248774(void*);
int ov75_02248774(void*s){switch(W(s,0x94)){case 0:sub_02039528(P(P(s,0),4));WirelessStatusIcon_Create();ov75_022494CC(s,P(s,0x2c),1,1,3855);ov75_0224785C(s);W(s,0x94)++;break;case 1:if(ov75_02249534(P(s,0x44))==0)W(s,0x94)++;break;case 2:Sys_SetSleepDisableFlag(4);ov00_021EC3F0((u8*)P(s,0)+20,2,1,20);ov00_021EC454(2);ov00_021EC4A4();W(s,8)=13;break;}return 0;}
