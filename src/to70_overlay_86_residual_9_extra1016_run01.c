#pragma force_active on
#include "global.h"
typedef struct{u8 p[384];u8 window[16];} Work;
extern void DrawFrameAndWindow2(void *,int,int,int);extern void FillWindowPixelBuffer(void *,int);extern void ov86_021E6024(Work *,int,int,int,int,int,u32,void *);extern void ScheduleWindowCopyToVram(void *);
void ov86_021E752C(Work *);
void ov86_021E752C(Work *w){DrawFrameAndWindow2(w->window,1,994,14);FillWindowPixelBuffer(w->window,15);ov86_021E6024(w,23,42,0,0,1,0x1020f,0);ScheduleWindowCopyToVram(w->window);}
