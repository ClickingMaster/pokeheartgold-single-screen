#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "font.h"
#include "item.h"
#include "message_format.h"
#include "msgdata.h"
void AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);

typedef struct Group {
    ItemSlot entries[36];
} Group;

typedef struct Work {
    void *root;
    u8 pad[0xC];
    MsgData *msgData;
    MessageFormat *msgFmt;
    String *strbuf;
    u8 pad1[0x2C - 0x1C];
    Window *windows;
    u8 pad2[0x3C - 0x30];
    Group groups[30];
    u8 pad3[0x114D - 0x3C - 30 * 0x90];
    u8 sel;
} Work;

typedef struct MsgEntry {
    int msgId;
    int unused;
} MsgEntry;

extern const MsgEntry ov08_02225BE0[];
void ov08_022235D4(Work *work, int slot, int msg, int win, int font, u32 color);

// @fn ov08_022235D4
void ov08_022235D4(Work *work, int slot, int msg, int win, int font, u32 color) {
    Window *windows = work->windows;
    String *str;
    u32 width;
    u32 x;

    FillWindowPixelBuffer(&windows[win], 0);
    if (work->groups[work->sel].entries[slot].id != 0) {
        str = NewString_ReadMsgData(work->msgData, ov08_02225BE0[msg].msgId);
        BufferItemName(work->msgFmt, 0, work->groups[work->sel].entries[slot].id);
        StringExpandPlaceholders(work->msgFmt, work->strbuf, str);
        width = FontID_String_GetWidth(font, work->strbuf, 0);
        x = (GetWindowWidth(&windows[win]) * 8 - width) / 2;
        AddTextPrinterParameterizedWithColor(&windows[win], font, work->strbuf, x, 7, 0xFF, color, NULL);
        String_Delete(str);
    }
    ScheduleWindowCopyToVram(&windows[win]);
}
