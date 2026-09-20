#include "to54_atomic_h03_to54_overlay39_palette_internal.h"

void ov39_022285CC(void *p) {
    void *plttData;
    u8 *colors;
    volatile int coefficient;
    void *narc;
    void *raw;
    void *bg = PTR(p, 4);
    int outer;
    int palette;

    narc = NARC_New(0x58, 0x7C);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, 0, 0, 0, 0x7C);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, 4, 0, 0, 0x7C);
    LoadFontPal1(0, 0x1A0, 0x7C);
    LoadFontPal1(4, 0x1A0, 0x7C);
    LoadUserFrameGfx2(bg, 0, 1, 0xA, (u8)Options_GetFrame(Save_PlayerData_GetOptionsAddr(PTR(PTR(PTR(p, 0), 0), 4))), 0x7C);
    LoadUserFrameGfx1(bg, 0, 0x1F, 0xB, 0, 0x7C);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bg, 1, 0, 0, 0, 0x7C);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bg, 1, 0, 0x600, 0, 0x7C);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0xB, bg, 5, 0, 0, 0, 0x7C);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xC, bg, 5, 0, 0x600, 0, 0x7C);
    BG_SetMaskColor(0, 0);
    BG_SetMaskColor(4, 0);
    MI_CpuFill8((u8 *)p + 0xA8, 0, 0x330);
    raw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, 0x7C);
    MIi_CpuCopy16(PTR(plttData, 0xC), (u16 *)((u8 *)p + 0xB0), 0x80);
    MIi_CpuCopy16(PTR(plttData, 0xC), (u16 *)((u8 *)p + 0x130), 0x80);
    Heap_Free(raw);
    palette = 0;
    outer = 0;
    {
        int blend;
        int last;
        u8 *dest = (u8 *)p + 0x130;
        u8 *src = (u8 *)p + 0xB0;
        while (outer < 3) {
            blend = 0;
            last = 0;
            colors = (u8 *)p + (outer + 1) * 0x20;
            do {
                int i;
                u8 *color;
                u8 *dst;
                u8 *source;
                if (palette >= 0x15) {
                    GF_AssertFail();
                }
                color = colors + 2;
                dst = dest + 2;
                source = src + 2;
                coefficient = (u8)(blend >> 8);
                for (i = 1; i < 0x10; i++) {
                    BlendPalette(source, dst, 1, coefficient, U16(color, 0xB0));
                    color += 2;
                    dst += 2;
                    source += 2;
                }
                dest += 0x20;
                palette++;
                if (last == 1) {
                    break;
                }
                blend += 0x300;
                if (blend >= 0x1000) {
                    blend = 0x1000;
                    last = 1;
                }
            } while (1);
            src += 0x20;
            outer++;
        }
    }
    DC_FlushRange((u8 *)p + 0x130, 0x2A0);
    U32(p, 0xAC) = 1;
    U32(p, 0x3D4) = 0;
    PTR(p, 0xA8) = SysTask_CreateOnVBlankQueue(ov39_022287F8, (u8 *)p + 0xA8, 0x14);
    NARC_Delete(narc);
}
