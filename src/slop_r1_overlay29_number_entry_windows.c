#include "slop_r1_overlay29_number_entry_windows_internal.h"

void ov29_0225D714(Ov29NumberEntryDataSlopR1 *data) {
    MsgData *msgData;
    String *string;
    u16 i;
    u16 center;
    u16 x;

    msgData = NewMsgDataFromNarc((MsgDataLoadType)0, (NarcId)0x1B, 0xC4, (enum HeapID)8);
    FontID_Alloc((FontID)4, (enum HeapID)8);
    for (i = 0; i < 3; i++) {
        AddWindow(data->bgConfig, &data->windows[i], &ov29_0225E178[i]);
        FillWindowPixelBuffer(&data->windows[i], 0);
        string = NewString_ReadMsgData(msgData, 0x18 + i);
        center = GetWindowWidth(&data->windows[i]) * 8 / 2;
        x = center - FontID_String_GetWidth((FontID)4, string, 0) / 2;
        AddTextPrinterParameterizedWithColor(&data->windows[i], (FontID)4, string, x, 0, 0xFF, 0xF0E00, NULL);
        String_Delete(string);
        CopyWindowPixelsToVram_TextMode(&data->windows[i]);
        ScheduleWindowCopyToVram(&data->windows[i]);
    }
    FontID_Release((FontID)4);
    DestroyMsgData(msgData);
}
