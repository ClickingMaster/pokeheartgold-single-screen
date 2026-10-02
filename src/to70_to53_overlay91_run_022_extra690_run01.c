#pragma force_active on
#include "global.h"
typedef struct {void *sprites[3];int state[3];VecFx32 pos[3];int same;} Work;
typedef struct{u32 a,b;VecFx32 pos;u8 tail[64];} Template;
extern void ov91_0225D4D4(void *,int,int,int *,int *),Sprite_SetAnimCtrlSeq(void *,int),Sprite_SetDrawFlag(void *,int),Sprite_SetAnimActiveFlag(void *,int),Sprite_SetAnimSpeed(void *,int);
extern void *Sprite_CreateAffine(Template *);
void ov91_0225D6B8(Work *,Template *,int,void *,int,int);
void ov91_0225D6B8(Work *w,Template *t,int ignored,void *arg,int a,int b){int i;struct {int y,x;} coord;for(i=0;i<3;i++){ov91_0225D4D4(arg,b,a,&coord.x,&coord.y);t->pos.x=coord.x<<12;t->pos.y=coord.y<<12;w->pos[i]=t->pos;w->sprites[i]=Sprite_CreateAffine(t);Sprite_SetAnimCtrlSeq(w->sprites[i],2);Sprite_SetDrawFlag(w->sprites[i],0);Sprite_SetAnimActiveFlag(w->sprites[i],1);Sprite_SetAnimSpeed(w->sprites[i],6<<10);w->state[i]=0;}if(b==a)w->same=1;else w->same=0;}
