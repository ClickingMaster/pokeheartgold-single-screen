#ifndef TO40_OPUS_R3_OVERLAY14_RESIDUAL_37_PRIVATE_H
#define TO40_OPUS_R3_OVERLAY14_RESIDUAL_37_PRIVATE_H

#include "global.h"

/* House style, copied verbatim from include/overlay_14_sol_partial_internal.h */
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

/* ---- non-overlay externals (global.h does not reach these headers) ---- */
extern void *Heap_Alloc(u32, u32);
extern void Heap_Free(void *);
extern void *AllocMonZeroed(u32);
extern u32 GetMonData(void *, s32, void *);
extern s32 Party_GetCount(void *);
extern void *Party_GetMonByIndex(void *, s32);
extern u32 ItemIdIsMail(u16);
extern void PlaySE(u32);
extern u32 System_GetTouchNew(void);
extern s32 PCStorage_CountMonsInBox(void *, u32);
extern s32 PCStorage_CountEmptySpotsInBox(void *, u32);
extern s32 PCStorage_CountMonsAndEggsInBox(void *, u32);
extern u32 PCStorage_IsBonusWallpaperUnlocked(void *, u32);
extern s32 GridInputHandler_GetNextInput(void *);
extern void GridInputHandler_SetNextInput(void *, s32);
extern void GridInputHandler_SetNextLastUnk0FInputs(void *, s32, s32, s32);
extern u32 GridInputHandler_IsButtonInputMode(void *);
extern void GridInputHandler_SetButtonInputMode(void *, u32);
extern const void *GridInputHandler_GetDpadBox(void *, s32);
extern void DpadMenuBox_GetPosition(const void *, u8 *, u8 *);
extern void ManagedSprite_SetPositionXY(void *, s32, s32);

/* ---- overlay_14 helpers defined elsewhere ---- */
extern u32 PCBox_GetSlotMonData(void *, u32, u32, u32);
extern u32 PCBox_PartyHasOtherUsableMon(void *, u32);
extern void PCBox_ClearMonInfoAndSelection(void *);
extern void PCBox_LoadScrolledWallpaper(void *, u32, u32);
extern u32 PCBox_GetWallpaperGfxIndex(void *, u32);
extern void PCBox_SlideOutMarkingMenu(void *);
extern void ov14_021E7E78(void *);
extern void ov14_021E7E88(void *);
extern void ov14_021E8020(void *);
extern void ov14_021E805C(void *);
extern void ov14_021E81A8(void *, u32);
extern void PCBox_SlideOutPartyButton(void *);
extern void PCBox_SlideInPartyButton(void *);
extern void ov14_021E8294(void *);
extern void ov14_021E82A8(void *);
extern void ov14_021E82DC(void *);
extern void ov14_021E8314(void *);
extern void ov14_021E8328(void *);
extern void ov14_021E8368(void *);
extern void ov14_021E83F4(void *);
extern void PCBox_SlideInActionMenu(void *);
extern void PCBox_SlideOutActionMenu(void *);
extern u32 PCBox_IsActionMenuOpen(void *);
extern void PCBox_SlideOutChangeBoxButton(void *);
extern u32 PCBox_IsChangeBoxButtonShown(void *);
extern void PCBox_SlideOutSummaryButton(void *);
extern u32 PCBox_IsSummaryButtonShown(void *);
extern void PCBox_SlideInBoxTray(void *);
extern void PCBox_SlideOutBoxTray(void *);
extern void ov14_021E8824(void *);
extern void ov14_021E884C(void *);
extern u32 ov14_021EC710(void *);
extern void ov14_021ED5B0(void *);
extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern u32 PCBox_ShowYesNo(void *, u32);
extern u32 PCBox_StartSubApp(void *, u32);
extern u32 PCBox_FlashPanel(void *, u32, u32);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_LoadBoxIconImages(void *, u32);
extern void ov14_021F3044(void *);
extern void ov14_021F30B0(void *);
extern void ov14_021F311C(void *);
extern void PCBox_SetIconLayer(void *, u32, u32);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void PCBox_SetSlotIconTranslucent(void *, u32, u32);
extern void PCBox_CreateSelectedIconOutline(void *);
extern void PCBox_SetBoxArrowsVisible(void *, u32);
extern void PCBox_UpdateCurrentWallpaperMarker(void *);
extern void PCBox_UpdateWallpaperCursor(void *);
extern void PCBox_LoadWallpaperThumbnails(void *);
extern void PCBox_SetupBoxTraySprites(void *);
extern void PCBox_UpdateCurrentBoxMarker(void *);
extern void PCBox_UpdateTrayCursor(void *);
extern void PCBox_LoadTrayPageThumbnails(void *);
extern void PCBox_DrawTrayBoxNameAndCount(void *);
extern void PCBox_DrawWallpaperName(void *);
extern void PCBox_DrawExitButton(void *, u32);
extern void PCBox_DrawChangeBoxButton(void *);
extern void ov14_021F63A8(void *);
extern void ov14_021F63B8(void *);
extern void ov14_021F63F0(void *);
extern void PCBox_DrawSummaryButton(void *, u32);
extern void PCBox_CloseMessageWindow(void *, u32);
extern void PCBox_PrintTakeItemPrompt(void *, u32, u32);
extern void PCBox_PrintMonActionMessage(void *, u32, u32, u32);
extern void PCBox_PrintPanelHint(void *, u32, u32);
extern void PCBox_CreateGridInput(void *, u32, u32);

/* ---- state callbacks (address-taken only) ---- */
extern void PCBox_UpdateMoveModeBoxDrag(void *);
extern void PCBox_UpdateMoveModePartyDrag(void *);
extern void PCBox_UpdateBoxTrayDrag(void *);
extern void PCBox_UpdateScrollToPrevBox(void *);
extern void PCBox_UpdateScrollToNextBox(void *);
extern void ov14_021E9434(void *);
extern void ov14_021E9450(void *);
extern void PCBox_UpdateMarkingMenuSlide(void *);
extern void ov14_021E94BC(void *);
extern void ov14_021E952C(void *);
extern void ov14_021E954C(void *);
extern void PCBox_UpdateBoxTraySlideIn(void *);
extern void PCBox_UpdateBoxTraySlideOut(void *);
extern void ov14_021E95B4(void *);
extern void ov14_021E9920(void *);
extern void ov14_021E9970(void *);
extern void PCBox_UpdateDepositDrag(void *);
extern void PCBox_UpdateWithdrawDrag(void *);
extern void ov14_021EA068(void *);
extern void ov14_021EA0B8(void *);

#endif
extern int GridInputHandler_HandleInput_AllowHold(void *);
extern void PCBox_SlideOutPartyPanel(void *);
extern void PCBox_ShowSlotMonInfo(void *, u32);
extern u32 ov14_021E834C(void *);
extern u32 ov14_021F0C58(void *);
extern void PCBox_DrawPartyButton(void *, u32);
extern void ov14_021F5E94(void *, u32);
extern void PCBox_DrawReturnButton(void *, u32);
extern u32 ov14_021F1170(void *, s32);
extern u32 ov14_021F11F8(void *, s32);
extern u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
extern void ov14_021E9518(void *);

void ov14_021ED35C(void *);
void ov14_021ED38C(void *);
u32 ov14_021ED414(void *);
void ov14_021ED5B0(void *);
