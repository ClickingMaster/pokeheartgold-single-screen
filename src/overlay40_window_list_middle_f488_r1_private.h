#ifndef TO50_R1_OV40_F488_PRIVATE_H
#define TO50_R1_OV40_F488_PRIVATE_H

#include "global.h"

#define PTR(p, off)   (*(void **)((u8 *)(p) + (off)))
#define SWORD(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define WORD(p, off)  (*(u32 *)((u8 *)(p) + (off)))
#define SHALF(p, off) (*(s16 *)((u8 *)(p) + (off)))

void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void CopyWindowToVram(void *);
void FillWindowPixelBuffer(void *, u8);
void GF_AssertFail(void);
void *NewString_ReadMsgData(void *, int);
void PlaySE(u16);
void String_Delete(void *);
BOOL TouchscreenHitbox_TouchNewIsIn(const void *);
void ov40_0222F8C0(void *);
void ov40_02230944(void *);
int ov40_0222EFD8(void *, void *, void **);
void *ov40_0222F38C(void *, void *);
void ov40_0222F5EC(void *, s16);

#endif
void *String_New(u32, u32);
void String16_FormatInteger(void *, int, int, int, int);
void CopyU16ArrayToString(void *, const u16 *);
void ov40_02230DCC(void *, void *);
int FontID_String_GetWidth(int, void *, int);
int ov40_0222F488(void *, void *);
