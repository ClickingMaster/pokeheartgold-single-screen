#pragma force_active on
#include "global.h"
extern void ov72_022386F4(void *,int);
typedef struct {u8 p[0xe00];void *obj;u8 p2[0x130c - 0xe04];u8 a,b,c,d;u16 enabled;u8 e,pad;s8 g,h,i;u8 p3[0x1364 - 0x1317];u16 state,next;u32 active;} Work;
int ov72_0223A520(Work *w,int a,int b);
int ov72_0223A520(Work *w,int a,int b){w->a=a;w->b=b;w->c=0;w->d=0;if(b!=100){w->enabled=1;w->active=1;}else{w->enabled=0;w->active=0;}w->e=0;w->g=-1;w->h=-1;w->i=-1;w->state=0;w->next=1;if(w->obj)ov72_022386F4(w->obj,0);return 1;}
