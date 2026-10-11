#pragma force_active on
#include "global.h"
#include "overlay71_opus_partial_internal.h"
#include "sys_task.h"
#pragma force_active on
typedef struct W{void *owner;SysTask *task;void *actor;void *sprite;VecFx32 base,pos,output;union { VecFx16 signedValue; Ov71S16Vec3 cameraValue; } rotation;u8 align3a[2];s32 baseDeltaX,baseDeltaY;struct {s32 x,y,dx,dy,z,dz,ticks;} motion;s32 state,delay,elapsed;s16 angle,angleDelta;}W;void ov71_02249A98(void *);extern void TradeSequence_SetModelScale(Ov71Cam2*,u32),TradeSequence_SetModelRotation(Ov71Cam*,const Ov71S16Vec3*);extern fx32 GF_CosDegNoWrap(u16),GF_SinDegNoWrap(u16);
void ov71_02249A98(void *raw){W *w=raw;int angle=(u16)w->angle/182;if(angle>=360)angle-=360;w->angle+=w->angleDelta;if(w->motion.ticks){w->motion.x=(s32)((u32)w->motion.x+(u32)w->motion.dx);w->motion.y=(s32)((u32)w->motion.y+(u32)w->motion.dy);int z=(s32)((u32)w->motion.z+(u32)w->motion.dz);w->motion.z=z;TradeSequence_SetModelScale(w->sprite,(u32)z);w->motion.ticks=(s32)((u32)w->motion.ticks-1);}{fx32 x=w->motion.x;fx32 c=GF_CosDegNoWrap((u16)angle);w->pos.x=FX_Mul(c,x);}{fx32 y=w->motion.y;fx32 c=GF_SinDegNoWrap((u16)angle);w->pos.y=FX_Mul(c,y);}w->pos.z=0;w->rotation.signedValue.x+=768;TradeSequence_SetModelRotation(w->sprite,&w->rotation.cameraValue);VEC_Add(&w->pos,&w->base,&w->output);}
