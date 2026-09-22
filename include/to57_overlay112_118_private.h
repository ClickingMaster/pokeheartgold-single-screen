#ifndef TO57_R3_10B_PRIVATE_H
#define TO57_R3_10B_PRIVATE_H
#include "global.h"
typedef struct Ov112R118 {
    u8 raw[0x1F2C4];
} Ov112R118;
void *String_New(int size, int heap);
void String_Delete(void *s);
void CopyU16ArrayToStringN(void *dst, const void *src, int n);
void BufferString(void *fmt, int field, void *str, u32 value, int a4, int a5);
void FillBgTilemapRect(void *bg, int layer, int fill, int x, int y, int width, int height, int palette);
void BgCommitTilemapBufferToVram(void *bg, int layer);
BOOL ov112_021ED610(void *p);
BOOL ov112_021ED640(void *p);
void *ov112_021EA08C(void *p, int a, int b);
void ov112_021E7464(void);
void ov112_021EAAE4(void *p, void *q);
int TextPrinterCheckActive(u8 id);
int IsFanfarePlaying(void);
void *AllocMonZeroed(int heap);
int Pokewalker_TryGetBoxMon(void *walker, void *mon);
u32 GetMonData(void *mon, int attr, void *dest);
void Heap_Free(void *p);
int ov112_021EDF4C(Ov112R118 *w);
int ov112_021EDFE8(Ov112R118 *w);
int ov112_021EE044(Ov112R118 *w);
#endif
