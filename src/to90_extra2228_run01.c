#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
typedef struct{u16 value,pad;} Entry;typedef struct{int count;u32 arg;u32 pad[2];int index,total;} State;State*ov85_021E8610(void*);extern const u16 ov85_021EA7C4[][5];void ov85_021E7B40(void*,u32,int,int,u32,u32,void*);int ov85_021E5E30(void*);
int ov85_021E5E30(void*s){State*t;if(--SW(s,12)>0)return 0;SW(s,12)=15;t=ov85_021E8610(s);ov85_021E7B40(s,((Entry*)((u8*)s+0x72))[t->index].value,t->index,t->count,ov85_021EA7C4[t->total][t->index],t->arg,(u8*)s+0x2d0+t->index*0xb0);t->index++;t->index%=t->total;if(++t->count==t->total){SW(s,12)=0;SW(s,0)=18;}return 0;}
