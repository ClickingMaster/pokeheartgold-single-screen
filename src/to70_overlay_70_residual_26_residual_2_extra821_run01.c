#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x2c];int state;u8 q[0xb84-0x30];s16 value;s8 choice;u8 r[0xb9c-0xb87];void *a,*b,*c;u8 s[0xf18-0xba8];u8 window[16];u8 t[0x1058-0xf28];u8 other[16];u8 z[0x11a8-0x1068];void *menu;} Work;extern u32 ov70_02242144(void *);extern void ClearFrameAndWindow2(void *,int),ov70_0223F3D8(void *,void *,void *,void *,int,int,int);int ov70_0223EF38(Work *);
int ov70_0223EF38(Work *w){u32 choice=ov70_02242144(w->menu);switch(choice){case -2:ClearFrameAndWindow2(w->window,0);w->state=0;break;case 0:case 1:case 2:w->choice=choice+1;w->state=10;ov70_0223F3D8(w->b,w->c,w->a,w->other,w->value,w->choice,-1);break;}return 3;}
