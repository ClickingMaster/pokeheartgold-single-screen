#pragma force_active on
#include "global.h"
typedef struct {u32 words[2];} Template;
typedef struct {u32 x;void *bg;u8 windows[17][16];void *msg,*other,*other2,*format,*string;} Work;
extern Template ov64_021E7040[];extern void FontID_Alloc(int,int);extern void AddWindow(void*,void*,Template*);extern void FillWindowPixelBuffer(void*,int);extern void ReadMsgDataIntoString(void*,int,void*);extern u32 FontID_String_GetWidth(int,void*,int);extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*);extern void CopyWindowPixelsToVram_TextMode(void*);extern void ScheduleWindowCopyToVram(void*);void ov64_021E6170(Work*);
void ov64_021E6170(Work*w){u32 i,width;FontID_Alloc(4,59);for(i=0;i<17;i++)AddWindow(w->bg,w->windows[i],&ov64_021E7040[i]);FillWindowPixelBuffer(w->windows[16],0);ReadMsgDataIntoString(w->msg,6,w->string);width=FontID_String_GetWidth(4,w->string,0);AddTextPrinterParameterizedWithColor(w->windows[16],4,w->string,24-width/2,0,255,0xf0100,0);CopyWindowPixelsToVram_TextMode(w->windows[16]);ScheduleWindowCopyToVram(w->windows[16]);}
