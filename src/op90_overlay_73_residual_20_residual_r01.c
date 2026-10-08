#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "font.h"
#include "render_window.h"
#include "palette.h"
#include "options.h"
#include "heap.h"
#include "sys_task_api.h"
typedef struct Args73 {
    u8 unk00[0x10];
    Options *options;
} Args73;
typedef struct Fade73 {
    SysTask *task;
    int active;
    u8 unk08[0];
} Fade73;
typedef struct Work73 {
    Args73 *args;
    BgConfig *bgConfig;
    u8 unk0008[0xF90];
    SysTask *fadeTask;
    int fadeActive;
    u16 basePltt[64];
    u16 fadePltt[336];
    u8 unk12C0[4];
    int fadeStep;
} Work73;
void ov73_021E8960(SysTask *task, void *data);
void ov73_021E8730(Work73 *work);
// @fn ov73_021E8730
void ov73_021E8730(Work73 *work) {
    NNSG2dPaletteData *plttData;
    NARC *narc;
    int outer;
    int count;
    int blend;
    int last;
    u16 *dest;
    u16 *src;
    BgConfig *bgConfig = work->bgConfig;
    void *raw;
    int i;
    narc = NARC_New(NARC_a_0_8_8, (enum HeapID)0x96);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, (enum HeapID)0x96);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, (enum HeapID)0x96);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1A0, (enum HeapID)0x96);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1A0, (enum HeapID)0x96);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 14, Options_GetFrame(work->args->options), (enum HeapID)0x96);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, (enum HeapID)0x96);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, (enum HeapID)0x96);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, FALSE, (enum HeapID)0x96);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 11, bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, (enum HeapID)0x96);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 12, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, FALSE, (enum HeapID)0x96);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    MI_CpuFill8(&work->fadeTask, 0, 0x330);
    raw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &plttData, (enum HeapID)0x96);
    MIi_CpuCopy16(plttData->pRawData, work->basePltt, 0x80);
    MIi_CpuCopy16(plttData->pRawData, work->fadePltt, 0x80);
    Heap_Free(raw);
    count = 0;
    outer = 0;
    dest = work->fadePltt;
    src = work->basePltt;
    while (outer < 3) {
        blend = 0;
        last = 0;
        while (TRUE) {
            GF_ASSERT(count < 21);
            for (i = 1; i < 16; i++) {
                BlendPalette(&src[i], &dest[i], 1, blend >> 8, work->basePltt[(outer + 1) * 16 + i]);
            }
            dest += 16;
            count++;
            if (last == 1) {
                break;
            }
            blend += 0x300;
            if (blend >= 0x1000) {
                blend = 0x1000;
                last = 1;
            }
        }
        src += 16;
        outer++;
    }
    DC_FlushRange(work->fadePltt, 0x2A0);
    work->fadeActive = 1;
    work->fadeStep = 0;
    work->fadeTask = SysTask_CreateOnVBlankQueue(ov73_021E8960, &work->fadeTask, 20);
    NARC_Delete(narc);
}
