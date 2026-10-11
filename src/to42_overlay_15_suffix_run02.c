#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "sprite_system.h"
#include "string_util.h"
#include "text.h"
#include "to42_overlay_15_suffix_private.h"
#include "sound_playback.h"
#include "yes_no_prompt.h"

void BagApp_LoadItemMenuStrings(BagWorkRaw *);
void BagApp_DrawRegisteredItemIcon(BagWorkRaw *, Window *, u16, BOOL);
void BagApp_FreeItemMenuStrings(BagWorkRaw *);
void BagApp_ShowItemDescription(BagWorkRaw *, Window *, u32);
void BagApp_ClearDescriptionWindow(BagWorkRaw *, Window *);
void BagApp_ShowPocketDescription(BagWorkRaw *, Window *, u32);
void BagApp_CloseItemPromptWindow(BagWorkRaw *);
void BagApp_CloseItemMenuWindows(BagWorkRaw *);
void BagApp_CloseItemActionWindows(BagWorkRaw *);
void BagApp_PrintMoveItemPrompt(BagWorkRaw *);
void BagApp_PrintQuantityDigits(BagWorkRaw *, u32);
u8 BagApp_PrintMessage(BagWorkRaw *, BOOL);
void BagApp_PrintTrashConfirmMessage(BagWorkRaw *);
void BagApp_DestroyYesNoPrompt(BagWorkRaw *);
void BagApp_CreateYesNoPrompt(BagWorkRaw *);
BOOL BagApp_TextPrinterCallback(TextPrinterTemplate *, u16);
void BagApp_PrintCancelButton(BagWorkRaw *, BOOL);
void BagApp_DrawSelectedItemEntry(BagWorkRaw *, s32, s32);
void ov15_021FF1E0(BagWorkRaw *);
void BagApp_PrintMoney(BagWorkRaw *, BOOL);
void BagApp_PrintSellPrice(BagWorkRaw *);
void BagApp_ClearSelectedItemEntry(BagWorkRaw *);
void BagApp_PrintPageNumber(BagWorkRaw *, s32, s32, s32);
void BagApp_ClearItemMenuLabels(Window *);
void BagApp_PrintItemMenuLabel(Window *, String **, u32);
void BagApp_PrintTrashLabel(BagWorkRaw *);
void BagApp_PrintSellLabel(BagWorkRaw *);
void BagApp_ClearQuantityLabel(BagWorkRaw *);
void BagApp_ClearCancelButtonText(BagWorkRaw *);
void BagApp_FreeSprites(BagWorkRaw *);
void BagApp_TickSprites(BagWorkRaw *);
void ov15_021FF950(BagWorkRaw *);
void ov15_021FF964(BagWorkRaw *);
void ov15_021FFEC0(BagWorkRaw *);
void BagApp_SetCursorPosition(BagWorkRaw *, s32);
void BagApp_HideCursor(BagWorkRaw *);
void BagApp_SetQuantityCursorPosition(BagWorkRaw *, s32);
void BagApp_LoadPocketObjPalette(BagWorkRaw *, s32);
void BagApp_HideItemMenuSprites(BagWorkRaw *);
void BagApp_ShowMoveSlotSprites(BagWorkRaw *, s32, s32, BOOL);
void BagApp_UpdatePageArrows(BagWorkRaw *);
void BagApp_ShowItemMenuSprites(BagWorkRaw *, const u8 *);
void BagApp_ShowSelectedItemIcon(BagWorkRaw *, s32);
s32 BagApp_GetUnusedDigitCount(s32);
void BagApp_HideQuantityButtons(BagWorkRaw *);
void BagApp_ShowPocketTabSprites(BagWorkRaw *, BOOL);
void BagApp_SetCancelSpriteVisible(BagWorkRaw *, BOOL);
void BagApp_ShowTMHMTypeIcons(BagWorkRaw *, u16, BOOL);

void BagApp_ShowItemDescription(BagWorkRaw *work, Window *window, u32 value) {
    FillWindowPixelBuffer(window, 0);
    BagApp_PrintItemDescription(work, value);
    ScheduleWindowCopyToVram(window);
}

void BagApp_ClearDescriptionWindow(BagWorkRaw *work, Window *window) {
    FillWindowPixelBuffer(window, 0);
    ScheduleWindowCopyToVram(window);
}

void BagApp_ShowPocketDescription(BagWorkRaw *work, Window *window, u32 index) {
    String *string = NewString_ReadMsgData(*(MsgData **)&work->raw[0x2F0], index + 0x78);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0x14, 0, 0xFF, 0x000F0E00, NULL);
    String_Delete(string);
    ScheduleWindowCopyToVram(window);
}

void BagApp_CloseItemPromptWindow(BagWorkRaw *work) {
    ClearFrameAndWindow2((Window *)&work->raw[0x24], TRUE);
    ClearWindowTilemapAndScheduleTransfer((Window *)&work->raw[0x24]);
}

void BagApp_CloseItemMenuWindows(BagWorkRaw *work) {
    BagApp_CloseItemPromptWindow(work);
    BagApp_RemoveItemActionWindows(work);
    BagApp_ShowTMHMTypeIcons(work, 0, FALSE);
}

void BagApp_CloseItemActionWindows(BagWorkRaw *work) {
    BagApp_RemoveItemActionWindows(work);
}

void BagApp_PrintMoveItemPrompt(BagWorkRaw *work) {
    FillWindowPixelBuffer((Window *)&work->raw[0x24], 0xFF);
    String *source = NewString_ReadMsgData(*(MsgData **)&work->raw[0x2F0], 0x2E);
    String *dest = String_New(0x82, HEAP_ID_6);
    BagApp_BufferSlotItemName(work, work->raw[0x672], 0);
    StringExpandPlaceholders(*(MessageFormat **)&work->raw[0x2F4], dest, source);
    DrawFrameAndWindow2((Window *)&work->raw[0x24], TRUE, 0x3E2, 0xC);
    AddTextPrinterParameterizedWithColor((Window *)&work->raw[0x24], 1, dest, 0, 0, 0xFF, 0x00010200, NULL);
    ScheduleWindowCopyToVram((Window *)&work->raw[0x24]);
    String_Delete(dest);
    String_Delete(source);
}
