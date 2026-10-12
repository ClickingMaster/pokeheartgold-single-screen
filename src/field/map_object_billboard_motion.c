#include "field/map_object_billboard_motion.h"

#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"
#include "field/field_follow_mon.h"
#include "field/map_object_model_list.h"

#include "field_system.h"
#include "follow_mon.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "field/map_object_render.h"
#include "party.h"
#include "pokemon.h"
#include "script_pokemon_util.h"

// Billboard sequence callback: called with the map object, its billboard, the object's draw work and the facing
// direction. The dispatch tables are in asm/map_object_billboard_motion_data.s.
typedef void (*BillboardMotionFunc)(void *object, void *billboard, void *work, int facing);

typedef struct BillboardFrameSteps4 {
    fx32 values[4];
} BillboardFrameSteps4;

typedef struct BillboardFrameSteps7 {
    fx32 values[7];
} BillboardFrameSteps7;

typedef struct BillboardFrameSteps8 {
    fx32 values[8];
} BillboardFrameSteps8;

// One sprite ID -> effect model offset entry of the table ending in 0xFFFF
typedef struct EffectModelOffset {
    u32 spriteId;
    VecFx32 offset;
} EffectModelOffset;

typedef struct FollowMonDrawWork {
    Sprite *sprite;
    VecFx32 vector;
    s8 direction;
    u8 unk11[6];
    u8 active : 1;
    u8 : 7;
} FollowMonDrawWork;

// Work of the stepped walk sequences below: the frame step list index, the previous facing and movement
typedef struct SteppedWalkWork {
    s16 step;
    s8 prevFacing;
    s8 movement;
} SteppedWalkWork;

typedef struct HeldStepWork {
    s8 facing;
    u8 unused;
    s8 movement;
    u8 held : 1;
    u8 unusedFlags : 7;
} HeldStepWork;

typedef struct HeldMotionWork {
    s8 facing;
    u8 unk1;
    s8 movement;
    u8 held : 1;
    u8 rest : 7;
    void *billboard;
} HeldMotionWork;

typedef struct FacingMotionWork {
    u8 facing;
    u8 unk1;
    u8 movement;
    u8 unk3;
    void *billboard;
} FacingMotionWork;

typedef struct SignedFacingMotionWork {
    s8 facing;
    u8 unk1;
    s8 movement;
    u8 unk3;
    void *billboard;
} SignedFacingMotionWork;

extern const BillboardMotionFunc gNpcBillboardMotions[9];
extern const BillboardMotionFunc gPcWomanBillboardMotions[10];
extern const BillboardMotionFunc gPlayerWalkingBillboardMotions[10];
extern const BillboardMotionFunc gPlayerCyclingBillboardMotions[12];
extern const BillboardMotionFunc gPlayerFishingBillboardMotions[4];
extern const struct {
    BillboardMotionFunc stand[1];
    BillboardMotionFunc walk[2];
} ov01_022089CC;
extern const u32 ov01_022089F8[];
extern const u32 ov01_02208A18[];
extern const u32 ov01_02208A28[];
extern const BillboardFrameSteps4 ov01_022089D8;
extern const BillboardFrameSteps4 ov01_02208A08;
extern const BillboardFrameSteps4 ov01_02208A38;
extern const BillboardFrameSteps7 ov01_02208A48;
extern const BillboardFrameSteps7 ov01_02208A64;
extern const BillboardFrameSteps8 ov01_02208A80;
extern const BillboardFrameSteps8 ov01_02208AA0;
extern const u32 ov01_022073D8[];

// map_object_behavior.c, field_object_models.c, field_map_object_render
void *ApricornTreeObject_GetBillboard(LocalMapObject *object);
// billboard_lists.h takes an int; this file passes the flag as a u8 (the caller narrows it)
void BillboardObject_SetVisible(void *billboard, u8 visible);
void FollowMonBillboard_AddFacingOffset(LocalMapObject *object, u32 direction, VecFx32 *out);
void FollowMonBillboard_Update(LocalMapObject *object, Sprite *sprite, void *work, u32 direction, BOOL update);
BOOL MapObject_IsBillboardAnimPaused(void *object);
void BillboardObject_SaveState(void *billboard, void *work);
void BillboardObject_RestoreState(void *billboard, void *work);

