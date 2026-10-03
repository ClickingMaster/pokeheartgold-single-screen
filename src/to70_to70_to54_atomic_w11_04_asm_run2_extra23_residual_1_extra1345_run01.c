#pragma force_active on
#include "global.h"
typedef struct{u8 raw[0x24];}Motion;
typedef struct{u8 state;u8 pad[3];void*parent;u8 pad2[4];void*system;u8 pad3[0x10];s16 x,y;void*s0,*s1;u32 unused;void*s2;Motion a,b,c;u8 cycle;u8 pad4[3];s16 dy;}Work;
extern void ov07_02222268(Motion*,s16,s16,s16,s16,int);extern int ov07_022222F0(Motion*,void*),ov07_02222314(Motion*,void*);extern void ov07_0221C448(void*,void*),ov07_02232508(void*),ManagedSprite_TickFrame(void*),SpriteSystem_DrawSprites(void*);
void ov07_0222AFAC(void*,Work*);
void ov07_0222AFAC(void*task,Work*w){int finished;switch(w->state){case 0:if(!w->cycle){ov07_02222268(&w->a,w->x,w->x+48,w->y,w->y,3);ov07_02222268(&w->b,w->x,w->x+48,w->y+w->dy,w->y+w->dy,6);ov07_02222268(&w->c,w->x,w->x+48,w->y+w->dy,w->y+w->dy,8);}else{ov07_02222268(&w->a,w->x+48,w->x,w->y,w->y,3);ov07_02222268(&w->b,w->x+48,w->x,w->y+w->dy,w->y+w->dy,6);ov07_02222268(&w->c,w->x+48,w->x,w->y+w->dy,w->y+w->dy,9);}w->cycle++;w->state++;break;case 1:finished=0;if(!ov07_022222F0(&w->b,w->s1))finished++;if(!ov07_02222314(&w->a,w->s0))finished++;if(!ov07_022222F0(&w->c,w->s2))finished++;if(finished>=3){if(w->cycle==1)w->state--;else w->state++;}break;default:ov07_0221C448(w->parent,task);ov07_02232508(w);return;}ManagedSprite_TickFrame(w->s1);SpriteSystem_DrawSprites(w->system);}
