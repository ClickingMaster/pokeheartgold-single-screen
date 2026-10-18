#include "field/map_object_billboard_motion.h"

#include "global.h"

#include "constants/scrcmd.h"
#include "constants/sprites.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"
#include "field/field_follow_mon.h"
#include "field/map_object_model_list.h"
#include "field/map_object_models.h"
#include "field/map_object_render.h"

#include "field_system.h"
#include "follow_mon.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "party.h"
#include "pokemon.h"
#include "script_pokemon_util.h"

// PlayerBillboardMotion_FishingBite: the rod bobs while a Pokemon bites
enum FishingBiteBillboardPhase {
    FISHING_BITE_PHASE_DIP,
    FISHING_BITE_PHASE_RISE,
    FISHING_BITE_PHASE_PAUSE,
};

// Billboard sequence callback: called with the map object, its billboard, the object's draw work and the facing
// direction.
typedef void (*BillboardMotionFunc)(void *object, void *billboard, void *work, int facing);

typedef struct LadderBillboardMotions {
    BillboardMotionFunc stand[1];
    BillboardMotionFunc walk[2];
} LadderBillboardMotions;

typedef struct BillboardFrameSteps4 {
    fx32 values[4];
} BillboardFrameSteps4;

typedef struct BillboardFacingSequences {
    u32 values[4];
} BillboardFacingSequences;

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
    u8 unk_01;
    s8 movement;
    u8 held : 1;
    u8 rest : 7;
    void *billboard;
} HeldMotionWork;

typedef struct FacingMotionWork {
    u8 facing;
    u8 unk_01;
    u8 movement;
    u8 unk_03;
    void *billboard;
} FacingMotionWork;

typedef struct SignedFacingMotionWork {
    s8 facing;
    u8 unk_01;
    s8 movement;
    u8 unk_03;
    void *billboard;
} SignedFacingMotionWork;

extern const u32 gEffectModelOffsets[];

