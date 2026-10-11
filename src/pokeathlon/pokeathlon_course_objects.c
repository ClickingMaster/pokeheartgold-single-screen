#include "pokeathlon/pokeathlon_course_objects.h"

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "sprite_transfer.h"

void PokeathlonCourseObject_SetTouchRadius(PokeathlonCourseObject *object, s32 radius) {
    object->touchRadius = radius;
}

void PokeathlonCourseObject_SetCollision(PokeathlonCourseObject *object, s32 offsetY, s32 radius) {
    object->collisionOffsetY = offsetY;
    object->collisionRadius = radius;
}

#pragma force_active on

void PokeathlonCourseObject_GetCollisionCircle(PokeathlonCourseObject *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY, s32 *radius) {
    PokeathlonCourseObject_GetCollisionCenter(object, x, y, outX, outY);
    *radius = object->collisionRadius;
}

#pragma force_active reset

s32 PokeathlonCourseObject_GetCollisionRadius(PokeathlonCourseObject *object) {
    return object->collisionRadius;
}

s32 PokeathlonCourseObject_GetCollisionOffsetY(PokeathlonCourseObject *object) {
    return object->collisionOffsetY;
}

// Places the Pokemon so that it stands on (x, y): the shadow and the marker go to that point.
void PokeathlonCourseObject_SetGroundPosition(PokeathlonCourseObject *object, s32 x, s32 y) {
    VecFx32 matrix;
    VecFx32 position = { 0 };
    Sprite *sprite = PokeathlonCourseMonGfx_GetSprite(object->gfx);
    s32 top;
    matrix = position;
    matrix.x = x << FX32_SHIFT;
    matrix.y = y << FX32_SHIFT;
    if (object->shadowSprite != NULL) {
        Sprite_SetMatrix(object->shadowSprite, &matrix);
    }
    if (object->markerSprite != NULL) {
        Sprite_SetMatrix(object->markerSprite, &matrix);
    }
    top = y + object->shadowOffsetY - 2 * object->halfHeight;
    {
        s32 left = x - object->halfWidth;
        object->x = x;
        object->y = top + object->halfHeight;
        position.x = left << FX32_SHIFT;
    }
    position.y = top << FX32_SHIFT;
    if (PokeathlonCourseMonGfx_IsShiftedUp(object->gfx)) {
        position.y += 0x8000;
    }
    Sprite_SetMatrix(sprite, &position);
}

// Places the Pokemon so that the middle of its visible part is at (x, y).
void PokeathlonCourseObject_SetCenterPosition(PokeathlonCourseObject *object, int x, int y, int moveShadow) {
    s32 halfWidth = object->halfWidth;
    s32 topRow = object->topRow;
    s32 visibleHeight = 2 * object->halfHeight - topRow;
    x -= halfWidth;
    y -= topRow + visibleHeight / 2;
    PokeathlonCourseMonObject_SetPosition(object, x, y, moveShadow);
}

// Centre of the collision circle of a Pokemon whose visible part is centred at (x, y) (fx32).
void PokeathlonCourseObject_GetCollisionCenter(PokeathlonCourseObject *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY) {
    s32 frameHeight = 2 * object->halfHeight;
    s32 center = object->topRow + (frameHeight - object->topRow) / 2;
    fx32 result = y - (center << FX32_SHIFT) + ((frameHeight - object->collisionOffsetY) << FX32_SHIFT);
    *outX = x;
    *outY = result;
}

// Ground point of a Pokemon whose visible part is centred at (x, y).
void PokeathlonCourseObject_CenterToGround(PokeathlonCourseObject *object, int x, int y, int *outX, int *outY) {
    int frameHeight;
    int center;
    if (object->shadowSprite == NULL) {
        GF_AssertFail();
        return;
    }
    frameHeight = 2 * object->halfHeight;
    center = object->topRow + (frameHeight - object->topRow) / 2;
    y -= center;
    y += frameHeight;
    y -= object->shadowOffsetY;
    *outX = x;
    *outY = y;
}

// Centre of the visible part of a Pokemon that stands on (x, y).
#pragma force_active on

void PokeathlonCourseObject_GroundToCenter(PokeathlonCourseObject *object, s32 x, s32 y, s32 *outX, s32 *outY) {
    y = y + object->shadowOffsetY - object->halfHeight * 2;
    *outX = x;
    *outY = object->topRow + (y + (object->halfHeight * 2 - object->topRow) / 2);
}

