#pragma force_active on
#include "global.h"
typedef struct{int x,y,z;}Vec;typedef struct{u16 x,y,z,w;}Angle;extern Vec ov59_0223CB58;extern void*Camera_New(int);extern void Camera_Init_FromPosDistanceAndAngle(Vec*,int,Angle*,int,int,void*),Camera_SetPerspectiveClippingPlane(int,int,void*),Camera_ClearFixedTarget(void*),Camera_SetStaticPtr(void*);typedef struct{u32 a,b;void*camera;}Work;
void AprijuiceScene_InitCamera(Work*w,int heap);
void AprijuiceScene_InitCamera(Work*w,int heap){Vec pos=ov59_0223CB58;Angle angle={0,0,0,0};w->camera=Camera_New(heap);Camera_Init_FromPosDistanceAndAngle(&pos,65536,&angle,1473,1,w->camera);Camera_SetPerspectiveClippingPlane(0,409600,w->camera);Camera_ClearFixedTarget(w->camera);Camera_SetStaticPtr(w->camera);}
