#include "to57_overlay08_bank14_private.h"
#pragma force_active on
void *String_New(int, int);
void BufferIntegerAsString(void *, int, int, int, int, int);
void BattlePartyMenu_PrintLevelAndNextExp(void *, int);
void BattlePartyMenu_PrintAttack(void *, int);
void BattlePartyMenu_PrintDefense(void *, int);
void BattlePartyMenu_PrintSpeed(void *, int);
void BattlePartyMenu_PrintSpAttack(void *, int);
void BattlePartyMenu_PrintSpDefense(void *, int);
void BattlePartyMenu_PrintHPStat(void *, int);
typedef struct Ov08ItemBits { u8 count : 7; u8 flag : 1; } Ov08ItemBits;

void BattlePartyMenu_PrintLevelAndNextExp(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    u8 *item = (u8 *)work + 4 + battler * 0x50;
    u16 selector = *((u8 *)work + 0x2075) * 22;
    void *label = NewString_ReadMsgData(work->msgData, 0x17);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0x130, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);

    label = NewString_ReadMsgData(work->msgData, 0x18);
    void *expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, ((Ov08ItemBits *)(item + 0x16))->count, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, label);
    int offset = (selector + 0xB) << 4;
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + offset, 0, expanded, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    String_Delete(expanded);

    label = NewString_ReadMsgData(work->msgData, 0x19);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0x140, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);

    label = NewString_ReadMsgData(work->msgData, 0x1A);
    expanded = String_New(0xE, *(int *)((u8 *)work->root + 0xC));
    if (((Ov08ItemBits *)(item + 0x16))->count < 0x64) {
        BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0,
                              *(u32 *)(item + 0x24) - *(u32 *)(item + 0x1C), 6, 1, 1);
    } else {
        BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, 0, 6, 1, 1);
    }
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, label);
    offset = (selector + 0xC) << 4;
    u32 width = GetWindowWidth((Window *)((u8 *)work->windowArray + offset));
    u16 x = width * 8 - FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + offset, 0, expanded, x, 0, 0xFF, 0x10200, 0);
    String_Delete(label);
    String_Delete(expanded);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0x130));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + ((selector + 0xB) << 4)));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0x140));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
void BattlePartyMenu_PrintAttack(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    u16 selector = *((u8 *)work + 0x2075) * 22;
    void *label = NewString_ReadMsgData(work->msgData, 0x20);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0xE0, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    void *template = NewString_ReadMsgData(work->msgData, 0x21);
    void *expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    int rowOffset = battler * 0x50;
    u16 value = *(u16 *)((u8 *)work + rowOffset + 0xA);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, value, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, template);
    u8 width = FontID_String_GetWidth(0, expanded, 0);
    int offset = (selector + 5) << 4;
    u8 x = GetWindowWidth((Window *)((u8 *)work->windowArray + offset)) * 8 - width;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + offset), 0, expanded, x, 0, 0xFF, 0x10200, 0);
    String_Delete(template);
    String_Delete(expanded);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0xE0));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
void BattlePartyMenu_PrintDefense(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    u16 selector = *((u8 *)work + 0x2075) * 22;
    void *label = NewString_ReadMsgData(work->msgData, 0x22);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0xF0, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    void *template = NewString_ReadMsgData(work->msgData, 0x23);
    void *expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    int rowOffset = battler * 0x50;
    u16 value = *(u16 *)((u8 *)work + rowOffset + 0xC);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, value, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, template);
    u8 width = FontID_String_GetWidth(0, expanded, 0);
    int offset = (selector + 6) << 4;
    u8 x = GetWindowWidth((Window *)((u8 *)work->windowArray + offset)) * 8 - width;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + offset), 0, expanded, x, 0, 0xFF, 0x10200, 0);
    String_Delete(template);
    String_Delete(expanded);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0xF0));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
