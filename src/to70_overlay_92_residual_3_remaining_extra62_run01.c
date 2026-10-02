#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#pragma force_active on
struct Scene {u8 pad[80];SpriteSystem *system;SpriteManager *manager;};
extern const OamManagerParam ov92_022638EC;
extern const OamCharTransferParam ov92_022638C0;
extern const SpriteResourceCountsListUnion ov92_022638D4;
extern void G2dRenderer_SetSubSurfaceCoords(G2dRenderer *,fx32,fx32);
void ov92_0225D7CC(struct Scene *);
void ov92_0225D7CC(struct Scene *scene) { OamManagerParam oam; OamCharTransferParam transfer; SpriteResourceCountsListUnion counts; scene->system=SpriteSystem_Alloc((enum HeapID)113); oam=ov92_022638EC; transfer=ov92_022638C0; SpriteSystem_Init(scene->system,&oam,&transfer,32); counts=ov92_022638D4; scene->manager=SpriteManager_New(scene->system); if(!SpriteSystem_InitSprites(scene->system,scene->manager,255))GF_AssertFail(); if(!SpriteSystem_InitManagerWithCapacities(scene->system,scene->manager,&counts))GF_AssertFail(); G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(scene->system),0,256<<12); }
