#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Params{int mode,fixed;VecFx32 value;}Params;typedef struct W{u8 pad[8];s8 sign;u8 gap[35];VecFx32 offset;}W;extern void ov07_022217A4(W*,Params*),ParticleEmitter_SetGravity(void*,VecFx16*);extern s8 ov07_02221664(W*);int ov07_022217B4(void*,W*);
int ov07_022217B4(void*object,W*w){Params params;VecFx16 value={0,0,0};ov07_022217A4(w,&params);w->sign=ov07_02221664(w);if(params.fixed==1)w->sign=1;switch(params.mode){case 0:ParticleEmitter_SetGravity(object,&value);value.x*=w->sign;value.y*=w->sign;value.z*=w->sign;break;case 1:value.x=params.value.x;value.y=params.value.y;value.z=params.value.z;value.x*=w->sign;value.y*=w->sign;value.z*=w->sign;ParticleEmitter_SetGravity(object,&value);return 1;case 2:case 3:case 255:break;}value.x-=w->offset.x;value.y-=w->offset.y;value.z-=w->offset.z;ParticleEmitter_SetGravity(object,&value);return 1;}
