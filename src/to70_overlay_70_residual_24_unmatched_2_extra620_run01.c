#pragma force_active on
#include "global.h"
typedef struct {u32 a;void *bg;u8 p[0xf10];u8 main[16];u8 q[0x130];u8 windows[6][16];} W;typedef struct {u16 x,y;} Pair;extern Pair ov70_02245920[6];
extern void AddWindowParameterized(void*,void*,int,u8,u8,u8,u8,u8,u16);extern void FillWindowPixelBuffer(void*,int);extern void CopyWindowToVram(void*);extern void Gts_CreatePanelWindow(W*,int);
void ov70_0223EB34(W*);
void ov70_0223EB34(W*w) {int i,tile;Pair*p;u8*win;AddWindowParameterized(w->bg,w->main,0,2,21,27,2,13,96);FillWindowPixelBuffer(w->main,0);p=ov70_02245920;i=0;tile=150;win=w->windows[0];for(;i<6;i++,tile+=22,p++,win+=16){AddWindowParameterized(w->bg,win,3,(u8)p->x,(u8)p->y,11,2,13,(u16)tile);FillWindowPixelBuffer(win,0);CopyWindowToVram(win);}Gts_CreatePanelWindow(w,3);}
