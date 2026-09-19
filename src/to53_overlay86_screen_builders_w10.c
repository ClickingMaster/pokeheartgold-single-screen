#include "global.h"

#include "bg_window.h"

typedef struct Work {
    u8 data[0x230];
} Work;

#define PTR(w, o) (*(void **)((w)->data + (o)))

extern void ov86_021E6024(Work *, u32, u32, u32, u32, u32, u32, u32);
extern void ov86_021E6064(Work *, u32, u32, u32, u32, u32, u32, u32);
extern u32 ov86_021E6484(Work *, u32);
extern u32 sub_0205BFF0(u32, u32);
extern u32 sub_0205C048(u32, u32);
extern u32 sub_0205C01C(u32, u32);
extern u32 sub_0205C074(u32, u32);
extern u32 FrontierSave_GetStat(void *, u32, u32);
extern void ov86_021E5FBC(Work *, u32, u32);
void ov86_021E6114(Work *);
void ov86_021E62F4(Work *);

void ov86_021E6114(Work *w) {
    u32 stat;

    FillWindowPixelBuffer((Window *)(w->data + 0x10), 0);
    FillWindowPixelBuffer((Window *)(w->data + 0x20), 0);
    FillWindowPixelBuffer((Window *)(w->data + 0x30), 0);
    FillWindowPixelBuffer((Window *)(w->data + 0x40), 0);
    ov86_021E6024(w, 0, 0x16, 0, 0, 0, 0xF0200, 0);
    if (w->data[6] == 0) {
        stat = 0x1A;
    } else if (w->data[6] == 1) {
        stat = 0x1B;
    } else {
        stat = 0x1C;
    }
    ov86_021E6024(w, 0, stat, 0xE0, 0, 0, 0xF0200, 1);
    ov86_021E6024(w, 1, 0x2E, 0, 0, 0, 0x10200, 0);
    ov86_021E6024(w, 1, 0x30, 0xE0, 0, 0, 0x10200, 1);
    ov86_021E6024(w, 2, ov86_021E6484(w, 0), 0, 0, 0, 0x10200, 0);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205BFF0(0, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 2, 0x32, 0x70, 0, 0, 0x10200, 2);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205C048(0, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 2, 0x34, 0xE0, 0, 0, 0x10200, 1);
    ov86_021E6024(w, 3, 0x2B, 0, 0, 0, 0x10200, 0);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205C01C(0, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 3, 0x32, 0x70, 0, 0, 0x10200, 2);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205C074(0, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 3, 0x34, 0xE0, 0, 0, 0x10200, 1);
    ScheduleWindowCopyToVram((Window *)(w->data + 0x10));
    ScheduleWindowCopyToVram((Window *)(w->data + 0x20));
    ScheduleWindowCopyToVram((Window *)(w->data + 0x30));
    ScheduleWindowCopyToVram((Window *)(w->data + 0x40));
}

void ov86_021E62F4(Work *w) {
    u32 stat;

    FillWindowPixelBuffer((Window *)(w->data + 0x50), 0);
    FillWindowPixelBuffer((Window *)(w->data + 0x60), 0);
    FillWindowPixelBuffer((Window *)(w->data + 0x70), 0);
    ov86_021E6024(w, 4, 0x2F, 0, 0, 0, 0x10200, 0);
    ov86_021E6024(w, 4, 0x30, 0xE0, 0, 0, 0x10200, 1);
    ov86_021E6024(w, 5, ov86_021E6484(w, 1), 0, 0, 0, 0x10200, 0);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205BFF0(1, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 5, 0x32, 0x70, 0, 0, 0x10200, 2);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205C048(1, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 5, 0x34, 0xE0, 0, 0, 0x10200, 1);
    ov86_021E6024(w, 6, 0x2B, 0, 0, 0, 0x10200, 0);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205C01C(1, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 6, 0x32, 0x70, 0, 0, 0x10200, 2);
    stat = FrontierSave_GetStat(PTR(w, 0x228), sub_0205C074(1, w->data[6]), 0xFF);
    ov86_021E5FBC(w, 0, stat);
    ov86_021E6064(w, 6, 0x34, 0xE0, 0, 0, 0x10200, 1);
    ScheduleWindowCopyToVram((Window *)(w->data + 0x50));
    ScheduleWindowCopyToVram((Window *)(w->data + 0x60));
    ScheduleWindowCopyToVram((Window *)(w->data + 0x70));
}
