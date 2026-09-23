#ifndef TO46_CLOSE_OVERLAY102_R14_MIDDLE_PRIVATE_H
#define TO46_CLOSE_OVERLAY102_R14_MIDDLE_PRIVATE_H

#include "global.h"
#include "filesystem.h"

typedef struct Ov102StateR14 {
    void *work;
    void *arg1;
    void *arg2;
    u32 unused;
    u32 index;
    u32 state;
} Ov102StateR14;

void Heap_Free(void *ptr);
void GfGfxLoader_LoadImageMappingFromOpenNarc(NARC *narc, u32 member, BOOL compressed, u32 screen, u32 offset, u32 size, u32 slot, u32 heapId, void *proxy);
void GfGfxLoader_PartiallyLoadPaletteFromOpenNarc(NARC *narc, u32 member, u32 palette, u32 offset, u32 heapId, void *proxy);
void *GfGfxLoader_GetCellBankFromOpenNarc(NARC *narc, u32 member, BOOL compressed, void *dst, u32 heapId);
void *GfGfxLoader_GetAnimBankFromOpenNarc(NARC *narc, u32 member, BOOL compressed, void *dst, u32 heapId);
void sub_0203A964(void);
void BeginNormalPaletteFade(u32 mask, s32 delay, s32 start, s32 end, u16 color, u32 duration, u32 heapId);
BOOL IsPaletteFadeFinished(void);
void ov102_021EA380(void *work);
void ov102_021EB1E0(void *work);
void ov102_021EB6C8(void *work);
void ov102_021EBCBC(void *work);
void ov102_021EC240(void *work);
void ov102_021E937C(void *work);
void FontID_Release(u32 fontId);
void FreeBgTilemapBuffer(void *bg, u32 layer);
void ov102_021E94A4(void *state);
void ov102_021E95E0(void *state, NARC *narc);
void ov102_021E9694(void *task, void *state);

#endif
