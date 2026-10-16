#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_LOCK_CAPSULE_CARD_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_LOCK_CAPSULE_CARD_H

#include "application/main_menu/wonder_card_album.h"

#include "bg_window.h"
#include "mystery_gift.h"
#include "overlay_manager.h"
#include "save.h"
#include "screen_fade.h"
#include "sys_task.h"

typedef struct LockCapsuleCardView LockCapsuleCardView;

BOOL LockCapsuleCard_PrintMessageOnly(void);
BOOL LockCapsuleCard_PrintCardTitle(LockCapsuleCardView *view, Window *window, u32 color);
BOOL LockCapsuleCard_PrintStatus(LockCapsuleCardView *view, Window *window, u32 color);
BOOL LockCapsuleCard_BufferReceiveDate(LockCapsuleCardView *view);
BOOL LockCapsuleCard_PrintCardText(LockCapsuleCardView *view, Window *window, u32 color);
BOOL LockCapsuleCard_PrintNothing(void);
BOOL LockCapsuleCard_ShowCardMenu(LockCapsuleCardView *view, Window *window, u32 color);
BOOL LockCapsuleCard_Init(OverlayManager *manager, int *state);
BOOL LockCapsuleCard_Main(OverlayManager *manager, int *state);
BOOL LockCapsuleCard_Exit(OverlayManager *manager, int *state);
void LockCapsuleCard_DrawCardOnBg(BgConfig *bgConfig, WonderCard *card, enum HeapID heapID);
void WonderCard_SetGraphicsBanks(void);
void WonderCard_InitBgLayers(BgConfig *bgConfig);
int MysteryGift_CheckCanReceive(SaveData *saveData, WonderCardInfo *header);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_LOCK_CAPSULE_CARD_H