void BillboardDraw_Init(void *object);
void BillboardDraw_Delete(void *object);
void BillboardDraw_Pause(void *object);
void BillboardDraw_Resume(void *object);
void FollowMonDraw_Init(void *object);
void FollowMonDraw_Delete(void *object);
void FollowMonDraw_Pause(void *object);
void FollowMonDraw_Resume(LocalMapObject *object);
void ov01_021F7704(void *object);
void *MapObjectManager_GetFollowMonBillboard(MapObjectManager *manager);
void BillboardDraw_UpdateNpc(void *object);
void BillboardDraw_UpdatePositionOnly(void *object);
void BillboardDraw_UpdateLooping(void *object);
void FollowMonDraw_UpdateLarge(LocalMapObject *object);
void FollowMonDraw_Update(LocalMapObject *object);
void BillboardMotion_Stand(void *object, void *billboard, const s8 *work, int facing);
void BillboardMotion_AnimateHalfSpeed(void *object, void *billboard, const s8 *work, int facing);
void BillboardMotion_AnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing);
void BillboardMotion_AnimateDoubleSpeed(void *object, void *billboard, const s8 *work, int facing);
void BillboardMotion_AnimateQuadSpeed(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F7A90(void *object, void *billboard, s8 *workRaw, int facing);
void ov01_021F7B14(void *object, void *billboard, s8 *workRaw, int facing);
void ov01_021F7B90(void *object, void *billboard, s8 *workRaw, int facing);
void BillboardDraw_UpdatePcWoman(void *object);
void ov01_021F7C80(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F7CBC(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F7CE4(void *object);
void PlayerDraw_Init(void *object);
void PlayerDraw_Delete(void *object);
void PlayerDraw_Pause(void *object);
void PlayerDraw_Resume(void *object);
void PlayerDraw_UpdateWalking(void *object);
void ov01_021F7EE4(void *object, void *billboard, HeldStepWork *work, int facing);
void ov01_021F7F54(void *object, void *billboard, HeldStepWork *work, int facing);
void ov01_021F7FC8(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F801C(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F8070(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F80C4(void *object, void *billboard, s8 *work, int facing);
void ov01_021F8164(void *object, void *billboard, s8 *work, int facing);
void ov01_021F81FC(void *object, void *billboard, s8 *work, int facing);
void ov01_021F829C(void *object, void *billboard, const s8 *work, int facing);
void PlayerDraw_UpdateCycling(void *object);
void ov01_021F8374(void *object, void *billboard, HeldMotionWork *work, int facing);
void ov01_021F8400(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F847C(void *object, void *billboard, const s8 *work, int facing);
void PlayerDraw_UpdateFishing(void *object);
void ov01_021F856C(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F85BC(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F85F4(void *object, void *billboard, void *workRaw, int facing);
void ov01_021F86B8(void *object, void *billboard, const s8 *work, int facing);
void PlayerDraw_SetFishingOffset(void *object, void *billboard, u32 facing);
void PlayerDraw_UpdateLadder(void *object);
void ov01_021F8804(void *object, void *billboard, const s8 *work, int facing);
void ov01_021F883C(void *object, void *billboard, const s8 *work, int facing);
void PlayerDraw_SetLadderOffset(void *object, void *billboard, u32 facing);
void PlayerDraw_UpdateSurfing(void *object);
void PlayerDraw_UpdateUseHmOrSaving(void *object);
void PlayerDraw_UpdateWatering(void *object);
void PlayerDraw_UpdatePoketch(void *object);
void PlayerDraw_UpdateHeal(void *object);
void PlayerDraw_UpdateApricornShake(void *object);
void ov01_021F8BA8(void *object, void *billboard, const s8 *work, int facing);
void PlayerDraw_SetApricornShakeOffset(void *object, void *billboard, int facing);
void BillboardObject_AlignSequenceFrame(void *billboard, int divisor);
void BillboardObject_AlignSequenceFrameTo8(void *billboard);
void BillboardObject_SetSequenceKeepAlignedFrame(void *billboard, u32 sequence, int divisor);
void BillboardObject_SetSequenceKeepAlignedFrameTo8(void *billboard, u32 sequence);
void MapObject_UpdateBillboardVisibility(void *object, void *billboard);
void MapObject_GetEffectModelOffset(u32 spriteId, VecFx32 *offset);
void ModelObjectDraw_Init(void *object);
void ModelObjectDraw_Update(void);
void ModelObjectDraw_Delete(void *object);
void ModelObjectDraw_Pause(void *object);
void ModelObjectDraw_Resume(void *object);

static inline void BillboardMotion_Loop(void *billboard, const s8 *work, int facing, int movement, const u32 *sequences) {
    if (facing != work[0] || work[2] != movement) {
        BillboardObject_SetSequence(billboard, sequences[facing]);
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    BillboardObject_AdvanceFrame(billboard, FX32_ONE);
}

static inline void BillboardMotion_Fall(void *billboard, const s8 *work, int facing, int movement) {
    fx32 frame;
    if (facing != work[0] || work[2] != movement) {
        BillboardObject_SetSequence(billboard, ov01_022089F8[facing]);
        BillboardObject_SetSequenceFrame(billboard, 15 * FX32_ONE);
    }
    frame = BillboardObject_GetSequenceFrame(billboard) - FX32_ONE;
    if (frame < 0) {
        frame = 0;
    }
    BillboardObject_SetSequenceFrame(billboard, frame);
    BillboardObject_AdvanceFrame(billboard, 0);
}

static inline void BillboardMotion_Walk(void *object, void *billboard, const s8 *work, int facing, fx32 speed) {
    if (facing != work[0]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else if (work[2] == 9) {
        BillboardObject_SetSequenceKeepAlignedFrameTo8(billboard, Facing_GetBillboardStandSequence(facing));
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, speed);
    }
}

Sprite *MapObject_GetBillboard(LocalMapObject *object) {
    int spriteId = MapObject_GetSpriteID(object);

    switch (spriteId) {
    case 0:
    case 0x15:
    case 0x61:
    case 0x62:
    case 0xB0:
    case 0xB1:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xBC:
    case 0xBD:
    case 0xC4:
    case 0xC5:
    case 0xC6:
    case 0xC7:
    case 0xC8:
    case 0xC9:
    case 0xF8:
    case 0xF9:
    case 0x102:
    case 0x103:
    case 0x104:
    case 0x105:
        return ((void **)MapObject_GetDrawData(object))[1];
    }

    if (spriteId >= 0x1AC && spriteId <= 0x3E1) {
        return ((void **)MapObject_GetDrawData(object))[0];
    }
    if (spriteId >= 0x106 && spriteId <= 0x10D) {
        return ApricornTreeObject_GetBillboard(object);
    }
    return ((void **)MapObject_GetDrawData(object))[1];
}

void BillboardDraw_Init(void *object) {
    s8 *work = (s8 *)MapObject_InitDrawData(object, 0x14);
    work[2] = -1;
    MapObject_CreateBillboard(object, work + 4);
    if (*(void **)(work + 4) != NULL) {
        MapObject_CallDrawUpdate(object);
    }
}

void BillboardDraw_Delete(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    MapObject_DeleteBillboard(object, work + 4);
}

void BillboardDraw_Pause(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    void *billboard = *(void **)(work + 4);
    if (billboard != NULL) {
        BillboardObject_SaveState(billboard, work + 8);
    }
    MapObject_DeleteBillboard(object, work + 4);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK21);
}

void BillboardDraw_Resume(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    if (MapObject_IsBillboardPending(object) != TRUE) {
        if (*(void **)(work + 4) == NULL) {
            MapObject_CreateBillboard(object, work + 4);
        }
        if (*(void **)(work + 4) != NULL) {
            BillboardObject_RestoreState(*(void **)(work + 4), work + 8);
            MapObject_UpdateBillboardPosition(object, *(void **)(work + 4));
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK21);
        }
    }
}

void FollowMonDraw_Init(void *object) {
    typedef struct {
        void *billboard;
        u8 unk04[0xC];
        s8 facing;
        u8 unk11[4];
        u8 unk15;
        u8 unk16;
        u8 flags;
    } Work;
    Work *work = (Work *)MapObject_InitDrawData(object, 0x18);
    work->facing = -1;
    MapObject_CreateBillboard(object, work);
    work->unk15 = 0;
    work->flags &= ~2;
    work->flags &= ~1;
    if (work->billboard != NULL) {
        MapObject_CallDrawUpdate(object);
    }
}

void FollowMonDraw_Delete(void *object) {
    MapObject_DeleteBillboard(object, MapObject_GetDrawData(object));
}

void FollowMonDraw_Pause(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    void *billboard = *(void **)work;
    if (billboard != NULL) {
        BillboardObject_SaveState(billboard, work + 4);
    }
    work[0x17] &= ~2;
    work[0x17] &= ~1;
    MapObject_DeleteBillboard(object, work);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK21);
}

#pragma force_active on
// Draw callback of the following Pokemon: (re)loads the party leader's overworld sprite on first use.
void FollowMonDraw_Resume(LocalMapObject *object) {
    typedef struct {
        Sprite *sprite;
        u8 unk4[0x13];
        u8 started : 1;
        u8 loaded : 1;
        u8 rest : 6;
    } Work;
    VecFx32 pos;
    VecFx32 facing;
    Work *work = (Work *)MapObject_GetDrawData(object);
    if (!work->loaded && MapObject_GetID(object) == 0xFD) {
        FieldSystem *fieldSystem = MapObjectManager_GetFieldSystem(MapObject_GetManager(object));
        Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        int species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        int form = GetMonData(mon, MON_DATA_FORM, NULL);
        u8 gender = GetMonGender(mon);
        u32 spriteId = FollowMon_GetSpriteID(species, form, gender);
        u8 shiny = MonIsShiny(mon);
        int x, y, z;
        work->loaded = TRUE;
        FollowMon_SetObjectParams(object, species, form, (u32)shiny);
        MapObject_SetSpriteID(object, spriteId);
        MapObject_RestoreDrawUpdateCallback(object);
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_START_MOVEMENT);
        MapObject_CopyPositionVector(object, &pos);
        x = MapObject_GetXCoord(object);
        pos.x = FX32_CONST(16) * x + FX32_CONST(8);
        MapObject_SetPreviousX(object, x);
        y = MapObject_GetYCoord(object);
        MapObject_SetPreviousY(object, y);
        z = MapObject_GetZCoord(object);
        pos.z = FX32_CONST(16) * z + FX32_CONST(8);
        MapObject_SetPreviousZ(object, z);
        MapObject_SetPositionVector(object, &pos);
        MapObject_ClearHeldMovement(object);
        if (fieldSystem->followMon.species != species || fieldSystem->followMon.form != form || fieldSystem->followMon.shiny != shiny || fieldSystem->followMon.gender != gender) {
            FieldSystem_SetFollowerPokeParam(fieldSystem, species, form, shiny, gender);
            MapObject_SetFacingDirectionDirect(object, DIR_SOUTH);
        }
        FieldSystem_UnkSub108_Set(fieldSystem->unk108, mon, species, GetMonData(mon, MON_DATA_PERSONALITY, NULL));
    }
    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    if (work->sprite == NULL) {
        MapObject_CreateBillboard(object, work);
    }
    if (work->sprite != NULL) {
        work->started = TRUE;
        {
            VecFx32 offset = { 0, 0, 0 };
            MapObject_CopyFacingVector(object, &facing);
            MapObject_GetSpriteID(object);
            FollowMonBillboard_AddFacingOffset(object, MapObject_GetFacingDirection(object), &offset);
            facing.x = offset.x;
            facing.z = offset.z;
            MapObject_SetFacingVector(object, &facing);
            if (MapObject_HasFollowMonSprite(object)) {
                MapObject_SetBillboardSepia((void *)TRUE, object, work->sprite);
            } else if (MapObject_IsShinyPokemon(object)) {
                FollowMonBillboard_SetResource(TRUE, object, work->sprite);
            }
            BillboardObject_RestoreState(work->sprite, work->unk4);
            MapObject_UpdateBillboardPosition(object, work->sprite);
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK21);
            MapObject_CallDrawUpdate(object);
        }
    }
}
#pragma force_active reset

void ov01_021F7704(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    work[0x15] = 0;
    MapObject_GetFacingVector(object)->y = 0;
}

void *MapObjectManager_GetFollowMonBillboard(MapObjectManager *manager) {
    return *(void **)MapObject_GetDrawData(MapObjectManager_GetFirstActiveObjectByID(manager, 0xFD));
}

#pragma force_active on
void BillboardDraw_UpdateNpc(void *object) {
    typedef struct {
        u8 unk0;
        u8 unk1;
        u8 facing;
        u8 movement;
        void *billboard;
    } Work;
    struct {
        volatile int value;
    } facing;
    Work *work = (Work *)MapObject_GetDrawData(object);
    void *billboard = work->billboard;
    int movement;
    if (MapObject_IsBillboardPending(object) == TRUE || billboard == NULL) {
        return;
    }
    facing.value = MapObject_GetFacingDirection(object);
    movement = MapObject_GetAnimation(object);
    if (movement >= 9) {
        GF_AssertFail();
    }
    gNpcBillboardMotions[movement](object, billboard, work, facing.value);
    MapObject_GetFacingVector(object)->z = FX32_CONST(0.5);
    work->facing = facing.value;
    work->movement = MapObject_GetAnimation(object);
    MapObject_UpdateBillboardPosition(object, billboard);
    MapObject_UpdateBillboardVisibility(object, billboard);
}
#pragma force_active reset

void BillboardDraw_UpdatePositionOnly(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    if (MapObject_IsBillboardPending(object) != TRUE) {
        void *billboard = *(void **)(work + 4);
        if (billboard != NULL) {
            MapObject_UpdateBillboardPosition(object, billboard);
            MapObject_UpdateBillboardVisibility(object, billboard);
        }
    }
}

void BillboardDraw_UpdateLooping(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    if (MapObject_IsBillboardPending(object) != TRUE) {
        void *billboard = *(void **)(work + 4);
        if (billboard != NULL) {
            if (!MapObject_IsBillboardAnimPaused(object)) {
                BillboardObject_AdvanceFrame(billboard, FX32_ONE);
            }
            MapObject_UpdateBillboardPosition(object, billboard);
            MapObject_UpdateBillboardVisibility(object, billboard);
        }
    }
}

void FollowMonDraw_UpdateLarge(LocalMapObject *object) {
    FollowMonDrawWork *work = (FollowMonDrawWork *)MapObject_GetDrawData(object);
    Sprite *sprite = work->sprite;
    u32 direction;

    if (MapObject_IsBillboardPending(object) == TRUE || sprite == NULL) {
        return;
    }
    if (!work->active) {
        work->active = TRUE;
        if (MapObject_HasFollowMonSprite(object)) {
            MapObject_SetBillboardSepia((void *)TRUE, object, work->sprite);
        } else if (MapObject_IsShinyPokemon(object)) {
            FollowMonBillboard_SetResource(TRUE, object, sprite);
        }
    }
    direction = MapObject_GetFacingDirection(object);
    FollowMonBillboard_Update(object, sprite, work, direction, TRUE);
    work->direction = direction;
    MapObject_UpdateBillboardPosition(object, sprite);
    MapObject_UpdateBillboardVisibility(object, sprite);
}

void FollowMonDraw_Update(LocalMapObject *object) {
    FollowMonDrawWork *work = (FollowMonDrawWork *)MapObject_GetDrawData(object);
    Sprite *sprite = work->sprite;
    u32 direction;

    if (MapObject_IsBillboardPending(object) == TRUE || sprite == NULL) {
        return;
    }
    if (!work->active) {
        work->active = TRUE;
        if (MapObject_HasFollowMonSprite(object)) {
            MapObject_SetBillboardSepia((void *)TRUE, object, work->sprite);
        } else if (MapObject_IsShinyPokemon(object)) {
            FollowMonBillboard_SetResource(TRUE, object, sprite);
        }
    }
    direction = MapObject_GetFacingDirection(object);
    FollowMonBillboard_Update(object, sprite, work, direction, TRUE);
    work->direction = direction;
    MapObject_UpdateBillboardPosition(object, sprite);
    MapObject_UpdateBillboardVisibility(object, sprite);
}

void FollowMonDraw_UpdatePositionOnly(LocalMapObject *object) {
    FollowMonDrawWork *work = (FollowMonDrawWork *)MapObject_GetDrawData(object);
    Sprite *sprite;

    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    sprite = work->sprite;
    if (sprite == NULL) {
        return;
    }
    if (!work->active) {
        work->active = TRUE;
        if (MapObject_IsShinyPokemon(object)) {
            FollowMonBillboard_SetResource(TRUE, object, sprite);
        }
    }
    MapObject_UpdateBillboardPosition(object, sprite);
    MapObject_UpdateBillboardVisibility(object, sprite);
}

void BillboardMotion_Stand(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else {
        BillboardObject_AlignSequenceFrameTo8(billboard);
    }
    BillboardObject_AdvanceFrame(billboard, 0);
}

void BillboardMotion_AnimateHalfSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_CONST(0.5));
    }
}

void BillboardMotion_AnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    }
}

