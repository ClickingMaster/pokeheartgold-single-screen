#ifndef OV15_PREFIX_PRIVATE_H
#define OV15_PREFIX_PRIVATE_H

#include "global.h"

void *Save_Bag_Get(void *);
void *Save_PlayerData_GetProfile(void *);
void *Save_PlayerData_GetOptionsAddr(void *);
void *Save_Roamers_Get(void *);
u8 *RoamerSave_GetRepelAddr(void *);
void RoamerSave_SetFlute(void *, u8);
void ReadMsgDataIntoString(void *, u32, void *);
u32 TMHMGetMove(u32);
void *String_New(u32, u32);
void String_Delete(void *);
s32 TouchscreenHitbox_FindRectAtTouchNew(const void *);
void BagApp_StartMoveItem(void *);
s32 ov15_021FA650(void *);
void MenuInputStateMgr_SetState(void *, u32);
void ClearWindowTilemapAndScheduleTransfer(void *);
void BagApp_CloseItemMenuWindows(void *);
void sub_020880CC(u32, u32);
void BagApp_FreeBagModel(void *);
void Camera_Delete(void *);
void GF3dRender_DeleteSimpleManager(void);

extern const void *sTouchHitboxTables[];

void BagApp_GetSaveStructPtrs(void *);
void *BagApp_GetSaveRoamers(void *);
void BagApp_SetRepelStepCount(void *, u8);
void BagApp_SetFlute(void *, u8);
void BagApp_ReadItemName(void *, void *, u32);
void BagApp_ReadTMHMMoveName(void *, void *, u32);
void BagApp_AllocItemNameStrings(void *);
void BagApp_FreeItemNameStrings(void *);
void ov15_021FA070(void);
void ov15_021FA0D8(void *);
s32 BagApp_GetTouchedButton(void *, u32);
s32 ov15_021FAC40(void);
s32 BagApp_State_RunUseItemTask(void *);
s32 ov15_021FBC6C(void *);
s32 BagApp_ItemAction_Confirm(void *);
s32 BagApp_StepQuantity(s32, s32, s32);
s32 BagApp_ItemAction_Move(void *);
s32 ov15_021FD3AC(void *);
BOOL BagApp_IsBerryOrMulch(u32, u32);
void BagApp_SetMenuInputState(void *, u32);
void BagApp_InitCameraTween(void *);
void BagApp_Free3D(void *);
void BagApp_AdvanceAnimLoop(void *);
void ov15_021FE504(void *);
void BagApp_ClearMoveInfoWindow(void *);
void ov15_021FE8A4(void *);

typedef struct Ov15Banks {
    u32 words[10];
} Ov15Banks;
extern const Ov15Banks sBagGraphicsBanks;
void GfGfx_SetBanks(const Ov15Banks *);
void DoScheduledBgGpuUpdates(void *);
void GF_RunVramTransferTasks(void);
void SpriteSystem_TransferOam(void);
BOOL TextPrinterCheckActive(u8);
void BagApp_CreateYesNoPrompt(void *);
void ScheduleWindowCopyToVram(void *);
void BagApp_PrintTrashConfirmMessage(void *);
void BagApp_HideQuantityButtons(void *);
void BagApp_HideCursor(void *);
void BagApp_ClearQuantityLabel(void *);
void BagApp_ShowCancelButton(void *, u32);
void RemoveWindow(void *);
void BagApp_RemoveItemActionWindows(void *);
void BagApp_RemoveItemListWindows(void *);
void *NewString_ReadMsgData(void *, u32);
void *BufferItemName(void *, u32, u16);
void *BufferItemNamePlural(void *, u32, u16);
BOOL SoundSys_GetGBSoundsState(void);
void SoundSys_ToggleGBSounds(void);

void BagApp_SetGraphicsBanks(void);
void BagApp_VBlankCallback(void *);
u16 BagApp_GetCurrentPocketSlotValue(void *, u16, BOOL);
void BagApp_SetPocketPositionFromButton(void *, s32);
s32 BagApp_GetPageItemCount(void *);
BOOL BagApp_ShouldMoveCursorToButton(void *, s32);
s32 BagApp_State_WaitTrashConfirmMessage(void *);
s32 BagApp_State_WaitTrashResultMessage(void *);
s32 BagApp_State_WaitSellPriceMessage(void *);
s32 BagApp_State_ShowTrashConfirm(void *);
void BagApp_RemoveWindows(void *);
void BagApp_BufferSlotItemName(void *, u32, u32);
void BagApp_BufferSlotItemNamePlural(void *, u32, u32);
void BagApp_ToggleGBSounds(void *);
s32 BagApp_ItemAction_Give(void *);
s32 BagApp_ItemAction_ChooseForBerryPatch(void *);
void ov15_021FE874(void *);

s32 BagApp_FindPocketIndex(void *, u8);
s32 BagApp_GetAdjacentPocket(void *, s32, s32);
void BagApp_RemoveItemListWindows(void *);

#endif
