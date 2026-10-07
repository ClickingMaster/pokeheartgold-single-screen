#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Params { int pad,force,speed; } Params;
typedef struct Work { int pad[2]; Params *params; int active; VecFx32 previous,position,direction,velocity; struct { u32 low:20; u32 angle:4; u32 p24:2; u32 damping:1; u32 extra:1; u32 moving:1; u32 rest:3; } flags; } Work;
extern void ov96_0220E960(VecFx32*,int,int);
extern int ov96_0220D6CC(VecFx32*);
void ov96_0220D744(Work*);
#pragma opt_propagation off
void ov96_0220D744(Work*w) {
 VecFx32 delta,normal,movement,zero2,zero1,deceleration,force,zero3;
 int magnitude;
 if(!w->active) return;
 VEC_Subtract(&w->previous,&w->position,&delta); magnitude=VEC_Mag(&delta); w->flags.moving=magnitude>0;
 if(w->flags.damping) {
 {int zero=0;volatile VecFx32 *p=&zero1;p->x=zero;volatile VecFx32 *q=&zero2;p->y=zero;q->x=zero;p->z=zero;q->y=zero;q->z=zero;}
 VEC_Normalize(&w->velocity,&zero2); VEC_MultAdd(0xccd,&zero2,&zero1,&deceleration);
 deceleration.x=deceleration.x<0?-deceleration.x:deceleration.x; deceleration.y=deceleration.y<0?-deceleration.y:deceleration.y;
 if(w->flags.extra) {
 int scale=w->params->force/100;
 VEC_Normalize(&delta,&normal); VEC_MultAdd(scale,&w->direction,&zero1,&force);
 deceleration.x+=force.x<0?-force.x:force.x; deceleration.y+=force.y<0?-force.y:force.y;
 }
 ov96_0220E960(&w->velocity,deceleration.x,deceleration.y); VEC_Add(&w->position,&w->velocity,&w->position); w->previous=w->position;
 if(w->velocity.x||w->velocity.y)return;w->flags.damping=0;
 }
 if(w->flags.moving) {
 VEC_Normalize(&delta,&normal); if(!w->flags.damping) w->flags.angle=ov96_0220D6CC(&normal);
 int speed=w->params->speed; if(magnitude<=speed) movement=delta;
 else { volatile VecFx32 *z=&zero3;z->x=0;z->y=0;z->z=0; VEC_MultAdd(speed,&normal,&zero3,&movement); }
 VEC_Add(&w->position,&movement,&w->position); w->direction=normal;
 }
}
