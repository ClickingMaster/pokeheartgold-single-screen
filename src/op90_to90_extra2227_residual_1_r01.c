#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "message_printer.h"
#include "party.h"
#include "pokemon.h"
typedef struct {
    u8 pad0[9];
    u8 unk9;
    u8 padA[0x2b4 - 0xa];
    MessagePrinter *printer;
    u8 pad2B8[0x54c - 0x2b8];
    u8 *present;
    u8 pad550[0x55c - 0x550];
    Party *party;
} Work;
extern BOOL ov80_02237D8C(u8);
extern u8 ov80_02237B58(u8, int);
void ov83_022448E4(Work *w, Window *win);
// @fn ov83_022448E4
void ov83_022448E4(Work *w, Window *win) {
    int x;
    int i;
    u8 n;
    Pokemon *mon;
    FillWindowPixelBuffer(win, 0);
    if (ov80_02237D8C(w->unk9) == FALSE) {
        x = 36;
    } else {
        x = 4;
    }
    n = ov80_02237B58(w->unk9, 1);
    for (i = 0; i < n; i++) {
        if (w->present[i] == 0) {
            FillWindowPixelRect(win, 0, x + i * 64, 1, 64, 16);
        } else {
            mon = Party_GetMonByIndex(w->party, i);
            PrintUIntOnWindow(w->printer, GetMonData(mon, MON_DATA_HP, NULL), 3, (PrintingMode)1, win, x + i * 64, 1);
            sub_0200CDAC(w->printer, 0, win, x + 24 + i * 64, 1);
            PrintUIntOnWindow(w->printer, GetMonData(mon, MON_DATA_MAX_HP, NULL), 3, (PrintingMode)0, win, x + 32 + i * 64, 1);
        }
    }
    ScheduleWindowCopyToVram(win);
}
