#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad0[8];VecFx32 position,next,direction;u8 pad2c[13];u8 a,pad3a,b,c,d,e,pad3f,speed,f;} Work;
extern const VecFx32 ov96_0221D268,ov96_0221D280;
void ov96_02212F94(Work*,const Work*,int);
void ov96_02212F94(Work*w,const Work*s,int side){struct{volatile int side;VecFx32 right,left,zero;}frame;frame.side=side;memset(&frame.zero,0,sizeof frame.zero);if(w->speed<=3){if(s->speed>3){w->a=s->a;w->c=s->c;w->d=s->d;w->e=s->e;w->speed=s->speed;w->f=s->f;}else{w->a=0;w->c=0;w->d=30;w->e=1;w->speed=3;w->f=0;}}else{w->a=0;w->c=0;w->d=30;w->e=1;w->speed=3;w->f=0;}VEC_Subtract(&w->position,&s->position,&w->direction);if(VEC_Mag(&w->direction)==0){frame.left=ov96_0221D268;frame.right=ov96_0221D280;if(frame.side==0)w->direction=frame.left;else w->direction=frame.right;}VEC_Normalize(&w->direction,&w->direction);VEC_MultAdd(w->speed<<12,&w->direction,(const VecFx32*)&frame.zero,&w->next);if(s->b<12)w->b=s->b;}
