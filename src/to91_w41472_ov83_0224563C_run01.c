#include "global.h"
#include "unk_02005D10.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))

u8 ov83_0224776C(u8,u8);int ov83_02245068(int);int Comm_GetLocalNetId(void);void ov83_02244ABC(u8*,int);void ov80_02237FA4(void*,int,int);void ov83_022477C4(u32,int);void ov83_022453DC(void*,void*);void ov83_02245390(void*);void ov83_02245824(u8*,int);void ov83_02245838(void*,int,int);void ov83_02245288(void*,int);void ov83_02245318(void*,int);void ov83_0224563C(u8*,u8,u32);
void ov83_0224563C(u8*s,u8 participant,u32 kind){u32 id,count;int amount;PlaySE(1507);count=s[0x15];id=ov83_0224776C(count,participant);switch(kind){case 0:amount=1;break;case 1:amount=ov83_02245068(s[0x12]);break;case 3:amount=2;break;case 4:amount=5;break;}if(!Comm_GetLocalNetId()){if(participant<count){ov83_02244ABC(s,5);ov80_02237FA4(*(void **)(s+4),s[9],amount);}else{ov83_022477C4((u32)*(void **)(s+0x24),5);*(u16 *)(s+0x5ba)-=amount;}}else{if(participant<count){ov83_022477C4((u32)*(void **)(s+0x24),5);*(u16 *)(s+0x5ba)-=amount;}else{ov83_02244ABC(s,5);ov80_02237FA4(*(void **)(s+4),s[9],amount);}}ov83_022453DC(s,(u8*)s+0x50);ov83_02245390(s);switch(kind){case 0:ov83_02245824(s,id);return;case 1:ov83_02245838(s,id,s[0x12]);return;case 3:ov83_02245288(s,id);return;case 4:ov83_02245318(s,id);break;}}
