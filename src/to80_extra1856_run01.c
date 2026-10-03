#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern u8 _021D5E68[];extern u32 _021D4680[];extern u8 _021D4620[];extern int rawip(char*,u32*);extern u32 resolve_sub(char*,u32,u16);u32 CPS_Resolve(char*);void find_session_from_id(u8*);
static inline u64 randomValue(void){*(u64*)(_021D4620+0x68)=*(u64*)(_021D4620+0x70)**(u64*)(_021D4620+0x68)+*(u64*)(_021D4620+0x78);return *(u64*)(_021D4620+0x68)>>32;}
u32 CPS_Resolve(char*s){u32 result;u16 id[2];u8 live[2];int i,j;id[0]=(randomValue()<<16)>>32;id[1]=(randomValue()<<16)>>32;if(rawip(s,&result))return result;live[0]=1;live[1]=1;for(i=0;i<3;i++){for(j=0;j<2;j++){if(live[j]){result=resolve_sub(s,_021D4680[j],id[j]);if(result&&result!=(u32)-1)goto done;if(result==(u32)-1)live[j]=0;}}}done:if(result==(u32)-1)result=0;return result;}
