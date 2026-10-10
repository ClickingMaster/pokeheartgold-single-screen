#ifndef TO50_OV102_R25_PRIVATE_H
#define TO50_OV102_R25_PRIVATE_H
#include "global.h"
void *ov102_021EA268(void *);
void FillBgTilemapRect(void *,u32,u32,u32,u32,u32,u32,u32);
void ClearWindowTilemapAndCopyToVram(void *);
void DrawFrame2AroundWindow(void *,u32,u32);
void FillWindowPixelBuffer(void *,u32);
void *MessageFormat_New_Custom(u32,u32,u32);
void *String_New(u32,u32);
void BufferPocketName(void *,u32,u32);
u32 ov102_021E8F70(void *);
void ReadMsgDataIntoString(void *,u32,void *);
void StringExpandPlaceholders(void *,void *,void *);
void AddTextPrinterParameterizedWithColor(void *,u32,void *,u32,u32,u32,u32,void *);
void String_Delete(void *);
void MessageFormat_Delete(void *);
void CopyWindowToVram(void *);
void ov102_021EADF8(u8 *,u32);
void ov102_021EAE40(u8 *,u32);
#endif
