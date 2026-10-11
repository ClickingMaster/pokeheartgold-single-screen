#include "text_render_helpers_private.h"

void sub_02010F34(int mode,u32 a,int x) {
 if(mode==0){ScreenWindow_QueueSetVisible(a,0);return;}ScreenWindow_QueueSetVisible(a,1);ScreenWindow_QueueSetInside(a,0x3f,0,0,x);ScreenWindow_QueueSetRect(a,0,0,0,0,0,x);ScreenWindow_QueueSetOutside(a,0x20,0,x);
}

void sub_02010F84(u32 a0,u32 a1,u32 a2,u32 a3,int a4,int a5,int a6,int a7,int a8,int a9) {
 if(a9==0){ScreenWindow_SetInside(a1,0,a3,a4);ScreenWindow_SetOutside(a2,0,a4);ScreenWindow_SetRect(a5,a6,a7,a8,a3,a4);return;}
 ScreenWindow_QueueSetInside(a0,a1,0,a3,a4);ScreenWindow_QueueSetOutside(a0,a2,0,a4);ScreenWindow_QueueSetRect(a0,a5,a6,a7,a8,a3,a4);
}

void sub_02010FEC(u32 a,int b,int c,int mode) {
 volatile u8 x[4];x[1]=ScreenWindow_GetInside(b,c);x[3]=x[1];x[0]=ScreenWindow_GetOutside(c);x[2]=x[0];
 if(mode==0){ScreenWindow_SetInside((u32)(x[2] << 27) >> 27,0,b,c);ScreenWindow_SetOutside((u32)(x[3] << 27) >> 27,0,c);return;}
 ScreenWindow_QueueSetInside(a,(u32)(x[2] << 27) >> 27,0,b,c);ScreenWindow_QueueSetOutside(a,(u32)(x[3] << 27) >> 27,0,c);
}