void BillboardMotion_AnimateDoubleSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, 2 * FX32_ONE);
    }
}

void BillboardMotion_AnimateQuadSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, 4 * FX32_ONE);
    }
}

void ov01_021F7A90(void *object, void *billboard, s8 *workRaw, int facing) {
    SteppedWalkWork *work = (SteppedWalkWork *)workRaw;
    if (facing != work->prevFacing) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        work->step = 0;
    }
    if (work->movement != 6) {
        work->step = 0;
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardFrameSteps7 steps = ov01_02208A64;
        BillboardObject_AdvanceFrame(billboard, steps.values[work->step]);
        work->step++;
        if (steps.values[work->step] == 0) {
            work->step = 0;
        }
    }
}

void ov01_021F7B14(void *object, void *billboard, s8 *workRaw, int facing) {
    SteppedWalkWork *work = (SteppedWalkWork *)workRaw;
    if (facing != work->prevFacing) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        work->step = 0;
    }
    if (work->movement != 7) {
        work->step = 0;
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardFrameSteps4 steps = ov01_02208A08;
        BillboardObject_AdvanceFrame(billboard, steps.values[work->step]);
        work->step++;
        if (steps.values[work->step] == 0) {
            work->step = 0;
        }
    }
}

void ov01_021F7B90(void *object, void *billboard, s8 *workRaw, int facing) {
    SteppedWalkWork *work = (SteppedWalkWork *)workRaw;
    if (facing != work->prevFacing) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        work->step = 0;
    }
    if (work->movement != 8) {
        work->step = 0;
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardFrameSteps8 steps = ov01_02208A80;
        BillboardObject_AdvanceFrame(billboard, steps.values[work->step]);
        work->step++;
        if (steps.values[work->step] == 0) {
            work->step = 0;
        }
    }
}

