#pragma force_active on
#include "global.h"
typedef struct{u8 a[40];}Motion;
typedef struct{u8 pad[0xc];u8 entities[0x780];u8 pad2[0x1cc];volatile s8 timer[16];Motion motion[16];}Work;
extern int ov49_02265434(void*,int);extern void ov49_022655E0(void*,int*,int*,int*),ModelInstance_SetPosition(void*,int,int,int),ov49_02265B14(void*,Work*,int,int);
int ov49_02267A1C(void*,Work*,int);
int ov49_02267A1C(void*p,Work*w,int i){struct{int x,y,z;}v;int r;int offset;w->timer[i]++;offset=i;offset*=40;r=ov49_02265434((u8*)w+0x968+offset,w->timer[i]);ov49_022655E0((void*)(0x968+(u32)w+offset),&v.x,&v.y,&v.z);ModelInstance_SetPosition(w->entities+i*120,v.x,v.y,v.z);ov49_02265B14(p,w,i,0);return r;}
