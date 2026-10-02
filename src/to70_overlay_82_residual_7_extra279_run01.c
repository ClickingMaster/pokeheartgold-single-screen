#pragma force_active on
#include "global.h"
#pragma force_active on
void *Party_GetMonByIndex(void *,int);int GetMonData(void *,int,void *);
void FillWindowPixelBuffer(void *,u8);void *String_New(int,int);void CopyU16ArrayToString(void *,u16 *);
void AddTextPrinterParameterizedWithColor(void *,u8,void *,int,int,int,u32,int);void String_Delete(void *);void CopyWindowToVram(void *);
void ov82_0223EFCC(u8 *,void *,int,int,u8,u8,u8,u8);
void ov82_0223EFCC(u8 *w,void *win,int x,int y,u8 fg,u8 shadow,u8 bg,u8 font){u16 name[12];void *str;GetMonData(Party_GetMonByIndex(*(void **)(w+532),0),179,name);FillWindowPixelBuffer(win,bg);str=String_New(11,105);CopyU16ArrayToString(str,name);AddTextPrinterParameterizedWithColor(win,font,str,x,y,0,(((u32)fg<<24)>>8)|(((u32)shadow<<24)>>16)|bg,0);String_Delete(str);CopyWindowToVram(win);}
