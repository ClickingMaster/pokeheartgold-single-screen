#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_OBJECTS_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_OBJECTS_H

#include "global.h"

#include "pokeathlon/pokeathlon_course_types_def.h"

#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"

// Helpers of the course Pokemon objects (position conversions, collision circle) and the
// sprite pool the events use for their own sprites.

// Sprite pool: resources loaded from NARCs into four resource managers, sprites created from
// them up front and handed out by resource id and kind.

typedef struct PokeathlonCourseSpritePoolResources {
    u32 id;
    SpriteResource *objects[4]; // char, palette, cell, animation
} PokeathlonCourseSpritePoolResources;

struct PokeathlonCourseSpritePoolSprite {
    u16 taken;
    u16 shown;
    Sprite *sprite;
    u16 kind;
    u16 resId;
};

struct PokeathlonCourseSpritePool {
    enum HeapID heapId;
    int count;
    PokeathlonCourseSpritePoolTemplate template;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    PokeathlonCourseSpritePoolResources *resources;
    PokeathlonCourseSpritePoolSprite *sprites;
};

void PokeathlonCourseObject_SetTouchRadius(PokeathlonCourseMonObject *object, s32 radius);
void PokeathlonCourseObject_SetCollision(PokeathlonCourseMonObject *object, s32 offsetY, s32 radius);
void PokeathlonCourseObject_GetCollisionCircle(PokeathlonCourseMonObject *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY, s32 *radius);
s32 PokeathlonCourseObject_GetCollisionRadius(PokeathlonCourseMonObject *object);
s32 PokeathlonCourseObject_GetCollisionOffsetY(PokeathlonCourseMonObject *object);
void PokeathlonCourseObject_SetGroundPosition(PokeathlonCourseMonObject *object, s32 x, s32 y);
void PokeathlonCourseObject_SetCenterPosition(PokeathlonCourseMonObject *object, int x, int y, int moveShadow);
void PokeathlonCourseObject_GetCollisionCenter(PokeathlonCourseMonObject *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY);
void PokeathlonCourseObject_CenterToGround(PokeathlonCourseMonObject *object, int x, int y, int *outX, int *outY);
void PokeathlonCourseObject_GroundToCenter(PokeathlonCourseMonObject *object, s32 x, s32 y, s32 *outX, s32 *outY);
BOOL PokeathlonCourseObject_IsInTouchRange(PokeathlonCourseMonObject *object, fx32 x, fx32 y, s32 targetX, s32 targetY);
void PokeathlonCourseObject_SetScale(PokeathlonCourseMonObject *object, f32 scaleX, f32 scaleY);
BOOL PokeathlonCourseObject_IsDrawn(PokeathlonCourseMonObject *object);
void PokeathlonCourseObjects_ShiftLargeUp(PokeathlonCourseMonObjects *objects);
void PokeathlonCourseObjects_PauseAnims(PokeathlonCourseMonObjects *objects, BOOL paused);
PokeathlonCourseSpritePool *PokeathlonCourseSpritePool_New(enum HeapID heapId, PokeathlonCourseSpritePoolTemplate *template);
void PokeathlonCourseSpritePool_Free(PokeathlonCourseSpritePool *pool);
void PokeathlonCourseSpritePool_SetResourceId(PokeathlonCourseSpritePool *pool, int index, u32 id);
void PokeathlonCourseSpritePool_LoadChar(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id, int vram);
void PokeathlonCourseSpritePool_LoadPltt(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id, int vram, u8 count);
void PokeathlonCourseSpritePool_LoadCell(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id);
void PokeathlonCourseSpritePool_LoadAnim(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id);
void PokeathlonCourseSpritePool_TransferResources(PokeathlonCourseSpritePool *pool);
PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_CreateAndTake(PokeathlonCourseSpritePool *pool, int priority, NNS_G2D_VRAM_TYPE whichScreen, u16 resId, u8 kind);
Sprite *PokeathlonCourseSpritePool_CreateSprite(PokeathlonCourseSpritePool *pool, int priority, NNS_G2D_VRAM_TYPE whichScreen, u16 resId, u8 kind);
PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_Take(PokeathlonCourseSpritePool *pool, int resId, int kind);
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonCourseSpritePoolSprite *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
void PokeathlonCourseSpritePoolSprite_TryChangeAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonCourseSpritePoolSprite *entry, const VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePoolSprite_SetScale(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *scale, u8 affineType);
void PokeathlonCourseSpritePoolSprite_SetRotation(PokeathlonCourseSpritePoolSprite *entry, u16 rotation, u8 affineType);
Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePool_Draw(PokeathlonCourseSpritePool *pool);
void PokeathlonCourseSpritePool_SetSurfaceCoords(PokeathlonCourseSpritePool *pool, fx32 mainX, fx32 mainY, fx32 subX, fx32 subY);
SpriteList *PokeathlonCourseSpritePool_GetSpriteList(PokeathlonCourseSpritePool *pool);
SpriteResource **PokeathlonCourseSpritePool_GetResourceSlot(PokeathlonCourseSpritePool *pool, u32 id, u32 type);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite *entry, int priority);
void PokeathlonCourseSpritePool_PauseAnims(PokeathlonCourseSpritePool *pool, BOOL paused);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_OBJECTS_H
