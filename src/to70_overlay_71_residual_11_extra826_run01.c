#pragma force_active on
#include "global.h"
typedef struct {void *camera;VecFx32 target;u16 angles[3];} W;
extern void *Camera_New(int);extern void Camera_Init_FromTargetDistanceAndAngle(VecFx32*,int,void*,int,int,int,void*);extern void Camera_SetLookAtCamUp(VecFx32*,void*);extern void Camera_SetStaticPtr(void*);extern void Camera_SetPerspectiveClippingPlane(int,int,void*);
void ov71_0224784C(W*,int,int,int);
void ov71_0224784C(W*w,int x,int y,int z) {VecFx32 up;w->camera=Camera_New(57);w->target.x=x;w->target.y=y;w->target.z=z;w->angles[0]=0;w->angles[1]=0;w->angles[2]=0;Camera_Init_FromTargetDistanceAndAngle(&w->target,75<<14,w->angles,4004,0,1,w->camera);up.x=0;up.y=4096;up.z=0;Camera_SetLookAtCamUp(&up,w->camera);Camera_SetStaticPtr(w->camera);Camera_SetPerspectiveClippingPlane(0,250<<14,w->camera);}
