#pragma force_active on
#include "global.h"
#pragma force_active on
extern void FillWindowPixelBuffer(void*,u8),CopyWindowPixelsToVram_TextMode(void*),ov81_022430B4(void*,void*,int,int,int,int,u32,int);extern u8 GetWindowWidth(void*);
void ov81_02241A98(u8*);
void ov81_02241A98(u8*w){int width;FillWindowPixelBuffer(w+0x150,2);FillWindowPixelBuffer(w+0x160,2);FillWindowPixelBuffer(w+0xe0,2);FillWindowPixelBuffer(w+0xf0,2);width=GetWindowWidth(w+0x150);ov81_022430B4(w+0x150,*(void**)(w+28),25,(width*8)/2,0,4,0xf0e02,2);width=GetWindowWidth(w+0x160);ov81_022430B4(w+0x160,*(void**)(w+28),22,(width*8)/2,0,4,0xf0e02,2);width=GetWindowWidth(w+0xe0);ov81_022430B4(w+0xe0,*(void**)(w+28),23,(width*8)/2,0,4,0xf0e02,2);width=GetWindowWidth(w+0xf0);ov81_022430B4(w+0xf0,*(void**)(w+28),24,(width*8)/2,0,4,0xf0e02,2);CopyWindowPixelsToVram_TextMode(w+0x150);CopyWindowPixelsToVram_TextMode(w+0x160);CopyWindowPixelsToVram_TextMode(w+0xe0);CopyWindowPixelsToVram_TextMode(w+0xf0);}
