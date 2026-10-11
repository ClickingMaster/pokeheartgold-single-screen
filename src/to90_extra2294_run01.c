#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
typedef struct{s32 kind,positive; s32 value[3];} Params;void ov07_022217A4(void*,Params*);s32 ov07_02221664(void*);void ParticleEmitter_GetConvergenceForce(void*,s16*);void ParticleEmitter_SetConvergenceForce(void*,s16*);int ov07_02221BF0(void*,void*);
int ov07_02221BF0(void*o,void*s){Params p;s16 out=0;ov07_022217A4(s,&p);B(s,8)=ov07_02221664(s);if(p.positive==1)B(s,8)=1;switch(p.kind){case 0:ParticleEmitter_GetConvergenceForce(o,&out);out*=*(s8*)((u8*)s+8);break;case 1:out=p.value[0];out*=*(s8*)((u8*)s+8);break;case 2:case 3:case 255:break;}ParticleEmitter_SetConvergenceForce(o,&out);return 1;}
