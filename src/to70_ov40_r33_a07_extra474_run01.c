#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x94];u8 a[16],b[16];} Panel;
typedef struct {u8 p[0x24];void *bg;u8 q[0x20];void *msg;u8 r[0x814];Panel *panel;} Work;
extern void InitWindow(void *);extern void AddWindowParameterized(void *,void *,int,int,int,int,int,int,int);extern void FillWindowPixelBuffer(void *,int);extern void *NewString_ReadMsgData(void *,int);extern int ov40_022306C0(void *,void *);extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,int);extern void String_Delete(void *);extern void ScheduleWindowCopyToVram(void *);void ov40_0223CD58(Work *);
void ov40_0223CD58(Work *w){Panel *p=w->panel;void *a=p->a,*s;InitWindow(a);AddWindowParameterized(w->bg,a,6,4,6,10,2,14,1);FillWindowPixelBuffer(a,0);s=NewString_ReadMsgData(w->msg,130);AddTextPrinterParameterizedWithColor(a,0,s,ov40_022306C0(a,s),0,255,0xf0d00,0);String_Delete(s);ScheduleWindowCopyToVram(a);a=p->b;InitWindow(a);AddWindowParameterized(w->bg,a,6,18,6,10,2,14,21);FillWindowPixelBuffer(a,0);s=NewString_ReadMsgData(w->msg,131);AddTextPrinterParameterizedWithColor(a,0,s,ov40_022306C0(a,s),0,255,0xf0d00,0);String_Delete(s);ScheduleWindowCopyToVram(a);}