#pragma force_active on
void BillboardDraw_UpdatePcWoman(void *object) {
    typedef struct {
        u8 unk0[2];
        u8 facing;
        u8 movement;
        void *billboard;
    } Work;
    volatile int facing[1];
    Work *work[1];
    int movement;
    void *billboard;
    work[0] = (Work *)MapObject_GetDrawData(object);
    billboard = work[0]->billboard;
    if (MapObject_IsBillboardPending(object) != TRUE && billboard != NULL) {
        facing[0] = MapObject_GetFacingDirection(object);
        movement = MapObject_GetAnimation(object);
        if (movement >= 10) {
            GF_AssertFail();
        }
        gPcWomanBillboardMotions[movement](object, billboard, work[0], facing[0]);
        work[0]->facing = facing[0];
        work[0]->movement = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}
#pragma force_active reset

void ov01_021F7C80(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2] || work[3] == 9) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else {
        BillboardObject_AlignSequenceFrameTo8(billboard);
    }
    BillboardObject_AdvanceFrame(billboard, 0);
}

void ov01_021F7CBC(void *object, void *billboard, const s8 *work, int facing) {
    if (work[3] != 9) {
        BillboardObject_SetSequence(billboard, 4);
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    BillboardObject_AdvanceFrame(billboard, FX32_ONE);
}

#pragma force_active on
void ov01_021F7CE4(void *object) {
    typedef struct {
        s8 unk0;
        s8 unk1;
        s8 facing;
        volatile s8 movement;
        void *billboard;
    } Work;
    int facing;
    Work *work = (Work *)MapObject_GetDrawData(object);
    void *billboard = work->billboard;
    int sequence;
    if (MapObject_IsBillboardPending(object) != TRUE && billboard != NULL) {
        facing = MapObject_GetFacingDirection(object);
        if (MapObject_GetAnimation(object) == 0) {
            sequence = (u32)facing > 1 ? 1 : 0;
            if (sequence != BillboardObject_GetSequence(billboard) || work->movement == 0) {
                BillboardObject_SetSequence(billboard, sequence);
                BillboardObject_SetSequenceFrame(billboard, 0);
                BillboardObject_AdvanceFrame(billboard, 0);
            }
            BillboardObject_AdvanceFrame(billboard, 0);
        } else {
            int other = (u32)facing > 1 ? 1 : 0;
            if (other != BillboardObject_GetSequence(billboard)) {
                BillboardObject_SetSequence(billboard, other);
                BillboardObject_SetSequenceFrame(billboard, 0);
            }
            BillboardObject_AdvanceFrame(billboard, FX32_ONE);
        }
        work->facing = facing;
        work->movement = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}
#pragma force_active reset

void PlayerDraw_Init(void *object) {
    s8 *work = (s8 *)MapObject_InitDrawData(object, 0x14);
    work[0] = -1;
    MapObject_CreateBillboard(object, work + 4);
    if (*(void **)(work + 4) != NULL) {
        MapObject_CallDrawUpdate(object);
    }
}

void PlayerDraw_Delete(void *object) {
    VecFx32 offset = { 0, 0, 0 };
    u8 *work = MapObject_GetDrawData(object);
    MapObject_DeleteBillboard(object, work + 4);
    MapObject_SetFacingVector(object, &offset);
}

void PlayerDraw_Pause(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    void *billboard = *(void **)(work + 4);
    if (billboard != NULL) {
        BillboardObject_SaveState(billboard, work + 8);
    }
    MapObject_DeleteBillboard(object, work + 4);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK21);
}

void PlayerDraw_Resume(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    if (MapObject_IsBillboardPending(object) != TRUE) {
        if (*(void **)(work + 4) == NULL) {
            MapObject_CreateBillboard(object, work + 4);
        }
        if (*(void **)(work + 4) != NULL) {
            BillboardObject_RestoreState(*(void **)(work + 4), work + 8);
            MapObject_UpdateBillboardPosition(object, *(void **)(work + 4));
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK21);
        }
    }
}

