#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x160];u8 window[16],window2[16];u8 pad2[0x258-0x180];u16 value;u8 pad3[2];s8 first,second;} State;
extern void FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *),ov86_021E6024(State *,int,int,int,int,int,u32,int),ov86_021E6064(State *,int,int,int,int,int,u32,int),ov86_021E5FBC(State *,int,int);extern int GetWindowWidth(void *);
void ov86_021E7CA0(State *),ov86_021E7CF8(State *);
void ov86_021E7CA0(State *s){int width;FillWindowPixelBuffer(s->window,0);width=GetWindowWidth(s->window);ov86_021E6024(s,21,s->value+56,(width*8)/2,0,4,0xf0100,2);ScheduleWindowCopyToVram(s->window);}
void ov86_021E7CF8(State *s){FillWindowPixelBuffer(s->window2,0);ov86_021E5FBC(s,0,s->first+1);ov86_021E6064(s,22,40,8,0,0,0x10200,1);ov86_021E6024(s,22,41,16,0,0,0x10200,2);ov86_021E5FBC(s,0,s->second+1);ov86_021E6064(s,22,40,32,0,0,0x10200,1);ScheduleWindowCopyToVram(s->window2);}
