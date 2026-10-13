#ifndef POKEHEARTGOLD_MAP_OBJECT_FOLLOW_INTERNAL_H
#define POKEHEARTGOLD_MAP_OBJECT_FOLLOW_INTERNAL_H

#include "field_system.h"
#include "map_object.h"

// Movement state of the follow-player movement types (the following Pokemon and objects that trail the player).
typedef struct FollowPlayerMovementState {
    u8 state;
    u8 playerKnown;
    u8 hopCount;
    u8 hopFrame;
    s16 playerX;
    s16 playerZ;
    u16 unk8;
    u16 noBallEffect : 1;
    u16 hopSpeed : 2;
} FollowPlayerMovementState;

// Movement state of the follow-partner movement type (the second trainer of a pair walks behind the first).
typedef struct FollowPartnerMovementState {
    u8 state;
    u8 partnerKnown;
    s16 partnerX;
    s16 partnerZ;
    u16 unk6;
    LocalMapObject *partner;
} FollowPartnerMovementState;

// Movement state of the hidden object movement types: the object sits sunk below a cover model until revealed.
typedef struct HiddenObjectMovementState {
    u8 state;
    u8 coverType;
    u8 revealed;
    u8 unk3;
    int coverEffect;
} HiddenObjectMovementState;

typedef BOOL (*FollowPlayerStateFunc)(LocalMapObject *object, FollowPlayerMovementState *state);
typedef BOOL (*FollowPartnerStateFunc)(LocalMapObject *object, FollowPartnerMovementState *state);
typedef BOOL (*HiddenObjectStateFunc)(LocalMapObject *object, HiddenObjectMovementState *state);

typedef struct MovementList {
    u32 movements[4];
} MovementList;

extern const FollowPartnerStateFunc gFollowPartnerStates[2];
extern const HiddenObjectStateFunc gHiddenObjectStates[2];
extern const VecFx32 gHiddenObjectResumeOffset;
extern const VecFx32 gHiddenObjectOffset;
extern const MovementList gStepMovements;
extern const FollowPlayerStateFunc gFollowPlayerStates[4];
extern const FollowPlayerStateFunc gMirrorPlayerStates[4];
extern const FollowPlayerStateFunc gFollowMonFollowPlayerStates[4];
extern const MovementList gRunMovements;
extern const MovementList gFastRunMovements;

void FollowPlayerMovement_Init(LocalMapObject *object);
void FollowPlayerMovement_UpdateFollowMon(LocalMapObject *object);
void FollowPlayerMovement_Update(LocalMapObject *object);
void FollowPlayerMovement_UpdateMirror(LocalMapObject *object);
void FollowPlayerMovement_Delete(LocalMapObject *object);
u8 FollowPlayerMovement_GetHopFrame(LocalMapObject *object);
u32 FollowPlayerMovement_GetHopSpeed(LocalMapObject *object);
void FollowPlayerMovement_DisableBallEffect(LocalMapObject *object);
void FollowPlayerMovement_Reset(LocalMapObject *object);
BOOL FollowPlayerState_FollowMonStart(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL FollowPlayerState_Start(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL FollowPlayerState_MirrorStart(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL FollowPlayerState_Moving(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL FollowPlayerState_Hop(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL sub_02065C90(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL FollowPlayerMovement_CheckPlayer(LocalMapObject *object, FollowPlayerMovementState *state);
void FollowPlayerMovement_RecordPlayer(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL FollowPlayerMovement_HasPlayerMoved(LocalMapObject *object, FollowPlayerMovementState *state);
void FollowPlayerMovement_UpdatePlayerCoords(LocalMapObject *object, FollowPlayerMovementState *state);
int FollowMon_GetMovement(LocalMapObject *object);
int FollowPlayerMovement_GetPlayerMovement(LocalMapObject *object);
BOOL FollowPlayerMovement_StartFollowMonStep(LocalMapObject *object, FollowPlayerMovementState *state);
BOOL FollowPlayerMovement_StartStep(LocalMapObject *object);
BOOL FollowPlayerMovement_StartMirrorStep(LocalMapObject *object);

void FollowPartnerMovement_Init(LocalMapObject *object);
void FollowPartnerMovement_Update(LocalMapObject *object);
void FollowPartnerMovement_Delete(LocalMapObject *object);
void FollowPartnerMovement_Resume(LocalMapObject *object);
BOOL FollowPartnerState_Start(LocalMapObject *object, FollowPartnerMovementState *state);
BOOL FollowPartnerState_Moving(LocalMapObject *object, FollowPartnerMovementState *state);
LocalMapObject *MapObject_FindTrainerPartner(LocalMapObject *object);
BOOL FollowPartnerMovement_FindPartner(LocalMapObject *object, FollowPartnerMovementState *state);
void FollowPartnerMovement_SetPartner(LocalMapObject *object, FollowPartnerMovementState *state, LocalMapObject *partner);
BOOL FollowPartnerMovement_HasPartnerMoved(LocalMapObject *object, FollowPartnerMovementState *state);
BOOL FollowPartnerMovement_StartStep(LocalMapObject *object, FollowPartnerMovementState *state);

void HiddenObjectMovement_Init(LocalMapObject *object, int coverType);
void HiddenObjectMovement_InitCover0(LocalMapObject *object);
void HiddenObjectMovement_InitCover1(LocalMapObject *object);
void HiddenObjectMovement_InitCover2(LocalMapObject *object);
void HiddenObjectMovement_InitCover3(LocalMapObject *object);
void HiddenObjectMovement_Update(LocalMapObject *object);
void HiddenObjectMovement_Delete(LocalMapObject *object);
void HiddenObjectMovement_Resume(LocalMapObject *object);
BOOL HiddenObjectState_CreateCover(LocalMapObject *object, HiddenObjectMovementState *state);
BOOL HiddenObjectState_Wait(LocalMapObject *object, HiddenObjectMovementState *state);
int HiddenObjectMovement_SetCoverEffect(LocalMapObject *object, int effect);
int HiddenObjectMovement_GetCoverEffect(LocalMapObject *object);
void HiddenObjectMovement_SetRevealed(LocalMapObject *object);

int GetMovementSpeed(u32 movement);
void FollowMon_SpawnGrassEffects(LocalMapObject *object);

// Overlay 1 functions without a (matching) header. The cover model's effect handle is the return value of
// FieldEffectManager_CreateEffect, which HiddenObjectCover_Spawn's tail call leaves in r0.
int HiddenObjectCover_Spawn(LocalMapObject *object, int coverType);
void TallGrass_Spawn(LocalMapObject *object, int mode);
void TallGrass_SpawnAt(LocalMapObject *object, int mode, int x, int z, int a4);
void VeryTallGrass_Spawn(LocalMapObject *object, int mode);
void VeryTallGrass_SpawnAt(LocalMapObject *object, int mode, int x, int z, int a4);

#endif // POKEHEARTGOLD_MAP_OBJECT_FOLLOW_INTERNAL_H
