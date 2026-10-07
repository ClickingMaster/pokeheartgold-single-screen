#pragma force_active on
#include "global.h"
typedef struct{int x,y,z;} Vec;typedef struct{u8 p[0x14];int state;u8 q[0x34];int min,max;} Parent;typedef struct{u8 p[8];void *sprite;int q,mode;Parent *parent;int speed;} Work;
extern void sub_02068DB8(void *,Vec *),sub_02068DA8(void *,Vec *),Sprite_SetMatrix(void *,Vec *),Sprite_SetDrawFlag(void *,int);void ov02_0224AAD4(void *,Work *);

// @fn ov02_0224AAD4
void ov02_0224AAD4(void *object,Work *w){Vec pos;sub_02068DB8(object,&pos);pos.x+=w->speed;pos.x%=1<<21;sub_02068DA8(object,&pos);Sprite_SetMatrix(w->sprite,&pos);
if(w->mode==1){Parent *p=w->parent;if(p->state==2){int visible=0;int min=p->min,max=p->max;int lo=pos.y-8192;if(lo>=min&&lo<=max&&pos.y+8192>=min&&pos.y+8192<=max)visible=1;Sprite_SetDrawFlag(w->sprite,visible);}else if(p->state==1)Sprite_SetDrawFlag(w->sprite,0);}
}
