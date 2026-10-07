#include "global.h"
typedef struct {u8 pad[0x10];u32 orientation,angle;} Context;
typedef struct {u16 angle,pad;VecFx32 velocity;fx32 speed;union {VecFx32 vector;fx32 words[3];} position;struct {u8 reserved[4];u32 initialized;} tail;} Param;
extern void ov91_02260400(int,int,s32 *),ov91_0226045C(const VecFx32 *,VecFx32 *,int,int),ov91_0225E2E4(void *,void *,int);
void ov91_0225D500(void *owner,s32 x,s32 y,s32 endx,u16 endy,int mode);
#pragma force_active on
void ov91_0225D500(void *owner,s32 x,s32 y,s32 endx,u16 endy,int mode){Context*p=owner;Param param;param.angle=p->angle;param.pad=0;param.velocity.x=(u32)(s32)(s16)((s32)((u32)x-(u32)endx)/2)<<12;param.velocity.z=(u32)(s32)(s16)((s32)((u32)y-(u32)endy)/2)<<12;param.velocity.y=0;if(VEC_Mag(&param.velocity)>0x1e200){VEC_Normalize(&param.velocity,&param.velocity);param.velocity.x=((s64)param.velocity.x*0x1e200+2048)>>12;param.velocity.z=((s64)param.velocity.z*0x1e200+2048)>>12;}param.velocity.y=38<<12;param.speed=VEC_Mag(&param.velocity);param.speed=((s64)param.speed*0x99a+2048)>>12;VEC_Normalize(&param.velocity,&param.velocity);ov91_02260400((s16)endx,(s16)endy,param.position.words);param.position.vector.y=0;ov91_0226045C(&param.position.vector,&param.position.vector,param.angle,p->orientation);ov91_0226045C(&param.velocity,&param.velocity,param.angle,p->orientation);param.tail.initialized=0;ov91_0225E2E4(p,&param,mode);}
