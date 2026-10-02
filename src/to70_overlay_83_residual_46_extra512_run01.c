#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_window.h"
typedef struct {const WindowTemplate *templates;u32 count;} Entry;
extern Entry ov83_0224819C[];
extern Entry ov83_0224819C_count[];
void ov83_022478D4(BgConfig *,Window *,u32);void ov83_0224791C(Window *,u32);void ov83_02247944(Window *,int);
void ov83_022478D4(BgConfig *bg,Window *windows,u32 n){const WindowTemplate *templates=ov83_0224819C[n].templates;u8 i=0;u32 count=*(u32 *)((u8 *)ov83_0224819C+4+n*8);for(;i<count;i++){AddWindow(bg,&windows[i],&templates[i]);FillWindowPixelBuffer(&windows[i],0);}}
void ov83_0224791C(Window *windows,u32 n){u16 i;u32 count=*(u32 *)((u8 *)ov83_0224819C+4+n*8);i=0;for(;i<count;i++)RemoveWindow(&windows[i]);}
void ov83_02247944(Window *w,int frame){LoadUserFrameGfx2(w->bgConfig,(GFBgLayer)GetWindowBgId(w),0x3d9,10,(u8)frame,(enum HeapID)107);FillWindowPixelBuffer(w,15);DrawFrameAndWindow2(w,1,0x3d9,10);ScheduleWindowCopyToVram(w);}