void PlayerDraw_UpdateWalking(void *object) {
    int facing;
    int movement;
    s8 *work = (s8 *)MapObject_GetDrawData(object);
    void *billboard = *(void **)(work + 4);

    if (MapObject_IsBillboardPending(object) != TRUE && billboard != NULL) {
        facing = MapObject_GetFacingDirection(object);
        movement = MapObject_GetAnimation(object);
        if (movement >= 10) {
            GF_AssertFail();
        }
        gPlayerWalkingBillboardMotions[movement](object, billboard, work, facing);
        MapObject_GetFacingVector(object)->z = FX32_CONST(0.5);
        work[0] = facing;
        work[2] = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

void ov01_021F7EE4(void *object, void *billboard, HeldStepWork *work, int facing) {
#pragma unused(object)
    if (facing != work->facing) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        work->held = 0;
        return;
    }
    if (work->movement == 9) {
        work->held = 1;
        return;
    }
    if (work->held == 1) {
        BillboardObject_SetSequenceKeepAlignedFrameTo8(billboard, Facing_GetBillboardStandSequence(facing));
        work->held = 0;
        return;
    }
    BillboardObject_AlignSequenceFrameTo8(billboard);
}

void ov01_021F7F54(void *object, void *billboard, HeldStepWork *work, int facing) {
    if (facing != work->facing) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else if (work->movement == 9) {
        work->held = 1;
        return;
    } else if (work->held == 1) {
        BillboardObject_SetSequenceKeepAlignedFrameTo8(billboard, Facing_GetBillboardStandSequence(facing));
        work->held = 0;
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_CONST(0.5));
    }
}

