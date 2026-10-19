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

void FollowPartnerMovement_Init(LocalMapObject *object);
void FollowPartnerMovement_Update(LocalMapObject *object);
void FollowPartnerMovement_Delete(LocalMapObject *object);
void FollowPartnerMovement_Resume(LocalMapObject *object);
LocalMapObject *MapObject_FindTrainerPartner(LocalMapObject *object);

void HiddenObjectMovement_InitCover0(LocalMapObject *object);
void HiddenObjectMovement_InitCover1(LocalMapObject *object);
void HiddenObjectMovement_InitCover2(LocalMapObject *object);
void HiddenObjectMovement_InitCover3(LocalMapObject *object);
void HiddenObjectMovement_Update(LocalMapObject *object);
void HiddenObjectMovement_Delete(LocalMapObject *object);
void HiddenObjectMovement_Resume(LocalMapObject *object);
int HiddenObjectMovement_SetCoverEffect(LocalMapObject *object, int effect);
int HiddenObjectMovement_GetCoverEffect(LocalMapObject *object);
void HiddenObjectMovement_SetRevealed(LocalMapObject *object);


int HiddenObjectCover_Spawn(LocalMapObject *object, int coverType);

#endif // POKEHEARTGOLD_MAP_OBJECT_FOLLOW_INTERNAL_H
