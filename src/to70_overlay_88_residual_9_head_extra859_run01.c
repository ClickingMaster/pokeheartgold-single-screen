#pragma force_active on
#include "global.h"
typedef struct {u16 pad;volatile u16 current;u16 max,size;u8 pad2[0x10];void *pixels;} W;
extern void FillWindowPixelRect(void *,int,int,int,int,int),BlitBitmapRect(void *,void *,u16,int,int,int,u16,int,u16,int,int),ScheduleWindowCopyToVram(void *);
void ov88_022595F8(W *,void *);
void ov88_022595F8(W *w,void *window){volatile int size=w->size;int width=w->current*size/w->max;int x,y;if(width<256){x=255-width;y=0;}else{y=width-256;x=0;width=size-width;if(width>255)width=255;}FillWindowPixelRect(window,0,0,0,255,16);BlitBitmapRect(window,w->pixels,(u16)y,0,1440,16,(u16)x,0,(u16)width,16,15);ScheduleWindowCopyToVram(window);}
