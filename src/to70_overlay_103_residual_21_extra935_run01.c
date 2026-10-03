#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x1c8];u8 window[0x68];void*msg;u8 printer;}Inner;
typedef struct{void*options;u8 p[8];Inner *inner;}Work;
extern int Options_GetTextFrameDelay(void*);
extern void FillWindowPixelBuffer(void*,int);
extern int AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*);

void ov103_021EE7DC(Work*w);

static inline u32 offset(void){return 456;}
void ov103_021EE7DC(Work*w){Inner *inner=w->inner;u32 off=offset();FillWindowPixelBuffer((void*)((u32)inner+off),15);*(u8*)((u32)w->inner+off+0x6c)=AddTextPrinterParameterizedWithColor((void*)((u32)inner+off),1,*(void**)((u32)w->inner+off+0x68),0,0,Options_GetTextFrameDelay(w->options),0x1020f,0);}
