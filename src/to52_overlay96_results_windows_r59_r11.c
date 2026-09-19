#include "to52_overlay96_results_windows_r59_r11_private.h"

void ov96_021F6F80(void *course, Ov96R59UiWork *work) {
    u8 i;
    Window *window;
    u8 playerIndex;
    String *name;
    u8 line;
    u8 slot;

    for (i = 0; i < 5; i++) {
        FillWindowPixelBuffer(&work->windows[i], 0);
    }
    playerIndex = ov96_021E5F24(course);
    name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, playerIndex), (enum HeapID)work->heapId);
    AddTextPrinterParameterizedWithColor(&work->windows[0], 0, name, 0, 0, 0xFF, 0x000F0E00, NULL);
    String_Delete(name);
    line = 1;
    slot = 0;
    window = &work->windows[0];
    for (; slot < 4; slot++) {
        if (slot != playerIndex) {
            name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, slot), (enum HeapID)work->heapId);
            AddTextPrinterParameterizedWithColor(window + line, 0, name, 0, 0, 0xFF, 0x000F0E00, NULL);
            String_Delete(name);
            line++;
        }
    }
    for (i = 0; i < 5; i++) {
        CopyWindowToVram(&work->windows[i]);
    }
}

void ov96_021F7050(Ov96R59UiWork *work) {
    u8 i;
    for (i = 0; i < 5; i++) {
        AddWindow(work->bgConfig, &work->windows[i], &ov96_0221C1CC[i]);
    }
    BG_FillCharDataRange(work->bgConfig, (GFBgLayer)6, 0, 1, 0);
    LoadFontPal0((enum GFPalLoadLocation)4, (enum GFPalSlotOffset)0x1E0, (enum HeapID)work->heapId);
    BG_FillCharDataRange(work->bgConfig, (GFBgLayer)3, 0, 1, 0);
    LoadFontPal0((enum GFPalLoadLocation)0, (enum GFPalSlotOffset)0x1E0, (enum HeapID)work->heapId);
}

void ov96_021F70AC(Ov96R59UiWork *work, int value) {
    MsgData *msgData;
    MessageFormat *format;
    String *string;
    FillWindowPixelBuffer(&work->windows[4], 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (NarcId)0x1B, 0x135, (enum HeapID)work->heapId);
    format = MessageFormat_New((enum HeapID)work->heapId);
    BufferIntegerAsString(format, 0, value, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(format, msgData, 0xA1, (enum HeapID)work->heapId);
    AddTextPrinterParameterizedWithColor(&work->windows[4], 4, string, 0, 0, 0xFF, 0x000F0E00, NULL);
    String_Delete(string);
    MessageFormat_Delete(format);
    DestroyMsgData(msgData);
    CopyWindowToVram(&work->windows[4]);
}
