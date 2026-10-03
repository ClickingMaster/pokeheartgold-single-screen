#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 p[0xd];u8 count;u8 p2[2];void *object;u8 snapshot,state,p3,connected;u8 p4[8];u32 token;u8 p5[0xc4];u32 mode;u32 tick[2];void *list[32];u8 p6[0x28];void *temporary;u8 p7[0x12];u16 flag;u8 p8[0x24];u32 pending;u8 p9[0x24];u32 peer,result;u8 p10[0x1c4];u8 active;} State;
extern State *ov00_021F955C(void);extern void ov00_02210CBC(void *),ov00_0221019C(void *),ov00_021F6FC4(void),ov00_021F956C(int),ov00_021F5D0C(int,int,int),ov00_021F70B8(void *),ov00_021F6EC4(int);extern int ov00_021F75F4(void *,int);int ov00_021F6100(void *);
static inline int sameToken(void){State *a=ov00_021F955C();State *b=ov00_021F955C();return a->token==b->peer;}
int ov00_021F6100(void *arg){int clear;State *s;if(ov00_021F955C()->connected!=0&&sameToken())clear=0;else clear=1;if(clear){ov00_021F955C()->connected=0;ov00_021F955C()->token=0;ov00_02210CBC(ov00_021F955C()->object);}if(ov00_021F955C()->count<31){s=ov00_021F955C();s->list[ov00_021F955C()->count+1]=0;}ov00_021F955C()->active=255;if(ov00_021F955C()->temporary!=0){ov00_0221019C(ov00_021F955C()->temporary);ov00_021F955C()->temporary=0;}s=ov00_021F955C();s->snapshot=ov00_021F955C()->count;ov00_021F955C()->result=0;if(!clear){if(ov00_021F955C()->state!=3)ov00_021F6FC4();}else if(ov00_021F955C()->state==0){ov00_021F956C(3);ov00_021F955C()->mode=2;s=ov00_021F955C();*(u64 *)s->tick=OS_GetTick();}else if(ov00_021F955C()->state==1){ov00_021F956C(4);ov00_021F5D0C(1,0,0);}else if(ov00_021F955C()->state==2){int i;ov00_021F956C(14);ov00_021F955C()->pending=0;ov00_021F955C()->flag=0;ov00_021F70B8(arg);for(i=1;i<=ov00_021F955C()->count;i++){if(ov00_021F75F4(ov00_021F955C()->list[i],13)==0)return 0;}if(ov00_021F955C()->count==0)ov00_021F6EC4(2);}return 1;}
