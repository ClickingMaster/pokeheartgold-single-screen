#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
typedef struct Work71 {
    void *unk00;
    u8 unk04[0xBC];
    BgConfig *bgConfig;
    void *unkC4;
    u8 unkC8[4];
    void *unkCC;
    int unkD0;
} Work71;
extern const GraphicsBanks ov71_0224BD54;
extern const GraphicsModes ov71_0224BCEC;
extern const BgTemplate ov71_0224BD1C;
extern const BgTemplate ov71_0224BD38;
extern const u16 ov71_0224BCE4[];
int ov71_022473A0(void *a0);
void *ov71_02246C6C(void *a0, int a1);
void *ov71_02247000(BgConfig *bgConfig, int a1, int a2, int a3, int a4, int a5, int *a6);
void ov71_02248604(Work71 *work);
// @fn ov71_02248604
void ov71_02248604(Work71 *work) {
    OSIntrMode intrMode;
    u32 srcOffset;
    void *buf;
    GfGfx_SetBanks(&ov71_0224BD54);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&ov71_0224BCEC);
    InitBgFromTemplate(work->bgConfig, GF_BG_LYR_MAIN_2, &ov71_0224BD1C, GF_BG_TYPE_AFFINE);
    InitBgFromTemplate(work->bgConfig, GF_BG_LYR_SUB_2, &ov71_0224BD1C, GF_BG_TYPE_AFFINE);
    intrMode = OS_DisableInterrupts();
    InitBgFromTemplate(work->bgConfig, GF_BG_LYR_MAIN_3, &ov71_0224BD38, GF_BG_TYPE_256x16PLTT);
    InitBgFromTemplate(work->bgConfig, GF_BG_LYR_SUB_3, &ov71_0224BD38, GF_BG_TYPE_256x16PLTT);
    OS_RestoreInterrupts(intrMode);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, (enum HeapID)0x39);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, (enum HeapID)0x39);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, (enum HeapID)0x39);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, (enum HeapID)0x39);
    srcOffset = ov71_022473A0(work->unk00);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, (enum HeapID)0x39);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, (enum HeapID)0x39);
    srcOffset = ov71_0224BCE4[srcOffset];
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, (enum HeapID)0x39);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, (enum HeapID)0x39);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, (enum HeapID)0x39);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, (enum HeapID)0x39);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, (enum HeapID)0x39);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, (enum HeapID)0x39);
    buf = Heap_Alloc((enum HeapID)0x39, 0x60);
    if (buf != NULL) {
        MI_CpuClear32(buf, 0x60);
        DC_FlushRange(buf, 0x60);
        GX_BeginLoadBGExtPltt();
        GXS_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt(buf, 0x6000, 0x60);
        GXS_LoadBGExtPltt(buf, 0x6000, 0x60);
        GX_EndLoadBGExtPltt();
        GXS_EndLoadBGExtPltt();
        Heap_Free(buf);
    }
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, -256);
    ToggleBgLayer(GF_BG_LYR_SUB_3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    work->unkC4 = ov71_02246C6C(work->unkC8, 1);
    work->unkCC = ov71_02247000(work->bgConfig, 0x480, 0x1000, 1, 0x128, 0, &work->unkD0);
}
