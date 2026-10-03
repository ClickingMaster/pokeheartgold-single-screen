#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8*ov00_021F955C(void);extern u8 ov00_0221A928[];
extern int ov00_021EC230(void),ov00_021F5B30(void),ov00_021F5FD4(int),ov00_021F82EC(void),ov00_021F631C(u32),ov00_021F6100(u32),ov00_021EE568(void),ov00_02213984(u32),ov00_021F6FF8(void),ov00_021F7384(void),ov00_021F7898(void);
extern void ov00_02210AC8(u32),ov00_0220CFC4(u32),ov00_021F2EE4(int,int),ov00_021F5B5C(u32,int),ov00_021F3CF8(u32),ov00_021F2EC4(void),ov00_021F4304(int,u32,u32,u16,void*,u32),ov00_021F6FC4(void),ov00_021F9104(int,int,int,void*),ov00_02213938(u32),ov00_022136E4(u32),ov00_021F1D34(u32),ov00_022103C4(void),ov00_0220FB40(void),ov00_021F7E4C(void),ov00_021F36BC(void);
void ov00_021F1DE4(int);
#define GET() ov00_021F955C()
#define W(p,o) (*(u32*)((p)+(o)))
#define I(p,o) (*(s32*)((p)+(o)))
#define B(p,o) ((p)[o])
#define H(p,o) (*(u16*)((p)+(o)))
#define T(p,o) (*(u64*)((p)+(o)))
#define ELAPSED(now,p,o) ((((now)-T(p,o))<<6)/0x82ea)
void ov00_021F1DE4(int active){if(!GET())return;if(ov00_021EC230())return;if(!active){if(W(GET(),0x10))ov00_02210AC8(W(GET(),0x10));if(!W(GET(),4))return;ov00_0220CFC4(*(u32*)W(GET(),4));return;}if(!W(GET(),0x1a0))return;switch(W(GET(),0x1a0)){case 0:case 6:case 8:case 9:case 10:case 12:break;case 4:if(W(GET(),0x1c8)){u64 now=OS_GetTick();u8*stamp=GET();u8*limit=GET();if(ELAPSED(now,stamp,0x1cc)>W(limit,0x1c8)){W(GET(),0x1c8)=0;if(B(GET(),0x15)==3){B(GET(),0x1aa)++;if(B(GET(),0x1aa)>5){ov00_021F2EE4(6,-80430);return;}ov00_021F5B5C(W(GET(),0xf4),0);if(ov00_021F5B30())return;}else if(!ov00_021F5FD4(0))return;}}if(W(GET(),0x1bc)){u32 limit=3000*B(GET(),0xd)+3000;u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x1c0)>=limit){ov00_021F5B5C(W(GET(),0xf4),0);if(ov00_021F5B30())return;}}break;
case 2:case 3:case 5:if(I(GET(),0xe8)>0){u32 limit;u64 now;if(W(GET(),0x1a0)==3)limit=3000*B(GET(),0xd)+3000;else limit=W(GET(),0xe8)==1?1000:3000;now=OS_GetTick();if(ELAPSED(now,GET(),0xec)>limit){ov00_021F3CF8(W(GET(),0x204));if(ov00_021F82EC())return;W(GET(),0xe8)=0;}}break;
case 7:if(T(GET(),0x18c)){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x18c)>25000){T(GET(),0x18c)=0;if(!ov00_021F631C(W(GET(),0xf4)))return;}}else if(B(GET(),0x3cc)==6){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x45c)>6000){B(GET(),0x3cd)++;if(B(GET(),0x3cd)>5){ov00_021F2EC4();if(!ov00_021F631C(W(GET(),0xf4)))return;}else{ov00_021F4304(6,W(GET(),0x454),W(GET(),0x3d0),H(GET(),0x3ce),GET()+0x3d4,W(GET(),0x458));if(ov00_021F5B30())return;}}}break;
case 11:if(B(GET(),0x3cc)==2){if(!B(GET(),0x15)){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x45c)>6000)goto expired11;}if(B(GET(),0x15)){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x45c)>19000){expired11:ov00_021F2EC4();if(!ov00_021F6100(((u32*)(GET()+0xf4))[B(GET(),0xd)+1]))return;}}}break;
case 13:if(B(GET(),0x3cc)==8){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x45c)>30000){B(GET(),0x3cd)++;if(B(GET(),0x3cd)){ov00_021F2EC4();if(B(GET(),0x15)==2){if(!ov00_021F6100(((u32*)(GET()+0xf4))[B(GET(),0xd)]))return;}else ov00_021F6FC4();}else{ov00_021F4304(8,W(GET(),0x454),W(GET(),0x3d0),H(GET(),0x3ce),GET()+0x3d4,W(GET(),0x458));if(ov00_021F5B30())return;}}}break;
case 1:if(ov00_021EE568()==5){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x1f8)>30000){if(B(GET(),0x1ad)>=5){if(!ov00_021F631C(W(GET(),0xf4)))return;}else{ov00_021F4304(64,W(GET(),0xf4),W(GET(),0x24),H(GET(),0xa4),NULL,0);if(ov00_021F5B30())return;B(GET(),0x1ad)++;T(GET(),0x1f8)=OS_GetTick()-12567750;}}}break;}
if(W(GET(),0x1a0)==11||W(GET(),0x1a0)==6){if(T(GET(),0x184)){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x184)>10000)ov00_021F9104(1,0,0,GET()+0x194);}}
if(W(GET(),0xe4)){W(ov00_0221A928,0x18)=0;W(ov00_0221A928,0x14)=0;ov00_02213938(W(GET(),0xe4));if(W(ov00_0221A928,0x14)){ov00_022136E4(W(GET(),0xe4));W(GET(),0xe4)=0;}if(W(GET(),0xe4)&&ov00_02213984(W(GET(),0xe4))&&T(GET(),0x174)){u64 now=OS_GetTick();if(now>T(GET(),0x174))ov00_021F2EE4(6,-85010);}}
ov00_021F1D34(W(GET(),0x10));W(ov00_0221A928,0xc)=0;W(ov00_0221A928,0x10)=1;ov00_022103C4();W(ov00_0221A928,0x10)=0;if(W(ov00_0221A928,0xc)==1){W(ov00_0221A928,0xc)=0;ov00_0220FB40();}if(W(GET(),4))ov00_0220CFC4(*(u32*)W(GET(),4));if(W(GET(),0x1a0)==18){u64 now=OS_GetTick();if(ELAPSED(now,GET(),0x1f0)>3000&&ov00_021F6FF8())return;}if(!ov00_021F7384())return;if(!ov00_021F7898())return;ov00_021F7E4C();if(!B(GET(),0x1ae))return;if(W(GET(),0x1a0)!=10)return;B(GET(),0x16)=B(GET(),0xd);ov00_021F36BC();B(GET(),0x1ae)=0;if(!W(GET(),0x47c))return;((void(*)(u32))W(GET(),0x47c))(W(GET(),0x480));}
