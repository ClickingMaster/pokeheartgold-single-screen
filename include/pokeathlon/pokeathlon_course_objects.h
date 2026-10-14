#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_OBJECTS_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_OBJECTS_H

#include "global.h"

#include "sprite.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// Helpers of the course Pokemon objects (position conversions, collision circle) and the
// sprite pool the events use for their own sprites.

// A Pokemon on a course screen: its sprite plus a shadow and a player marker (0x44). Created
// by the course sprite factory (PokeathlonCourseMonObjects_Load); this is the part of its layout the helpers
// below use.
typedef struct PokeathlonCourseObject {
    void *gfx;            // 0x00: the loaded Pokemon sprite (PokeathlonCourseMonGfx_GetSprite gives its Sprite)
    Sprite *shadowSprite; // 0x04
    Sprite *markerSprite; // 0x08
    s32 markerOffsetY;    // 0x0C
    s32 shadowOffsetY;    // 0x10: rows between the frame bottom and the ground point
    s32 halfWidth;        // 0x14
    s32 halfHeight;       // 0x18
    s32 touchRadius;      // 0x1C
    s32 x;                // 0x20: centre
    s32 y;                // 0x24
    s32 topRow;           // 0x28: first non-transparent pixel row of the frame
    s32 collisionOffsetY; // 0x2C: centre of the collision circle, rows above the frame bottom
    s32 collisionRadius;  // 0x30
} PokeathlonCourseObject;

// What the course sprite factory needs to know about a Pokemon (0x10).
typedef struct PokeathlonCourseObjectInfo {
    u16 species;     // 0x00
    u16 form;        // 0x02
    u16 isLarge;     // 0x04
    u8 shiny;        // 0x06
    u8 gender;       // 0x07
    int participant; // 0x08
    u32 personality; // 0x0C
} PokeathlonCourseObjectInfo;

// How the course sprite factory sets up the Pokemon objects.
typedef struct PokeathlonCourseObjectsTemplate {
    BOOL loadAtOnce;               // 0x00
    int priority;                  // 0x04
    BOOL vramTransfer;             // 0x08
    int vram;                      // 0x0C: 1 main, 2 sub, 3 both
    NNS_G2D_VRAM_TYPE whichScreen; // 0x10
    int sizeTypes[12];             // 0x14
} PokeathlonCourseObjectsTemplate;

// The Pokemon objects of a course screen (only the field the helpers use).
typedef struct PokeathlonCourseObjects {
    enum HeapID heapId; // 0x00
    int count;          // 0x04
    u8 unk08[0xC];      // 0x08
    void *gfxManager;   // 0x14
} PokeathlonCourseObjects;

// Course object code (OVY_96, not yet tidied).
void *PokeathlonCourseMonGfx_GetSprite(void *gfx);
BOOL PokeathlonCourseMonGfx_IsShiftedUp(void *gfx);
void PokeathlonCourseMonGfx_SetScale(void *gfx, void *scale, void *y);
void PokeathlonCourseMonGfx_ShiftLargeUp(void *gfxManager);
PokeathlonCourseObject *PokeathlonCourseMonObjects_Get(void *objects, u8 index);
void *PokeathlonCourseMonObject_GetGfx(void *object);
void PokeathlonCourseMonObject_SetPosition(PokeathlonCourseObject *object, int x, int y, int moveShadow);

// ---------------------------------------------------------------------------------------------
// Sprite pool: resources loaded from NARCs into four resource managers, sprites created from
// them up front and handed out by resource id and kind.

typedef struct PokeathlonCourseSpritePoolTemplate {
    int capacity;      // 0x00: sprites
    int resourceCount; // 0x04: resource ids
    u8 resCounts[4];   // 0x08: char, palette, cell, animation
} PokeathlonCourseSpritePoolTemplate;

typedef struct PokeathlonCourseSpritePoolResources {
    u32 id;                     // 0x00
    SpriteResource *objects[4]; // 0x04: char, palette, cell, animation
} PokeathlonCourseSpritePoolResources;

typedef struct PokeathlonCourseSpritePoolSprite {
    u16 taken;      // 0x00
    u16 shown;      // 0x02
    Sprite *sprite; // 0x04
    u16 kind;       // 0x08
    u16 resId;      // 0x0A
} PokeathlonCourseSpritePoolSprite;

typedef struct PokeathlonCourseSpritePool {
    enum HeapID heapId;                             // 0x000
    int count;                                      // 0x004
    PokeathlonCourseSpritePoolTemplate template;    // 0x008
    SpriteList *spriteList;                         // 0x014
    G2dRenderer renderer;                           // 0x018
    GF_2DGfxResMan *resMans[4];                     // 0x140
    PokeathlonCourseSpritePoolResources *resources; // 0x150
    PokeathlonCourseSpritePoolSprite *sprites;      // 0x154
} PokeathlonCourseSpritePool;                       // size: 0x158

void PokeathlonCourseObject_SetTouchRadius(PokeathlonCourseObject *object, s32 radius);
void PokeathlonCourseObject_SetCollision(PokeathlonCourseObject *object, s32 offsetY, s32 radius);
void PokeathlonCourseObject_GetCollisionCircle(PokeathlonCourseObject *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY, s32 *radius);
s32 PokeathlonCourseObject_GetCollisionRadius(PokeathlonCourseObject *object);
s32 PokeathlonCourseObject_GetCollisionOffsetY(PokeathlonCourseObject *object);
void PokeathlonCourseObject_SetGroundPosition(PokeathlonCourseObject *object, s32 x, s32 y);
void PokeathlonCourseObject_SetCenterPosition(PokeathlonCourseObject *object, int x, int y, int moveShadow);
void PokeathlonCourseObject_GetCollisionCenter(PokeathlonCourseObject *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY);
void PokeathlonCourseObject_CenterToGround(PokeathlonCourseObject *object, int x, int y, int *outX, int *outY);
void PokeathlonCourseObject_GroundToCenter(PokeathlonCourseObject *object, s32 x, s32 y, s32 *outX, s32 *outY);
BOOL PokeathlonCourseObject_IsInTouchRange(PokeathlonCourseObject *object, fx32 x, fx32 y, s32 targetX, s32 targetY);
void PokeathlonCourseObject_SetScale(PokeathlonCourseObject *object, void *scale, void *a2);
BOOL PokeathlonCourseObject_IsDrawn(PokeathlonCourseObject *object);
void PokeathlonCourseObjects_ShiftLargeUp(PokeathlonCourseObjects *objects);
void PokeathlonCourseObjects_PauseAnims(PokeathlonCourseObjects *objects, BOOL paused);
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
void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *position);
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
