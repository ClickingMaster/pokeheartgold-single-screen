#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define S(p,o) (*(s16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
void EasyChatApp_GetCategoryWord(void*,int,void*),FillWindowPixelBuffer(void*,u8),BlitBitmapRectToWindow(void*,const void*,u16,u16,u16,u16,u16,u16,u16,u16);u8 AddTextPrinterParameterizedWithColor(void*,u32,void*,u32,u32,u32,u32,void*);void ov102_021EC13C(u8*,int,u32);
void ov102_021EC13C(u8*d,int row,u32 position){u32 x,height;EasyChatApp_GetCategoryWord(P(d,4),row,P(d,60));if(position<=240){AddTextPrinterParameterizedWithColor(d+16,0,P(d,60),(row&1)*112,position,255,0x10200,0);return;}height=256-position;FillWindowPixelBuffer(d+32,0);AddTextPrinterParameterizedWithColor(d+32,0,P(d,60),0,0,255,0x10200,0);x=(row&1)*112;BlitBitmapRectToWindow(d+16,P(d,44),0,0,96,16,(u16)x,(u16)position,96,(u16)height);BlitBitmapRectToWindow(d+16,P(d,44),0,(u16)height,96,16,(u16)x,0,96,(u16)(16-height));}
