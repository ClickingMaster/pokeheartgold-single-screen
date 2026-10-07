#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
#include "string.h"

void AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
#define MAKE_TEXT_COLOR(fg, sh, bg) ((((fg) & 0xFF) << 16) | (((sh) & 0xFF) << 8) | (((bg) & 0xFF) << 0))
u8 ov80_02237920(u8 a);
void ov82_0223F040(void *unused, Window *window, int fg, int sh, u8 bg, u8 font);

// @fn ov82_0223F040
void ov82_0223F040(void *unused, Window *window, int fg, int sh, u8 bg, u8 font) {
    int row;
    MsgData *msgData;
    int base;
    int y;
    int col;
    int idx;
    String *string;
    int x;

    FillWindowPixelBuffer(window, bg);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x2DF, HEAP_ID_105);
    string = String_New(11, HEAP_ID_105);
    for (row = 0, base = 0, y = 16; row < 5; base += 4, y += 0x24, row++) {
        for (col = 0, x = 1; col < 4; col++, x += 0x40) {
            idx = col + base;
            if (ov80_02237920(idx) != 0xFE) {
                String_SetEmpty(string);
                ReadMsgDataIntoString(msgData, ov80_02237920(idx), string);
                AddTextPrinterParameterizedWithColor(window, font, string, x, y, 0, MAKE_TEXT_COLOR(fg, sh, bg), NULL);
            }
        }
    }
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}
