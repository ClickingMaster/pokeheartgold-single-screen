#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[4];} Window;typedef struct {u8 pad[12];Window windows[101];void *messages;} Work;
extern void FillWindowPixelBuffer(Window *,int),ScheduleWindowCopyToVram(Window *);extern int GetWindowWidth(Window *),ov18_021F09D8(Work *,int);extern void ov18_021EE3AC(Work *,void *,int,int,int,int,int,u32,int);
void ov18_021F0F68(Work *,int,int);
void ov18_021F0F68(Work *w,int index,int value){int message,width;FillWindowPixelBuffer(&w->windows[index],0);message=ov18_021F09D8(w,value);width=GetWindowWidth(&w->windows[index]);ov18_021EE3AC(w,w->messages,index,message,(width*8)/2,0,0,0x50900,2);ScheduleWindowCopyToVram(&w->windows[index]);}
