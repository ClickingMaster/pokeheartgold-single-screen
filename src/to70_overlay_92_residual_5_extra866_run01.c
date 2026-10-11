#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x78];int pos[3];u8 gap[0x1e0-0x84];int x,y,z,a,b,c,d,e,f;} Work;extern void ModelInstance_Init(void*,int*),ModelInstance_SetPosition(void*,int,int,int),ModelInstance_SetScale(void*,int,int,int),ModelInstance_SetVisible(void*,int);

void ov92_0225D9B4(Work*w,int mode);

void ov92_0225D9B4(Work*w,int mode){ModelInstance_Init(w,w->pos);ModelInstance_SetPosition(w,0,-0x14000,0);if(mode==0)ModelInstance_SetScale(w,0x1000,0x1000,0x1000);else ModelInstance_SetScale(w,0x1800,0x1800,0x1800);ModelInstance_SetVisible(w,1);w->x=0;w->y=-0x14000;w->z=0;w->e=0;w->f=0;w->a=0;w->b=1;}
