#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x14e];u8 x0,y0,x1,y1,x2,y2,x3,y3;} Work;
static inline void Window0(int left,int top,int right,int bottom){*(volatile u16*)0x04000040=((left<<8)&0xff00)|right;*(volatile u16*)0x04000044=((top<<8)&0xff00)|bottom;}
static inline void Window1(int left,int top,int right,int bottom){*(volatile u16*)0x04000042=((left<<8)&0xff00)|right;*(volatile u16*)0x04000046=((top<<8)&0xff00)|bottom;}
void ov80_0223A0EC(void *,Work *);
void ov80_0223A0EC(void *task,Work *w){Window0(w->x0,w->y0,w->x1,w->y1);Window1(w->x2,w->y2,w->x3,w->y3);}
