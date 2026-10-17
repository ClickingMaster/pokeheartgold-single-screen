#include "legendary_cinematic.h"

#include "global.h"

#include "follow_mon.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "legendary_cinematic_internal.h"
#include "screen_fade.h"
#include "swap_3d_buffers.h"
#include "systask_environment.h"

static inline void LegendaryCinematic_SetCaptureRegister(GXCaptureSize size, GXCaptureMode mode, GXCaptureSrcA srcA, GXCaptureSrcB srcB, GXCaptureDest dest, int eva, int evb);
static BOOL LegendaryCinematic_IsModelAnimating(LegendaryCinematic *cinematic, u32 index);
static void LegendaryCinematic_SetBanks(void);
static void LegendaryCinematic_SetGraphicsModes(void);
static void LegendaryCinematic_InitCameraAndLights(LegendaryCinematic *cinematic);
static void LegendaryCinematic_FreeCamera(LegendaryCinematic *cinematic);
static LegendaryCinematicCapture *LegendaryCinematic_StartCapture(const LegendaryCinematicCaptureConfig *config);
static void LegendaryCinematic_StopCapture(LegendaryCinematicCapture *capture, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0As);
static void LegendaryCinematic_CaptureTask(SysTask *task, LegendaryCinematicCapture *capture);
static void LegendaryCinematic_CaptureStartTask(SysTask *task, LegendaryCinematicCapture *capture);
static void LegendaryCinematic_InitCapture(LegendaryCinematicCaptureConfig *config);

u32 GX_GetBankForLCDC(void);

static const VecFx32 sLegendaryCinematicModelScale = { FX32_ONE, FX32_ONE, FX32_ONE };

static const GraphicsModes sLegendaryCinematicGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const MtxFx33 sLegendaryCinematicModelRotation = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

static const GraphicsBanks sLegendaryCinematicGraphicsBanks = {
    .bg = GX_VRAM_BG_128_C,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_16_F,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_AB,
    .texpltt = GX_VRAM_TEXPLTT_0123_E,
};

static const LegendaryCinematicCaptureConfig sLegendaryCinematicCaptureConfig = {
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
    .heapID = HEAP_ID_153,
};

static inline void LegendaryCinematic_SetCaptureRegister(GXCaptureSize size, GXCaptureMode mode, GXCaptureSrcA srcA, GXCaptureSrcB srcB, GXCaptureDest dest, int eva, int evb) {
    reg_GX_DISPCAPCNT = REG_GX_DISPCAPCNT_FIELD(1, mode, 0, srcB, srcA, size, 0, dest, evb, eva);
}

