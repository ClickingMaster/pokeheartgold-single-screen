#ifndef OVERLAY14_BOX_UI_ACTIONS_INTERNAL_H
#define OVERLAY14_BOX_UI_ACTIONS_INTERNAL_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

extern void BgCellManager_DrawCellAt(void *, int, int, int);
extern void BgCellManager_EraseCell(void *, int);
extern void BgCellManager_StartCellMove(void *, int, int, int, u8);
extern u32 BgCellManager_UpdateCellMove(void *, int);
extern u32 BgCellManager_IsCellMoving(void *, int);
extern void BgCellManager_GetCellPosition(void *, int, s8 *, s8 *);
extern void FillBgTilemapRect(void *, u32, u32, u32, u32, u32, u32, u32);
extern void ScheduleBgTilemapBufferTransfer(void *, u8);
extern void ScheduleSetBgPosText(void *, int, int, int);
extern void *Heap_Alloc(u32, u32);
extern void Heap_Free(void *);
extern int Party_GetCount(void *);
extern u32 System_GetTouchHeldCoords(u32 *, u32 *);
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern u32 GridInputHandler_IsButtonInputMode(void *);
extern void GridInputHandler_SetButtonInputMode(void *, u32);
extern void *OverlayManager_New(const void *, void *, u32);
extern void HandleLoadOverlay(u32, s32);
extern void UnloadOverlayByID(u32);
extern void PCStorage_SetBoxModified(void *, u32);
extern u32 PCStorage_FindFirstEmptySlot(void *, int *, int *);
extern void PCStorage_GetBoxName(void *, u32, void *);
extern void PCStorage_SetBoxName(void *, u32, void *);
extern u16 BagView_GetItemId(void *);
extern void *Bag_CreateView(void *, const u8 *, u32);
extern void *Save_Bag_Get(void *);
extern void *NamingScreen_CreateArgs(u32, u32, u32, u32, void *, void *);
extern void NamingScreen_DeleteArgs(void *);
extern void *Save_PlayerData_GetProfile(void *);
extern u32 SaveArray_IsNatDexEnabled(void *);
extern void *Save_SpecialRibbons_Get(void *);
extern void sub_0208AD34(void *, void *);
extern void sub_02089D40(void *, const void *);
extern void *sub_02088288(void *);
extern void *sub_0208828C(void *);
extern void sub_0207789C(void *, void *, u32, u32, void *);

extern const u8 ov14_021F7BF0[];
extern const u8 ov14_021F7C08[];
extern const u8 ov14_021F7D0C[];
extern const u8 ov14_021F7D14[];
extern const u8 gOverlayTemplate_PokemonSummary[];
extern const u8 gOverlayTemplate_NamingScreen[];
extern const u8 gOverlayTemplate_Bag[];

extern void PCBox_AnimatePaletteFlash(void *);
extern void *PCBox_GetSlotBoxMon(void *, u32, u32);
extern u32 PCBox_GetSlotMonData(void *, u32, u32, u32);
extern u32 PCBox_AnimateMonMove(void *);
extern u32 ov14_021E66F4(void *);
extern u32 ov14_021E6814(void *);
extern void PCBox_SetupMonMove(void *, u32, u32);
extern u32 PCBox_GetMoveEntryTarget(void *, u32);
extern void ov14_021E70E0(void *, u32);
extern void PCBox_ApplyMoveEntryIcons(void *, void *);
extern void PCBox_ShowSlotMonInfo(void *, u32);
extern u32 PCBox_GetTouchedBoxSlot(s16, s16);
extern u32 PCBox_GetTouchedPartySlot(s16, s16, const void *);
extern u32 PCBox_GetTouchedTrayBox(s16, s16);
extern void PCBox_SlideInPartyPanel(void *);
extern void ov14_021E7FB8(void *);
extern void ov14_021E7FEC(void *);
extern u32 PCBox_UpdatePartyPanelSlide(void *);
extern void PCBox_HidePartyButton(void *);
extern void ov14_021E827C(void *);
extern void PCBox_ScrollBoxIcons(void *, s32);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void PCBox_SetSlotIconTranslucent(void *, u32, u32);
extern void PCBox_UpdateSelectedIconOutline(void *);
extern void PCBox_SetSelectedIconOutlineVisible(void *, u32);
extern void PCBox_SetupBoxTraySprites(void *);
extern void PCBox_MoveBoxTraySprites(void *, u32);
extern void PCBox_UpdateCurrentBoxMarker(void *);
extern void PCBox_UpdateTrayCursor(void *);
extern void PCBox_DrawTrayBoxNameAndCount(void *);
extern void PCBox_DrawReturnButton(void *, u32);
extern void PCBox_DrawExitButton(void *, u32);
extern void PCBox_DrawBoxMenu(void *);
extern void ov14_021F69F0(void *, u32);

