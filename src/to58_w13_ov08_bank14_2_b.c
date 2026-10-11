#include "to58_overlay08_bank14_2_private.h"
extern void *String_New(int, int);
extern void BufferAbilityName(void *, int, int);
extern void BufferItemName(void *, int, int);
void BattlePartyMenu_PrintAbilityName(void *, int, int);
void BattlePartyMenu_PrintHeldItemName(void *, int, int);
void BattlePartyMenu_PrintMoveName(void *, int, int, int, u16, u16, u32);

void BattlePartyMenu_PrintAbilityName(void *arg, int window, int mon) {
    Ov08R2W11Work *work = arg;
    void *string = String_New(16, *(int *)((u8 *)work->root + 12));
    void *message = NewString_ReadMsgData(work->msgData, 8);
    BufferAbilityName(*(void **)((u8 *)work + 0x1FAC), 0, *(u16 *)((u8 *)work + mon * 0x50 + 0x1C));
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), string, message);
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + window * 16), 0, string, 0, 0, 255, 0xF0E00, 0);
    String_Delete(message);
    String_Delete(string);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + window * 16));
}

void BattlePartyMenu_PrintHeldItemName(void *arg, int window, int mon) {
    Ov08R2W11Work *work = arg;
    u8 *entry = (u8 *)work + 4 + mon * 0x50;
    void *string;
    if (*(u16 *)(entry + 0x1A) == 0) {
        string = NewString_ReadMsgData(work->msgData, 20);
    } else {
        void *message;
        string = String_New(18, *(int *)((u8 *)work->root + 12));
        message = NewString_ReadMsgData(work->msgData, 9);
        BufferItemName(*(void **)((u8 *)work + 0x1FAC), 0, *(u16 *)(entry + 0x1A));
        StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), string, message);
        String_Delete(message);
    }
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + window * 16), 0, string, 0, 0, 255, 0xF0E00, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + window * 16));
}

void BattlePartyMenu_PrintMoveName(void *arg, int move, int window, int msgNo, u16 font, u16 y, u32 color) {
    Ov08R2W11Work *work = arg;
    Window *windows = work->windowArray;
    int offset = window << 4;
    Window *win = (Window *)((u8 *)windows + offset);
    void *string = String_New(16, *(int *)((u8 *)work->root + 12));
    void *message = NewString_ReadMsgData(work->msgData, msgNo);
    int x;
    BufferMoveName(*(void **)((u8 *)work + 0x1FAC), 0, move);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), string, message);
    if (font == 4) {
        x = ((GetWindowWidth(win) << 3) - FontID_String_GetWidth(font, string, 0)) >> 1;
    } else {
        x = 0;
    }
    AddTextPrinterParameterizedWithColor(win, font, string, x, y, 255, color, 0);
    String_Delete(message);
    String_Delete(string);
    ScheduleWindowCopyToVram(win);
}
