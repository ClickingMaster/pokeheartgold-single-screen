#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 prefix[0x2c];u32 state;u8 gap30[0xb8c-0x30];s8 selection;u8 gapb8d[0xba0-0xb8d];void*message;u8 gapba4[0xf18-0xba4];u8 window[16];u8 gapf28[0x1088-0xf28];u8 text[16];u8 gap1098[0x11a8-0x1098];void*menu;}Work;
extern u32 ov70_02242144(void*);extern void ov70_0223CD28(int),ClearFrameAndWindow2(void*,int),FillWindowPixelBuffer(void*,int),ov70_0223F2BC(void*,void*,int,int,int,int,int);int ov70_0223C660(Work*);
int ov70_0223C660(Work*w){u32 result=ov70_02242144(w->menu);switch(result){case (u32)-2:ov70_0223CD28(0);ClearFrameAndWindow2(w->window,0);w->state=0;break;case 0:case 1:case 2:ov70_0223CD28(0);w->selection=result+1;w->state=0;FillWindowPixelBuffer(w->text,0);ov70_0223F2BC(w->text,w->message,w->selection,1,0,0,0x10200);break;}return 3;}
