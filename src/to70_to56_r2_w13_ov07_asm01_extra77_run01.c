#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *owner,*pad4,*sys;int state;void *sprite;u8 scale[0x24];int angle;u8 rotation[0x10];int count,last,sign;} Work;
extern int ov07_02222558(void *),ov07_02222440(void *);
extern void ov07_02222644(void *,float *,float *),ManagedSprite_SetAffineScale(void *,float,float),ov07_02222508(void *,int,int,int,int),ManagedSprite_SetAffineZRotation(void *,u16),ov07_022223F0(void *,int,int,int),Sprite_DeleteAndFreeResources(void *),ov07_0221C448(void *,void *),Heap_Free(void *),SpriteSystem_DrawSprites(void *);
void ov07_0222D2B0(void *,Work *);
void ov07_0222D2B0(void *task,Work *w){float x,y;int active;switch(w->state){case 0:if(ov07_02222558(w->scale)==1){ov07_02222644(w->scale,&x,&y);ManagedSprite_SetAffineScale(w->sprite,x*(float)w->sign,y);}else{w->state++;w->count=5;ov07_02222508(w->scale,10,10,1,8);}break;case 1:active=ov07_02222440(&w->angle);ManagedSprite_SetAffineZRotation(w->sprite,w->angle);if(!active){if(w->count>0){int current,last;w->count--;current=w->angle;last=w->last;w->last=current;ov07_022223F0(&w->angle,current,last,4);}else w->state++;}break;case 2:if(ov07_02222558(w->scale)==1){ov07_02222644(w->scale,&x,&y);ManagedSprite_SetAffineScale(w->sprite,x*(float)w->sign,y);}else w->state++;break;case 3:Sprite_DeleteAndFreeResources(w->sprite);ov07_0221C448(w->owner,task);Heap_Free(w);return;}SpriteSystem_DrawSprites(w->sys);}
