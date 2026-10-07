#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov112_021F05FC(void*,int),ov112_021F09B4(void*),ov112_021F0B28(void*),GfGfx_EngineBTogglePlanes(u8,u8);extern void AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16);extern void*NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int),*String_New(u32,int);
typedef struct{int heap;void*bg;u8 windows[6][16];void*msg,*format,*string;u8 tail[1];}Work;
void ov112_021F03BC(Work*,void*);
void ov112_021F03BC(Work*w,void*bg){w->bg=bg;ov112_021F05FC(bg,w->heap);AddWindowParameterized(w->bg,w->windows[0],5,14,9,10,2,15,29);AddWindowParameterized(w->bg,w->windows[1],5,23,6,5,2,15,17);AddWindowParameterized(w->bg,w->windows[2],5,14,12,8,2,15,49);AddWindowParameterized(w->bg,w->windows[3],5,14,6,8,2,15,1);AddWindowParameterized(w->bg,w->windows[4],5,22,6,1,2,15,27);AddWindowParameterized(w->bg,w->windows[5],5,14,14,16,2,15,65);w->msg=NewMsgDataFromNarc(1,27,276,w->heap);w->format=MessageFormat_New(w->heap);w->string=String_New(11,w->heap);ov112_021F09B4(w->tail);ov112_021F0B28(w->tail);GfGfx_EngineBTogglePlanes(2,0);GfGfx_EngineBTogglePlanes(4,0);}
