#include "field/field_motion_blur.h"

#include "global.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "sys_task_api.h"
#include "systask_environment.h"

// declared only by the NitroSDK internal headers, which clash with lib/include
u32 GX_GetBankForLCDC(void);

static inline void FieldCapture_SetCaptureRegister(GXCaptureSize size, GXCaptureMode mode, GXCaptureSrcA srcA, GXCaptureSrcB srcB, GXCaptureDest dest, int eva, int evb) {
    reg_GX_DISPCAPCNT = REG_GX_DISPCAPCNT_FIELD(1, mode, 0, srcB, srcA, size, 0, dest, evb, eva);
}

static const FieldCaptureConfig sMotionBlurConfig = {
    .dispMode = GX_DISPMODE_VRAM_C,
    .bgMode = GX_BGMODE_0,
    .bg0As = GX_BG0_AS_3D,
    .size = GX_CAPTURE_SIZE_256x192,
    .mode = GX_CAPTURE_MODE_AB,
    .srcA = GX_CAPTURE_SRCA_2D3D,
    .srcB = GX_CAPTURE_SRCB_VRAM_0x00000,
    .dest = GX_CAPTURE_DEST_VRAM_C_0x00000,
    .eva = 0,
    .evb = 0,
    .heapID = HEAP_ID_FIELD1,
};

FieldCapture *FieldCapture_Start(const FieldCaptureConfig *config) {
    SysTask *task = CreateSysTaskAndEnvironment((SysTaskFunc)FieldCapture_Task, sizeof(FieldCapture), 5, config->heapID);
    FieldCapture *capture = SysTask_GetData(task);
    capture->config = *config;
    capture->task = task;
    capture->started = FALSE;
    capture->savedLcdcBanks = GX_GetBankForLCDC();
    FieldCapture_Init(&capture->config);
    SysTask_CreateOnVWaitQueue((SysTaskFunc)FieldCapture_StartTask, capture, 0);
    return capture;
}

void FieldCapture_Stop(FieldCapture **capture, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0As) {
    GX_SetGraphicsMode(dispMode, bgMode, bg0As);
    GX_SetBankForLCDC((GXVRamLCDC)(*capture)->savedLcdcBanks);
    switch ((*capture)->config.dispMode) {
    case GX_DISPMODE_VRAM_A:
        MI_CpuClearFast((void *)HW_LCDC_VRAM, 0x20000);
        break;
    case GX_DISPMODE_VRAM_B:
        MI_CpuClearFast((void *)(HW_LCDC_VRAM + 0x20000), 0x20000);
        break;
    case GX_DISPMODE_VRAM_C:
        MI_CpuClearFast((void *)(HW_LCDC_VRAM + 0x40000), 0x20000);
        break;
    case GX_DISPMODE_VRAM_D:
        MI_CpuClearFast((void *)(HW_LCDC_VRAM + 0x60000), 0x20000);
        break;
    default:
        GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
        break;
    }
    DestroySysTaskAndEnvironment((*capture)->task);
    *capture = NULL;
}

void FieldCapture_Task(SysTask *task, FieldCapture *capture) {
    if (capture->started) {
        FieldCapture_SetCaptureRegister(capture->config.size, capture->config.mode, capture->config.srcA, capture->config.srcB, capture->config.dest, capture->config.eva, capture->config.evb);
    }
}

// Runs once on the VBlank queue: switches the display to the capture bank.
void FieldCapture_StartTask(SysTask *task, FieldCapture *capture) {
    switch (capture->config.dispMode) {
    case GX_DISPMODE_VRAM_A:
        GX_SetBankForLCDC(GX_VRAM_LCDC_A);
        break;
    case GX_DISPMODE_VRAM_B:
        GX_SetBankForLCDC(GX_VRAM_LCDC_B);
        break;
    case GX_DISPMODE_VRAM_C:
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        break;
    case GX_DISPMODE_VRAM_D:
        GX_SetBankForLCDC(GX_VRAM_LCDC_D);
        break;
    default:
        GX_SetBankForLCDC(GX_VRAM_LCDC_NONE);
        break;
    }
    GX_SetGraphicsMode(capture->config.dispMode, capture->config.bgMode, capture->config.bg0As);
    capture->started = TRUE;
    SysTask_Destroy(task);
}

void FieldCapture_Init(FieldCaptureConfig *config) {
    switch (config->dispMode) {
    case GX_DISPMODE_VRAM_A:
        MI_CpuClearFast((void *)HW_LCDC_VRAM, 0x20000);
        break;
    case GX_DISPMODE_VRAM_B:
        MI_CpuClearFast((void *)(HW_LCDC_VRAM + 0x20000), 0x20000);
        break;
    case GX_DISPMODE_VRAM_C:
        MI_CpuClearFast((void *)(HW_LCDC_VRAM + 0x40000), 0x20000);
        break;
    case GX_DISPMODE_VRAM_D:
        MI_CpuClearFast((void *)(HW_LCDC_VRAM + 0x60000), 0x20000);
        break;
    }
    FieldCapture_SetCaptureRegister(config->size, config->mode, config->srcA, config->srcB, config->dest, 16, 0);
}

// Hides BG1-3 and blends each frame with the previous capture (eva: live picture, evb: capture).
FieldMotionBlur *FieldMotionBlur_Start(int eva, int evb) {
    FieldMotionBlur *motionBlur;
    FieldCaptureConfig config;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GX_ResetBankForBG();
    motionBlur = Heap_Alloc(HEAP_ID_FIELD1, sizeof(FieldMotionBlur));
    MI_CpuClear32(motionBlur, sizeof(FieldMotionBlur));
    config = sMotionBlurConfig;
    config.eva = eva;
    config.evb = evb;
    motionBlur->capture = FieldCapture_Start(&config);
    return motionBlur;
}

void FieldMotionBlur_Stop(FieldMotionBlur **motionBlur) {
    FieldCapture_Stop(&(*motionBlur)->capture, GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    Heap_FreeExplicit(HEAP_ID_FIELD1, *motionBlur);
}
