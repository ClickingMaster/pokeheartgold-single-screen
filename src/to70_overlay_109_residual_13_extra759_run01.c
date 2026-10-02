#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_window.h"
#include "text.h"
typedef struct {u8 pad[0x21];u8 printer;u8 pad2[0x12];String *strings[11];Window window;} Work;
void ov109_021E73F8(Work *,int,BOOL,u8);
void ov109_021E73F8(Work *w,int n,BOOL frame,u8 speed){if(frame)DrawFrameAndWindow2(&w->window,1,1,13);FillWindowPixelBuffer(&w->window,15);if(speed==0)AddTextPrinterParameterizedWithColor(&w->window,0,w->strings[n],0,0,255,0x1020f,NULL);else w->printer=AddTextPrinterParameterizedWithColor(&w->window,0,w->strings[n],0,0,speed,0x1020f,NULL);ScheduleWindowCopyToVram(&w->window);}