void BattlePartyMenu_PrintSpeed(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    u16 selector = *((u8 *)work + 0x2075) * 22;
    void *label = NewString_ReadMsgData(work->msgData, 0x28);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0x100, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    void *template = NewString_ReadMsgData(work->msgData, 0x29);
    void *expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    int rowOffset = battler * 0x50;
    u16 value = *(u16 *)((u8 *)work + rowOffset + 0xE);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, value, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, template);
    u8 width = FontID_String_GetWidth(0, expanded, 0);
    int offset = (selector + 7) << 4;
    u8 x = GetWindowWidth((Window *)((u8 *)work->windowArray + offset)) * 8 - width;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + offset), 0, expanded, x, 0, 0xFF, 0x10200, 0);
    String_Delete(template);
    String_Delete(expanded);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0x100));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
void BattlePartyMenu_PrintSpAttack(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    u16 selector = *((u8 *)work + 0x2075) * 22;
    void *label = NewString_ReadMsgData(work->msgData, 0x24);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0x110, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    void *template = NewString_ReadMsgData(work->msgData, 0x25);
    void *expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    int rowOffset = battler * 0x50;
    u16 value = *(u16 *)((u8 *)work + rowOffset + 0x10);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, value, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, template);
    u8 width = FontID_String_GetWidth(0, expanded, 0);
    int offset = (selector + 8) << 4;
    u8 x = GetWindowWidth((Window *)((u8 *)work->windowArray + offset)) * 8 - width;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + offset), 0, expanded, x, 0, 0xFF, 0x10200, 0);
    String_Delete(template);
    String_Delete(expanded);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0x110));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
void BattlePartyMenu_PrintSpDefense(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    u16 selector = *((u8 *)work + 0x2075) * 22;
    void *label = NewString_ReadMsgData(work->msgData, 0x26);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0x120, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);
    void *template = NewString_ReadMsgData(work->msgData, 0x27);
    void *expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    int rowOffset = battler * 0x50;
    u16 value = *(u16 *)((u8 *)work + rowOffset + 0x12);
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, value, 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, template);
    u8 width = FontID_String_GetWidth(0, expanded, 0);
    int offset = (selector + 9) << 4;
    u8 x = GetWindowWidth((Window *)((u8 *)work->windowArray + offset)) * 8 - width;
    AddTextPrinterParameterizedWithColor((Window *)((u8 *)work->windowArray + offset), 0, expanded, x, 0, 0xFF, 0x10200, 0);
    String_Delete(template);
    String_Delete(expanded);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0x120));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
void BattlePartyMenu_PrintHPStat(void *arg, int battler) {
    Ov08R2W11Work *work = arg;
    u8 *item = (u8 *)work + 4 + battler * 0x50;
    u16 selector = *((u8 *)work + 0x2075) * 22;
    void *label = NewString_ReadMsgData(work->msgData, 0x1C);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + 0xD0, 0, label, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(label);

    label = NewString_ReadMsgData(work->msgData, 0x1F);
    u32 width = FontID_String_GetWidth(0, label, 0);
    u32 windowWidth = GetWindowWidth((Window *)((u8 *)work->windowArray + 0x40));
    u16 middle = (windowWidth * 8 - width) / 2;
    int offset = (selector + 4) << 4;
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + offset, 0, label, middle, 0, 0xFF, 0x10200, 0);
    String_Delete(label);

    label = NewString_ReadMsgData(work->msgData, 0x1D);
    void *expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, *(u16 *)(item + 0x10), 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, label);
    u32 expandedWidth = FontID_String_GetWidth(0, expanded, 0);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + offset, 0, expanded, middle - expandedWidth, 0, 0xFF, 0x10200, 0);
    String_Delete(label);
    String_Delete(expanded);

    label = NewString_ReadMsgData(work->msgData, 0x1E);
    expanded = String_New(8, *(int *)((u8 *)work->root + 0xC));
    BufferIntegerAsString(*(void **)((u8 *)work + 0x1FAC), 0, *(u16 *)(item + 0x12), 3, 0, 1);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), expanded, label);
    AddTextPrinterParameterizedWithColor((u8 *)work->windowArray + offset, 0, expanded, middle + width, 0, 0xFF, 0x10200, 0);
    String_Delete(label);
    String_Delete(expanded);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + 0xD0));
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