void *ApricornTreeObject_GetBillboard(LocalMapObject *object);
void BillboardObject_SetVisible(void *billboard, u8 visible);
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
void FollowMonDraw_ClearBob(void *object);
void BillboardDraw_UpdateNpc(void *object);
void BillboardDraw_UpdatePositionOnly(void *object);
void BillboardDraw_UpdateLooping(void *object);
void FollowMonDraw_UpdateLarge(LocalMapObject *object);
void FollowMonDraw_Update(LocalMapObject *object);
static void BillboardMotion_Stand(void *object, void *billboard, const s8 *work, int facing);
static void BillboardMotion_AnimateHalfSpeed(void *object, void *billboard, const s8 *work, int facing);
static void BillboardMotion_AnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing);
static void BillboardMotion_AnimateDoubleSpeed(void *object, void *billboard, const s8 *work, int facing);
static void BillboardMotion_AnimateQuadSpeed(void *object, void *billboard, const s8 *work, int facing);
static void BillboardMotion_WalkSlightlyFast(void *object, void *billboard, s8 *workRaw, int facing);
static void BillboardMotion_WalkSlightlyFaster(void *object, void *billboard, s8 *workRaw, int facing);
static void BillboardMotion_WalkEverSoSlightlyFast(void *object, void *billboard, s8 *workRaw, int facing);
void BillboardDraw_UpdatePcWoman(void *object);
static void BillboardMotion_PcWomanStand(void *object, void *billboard, const s8 *work, int facing);
static void BillboardMotion_PcWomanBow(void *object, void *billboard, const s8 *work, int facing);
void BillboardDraw_UpdateTwoSequences(void *object);
void PlayerDraw_Init(void *object);
void PlayerDraw_Delete(void *object);
void PlayerDraw_Pause(void *object);
void PlayerDraw_Resume(void *object);
void PlayerDraw_UpdateWalking(void *object);
static void PlayerBillboardMotion_Stand(void *object, void *billboard, HeldStepWork *work, int facing);
static void PlayerBillboardMotion_AnimateHalfSpeed(void *object, void *billboard, HeldStepWork *work, int facing);
static void PlayerBillboardMotion_AnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing);
static void PlayerBillboardMotion_AnimateDoubleSpeed(void *object, void *billboard, const s8 *work, int facing);
static void PlayerBillboardMotion_AnimateQuadSpeed(void *object, void *billboard, const s8 *work, int facing);
static void PlayerBillboardMotion_WalkSlightlyFast(void *object, void *billboard, s8 *work, int facing);
static void PlayerBillboardMotion_WalkSlightlyFaster(void *object, void *billboard, s8 *work, int facing);
static void PlayerBillboardMotion_WalkEverSoSlightlyFast(void *object, void *billboard, s8 *work, int facing);
static void PlayerBillboardMotion_Run(void *object, void *billboard, const s8 *work, int facing);
void PlayerDraw_UpdateCycling(void *object);
static void PlayerBillboardMotion_CyclingStand(void *object, void *billboard, HeldMotionWork *work, int facing);
static void PlayerBillboardMotion_CyclingAnimateHalfSpeed(void *object, void *billboard, const s8 *work, int facing);
static void PlayerBillboardMotion_CyclingAnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing);
void PlayerDraw_UpdateFishing(void *object);
static void PlayerBillboardMotion_FishingIdle(void *object, void *billboard, const s8 *work, int facing);
static void PlayerBillboardMotion_FishingCast(void *object, void *billboard, const s8 *work, int facing);
static void PlayerBillboardMotion_FishingBite(void *object, void *billboard, void *workRaw, int facing);
static void PlayerBillboardMotion_FishingReel(void *object, void *billboard, const s8 *work, int facing);
static void PlayerDraw_SetFishingOffset(LocalMapObject *object, void *billboard, u32 facing);
void PlayerDraw_UpdateLadder(void *object);
static void PlayerBillboardMotion_Ladder0(void *object, void *billboard, const s8 *work, int facing);
static void PlayerBillboardMotion_Ladder1(void *object, void *billboard, const s8 *work, int facing);
static void PlayerDraw_SetLadderOffset(LocalMapObject *object, void *billboard, u32 facing);
void PlayerDraw_UpdateSurfing(void *object);
void PlayerDraw_UpdateUseHmOrSaving(void *object);
void PlayerDraw_UpdateWatering(void *object);
void PlayerDraw_UpdatePoketch(void *object);
void PlayerDraw_UpdateHeal(void *object);
void PlayerDraw_UpdateApricornShake(void *object);
static void PlayerBillboardMotion_ApricornShake(void *object, void *billboard, const s8 *work, int facing);
static void PlayerDraw_SetApricornShakeOffset(LocalMapObject *object, void *billboard, int facing);
static void BillboardObject_AlignSequenceFrame(void *billboard, int divisor);
static void BillboardObject_AlignSequenceFrameTo8(void *billboard);
static void BillboardObject_SetSequenceKeepAlignedFrame(void *billboard, u32 sequence, int divisor);
static void BillboardObject_SetSequenceKeepAlignedFrameTo8(void *billboard, u32 sequence);
static void MapObject_UpdateBillboardVisibility(LocalMapObject *object, void *billboard);
static void MapObject_GetEffectModelOffset(u32 spriteId, VecFx32 *offset);
void ModelObjectDraw_Init(void *object);
void ModelObjectDraw_Update(void);
void ModelObjectDraw_Delete(void *object);
void ModelObjectDraw_Pause(void *object);
void ModelObjectDraw_Resume(void *object);

static const LadderBillboardMotions sPlayerApricornLadderBillboardMotions = {
    .stand = {
              (BillboardMotionFunc)PlayerBillboardMotion_ApricornShake,
              },
    .walk = {
              (BillboardMotionFunc)PlayerBillboardMotion_Ladder0,
              (BillboardMotionFunc)PlayerBillboardMotion_Ladder1,
              },
};

static const BillboardFrameSteps4 sWalkSlightlyFasterFrameSteps = {
    .values = { FX32_CONST(3), FX32_CONST(2), FX32_CONST(3), 0 },
};

