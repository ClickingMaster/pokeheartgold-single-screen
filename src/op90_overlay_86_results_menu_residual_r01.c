#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct {
    u8 pad0[6];
    u8 unk6;
    u8 pad7[9];
    Window windows[9];
    u8 padA0[0x228 - 0xa0];
    void *frontierSave;
} Ov86Work;
extern void ov86_021E6024(Ov86Work *, u32, u32, u32, u32, u32, u32, u32);
extern void ov86_021E6064(Ov86Work *, u32, u32, u32, u32, u32, u32, u32);
extern u32 ov86_021E6DD8(Ov86Work *, u32);
extern void ov86_021E5FBC(Ov86Work *, int, int);
extern u8 sub_0205C310(u32);
extern u8 sub_0205C350(u32);
extern u16 FrontierSave_GetStat(void *, int, int);
static inline void PrintStat(Ov86Work *w, u32 win, u32 stat) { ov86_021E5FBC(w, 0, FrontierSave_GetStat(w->frontierSave, stat, 0xFF)); ov86_021E6064(w, win, 50, 112, 0, 0, 0x10200, 2); }
void ov86_021E6BA8(Ov86Work *w);
// @fn ov86_021E6BA8
void ov86_021E6BA8(Ov86Work *w) {
    FillWindowPixelBuffer(&w->windows[0], 0);
    FillWindowPixelBuffer(&w->windows[1], 0);
    FillWindowPixelBuffer(&w->windows[4], 0);
    FillWindowPixelBuffer(&w->windows[5], 0);
    FillWindowPixelBuffer(&w->windows[6], 0);
    FillWindowPixelBuffer(&w->windows[7], 0);
    FillWindowPixelBuffer(&w->windows[8], 0);
    ov86_021E6024(w, 0, 21, 0, 0, 0, 0xF0200, 0);
    ov86_021E6024(w, 0, 28, 224, 0, 0, 0xF0200, 1);
    ov86_021E6024(w, 1, 44, 0, 0, 0, 0x10200, 0);
    ov86_021E6024(w, 4, ov86_021E6DD8(w, w->unk6), 0, 0, 0, 0x10200, 0);
    PrintStat(w, 4, sub_0205C310(w->unk6));
    ov86_021E6024(w, 5, 43, 0, 0, 0, 0x10200, 0);
    PrintStat(w, 5, sub_0205C350(w->unk6));
    ov86_021E6024(w, 6, 45, 0, 0, 0, 0x10200, 0);
    ov86_021E6024(w, 7, ov86_021E6DD8(w, 3), 0, 0, 0, 0x10200, 0);
    PrintStat(w, 7, sub_0205C310(3));
    ov86_021E6024(w, 8, 43, 0, 0, 0, 0x10200, 0);
    PrintStat(w, 8, sub_0205C350(3));
    ScheduleWindowCopyToVram(&w->windows[0]);
    ScheduleWindowCopyToVram(&w->windows[1]);
    ScheduleWindowCopyToVram(&w->windows[4]);
    ScheduleWindowCopyToVram(&w->windows[5]);
    ScheduleWindowCopyToVram(&w->windows[6]);
    ScheduleWindowCopyToVram(&w->windows[7]);
    ScheduleWindowCopyToVram(&w->windows[8]);
}
