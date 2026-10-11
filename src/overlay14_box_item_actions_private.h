#ifndef OVERLAY14_BOX_ITEM_ACTIONS_PRIVATE_H
#define OVERLAY14_BOX_ITEM_ACTIONS_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct {
    u8 lo : 1;
    u8 hi : 7;
} Bits17;

typedef struct {
    u8 lo : 4;
    u8 hi : 4;
} Bits44;

#define BF17(p, o) (*(Bits17 *)((u8 *)PTR_AT((p), 0x34) + (o)))
#define BF44(p, o) (*(Bits44 *)((u8 *)PTR_AT((p), 0x34) + (o)))
#define WQ8(p, o)  (*(u8 *)((u8 *)PTR_AT((p), 0x34) + (o)))
#define CELL(p)    PTR_AT(PTR_AT((p), 0x34), 0x2f0)

extern void PlaySE(u16);
extern void Heap_Free(void *);
extern void *Save_Bag_Get(void *);
extern u32 Bag_AddItem(void *, u16, u16, u32);
extern u32 GridInputHandler_GetNextInput(void *);
extern u32 GridInputHandler_IsButtonInputMode(void *);
extern void GridInputHandler_SetButtonInputMode(void *, u32);
extern void BgCellManager_GetCellPosition(void *, u32, s8 *, s8 *);
extern void BgCellManager_GetCellSize(void *, u32, u16 *, u16 *);
extern u8 BgCellManager_GetCellBgId(void *, u32);

extern int PCBox_GetSlotMonData(void *, int, int, int);
extern void *PCBox_GetSlotBoxMon(void *, int, int);
extern void PCBox_SetSlotMonData(void *, int, int, void *);
extern u32 PCBox_UpdateArceusGiratinaForm(void *);
extern void PCBox_StartReleaseMoveCheck(void *);
extern u32 PCBox_ShowSlotMonInfo(void *, u32);
extern void PCBox_RedrawSlotMonInfo(void *, u32);
extern void PCBox_ClearMonInfoAndSelection(void *);
extern void ov14_021E884C(void *);
extern void ov14_021E88F8(void *);
extern u32 PCBox_StartFadeOut(void *, u32);
extern u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
extern u32 PCBox_ShowYesNo(void *, u32);
extern void ov14_021F0AD8(void *);
extern void ov14_021F0C88(void *);
extern u32 ov14_021F1100(void *, u32);
extern u32 ov14_021F15C8(void *, u32);
extern void PCBox_SetSpriteAnim(void *, u32, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_RefreshSlotIcon(void *, int, int, int);
extern void PCBox_StartReleaseScale(void *);
extern void PCBox_SetIconsTranslucent(void *, u32, u32);
extern void ov14_021F391C(void *, u32);
extern u32 PCBox_HideSelectedIconOutline(void *);
extern u32 PCBox_SetSelectedIconOutlineVisible(void *, u32);
extern void PCBox_SetBoxArrowsVisible(void *, u32);
extern void PCBox_CloseMessageWindow(void *, u32);
extern void PCBox_PrintExitPrompt(void *, int, int);
extern void PCBox_PrintTookItem(void *, int, int);
extern void PCBox_PrintBagFull(void *, int);
extern void PCBox_PrintMonActionMessage(void *, u32, u32, u32);
extern void PCBox_PrintItemActionMessage(void *, u32, u32);
extern u32 ov14_021F7340(void *);
extern u32 PCBox_IsSummaryShortcutPressed(void *);

u32 ov14_021F1F44(void *p);
u32 ov14_021F2010(void *p);
u32 PCBox_TakeHeldItemToBag(void *p);
u32 PCBox_ConfirmRelease(void *p);
u32 PCBox_PutAwayHeldItem(void *p);
u32 PCBox_ConfirmExit(void *p);
u32 PCBox_CancelExit(void *p);
u32 PCBox_FlashPanel(void *p, u32 a1, u32 a2);
u32 ov14_021F2330(void *p, u32 a1, u32 a2);
u32 ov14_021F23F0(void *p, u32 a1, u32 a2);
u32 ov14_021F2490(void *p, u32 a1, u32 a2);
void ov14_021F2534(void *p);
u32 ov14_021F2574(void *p);
u32 ov14_021F259C(void *p);
void ov14_021F25C4(void *p);
void ov14_021F25D4(void *p);
u32 ov14_021F25E4(void *p);

#endif
