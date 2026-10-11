#pragma force_active on
#include "global.h"
#include "heap.h"
#include <string.h>
typedef struct Pair {
    u16 sprite;
    u16 model;
} Pair;
typedef struct Work {
    int mode;
    u32 state;
    void *modelA;
    void *modelB;
    NNSG3dResFileHeader *texture;
    u8 anim[0x10];
    void *obj;
    u8 resource[0x28];
    void *owner;
    void *reflection;
} Work;
typedef struct Frame {
    int priority;
    u32 sprite;
    void *ctx;
    void *old;
    int fog;
    VecFx32 copy;
    VecFx32 pos;
} Frame;
typedef struct System {
    u8 pad[0x104];
    void *manager;
} System;
extern const Pair ov01_02207294[];
extern const Pair ov01_022072CC[];
System *ov01_021FA3DC(void *object);
void *ov01_021FA1D0(System *system);
void *ov01_021F72DC(void *object);
void *FldObjSys_ReadMModelFromNarc(System *system, u32 id, u32 a2);
NNSG3dResMdl *BillboardObject_GetModel(void *obj);
int NNS_G3dMdlGetMdlFogEnableFlag(NNSG3dResMdl *mdl, int idx);
int MapObject_GetID(void *object);
u32 MapObject_GetSpriteID(void *object);
u8 ov01_021FA28C(u32 sprite);
u8 ov01_021FA2A0(u32 sprite);
u32 ov01_021FA2AC(u32 sprite);
void BillboardGfxSequence_Init(void *model, void *anim);
int GetMoveModelNoBySpriteId(u32 sprite);
void BillboardResource_Init(void *resource, void *model, NNSG3dResTex *tex, u32 a3, void *anim);
VecFx32 *BillboardObject_GetPosition(void *obj);
void *ov01_021FA31C(void *ctx, void *resource, VecFx32 *pos);
u32 BillboardObject_GetSequence(void *obj);
void BillboardObject_SetSequence(void *obj, u32 v);
fx32 BillboardObject_GetSequenceFrame(void *obj);
void BillboardObject_SetSequenceFrame(void *obj, fx32 v);
fx32 BillboardObject_GetFrame(void *obj);
void BillboardObject_SetFrame(void *obj, fx32 v);
void BillboardObject_AdvanceFrame(void *obj, fx32 v);
void BillboardObject_SetVisible(void *obj, int v);
void BillboardObject_UpdateTextureAnimation(void *obj);
BOOL MapObject_CheckFlag24(void *object);
s32 MapObject_GetPriorityPlusValue(void *object, s32 v);
u32 MapObject_GetMetatileBehaviorInDirection(void *object, int v);
BOOL MetatileBehavior_IsReflective(u8 tile);
BOOL MetatileBehavior_IsPuddle(u8 tile);
void MapObject_CopyPositionVector(void *object, VecFx32 *pos);
void *FieldEffectManager_GetFromMapObject(void *object);
void *Reflection_SpawnForBillboard(void *field, void *resource, void *obj, VecFx32 *pos, int kind, int priority);
void sub_0205E420(void *object);
void ov01_021FA108(void *manager, u32 sprite, void *object);
void sub_0205E38C(void *object, int mode);
u32 MapObjectManager_GetPriority(void *manager);
void *SysTask_CreateOnVWaitQueue(void (*)(void *, void *), void *, int);
void ov01_021FAB9C(void *task, void *data);
void ov01_021FA97C(void *object, int mode);
// @fn ov01_021FA97C
void ov01_021FA97C(void *object, int mode) {
    int fog;
    u32 sprite;
    void *ctx;
    void *old;
    int priority;
    VecFx32 copy;
    VecFx32 pos;
    void *task;
    int kind;
    int refl;
    const Pair *p;
    NNSG3dResTex *tex;
    u8 behavior;
    int id;
    Work *w;
    System *system;

    system = ov01_021FA3DC(object);
    ctx = ov01_021FA1D0(system);
    old = ov01_021F72DC(object);
    w = Heap_AllocAtEnd((enum HeapID)4, sizeof(Work));
    memset(w, 0, sizeof(Work));
    w->mode = mode;
    w->owner = object;
    switch (MapObject_GetID(object)) {
    case 0xff:
        fog = NNS_G3dMdlGetMdlFogEnableFlag(BillboardObject_GetModel(ov01_021F72DC(object)), 0);
        break;
    default:
        fog = 0;
        break;
    }
    sprite = MapObject_GetSpriteID(object);
    kind = ov01_021FA28C(sprite);
    p = ov01_02207294;
    do {
        if (p->sprite == kind) {
            break;
        }
        p++;
    } while (p->sprite != 0xff);
    GF_ASSERT(p->sprite != 0xff);
    w->modelA = FldObjSys_ReadMModelFromNarc(system, p->model, 0);
    kind = ov01_021FA2A0(sprite);
    p = ov01_022072CC;
    do {
        if (p->sprite == kind) {
            break;
        }
        p++;
    } while (p->sprite != 0xff);
    GF_ASSERT(p->sprite != 0xff);
    w->modelB = FldObjSys_ReadMModelFromNarc(system, p->model, 0);
    BillboardGfxSequence_Init(w->modelB, w->anim);
    id = GetMoveModelNoBySpriteId(sprite);
    if (id < 0) {
        GF_AssertFail();
        id = 0;
    }
    w->texture = FldObjSys_ReadMModelFromNarc(system, id, 0);
    tex = NNS_G3dGetTex(w->texture);
    BillboardResource_Init(w->resource, w->modelA, tex, ov01_021FA2AC(sprite), w->anim);
    copy = *BillboardObject_GetPosition(old);
    w->obj = ov01_021FA31C(ctx, w->resource, &copy);
    GF_ASSERT(w->obj != NULL);
    if (fog == 0) {
        NNS_G3dMdlSetMdlFogEnableFlagAll(BillboardObject_GetModel(w->obj), 0);
    }
    BillboardObject_SetSequence(w->obj, BillboardObject_GetSequence(old));
    BillboardObject_SetSequenceFrame(w->obj, BillboardObject_GetSequenceFrame(old));
    BillboardObject_SetFrame(w->obj, BillboardObject_GetFrame(old));
    BillboardObject_AdvanceFrame(w->obj, 0);
    BillboardObject_SetVisible(w->obj, 1);
    BillboardObject_UpdateTextureAnimation(w->obj);
    if (MapObject_CheckFlag24(object) == TRUE) {
        priority = MapObject_GetPriorityPlusValue(object, 2);
        behavior = MapObject_GetMetatileBehaviorInDirection(object, 1);
        if (MetatileBehavior_IsReflective(behavior) == TRUE) {
            refl = 2;
        } else if (MetatileBehavior_IsPuddle(behavior) == TRUE) {
            refl = 0;
        } else {
            refl = 1;
        }
        MapObject_CopyPositionVector(object, &pos);
        w->reflection = Reflection_SpawnForBillboard(FieldEffectManager_GetFromMapObject(object), w->resource, w->obj, &pos, refl, priority);
    }
    sub_0205E420(object);
    ov01_021FA108(system->manager, sprite, object);
    sub_0205E38C(object, mode);
    MapObjectManager_GetPriority(system->manager);
    task = SysTask_CreateOnVWaitQueue(ov01_021FAB9C, w, 0xff);
    GF_ASSERT(task != NULL);
    ov01_021FAB9C(task, w);
}
