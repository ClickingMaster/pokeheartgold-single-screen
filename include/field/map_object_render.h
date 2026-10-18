#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_H

#include "map_object.h"
#include "player_avatar.h"
#include "sprite.h"

// The billboard renderer of the map objects: the resource caches behind every map object's billboard and the
// queue that loads them over several frames. It lives inside MapObjectManager (MapObjectManager_GetRender).
typedef struct MapObjectRender MapObjectRender;

void MapObjectRender_ReleaseUnused(MapObjectManager *manager, MapObjectRender *render);
void *MapObjectRender_GetBillboards(MapObjectRender *render);
BOOL MapObject_IsBillboardPending(LocalMapObject *object);
void MapObject_ChangeBillboardSprite(LocalMapObject *object, u32 spriteId);
// Returns 0; the callers ignore it.
int MapObject_UpdateBillboardPosition(LocalMapObject *object, Sprite *billboard);
void MapObjectRender_ReleaseSpriteResources(MapObjectManager *manager, u32 spriteId, LocalMapObject *skip);
void MapObject_CreateBillboard(LocalMapObject *object, void *work);
void MapObject_DeleteBillboard(LocalMapObject *object, void *work);
u32 Facing_GetBillboardStandSequence(u32);
u32 Facing_GetBillboardWalkSequence(u32);
u32 MapObjectRender_GetFacingSequence(u32);

void MapObjectRender_Init(MapObjectRender *render, MapObjectManager *manager, u32 count, u32 priority, u32 textureCount, u32 residentTextureCount, const u32 *textureIds, u32 loadsPerFrame);
void MapObjectRender_Free(MapObjectRender *render);
u8 MapObjectRender_GetSpriteModelId(u32 spriteId);
void MapObject_SetBillboardPending(LocalMapObject *object, BOOL set);

#endif // POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_H
