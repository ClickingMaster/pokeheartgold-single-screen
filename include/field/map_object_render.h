#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_H

#include "map_object.h"
#include "player_avatar.h"
#include "sprite.h"

// The billboard renderer of the map objects: the resource caches behind every map object's billboard and the
// queue that loads them over several frames. It lives inside MapObjectManager (sub_0205F1A0).
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
u32 ov01_021FA464(u32);

#endif // POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_H