#pragma force_active reset

BOOL PokeathlonCourseObject_IsInTouchRange(PokeathlonCourseObject *object, fx32 x, fx32 y, s32 targetX, s32 targetY) {
    VecFx32 delta;
    VecFx32 position;
    VecFx32 target;
    target.x = targetX << FX32_SHIFT;
    target.y = targetY << FX32_SHIFT;
    target.z = 0;
    position.x = x;
    position.y = y;
    position.z = 0;
    VEC_Subtract(&target, &position, &delta);
    if (VEC_Mag(&delta) < (object->touchRadius << FX32_SHIFT)) {
        return TRUE;
    }
    return FALSE;
}

void PokeathlonCourseObject_SetScale(PokeathlonCourseObject *object, void *scale, void *a2) {
    PokeathlonCourseMonGfx_SetScale(PokeathlonCourseMonObject_GetGfx(object), scale, a2);
}

#pragma force_active on

BOOL PokeathlonCourseObject_IsDrawn(PokeathlonCourseObject *object) {
    if (Sprite_GetDrawFlag(PokeathlonCourseMonGfx_GetSprite(object->gfx))) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void PokeathlonCourseObjects_ShiftLargeUp(PokeathlonCourseObjects *objects) {
    PokeathlonCourseMonGfx_ShiftLargeUp(objects->gfxManager);
}

void PokeathlonCourseObjects_PauseAnims(PokeathlonCourseObjects *objects, BOOL paused) {
    u8 i = 0;
    BOOL active;
    if (objects->count > 0) {
        if (paused == FALSE) {
            active = TRUE;
        } else {
            active = FALSE;
        }
        do {
            Sprite_SetAnimActiveFlag(PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonObject_GetGfx(PokeathlonCourseMonObjects_Get(objects, i))), active);
            i++;
        } while (i < objects->count);
    }
}

#pragma force_active on

PokeathlonCourseSpritePool *PokeathlonCourseSpritePool_New(enum HeapID heapId, PokeathlonCourseSpritePoolTemplate *template) {
    PokeathlonCourseSpritePool *pool = Heap_Alloc(heapId, sizeof(PokeathlonCourseSpritePool));
    int i;
    MIi_CpuClearFast(0, (u32 *)pool, sizeof(PokeathlonCourseSpritePool));
    pool->heapId = heapId;
    pool->template = *template;
    pool->resources = Heap_Alloc(heapId, pool->template.resourceCount * sizeof(PokeathlonCourseSpritePoolResources));
    MIi_CpuClear32(0, (u32 *)pool->resources, pool->template.resourceCount * sizeof(PokeathlonCourseSpritePoolResources));
    pool->sprites = Heap_Alloc(pool->heapId, pool->template.capacity * sizeof(PokeathlonCourseSpritePoolSprite));
    MIi_CpuClear32(0, (u32 *)pool->sprites, pool->template.capacity * sizeof(PokeathlonCourseSpritePoolSprite));
    pool->spriteList = G2dRenderer_Init(pool->template.capacity, &pool->renderer, pool->heapId);
    for (i = 0; i < 4; i++) {
        pool->resMans[i] = Create2DGfxResObjMan(pool->template.resCounts[i], (GfGfxResType)i, pool->heapId);
    }
    return pool;
}

#pragma force_active reset

void PokeathlonCourseSpritePool_Free(PokeathlonCourseSpritePool *pool) {
    u8 i = 0;
    if (pool->template.resourceCount > 0) {
        do {
            if (pool->resources[i].objects[0]) {
                SpriteTransfer_DeleteCharTransferTask(pool->resources[i].objects[0]);
            }
            if (pool->resources[i].objects[1]) {
                SpriteTransfer_DeletePlttTransferTask(pool->resources[i].objects[1]);
            }
            i++;
        } while (i < pool->template.resourceCount);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(pool->resMans[i]);
    }
    SpriteList_Delete(pool->spriteList);
    Heap_Free(pool->sprites);
    Heap_Free(pool->resources);
    Heap_Free(pool);
}

void PokeathlonCourseSpritePool_SetResourceId(PokeathlonCourseSpritePool *pool, int index, u32 id) {
    if (index >= pool->template.resourceCount) {
        GF_AssertFail();
    }
    pool->resources[index].id = id;
}

