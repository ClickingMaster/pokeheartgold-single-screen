#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work{void*bg;u8 pad4[0x34];u8 text[16],window[16];u8 pad58[0x157c];int state;}Work;
extern void*OverlayManager_GetData(void*);extern int ov74_02235230(Work*);extern void ov74_02229F60(void*,int),AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16),DrawFrameAndWindow1(void*,int,int,int),ov74_0222A89C(void*,const void*,int,void*,int);extern u8 ov74_0223C220[],ov74_0223C230[],ov74_0223C240[],ov74_0223C250[];
int ov74_0222A2E0(void*),ov74_0222A35C(void*),ov74_0222A3CC(void*);void ov74_0222A43C(void*);
int ov74_0222A2E0(void*manager){void*window;Work*w=OverlayManager_GetData(manager);w->state=ov74_02235230(w)?2:1;ov74_02229F60(w->text,0);window=w->window;if(!*(void**)window)AddWindowParameterized(w->bg,window,0,23,10,6,4,0,428);DrawFrameAndWindow1(window,1,31,3);ov74_0222A89C(manager,ov74_0223C250,2,window,2);return 5;}
int ov74_0222A35C(void*manager){Work*w=OverlayManager_GetData(manager);w->state=4;ov74_02229F60(w->text,0);void*window=w->window;if(!*(void**)window)AddWindowParameterized(w->bg,window,0,23,10,6,4,0,428);DrawFrameAndWindow1(window,1,31,3);ov74_0222A89C(manager,ov74_0223C220,2,window,80);return 5;}
int ov74_0222A3CC(void*manager){Work*w=OverlayManager_GetData(manager);w->state=3;ov74_02229F60(w->text,0);void*window=w->window;if(!*(void**)window)AddWindowParameterized(w->bg,window,0,23,10,6,4,0,428);DrawFrameAndWindow1(window,1,31,3);ov74_0222A89C(manager,ov74_0223C230,2,window,79);return 5;}
void ov74_0222A43C(void*manager){Work*w=OverlayManager_GetData(manager);void*window=w->window;if(!*(void**)window)AddWindowParameterized(w->bg,window,0,23,10,6,4,0,428);DrawFrameAndWindow1(window,1,31,3);ov74_0222A89C(manager,ov74_0223C240,2,window,4);}
