#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[12];void *data;} Pal;
typedef struct {u8 pad[0x6a0];Pal *palette;} Work;
extern void GXS_LoadOBJPltt(const void*,u32,u32);

void BagApp_LoadPocketObjPalette(Work *w,int i);

extern void ManagedSprite_SetDrawFlag(void*,int);
typedef struct{u8 p[0x2a4];void *sprites[6];void *last;}SpriteWork;
void BagApp_ShowMoveSlotSprites(SpriteWork*,int,int,int);
void BagApp_ShowMoveSlotSprites(SpriteWork*w,int count,int previous,int hide){int i;SpriteWork*cursor;if(count==0){cursor=w;for(i=0;i<6;i++,cursor=(SpriteWork*)((u8*)cursor+4))ManagedSprite_SetDrawFlag(cursor->sprites[0],0);ManagedSprite_SetDrawFlag(w->last,0);return;}i=0;cursor=w;do{if(i<count)ManagedSprite_SetDrawFlag(cursor->sprites[0],1);else ManagedSprite_SetDrawFlag(cursor->sprites[0],0);i++;cursor=(SpriteWork*)((u8*)cursor+4);}while(i<6);if(previous>=0){previous+=21;ManagedSprite_SetDrawFlag(*(void**)((u8*)w+0x250+previous*4),0);}if(hide)ManagedSprite_SetDrawFlag(w->last,0);}
