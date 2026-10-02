#pragma force_active on
#include "global.h"
extern void FillWindowPixelBuffer(void*,u8);extern void *ov47_02259D58(void*,int,int);extern void AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,u32,void*);extern void ScheduleWindowCopyToVram(void*);

void ov47_0225916C(void*w,int a);

void ov47_0225916C(void*w,int a){void*s;FillWindowPixelBuffer(w,0);s=ov47_02259D58((u8*)w+0x2c,a,0);AddTextPrinterParameterizedWithColor(w,1,s,0,0,255,0x10200,0);s=ov47_02259D58((u8*)w+0x2c,a,1);AddTextPrinterParameterizedWithColor(w,1,s,0,24,255,0x10200,0);s=ov47_02259D58((u8*)w+0x2c,a,2);AddTextPrinterParameterizedWithColor(w,1,s,0,48,255,0x10200,0);ScheduleWindowCopyToVram(w);}
