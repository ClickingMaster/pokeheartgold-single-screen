#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int),*ReadMsgData_ExpandPlaceholders(void *,void *,int,int);
extern void FillWindowPixelBuffer(void *,int),BufferIntegerAsString(void *,int,int,int,int,int),AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *),CopyWindowToVram(void *),String_Delete(void *),DestroyMsgData(void *),MessageFormat_Delete(void *);
void ov74_0222EA88(void *,void *,int);
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
void ov74_0222EA88(void *s,void *window,int value){void *str;*(int *)((u8 *)s+0x2bf4)=value;PTR(s,0x2a04)=NewMsgDataFromNarc(1,27,247,85);PTR(s,0x2a00)=MessageFormat_New(85);FillWindowPixelBuffer(window,0);BufferIntegerAsString(PTR(s,0x2a00),0,*(int *)((u8 *)s+0x2bf4),1,1,1);str=ReadMsgData_ExpandPlaceholders(PTR(s,0x2a00),PTR(s,0x2a04),57,85);AddTextPrinterParameterizedWithColor(window,1,str,0,0,255,0x10200,NULL);CopyWindowToVram(window);String_Delete(str);DestroyMsgData(PTR(s,0x2a04));MessageFormat_Delete(PTR(s,0x2a00));}
