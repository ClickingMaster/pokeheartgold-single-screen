#ifndef TO52_OV96_RESULTS_GRAPHICS_R43_R3_H
#define TO52_OV96_RESULTS_GRAPHICS_R43_R3_H
#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "msgdata.h"
#include "string_util.h"
u8 AddTextPrinterParameterizedWithColor(Window *, u32, String *, u32, u32, u32, u32, void *);
typedef struct Ov96R43Work {
    enum HeapID heapId;
    BgConfig *bgConfig;
    u32 kind;
    NARC *narc;
    Window windowA;
    Window windowB;
} Ov96R43Work;
void ov96_021EE644(Ov96R43Work *);
void ov96_021EE6A0(Ov96R43Work *);
void ov96_021EE700(Window *, MsgData *, String *, u32, u32);
void ScheduleWindowCopyToVram(Window *);
#endif
