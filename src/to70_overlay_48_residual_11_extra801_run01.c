#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *camera;u8 p[8];int distance;u16 mode,active;} Motion;
typedef struct {u8 p[0xc004];volatile int a,b;} Work;
extern void Camera_SetDistance(s32,void *);int ov48_02259C78(Motion *,Work *);
int ov48_02259C78(Motion *m,Work *w){if(m->active==0)return 1;switch(m->mode){case 1:if(m->distance>0x58000){m->distance-=0x8000;w->a-=128;w->b=w->a;}else{m->distance=0x50000;m->active=0;}break;case 0:if(m->distance<0x120000){m->distance+=0x8000;w->a+=128;w->b=w->a;}else{m->distance=0x128000;m->active=0;}break;}Camera_SetDistance(m->distance,m->camera);return 0;}
