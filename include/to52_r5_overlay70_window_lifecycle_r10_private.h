#ifndef TO52_R5_W01_PRIVATE_H
#define TO52_R5_W01_PRIVATE_H

#include "global.h"

typedef u8 Ov70Work;

#define PTR_AT(p, off) (*(void **)((u8 *)(p) + (off)))

void AddWindowParameterized(void *bgConfig, void *window, u32 bgId, u32 x, u32 y, u32 width, u32 height, u32 palette, u16 baseTile);
void FillWindowPixelBuffer(void *window, u8 fillValue);
u8 AddTextPrinterParameterizedWithColor(void *window, int fontId, void *string, u32 x, u32 y, u32 speed, u32 color, void *callback);
void RemoveWindow(void *window);
void *String_New(u32 maxLength, u32 heapId);
void *NewString_ReadMsgData(void *msgData, u32 msgNo);
void ov70_02239D44(Ov70Work *work, int arg);
void ov70_022394B8(Ov70Work *work);
void ov70_022395C4(Ov70Work *work);
void ov70_02239614(Ov70Work *work);

#endif
