#include "field_ground_dust.h"

#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "field_effects.h"
#include "map_object.h"
#include "map_object_movement_utilities.h"

// The dust a map object raises when it lands on plain ground (field effect renderer 11): a billboard
// at the tile that plays its 9 frames once.

#define GROUND_DUST_RECORD   8
#define GROUND_DUST_RESOURCE 6
#define GROUND_DUST_SEQUENCE 6
#define GROUND_DUST_TEXTURE  7

// Renderer data, from FieldEffectManager_GetRendererData(manager, 11).
struct GroundDust {
    FieldEffectManager *manager;
};

typedef struct GroundDustEffectArgs {
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    GroundDust *dust;
    LocalMapObject *mapObject;
} GroundDustEffectArgs;

typedef struct GroundDustEffect {
    BOOL paused;
    u32 objectId;
    u32 mapId;
    u8 unk_0C[4];
    GroundDustEffectArgs args;
    void *billboard;
} GroundDustEffect;

static void GroundDust_LoadGfx(GroundDust *dust);
static void GroundDust_FreeGfx(GroundDust *dust);
static BOOL GroundDustEffect_Init(FieldEffect *effect, void *state);
static void GroundDustEffect_Destroy(FieldEffect *effect, void *state);
static void GroundDustEffect_Update(FieldEffect *effect, void *state);
static void GroundDustEffect_Draw(FieldEffect *effect, void *state);

static const FieldEffectBasicTemplate sGroundDustEffectTemplate = {
    sizeof(GroundDustEffect),
    GroundDustEffect_Init,
    GroundDustEffect_Destroy,
    GroundDustEffect_Update,
    GroundDustEffect_Draw,
};

static const u32 sGroundDustRecordParams[6] = { 0, 9, 1, 0, 0, 2 };

GroundDust *GroundDust_New(FieldEffectManager *manager) {
    GroundDust *dust = FieldEffectManager_HeapAllocAndFill(manager, sizeof(GroundDust), FALSE, 0);
    dust->manager = manager;
    GroundDust_LoadGfx(dust);
    return dust;
}

void GroundDust_Free(GroundDust *dust) {
    GroundDust_FreeGfx(dust);
    FieldEffectManager_HeapFree(dust);
}

static void GroundDust_LoadGfx(GroundDust *dust) {
    FieldEffectManager_LoadBillboardResource(dust->manager, GROUND_DUST_RESOURCE, 0x79);
    FieldEffectManager_LoadBillboardSequence(dust->manager, GROUND_DUST_SEQUENCE, 0x91);
    FieldEffectManager_LoadTexResource(dust->manager, GROUND_DUST_TEXTURE, 0x14, TRUE);
    FieldEffectManager_AddRecord(dust->manager, GROUND_DUST_RECORD, GROUND_DUST_RESOURCE, GROUND_DUST_SEQUENCE, GROUND_DUST_TEXTURE, 0, sGroundDustRecordParams);
}

static void GroundDust_FreeGfx(GroundDust *dust) {
    FieldEffectManager_FreeBillboardResource(dust->manager, GROUND_DUST_RESOURCE);
    FieldEffectManager_FreeBillboardSequence(dust->manager, GROUND_DUST_SEQUENCE);
    FieldEffectManager_FreeTexResource(dust->manager, GROUND_DUST_TEXTURE);
    FieldEffectManager_ReleaseRecord(dust->manager, GROUND_DUST_RECORD);
}

void GroundDust_Spawn(LocalMapObject *mapObject) {
    GroundDustEffectArgs args;
    VecFx32 position;
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    int x;
    int z;

    args.fieldSystem = MapObject_GetFieldSystem(mapObject);
    args.manager = manager;
    args.dust = FieldEffectManager_GetRendererData(manager, 11);
    args.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    x = MapObject_GetXCoord(mapObject);
    z = MapObject_GetZCoord(mapObject);
    SetPositionToTileCenter(x, z, &position);
    FieldEffectManager_CreateEffect(manager, &sGroundDustEffectTemplate, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

static BOOL GroundDustEffect_Init(FieldEffect *effect, void *state) {
    GroundDustEffect *work = state;
    VecFx32 position;
    work->args = *(GroundDustEffectArgs *)FieldEffect_GetUserData(effect);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    FieldEffect_GetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, GROUND_DUST_RECORD, &position);
    return TRUE;
}

static void GroundDustEffect_Destroy(FieldEffect *effect, void *state) {
    GroundDustEffect *work = state;
    BillboardObject_Delete(work->billboard);
}

static void GroundDustEffect_Update(FieldEffect *effect, void *state) {
    GroundDustEffect *work = state;
    if (work->paused == FALSE) {
        fx32 frame;
        BillboardObject_AdvanceFrame(work->billboard, FX32_ONE);
        frame = BillboardObject_GetSequenceFrame(work->billboard);
        if (frame / FX32_ONE >= 9) {
            FieldEffectManager_DestroyEffect((int)effect);
        }
    }
}

static void GroundDustEffect_Draw(FieldEffect *effect, void *state) {
    GroundDustEffect *work = state;
    VecFx32 position;
    FieldEffect_GetPosition(effect, &position);
    position.z += 0x8000;
    BillboardObject_SetPosition(work->billboard, &position);
}
