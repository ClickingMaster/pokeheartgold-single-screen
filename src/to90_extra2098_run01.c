#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
extern void ov07_02222338(void *,void *,int,int,int,int,int,u32),ManagedSprite_SetOamMode(void *,int),ov07_02231E08(void *,int,int),Sprite_DeleteAndFreeResources(void *),ov07_0221C448(void *,void *),Heap_Free(void *),ManagedSprite_TickFrame(void *),SpriteSystem_DrawSprites(void *);extern int ov07_022223CC(void *,void *,void *),ManagedSprite_IsAnimated(void *);
void ov07_02226B2C(void *,u8 *);
void ov07_02226B2C(void *owner,u8 *p){switch(p[0]){case 0:ov07_02222338(p+0x24,p+0x48,-30,112,160,112,21,1<<18);p[0]++;break;case 1:if(!ov07_022223CC(p+0x24,p+0x48,P(p,28)))p[0]++;break;case 2:if(!ManagedSprite_IsAnimated(P(p,28))){ManagedSprite_SetOamMode(P(p,28),1);p[32]=16;p[33]=0;ov07_02231E08(P(p,4),p[32],p[33]);p[0]++;}break;case 3:if(p[32])p[32]--;if(p[33]<16)p[33]++;*(vu16 *)0x04000052=p[32]|(p[33]<<8);if(!p[32])p[0]++;break;default:Sprite_DeleteAndFreeResources(P(p,28));ov07_0221C448(P(p,4),owner);Heap_Free(p);return;}ManagedSprite_TickFrame(P(p,28));SpriteSystem_DrawSprites(P(p,16));}