void PokeathlonCourseSpritePool_LoadChar(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id, int vram) {
    SpriteResource **slot = PokeathlonCourseSpritePool_GetResourceSlot(pool, id, 0);
    *slot = AddCharResObjFromNarc(pool->resMans[0], narcId, fileId, FALSE, id, vram, pool->heapId);
}

void PokeathlonCourseSpritePool_LoadPltt(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id, int vram, u8 count) {
    SpriteResource **slot = PokeathlonCourseSpritePool_GetResourceSlot(pool, id, 1);
    *slot = AddPlttResObjFromNarc(pool->resMans[1], narcId, fileId, FALSE, id, vram, count, pool->heapId);
}

void PokeathlonCourseSpritePool_LoadCell(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id) {
    SpriteResource **slot = PokeathlonCourseSpritePool_GetResourceSlot(pool, id, 2);
    *slot = AddCellOrAnimResObjFromNarc(pool->resMans[2], narcId, fileId, FALSE, id, GF_GFX_RES_TYPE_CELL, pool->heapId);
}

void PokeathlonCourseSpritePool_LoadAnim(PokeathlonCourseSpritePool *pool, NarcId narcId, int fileId, int id) {
    SpriteResource **slot = PokeathlonCourseSpritePool_GetResourceSlot(pool, id, 3);
    *slot = AddCellOrAnimResObjFromNarc(pool->resMans[3], narcId, fileId, FALSE, id, GF_GFX_RES_TYPE_ANIM, pool->heapId);
}

void PokeathlonCourseSpritePool_TransferResources(PokeathlonCourseSpritePool *pool) {
    int i = 0;
    if (pool->template.resourceCount > 0) {
        do {
            if (pool->resources[i].objects[0]) {
                SpriteTransfer_CreateCharTransferTask_AllocAtEnd(pool->resources[i].objects[0]);
            }
            if (pool->resources[i].objects[1]) {
                SpriteTransfer_CreateExtPlttTransferTask(pool->resources[i].objects[1]);
            }
            i++;
        } while (i < pool->template.resourceCount);
    }
}

PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_CreateAndTake(PokeathlonCourseSpritePool *pool, int priority, NNS_G2D_VRAM_TYPE whichScreen, u16 resId, u8 kind) {
    PokeathlonCourseSpritePool_CreateSprite(pool, priority, whichScreen, resId, kind);
    return PokeathlonCourseSpritePool_Take(pool, resId, kind);
}

#pragma force_active on

Sprite *PokeathlonCourseSpritePool_CreateSprite(PokeathlonCourseSpritePool *pool, int priority, NNS_G2D_VRAM_TYPE whichScreen, u16 resId, u8 kind) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    GF_ASSERT(pool->count < pool->template.capacity);
    CreateSpriteResourcesHeader(&header, resId, resId, resId, resId, -1, -1, 0, priority, pool->resMans[0], pool->resMans[1], pool->resMans[2], pool->resMans[3], NULL, NULL);
    template.spriteList = pool->spriteList;
    template.header = &header;
    template.heapID = pool->heapId;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = whichScreen;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    pool->sprites[pool->count].taken = 0;
    pool->sprites[pool->count].shown = 0;
    pool->sprites[pool->count].sprite = sprite;
    pool->sprites[pool->count].kind = kind;
    pool->sprites[pool->count].resId = resId;
    pool->count++;
    return sprite;
}

#pragma force_active reset

// Hands out a free sprite of resource id and kind.
PokeathlonCourseSpritePoolSprite *PokeathlonCourseSpritePool_Take(PokeathlonCourseSpritePool *pool, int resId, int kind) {
    int i = 0;
    int capacity = pool->template.capacity;
    if (capacity > 0) {
        PokeathlonCourseSpritePoolSprite *entry = pool->sprites;
        do {
            if (entry->taken == 0 && entry->resId == resId && entry->kind == kind) {
                entry->taken = 1;
                return entry;
            }
            i++;
            entry++;
        } while (i < capacity);
    }
    GF_AssertFail();
    return NULL;
}

