#ifndef TO37_SOL_R11_OVERLAY14_R22_PRIVATE_H
#define TO37_SOL_R11_OVERLAY14_R22_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov14DispatchEntry {
    u32 (*init)(void *);
    void (*exit)(void *);
    u32 next;
} Ov14DispatchEntry;

extern const Ov14DispatchEntry sPCBoxSubApps[];
extern const u8 ov14_021F7D1C[];
extern const u8 ov14_021F7D2C[];
extern const u8 ov14_021F7D3C[];
extern u8 gSystem[];
extern void ov14_021E9434(void *);

extern u32 OverlayManager_Run(void *);
extern void OverlayManager_Delete(void *);
extern void PlaySE(u32);
extern void ov14_021F0BF4(void *);
extern void PCBox_PrintMonActionMessage(void *, u32, u32, u32);
extern void PCBox_SetBoxArrowsVisible(void *, u32);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void PCBox_ShowSlotMonInfo(void *, u32);
extern void PCBox_StartFadeIn(void *, u32);
extern u32 PCBox_WaitForUpdateFunc(void *, void *, u32);
extern void PCBox_ApplyMonMove(void *);
extern void System_GetTouchNewCoords(u32 *, u32 *);
extern u32 System_GetTouchNew(void);
extern u32 GridInputHandler_GetNextInput(void *);
extern u32 PCBox_IsActionMenuOpen(void *);
extern void PCBox_SlideOutActionMenu(void *);
extern void PCBox_SlideInPartyButton(void *);
extern void ov14_021E82A8(void *);
extern void ov14_021E8328(void *);
extern void ov14_021E884C(void *);
extern void PCBox_ClearMonInfo(void *);
extern void PCBox_ClearMonInfoAndSelection(void *);
extern void ov14_021EA180(void *);
extern void PCBox_FreeMoveWork(void *);
extern void GridInputHandler_SetNextInput(void *, u32);
extern void GridInputHandler_SetButtonInputMode(void *, u32);
extern void ov14_021F6688(void *, u32);
extern void PCBox_DrawActionMenu(void *, const void *, u32);
extern void PCBox_SlideInActionMenu(void *);
extern void ov14_021E8740(void *);
extern void ov14_021E8664(void *);
extern void ov14_021E87F4(void *);
extern void ov14_021E83C4(void *);
extern void PCBox_ShowSummaryButton(void *);
extern void PCBox_SlideInBoxTray(void *);
extern void ov14_021E8874(void *);
extern void ov14_021E82FC(void *);
extern void ov14_021E892C(void *);
extern u32 PCBox_GetSlotMonData(void *, u32, u32, u32);
extern void *PCBox_GetSlotBoxMon(void *, u32, u32);
extern void *PCBox_SetSlotMonData(void *, u32, u32, void *);
extern u32 PCBox_UpdateArceusGiratinaForm(void *);
extern u32 PCBox_UpdateBoxTraySlideIn(void *);
extern void ov14_021EA1F0(void *);
extern void ov14_021F0B70(void *);
extern void ov14_021F0BB4(void *);
extern void PCBox_CreateSelectedIconOutline(void *);
extern void PCBox_SetupBoxTraySprites(void *);
extern void PCBox_UpdateCurrentBoxMarker(void *);
extern void PCBox_UpdateTrayCursor(void *);
extern void PCBox_DrawTrayBoxNameAndCount(void *);
extern void PCBox_DrawBagGiveButton(void *, u32);
extern void PCBox_StartCursorSlide(void *, u32, u32);
extern s32 PCBox_FindTouchedBoxSlot(void);
extern s32 PCBox_GetMoveModeMenuInput(void *);
extern u32 ov14_021F1128(void *);
extern u32 PCBox_ScrollToPrevBox(void *, u32);
extern u32 PCBox_ScrollToNextBox(void *, u32);
extern u32 PCBox_FlashPanel(void *, u32, u32);
extern u32 ov14_021F2330(void *, u32, u32);
extern u32 PCBox_StartMoveModeBoxDrag(void *, u32);
extern u32 ov14_021F0530(void *, u32);
extern u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
extern u32 PCBox_SetSelectedIconOutlineVisible(void *, u32);
extern void PCBox_RefreshSlotIcon(void *, u32, u32, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_LoadHeldItemIcon(void *, u32);
extern void PCBox_MoveHeldItemIconToSlot(void *, u32, u32);
extern void PCBox_CreateHeldItemIconOutline(void *);
extern void PCBox_ShowHeldItemIconOutline(void *);
extern u32 Bag_TakeItem(void *, u32, u32, u32);
extern void *GridInputHandler_GetDpadBox(void *, u32);
extern void DpadMenuBox_GetPosition(void *, u8 *, u8 *);
extern void ManagedSprite_SetPositionXY(void *, s32, s32);
extern void PCBox_PrintMenuPrompt(void *, u32, u32);
extern void PCBox_CreateGridInput(void *, u32, u32);
extern s32 _s32_div_f(s32, s32);

u32 PCBox_State_StartSubApp(void *);
u32 PCBox_State_RunSubApp(void *);
void PCBox_State_Start(void *);
u32 PCBox_State_MoveModeInput(void *);
u32 PCBox_State_ApplyMonMove(void *);
void ov14_021EB7E4(void *);
void ov14_021EB8C0(void *);
void ov14_021EBAEC(void *);
void PCBox_State_GiveItemFromBag(void *);

#endif
