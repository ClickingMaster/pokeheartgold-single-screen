#pragma force_active on
#include "global.h"
#define P(p,o) (*(void**)((p)+(o)))
extern int Comm_GetLocalNetId(void),FontID_String_GetWidth(int,void*,int);extern void ov73_021E71E4(u8*,int,int),AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*),String_Delete(void*),ScheduleBgTilemapBufferTransfer(void*,int);extern void*NewString_ReadMsgData(void*,int);
void ov73_021E7230(u8*);
void ov73_021E7230(u8*w){void*str;int width;if(!Comm_GetLocalNetId()){ov73_021E71E4(w,0,0);str=NewString_ReadMsgData(P(w,0x28),21);width=64-FontID_String_GetWidth(4,str,0);AddTextPrinterParameterizedWithColor(w+0x2b8,4,str,width/2,0,0,0x40100,NULL);String_Delete(str);}*(u32*)(w+0x4a38)=-1;ov73_021E71E4(w,1,0);str=NewString_ReadMsgData(P(w,0x28),22);width=64-FontID_String_GetWidth(4,str,0);AddTextPrinterParameterizedWithColor(w+0x2c8,4,str,width/2,0,0,0x40100,NULL);String_Delete(str);ScheduleBgTilemapBufferTransfer(P(w,0),1);ScheduleBgTilemapBufferTransfer(P(w,0),0);}