static const u32 sPlayerFishingSequences[4] = { 0, 1, 2, 3 };

static const BillboardMotionFunc sPlayerFishingBillboardMotions[4] = {
    (BillboardMotionFunc)PlayerBillboardMotion_FishingIdle,
    (BillboardMotionFunc)PlayerBillboardMotion_FishingCast,
    PlayerBillboardMotion_FishingBite,
    (BillboardMotionFunc)PlayerBillboardMotion_FishingReel,
};

static const u32 sPlayerLadderSequences[4] = { 0, 1, 2, 3 };

static const u32 sPlayerApricornShakeSequences[4] = { 0, 1, 2, 3 };

static const BillboardFrameSteps4 sPlayerWalkSlightlyFasterFrameSteps = {
    .values = { FX32_CONST(3), FX32_CONST(2), FX32_CONST(3), 0 },
};

static const BillboardFacingSequences sPlayerCyclingStandSequences = {
    .values = { 6, 7, 8, 9 },
};

static const BillboardFrameSteps7 sWalkSlightlyFastFrameSteps = {
    .values = { FX32_ONE, FX32_ONE, FX32_CONST(2), FX32_ONE, FX32_ONE, FX32_CONST(2), 0 },
};

static const BillboardFrameSteps7 sPlayerWalkSlightlyFastFrameSteps = {
    .values = { FX32_ONE, FX32_ONE, FX32_CONST(2), FX32_ONE, FX32_ONE, FX32_CONST(2), 0 },
};

static const BillboardFrameSteps8 sWalkEverSoSlightlyFastFrameSteps = {
    .values = { FX32_CONST(2), FX32_ONE, FX32_ONE, FX32_ONE, FX32_ONE, FX32_ONE, FX32_ONE, 0 },
};

static const BillboardFrameSteps8 sPlayerWalkEverSoSlightlyFastFrameSteps = {
    .values = { FX32_CONST(2), FX32_ONE, FX32_ONE, FX32_ONE, FX32_ONE, FX32_ONE, FX32_ONE, 0 },
};

static const BillboardMotionFunc sNpcBillboardMotions[9] = {
    (BillboardMotionFunc)BillboardMotion_Stand,
    (BillboardMotionFunc)BillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateNormalSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateDoubleSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateQuadSpeed,
    (BillboardMotionFunc)BillboardMotion_WalkSlightlyFast,
    (BillboardMotionFunc)BillboardMotion_WalkSlightlyFaster,
    (BillboardMotionFunc)BillboardMotion_WalkEverSoSlightlyFast,
};

static const BillboardMotionFunc sPlayerCyclingBillboardMotions[10] = {
    (BillboardMotionFunc)PlayerBillboardMotion_CyclingStand,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateNormalSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateDoubleSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_WalkSlightlyFast,
    (BillboardMotionFunc)PlayerBillboardMotion_WalkSlightlyFast,
    (BillboardMotionFunc)PlayerBillboardMotion_WalkSlightlyFast,
    (BillboardMotionFunc)PlayerBillboardMotion_CyclingAnimateHalfSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_CyclingAnimateNormalSpeed,
};

static const BillboardMotionFunc sPcWomanBillboardMotions[10] = {
    (BillboardMotionFunc)BillboardMotion_PcWomanStand,
    (BillboardMotionFunc)BillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateNormalSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateDoubleSpeed,
    (BillboardMotionFunc)BillboardMotion_AnimateQuadSpeed,
    (BillboardMotionFunc)BillboardMotion_WalkSlightlyFast,
    (BillboardMotionFunc)BillboardMotion_WalkSlightlyFaster,
    (BillboardMotionFunc)BillboardMotion_WalkEverSoSlightlyFast,
    (BillboardMotionFunc)BillboardMotion_PcWomanBow,
};

