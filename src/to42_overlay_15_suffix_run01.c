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

void BagApp_LoadItemMenuStrings(BagWorkRaw *work) {
    typedef struct {
        u8 pad[0x2F0];
        MsgData *msg;
        u8 pad2[0xC];
        String *strings[16];
    } Work;
    Work *w = (Work *)work;
    w->strings[0] = NewString_ReadMsgData(w->msg, 0);
    w->strings[1] = NewString_ReadMsgData(w->msg, 6);
    w->strings[2] = NewString_ReadMsgData(w->msg, 0x10);
    w->strings[3] = NewString_ReadMsgData(w->msg, 0x62);
    w->strings[4] = NewString_ReadMsgData(w->msg, 0x63);
    w->strings[5] = NewString_ReadMsgData(w->msg, 1);
    w->strings[6] = NewString_ReadMsgData(w->msg, 2);
    w->strings[7] = NewString_ReadMsgData(w->msg, 0x12);
    w->strings[8] = NewString_ReadMsgData(w->msg, 3);
    w->strings[9] = NewString_ReadMsgData(w->msg, 4);
    w->strings[10] = NewString_ReadMsgData(w->msg, 5);
    w->strings[11] = NewString_ReadMsgData(w->msg, 8);
    w->strings[12] = NewString_ReadMsgData(w->msg, 0x4B);
    w->strings[13] = NewString_ReadMsgData(w->msg, 0x56);
    w->strings[14] = NewString_ReadMsgData(w->msg, 0);
    w->strings[15] = NewString_ReadMsgData(w->msg, 0x80);
}

void BagApp_FreeItemMenuStrings(BagWorkRaw *work) {
    typedef struct {
        u8 pad[0x300];
        String *strings[16];
    } Work;
    u16 i;
    for (i = 0; i < 16; i++) {
        String_Delete(((Work *)work)->strings[i]);
    }
}
