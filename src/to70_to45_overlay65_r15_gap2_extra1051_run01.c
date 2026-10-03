#pragma force_active on
#include "global.h"
extern int FontID_String_GetWidth(int,void *,int);extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,void *);void ov65_0221FB4C(void *,void *,int,int,int,int);
void ov65_0221FB4C(void *window,void *str,int unused,int speed,int align,int y){int x=align;if(align==1){int width=FontID_String_GetWidth(0,str,0);x=(((u8*)window)[7]*8-width)/2;}AddTextPrinterParameterizedWithColor(window,0,str,x,y,speed,0xb0c00,0);}
