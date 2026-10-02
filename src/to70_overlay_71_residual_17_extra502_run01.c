#pragma force_active on
#include "global.h"
typedef struct {void *task,*sprite; s32 x,y,x2; s16 a,b,c; u16 pad; s32 speed,count,done,ticks; volatile s16 step;} Work;
extern void ov71_022476EC(void *,void *);
extern void ov71_022476C4(void *,void *);
extern void ov71_022481EC(void *);
extern void PlaySE(int);
void ov71_0224820C(void *,Work *);
void ov71_0224820C(void *task,Work *w){
 if(!w->done){w->y+=w->speed;if(w->speed>=0){w->speed-=1920;}else{w->speed-=1920;if(w->y<=-16384){w->y=-16384;w->speed=-(w->speed*44/100);if(w->speed<4000)w->done=1;PlaySE(1510);w->count++;switch(w->count){case 1:w->step=176;break;case 3:w->step+=80;break;}}}}
 w->a+=w->step;w->c-=w->step;ov71_022476EC(w->sprite,&w->a);w->x+=w->step*5;w->x2+=w->step*5;ov71_022476C4(w->sprite,&w->x);if(w->done){w->step-=14;w->ticks++;if(w->ticks>30)ov71_022481EC(w->task);}
}