static const BillboardMotionFunc sPlayerWalkingBillboardMotions[10] = {
    (BillboardMotionFunc)PlayerBillboardMotion_Stand,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateHalfSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateNormalSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateDoubleSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_AnimateQuadSpeed,
    (BillboardMotionFunc)PlayerBillboardMotion_WalkSlightlyFast,
    (BillboardMotionFunc)PlayerBillboardMotion_WalkSlightlyFaster,
    (BillboardMotionFunc)PlayerBillboardMotion_WalkEverSoSlightlyFast,
    (BillboardMotionFunc)PlayerBillboardMotion_Run,
};

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
        BillboardObject_SetSequence(billboard, sPlayerFishingSequences[facing]);
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
    case SPRITE_HERO:
    case SPRITE_CYCLEHERO:
    case SPRITE_HEROINE:
    case SPRITE_CYCLEHEROINE:
    case SPRITE_SPHERO:
    case SPRITE_SPHEROINE:
    case SPRITE_SWIMHERO:
    case SPRITE_SWIMHEROINE:
    case SPRITE_WATERHERO:
    case SPRITE_WATERHEROINE:
    case SPRITE_FISHINGHERO:
    case SPRITE_FISH_HEROINE:
    case SPRITE_POKEHERO:
    case SPRITE_POKEHEROINE:
    case SPRITE_SAVEHERO:
    case SPRITE_SAVEHEROINE:
    case SPRITE_BANZAIHERO:
    case SPRITE_BANZAIHEROINE:
    case SPRITE_LADDERHERO:
    case SPRITE_LADDERHEROINE:
    case SPRITE_RHERO:
    case SPRITE_RHEROINE:
    case SPRITE_RBANZAIHERO:
    case SPRITE_RBANZAIINE:
        return ((void **)MapObject_GetDrawData(object))[1];
    }

    if (spriteId >= SPRITE_FOLLOWER_MON_BULBASAUR && spriteId <= SPRITE_FOLLOWER_MON_ARCEUS_DARK) {
        return ((void **)MapObject_GetDrawData(object))[0];
    }
    if (spriteId >= SPRITE_BONGURI && spriteId <= SPRITE_BONGURI_BK) {
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
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_InitDrawData(object, sizeof(FollowMonBillboardWork));
    work->direction = -1;
    MapObject_CreateBillboard(object, work);
    work->bobbing = 0;
    work->spriteLoaded = FALSE;
    work->paletteApplied = FALSE;
    if (work->billboard != NULL) {
        MapObject_CallDrawUpdate(object);
    }
}

void FollowMonDraw_Delete(void *object) {
    MapObject_DeleteBillboard(object, MapObject_GetDrawData(object));
}

void FollowMonDraw_Pause(void *object) {
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    Sprite *billboard = work->billboard;
    if (billboard != NULL) {
        BillboardObject_SaveState(billboard, &work->savedState);
    }
    work->spriteLoaded = FALSE;
    work->paletteApplied = FALSE;
    MapObject_DeleteBillboard(object, work);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK21);
}

// Draw callback of the following Pokemon: (re)loads the party leader's overworld sprite on first use.
void FollowMonDraw_Resume(LocalMapObject *object) {
    VecFx32 pos;
    VecFx32 facing;
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    if (!work->spriteLoaded && MapObject_GetID(object) == obj_partner_poke) {
        FieldSystem *fieldSystem = MapObjectManager_GetFieldSystem(MapObject_GetManager(object));
        Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        int species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        int form = GetMonData(mon, MON_DATA_FORM, NULL);
        u8 gender = GetMonGender(mon);
        u32 spriteId = FollowMon_GetSpriteID(species, form, gender);
        u8 shiny = MonIsShiny(mon);
        int x, y, z;
        work->spriteLoaded = TRUE;
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
        FollowMonMood_SetMon(fieldSystem->followMonMood, mon, species, GetMonData(mon, MON_DATA_PERSONALITY, NULL));
    }
    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    if (work->billboard == NULL) {
        MapObject_CreateBillboard(object, work);
    }
    if (work->billboard != NULL) {
        work->paletteApplied = TRUE;
        {
            VecFx32 offset = { 0, 0, 0 };
            MapObject_CopyFacingVector(object, &facing);
            MapObject_GetSpriteID(object);
            FollowMonBillboard_AddFacingOffset(object, MapObject_GetFacingDirection(object), &offset);
            facing.x = offset.x;
            facing.z = offset.z;
            MapObject_SetFacingVector(object, &facing);
            if (MapObject_HasFollowMonSprite(object)) {
                MapObject_SetBillboardSepia((void *)TRUE, object, work->billboard);
            } else if (MapObject_IsShinyPokemon(object)) {
                FollowMonBillboard_SetResource(TRUE, object, work->billboard);
            }
            BillboardObject_RestoreState(work->billboard, &work->savedState);
            MapObject_UpdateBillboardPosition(object, work->billboard);
            MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK21);
            MapObject_CallDrawUpdate(object);
        }
    }
}

