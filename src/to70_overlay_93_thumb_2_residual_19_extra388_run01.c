#pragma force_active on
#include "global.h"
extern void ManagedSprite_GetPositionXYWithSubscreenOffset(void *,s16 *,s16 *,int);
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *,s16,s16,int);
extern void ManagedSprite_SetAnim(void *,int),ManagedSprite_SetOamMode(void *,int),ManagedSprite_SetDrawFlag(void *,int),ManagedSprite_AddSpritePrecisePositionXY(void *,int,int),Sprite_TickFrame(void *);
extern s32 LCRandom(void);
typedef struct {void *sprites[2];s32 x[2],y[2];u8 active,timer;} State;

void ov93_0226249C(void *a,void **b,State *s);
void ov93_02262540(void *a,void *b,State *s);

void ov93_0226249C(void *a,void **b,State *s){s16 xy[2];int i;ManagedSprite_GetPositionXYWithSubscreenOffset(*b,&xy[1],&xy[0],0x160000);for(i=0;i<2;i++){ManagedSprite_SetPositionXYWithSubscreenOffset(s->sprites[i],xy[1],xy[0]+28,0x160000);ManagedSprite_SetAnim(s->sprites[i],27);ManagedSprite_SetOamMode(s->sprites[i],1);ManagedSprite_SetDrawFlag(s->sprites[i],1);s->x[i]=LCRandom()%4096+4096;s->y[i]=LCRandom()%1024+1024;if(i&1)s->x[i]*=-1;}s->timer=0;s->active=1;}
void ov93_02262540(void *a,void *b,State *s){int i;switch(s->active){case 0:break;case 1:for(i=0;i<2;i++){ManagedSprite_AddSpritePrecisePositionXY(s->sprites[i],s->x[i],-s->y[i]);Sprite_TickFrame(*(void **)s->sprites[i]);}s->timer++;if(s->timer>8){for(i=0;i<2;i++)ManagedSprite_SetDrawFlag(s->sprites[i],0);s->active=0;}break;}}
