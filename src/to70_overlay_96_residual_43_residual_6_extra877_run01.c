#pragma force_active on
#include "global.h"
extern void AddWindowParameterized(void*,void*,u8,int,int,int,int,int,int),FillWindowPixelBuffer(void*,int),DrawFrameAndWindow2(void*,int,int,int);extern void*ReadMsgData_ExpandPlaceholders(void*,void*,int,int);extern int AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*);

int ov96_021EEA08(void*bg,void*window,void**out,int speed,void*msg,void*format,int id,u8 bgid,int heap);

int ov96_021EEA08(void*bg,void*window,void**out,int speed,void*msg,void*format,int id,u8 bgid,int heap){int printer;if(*(void**)window==0)AddWindowParameterized(bg,window,bgid,2,19,27,4,15,1);FillWindowPixelBuffer(window,15);*out=ReadMsgData_ExpandPlaceholders(msg,format,id,heap);printer=AddTextPrinterParameterizedWithColor(window,1,*out,0,0,speed,0x1020f,0);DrawFrameAndWindow2(window,0,0x3d2,13);return printer;}
