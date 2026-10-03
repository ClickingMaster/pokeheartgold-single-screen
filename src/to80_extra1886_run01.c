#pragma force_active on
#include "global.h"
#include "camera.h"
typedef struct{fx32 distance;int perspective;VecFx32 target;CameraAngle angle;Camera*camera;} View;

void ov85_021E7424(void*w);

#pragma force_active on
void ov85_021E7424(void*w){View*v=(View*)((u8*)w+0xd0c);VecFx32 up;v->camera=Camera_New((enum HeapID)102);v->target.x=0;v->target.y=0;v->target.z=0;v->angle.x=0xe93f;v->angle.y=0;v->angle.z=0;((View*)((u8*)w+0xd0c))->distance=0x143000;v->perspective=0x444;Camera_Init_FromTargetDistanceAndAngle(&v->target,v->distance,&v->angle,(u16)v->perspective,0,1,v->camera);up.x=0;up.y=4096;up.z=0;Camera_SetLookAtCamUp(&up,v->camera);Camera_SetStaticPtr(v->camera);}
