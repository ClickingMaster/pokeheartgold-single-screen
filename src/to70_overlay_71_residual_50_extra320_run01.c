#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{int unused,state;void*object;int timer;int gap;void*sprite;int angle,delta,count;VecFx32 pos;int velocity;}W;
extern void ov71_0224BA64(W*),ov71_02247708(void*,int),ov71_0224BAA0(W*,int,int),ov71_022476C4(void*,const VecFx32*),PlaySE(int),ov71_0224B910(void*,int,int,int),ov71_0224BA48(void*);extern int ov71_0224B960(void*);void ov71_0224BABC(void*,W*);
void ov71_0224BABC(void*task,W*w){ov71_0224BA64(w);switch(w->state){case 0:if(w->count){w->angle+=w->delta;w->count--;ov71_02247708(w->sprite,w->angle>>12);}else{ov71_02247708(w->sprite,31);w->state++;}break;case 1:ov71_0224BAA0(w,384,30);w->timer=0;w->state++;break;case 2:w->timer++;if(w->timer>10){w->velocity=0;w->state++;}break;case 3:w->pos.y+=w->velocity;w->velocity-=1920;ov71_022476C4(w->sprite,&w->pos);if(w->pos.y<-45056){PlaySE(1510);w->velocity*=-1;ov71_0224B910(w->object,0,16,8);w->state++;}break;case 4:w->pos.y+=w->velocity;if(w->pos.y>=77824)w->pos.y=77824;ov71_022476C4(w->sprite,&w->pos);if(w->pos.y==77824)w->state++;break;case 5:if(ov71_0224B960(w->object))ov71_0224BA48(task);break;}}
