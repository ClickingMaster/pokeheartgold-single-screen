#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "font.h"
#include "text.h"
#include "msgdata.h"
#include "message_format.h"
#include "unk_02097268.h"
typedef struct {
    u8 id;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Entry;
typedef struct {
    u8 name;
    u8 count;
    Entry entries[30];
} Record;
typedef struct State {
    u32 heap;
    u8 pad4[0x1c - 4];
    Record records[6];
    u8 pad2f8[0x304 - 0x2f8];
    MsgData *msgData;
    MessageFormat *fmt;
    String *str;
    u8 pad310[0x338 - 0x310];
    String *template;
    u8 pad33c[0x3b4 - 0x33c];
    Window windows[15];
    u8 pad4a4[0x184E3 - 0x4a4];
    u8 enabled;
} State;
extern const u32 ov108_021EA724[2];
void ov108_021E7CD8(State *s, u32 index);
// @fn ov108_021E7CD8
void ov108_021E7CD8(State *s, u32 index) {
    int i;
    Record *r;
    u8 x;
    u8 counts[5];
    SafariObjectConfig cfg;
    if (index >= 6) {
        if (s->enabled) {
            for (i = 0; i < 5; i++) {
                FillWindowPixelBuffer(&s->windows[i * 2 + 6], ov108_021EA724[i % 2]);
                ScheduleWindowCopyToVram(&s->windows[i * 2 + 6]);
            }
        }
        FillWindowPixelBuffer(&s->windows[4], 12);
        ScheduleWindowCopyToVram(&s->windows[4]);
        return;
    }
    r = &s->records[index];
    {
        Window *win = &s->windows[4];
        ReadMsgDataIntoString(s->msgData, r->name + 16, s->str);
        x = (GetWindowWidth(win) * 8 - FontID_String_GetWidth(0, s->str, 0)) / 2;
        FillWindowPixelBuffer(win, 12);
        AddTextPrinterParameterizedWithColor(win, 0, s->str, x, 4, 255, 0x80b0c, NULL);
        ScheduleWindowCopyToVram(win);
    }
    if (s->enabled) {
        MI_CpuFill8(counts, 0, 5);
        for (i = 0; i < r->count; i++) {
            GetSafariObjectConfig(&cfg, r->entries[i].id, 2);
            if (cfg.objectType == 0) {
                counts[4]++;
            } else {
                counts[cfg.objectType - 1]++;
            }
        }
        for (i = 0; i < 5; i++) {
            Window *win = &s->windows[i * 2 + 6];
            BufferIntegerAsString(s->fmt, 0, counts[i], 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            StringExpandPlaceholders(s->fmt, s->str, s->template);
            FillWindowPixelBuffer(win, ov108_021EA724[i % 2]);
            AddTextPrinterParameterizedWithColor(win, 0, s->str, 0, 0, 255, ov108_021EA724[i % 2], NULL);
            ScheduleWindowCopyToVram(win);
        }
    }
}
