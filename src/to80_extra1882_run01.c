#pragma force_active on
#include "global.h"
extern void *NewMsgDataFromNarc(int,int,int,int),*NewString_ReadMsgData(void *,int);
extern void FillWindowPixelBuffer(void *,int),String_Delete(void *),DestroyMsgData(void *),ScheduleWindowCopyToVram(void *),ClearWindowTilemapAndScheduleTransfer(void *),AddTextPrinterParameterizedWithColor(void *,int,void *,u32,int,int,int,void *);
extern int FontID_String_GetWidth(int,void *,int);
void ov57_0223866C(void *,int);
#pragma force_active on
void ov57_0223866C(void *w,int show){if(show){void *msg=NewMsgDataFromNarc(0,27,11,52);void *s=NewString_ReadMsgData(msg,17);FillWindowPixelBuffer((u8*)w+0x10c,0);{u32 x=64-FontID_String_GetWidth(4,s,0);AddTextPrinterParameterizedWithColor((u8*)w+0x10c,4,s,x>>1,0,255,0x10f00,0);}String_Delete(s);DestroyMsgData(msg);ScheduleWindowCopyToVram((u8*)w+0x10c);}else ClearWindowTilemapAndScheduleTransfer((u8*)w+0x10c);}