void ov01_021F7FC8(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Walk(object, billboard, work, facing, FX32_ONE);
}

void ov01_021F801C(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Walk(object, billboard, work, facing, 2 * FX32_ONE);
}

void ov01_021F8070(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Walk(object, billboard, work, facing, 4 * FX32_ONE);
}

void ov01_021F80C4(void *object, void *billboard, s8 *work, int facing) {
    if (facing != work[0]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        work[1] = 0;
    } else if (work[2] == 9) {
        BillboardObject_SetSequenceKeepAlignedFrameTo8(billboard, Facing_GetBillboardStandSequence(facing));
    }
    if (work[2] != 6) {
        work[1] = 0;
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardFrameSteps7 steps = ov01_02208A48;
        BillboardObject_AdvanceFrame(billboard, steps.values[work[1]]);
        work[1]++;
        if (steps.values[work[1]] == 0) {
            work[1] = 0;
        }
    }
}

void ov01_021F8164(void *object, void *billboard, s8 *work, int facing) {
    if (facing != work[0]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        work[1] = 0;
    } else if (work[2] == 9) {
        BillboardObject_SetSequenceKeepAlignedFrameTo8(billboard, Facing_GetBillboardStandSequence(facing));
    }
    if (work[2] != 7) {
        work[1] = 0;
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardFrameSteps4 steps = ov01_022089D8;
        BillboardObject_AdvanceFrame(billboard, steps.values[work[1]]);
        work[1]++;
        if (steps.values[work[1]] == 0) {
            work[1] = 0;
        }
    }
}

void ov01_021F81FC(void *object, void *billboard, s8 *work, int facing) {
    if (facing != work[0]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        work[1] = 0;
    } else if (work[2] == 9) {
        BillboardObject_SetSequenceKeepAlignedFrameTo8(billboard, Facing_GetBillboardStandSequence(facing));
    }
    if (work[2] != 8) {
        work[1] = 0;
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardFrameSteps8 steps = ov01_02208AA0;
        BillboardObject_AdvanceFrame(billboard, steps.values[work[1]]);
        work[1]++;
        if (steps.values[work[1]] == 0) {
            work[1] = 0;
        }
    }
}

void ov01_021F829C(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[0]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardWalkSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else if (work[2] != 9) {
        BillboardObject_SetSequenceKeepAlignedFrame(billboard, Facing_GetBillboardWalkSequence(facing), 4);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    }
}

#pragma force_active on
void PlayerDraw_UpdateCycling(void *object) {
    HeldMotionWork *work = (HeldMotionWork *)MapObject_GetDrawData(object);
    int facing, movement;
    void *billboard = work->billboard;
    if (MapObject_IsBillboardPending(object) != TRUE && billboard != NULL) {
        facing = MapObject_GetFacingDirection(object);
        movement = MapObject_GetAnimation(object);
        if (movement >= 12) {
            GF_AssertFail();
        }
        if (work->movement == 0 && movement != work->movement && work->held == 0) {
            work->facing = -1;
        }
        gPlayerCyclingBillboardMotions[movement](object, billboard, work, facing);
        work->facing = facing;
        work->movement = movement;
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

void ov01_021F8374(void *object, void *billboard, HeldMotionWork *work, int facing) {
    BillboardFrameSteps4 sequences = ov01_02208A38;
    int sequence;
    sequence = sequences.values[facing];
    if (facing != work->facing) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
        work->held = 0;
    } else if (work->movement) {
        work->held = 1;
    } else if (work->held == 1) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
        work->held = 0;
    } else {
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    }
}
#pragma force_active reset

void ov01_021F8400(void *object, void *billboard, const s8 *work, int facing) {
    u32 sequence = ov01_021FA464(facing);
    if (facing != work[0] || work[2] != 0) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else if (work[2] != 10) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else if (BillboardObject_GetSequence(billboard) != sequence) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_CONST(0.5));
    }
}

void ov01_021F847C(void *object, void *billboard, const s8 *work, int facing) {
    u32 sequence = ov01_021FA464(facing);
    if (facing != work[0]) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else if (work[2] != 11) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else if (BillboardObject_GetSequence(billboard) != sequence) {
        BillboardObject_SetSequence(billboard, sequence);
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    }
}

#pragma force_active on
void PlayerDraw_UpdateFishing(void *object) {
    struct {
        volatile int value;
    } facing;
    FacingMotionWork *work = (FacingMotionWork *)MapObject_GetDrawData(object);
    void *billboard = work->billboard;
    int movement;
    if (billboard == NULL || MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    facing.value = MapObject_GetFacingDirection(object);
    movement = MapObject_GetAnimation(object);
    if (movement >= 4) {
        GF_AssertFail();
    }
    gPlayerFishingBillboardMotions[movement](object, billboard, work, facing.value);
    PlayerDraw_SetFishingOffset(object, billboard, facing.value);
    work->facing = facing.value;
    work->movement = MapObject_GetAnimation(object);
    MapObject_UpdateBillboardPosition(object, billboard);
    MapObject_UpdateBillboardVisibility(object, billboard);
}
#pragma force_active reset

void ov01_021F856C(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Fall(billboard, work, facing, 0);
}

void ov01_021F85BC(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 1, ov01_022089F8);
}

