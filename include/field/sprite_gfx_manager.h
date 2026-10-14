#ifndef POKEHEARTGOLD_FIELD_SPRITE_GFX_MANAGER_H
#define POKEHEARTGOLD_FIELD_SPRITE_GFX_MANAGER_H

#include "filesystem.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// Sprite list plus char / palette / cell / animation resources of the overlay 2 field animations (cut-ins), on the
// main screen. Resources are looked up by id; a slot whose id is the kind's `none` id is free.

typedef struct SpriteGfxResource {
    s16 id;
    s16 unused;
    SpriteResource *res;
} SpriteGfxResource;

typedef struct SpriteGfxManager {
    u8 charCount;
    u8 plttCount;
    u8 cellCount;
    u8 animCount;
    s8 charNone;
    s8 plttNone;
    s8 cellNone;
    s8 animNone;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *animMan;
    SpriteGfxResource *charRes;
    SpriteGfxResource *plttRes;
    SpriteGfxResource *cellRes;
    SpriteGfxResource *animRes;
} SpriteGfxManager;

void SpriteGfxManager_Init(SpriteGfxManager *manager, int numSprites, int numChar, int numPltt, int numCell, int numAnim, int charNone, int plttNone, int cellNone, int animNone);
void SpriteGfxManager_Free(SpriteGfxManager *manager);
void SpriteGfxManager_LoadChar(SpriteGfxManager *manager, NARC *narc, int fileId, int id);
void SpriteGfxManager_TransferChar(SpriteGfxManager *manager, int id);
void SpriteGfxManager_ReleaseCharData(SpriteGfxManager *manager, int id);
void SpriteGfxManager_LoadPltt(SpriteGfxManager *manager, NARC *narc, int fileId, int id);
void SpriteGfxManager_TransferPltt(SpriteGfxManager *manager, int id);
void SpriteGfxManager_ReleasePlttData(SpriteGfxManager *manager, int id);
void SpriteGfxManager_LoadCell(SpriteGfxManager *manager, NARC *narc, int fileId, int id);
void SpriteGfxManager_LoadAnim(SpriteGfxManager *manager, NARC *narc, int fileId, int id);
Sprite *SpriteGfxManager_CreateSprite(SpriteGfxManager *manager, const VecFx32 *pos, int charId, int plttId, int cellId, int animId, int priority, u32 drawPriority);
void Sprite_CopyPositionVector(Sprite *sprite, VecFx32 *pos);

#endif // POKEHEARTGOLD_FIELD_SPRITE_GFX_MANAGER_H
