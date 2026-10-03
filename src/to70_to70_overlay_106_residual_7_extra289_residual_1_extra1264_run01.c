#pragma force_active on
#include "global.h"
#include "sprite_system.h"
extern void GfGfx_EngineATogglePlanes(int,int);
typedef struct{void*unused;NARC*narc;SpriteSystem*system;SpriteManager*manager;ManagedSprite*sprite;}Sub;
extern OamManagerParam ov106_021E6FC8;extern OamCharTransferParam ov106_021E6F48;extern SpriteResourceCountsListUnion ov106_021E6F5C;extern ManagedSpriteTemplate ov106_021E7010;
extern void ov106_021E66FC(void*);
void ov106_021E6520(void*);
void ov106_021E6520(void*w){Sub*s=*(Sub**)((u8*)w+0x418);OamManagerParam oam;OamCharTransferParam chars;SpriteResourceCountsListUnion counts;
 s->system=SpriteSystem_Alloc((enum HeapID)153);s->manager=SpriteManager_New(s->system);oam=ov106_021E6FC8;chars=ov106_021E6F48;SpriteSystem_Init(s->system,&oam,&chars,32);counts=ov106_021E6F5C;SpriteSystem_InitSprites(s->system,s->manager,1);SpriteSystem_InitManagerWithCapacities(s->system,s->manager,&counts);G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(s->system),0,1<<22);
 SpriteSystem_LoadCellResObjFromOpenNarc(s->system,s->manager,s->narc,205,1,51433);SpriteSystem_LoadAnimResObjFromOpenNarc(s->system,s->manager,s->narc,206,1,51433);SpriteSystem_LoadPlttResObjFromOpenNarc(s->system,s->manager,s->narc,207,0,1,1,51433);SpriteSystem_LoadCharResObjFromOpenNarc(s->system,s->manager,s->narc,204,1,1,51433);
 s->sprite=SpriteSystem_NewSprite(s->system,s->manager,&ov106_021E7010);ManagedSprite_SetDrawFlag(s->sprite,0);ov106_021E66FC(w);GfGfx_EngineATogglePlanes(16,1);}
