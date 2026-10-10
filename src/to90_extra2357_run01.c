#pragma force_active on
#ifndef TO50_OV102_R20_GAP2_PRIVATE_H
#define TO50_OV102_R20_GAP2_PRIVATE_H
#include "global.h"
#include "filesystem.h"
typedef struct BgConfig BgConfig;
void *ov102_021EA268(void *);
void ov102_021EA80C(void *, NARC *);
void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *, s32, void *, u32, u32, u32, BOOL, u32);
u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *, s32, void *, u32, u32, u32, BOOL, u32);
u32 GfGfxLoader_LoadCharData(u32, s32, void *, u32, u32, u32, BOOL, u32);
void BgSetPosTextAndCommit(void *, u32, u32, s32);
void AddWindowParameterized(void *, void *, u8, u8, u8, u8, u8, u8, u16);
void *YesNoPrompt_Create(u32);
void GfGfxLoader_GXLoadPal(u32, s32, u32, u32, u32, u32);
u32 ov102_021E9084(void *);
void LoadUserFrameGfx2(void *, u32, u16, u32, u8, u32);
void DrawFrame2AroundWindow(void *, u32, u32);
void PutWindowTilemap(void *);
void ov102_021EA8C0(void *);
void ov102_021EAA3C(void *);
void ov102_021EAE40(void *, u32);
void ov102_021EA920(void *);
u32 ov102_021E8F6C(void *);
void ov102_021EAC20(void *);
void ov102_021EA41C(u8 *, NARC *);
void ov102_021EA644(u8 *);
void ov102_021EA678(void *, BOOL);
#endif

#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define S16(p,o) (*(s16 *)((u8 *)(p)+(o)))

#pragma force_active on
void ov102_021EA41C(u8 *work, NARC *narc) {
    void *bg = ov102_021EA268(PTR(work, 0));
    u32 n;
    ov102_021EA80C(work, narc);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, bg, 0, 0, 0, TRUE, 0x23);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, bg, 4, 0, 0, TRUE, 0x23);
    S16(work, 0x1E2) = -192;
    BgSetPosTextAndCommit(bg, 4, 3, S16(work, 0x1E2));
    n = GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bg, 0, 0, 0, TRUE, 0x23);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, bg, 4, 0, 0, TRUE, 0x23);
    n >>= 5;
    AddWindowParameterized(bg, work + 0x0C, 0, 3, 1, 0x1B, 4, 0, (u16)n);
    AddWindowParameterized(bg, work + 0x1C, 4, 3, 1, 0x1B, 4, 0, (u16)n);
    AddWindowParameterized(bg, work + 0x2C, 0, 2, 0x15, 0x1B, 2, 0x0B, (u16)(n + 0x6C));
    AddWindowParameterized(bg, work + 0x3C, 0, 2, 0x15, 0x13, 2, 0x0B, (u16)(n + 0x6C));
    AddWindowParameterized(bg, work + 0x4C, 0, 0x19, 0x0C, 8, 4, 0x0B, (u16)(n + 0xA2));
    U16(work, 0x1EE) = (u16)(n + 0xA2);
    PTR(work, 0x1E8) = YesNoPrompt_Create(0x23);
    GfGfxLoader_LoadCharData(0x26, 0, bg, 0, n + 0xDE, 0, FALSE, 0x23);
    U32(work, 0x90) = n + 0xDE;
    GfGfxLoader_GXLoadPal(0x26, 0x19, 0, 7 << 6, 0x20, 0x23);
    U16(work, 0x1EC) = (u16)(n + 0xE7);
    LoadUserFrameGfx2(bg, 0, (u16)(n + 0xE7), 0x0F, (u8)ov102_021E9084(PTR(work, 4)), 0x23);
    PTR(work, 0x5C) = work + 0x2C;
    DrawFrame2AroundWindow(work + 0x2C, n + 0xE7, 0x0F);
    PutWindowTilemap(work + 0x0C);
    PutWindowTilemap(work + 0x1C);
    PutWindowTilemap(work + 0x2C);
    PutWindowTilemap(work + 0x3C);
    ov102_021EA8C0(work);
    ov102_021EAA3C(work);
    ov102_021EAE40(work, 0);
    ov102_021EA920(work);
    if (ov102_021E8F6C(PTR(work, 4)) == 2) ov102_021EAC20(work);
}
