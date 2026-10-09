#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "string_util.h"
#include "text.h"

typedef struct Ov27Unk {
    u8 unk0[4];
    BgConfig *bgConfig;
    u8 unk8[0x4A0];
    MsgData *msgData;
    MessageFormat *msgFormat;
} Ov27Unk;

void ov27_0225BED8(Ov27Unk *data, u32 msgId, u8 x, u8 y, u8 width, u8 height, u32 baseTile, s16 textX, s16 textY);

// @fn ov27_0225BED8
void ov27_0225BED8(Ov27Unk *data, u32 msgId, u8 x, u8 y, u8 width, u8 height, u32 baseTile, s16 textX, s16 textY)
{
    Window window;
    String *string;

    AddWindowParameterized(data->bgConfig, &window, 5, x, y, width, height, 4, (u16)baseTile);
    FillWindowPixelBuffer(&window, 0);
    string = ReadMsgData_ExpandPlaceholders(data->msgFormat, data->msgData, msgId, HEAP_ID_8);
    if (textX == -1) {
        textX = (width * 8 - FontID_String_GetWidth(0, string, 0));
        textX /= 2;
    }
    AddTextPrinterParameterizedWithColor(&window, 0, string, textX, textY, 0xFF, 0xE0200, NULL);
    String_Delete(string);
    CopyWindowToVram(&window);
    RemoveWindow(&window);
}
