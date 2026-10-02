#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x2c];int state;u8 q[0xb8a-0x30];s16 value;u8 r[0xba0-0xb8c];void *msg;u8 s[0x10a8-0xba4];u8 window[16];u8 t[0x11a8-0x10b8];void *menu;} Work;extern u32 ov70_02242144(void *);extern void ov70_0223CD28(int),ov70_0223F828(s16 *,int,int),FillWindowPixelBuffer(void *,int),ov70_0223F370(void *,void *,int,int,int,int,int);int ov70_0223C744(Work *);
int ov70_0223C744(Work *w){u32 choice=ov70_02242144(w->menu);switch(choice){case 11:case -2:ov70_0223CD28(0);w->state=0;break;case -1:break;default:ov70_0223CD28(0);ov70_0223F828(&w->value,choice,1);w->state=0;FillWindowPixelBuffer(w->window,0);ov70_0223F370(w->window,w->msg,choice,0,0,0x10200,1);break;}return 3;}
