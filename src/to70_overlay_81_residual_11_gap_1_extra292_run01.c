#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[28];void*message;u8 gap[224];u8 windows[4][16];}W;extern void FillWindowPixelBuffer(void*,int),ov81_022430B4(void*,void*,int,int,int,int,int,int),CopyWindowPixelsToVram_TextMode(void*);extern u8 GetWindowWidth(void*);void ov81_02241524(W*);
void ov81_02241524(W*w){int width;
FillWindowPixelBuffer(w->windows[0],2);
FillWindowPixelBuffer(w->windows[1],2);
FillWindowPixelBuffer(w->windows[2],2);
FillWindowPixelBuffer(w->windows[3],2);
width=GetWindowWidth(w->windows[0]);ov81_022430B4(w->windows[0],w->message,5,(width*8)/2,0,4,0xf0e02,2);
width=GetWindowWidth(w->windows[1]);ov81_022430B4(w->windows[1],w->message,6,(width*8)/2,0,4,0xf0e02,2);
width=GetWindowWidth(w->windows[2]);ov81_022430B4(w->windows[2],w->message,8,(width*8)/2,0,4,0xf0e02,2);
width=GetWindowWidth(w->windows[3]);ov81_022430B4(w->windows[3],w->message,7,(width*8)/2,0,4,0xf0e02,2);
CopyWindowPixelsToVram_TextMode(w->windows[0]);
CopyWindowPixelsToVram_TextMode(w->windows[1]);
CopyWindowPixelsToVram_TextMode(w->windows[2]);
CopyWindowPixelsToVram_TextMode(w->windows[3]);
}
