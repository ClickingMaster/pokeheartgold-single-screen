#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct{u8 p[0x44];SpriteList *list;GF_2DGfxResMan *a,*b,*c,*d;} Work;
Sprite *ov41_02246280(Work *w,int id,int x,int y,u32 priority,NNS_G2D_VRAM_TYPE screen);
Sprite *ov41_02246280(Work *w,int id,int x,int y,u32 priority,NNS_G2D_VRAM_TYPE screen){SpriteResourcesHeader h;SimpleSpriteTemplate t;CreateSpriteResourcesHeader(&h,id,id,id,id,-1,-1,0,0,w->a,w->b,w->c,w->d,0,0);t.spriteList=w->list;t.header=&h;t.position.x=x<<12;t.position.y=y<<12;t.position.z=0;t.priority=priority;t.whichScreen=screen;t.heapID=(enum HeapID)14;return Sprite_Create(&t);}
