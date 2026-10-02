#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x2c];int flag;u8 p2[0xb70];void *text,*msg;u8 p3[8];void *fmt;u8 p4[0x364];u8 frame[0x10];u8 p5[0x260];u8 window[0x20];void *menu;u8 p6[0x120];void *arg;} Work;
extern int ov70_02242144(void *);
extern void ov70_0223CD28(int),ClearFrameAndWindow2(void *,int),ov70_0223F8A8(Work *,int),FillWindowPixelBuffer(void *,int),ov70_0223F244(void *,void *,void *,void *,int,int,int);
extern int ov70_0224590C[];
int ov70_0223C818(Work *);
int ov70_0223C818(Work *w){int result=ov70_02242144(w->menu);if(result!=-1){if(result==-2||result==ov70_0224590C[0]+1){ov70_0223CD28(0);ClearFrameAndWindow2(w->frame,0);w->flag=0;}else{ov70_0223CD28(0);ov70_0223F8A8(w,result);w->flag=0;FillWindowPixelBuffer(w->window,0);ov70_0223F244(w->window,w->fmt,w->text,w->arg,0,0,0x10200);}}return 3;}
