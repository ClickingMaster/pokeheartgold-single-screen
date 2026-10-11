#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x2c];fx32 x,y,z;} Scene;
typedef struct{u8 p[0x7c];u8 transform[0x7c];Scene *scene;} Work;
extern int ov91_0225E9AC(Scene *),ov91_0225E990(Scene *);
extern void ModelInstance_SetPosition(void *,fx32,fx32,fx32),ModelInstance_SetScale(void *,fx32,fx32,fx32);
void ov91_02260950(Work *);
void ov91_02260950(Work *w){fx32 base,delta,scale,value;fx32 factor=4096;if(ov91_0225E9AC(w->scene)==1)base=(fx32)0xffed4000;else if(ov91_0225E990(w->scene)==0)base=(fx32)0xfffcd000;else base=(fx32)0xffed4000;ModelInstance_SetPosition(w->transform,w->scene->x,base,w->scene->z);delta=w->scene->y-base;value=(fx32)(((s64)delta*factor+factor/2)>>12);scale=FX_Div(value,0x12c000)+4096;ModelInstance_SetScale(w->transform,scale,scale,scale);value=(fx32)(((s64)delta*65536+2048)>>12);value=FX_Div(value,0x12c000);NNS_G3dGlbPolygonAttr(0,(GXPolygonMode)0,(GXCull)0,0,24-(value>>12),0);}
