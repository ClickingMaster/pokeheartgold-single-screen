#pragma force_active on
#include "global.h"
#include "pokemon.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "heap.h"
typedef struct {
    u8 pad0[0xc];
    enum HeapID heapId;
} BattleCtx;
typedef struct {
    Pokemon *mon;
    u8 pad4[0x16 - 0x4];
    u8 unk16_0 : 7;
    u8 isEgg : 1;
    u8 gender : 3;
    u8 unk17_3 : 4;
    u8 hide : 1;
    u8 pad18[0x50 - 0x18];
} MonEntry;
typedef struct {
    BattleCtx *ctx;
    MonEntry mons[6];
    u8 pad1E4[0x1FA8 - 4 - 6 * 0x50];
    MsgData *msgData;
    MessageFormat *msgFormat;
    u8 pad1FB0[0x2070 - 0x1FB0];
    Window *windows;
} Work;
extern const u32 ov08_02224FF4[];
BoxPokemon *Mon_GetBoxMon(Pokemon *);
u8 AddTextPrinterParameterizedWithColor(Window *, int, String *, int, int, int, u32, void *);
int FontID_String_GetWidth(int, String *, int);
void ov08_0221DDCC(Work *w, int winIdx, int fontId, u8 slot, u8 x, u8 y);
// @fn ov08_0221DDCC
void ov08_0221DDCC(Work *w, int winIdx, int fontId, u8 slot, u8 x, u8 y) {
    MonEntry *entry;
    String *str;
    String *tmpl;
    Window *windows;
    Window *win;
    windows = w->windows;
    win = &windows[winIdx];
    entry = &w->mons[slot];
    str = String_New(12, w->ctx->heapId);
    tmpl = NewString_ReadMsgData(w->msgData, ov08_02224FF4[slot]);
    BufferBoxMonNickname(w->msgFormat, 0, Mon_GetBoxMon(entry->mon));
    StringExpandPlaceholders(w->msgFormat, str, tmpl);
    if (fontId == 0) {
        AddTextPrinterParameterizedWithColor(win, fontId, str, x, y, 0xFF, 0xF0E00, NULL);
    } else {
        AddTextPrinterParameterizedWithColor(win, fontId, str, x, y, 0xFF, 0x70809, NULL);
    }
    String_Delete(tmpl);
    String_Delete(str);
    if (entry->isEgg == 0 && entry->hide == 0) {
        if (entry->gender == 0) {
            String *s = NewString_ReadMsgData(w->msgData, 16);
            u32 width = GetWindowWidth(win);
            int x2 = width * 8 - FontID_String_GetWidth(0, s, 0);
            if (fontId == 0) {
                AddTextPrinterParameterizedWithColor(win, 0, s, x2, y, 0xFF, 0x70800, NULL);
            } else {
                AddTextPrinterParameterizedWithColor(win, 0, s, x2, y, 0xFF, 0xA0B00, NULL);
            }
            String_Delete(s);
        } else if (entry->gender == 1) {
            String *s = NewString_ReadMsgData(w->msgData, 17);
            u32 width = GetWindowWidth(win);
            int x2 = width * 8 - FontID_String_GetWidth(0, s, 0);
            if (fontId == 0) {
                AddTextPrinterParameterizedWithColor(win, 0, s, x2, y, 0xFF, 0x30400, NULL);
            } else {
                AddTextPrinterParameterizedWithColor(win, 0, s, x2, y, 0xFF, 0xC0D00, NULL);
            }
            String_Delete(s);
        }
    }
    ScheduleWindowCopyToVram(win);
}