void PokeathlonCourseSpritePoolSprite_Show(PokeathlonCourseSpritePoolSprite *entry, BOOL shown, BOOL draw) {
    if (entry->taken != 0) {
        if (shown) {
            entry->shown = 1;
            if (draw) {
                Sprite_SetDrawFlag(entry->sprite, 1);
                return;
            }
            Sprite_SetDrawFlag(entry->sprite, 0);
            return;
        }
        entry->shown = 0;
        Sprite_SetDrawFlag(entry->sprite, 0);
        return;
    }
    GF_AssertFail();
}

void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonCourseSpritePoolSprite *entry, int seq) {
    Sprite_SetAnimCtrlSeq(entry->sprite, seq);
}

void PokeathlonCourseSpritePoolSprite_TryChangeAnim(PokeathlonCourseSpritePoolSprite *entry, int seq) {
    Sprite_TryChangeAnimSeq(entry->sprite, seq);
}

BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(PokeathlonCourseSpritePoolSprite *entry) {
    return Sprite_IsAnimated(entry->sprite);
}

void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *position) {
    Sprite_SetMatrix(entry->sprite, position);
}

VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(PokeathlonCourseSpritePoolSprite *entry) {
    return Sprite_GetMatrixPtr(entry->sprite);
}

void PokeathlonCourseSpritePoolSprite_SetScale(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *scale, u8 affineType) {
    Sprite_SetScaleAndAffineType(entry->sprite, scale, affineType);
}

void PokeathlonCourseSpritePoolSprite_SetRotation(PokeathlonCourseSpritePoolSprite *entry, u16 rotation, u8 affineType) {
    Sprite_SetRotationAndAffineType(entry->sprite, rotation, affineType);
}

Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(PokeathlonCourseSpritePoolSprite *entry) {
    return entry->sprite;
}

void PokeathlonCourseSpritePool_Draw(PokeathlonCourseSpritePool *pool) {
    SpriteList_RenderAndAnimateSprites(pool->spriteList);
}

#pragma force_active on

void PokeathlonCourseSpritePool_SetSurfaceCoords(PokeathlonCourseSpritePool *pool, fx32 mainX, fx32 mainY, fx32 subX, fx32 subY) {
    G2dRenderer_SetSubSurfaceCoords(&pool->renderer, subX, subY);
    G2dRenderer_SetMainSurfaceCoords(&pool->renderer, mainX, mainY);
}

#pragma force_active reset

SpriteList *PokeathlonCourseSpritePool_GetSpriteList(PokeathlonCourseSpritePool *pool) {
    return pool->spriteList;
}

#pragma opt_lifetimes off
#pragma opt_common_subs on

// Slot of resource type (0 char .. 3 animation) of the resources with the given id.
#pragma force_active on

SpriteResource **PokeathlonCourseSpritePool_GetResourceSlot(PokeathlonCourseSpritePool *pool, u32 id, u32 type) {
    int i;
    u32 count;
    PokeathlonCourseSpritePoolResources *entry;
    PokeathlonCourseSpritePoolResources *resources;
    if (type >= 4) {
        GF_AssertFail();
    }
    count = pool->template.resourceCount;
    i = 0;
    if ((s32)count > 0) {
        resources = pool->resources;
        entry = resources;
        do {
            if (id == entry->id) {
                return &resources[i].objects[type];
            }
            i++;
            entry++;
        } while (i < (s32)count);
    }
    GF_AssertFail();
    return NULL;
}

#pragma force_active reset

#pragma opt_lifetimes reset
#pragma opt_common_subs reset

void PokeathlonCourseSpritePoolSprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite *entry, int priority) {
    Sprite_SetDrawPriority(entry->sprite, priority);
}

#pragma force_active on

void PokeathlonCourseSpritePool_PauseAnims(PokeathlonCourseSpritePool *pool, BOOL paused) {
    int i = 0;
    int active;
    int offset;
    if (pool->template.capacity > 0) {
        offset = 0;
        active = paused == FALSE ? TRUE : FALSE;
        do {
            PokeathlonCourseSpritePoolSprite *entry = (PokeathlonCourseSpritePoolSprite *)((u8 *)pool->sprites + offset);
            if (entry->taken) {
                Sprite_SetAnimActiveFlag(PokeathlonCourseSpritePoolSprite_GetSprite(entry), active);
            }
            i++;
            offset += sizeof(PokeathlonCourseSpritePoolSprite);
        } while (i < pool->template.capacity);
    }
}

#pragma force_active reset
