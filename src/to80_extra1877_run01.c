#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct{int a;u16 name[224];u8 window[16];} Name;
extern void *String_New(int,int);
extern void CopyU16ArrayToString(void *,u16 *),FillWindowPixelBuffer(void *,int),AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,void *),ScheduleWindowCopyToVram(void *),String_Delete(void *);
extern int VsRecorder_GetCenteredTextX(void *,void *);
void ov40_02237144(void *);
void ov40_02237144(void *arg){Name *w=*(Name**)((u8*)arg+0x860);void*window=w->window;void *s=String_New(255,109);CopyU16ArrayToString(s,w->name);FillWindowPixelBuffer(window,0);AddTextPrinterParameterizedWithColor(window,0,s,VsRecorder_GetCenteredTextX(window,s),4,255,0xf0100,0);ScheduleWindowCopyToVram(window);String_Delete(s);}
