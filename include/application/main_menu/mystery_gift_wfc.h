#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WFC_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WFC_H

#include "global.h"

#include "application/main_menu/mystery_gift_app.h"

#include "overlay_manager.h"

// Download of a Mystery Gift over Nintendo Wi-Fi Connection (src/application/main_menu/mystery_gift_wfc.c).

// results of MysteryGiftWfc_Run
#define MYSTERY_GIFT_WFC_RESULT_RUNNING   0
#define MYSTERY_GIFT_WFC_RESULT_RECEIVED  1
#define MYSTERY_GIFT_WFC_RESULT_NO_GIFT   2
#define MYSTERY_GIFT_WFC_RESULT_CANCELLED 3
#define MYSTERY_GIFT_WFC_RESULT_ERROR     4 // shown and acknowledged

void *MysteryGiftWfc_Alloc(void *unused, u32 size);
void MysteryGiftWfc_Free(void *unused, void *ptr);
int MysteryGiftWfc_HandleError(MysteryGiftAppData *data);
BOOL MysteryGiftWfc_CheckInetStatus4(MysteryGiftAppData *data);
void MysteryGiftWfc_WaitForRequest(MysteryGiftAppData *data, int *state, int nextState);
void MysteryGiftWfc_StartCleanup(MysteryGiftAppData *data, int result, int *state, int nextState, int failState);
void MysteryGiftWfc_OnRequestDone(int unused, int error);
void MysteryGiftWfc_OnCleanupDone(void);
void MysteryGiftWfc_Shutdown(void);
int MysteryGiftWfc_Run(OverlayManager *manager, int *state);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WFC_H
