#ifndef TO50_OVERLAY15_PREFIX_R14_PRIVATE_H
#define TO50_OVERLAY15_PREFIX_R14_PRIVATE_H

#include "global.h"
struct ItemSlot;

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define VU8(p, o) (*(volatile u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern u8 gSystem[];

extern u32 YesNoPrompt_HandleInput(void *);
extern void BagApp_DestroyYesNoPrompt(void *);
extern void *NewString_ReadMsgData(void *, s32);
extern void BufferItemNamePlural(void *, u32, u16);
extern void BufferItemName(void *, u32, u16);
extern void BufferIntegerAsString(void *, u32, s32, u32, u32, u32);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void String_Delete(void *);
extern s32 BagApp_PrintMessage(void *, s32);
extern void EraseFrameAndWindow1(void *, s32);
extern void FillWindowPixelBuffer(void *, u32);
extern void ClearFrameAndWindow2(void *, BOOL);
extern void ClearWindowTilemapAndScheduleTransfer(void *);
extern void ScheduleWindowCopyToVram(void *);
extern s32 BagApp_GetPageItemCount(void *);
extern void BagApp_DrawItemIcons(void *, void *, s32, BOOL);
extern void BagApp_ClearMoveInfoWindow(void *);
extern void BagApp_CloseItemMenuWindows(void *);
extern void BagApp_RedrawPocket(void *);
extern void BagApp_ShowPocketTabSprites(void *, s32);
extern void BagApp_ShowCancelButton(void *, s32);
extern BOOL TextPrinterCheckActive(u8);
extern void PlaySE(u32);
extern void PlayerProfile_AddMoney(void *, u32);
extern BOOL Pocket_TakeItem(struct ItemSlot *, u32, u16, u16, u32);
extern void BagApp_LoadPocketItemNames(void *);
extern void BagApp_PrintCursorDescription(void *);

s32 BagApp_State_TrashYesNo(void *);

#endif
