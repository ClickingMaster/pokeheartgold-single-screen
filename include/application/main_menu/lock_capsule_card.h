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

void LockCapsuleCard_DeleteSprites(LockCapsuleCardView *view);
void LockCapsuleCard_EraseWindowIfInUse(Window *window, BOOL dontCopyToVram);
void LockCapsuleCard_ClearWindowIfInUse(Window *window, BOOL dontCopyToVram);
void LockCapsuleCard_CreateListMenu(LockCapsuleCardView *view, const MysteryGiftMenuItem *items, int count, Window *window, int itemsAbove);
void LockCapsuleCard_DestroyListMenu(LockCapsuleCardView *view);
BOOL LockCapsuleCard_PrintMessageOnly(void);
BOOL LockCapsuleCard_PrintCardTitle(LockCapsuleCardView *view, Window *window, u32 color);
BOOL LockCapsuleCard_PrintStatus(LockCapsuleCardView *view, Window *window, u32 color);
BOOL LockCapsuleCard_BufferReceiveDate(LockCapsuleCardView *view);
BOOL LockCapsuleCard_PrintCardText(LockCapsuleCardView *view, Window *window, u32 color);
BOOL LockCapsuleCard_PrintNothing(void);
BOOL LockCapsuleCard_ShowCardMenu(LockCapsuleCardView *view, Window *window, u32 color);
void LockCapsuleCard_StartFade(LockCapsuleCardView *view, enum FadeType fadeType, int nextState, int *state);
void LockCapsuleCard_DrawCard(LockCapsuleCardView *view, int unused, int page);
int LockCapsuleCard_PrintTextBox(LockCapsuleCardView *view, Window *window, int boxId, int baseTile);
void LockCapsuleCard_LoadScreenData(LockCapsuleCardView *view, int fileId, int bgId, int offset);
void LockCapsuleCard_LoadCardBg(LockCapsuleCardView *view, int page);
void LockCapsuleCard_HandleListMenuInput(OverlayManager *manager, int *state, MysteryGiftMenuFunc cancelFunc);
void LockCapsuleCard_CreateMonIconSprites(LockCapsuleCardView *view);
BOOL LockCapsuleCard_Init(OverlayManager *manager, int *state);
void LockCapsuleCard_FlipEffectTask(SysTask *task, WonderCardFlipEffect *effect);
void LockCapsuleCard_StartFlipEffect(LockCapsuleCardView *view, int mode, fx32 speed, fx32 accel);
void LockCapsuleCard_StopFlipEffect(LockCapsuleCardView *view);
BOOL LockCapsuleCard_UpdateFlipEffect(LockCapsuleCardView *view);
BOOL LockCapsuleCard_Main(OverlayManager *manager, int *state);
BOOL LockCapsuleCard_Exit(OverlayManager *manager, int *state);
void LockCapsuleCard_DrawCardOnBg(BgConfig *bgConfig, WonderCard *card, enum HeapID heapID);
void WonderCard_SetGraphicsBanks(void);
void WonderCard_InitBgLayers(BgConfig *bgConfig);
int MysteryGift_CheckCanReceive(SaveData *saveData, UnkWonderCardSubstruct_104 *header);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_LOCK_CAPSULE_CARD_H
