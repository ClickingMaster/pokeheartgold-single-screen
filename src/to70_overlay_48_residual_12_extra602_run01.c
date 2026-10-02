#pragma force_active on
#include "global.h"
extern void *memset(void *,int,u32);
extern void AddWindowParameterized(void *,void *,int,int,int,int,int,int,int);
extern void FillWindowPixelBuffer(void *,int);
extern void *ov48_0225B0C4(void *,int);
extern void FontID_Alloc(int,int);
extern int FontID_String_GetCenterAlignmentX(int,void *,int,int);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,int);
extern void FontID_Release(int);
extern void DrawFrameAndWindow1(void *,int,int,int);
typedef struct {u8 p[28];u8 window[16];} Work;
void ov48_02259D00(Work *,void **,void *,int);
void ov48_02259D00(Work *w,void **config,void *messages,int heap){void *s;int x;memset(w,0,44);AddWindowParameterized(*config,w->window,1,25,21,6,2,1,40);FillWindowPixelBuffer(w->window,15);s=ov48_0225B0C4(messages,1);FontID_Alloc(2,heap);x=FontID_String_GetCenterAlignmentX(2,s,0,48);AddTextPrinterParameterizedWithColor(w->window,2,s,x,0,255,0x2010f,0);FontID_Release(2);DrawFrameAndWindow1(w->window,0,31,0);}
