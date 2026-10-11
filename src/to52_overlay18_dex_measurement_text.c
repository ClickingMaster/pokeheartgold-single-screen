#include "to52_overlay18_dex_measurement_text_private.h"

void PokedexApp_PrintHeight(void *work, u32 msgNo, u32 mode, u32 windowIndex, u32 x, u32 y, u32 color, u32 align) {
    void *msgData = NewMsgDataFromNarc(0, 0x1b, GetDexHeightMsgBank(), 0x25);
    void *string;
    if (mode == 2) {
        string = NewString_ReadMsgData(msgData, msgNo);
    } else {
        string = NewString_ReadMsgData(msgData, 0);
    }
    PokedexApp_PrintString((u8 *)work + 0xc + (windowIndex << 4), string, x, y, 0, color, align);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void PokedexApp_DrawHeight(void *work, u32 msgNo, u32 slot, u32 windowIndex) {
    u8 *windows = (u8 *)work + 0xc;
    u32 offset = windowIndex << 4;
    FillWindowPixelBuffer(windows + offset, 0);
    PokedexApp_PrintHeight(work, msgNo, *(u16 *)((u8 *)work + (slot << 2) + 0x1032), windowIndex, 4, 0, 0x20100, 0);
    ScheduleWindowCopyToVram(windows + offset);
}

void PokedexApp_PrintWeight(void *work, u32 msgNo, u32 mode, u32 windowIndex, u32 x, u32 y, u32 color, u32 align) {
    void *msgData = NewMsgDataFromNarc(0, 0x1b, GetDexWeightMsgBank(), 0x25);
    void *string;
    if (mode == 2) {
        string = NewString_ReadMsgData(msgData, msgNo);
    } else {
        string = NewString_ReadMsgData(msgData, 0);
    }
    PokedexApp_PrintString((u8 *)work + 0xc + (windowIndex << 4), string, x, y, 0, color, align);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void PokedexApp_DrawWeight(void *work, u32 msgNo, u32 slot, u32 windowIndex) {
    u8 *windows = (u8 *)work + 0xc;
    u32 offset = windowIndex << 4;
    FillWindowPixelBuffer(windows + offset, 0);
    PokedexApp_PrintWeight(work, msgNo, *(u16 *)((u8 *)work + (slot << 2) + 0x1032), windowIndex, 4, 0, 0x20100, 0);
    ScheduleWindowCopyToVram(windows + offset);
}
