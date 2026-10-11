#include "to50_overlay15_bag_item_move_text_private.h"
void BagApp_PrintItemDescription(void *work, u16 item) {
    void *str;
    if (item != 0xFFFF) {
        str = String_New(0x82, 6);
        GetItemDescIntoString(str, item, 6);
    } else {
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x61);
    }
    AddTextPrinterParameterizedWithColor((u8 *)work + 4, 0, str, 0x14, 0, 0xFF, 0xF0E00, 0);
    String_Delete(str);
}

void BagApp_PrintTMHMMoveInfo(void *work, u16 tmhm) {
    void *window = (u8 *)work + 0x14;
    u16 move = TMHMGetMove(tmhm);
    void *str;
    u32 maxpp;
    u16 value;

    str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x65);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, 0xFF, 0xF0E00, 0);
    String_Delete(str);
    str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x59);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0x10, 0xFF, 0xF0E00, 0);
    String_Delete(str);
    str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x5C);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0x48, 0, 0xFF, 0xF0E00, 0);
    String_Delete(str);
    str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x5A);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0xA8, 0, 0xFF, 0xF0E00, 0);
    String_Delete(str);
    str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x5B);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0xA8, 0x10, 0xFF, 0xF0E00, 0);
    String_Delete(str);

    maxpp = GetMoveMaxPP(move, 0);
    str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x5D);
    BufferIntegerAsString(PTR(work, 0x2F4), 0, maxpp, 2, 1, 1);
    StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), str);
    String_Delete(str);
    AddTextPrinterParameterizedWithColor(window, 0, PTR(work, 0x5E4), 0x30, 0x10, 0xFF, 0xF0E00, 0);

    value = GetMoveAttr(move, 2);
    if (value <= 1) {
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x19);
    } else {
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x5E);
    }
    BufferIntegerAsString(PTR(work, 0x2F4), 0, value, 3, 0, 1);
    StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), str);
    String_Delete(str);
    AddTextPrinterParameterizedWithColor(window, 0, PTR(work, 0x5E4), 0xE8, 0, 0xFF, 0xF0E00, 0);

    value = GetMoveAttr(move, 4);
    if (value == 0) {
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x19);
    } else {
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x5E);
    }
    BufferIntegerAsString(PTR(work, 0x2F4), 0, value, 3, 0, 1);
    StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), str);
    String_Delete(str);
    AddTextPrinterParameterizedWithColor(window, 0, PTR(work, 0x5E4), 0xE8, 0x10, 0xFF, 0xF0E00, 0);
    ScheduleWindowCopyToVram(window);
}
