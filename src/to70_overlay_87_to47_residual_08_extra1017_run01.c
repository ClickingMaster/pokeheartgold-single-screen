#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x3c];void*format;u8 pad2[0x1c];u8 windows[0x30e];u16 items[1];} Work;
extern u8 ov87_021E8388[];extern void BufferItemName(void*,int,u16);extern int ov87_021E6F98(Work*,void*,int,int,int,int,int,int,int,int);extern void ScheduleWindowCopyToVram(void*);

int ov87_021E71EC(Work*w,int idx);

int ov87_021E71EC(Work*w,int idx){int result;BufferItemName(w->format,0,w->items[idx]);result=ov87_021E6F98(w,w->windows+(idx+2)*16,1,44,ov87_021E8388[idx],0,1,2,0,0);ScheduleWindowCopyToVram(w->windows+(idx+2)*16);return result;}