void FollowMonDraw_ClearBob(void *object) {
    u8 *work = MapObject_GetDrawData(object);
    work[0x15] = 0;
    MapObject_GetFacingVector(object)->y = 0;
}

void *MapObjectManager_GetFollowMonBillboard(MapObjectManager *manager) {
    return ((FollowMonBillboardWork *)MapObject_GetDrawData(MapObjectManager_GetFirstActiveObjectByID(manager, obj_partner_poke)))->billboard;
}

void BillboardDraw_UpdateNpc(void *object) {
    typedef struct {
        u8 unk_00;
        u8 unk_01;
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
    sNpcBillboardMotions[movement](object, billboard, work, facing.value);
    MapObject_GetFacingVector(object)->z = FX32_CONST(0.5);
    work->facing = facing.value;
    work->movement = MapObject_GetAnimation(object);
    MapObject_UpdateBillboardPosition(object, billboard);
    MapObject_UpdateBillboardVisibility(object, billboard);
}

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
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    Sprite *sprite = work->billboard;
    u32 direction;

    if (MapObject_IsBillboardPending(object) == TRUE || sprite == NULL) {
        return;
    }
    if (!work->paletteApplied) {
        work->paletteApplied = TRUE;
        if (MapObject_HasFollowMonSprite(object)) {
            MapObject_SetBillboardSepia((void *)TRUE, object, work->billboard);
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
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    Sprite *sprite = work->billboard;
    u32 direction;

    if (MapObject_IsBillboardPending(object) == TRUE || sprite == NULL) {
        return;
    }
    if (!work->paletteApplied) {
        work->paletteApplied = TRUE;
        if (MapObject_HasFollowMonSprite(object)) {
            MapObject_SetBillboardSepia((void *)TRUE, object, work->billboard);
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
    FollowMonBillboardWork *work = (FollowMonBillboardWork *)MapObject_GetDrawData(object);
    Sprite *sprite;

    if (MapObject_IsBillboardPending(object) == TRUE) {
        return;
    }
    sprite = work->billboard;
    if (sprite == NULL) {
        return;
    }
    if (!work->paletteApplied) {
        work->paletteApplied = TRUE;
        if (MapObject_IsShinyPokemon(object)) {
            FollowMonBillboard_SetResource(TRUE, object, sprite);
        }
    }
    MapObject_UpdateBillboardPosition(object, sprite);
    MapObject_UpdateBillboardVisibility(object, sprite);
}

static void BillboardMotion_Stand(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else {
        BillboardObject_AlignSequenceFrameTo8(billboard);
    }
    BillboardObject_AdvanceFrame(billboard, 0);
}

static void BillboardMotion_AnimateHalfSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_CONST(0.5));
    }
}

static void BillboardMotion_AnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, FX32_ONE);
    }
}

static void BillboardMotion_AnimateDoubleSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, 2 * FX32_ONE);
    }
}

static void BillboardMotion_AnimateQuadSpeed(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2]) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    if (!MapObject_IsBillboardAnimPaused(object)) {
        BillboardObject_AdvanceFrame(billboard, 4 * FX32_ONE);
    }
}

