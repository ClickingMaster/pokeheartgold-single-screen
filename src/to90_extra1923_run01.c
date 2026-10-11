#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[12];void*sprite;VecFx32 base,pos,output;VecFx16 rotation;u8 gap[10];int x,y,dx,dy,z,dz,ticks;u8 gap2[12];s16 angle,angleDelta;}W;void ov71_02249A60(W*,int,int,int,int),ov71_02249A98(W*);extern void TradeSequence_SetModelScale(void*),TradeSequence_SetModelRotation(void*,const VecFx16*);extern fx32 GF_CosDegNoWrap(u16),GF_SinDegNoWrap(u16);
void ov71_02249A60(W*w,int x,int y,int z,int ticks){int count=ticks;if(count){w->dx=(x-w->x)/count;w->dy=(y-w->y)/count;w->dz=(z-w->z)/count;w->ticks=*(volatile int*)&ticks;}}
