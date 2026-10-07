#pragma force_active on
#include "global.h"
#pragma force_active on
#include "bg_window.h"
typedef struct {BgConfig *bg;void *save;u8 p[0x20];u8 state[16];Window win;u8 p2[0x580];int tick,flag;} Work;
extern void DrawFrameAndWindow1(Window *,int,int,int);extern void *OverlayManager_GetData(void *);extern void *Save_MysteryGift_Get(void *);extern void ov74_0222CE10(Work *);extern void ov74_02229F60(void *,int);extern void ov74_0222A89C(void *,void *,int,Window *,int);extern u8 ov74_0223C2B0[];int ov74_0222A5CC(void *);
int ov74_0222A5CC(void *mgr){Window *win;Work *w=OverlayManager_GetData(mgr);Save_MysteryGift_Get(w->save);if(w->tick==30){w->flag=1;ov74_0222CE10(w);}ov74_02229F60(w->state,0);win=&w->win;if(win->bgConfig==0)AddWindowParameterized(w->bg,win,0,15,9,16,8,0,292);DrawFrameAndWindow1(win,1,31,3);ov74_0222A89C(mgr,ov74_0223C2B0,4,win,1);return 4;}
