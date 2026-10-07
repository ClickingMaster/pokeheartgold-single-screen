#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "gf_gfx_planes.h"
typedef struct {enum HeapID heap;u8 pad[4];u16 flag;u8 rest[0x472];SpriteSystem *system;SpriteManager *manager;ManagedSprite *sprites[4];} Context;
extern const OamManagerParam ov67_021E6E18;
extern const OamCharTransferParam ov67_021E6D7C;
extern const SpriteResourceCountsListUnion ov67_021E6D90;
extern const ManagedSpriteTemplate ov67_021E6F50[];
void ov67_021E6820(Context *p);
#pragma force_active on
void ov67_021E6820(Context *p){
 OamManagerParam oam;OamCharTransferParam transfer;SpriteResourceCountsListUnion counts;NARC *narc;u32 i;
 p->system=SpriteSystem_Alloc(p->heap);p->manager=SpriteManager_New(p->system);
 oam=ov67_021E6E18;transfer=ov67_021E6D7C;SpriteSystem_Init(p->system,&oam,&transfer,32);SpriteSystem_InitSprites(p->system,p->manager,4);counts=ov67_021E6D90;SpriteSystem_InitManagerWithCapacities(p->system,p->manager,&counts);
 G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(p->system),0,512<<12);
 narc=NARC_New((NarcId)118,p->heap);
 SpriteSystem_LoadCharResObjFromOpenNarc(p->system,p->manager,narc,0,1,1,0xd158);SpriteSystem_LoadPlttResObjFromOpenNarc(p->system,p->manager,narc,3,0,2,1,0xd158);SpriteSystem_LoadCellResObjFromOpenNarc(p->system,p->manager,narc,1,1,0xd158);SpriteSystem_LoadAnimResObjFromOpenNarc(p->system,p->manager,narc,2,1,0xd158);NARC_Delete(narc);
 for(i=0;i<4;i++)p->sprites[i]=SpriteSystem_NewSprite(p->system,p->manager,&ov67_021E6F50[i]);
 if(p->flag==0){ManagedSprite_SetDrawFlag(p->sprites[2],0);ManagedSprite_SetDrawFlag(p->sprites[3],0);}GfGfx_EngineATogglePlanes(16,1);
}
