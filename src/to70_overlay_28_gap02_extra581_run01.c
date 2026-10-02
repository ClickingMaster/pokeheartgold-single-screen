#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x10];void *bg;u8 p2[0x18c];void *text[5];u8 windows[5][16];u8 p4[0x130];int selected;} Work;
extern u8 ov28_0225EB52[][8],ov28_0225EB7C[];
extern void FontID_Alloc(int,int),AddWindow(void *,void *,void *),FillWindowPixelBuffer(void *,int),AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,void *),CopyWindowPixelsToVram_TextMode(void *),ScheduleWindowCopyToVram(void *);
void ov28_0225D7E0(Work *);
void ov28_0225D7E0(Work *w){u32 i;for(FontID_Alloc(4,8),i=0;i<5;i++){AddWindow(w->bg,w->windows[i],ov28_0225EB52[i]);FillWindowPixelBuffer(w->windows[i],0);AddTextPrinterParameterizedWithColor(w->windows[i],4,w->text[i],0,0,255,0xf0100,0);CopyWindowPixelsToVram_TextMode(w->windows[i]);}ScheduleWindowCopyToVram(w->windows[ov28_0225EB7C[w->selected]]);ScheduleWindowCopyToVram(w->windows[4]);}
