#ifndef OV15_R16_PRIVATE_H
#define OV15_R16_PRIVATE_H

#include "global.h"

extern u8 gSystem[];

BOOL TextPrinterCheckActive(u8);
void ClearFrameAndWindow2(void *, BOOL);
void ClearWindowTilemapAndScheduleTransfer(void *);
void FillWindowPixelBuffer(void *, u32);
void EraseFrameAndWindow1(void *, BOOL);
void *NewString_ReadMsgData(void *, u32);
void BufferIntegerAsString(void *, u32, s32, u32, u32, BOOL);
void StringExpandPlaceholders(void *, void *, void *);
void String_Delete(void *);
void ScheduleWindowCopyToVram(void *);

s32 BagApp_GetPageItemCount(void *);
void BagApp_RedrawPocket(void *);
void BagApp_LoadSubScreenLayout(void *, u32, u32, u32);
void BagApp_ShowCancelButton(void *, u32);
void BagApp_ClearMoveInfoWindow(void *);
void BagApp_CloseItemMenuWindows(void *);
s32 BagApp_PrintMessage(void *, u32);
void BagApp_PrintCancelButton(void *, u32);
void BagApp_DrawItemList(void *, s32, s32, s32);
void BagApp_PrintSellLabel(void *);
void BagApp_ClearQuantityLabel(void *);
void BagApp_HideCursor(void *);
void BagApp_SetQuantityCursorPosition(void *, u32);
void BagApp_DrawItemIcons(void *, void *, s32, u32);
void BagApp_ShowQuantityButtons(void *, u32, u32);
void BagApp_HideQuantityButtons(void *);
void BagApp_ShowPocketTabSprites(void *, u32);
void BagApp_SetCancelSpriteVisible(void *, u32);

s32 BagApp_State_WaitCantHoldMessage(void *);
s32 BagApp_State_SetupSellQuantity(void *);
s32 BagApp_State_ShowSellPrice(void *);
s32 BagApp_State_CancelSellQuantity(void *);

#endif
