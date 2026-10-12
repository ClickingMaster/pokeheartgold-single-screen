#include "field/field_bottom_screen.h"

#include "global.h"

#include "application/view_photo.h"
#include "field/bottom_screen_app.h"

#include "assert.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_19);

// defined in field_interaction.c (empty), not declared by a header yet
void sub_0203DB6C(FieldSystem *fieldSystem);
void sub_0203DB70(FieldSystem *fieldSystem);
BOOL sub_0203DB74(FieldSystem *fieldSystem);

static const BottomScreenOps sBottomScreenOps[];

int FieldSystem_GetBottomScreenIndex(FieldSystem *fieldSystem) {
    int type = fieldSystem->bottomScreenType;
    GF_ASSERT(type != 0);
    GF_ASSERT(type < 7);
    return type - 1;
}

void FieldSystem_InitBottomScreen(FieldSystem *fieldSystem) {
    sBottomScreenOps[FieldSystem_GetBottomScreenIndex(fieldSystem)].init(fieldSystem);
}

BOOL FieldSystem_IsBottomScreenRunningDummy(FieldSystem *fieldSystem) {
    BOOL (*isRunning)(FieldSystem *) = sBottomScreenOps[FieldSystem_GetBottomScreenIndex(fieldSystem)].isRunning;
    if (isRunning == NULL) {
        return TRUE;
    }
    return isRunning(fieldSystem);
}

void FieldSystem_EndBottomScreen(FieldSystem *fieldSystem) {
    sBottomScreenOps[FieldSystem_GetBottomScreenIndex(fieldSystem)].end(fieldSystem);
}

BOOL FieldSystem_IsBottomScreenDone(FieldSystem *fieldSystem) {
    return sBottomScreenOps[FieldSystem_GetBottomScreenIndex(fieldSystem)].isDone(fieldSystem);
}

void BottomScreen_Blank_Init(FieldSystem *fieldSystem) {
}

void BottomScreen_Blank_End(FieldSystem *fieldSystem) {
}

BOOL BottomScreen_Blank_IsDone(FieldSystem *fieldSystem) {
    return TRUE;
}

void BottomScreen_ViewPhoto_Init(FieldSystem *fieldSystem) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_19), OVY_LOAD_ASYNC);
    fieldSystem->bottomScreenTask = FieldSystem_CreateViewPhotoTask(fieldSystem); // the bottom screen app task
}

void BottomScreen_ViewPhoto_End(FieldSystem *fieldSystem) {
    FieldSystem_DestroyViewPhotoTask(fieldSystem);
    fieldSystem->bottomScreenTask = NULL;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_19));
}

BOOL BottomScreen_ViewPhoto_IsDone(FieldSystem *fieldSystem) {
    return TRUE;
}

static const BottomScreenOps sBottomScreenOps[] = {
    { BottomScreen_Apps_Init,           NULL, BottomScreen_Apps_End,          BottomScreen_Apps_IsDone                 },
    { BottomScreen_Blank_Init, NULL, BottomScreen_Blank_End, BottomScreen_Blank_IsDone     },
    { BottomScreen_Apps_Init,           NULL, BottomScreen_Apps_End,          BottomScreen_Apps_IsDone                 },
    { BottomScreen_TouchSave_Init,           NULL, BottomScreen_TouchSave_End,          BottomScreen_TouchSave_IsDone                 },
    { sub_0203DB6C,            NULL, sub_0203DB70,           sub_0203DB74                  },
    { BottomScreen_ViewPhoto_Init, NULL, BottomScreen_ViewPhoto_End, BottomScreen_ViewPhoto_IsDone },
};
