#pragma force_active on
#include "global.h"
typedef struct{u8 data[8];} WindowTemplate;
extern WindowTemplate ov59_0223C798[];extern void AddWindow(void *,void *,WindowTemplate *);extern void FillWindowPixelBuffer(void *,int);extern void AddWindowParameterized(void *,void *,int,u8,u8,u8,u8,int,u16);extern void *YesNoPrompt_Create(int);
typedef struct{int heap;u8 p4[80];void *bg;u8 p58[0xd0];u8 windows[18][16];void *prompt;} Work;
void ov59_022397E4(Work *);
void ov59_022397E4(Work *w){int i=0,tile,y;WindowTemplate *t=ov59_0223C798;u8 *win=w->windows[0];for(;i<11;i++){AddWindow(w->bg,win,t);FillWindowPixelBuffer(win,0);t++;win+=16;}tile=444;i=0;y=6;for(;i<7;i++){AddWindowParameterized(w->bg,w->windows[i+11],2,(u8)((i%2)*6+5),(u8)y,2,2,12,(u16)tile);FillWindowPixelBuffer(w->windows[i+11],0);tile+=4;y+=2;}w->prompt=YesNoPrompt_Create(w->heap);}
