#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
void PlaySE(u16);
typedef struct{s32 x,y,z;} Vec;typedef struct{s32 kind,positive;Vec value;} Params;void ov07_022217A4(void*,Params*);s32 ov07_02221664(void*);void ParticleEmitter_GetConvergencePos(void*,Vec*);void ov07_02231B90(void*,void*,Vec*);void ParticleEmitter_SetConvergencePos(void*,Vec*);int ov07_02221AE4(void*,void*);
int ov07_02221AE4(void*obj,void*s){Params p;Vec out={0,0,0};ov07_022217A4(s,&p);*(s8*)((u8*)s+8)=ov07_02221664(s);if(p.positive==1)B(s,8)=1;switch(p.kind){case 0:ParticleEmitter_GetConvergencePos(obj,&out);out.x*=*(s8*)((u8*)s+8);out.y*=*(s8*)((u8*)s+8);out.z*=*(s8*)((u8*)s+8);break;case 1:out.x=p.value.x;out.y=p.value.y;out.z=p.value.z;out.x*=*(s8*)((u8*)s+8);out.y*=*(s8*)((u8*)s+8);out.z*=*(s8*)((u8*)s+8);break;case 4:ov07_02231B90(P(s,0),P(s,0x28),&out);{int a=p.value.x,b=p.value.y;out.x=out.x*a/b;out.y=out.y*a/b;out.z=out.z*a/b;}break;case 2:ov07_02231B90(P(s,0),P(s,0x24),&out);break;case 3:ov07_02231B90(P(s,0),P(s,0x28),&out);break;case 255:break;}out.x-=SW(s,0x2c);out.y-=SW(s,0x30);out.z-=SW(s,0x34);ParticleEmitter_SetConvergencePos(obj,&out);return 1;}
