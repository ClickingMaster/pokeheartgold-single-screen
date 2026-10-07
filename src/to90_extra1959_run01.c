#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work { u8 pad0[12];u8 window[16];u8 pad1[0x65c-28];void *messages,*formatter;u8 pad2[0x102c-0x664];u16 value;} Work;
void FillWindowPixelBuffer(void*,u8);void ScheduleWindowCopyToVram(void*);void BufferIntegerAsString(void*,u32,s32,u32,u32,u32);void ov18_021F9648(void*,void*,int,int,int,int,int,int);void ov18_021EE3AC(void*,void*,int,int,int,int,int,int,int);void ov18_021EEED0(Work*,u32);
void ov18_021EEED0(Work*w,u32 page){void *window=w->window;FillWindowPixelBuffer(window,0);switch(page){
case 0:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,13,112,22,0,0x20100,2);break;
case 1:ov18_021F9648(window,w->messages,14,112,6,0,0x20100,2);BufferIntegerAsString(w->formatter,0,w->value,3,2,1);ov18_021EE3AC(w,w->messages,0,15,112,22,0,0x20100,2);break;
case 2:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,16,112,22,0,0x20100,2);break;
case 3:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,17,112,22,0,0x20100,2);break;
case 4:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,18,112,22,0,0x20100,2);break;
case 5:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,20,112,22,0,0x20100,2);break;
case 6:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,19,112,22,0,0x20100,2);break;
case 7:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,22,112,22,0,0x20100,2);break;
case 8:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,21,112,22,0,0x20100,2);break;
case 9:ov18_021F9648(window,w->messages,12,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,25,112,22,0,0x20100,2);break;
case 10:ov18_021F9648(window,w->messages,23,112,6,0,0x20100,2);ov18_021F9648(window,w->messages,24,112,22,0,0x20100,2);break;
}ScheduleWindowCopyToVram(window);}
