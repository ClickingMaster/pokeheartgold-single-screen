#pragma force_active on
#include "global.h"
typedef struct {s32 x,y,z;} Vec;
typedef struct {u8 p[0x9c];void*a,*b;} Work;
extern Vec ov93_02262A38;extern u8 ov93_02262A18[];
extern void *Camera_New(int);extern void Camera_Init_FromTargetDistanceAndAngle(Vec*,s32,void*,int,int,int,void*),Camera_SetPerspectiveClippingPlane(s32,s32,void*),Camera_SetStaticPtr(void*);
void ov93_0225CFC0(Work*w);
void ov93_0225CFC0(Work*w){Vec v=ov93_02262A38;w->a=Camera_New(117);Camera_Init_FromTargetDistanceAndAngle(&v,123<<12,ov93_02262A18,4004,1,0,w->a);Camera_SetPerspectiveClippingPlane(4096,225<<14,w->a);Camera_SetStaticPtr(w->a);w->b=Camera_New(117);Camera_Init_FromTargetDistanceAndAngle(&v,5<<16,ov93_02262A18,4004,1,0,w->b);Camera_SetPerspectiveClippingPlane(4096,225<<14,w->b);}
