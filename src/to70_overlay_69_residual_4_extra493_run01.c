#pragma force_active on
#include "global.h"
#include "system.h"
extern void FillWindowPixelRect(void*,int,int,int,int,int),*String_New(int,int),ReadMsgDataIntoString(void*,int,void*),String_Delete(void*);
extern int Options_GetTextFrameDelay(void*),AddTextPrinterParameterized(void*,int,void*,int,int,int,void*),TextPrinterCheckActive(u8);
typedef struct{int heap;int a;void*options;u8 p[0xc008];u8 window[16];u8 q[0x4c];void*msg;int state,printer;void*text;}Work;
int ov69_021E6810(Work*,int,int);
int ov69_021E6810(Work*w,int id,int autoClose){int result=0;switch(w->state){case 0:FillWindowPixelRect(w->window,15,0,0,216,32);w->text=String_New(1024,w->heap);ReadMsgDataIntoString(w->msg,id,w->text);w->printer=AddTextPrinterParameterized(w->window,1,w->text,0,0,Options_GetTextFrameDelay(w->options),0);w->state=1;break;case 1:if(!TextPrinterCheckActive((u8)w->printer)){String_Delete(w->text);w->state=2;}break;case 2:if(autoClose||(gSystem.newKeys&1)){w->state=0;result=1;}break;}return result;}
