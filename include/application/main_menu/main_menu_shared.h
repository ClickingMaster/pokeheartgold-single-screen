#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_SHARED_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_SHARED_H

#include "global.h"

#include "bg_window.h"
#include "list_menu.h"
#include "mystery_gift.h"
#include "save.h"

// Shared services of the main menu overlay, part 2 (src/application/main_menu/main_menu_shared.c; part 1 is
// main_menu_util.c, they share the MainMenuUtilWork): the sub screen of the card screens, the encryption of a shared
// Wonder Card, the list menu cursor sound, the GBA cartridge interrupt and an asynchronous save.

// States of the asynchronous save (MainMenuUtil_GetSaveState)
#define SAVE_STATE_START   0
#define SAVE_STATE_WRITING 1
#define SAVE_STATE_DONE    2
#define SAVE_STATE_FAILED  3 // or cancelled
#define SAVE_STATE_PAUSED  4 // after WRITE_STATUS_NEXT, until MainMenuUtil_ResumeSave

void MainMenuUtil_ShowCardSubScreen(BgConfig *bgConfig, WonderCard *card);
void MainMenuUtil_EncryptWonderCard(WonderCardInfo *header, void *dest, enum HeapID heapID);
void MainMenuUtil_DecryptWonderCard(WonderCardInfo *header, WonderCard *dest, enum HeapID heapID);
void MainMenuUtil_ListMenuCursorCallback(ListMenu *list, s32 index, u8 onInit);
void MainMenuUtil_CartridgeIrqHandler(void);
void MainMenuUtil_EnableCartridgePullOutCheck(BOOL enable);
void MainMenuUtil_IgnoreCartridgeIrqHandler(void);
void MainMenuUtil_IgnoreCartridgeIrq(void);
void MainMenuUtil_StartSave(SaveData *saveData);
int MainMenuUtil_RunSave(void);
void MainMenuUtil_ResumeSave(void);
void MainMenuUtil_CancelSave(void);
int MainMenuUtil_GetSaveState(void);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_SHARED_H
