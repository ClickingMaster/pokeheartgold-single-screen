#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{void*sprite;int active,vx,vy;s16 x,y;};void ManagedSprite_SetPositionXY(void*,int,int);int ov96_0220B2D4(struct S*);
int ov96_0220B2D4(struct S*s){s->x+=(s->vx>>12);s->vx=(s->vx<<2)/3;s->y-=(s->vy>>12);s->vy-=4;ManagedSprite_SetPositionXY(s->sprite,s->x,s->y);if(s->x<0||s->x>256)return 1;return 0;}
