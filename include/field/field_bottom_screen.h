#ifndef POKEHEARTGOLD_FIELD_BOTTOM_SCREEN_H
#define POKEHEARTGOLD_FIELD_BOTTOM_SCREEN_H

#include "field_system.h"

// The field's bottom screen app, picked by fieldSystem->bottomScreenType (1-6; set from the map load mode).
typedef struct BottomScreenOps {
    void (*init)(FieldSystem *fieldSystem);
    BOOL (*isRunning)(FieldSystem *fieldSystem); // NULL: always running
    void (*end)(FieldSystem *fieldSystem);
    BOOL (*isDone)(FieldSystem *fieldSystem);
} BottomScreenOps;

int FieldSystem_GetBottomScreenIndex(FieldSystem *fieldSystem);
void FieldSystem_InitBottomScreen(FieldSystem *fieldSystem);
BOOL FieldSystem_IsBottomScreenRunningDummy(FieldSystem *fieldSystem);
void FieldSystem_EndBottomScreen(FieldSystem *fieldSystem);
BOOL FieldSystem_IsBottomScreenDone(FieldSystem *fieldSystem);
void BottomScreen_Blank_Init(FieldSystem *fieldSystem);
void BottomScreen_Blank_End(FieldSystem *fieldSystem);
BOOL BottomScreen_Blank_IsDone(FieldSystem *fieldSystem);
void BottomScreen_ViewPhoto_Init(FieldSystem *fieldSystem);
void BottomScreen_ViewPhoto_End(FieldSystem *fieldSystem);
BOOL BottomScreen_ViewPhoto_IsDone(FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_FIELD_BOTTOM_SCREEN_H
