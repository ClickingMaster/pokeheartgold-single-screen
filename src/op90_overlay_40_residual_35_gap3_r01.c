#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "string_util.h"
#include "text.h"

typedef struct Ov40WinDef {
    int x;
    int y;
    int width;
    int height;
} Ov40WinDef;

typedef struct Ov40WinDefs {
    Ov40WinDef defs[6];
} Ov40WinDefs;

typedef struct Ov40MsgIds {
    u32 ids[6];
} Ov40MsgIds;

typedef struct Ov40Sub {
    u8 unk0[0x614];
    Window windows[6];
    u8 unk674[0x120];
    int numWindows;
} Ov40Sub;

typedef struct Ov40Unk {
    u8 unk0[0x24];
    BgConfig *bgConfig;
    u8 unk28[0x20];
    MsgData *msgData;
    u8 unk4C[0x814];
    Ov40Sub *sub;
} Ov40Unk;

extern const Ov40WinDefs ov40_022458E8;
extern const Ov40MsgIds ov40_022456F0;

u32 VsRecorder_GetCenteredTextX(Window *window, String *string);
void ov40_0223DBD4(Ov40Unk *data, int mode);

// @fn ov40_0223DBD4
void ov40_0223DBD4(Ov40Unk *data, int mode) {
    Ov40Sub *sub = data->sub;
    Ov40WinDefs defs = ov40_022458E8;
    Ov40MsgIds msgIds = ov40_022456F0;
    const Ov40WinDef *def;
    Window *window;
    int i;
    const u32 *msgId;
    String *string;
    int baseTile;
    BOOL center;

    switch (mode) {
    case 0:
        sub->numWindows = 6;
        def = defs.defs;
        msgId = msgIds.ids;
        center = FALSE;
        break;
    case 1:
        break;
    }
    baseTile = 256;
    for (i = 0; i < sub->numWindows; i++) {
        window = &sub->windows[i];
        InitWindow(window);
        AddWindowParameterized(data->bgConfig, window, GF_BG_LYR_MAIN_2, def[i].x, def[i].y, def[i].width, def[i].height, 14, baseTile);
        FillWindowPixelBuffer(window, 0);
        string = NewString_ReadMsgData(data->msgData, *msgId);
        baseTile += def[i].width * def[i].height;
        if (center) {
            VsRecorder_GetCenteredTextX(window, string);
        }
        AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, 0xFF, 0xF0D00, NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(string);
        msgId++;
    }
}
