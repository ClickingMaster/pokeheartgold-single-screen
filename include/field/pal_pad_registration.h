#ifndef POKEHEARTGOLD_FIELD_PAL_PAD_REGISTRATION_H
#define POKEHEARTGOLD_FIELD_PAL_PAD_REGISTRATION_H

#include "bg_window.h"
#include "field_system.h"
#include "message_format.h"
#include "msgdata.h"

// After a wireless session (ScrCmd_151): offers to register each link partner who asked for it in the Pal Pad's
// Friend Roster, and when the roster is full lets the player pick an entry to delete (bottom-screen Friend Roster
// app). The touch YES/NO helpers live in bottom_screen_app.c.
typedef struct PalPadRegistration {
    u8 unk_00[0x8];
    String *rawMessage;
    String *message;
    Window window;
    u8 unk_20[0x10];
    FieldSystem *fieldSystem;
    SaveData *saveData;
    MessageFormat *msgFormat;
    MsgData *msgData;
    u32 printerId;
    u32 state;
    u32 requests[16];  // per link partner, 2: wants to be registered
    u32 partnerNetId;  // -1: none left
    u32 deleteSlot;    // Friend Roster entry the player chose to delete
    u32 yesNoAnswer;   // written by the touch YES/NO menu; 0: yes
    s32 rosterChoice;  // written by the Friend Roster app; -1: none yet, -2: cancelled
    u16 yesNoState;
    u16 rosterAppState;
} PalPadRegistration;

BOOL PalPadRegistration_IsFriendRosterAppRunning(PalPadRegistration *registration);
BOOL PalPadRegistration_TryShowTouchYesNo(PalPadRegistration *registration);
BOOL PalPadRegistration_IsTouchYesNoDone(PalPadRegistration *registration);
BOOL PalPadRegistration_RunTouchYesNo(PalPadRegistration *registration);

#endif // POKEHEARTGOLD_FIELD_PAL_PAD_REGISTRATION_H
