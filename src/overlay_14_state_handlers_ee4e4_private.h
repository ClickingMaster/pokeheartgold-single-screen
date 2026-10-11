#ifndef OVERLAY_14_STATE_HANDLERS_EE4E4_PRIVATE_H
#define OVERLAY_14_STATE_HANDLERS_EE4E4_PRIVATE_H

#include "global.h"

#include "system.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

u32 PCBox_State_SwitchBox(void *p);
u32 ov14_021EE684(void *p);
u32 ov14_021EE728(void *p);
u32 ov14_021EEC9C(void *p);
u32 ov14_021EEE94(void *p);
u32 ov14_021EEED4(void *p);
u32 ov14_021EEF34(void *p);
u32 ov14_021EF190(void *p);
u32 ov14_021EF1EC(void *p);
u32 ov14_021EF8AC(void *p);
u32 ov14_021EF920(void *p);
u32 ov14_021F00BC(void *p);
u32 ov14_021F0120(void *p);
u32 ov14_021F0164(void *p);
u32 PCBox_StartFadeIn(void *p, u32 v);
u32 PCBox_StartFadeOut(void *p, u32 v);

extern void Heap_Free(void *);
extern void PlaySE(u32);
extern u32 PCStorage_CountMonsAndEggsInBox(void *, u32);
extern void GridInputHandler_SetButtonInputMode(void *, u32);

extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern void PCBox_PrintMonActionMessage(void *, u32, u32, u32);
extern void PCBox_HideBoxTraySprites(void *);
extern void PCBox_CreateGridInput(void *, u32, u32);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_CreateSelectedIconOutline(void *);
extern void PCBox_SetSelectedIconOutlineVisible(void *, u32);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void ov14_021E8588(void *);
extern void ov14_021E8314(void *);
extern void ov14_021E8048(void *);
extern void ov14_021E8328(void *);
extern void ov14_021E88F8(void *);
extern void PCBox_CloseMessageWindow(void *, u32);
extern void PCBox_AllocMoveWork(void *);
extern void PCBox_DrawReturnButton(void *, u32);
extern u32 PCBox_ShowSlotMonInfo(void *, u32);
extern void PCBox_HideSelectedIconOutline(void *);
extern void PCBox_PrintItemActionMessage(void *, u32, u32);
extern void PCBox_PrintMailCantBeTaken(void *, u32);

extern void ov14_021E98AC(void *);
extern void ov14_021E99F0(void *);
extern void ov14_021E952C(void *);
extern void PCBox_AnimateMoveToFirstEmptySlot(void *);
extern void ov14_021E95B4(void *);

extern void BeginNormalPaletteFade(u32, u32, u32, u32, u32, u32, u32);
extern void GridInputHandler_SetNextInput(void *, u32);
extern void *GridInputHandler_GetDpadBox(void *, u32);
extern void DpadMenuBox_GetPosition(const void *, u8 *, u8 *);
extern void ManagedSprite_SetPositionXY(void *, s32, s32);
extern void BgCellManager_DrawCell(void *, u32);
extern void ov14_021F6688(void *, u32);
extern u32 PCBox_IsSpriteAnimated(void *, u32);
extern void ov14_021F391C(void *, u32);
extern void PCBox_SetSlotIconTranslucent(void *, u32, u32);
extern void PCBox_PrintItemHeld(void *, u32, u32);
extern void ov14_021F1F38(void *);
extern void PCBox_ApplyMonMove(void *);
extern void PCBox_RevertShayminFormOnMove(void *, u32, u32);
extern void PCBox_FreeMoveWork(void *);
extern void ov14_021E8094(void *);
extern void PCBox_SlideInPartyButton(void *);
extern void ov14_021E82A8(void *);
extern void ov14_021E954C(void *);
extern void ov14_021E94BC(void *);

extern u32 ItemIdIsMail(u32);
extern void PCBox_PrintMailCantBeMoved(void *, u32);
extern void PCBox_LoadBoxIconImages(void *, u32);
extern u32 PCBox_GetWallpaperGfxIndex(void *, u32);
extern void PCBox_LoadScrolledWallpaper(void *, u32, u32);
extern void PCBox_UpdateScrollToPrevBox(void *);
extern void PCBox_UpdateScrollToNextBox(void *);

