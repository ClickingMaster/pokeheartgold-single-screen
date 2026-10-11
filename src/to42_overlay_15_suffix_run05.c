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

void BagApp_PrintMoney(BagWorkRaw *work, BOOL skipLabel) {
    String *dest = String_New(0x100, HEAP_ID_6);
    Window *window = (Window *)&work->raw[0x214];
    if (skipLabel == FALSE) {
        FillWindowPixelBuffer(window, 0);
        String *label = NewString_ReadMsgData(*(MsgData **)&work->raw[0x2F0], 0x50);
        AddTextPrinterParameterizedWithColor(window, 0, label, 4, 0, 0xFF, 0x00010200, NULL);
        String_Delete(label);
    } else {
        FillWindowPixelRect(window, 0, 0, 0x10, 0x48, 0x10);
    }
    String *source = NewString_ReadMsgData(*(MsgData **)&work->raw[0x2F0], 0x51);
    u32 money = PlayerProfile_GetMoney(*(PlayerProfile **)&work->raw[0x23C]);
    BufferIntegerAsString(*(MessageFormat **)&work->raw[0x2F4], 0, money, 6, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(*(MessageFormat **)&work->raw[0x2F4], dest, source);
    u32 width = FontID_String_GetWidth(0, dest, 0);
    AddTextPrinterParameterizedWithColor(window, 0, dest, 0x44 - (width + 8), 0x10, 0xFF, 0x00010200, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(source);
    String_Delete(dest);
}

void ov15_021FF1E0(BagWorkRaw *work) {
    Window *window = (Window *)&work->raw[0x54];
    FillWindowPixelBuffer(window, 0xF);
    DrawFrameAndWindow1(window, TRUE, 0x3F7, 0xE);
    String *string = NewString_ReadMsgData(*(MsgData **)&work->raw[0x2F0], 0x73);
    AddTextPrinterParameterized(window, 0, string, 0, 0, 0xFF, NULL);
    String_Delete(string);
    string = NewString_ReadMsgData(*(MsgData **)&work->raw[0x2F0], 0x74);
    BufferIntegerAsString(*(MessageFormat **)&work->raw[0x2F4], 0, 0, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(*(MessageFormat **)&work->raw[0x2F4], *(String **)&work->raw[0x5E4], string);
    String_Delete(string);
    u32 width = FontID_String_GetWidth(0, *(String **)&work->raw[0x5E4], 0);
    AddTextPrinterParameterized(window, 0, *(String **)&work->raw[0x5E4], 0x58 - width, 0x10, 0xFF, NULL);
    ScheduleWindowCopyToVram(window);
}

void BagApp_PrintCancelButton(BagWorkRaw *work, BOOL left) {
    String *string = NewString_ReadMsgData(*(MsgData **)&work->raw[0x2F0], 8);
    FillWindowPixelBuffer((Window *)&work->raw[0x74], 0);
    if (left == FALSE) {
        u32 width = FontID_String_GetWidth(0, string, 0);
        AddTextPrinterParameterizedWithColor((Window *)&work->raw[0x74], 0, string, ((0x30 - width) / 2) + 8, 0, 0xFF, 0x000F0E00, NULL);
    } else {
        AddTextPrinterParameterizedWithColor((Window *)&work->raw[0x74], 0, string, 5, 0, 0xFF, 0x000F0E00, NULL);
    }
    ScheduleWindowCopyToVram((Window *)&work->raw[0x74]);
    String_Delete(string);
}
