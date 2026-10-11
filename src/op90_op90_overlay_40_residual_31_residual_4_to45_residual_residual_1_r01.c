#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "string_util.h"
#include "text.h"

typedef struct Ov40WinRect {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} Ov40WinRect;

typedef struct Ov40Sub {
    u8 unk0[0x1B4];
    Window windows[1];
    u8 unk1C4[0x1BC];
    int unk380;
} Ov40Sub;

typedef struct Ov40Unk {
    u8 unk0[0x24];
    BgConfig *bgConfig;
    u8 unk28[0x20];
    MsgData *msgData;
    u8 unk4C[0x814];
    Ov40Sub *sub;
} Ov40Unk;

extern const Ov40WinRect ov40_02245268[];

u32 VsRecorder_GetCenteredTextX(Window *window, String *string);
void ov40_02237C9C(Ov40Unk *data);

#pragma optimization_level 2
// @fn ov40_02237C9C
void ov40_02237C9C(Ov40Unk *data) {
    Ov40Sub *sub;
    int baseTile;
    Window *window;
    const Ov40WinRect *rect;
    String *string;
    u32 textX;
    u32 textY;
    u32 i;

    sub = data->sub;
    rect = ov40_02245268;
    baseTile = 1;
    i = 0;
    window = sub->windows;
    do {
        InitWindow(window);
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_2, rect->x, rect->y, rect->width, rect->height, 14, baseTile);
        FillWindowPixelBuffer(window, 0);
        if (i == 0) {
            string = NewString_ReadMsgData(data->msgData, sub->unk380 + 0x136);
            textX = VsRecorder_GetCenteredTextX(window, string);
            textY = 0;
        }
        if (string != NULL) {
            AddTextPrinterParameterizedWithColor(window, 0, string, textX, textY, 0xFF, 0xF0D00, NULL);
            String_Delete(string);
        }
        ScheduleWindowCopyToVram(window);
        baseTile += rect->width * rect->height;
        window++;
        rect++;
        i++;
    } while (i < 1);
}
#pragma optimization_level reset
