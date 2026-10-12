#ifndef POKEHEARTGOLD_FIELD_BOTTOM_SCREEN_APP_H
#define POKEHEARTGOLD_FIELD_BOTTOM_SCREEN_APP_H

#include "field_system.h"

// The field's bottom-screen apps. Bottom screen types 1 and 3 (field_bottom_screen.c) run them in
// FieldSystem.bottomScreenTask; FieldSystem_SwitchBottomScreenApp fades the sub screen out, exits the
// current app and starts the requested one. The getters and setters for the running app are declared in
// overlay_01.h.
enum BottomScreenAppId {
    BOTTOM_SCREEN_APP_TOUCHSCREEN_MENU,
    BOTTOM_SCREEN_APP_TOUCH_SAVE,
    BOTTOM_SCREEN_APP_TOUCH_MART,
    BOTTOM_SCREEN_APP_CHOICE_MENU,
    BOTTOM_SCREEN_APP_DOWSING_MCHN,
    BOTTOM_SCREEN_APP_NUMBER_ENTRY,
    BOTTOM_SCREEN_APP_FRIEND_ROSTER,
    BOTTOM_SCREEN_APP_COMM_CLUB_BUTTONS,
    BOTTOM_SCREEN_APP_UNION_CHAT_LOG,
};

enum BottomScreenAppState {
    BOTTOM_SCREEN_APP_STATE_INIT,
    BOTTOM_SCREEN_APP_STATE_RUNNING,
    BOTTOM_SCREEN_APP_STATE_SWITCH_REQUESTED,
    BOTTOM_SCREEN_APP_STATE_FADE_OUT,
    BOTTOM_SCREEN_APP_STATE_EXIT,
    BOTTOM_SCREEN_APP_STATE_WAIT_EXIT,
    BOTTOM_SCREEN_APP_STATE_START_NEXT,
    BOTTOM_SCREEN_APP_STATE_FADE_IN,
};

void ov01_021F6830(FieldSystem *fieldSystem, int a1, int a2);
void BottomScreen_TouchSave_Init(FieldSystem *fieldSystem);
void BottomScreen_TouchSave_End(FieldSystem *fieldSystem);
BOOL BottomScreen_TouchSave_IsDone(FieldSystem *fieldSystem);
void BottomScreen_Apps_Init(FieldSystem *fieldSystem);
void BottomScreen_Apps_End(FieldSystem *fieldSystem);
BOOL BottomScreen_Apps_IsDone(FieldSystem *fieldSystem);
void FieldSystem_RequestTouchscreenMenuSelectedIconAnim(FieldSystem *fieldSystem);
void FieldSystem_RequestTouchscreenMenuRegisteredItemAnim(FieldSystem *fieldSystem, int item);

#endif // POKEHEARTGOLD_FIELD_BOTTOM_SCREEN_APP_H