#pragma force_active on
void ov01_021F85F4(void *object, void *billboard, void *workRaw, int facing) {
    typedef struct {
        s8 facing;
        volatile s8 ticks;
        s8 movement;
        u8 unk3_0 : 1;
        u8 phase : 7;
    } Work;
    Work *work = workRaw;
    int frame;
    if (facing != work->facing || work->movement != 2) {
        BillboardObject_SetSequence(billboard, ov01_022089F8[facing]);
        BillboardObject_SetSequenceFrame(billboard, 15 * FX32_ONE);
        work->phase = 0;
    }
    frame = BillboardObject_GetSequenceFrame(billboard);
    switch (work->phase) {
    case 0:
        frame -= FX32_ONE;
        if (frame <= 10 * FX32_ONE) {
            work->phase = (u8)(work->phase + 1);
        }
        break;
    case 1:
        frame += FX32_ONE;
        if (frame >= 15 * FX32_ONE) {
            work->phase = (u8)(work->phase + 1);
            work->ticks = 0;
        }
        break;
    case 2:
        work->ticks++;
        if (work->ticks >= 30) {
            work->ticks = 0;
            work->phase = 0;
        }
        break;
    }
    BillboardObject_SetSequenceFrame(billboard, frame);
    BillboardObject_AdvanceFrame(billboard, 0);
}
#pragma force_active reset

void ov01_021F86B8(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Fall(billboard, work, facing, 3);
}

#pragma force_active on
void PlayerDraw_SetFishingOffset(void *object, void *billboard, u32 facing) {
    VecFx32 offset = { 0, 0, 0 };
    int frame = BillboardObject_GetFrame(billboard) / FX32_ONE;
    switch (facing) {
    case 0:
        break;
    case 1:
        if (frame >= 24) {
            offset.z = 6 * FX32_ONE;
        }
        break;
    case 2:
        if (frame < 40) {
            offset.x = FX32_ONE;
        } else {
            offset.x = -5 * FX32_ONE;
        }
        break;
    case 3:
        if (frame < 56) {
            offset.x = -FX32_ONE;
        } else {
            offset.x = 5 * FX32_ONE;
        }
        break;
    }
    offset.z += FX32_CONST(0.5);
    MapObject_SetFacingVector(object, &offset);
}

void PlayerDraw_UpdateLadder(void *object) {
    int facing;
    u8 *work = MapObject_GetDrawData(object);
    void *billboard = *(void **)(work + 4);
    int movement;
    BillboardMotionFunc func;
    if (billboard != NULL && MapObject_IsBillboardPending(object) != TRUE) {
        facing = MapObject_GetFacingDirection(object);
        movement = MapObject_GetAnimation(object);
        if (movement >= 2) {
            GF_AssertFail();
        }
        func = ov01_022089CC.walk[movement];
        func(object, billboard, work, facing);
        PlayerDraw_SetLadderOffset(object, billboard, facing);
        work[0] = facing;
        work[2] = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}
#pragma force_active reset

void ov01_021F8804(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 0, ov01_02208A18);
}

void ov01_021F883C(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 1, ov01_02208A18);
}

#pragma force_active on
void PlayerDraw_SetLadderOffset(void *object, void *billboard, u32 facing) {
    VecFx32 offset = { 0, 0, 0 };
    int frame = BillboardObject_GetFrame(billboard) / FX32_ONE;
    switch (facing) {
    case 0:
        break;
    case 1:
        if (frame >= 24) {
            offset.z = 6 * FX32_ONE;
        }
        break;
    case 2:
        if (frame < 40) {
            offset.x = FX32_ONE;
        } else {
            offset.x = -5 * FX32_ONE;
        }
        break;
    case 3:
        if (frame < 56) {
            offset.x = -FX32_ONE;
        } else {
            offset.x = 5 * FX32_ONE;
        }
        break;
    }
    MapObject_SetFacingVector(object, &offset);
}
#pragma force_active reset

