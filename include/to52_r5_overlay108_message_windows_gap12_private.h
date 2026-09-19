#ifndef W07_OV108_PRIVATE_H
#define W07_OV108_PRIVATE_H
#include "global.h"
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
void FillWindowPixelBuffer(void *, int);
void ReadMsgDataIntoString(void *, int, void *);
void BufferString(void *, u32, void *, s32, s32, s32);
void BufferIntegerAsString(void *, u32, s32, u32, u32, u32);
void StringExpandPlaceholders(void *, void *, void *);
void DrawFrameAndWindow2(void *, int, int, int);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, int);
void ScheduleWindowCopyToVram(void *);
void ov108_021EA624(void *);
void ov108_021EA334(void *, int);
void ov108_021EA418(void *, int);
void ov108_021EA47C(void *);
#endif
