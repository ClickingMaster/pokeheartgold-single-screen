#ifndef POKEHEARTGOLD_OVERLAY08_EXACT_BANK_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY08_EXACT_BANK_PRIVATE_H

#include "global.h"

#include "battle/battle_dpad_cursor.h"

typedef struct Ov08WorkMode {
    u8 padding[0x2077];
    u8 mode : 4;
    u8 upper : 4;
} Ov08WorkMode;

extern int TouchscreenHitbox_FindRectAtTouchNew(const void *hitboxes);
extern void ClearFrameAndWindow2(void *window, BOOL dontCopy);
extern u8 TextPrinterCheckActive(u8 printerId);
extern void WindowArray_Delete(void *windows, s32 count);

extern void BattlePartyMenu_ChangePage(void *work, int state);
extern void BattleBagMenu_ChangePage(void *work, int state);
extern void BattlePartyMenu_ShowMessage(void *work);
extern void ov08_0221D81C(void *work);
extern void ScheduleBgTilemapBufferTransfer(void *bgConfig, int bgId);
extern void BattleBagMenu_PrintItemList(void *work);
extern void BattleBagMenu_PrintPocketName(void *work);
extern void BattleBagMenu_PrintPageNumber(void *work);
extern u32 BattleSystem_GetBattleType(void *battleSystem);
extern void ManagedSprite_SetDrawFlag(void *sprite, BOOL draw);
extern void ManagedSprite_SetPositionXY(void *sprite, s16 x, s16 y);
extern void ManagedSprite_SetAnim(void *sprite, u32 anim);
extern u32 ManagedSprite_GetActiveAnim(void *sprite);
extern void ManagedSprite_SetAnimationFrame(void *sprite, int frame);
extern void BattlePartyMenu_ShowMoveTypeIcons(void *work);
extern void BattlePartyMenu_ShowMoveContestTypeIcons(void *work);
extern void DestroyMsgData(void *msgData);
extern void MessagePrinter_Delete(void *printer);
extern void MessageFormat_Delete(void *format);
extern void String_Delete(void *string);
extern void BattleCursor_Disable(void *cursor);

int BattlePartyMenu_ShowPartyListPage(void *work);
int BattlePartyMenu_ShowMonMenuPage(void *work);
int BattlePartyMenu_ShowSummaryPage(void *work);
int BattlePartyMenu_ShowCheckMovesPage(void *work);
int BattlePartyMenu_ShowMoveDetailPage(void *work);
int BattlePartyMenu_ShowPendingMessage(u8 *work);
int BattlePartyMenu_CloseMessage(u8 *work);
int BattlePartyMenu_WaitButtonAnim(const u8 *work);
int BattlePartyMenu_WaitMessage(const u8 *work);
int BattlePartyMenu_CheckTouchHitbox(void *unused, const void *hitboxes);
BOOL BattlePartyMenu_IsDoubleBattle(void *work);
BOOL BattlePartyMenu_IsMultiBattle(void *work);
void BattlePartyMenu_ClearContestMoveInfo(u8 *work);
void BattlePartyMenu_RemovePageWindows(u8 *work);
int BattleBagMenu_ShowPocketMenuPage(void *work);
int BattleBagMenu_ShowItemListPage(void *work);
int BattleBagMenu_ShowUseItemPage(void *work);
int BattleBagMenu_CloseMessage(u8 *work);
int BattleBagMenu_WaitMessage(const u8 *work);
int BattleBagMenu_WaitButtonAnim(const u8 *work);
void BattlePartyMenu_ShowSpriteAt(void *sprite, int x, int y);
void BattlePartyMenu_ShowCapsuleIcon(BOOL enabled, void *sprite, int x, int y);
void BattlePartyMenu_ShowMoveIcons(Ov08WorkMode *work);
void BattlePartyMenu_SetSpriteAnim(void *sprite, u32 anim);
void BattleBagMenu_FreeText(u8 *work);
int BattleBagMenu_CheckTouchHitbox(void *unused, const void *hitboxes);
void BattleBagMenu_RemovePageWindows(u8 *work);
void BattleBagMenu_PrintItemListPage(void *work);
void BattleBagMenu_ShowSpriteAt(void *sprite, int x, int y);
void BattleBagMenu_DeactivateCursor(u8 *work);

#endif
