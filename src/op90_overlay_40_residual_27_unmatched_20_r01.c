#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "string_util.h"
#include "text.h"
#include "touchscreen.h"

typedef struct Ov40Sub {
    u8 unk0[0xD0];
    Window windows[9];
} Ov40Sub;

typedef struct Ov40Unk {
    u8 unk0[0x24];
    BgConfig *bgConfig;
    u8 unk28[0x20];
    MsgData *msgData;
    u8 unk4C[0x814];
    Ov40Sub *sub;
} Ov40Unk;

extern const TouchscreenHitbox ov40_02245708[];

u32 VsRecorder_GetCenteredTextX(Window *window, String *string);
int ov40_02235DAC(Ov40Unk *data, u32 index);
void ov40_02235FFC(Ov40Unk *data);

// @fn ov40_02235FFC
void ov40_02235FFC(Ov40Unk *data) {
    u32 textX;
    String *string;
    int baseTile;
    u8 x, y, width;
    int textY;
    u8 height;
    u32 i;
    Window *window;
    const TouchscreenHitbox *hitbox;

    baseTile = 1;
    hitbox = ov40_02245708;
    for (i = 0, window = data->sub->windows; i < 9; i++) {
        x = hitbox->rect.left >> 3;
        y = hitbox->rect.top >> 3;
        width = ((u32)hitbox->rect.right >> 3) - x;
        height = ((u32)hitbox->rect.bottom >> 3) - y;
        InitWindow(window);
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_SUB_2, x, y, width, height, 14, baseTile);
        FillWindowPixelBuffer(window, 0);
        string = NewString_ReadMsgData(data->msgData, i + 0x44);
        textX = VsRecorder_GetCenteredTextX(window, string);
        textY = (height * 8 - 16) / 2;
        if (ov40_02235DAC(data, i) == 1) {
            AddTextPrinterParameterizedWithColor(window, 0, string, textX, textY, 0xFF, 0xF0D00, NULL);
        } else {
            AddTextPrinterParameterizedWithColor(window, 0, string, textX, textY, 0xFF, 0xC0B00, NULL);
        }
        ScheduleWindowCopyToVram(window);
        String_Delete(string);
        baseTile += width * height;
        window++;
        hitbox++;
    }
}
