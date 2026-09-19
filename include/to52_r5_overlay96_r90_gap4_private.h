#ifndef W16_R90_H
#define W16_R90_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern const s32 ov96_0221CBCC[8];
void FillWindowPixelBuffer(void *, int);
void BufferString(void *, u32, void *, u32, u32, u32);
void *ReadMsgData_ExpandPlaceholders(void *, void *, u32, u32);
void AddTextPrinterParameterizedWithColor(void *, u32, void *, s32, s32, s32, u32, void *);
void String_Delete(void *);
void CopyWindowToVram(void *);
void ov96_021EB408(void *, u32, u32, u32, u32);
void *ov96_021EB4F4(void *, u32, u32);
void *ov96_021EB5B8(void *);
void Sprite_SetMatrix(void *, const VecFx32 *);
void ov96_021EB52C(void *, BOOL, BOOL);
void ov96_022089D0(void *, void *);
void *ov96_021EB5EC(void *, u32, u32);
void *SpriteTransfer_GetCharProxy(void *);
void *SpriteTransfer_GetPaletteProxy(void *, void *);
void ov96_022088AC(void *, u32);
void ov96_02208914(void *, void *);
#endif
