#include "global.h"

#include "bg_window.h"

extern const BgTemplate ov112_021FF134;
extern const BgTemplate ov112_021FF150;
void ov112_021F0668(BgConfig *bgConfig);

void ov112_021F0668(BgConfig *bgConfig) {
    BgTemplate template5 = ov112_021FF134;
    InitBgFromTemplate(bgConfig, 5, &template5, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 5);

    BgTemplate template7 = ov112_021FF150;
    InitBgFromTemplate(bgConfig, 7, &template7, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 7);
}

#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

typedef struct Ov112DisplayWork {
    enum HeapID heapId;
    u32 unk04;
    Window windows[6];
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
} Ov112DisplayWork;

typedef struct Ov112DisplayData {
    u8 unk00[4];
    u16 species;
    u8 unk06[2];
    u8 status;
    u8 unk09;
    u16 name[14];
    u16 item;
    u8 count;
} Ov112DisplayData;

void ov112_021F06CC(Ov112DisplayWork *work, Ov112DisplayData *data);

void ov112_021F06CC(Ov112DisplayWork *work, Ov112DisplayData *data) {
    String *string;
    u32 count = data->count;
    u16 item = data->item;
    u32 width;
    u32 x;

    CopyU16ArrayToString(work->string, data->name);
    FillWindowPixelBuffer(&work->windows[3], 0);
    AddTextPrinterParameterizedWithColor(&work->windows[3], 0, work->string, 0, 0, 0xFF, 0x10200, NULL);
    CopyWindowToVram(&work->windows[3]);

    string = GetSpeciesName(data->species, work->heapId);
    BufferString(work->msgFormat, 0, string, 2, 1, 2);
    String_Delete(string);
    string = ReadMsgData_ExpandPlaceholders(work->msgFormat, work->msgData, 0, work->heapId);
    FillWindowPixelBuffer(&work->windows[0], 0);
    AddTextPrinterParameterizedWithColor(&work->windows[0], 0, string, 0, 0, 0xFF, 0x10200, NULL);
    CopyWindowToVram(&work->windows[0]);
    String_Delete(string);

    BufferIntegerAsString(work->msgFormat, 0, count, 3, (PrintingMode)1, 1);
    string = ReadMsgData_ExpandPlaceholders(work->msgFormat, work->msgData, 3, work->heapId);
    FillWindowPixelBuffer(&work->windows[1], 0);
    width = GetWindowWidth(&work->windows[1]);
    x = FontID_String_GetWidth(0, string, 0);
    x = width * 8 - x;
    AddTextPrinterParameterizedWithColor(&work->windows[1], 0, string, x, 0, 0xFF, 0x10200, NULL);
    CopyWindowToVram(&work->windows[1]);
    String_Delete(string);

    if (data->status == 0) {
        string = NewString_ReadMsgData(work->msgData, 1);
        FillWindowPixelBuffer(&work->windows[4], 0);
        AddTextPrinterParameterizedWithColor(&work->windows[4], 0, string, 0, 0, 0xFF, 0x70800, NULL);
        CopyWindowToVram(&work->windows[4]);
        String_Delete(string);
    } else if (data->status == 1) {
        string = NewString_ReadMsgData(work->msgData, 2);
        FillWindowPixelBuffer(&work->windows[4], 0);
        AddTextPrinterParameterizedWithColor(&work->windows[4], 0, string, 0, 0, 0xFF, 0x30400, NULL);
        CopyWindowToVram(&work->windows[4]);
        String_Delete(string);
    } else {
        FillWindowPixelBuffer(&work->windows[4], 0);
        CopyWindowToVram(&work->windows[4]);
    }

    string = NewString_ReadMsgData(work->msgData, 4);
    FillWindowPixelBuffer(&work->windows[2], 0);
    AddTextPrinterParameterizedWithColor(&work->windows[2], 0, string, 0, 0, 0xFF, 0x10200, NULL);
    CopyWindowToVram(&work->windows[2]);
    String_Delete(string);

    BufferItemName(work->msgFormat, 0, item);
    string = ReadMsgData_ExpandPlaceholders(work->msgFormat, work->msgData, 5, work->heapId);
    FillWindowPixelBuffer(&work->windows[5], 0);
    AddTextPrinterParameterizedWithColor(&work->windows[5], 0, string, 0, 0, 0xFF, 0x10200, NULL);
    CopyWindowToVram(&work->windows[5]);
    String_Delete(string);
}
