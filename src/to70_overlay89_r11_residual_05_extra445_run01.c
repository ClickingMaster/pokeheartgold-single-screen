#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int state;volatile s32 angle;s32 baseX;u8 data[4];} Motion;
typedef struct {u8 pad[0x1c];u8 entity[0x78];Motion motion;} Work;
extern void ModelInstance_GetPosition(void *,s32 *,s32 *,s32 *);extern s32 GF_SinDegFX32(s32);extern void ModelInstance_SetPosition(void *,s32,s32,s32);extern void ov89_0225ADA4(void *,Work *,void *,int);
int ov89_0225B82C(void *,Work *);
int ov89_0225B82C(void *unused,Work *w){s32 x,y,z;Motion *m=&w->motion;switch(m->state){case 0:ModelInstance_GetPosition(w->entity,&x,&y,&z);m->baseX=x;m->state++;case 1:{s32 angle,sine,offset;ModelInstance_GetPosition(w->entity,&x,&y,&z);if(x<-(5<<16)||x>(5<<16)||y>(3<<16)||y<-(3<<16))return 1;angle=m->angle+0x8000;m->angle=angle;if(angle>=(360<<12))m->angle-=360<<12;sine=GF_SinDegFX32(m->angle);offset=(s32)(((s64)sine*32768+2048)>>12);ModelInstance_SetPosition(w->entity,m->baseX+offset,y-4096,z);break;}}ov89_0225ADA4(w->entity,w,m->data,0);return 0;}
