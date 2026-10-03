#pragma force_active on
#include "global.h"
#pragma force_active on
extern u32 FontID_String_GetWidth(int,void*,int);
extern int AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*);

void ov83_02247998(void*w,void *s,int x,int y,int font,int color,int align);

void ov83_02247998(void*w,void *s,int x,int y,int font,int color,int align){if(align==1)x-=FontID_String_GetWidth(font,s,0);else if(align==2)x-=FontID_String_GetWidth(font,s,0)>>1;AddTextPrinterParameterizedWithColor(w,font,s,x,y,255,color,0);}