FS_EXTERN_OVERLAY(OVY_15);

void ov14_021E8294(void *p);
void ov14_021E82A8(void *p);
void ov14_021E82BC(void *p);
void ov14_021E82DC(void *p);
void ov14_021E82FC(void *p);
void ov14_021E8314(void *p);
void ov14_021E8328(void *p);
u32 ov14_021E834C(void *p);
void ov14_021E8368(void *p);
void ov14_021E8394(void *p);
void ov14_021E83C4(void *p);
void ov14_021E83F4(void *p);
void PCBox_SlideInActionMenu(void *p);
void PCBox_SlideOutActionMenu(void *p);
u32 PCBox_UpdateActionMenuSlide(void *p);
u32 PCBox_IsActionMenuOpen(void *p);
void ov14_021E8560(void *p);
void ov14_021E8588(void *p);
void ov14_021E85AC(void *p);
void PCBox_SlideInChangeBoxButton(void *p);
void PCBox_SlideOutChangeBoxButton(void *p);
u32 PCBox_IsChangeBoxButtonShown(void *p);
void PCBox_HideSummaryButton(void *p);
void PCBox_ShowSummaryButton(void *p);
void PCBox_SlideInSummaryButton(void *p);
void PCBox_SlideOutSummaryButton(void *p);
u32 PCBox_IsSummaryButtonShown(void *p);
void ov14_021E8664(void *p);
void PCBox_SlideInBoxTray(void *p);
void PCBox_SlideOutBoxTray(void *p);
void ov14_021E8718(void *p);
void ov14_021E8740(void *p);
void PCBox_SlideInBoxMenu(void *p);
void PCBox_SlideOutBoxMenu(void *p);
void ov14_021E87F4(void *p);
void ov14_021E8824(void *p);
void ov14_021E884C(void *p);
void ov14_021E8874(void *p);
void ov14_021E88A4(void *p);
void ov14_021E88BC(void *p);
void ov14_021E88F8(void *p);
void ov14_021E892C(void *p);
void ov14_021E8944(void *p);
void PCBox_DrawMarkings(void *p, u32 mask);
u32 PCBox_StartSummaryApp(void *p);
u32 PCBox_EndSummaryApp(void *p);
u32 PCBox_StartBagApp(void *p);
u32 PCBox_EndBagApp(void *p);
u32 PCBox_StartNamingScreenApp(void *p);
u32 PCBox_EndNamingScreenApp(void *p);
u32 PCBox_UpdateMoveModeBoxDrag(void *p);
u32 PCBox_UpdateMoveModePartyDrag(void *p);
u32 PCBox_UpdateBoxTrayDrag(void *p);
u32 ov14_021E9194(void *p);
u32 PCBox_AnimateMoveToPartyEnd(void *p);
u32 PCBox_AnimateMoveToFirstEmptySlot(void *p);
u32 PCBox_UpdateScrollToPrevBox(void *p);
u32 PCBox_UpdateScrollToNextBox(void *p);
u32 ov14_021E9434(void *p);
u32 ov14_021E9450(void *p);

#endif
