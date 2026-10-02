#pragma force_active on
#include "global.h"
#include "camera.h"
extern VecFx32 ov48_0225B16C,ov48_0225B184;typedef struct{Camera*camera;int x,y;volatile fx32 distance;u16 mode;} Work;

void ov48_02259BC0(Work*w,volatile int flag,int unused,int heap);

void ov48_02259BC0(Work*w,volatile int flag,int unused,int heap){fx32 distance;w->camera=Camera_New((enum HeapID)heap);Camera_Init_FromTargetAndPos(&ov48_0225B184,&ov48_0225B16C,0x5c1,0,0,w->camera);Camera_SetPerspectiveClippingPlane(0,0x64000,w->camera);Camera_ApplyPerspectiveType(0,w->camera);Camera_SetStaticPtr(w->camera);if(((u32)flag<<31>>31)==0){w->mode=1;distance=0x50000;}else{w->mode=0;distance=0x128000;}w->distance=distance;Camera_SetDistance(w->distance,w->camera);}
