#ifndef TO42_OVERLAY15_BAG_RESIDUAL8_PRIVATE_H
#define TO42_OVERLAY15_BAG_RESIDUAL8_PRIVATE_H

#include "global.h"

#include "system.h"

#define PTR(p, o)    (*(void **)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))

extern const u8 sMoveNavTable[];
extern const u8 ov15_02200585[];
extern const u8 ov15_02200586[];
extern const u8 ov15_02200587[];
extern const u8 sItemMenuNavTable[];
extern const u8 ov15_02200529[];
extern const u8 ov15_0220052A[];
extern const u8 ov15_0220052B[];
extern const u8 sItemMenuCursorButtons[];

int BagApp_GetPageItemCount(void *);
int BagApp_GetMovingItemPageSlot(void *);
void BagApp_SetPocketPositionFromButton(void *, int);
int BagApp_GetTouchedButton(void *, int);
void BagApp_LoadPocketItemNames(void *);
void BagApp_LoadInfoPanelTilemap(void *, int);
void BagApp_LoadSubScreenLayout(void *, int, int, int);
void BagApp_DrawItemList(void *, int, int, int);
void BagApp_PrintPageNumber(void *, int, int, int);
void BagApp_DrawItemIcons(void *, void *, int, int);
void BagApp_ShowMovingItemIcon(void *, void *, int);
void BagApp_LoadPocketObjPalette(void *, int);
void BagApp_LoadPocketSubBgPalette(void *, int, int);
void BagApp_SetCursorPosition(void *, int);
void BagApp_HideItemMenuSprites(void *);
void BagApp_ClearSelectedItemEntry(void *);
void BagApp_ClearItemMenuLabels(void *);
void BagApp_ClearMoveInfoWindow(void *);
void BagApp_CloseItemMenuWindows(void *);
void BagApp_CloseItemPromptWindow(void *);
void BagApp_ShowCancelButton(void *, int);
void BagApp_HideCursor(void *);
int BagApp_PrintMessage(void *, int);
void BagApp_RedrawPocket(void *);
int BagApp_HandleItemMenuInput(void *);
int BagApp_UseSelectedItem(void *);
int BagApp_TryUseItemInPlace(void *, int);
void BagApp_UseTMHMTask(void);
void BagApp_UseItemInPlaceTask(void);
void BagApp_State_EndMoveItem(void *);
void BagApp_ShowItemMenuSprites(void *, const u8 *);
void BagApp_PrintItemMenuLabel(void *, void *, u8);
void BagApp_StartButtonFlash(void *, int, int, int, int);

int BagApp_GetMoveCursorFromDpad(int);
int BagApp_GetMoveTargetResult(void *, int);
void BagApp_State_DropMovedItem(void *);
void BagApp_UpdateMoveMarker(void *);
void BagApp_RedrawMovePage(void *, void *);
void BagApp_RedrawPocket(void *);
void BagApp_DrawItemMenu(void *, const u8 *);
int BagApp_State_ItemMenuInput(void *);
int BagApp_State_CloseItemMenu(void *);
void BagApp_State_RunItemAction(void *);
int BagApp_ItemAction_Use(void *);
int BagApp_State_WaitItemMessage(void *);

void PlaySE(u16);
void MoveItemSlotInList(void *, int, int, int, int);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ClearFrameAndWindow2(void *, int);
void ClearWindowTilemapAndScheduleTransfer(void *);
void ScheduleWindowCopyToVram(void *);
BOOL TextPrinterCheckActive(u8);
void sub_020880CC(int, int);
u32 GetItemAttr(u16, int, int);
void *GetItemFieldUseFunc(int, u16);
void GetItemUseErrorMessage(void *, void *, u16, int, int);
BOOL TryFormatRegisteredKeyItemUseMessage(void *, void *, u16, int);

#endif
