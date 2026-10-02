#pragma force_active on
#include "global.h"
extern void LoadUserFrameGfx2(void *,int,int,int,u8,int);
extern int GetWindowBgId(void *);
extern void FillWindowPixelBuffer(void *,int);
extern void DrawFrameAndWindow2(void *,int,int,int);
extern u32 FontID_String_GetWidth(int,void *,int);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *);
extern void *NewString_ReadMsgData(void *,int);
extern void String_Delete(void *);
void ov81_02243028(void **,int);
void ov81_02243068(void *,void *,int,int,int,u32,int);
void ov81_022430B4(void *,void *,int,int,int,int,u32,int);
void ov81_02243028(void **w,int frame) { LoadUserFrameGfx2(*w,GetWindowBgId(w),994,11,(u8)frame,100); FillWindowPixelBuffer(w,15); DrawFrameAndWindow2(w,1,994,11); }
void ov81_02243068(void *w,void *s,int x,int y,int font,u32 color,int align) { if(align==1) x-=FontID_String_GetWidth(font,s,0); else if(align==2) x-=FontID_String_GetWidth(font,s,0)/2; AddTextPrinterParameterizedWithColor(w,font,s,x,y,255,color,0); }
void ov81_022430B4(void *w,void *msg,int id,int x,int y,int font,u32 color,int align) {void *s=NewString_ReadMsgData(msg,id);ov81_02243068(w,s,x,y,font,color,align);String_Delete(s);}