void PlayerDraw_UpdateSurfing(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    void *billboard = *(void **)(work + 4);
    int facing;
    if (MapObject_IsBillboardPending(object) != TRUE && billboard != NULL) {
        facing = MapObject_GetFacingDirection(object);
        work[0] = facing;
        work[2] = MapObject_GetAnimation(object);
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
        BillboardObject_AdvanceFrame(billboard, 0);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

void PlayerDraw_UpdateUseHmOrSaving(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    void *billboard = *(void **)(work + 4);
    if (MapObject_IsBillboardPending(object) != TRUE && billboard != NULL) {
        work[0] = MapObject_GetFacingDirection(object);
        work[2] = MapObject_GetAnimation(object);
        if (BillboardObject_GetSequence(billboard) != FALSE) {
            BillboardObject_SetSequence(billboard, 0);
            BillboardObject_SetSequenceFrame(billboard, 0);
        }
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
        MapObject_GetFacingVector(object)->z = FX32_CONST(0.5);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

#pragma force_active on
void PlayerDraw_UpdateWatering(void *object) {
    FacingMotionWork *work = (FacingMotionWork *)MapObject_GetDrawData(object);
    void *billboard = work->billboard;
    int facing;
    if (MapObject_IsBillboardPending(object) == TRUE || billboard == NULL) {
        return;
    }
    facing = MapObject_GetFacingDirection(object);
    if (MapObject_GetAnimation(object) == 0) {
        if (BillboardObject_GetSequence(billboard) != 1) {
            BillboardObject_SetSequence(billboard, 1);
            BillboardObject_SetSequenceFrame(billboard, 0);
        }
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    } else {
        if (BillboardObject_GetSequence(billboard) != 0) {
            BillboardObject_SetSequence(billboard, 0);
            BillboardObject_SetSequenceFrame(billboard, 0);
        }
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    }
    work->facing = facing;
    work->movement = MapObject_GetAnimation(object);
    MapObject_UpdateBillboardPosition(object, billboard);
    MapObject_UpdateBillboardVisibility(object, billboard);
}
#pragma force_active reset

void PlayerDraw_UpdatePoketch(void *object) {
    s8 *work = (s8 *)MapObject_GetDrawData(object);
    void *billboard = *(void **)(work + 4);
    int facing;
    if (MapObject_IsBillboardPending(object) != TRUE && billboard != NULL) {
        facing = MapObject_GetFacingDirection(object);
        if (facing != work[0]) {
            BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
            BillboardObject_SetSequenceFrame(billboard, 0);
        }
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
        work[0] = facing;
        work[2] = MapObject_GetAnimation(object);
    }
}

#pragma force_active on
void PlayerDraw_UpdateHeal(void *object) {
    int movement;
    SignedFacingMotionWork *work = (SignedFacingMotionWork *)MapObject_GetDrawData(object);
    void *billboard = work->billboard;
    if (MapObject_IsBillboardPending(object) != TRUE && billboard) {
        int facing = MapObject_GetFacingDirection(object);
        movement = MapObject_GetAnimation(object);
        if (movement != work->movement) {
            if (movement == 0) {
                BillboardObject_SetSequence(billboard, 0);
            } else {
                BillboardObject_SetSequence(billboard, 1);
            }
            BillboardObject_SetSequenceFrame(billboard, 0);
        }
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
        MapObject_GetFacingVector(object)->z = FX32_CONST(0.5);
        work->facing = facing;
        work->movement = movement;
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

void PlayerDraw_UpdateApricornShake(void *object) {
    int facing;
    SignedFacingMotionWork *work = (SignedFacingMotionWork *)MapObject_GetDrawData(object);
    void *billboard = work->billboard;
    if (billboard && MapObject_IsBillboardPending(object) != TRUE) {
        facing = MapObject_GetFacingDirection(object);
        int movement = MapObject_GetAnimation(object);
        if (movement >= 1) {
            GF_AssertFail();
        }
        ov01_022089CC.stand[movement](object, billboard, work, facing);
        PlayerDraw_SetApricornShakeOffset(object, billboard, facing);
        work->facing = facing;
        work->movement = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}
#pragma force_active reset

void ov01_021F8BA8(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 0, ov01_02208A28);
}

void PlayerDraw_SetApricornShakeOffset(void *object, void *billboard, int facing) {
#pragma unused(billboard, facing)
    VecFx32 offset = { 0, 0, 0 };
    offset.z += FX32_CONST(0.5);
    MapObject_SetFacingVector(object, &offset);
}

// Rounds the sequence frame down to a multiple of divisor frames
void BillboardObject_AlignSequenceFrame(void *billboard, int divisor) {
    s32 frame = BillboardObject_GetSequenceFrame(billboard) / FX32_ONE;
    BillboardObject_SetSequenceFrame(billboard, (frame - frame % divisor) * FX32_ONE);
    BillboardObject_AdvanceFrame(billboard, 0);
}

void BillboardObject_AlignSequenceFrameTo8(void *billboard) {
    BillboardObject_AlignSequenceFrame(billboard, 8);
}

void BillboardObject_SetSequenceKeepAlignedFrame(void *billboard, u32 sequence, int divisor) {
    fx32 frame;
    BillboardObject_AlignSequenceFrame(billboard, divisor);
    frame = BillboardObject_GetSequenceFrame(billboard);
    BillboardObject_SetSequence(billboard, sequence);
    BillboardObject_SetSequenceFrame(billboard, frame);
}

void BillboardObject_SetSequenceKeepAlignedFrameTo8(void *billboard, u32 sequence) {
    fx32 frame;
    BillboardObject_AlignSequenceFrameTo8(billboard);
    frame = BillboardObject_GetSequenceFrame(billboard);
    BillboardObject_SetSequence(billboard, sequence);
    BillboardObject_SetSequenceFrame(billboard, frame);
}

void MapObject_UpdateBillboardVisibility(void *object, void *billboard) {
    int visible = TRUE;
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        visible = FALSE;
    }
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK12) == TRUE && MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK13) == FALSE) {
        visible = FALSE;
    }
    BillboardObject_SetVisible(billboard, visible);
}

void MapObject_GetEffectModelOffset(u32 spriteId, VecFx32 *offset) {
    const u32 *entry = ov01_022073D8;
    while (entry[0] != 0xFFFF) {
        if (entry[0] == spriteId) {
            *offset = *(const VecFx32 *)(entry + 1);
            return;
        }
        entry += 4;
    }
    GF_AssertFail();
}

void ModelObjectDraw_Init(void *object) {
    void **effect = (void **)MapObject_InitDrawData(object, 4);
    VecFx32 offset;
    MapObject_GetEffectModelOffset(MapObject_GetSpriteID(object), &offset);
    *effect = MapObjectModel_CreateEffect(object, &offset);
}

void ModelObjectDraw_Update(void) {
}

void ModelObjectDraw_Delete(void *object) {
    void **effect = (void **)MapObject_GetDrawData(object);
    if (*effect != NULL) {
        FieldEffectManager_DestroyEffect((int)*effect);
        *effect = NULL;
    }
}

void ModelObjectDraw_Pause(void *object) {
    void **effect = (void **)MapObject_GetDrawData(object);
    if (*effect != NULL) {
        FieldEffectManager_DestroyEffect((int)*effect);
        *effect = NULL;
    }
}

void ModelObjectDraw_Resume(void *object) {
    void **effect = (void **)MapObject_GetDrawData(object);
    VecFx32 offset;
    MapObject_GetEffectModelOffset(MapObject_GetSpriteID(object), &offset);
    *effect = MapObjectModel_CreateEffect(object, &offset);
}
