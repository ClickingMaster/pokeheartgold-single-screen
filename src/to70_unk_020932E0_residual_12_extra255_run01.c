#pragma force_active on
#include "global.h"
#pragma force_active on
void LoadUserFrameGfx2(void *,int,int,int,int,int);
void AddWindowParameterized(void *,void *,int,int,int,int,int,int,int);
void *NewMsgDataFromNarc(int,int,int,int);void *NewString_ReadMsgData(void *,int);
void FillWindowPixelBuffer(void *,int);
void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,int);
void String_Delete(void *),DestroyMsgData(void *),CopyWindowToVram(void *);
void GfGfx_EngineATogglePlanes(int,int);
void sub_0209501C(u8 *,int,int);
#define P(o) (*(void **)(w+(o)))
#define W(o) (*(int *)(w+(o)))
void sub_0209501C(u8 *w,int msg,int font){void *data,*str;
 LoadUserFrameGfx2(P(0),0,978,11,0,W(4));
 AddWindowParameterized(P(0),w+0x4660,3,2,21,21,2,13,198);
 data=NewMsgDataFromNarc(1,27,298,W(4));str=NewString_ReadMsgData(data,msg);
 FillWindowPixelBuffer(w+0x4660,15);
 AddTextPrinterParameterizedWithColor(w+0x4660,font,str,0,0,255,0x1020f,0);
 String_Delete(str);DestroyMsgData(data);CopyWindowToVram(w+0x4660);GfGfx_EngineATogglePlanes(8,1);
}
