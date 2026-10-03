#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
#include "bg_window.h"
extern WindowTemplate ov81_022435A8[];
void ov81_02242FDC(BgConfig*p,Window*q);
void ov81_02242FDC(BgConfig*p,Window*q){u8 i;for(i=0;i<18;i++){AddWindow(p,&q[i],&ov81_022435A8[i]);FillWindowPixelBuffer(&q[i],0);}}