static void BillboardMotion_WalkSlightlyFast(void *object, void *billboard, s8 *workRaw, int facing) {
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
        BillboardFrameSteps7 steps = sWalkSlightlyFastFrameSteps;
        BillboardObject_AdvanceFrame(billboard, steps.values[work->step]);
        work->step++;
        if (steps.values[work->step] == 0) {
            work->step = 0;
        }
    }
}

static void BillboardMotion_WalkSlightlyFaster(void *object, void *billboard, s8 *workRaw, int facing) {
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
        BillboardFrameSteps4 steps = sWalkSlightlyFasterFrameSteps;
        BillboardObject_AdvanceFrame(billboard, steps.values[work->step]);
        work->step++;
        if (steps.values[work->step] == 0) {
            work->step = 0;
        }
    }
}

static void BillboardMotion_WalkEverSoSlightlyFast(void *object, void *billboard, s8 *workRaw, int facing) {
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
        BillboardFrameSteps8 steps = sWalkEverSoSlightlyFastFrameSteps;
        BillboardObject_AdvanceFrame(billboard, steps.values[work->step]);
        work->step++;
        if (steps.values[work->step] == 0) {
            work->step = 0;
        }
    }
}

void BillboardDraw_UpdatePcWoman(void *object) {
    typedef struct {
        u8 unk_00[2];
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
        sPcWomanBillboardMotions[movement](object, billboard, work[0], facing[0]);
        work[0]->facing = facing[0];
        work[0]->movement = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

static void BillboardMotion_PcWomanStand(void *object, void *billboard, const s8 *work, int facing) {
    if (facing != work[2] || work[3] == 9) {
        BillboardObject_SetSequence(billboard, Facing_GetBillboardStandSequence(facing));
        BillboardObject_SetSequenceFrame(billboard, 0);
    } else {
        BillboardObject_AlignSequenceFrameTo8(billboard);
    }
    BillboardObject_AdvanceFrame(billboard, 0);
}

static void BillboardMotion_PcWomanBow(void *object, void *billboard, const s8 *work, int facing) {
    if (work[3] != 9) {
        BillboardObject_SetSequence(billboard, 4);
        BillboardObject_SetSequenceFrame(billboard, 0);
    }
    BillboardObject_AdvanceFrame(billboard, FX32_ONE);
}

void BillboardDraw_UpdateTwoSequences(void *object) {
    typedef struct {
        s8 unk_00;
        s8 unk_01;
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
        sPlayerWalkingBillboardMotions[movement](object, billboard, work, facing);
        MapObject_GetFacingVector(object)->z = FX32_CONST(0.5);
        work[0] = facing;
        work[2] = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

static void PlayerBillboardMotion_Stand(void *object, void *billboard, HeldStepWork *work, int facing) {
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

static void PlayerBillboardMotion_AnimateHalfSpeed(void *object, void *billboard, HeldStepWork *work, int facing) {
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

static void PlayerBillboardMotion_AnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Walk(object, billboard, work, facing, FX32_ONE);
}

static void PlayerBillboardMotion_AnimateDoubleSpeed(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Walk(object, billboard, work, facing, 2 * FX32_ONE);
}

static void PlayerBillboardMotion_AnimateQuadSpeed(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Walk(object, billboard, work, facing, 4 * FX32_ONE);
}

static void PlayerBillboardMotion_WalkSlightlyFast(void *object, void *billboard, s8 *work, int facing) {
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
        BillboardFrameSteps7 steps = sPlayerWalkSlightlyFastFrameSteps;
        BillboardObject_AdvanceFrame(billboard, steps.values[work[1]]);
        work[1]++;
        if (steps.values[work[1]] == 0) {
            work[1] = 0;
        }
    }
}

static void PlayerBillboardMotion_WalkSlightlyFaster(void *object, void *billboard, s8 *work, int facing) {
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
        BillboardFrameSteps4 steps = sPlayerWalkSlightlyFasterFrameSteps;
        BillboardObject_AdvanceFrame(billboard, steps.values[work[1]]);
        work[1]++;
        if (steps.values[work[1]] == 0) {
            work[1] = 0;
        }
    }
}

static void PlayerBillboardMotion_WalkEverSoSlightlyFast(void *object, void *billboard, s8 *work, int facing) {
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
        BillboardFrameSteps8 steps = sPlayerWalkEverSoSlightlyFastFrameSteps;
        BillboardObject_AdvanceFrame(billboard, steps.values[work[1]]);
        work[1]++;
        if (steps.values[work[1]] == 0) {
            work[1] = 0;
        }
    }
}

static void PlayerBillboardMotion_Run(void *object, void *billboard, const s8 *work, int facing) {
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
        sPlayerCyclingBillboardMotions[movement](object, billboard, work, facing);
        work->facing = facing;
        work->movement = movement;
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

static void PlayerBillboardMotion_CyclingStand(void *object, void *billboard, HeldMotionWork *work, int facing) {
    BillboardFacingSequences sequences = sPlayerCyclingStandSequences;
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

static void PlayerBillboardMotion_CyclingAnimateHalfSpeed(void *object, void *billboard, const s8 *work, int facing) {
    u32 sequence = MapObjectRender_GetFacingSequence(facing);
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

static void PlayerBillboardMotion_CyclingAnimateNormalSpeed(void *object, void *billboard, const s8 *work, int facing) {
    u32 sequence = MapObjectRender_GetFacingSequence(facing);
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
    sPlayerFishingBillboardMotions[movement](object, billboard, work, facing.value);
    PlayerDraw_SetFishingOffset(object, billboard, facing.value);
    work->facing = facing.value;
    work->movement = MapObject_GetAnimation(object);
    MapObject_UpdateBillboardPosition(object, billboard);
    MapObject_UpdateBillboardVisibility(object, billboard);
}

static void PlayerBillboardMotion_FishingIdle(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Fall(billboard, work, facing, 0);
}

static void PlayerBillboardMotion_FishingCast(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 1, sPlayerFishingSequences);
}

static void PlayerBillboardMotion_FishingBite(void *object, void *billboard, void *workRaw, int facing) {
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
        BillboardObject_SetSequence(billboard, sPlayerFishingSequences[facing]);
        BillboardObject_SetSequenceFrame(billboard, 15 * FX32_ONE);
        work->phase = FISHING_BITE_PHASE_DIP;
    }
    frame = BillboardObject_GetSequenceFrame(billboard);
    switch (work->phase) {
    case FISHING_BITE_PHASE_DIP:
        frame -= FX32_ONE;
        if (frame <= 10 * FX32_ONE) {
            work->phase = (u8)(work->phase + 1);
        }
        break;
    case FISHING_BITE_PHASE_RISE:
        frame += FX32_ONE;
        if (frame >= 15 * FX32_ONE) {
            work->phase = (u8)(work->phase + 1);
            work->ticks = 0;
        }
        break;
    case FISHING_BITE_PHASE_PAUSE:
        work->ticks++;
        if (work->ticks >= 30) {
            work->ticks = 0;
            work->phase = FISHING_BITE_PHASE_DIP;
        }
        break;
    }
    BillboardObject_SetSequenceFrame(billboard, frame);
    BillboardObject_AdvanceFrame(billboard, 0);
}

static void PlayerBillboardMotion_FishingReel(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Fall(billboard, work, facing, 3);
}

static void PlayerDraw_SetFishingOffset(LocalMapObject *object, void *billboard, u32 facing) {
    VecFx32 offset = { 0, 0, 0 };
    int frame = BillboardObject_GetFrame(billboard) / FX32_ONE;
    switch (facing) {
    case DIR_NORTH:
        break;
    case DIR_SOUTH:
        if (frame >= 24) {
            offset.z = 6 * FX32_ONE;
        }
        break;
    case DIR_WEST:
        if (frame < 40) {
            offset.x = FX32_ONE;
        } else {
            offset.x = -5 * FX32_ONE;
        }
        break;
    case DIR_EAST:
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
        func = sPlayerApricornLadderBillboardMotions.walk[movement];
        func(object, billboard, work, facing);
        PlayerDraw_SetLadderOffset(object, billboard, facing);
        work[0] = facing;
        work[2] = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

static void PlayerBillboardMotion_Ladder0(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 0, sPlayerLadderSequences);
}

static void PlayerBillboardMotion_Ladder1(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 1, sPlayerLadderSequences);
}

static void PlayerDraw_SetLadderOffset(LocalMapObject *object, void *billboard, u32 facing) {
    VecFx32 offset = { 0, 0, 0 };
    int frame = BillboardObject_GetFrame(billboard) / FX32_ONE;
    switch (facing) {
    case DIR_NORTH:
        break;
    case DIR_SOUTH:
        if (frame >= 24) {
            offset.z = 6 * FX32_ONE;
        }
        break;
    case DIR_WEST:
        if (frame < 40) {
            offset.x = FX32_ONE;
        } else {
            offset.x = -5 * FX32_ONE;
        }
        break;
    case DIR_EAST:
        if (frame < 56) {
            offset.x = -FX32_ONE;
        } else {
            offset.x = 5 * FX32_ONE;
        }
        break;
    }
    MapObject_SetFacingVector(object, &offset);
}

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
        sPlayerApricornLadderBillboardMotions.stand[movement](object, billboard, work, facing);
        PlayerDraw_SetApricornShakeOffset(object, billboard, facing);
        work->facing = facing;
        work->movement = MapObject_GetAnimation(object);
        MapObject_UpdateBillboardPosition(object, billboard);
        MapObject_UpdateBillboardVisibility(object, billboard);
    }
}

static void PlayerBillboardMotion_ApricornShake(void *object, void *billboard, const s8 *work, int facing) {
    BillboardMotion_Loop(billboard, work, facing, 0, sPlayerApricornShakeSequences);
}

static void PlayerDraw_SetApricornShakeOffset(LocalMapObject *object, void *billboard, int facing) {
#pragma unused(billboard, facing)
    VecFx32 offset = { 0, 0, 0 };
    offset.z += FX32_CONST(0.5);
    MapObject_SetFacingVector(object, &offset);
}

// Rounds the sequence frame down to a multiple of divisor frames
static void BillboardObject_AlignSequenceFrame(void *billboard, int divisor) {
    s32 frame = BillboardObject_GetSequenceFrame(billboard) / FX32_ONE;
    BillboardObject_SetSequenceFrame(billboard, (frame - frame % divisor) * FX32_ONE);
    BillboardObject_AdvanceFrame(billboard, 0);
}

static void BillboardObject_AlignSequenceFrameTo8(void *billboard) {
    BillboardObject_AlignSequenceFrame(billboard, 8);
}

static void BillboardObject_SetSequenceKeepAlignedFrame(void *billboard, u32 sequence, int divisor) {
    fx32 frame;
    BillboardObject_AlignSequenceFrame(billboard, divisor);
    frame = BillboardObject_GetSequenceFrame(billboard);
    BillboardObject_SetSequence(billboard, sequence);
    BillboardObject_SetSequenceFrame(billboard, frame);
}

static void BillboardObject_SetSequenceKeepAlignedFrameTo8(void *billboard, u32 sequence) {
    fx32 frame;
    BillboardObject_AlignSequenceFrameTo8(billboard);
    frame = BillboardObject_GetSequenceFrame(billboard);
    BillboardObject_SetSequence(billboard, sequence);
    BillboardObject_SetSequenceFrame(billboard, frame);
}

static void MapObject_UpdateBillboardVisibility(LocalMapObject *object, void *billboard) {
    int visible = TRUE;
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        visible = FALSE;
    }
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK12) == TRUE && MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK13) == FALSE) {
        visible = FALSE;
    }
    BillboardObject_SetVisible(billboard, visible);
}

static void MapObject_GetEffectModelOffset(u32 spriteId, VecFx32 *offset) {
    const u32 *entry = gEffectModelOffsets;
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