extern void ov14_021F6678(void *, u32);
extern u32 PCBox_ShowYesNo(void *, u32);

/* --- round 3 additions (this worker) --- */
extern u32 PCBox_FindTouchedBoxSlot(void); /* zero-arg: called with no r0 setup */
extern u32 ov14_021F6A34(void); /* zero-arg */
extern u32 PCBox_GetItemModeMenuInput(void *);
extern u32 PCBox_GetBoxTrayInput(void *);
extern u32 ov14_021F7A50(void *);
extern u32 PCBox_IsSummaryShortcutPressed(void *);
/* CORRECTION: the shared internal header declares PCBox_StartCursorSlide as 1-arg;
   this object calls it with three (work, slot, input). */
extern void PCBox_StartCursorSlide(void *, u32, u32);
extern u32 PCBox_IsActionMenuOpen(void *);
extern u32 PCBox_IsChangeBoxButtonShown(void *);
extern u32 PCBox_IsSummaryButtonShown(void *);
extern void PCBox_SlideOutActionMenu(void *);
extern void PCBox_SlideInSummaryButton(void *);
extern void PCBox_SlideOutSummaryButton(void *);
extern void ov14_021EA674(void *);
extern u32 ov14_021F18B0(void *, u32);
extern u32 ov14_021F1808(void *, u32);
extern u32 ov14_021F19F0(void *, u32);
extern u32 ov14_021F1B4C(void *, u32);
extern u32 ov14_021F1C4C(void *, u32);
extern u32 ov14_021F1128(void *);
extern void PCBox_ChangeTrayPage(void *, s32);
extern u32 PCBox_ScrollToPrevBox(void *, u32);
extern u32 PCBox_ScrollToNextBox(void *, u32);
extern u32 PCBox_HideInfoPanelAndWait(void *, u32);
extern u32 ov14_021F0D58(void *, u32);
extern u32 ov14_021F0EE8(void *, u32);
extern u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
extern u32 ov14_021F2330(void *, u32, u32);
extern u32 ov14_021F2490(void *, u32, u32);
extern u32 PCBox_FlashPanel(void *, u32, u32);
extern void PCBox_DrawSummaryButton(void *, u32);
extern void PCBox_HideSelectedIconOutline(void *);
extern int PCBox_GetSlotMonData(void *, int, int, int);

extern u32 GridInputHandler_IsButtonInputMode(void *);
extern u32 GridInputHandler_GetNextInput(void *);
extern void PCBox_ClearMonInfo(void *);
extern void PCBox_ClearMonInfoAndSelection(void *);

extern BOOL System_GetTouchNewCoords(u32 *x, u32 *y);

extern u32 PCBox_IsSpriteVisible(void *, u32);
extern void PCBox_DrawBagGiveButton(void *, u32);
extern void BgCellManager_EraseCell(void *, u32);
extern void ov14_021E9450(void *);

extern int PCBox_SetSlotMonData(void *, int, int, void *);
extern void PCBox_GetSlotBoxMon(void *, u32, u32);
extern u32 PCBox_UpdateArceusGiratinaForm(void);
extern void PCBox_RefreshSlotIcon(void *, u32, u32, u32);
extern void PCBox_LoadHeldItemIcon(void *, u32);
extern void PCBox_CreateHeldItemIconOutline(void *);
extern u32 BgCellManager_IsCellMoving(void *, u32);
extern void ov14_021EA728(void *);
extern void ov14_021EA928(void *);

extern void ov14_021F39A0(void *, u32, u32);
extern u32 PCBox_PrintItemNameAndDesc(void *, u32);
extern void BgCellManager_PutWindowTiles(void *, u32, void *);
extern void PCBox_LoadInfoItemIcon(void *, u32);
extern void ov14_021E88BC(void *);
extern void PCBox_SlideInPartyPanel(void *);
extern void ov14_021E9970(void *);
extern void ov14_021EAA04(void *);
extern void ov14_021E9518(void *);
extern void ov14_021EAF08(void *);
extern u32 PCBox_FindTouchedPartySlot(void);
extern u32 PCBox_GetItemModePartyMenuInput(void *);
extern u32 ov14_021F1D6C(void *, u32);
extern u32 ov14_021F1CDC(void *, u32);

#endif
