#pragma force_active on
#include "global.h"
typedef struct{volatile u8 state,timer;u16 pad;void*task,*sys,*a,*b;}Work;
extern void ManagedSprite_SetDrawFlag(void*,int),ManagedSprite_SetOamMode(void*,int),ManagedSprite_TickFrame(void*),SpriteSystem_DrawSprites(void*),ov07_0221C448(void*,void*),Heap_Free(void*);
void ov07_02228684(void*,Work*);
void ov07_02228684(void*p,Work*w){switch(w->state){case 0:w->timer++;if(w->timer>=10){w->timer=0;w->state++;}break;case 1:ManagedSprite_SetDrawFlag(w->a,1);ManagedSprite_SetDrawFlag(w->b,1);ManagedSprite_SetOamMode(w->b,2);w->state++;break;case 2:w->timer++;if(w->timer>=10){*(volatile u16*)0x0400004a=(*(volatile u16*)0x0400004a&~63)|31;*(volatile u16*)0x0400004a=(*(volatile u16*)0x0400004a&~0x3f00)|(31<<8);ManagedSprite_SetDrawFlag(w->a,0);ManagedSprite_SetDrawFlag(w->b,0);w->timer=0;w->state++;}break;default:*(volatile u32*)0x04000000&=0xffff1fff;ov07_0221C448(w->task,p);Heap_Free(w);return;}ManagedSprite_TickFrame(w->a);ManagedSprite_TickFrame(w->b);SpriteSystem_DrawSprites(w->sys);}
