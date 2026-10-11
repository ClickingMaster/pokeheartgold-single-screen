#ifndef POKEHEARTGOLD_MAILBOX_APP_H
#define POKEHEARTGOLD_MAILBOX_APP_H

#include "field_system.h"
#include "overlay_manager.h"
#include "save.h"

// The PC mailbox (overlay 103): lists the stored mail, ten per page, and lets the player read, erase or give
// a piece of mail to a party Pokemon.
typedef struct UnkStruct_0203F074 {
    SaveData *saveData;
    FieldSystem *fieldSystem;
    MenuInputStateMgr *menuInputStatePtr;
} UnkStruct_0203F074;

BOOL MailboxApp_Init(OverlayManager *man, int *state);
BOOL MailboxApp_Main(OverlayManager *man, int *state);
BOOL MailboxApp_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_MAILBOX_APP_H
