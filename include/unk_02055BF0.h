#ifndef POKEHEARTGOLD_UNK_02055BF0_H
#define POKEHEARTGOLD_UNK_02055BF0_H

#include "field_system.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_0206793C.h"

typedef struct FieldFadeEnvironment {
    u32 state;
    enum FadeMode fadeMode;
    enum FadeType typeTop;
    enum FadeType typeBottom;
    u16 colour;
    u16 unk12;
    u32 duration;
    int framesPer;
    enum HeapID heapID;
} FieldFadeEnvironment;

typedef struct FieldTransitionEnvironment {
    u8 state;
    u8 delayTimer;
    u16 transitionState;
    Location location;
    FieldEnvSubUnk18 *transitionWork;
    u32 transitionNo;
    u32 destinationMapID;
    FieldEffect *followMonBall;
} FieldTransitionEnvironment; // size: 0x28

void NewFieldFadeEnvironment(TaskManager *man, enum FadeMode fadeMode, enum FadeType typeTop, enum FadeType typeBottom, u16 colour, int duration, int framesPer, enum HeapID heapID);
void NewFieldTransitionEnvironment(FieldSystem *fieldSystem, int mapID, int warpID, int x, int z, int dir, u32 transNo);
void NewFieldTransitionEnvironmentByMapType(FieldSystem *fieldSystem, int mapID, int warpID, int x, int z, int dir);

BOOL FieldTransition_ExitWithFade(TaskManager *man);
BOOL FieldTransition_ExitThroughDoor(TaskManager *man);
BOOL FieldTransition_ExitThroughSideDoor(TaskManager *man);
BOOL FieldTransition_ExitOnWarpStairs(TaskManager *man);
BOOL FieldTransition_ExitStairsFadeOut(TaskManager *man);
BOOL FieldTransition_ExitStairsFadeToWhite(TaskManager *man);
BOOL FieldTransition_ExitRising(TaskManager *man);
BOOL FieldTransition_ExitSinking(TaskManager *man);
BOOL FieldTransition_EnterWithDirectionalFade(TaskManager *man);
BOOL FieldTransition_EnterWithFade(TaskManager *man);

#endif // POKEHEARTGOLD_UNK_02055BF0_H
