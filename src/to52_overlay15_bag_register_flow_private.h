#ifndef TO52_OV15_BAG_REGISTER_PRIVATE_H
#define TO52_OV15_BAG_REGISTER_PRIVATE_H
#include "global.h"
struct ItemSlot;
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
typedef struct BagSelectionEntry {
    void *slots;
    u8 padding[5];
    u8 quantity;
    u8 tail[2];
} BagSelectionEntry;
typedef struct BagArgs {
    u8 padding[4];
    BagSelectionEntry entries[8];
    u8 selected;
    u8 padding65;
    u16 item;
} BagArgs;
extern u8 gSystem[];
extern BOOL TextPrinterCheckActive(u8);
extern void ClearFrameAndWindow2(void *, BOOL);
extern void ClearWindowTilemapAndScheduleTransfer(void *);
extern void ScheduleWindowCopyToVram(void *);
extern BOOL Pocket_TakeItem(struct ItemSlot *, u32, u16, u16, u32);
extern void BagApp_LoadPocketItemNames(void *);
extern s32 BagApp_GetPageItemCount(void *);
extern void BagApp_DrawItemIcons(void *, void *, s32, BOOL);
extern void BagApp_ClearMoveInfoWindow(void *);
extern void BagApp_CloseItemMenuWindows(void *);
extern void BagApp_RedrawPocket(void *);
extern void BagApp_PrintCursorDescription(void *);
extern void BagApp_ShowCancelButton(void *, s32);
extern s32 Bag_TryRegisterItem(void *, u16);
extern void Bag_UnregisterItem(void *, u16);
extern void *NewString_ReadMsgData(void *, s32);
extern void FillWindowPixelBuffer(void *, u32);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void String_Delete(void *);
extern s32 BagApp_PrintMessage(void *, s32);
extern void BagApp_HideItemMenuSprites(void *);
extern void BagApp_HideCursor(void *);
extern void BagApp_ClearSelectedItemEntry(void *);
extern void BagApp_ClearItemMenuLabels(void *);
s32 BagApp_State_FinishTrash(void *);
s32 BagApp_ItemAction_Register(void *);
s32 BagApp_State_WaitRegisterFailMessage(void *);
s32 BagApp_ItemAction_Deselect(void *);
#endif