int HoOhCinematic_Init(LegendaryCinematic *cinematic);
int HoOhCinematic_Exit(LegendaryCinematic *cinematic);
int HoOhCinematic_WaitFade(LegendaryCinematic *cinematic);
int HoOhCinematic_PlayFirstScene(LegendaryCinematic *cinematic);
int HoOhCinematic_LoadSecondScene(LegendaryCinematic *cinematic);
int HoOhCinematic_PlaySecondScene(LegendaryCinematic *cinematic);
int LugiaCinematic_Init(LegendaryCinematic *cinematic);
int LugiaCinematic_Exit(LegendaryCinematic *cinematic);
int LugiaCinematic_WaitFade(LegendaryCinematic *cinematic);
int LugiaCinematic_Play(LegendaryCinematic *cinematic);
int ArceusCinematic_Init(LegendaryCinematic *cinematic);
int ArceusCinematic_Exit(LegendaryCinematic *cinematic);
int ArceusCinematic_WaitFade(LegendaryCinematic *cinematic);
int ArceusCinematic_Blend(LegendaryCinematic *cinematic);
int ArceusCinematic_RunScript(LegendaryCinematic *cinematic);
int ArceusScriptCmd_Wait(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_FadeInFromBlack(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_FadeOutToBlack(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_FadeInFromWhite(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_FadeOutToWhite(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_WaitFade(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_LoadBg(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_ToggleBg(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_SetBgPriority(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_SetBlendPlane(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_StartBlend(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_ResetBlend(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_SaveBgm(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_StopBgm(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_PlayBgm(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_SetSpriteVisible(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_SetSpritePosition(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_SetSpritePriority(LegendaryCinematic *cinematic, const int *cmd);
int ArceusScriptCmd_Nop(void);

void LegendaryCinematic_Init3D(LegendaryCinematic *cinematic) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(GX_PLANEMASK_NONE);
    GfGfx_EngineBSetPlanes(GX_PLANEMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    LegendaryCinematic_SetBanks();
    LegendaryCinematic_SetGraphicsModes();
    LegendaryCinematic_InitCameraAndLights(cinematic);
}

void LegendaryCinematic_Free3D(LegendaryCinematic *cinematic) {
    LegendaryCinematic_FreeCamera(cinematic);
}

void LegendaryCinematic_FadeInFromBlack(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}

void LegendaryCinematic_FadeOutToBlack(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}

void LegendaryCinematic_FadeInFromWhite(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}

void LegendaryCinematic_FadeOutToWhite(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, steps, framesPerStep, HEAP_ID_153);
    cinematic->nextState = nextState;
}

void LegendaryCinematic_SetCamera(LegendaryCinematic *cinematic, const LegendaryCinematicCamera *camera) {
    VecFx32 target = camera->target;
    cinematic->cameraTarget = target;
    Camera_Init_FromTargetDistanceAndAngle(&cinematic->cameraTarget, camera->param.distance, &camera->param.angle, camera->param.perspective, camera->param.perspectiveType, TRUE, cinematic->camera);
    Camera_SetPerspectiveClippingPlane(camera->nearClip, camera->farClip, cinematic->camera);
    Camera_SetStaticPtr(cinematic->camera);
}

void LegendaryCinematic_SetLights(const ModelAttributes *attributes) {
    u32 i = 0;
    const VecFx16 *vec = attributes->lightVectors;
    const GXRgb *color = (const GXRgb *)attributes;
    // the light colors follow the four light vectors: color[12] is lightColors[i]
    while (i < 4) {
        NNS_G3dGlbLightVector((GXLightId)i, vec->x, vec->y, vec->z);
        NNS_G3dGlbLightColor((GXLightId)i, color[12]);
        i++;
        vec++;
        color++;
    }
}

void LegendaryCinematic_SetEdgeMarking(const GXRgb *colorTable) {
    if (colorTable == NULL) {
        G3X_EdgeMarking(FALSE);
    } else {
        G3X_EdgeMarking(TRUE);
        G3X_SetEdgeColorTable(colorTable);
    }
}

void LegendaryCinematic_LoadModels(LegendaryCinematic *cinematic, NarcId narcId, const LegendaryCinematicModelSpec *specs, u32 count) {
    NNSG3dResTex *tex;
    u16 i;
    LegendaryCinematicModel *model;
    u16 j;
    const LegendaryCinematicModelSpec *spec;

    HeapExp_FndInitAllocator(&cinematic->allocator, HEAP_ID_153, 4);
    for (i = 0; i < count; i++) {
        model = &cinematic->models[i];
        spec = &specs[i];
        model->fileHeader = GfGfxLoader_LoadFromNarc(narcId, spec->model, TRUE, HEAP_ID_153, TRUE);
        GF3dRender_InitObjFromHeader(&model->renderObj, &model->resMdl, &model->fileHeader);
        tex = NNS_G3dGetTex(model->fileHeader);
        if (spec->noMaterialColors == TRUE) {
            NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_DIFFUSE);
            NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_AMBIENT);
            NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_SPECULAR);
            NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_EMISSION);
        }
        for (j = 0; j < LEGENDARY_CINEMATIC_MODEL_ANIMS; j++) {
            if (spec->anims[j] == MODEL_ANIM_NONE) {
                model->anmData[j] = NULL;
                model->anmObj[j] = NULL;
            } else {
                void *anm;
                model->anmData[j] = GfGfxLoader_LoadFromNarc(narcId, spec->anims[j], TRUE, HEAP_ID_153, TRUE);
                anm = NNS_G3dGetAnmByIdx(model->anmData[j], 0);
                model->anmObj[j] = NNS_G3dAllocAnmObj(&cinematic->allocator, anm, model->resMdl);
                NNS_G3dAnmObjInit(model->anmObj[j], anm, model->resMdl, tex);
                NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->anmObj[j]);
            }
        }
    }
    cinematic->numModels = count;
}

void LegendaryCinematic_FreeModels(LegendaryCinematic *cinematic) {
    u16 i;
    for (i = 0; i < cinematic->numModels; i++) {
        LegendaryCinematicModel *model = &cinematic->models[i];
        u16 j;
        for (j = 0; j < LEGENDARY_CINEMATIC_MODEL_ANIMS; j++) {
            if (model->anmData[j] != NULL) {
                NNS_G3dFreeAnmObj(&cinematic->allocator, model->anmObj[j]);
                Heap_Free(model->anmData[j]);
            }
        }
        Heap_Free(model->fileHeader);
    }
}

void LegendaryCinematic_DrawModels(LegendaryCinematic *cinematic) {
    MtxFx33 rotation = sLegendaryCinematicModelRotation;
    VecFx32 scale = sLegendaryCinematicModelScale;
    VecFx32 translation = { 0, 0, 0 };
    u32 i;
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    for (i = 0; i < cinematic->numModels; i++) {
        LegendaryCinematicModel *model = &cinematic->models[i];
        u32 j = 0;
        u8 *slot = (u8 *)model;
        // walks the animation slots with a byte pointer
        while (j < LEGENDARY_CINEMATIC_MODEL_ANIMS) {
            NNSG3dAnmObj *anmObj = ((LegendaryCinematicModel *)slot)->anmObj[0];
            if (anmObj != NULL) {
                fx32 frame = anmObj->frame;
                fx32 end = NNS_G3dAnmObjGetNumFrame(anmObj);
                if (frame + FX32_ONE < end) {
                    anmObj->frame = frame + FX32_ONE;
                }
            }
            j++;
            slot += sizeof(NNSG3dAnmObj *);
        }
        GF3dRender_DrawModel(&model->renderObj, &translation, &rotation, &scale);
    }
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

static BOOL LegendaryCinematic_IsModelAnimating(LegendaryCinematic *cinematic, u32 index) {
    u32 offset = sizeof(LegendaryCinematicModel) * index;
    u8 *model = (u8 *)cinematic->models;
    fx32 step;
    u32 i;
    step = FX32_ONE;
    i = 0;
    model += offset;
    // walks the animation slots of models[index] with a byte pointer
    while (i < LEGENDARY_CINEMATIC_MODEL_ANIMS) {
        NNSG3dAnmObj *anmObj = ((LegendaryCinematicModel *)model)->anmObj[0];
        if (anmObj != NULL) {
            fx32 frame = anmObj->frame + step;
            fx32 end = NNS_G3dAnmObjGetNumFrame(anmObj);
            if (frame < end) {
                return TRUE;
            }
        }
        i++;
        model += sizeof(NNSG3dAnmObj *);
    }
    return FALSE;
}

BOOL LegendaryCinematic_AreModelsAnimating(LegendaryCinematic *cinematic) {
    u32 i;
    for (i = 0; i < cinematic->numModels; i++) {
        if (LegendaryCinematic_IsModelAnimating(cinematic, i) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

static void LegendaryCinematic_SetBanks(void) {
    GfGfx_SetBanks(&sLegendaryCinematicGraphicsBanks);
}

static void LegendaryCinematic_SetGraphicsModes(void) {
    GraphicsModes modes = sLegendaryCinematicGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
}

static void LegendaryCinematic_InitCameraAndLights(LegendaryCinematic *cinematic) {
    ModelAttributes *attributes;
    cinematic->camera = Camera_New(HEAP_ID_153);
    GF3dRender_InitSimpleManager(HEAP_ID_153);
    G3X_AntiAlias(TRUE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(31, 0, 0), 0, 0x7FFF, 63, FALSE);
    attributes = (ModelAttributes *)&cinematic->args->modelAttributes;
    NNS_G3dGlbMaterialColorDiffAmb(attributes->diffuse, attributes->ambient, attributes->setDiffuseColorAsVertexColor);
    attributes = (ModelAttributes *)&cinematic->args->modelAttributes;
    NNS_G3dGlbMaterialColorSpecEmi(attributes->specular, attributes->emission, attributes->enableSpecularReflectShininessTable);
    attributes = (ModelAttributes *)&cinematic->args->modelAttributes;
    NNS_G3dGlbPolygonAttr(attributes->lightMask, attributes->polygonMode, attributes->cullMode, attributes->id, attributes->alpha, attributes->miscFlags);
    SetBgPriority(GF_BG_LYR_MAIN_0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
}

static void LegendaryCinematic_FreeCamera(LegendaryCinematic *cinematic) {
    GF3dRender_DeleteSimpleManager();
    Camera_Delete(cinematic->camera);
}

static LegendaryCinematicCapture *LegendaryCinematic_StartCapture(const LegendaryCinematicCaptureConfig *config) {
    SysTask *task = CreateSysTaskAndEnvironment((SysTaskFunc)LegendaryCinematic_CaptureTask, sizeof(LegendaryCinematicCapture), 5, config->heapID);
    LegendaryCinematicCapture *capture = SysTask_GetData(task);
    capture->config = *config;
    capture->task = task;
    capture->started = FALSE;
    capture->savedLcdcBanks = GX_GetBankForLCDC();
    LegendaryCinematic_InitCapture(&capture->config);
    SysTask_CreateOnVWaitQueue((SysTaskFunc)LegendaryCinematic_CaptureStartTask, capture, 0);
    return capture;
}

static void LegendaryCinematic_StopCapture(LegendaryCinematicCapture *capture, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0As) {
    GX_SetGraphicsMode(dispMode, bgMode, bg0As);
    GX_SetBankForLCDC((GXVRamLCDC)capture->savedLcdcBanks);
    switch (capture->config.dispMode) {
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
    DestroySysTaskAndEnvironment(capture->task);
}

static void LegendaryCinematic_CaptureTask(SysTask *task, LegendaryCinematicCapture *capture) {
    if (capture->started) {
        LegendaryCinematic_SetCaptureRegister(capture->config.size, capture->config.mode, capture->config.srcA, capture->config.srcB, capture->config.dest, capture->config.eva, capture->config.evb);
    }
}

static void LegendaryCinematic_CaptureStartTask(SysTask *task, LegendaryCinematicCapture *capture) {
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

static void LegendaryCinematic_InitCapture(LegendaryCinematicCaptureConfig *config) {
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
    LegendaryCinematic_SetCaptureRegister(config->size, config->mode, config->srcA, config->srcB, config->dest, 16, 0);
}

LegendaryCinematicCapture *LegendaryCinematic_StartMotionBlur(int eva, int evb) {
    LegendaryCinematicCaptureConfig config;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GX_ResetBankForBG();
    config = sLegendaryCinematicCaptureConfig;
    config.eva = eva;
    config.evb = evb;
    return LegendaryCinematic_StartCapture(&config);
}

void LegendaryCinematic_StopMotionBlur(LegendaryCinematicCapture *capture) {
    LegendaryCinematic_StopCapture(capture, GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
}
