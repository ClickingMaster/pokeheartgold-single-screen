#include "global.h"

typedef struct Ov41WindowConfig {
    void *resource;
    u32 field04;
    u32 field08;
    u32 field0C;
    u32 field10;
    u32 field14;
    u32 field18;
    u32 field1C;
    u32 field20;
    u32 field24;
    u32 field28;
    u32 field2C;
} Ov41WindowConfig;

extern void ov41_02249C7C(void *, const Ov41WindowConfig *);
extern u32 Options_GetFrame(void *);
extern void LoadUserFrameGfx2(void *, u32, u16, u8, u8, u32);
extern void LoadFontPal1(u32, u32, u32);
extern void *AllocWindows(u32, s32);
extern void InitWindow(void *);
extern void AddWindowParameterized(void *, void *, u8, u8, u8, u8, u8, u8, u16);
extern void FillWindowPixelBuffer(void *, u8);
extern void DrawFrameAndWindow2(void *, BOOL, u16, u8);
extern void CopyWindowToVram(void *);

void ov41_0224ACA4(void *, void *);
void ov41_0224ACDC(void *, void *);
void ov41_0224AD0C(void **, void *, int, int, int, int, int, int);

void ov41_0224ACA4(void *dest, void *resource) {
    Ov41WindowConfig config;

    config.resource = resource;
    config.field04 = 0x1A;
    config.field08 = 0xE0;
    config.field0C = 0xE1;
    config.field10 = 0xE2;
    config.field14 = 0;
    config.field18 = 0;
    config.field1C = 4;
    config.field20 = 1;
    config.field24 = 0;
    config.field28 = 0;
    config.field2C = 0xE;
    ov41_02249C7C(dest, &config);
}

void ov41_0224ACDC(void *bgConfig, void *options) {
    u8 frame = Options_GetFrame(options);

    LoadUserFrameGfx2(bgConfig, 5, 1, 1, frame, 0xE);
    LoadFontPal1(4, 0x40, 0xE);
}

void ov41_0224AD0C(void **windowOut, void *bgConfig, int left, int top, int width, int height, int baseTile, int drawFrame) {
    *windowOut = AllocWindows(0xE, 1);
    InitWindow(*windowOut);
    AddWindowParameterized(bgConfig, *windowOut, 5, left, top, width, height, 2, baseTile);
    FillWindowPixelBuffer(*windowOut, 0xF);
    if (drawFrame != FALSE) {
        DrawFrameAndWindow2(*windowOut, FALSE, 1, 1);
    }
    CopyWindowToVram(*windowOut);
}
