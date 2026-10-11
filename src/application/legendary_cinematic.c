#include "legendary_cinematic.h"

#include "global.h"

#include "constants/mmodel.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "field/model_attributes.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "party.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sound.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "unk_02013FDC.h"
#include "unk_02026E30.h"
#include "legendary_cinematic_manager.h"

#define LEGENDARY_CINEMATIC_MAX_MODELS  8
#define LEGENDARY_CINEMATIC_MODEL_ANIMS 4
#define ARCEUS_CINEMATIC_BG_PALETTES    3
#define MODEL_ANIM_NONE                 0xFF

typedef struct LegendaryCinematicModel {
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *resMdl;
    NNSG3dResFileHeader *fileHeader;
    void *anmData[LEGENDARY_CINEMATIC_MODEL_ANIMS];
    NNSG3dAnmObj *anmObj[LEGENDARY_CINEMATIC_MODEL_ANIMS];
} LegendaryCinematicModel;

// A model of NARC_a_2_4_3 (Ho-Oh) / NARC_a_2_4_4 (Lugia) with its animations (MODEL_ANIM_NONE: unused slot).
typedef struct LegendaryCinematicModelSpec {
    u32 model;
    u8 anims[LEGENDARY_CINEMATIC_MODEL_ANIMS];
    u32 noMaterialColors;
} LegendaryCinematicModelSpec;

typedef struct LegendaryCinematicCamera {
    CameraParam param;
    VecFx32 target;
    fx32 nearClip;
    fx32 farClip;
} LegendaryCinematicCamera;

// Display capture into a VRAM bank (blended with the live picture: motion blur).
typedef struct LegendaryCinematicCaptureConfig {
    GXDispMode dispMode; // a VRAM display mode: the bank that receives the capture
    GXBGMode bgMode;
    GXBG0As bg0As;
    GXCaptureSize size;
    GXCaptureMode mode;
    GXCaptureSrcA srcA;
    GXCaptureSrcB srcB;
    GXCaptureDest dest;
    int eva;
    int evb;
    enum HeapID heapID;
} LegendaryCinematicCaptureConfig;

typedef struct LegendaryCinematicCapture {
    u32 savedLcdcBanks;
    LegendaryCinematicCaptureConfig config;
    BOOL started;
    SysTask *task;
} LegendaryCinematicCapture;

typedef struct ArceusBgPalette {
    u8 data[0x2000];
    u32 size;
} ArceusBgPalette;

typedef struct ArceusCinematic {
    BgConfig *bgConfig;
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[1];
    const int *scriptPos;
    u16 savedBgm;
    int blendPlane1;
    int blendStartEv;
    int blendPlane2;
    int blendStartEv2;
    int blendTargetEv;
    int blendDuration;
    ArceusBgPalette bgPalettes[ARCEUS_CINEMATIC_BG_PALETTES];
} ArceusCinematic;

typedef struct LegendaryCinematic {
    LegendaryCinematicArgs *args;
    LegendaryCinematicCapture *capture;
    NNSFndAllocator allocator;
    Camera *camera;
    LegendaryCinematicModel models[LEGENDARY_CINEMATIC_MAX_MODELS];
    u32 numModels;
    VecFx32 cameraTarget;
    u32 state;
    u32 nextState;
    u32 frame;
    ArceusCinematic *arceus;
} LegendaryCinematic;

// Arceus script commands: an opcode followed by its arguments. A command returns one of these.
enum ArceusScriptResult {
    ARCEUS_SCRIPT_WAIT,     // run the same command again next frame
    ARCEUS_SCRIPT_CONTINUE, // go to the next command now
    ARCEUS_SCRIPT_YIELD,    // go to the next command next frame
};

#define ARCEUS_SCRIPT_END 19

typedef int (*ArceusScriptCmd)(LegendaryCinematic *cinematic, const int *cmd);

extern const VecFx32 _021E6D70;
extern const GraphicsModes ov106_021E6D7C;
extern const MtxFx33 ov106_021E6D8C;
extern const GraphicsBanks ov106_021E6DB0;
extern const LegendaryCinematicCaptureConfig ov106_021E6DD8;
extern const GXRgb ov106_021E6E04[8];
extern const LegendaryCinematicModelSpec ov106_021E6E2C[];
extern const LegendaryCinematicModelSpec ov106_021E6E50[];
extern const LegendaryCinematicCamera ov106_021E6E74;
extern const LegendaryCinematicCamera ov106_021E6E9C;
extern const GXRgb ov106_021E6EC4[8];
extern const LegendaryCinematicModelSpec ov106_021E6EE4[];
extern const LegendaryCinematicCamera ov106_021E6EFC;
extern const GraphicsModes ov106_021E6F24;
extern const OamCharTransferParam ov106_021E6F48;
extern const SpriteResourceCountsListUnion ov106_021E6F5C;
extern const BgTemplate ov106_021E6F74;
extern const BgTemplate ov106_021E6F90;
extern const BgTemplate ov106_021E6FAC;
extern const OamManagerParam ov106_021E6FC8;
extern const GraphicsBanks ov106_021E6FE8;
extern const ManagedSpriteTemplate ov106_021E7010;
extern const ArceusScriptCmd ov106_021E7044[];
extern const int ov106_021E7090[];
extern const int ov106_021E70E0[];
extern int (*const sScriptCinematicSubs_HoOh[])(LegendaryCinematic *);
extern int (*const sScriptCinematicSubs_Lugia[])(LegendaryCinematic *);
extern int (*const sScriptCinematicSubs_Arceus[])(LegendaryCinematic *);

// declared only by the NitroSDK internal headers, which clash with lib/include
u32 GX_GetBankForLCDC(void);
// defined in sound_se.c, not declared by a header yet
BOOL PlaySE_SetVolume(u16 seqNo, u16 volume);
// sound_02004A44.h declares the volume as u16, but LugiaCinematic_Play passes it untruncated
u16 GF_GetCurrentPlayingBGM();
void GF_SetVolumeBySeqNo(u16 seqNo, int volume);

static inline void LegendaryCinematic_SetCaptureRegister(GXCaptureSize size, GXCaptureMode mode, GXCaptureSrcA srcA, GXCaptureSrcB srcB, GXCaptureDest dest, int eva, int evb) {
    reg_GX_DISPCAPCNT = REG_GX_DISPCAPCNT_FIELD(1, mode, 0, srcB, srcA, size, 0, dest, evb, eva);
}

void LegendaryCinematic_Init3D(LegendaryCinematic *cinematic);
void LegendaryCinematic_Free3D(LegendaryCinematic *cinematic);
void LegendaryCinematic_FadeInFromBlack(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState);
void LegendaryCinematic_FadeOutToBlack(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState);
void LegendaryCinematic_FadeInFromWhite(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState);
void LegendaryCinematic_FadeOutToWhite(LegendaryCinematic *cinematic, int steps, int framesPerStep, u32 nextState);
void LegendaryCinematic_SetCamera(LegendaryCinematic *cinematic, const LegendaryCinematicCamera *camera);
void LegendaryCinematic_SetLights(const ModelAttributes *attributes);
void LegendaryCinematic_SetEdgeMarking(const GXRgb *colorTable);
void LegendaryCinematic_LoadModels(LegendaryCinematic *cinematic, NarcId narcId, const LegendaryCinematicModelSpec *specs, u32 count);
void LegendaryCinematic_FreeModels(LegendaryCinematic *cinematic);
void LegendaryCinematic_DrawModels(LegendaryCinematic *cinematic);
BOOL LegendaryCinematic_IsModelAnimating(LegendaryCinematic *cinematic, u32 index);
BOOL LegendaryCinematic_AreModelsAnimating(LegendaryCinematic *cinematic);
void LegendaryCinematic_SetBanks(void);
void LegendaryCinematic_SetGraphicsModes(void);
void LegendaryCinematic_InitCameraAndLights(LegendaryCinematic *cinematic);
void LegendaryCinematic_FreeCamera(LegendaryCinematic *cinematic);
LegendaryCinematicCapture *LegendaryCinematic_StartCapture(const LegendaryCinematicCaptureConfig *config);
void LegendaryCinematic_StopCapture(LegendaryCinematicCapture *capture, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0As);
void LegendaryCinematic_CaptureTask(SysTask *task, LegendaryCinematicCapture *capture);
void LegendaryCinematic_CaptureStartTask(SysTask *task, LegendaryCinematicCapture *capture);
void LegendaryCinematic_InitCapture(LegendaryCinematicCaptureConfig *config);
LegendaryCinematicCapture *LegendaryCinematic_StartMotionBlur(int eva, int evb);
void LegendaryCinematic_StopMotionBlur(LegendaryCinematicCapture *capture);
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
void ArceusCinematic_VBlank(ArceusCinematic *arceus);
void ArceusCinematic_SetBanks(void);
void ArceusCinematic_InitBgs(ArceusCinematic *arceus);
void ArceusCinematic_FreeBgs(ArceusCinematic *arceus);
void ArceusCinematic_InitSprites(LegendaryCinematic *cinematic);
void ArceusCinematic_DeleteSprite(ArceusCinematic *arceus, int index);
void ArceusCinematic_FreeSprites(ArceusCinematic *arceus);
void ArceusCinematic_UpdateSprites(ArceusCinematic *arceus);
void ArceusCinematic_LoadObjChar(void *src, u32 offset, u32 size);
void ArceusCinematic_LoadObjPltt(void *src, u32 offset, u32 size);
int ArceusCinematic_GetOverworldModel(int species, int form, int gender);
void ArceusCinematic_LoadArceusSprite(LegendaryCinematic *cinematic);
int ArceusCinematic_Init(LegendaryCinematic *cinematic);
int ArceusCinematic_Exit(LegendaryCinematic *cinematic);
int ArceusCinematic_WaitFade(LegendaryCinematic *cinematic);
int ArceusCinematic_Blend(LegendaryCinematic *cinematic);
int ArceusCinematic_RunScript(LegendaryCinematic *cinematic);
void ArceusCinematic_LoadBgPalette(ArceusCinematic *arceus, int bgId, int fileId);
void ArceusCinematic_TransferBgPalettes(ArceusCinematic *arceus);
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
    GfGfx_EngineASetPlanes(0);
    GfGfx_EngineBSetPlanes(0);
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

#pragma force_active on
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
#pragma force_active reset

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
    MtxFx33 rotation = ov106_021E6D8C;
    VecFx32 scale = _021E6D70;
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

BOOL LegendaryCinematic_IsModelAnimating(LegendaryCinematic *cinematic, u32 index) {
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

void LegendaryCinematic_SetBanks(void) {
    GfGfx_SetBanks(&ov106_021E6DB0);
}

void LegendaryCinematic_SetGraphicsModes(void) {
    GraphicsModes modes = ov106_021E6D7C;
    SetBothScreensModesAndDisable(&modes);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
}

void LegendaryCinematic_InitCameraAndLights(LegendaryCinematic *cinematic) {
    ModelAttributes *attributes;
    cinematic->camera = Camera_New(HEAP_ID_153);
    GF3dRender_InitSimpleManager(HEAP_ID_153);
    G3X_AntiAlias(TRUE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(31, 0, 0), 0, 0x7FFF, 63, FALSE);
    attributes = (ModelAttributes *)&cinematic->args->unk4;
    NNS_G3dGlbMaterialColorDiffAmb(attributes->diffuse, attributes->ambient, attributes->setDiffuseColorAsVertexColor);
    attributes = (ModelAttributes *)&cinematic->args->unk4;
    NNS_G3dGlbMaterialColorSpecEmi(attributes->specular, attributes->emission, attributes->enableSpecularReflectShininessTable);
    attributes = (ModelAttributes *)&cinematic->args->unk4;
    NNS_G3dGlbPolygonAttr(attributes->lightMask, attributes->polygonMode, attributes->cullMode, attributes->id, attributes->alpha, attributes->miscFlags);
    SetBgPriority(GF_BG_LYR_MAIN_0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
}

void LegendaryCinematic_FreeCamera(LegendaryCinematic *cinematic) {
    GF3dRender_DeleteSimpleManager();
    Camera_Delete(cinematic->camera);
}

LegendaryCinematicCapture *LegendaryCinematic_StartCapture(const LegendaryCinematicCaptureConfig *config) {
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

void LegendaryCinematic_StopCapture(LegendaryCinematicCapture *capture, GXDispMode dispMode, GXBGMode bgMode, GXBG0As bg0As) {
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

void LegendaryCinematic_CaptureTask(SysTask *task, LegendaryCinematicCapture *capture) {
    if (capture->started) {
        LegendaryCinematic_SetCaptureRegister(capture->config.size, capture->config.mode, capture->config.srcA, capture->config.srcB, capture->config.dest, capture->config.eva, capture->config.evb);
    }
}

void LegendaryCinematic_CaptureStartTask(SysTask *task, LegendaryCinematicCapture *capture) {
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

void LegendaryCinematic_InitCapture(LegendaryCinematicCaptureConfig *config) {
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
    config = ov106_021E6DD8;
    config.eva = eva;
    config.evb = evb;
    return LegendaryCinematic_StartCapture(&config);
}

void LegendaryCinematic_StopMotionBlur(LegendaryCinematicCapture *capture) {
    LegendaryCinematic_StopCapture(capture, GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
}

int ScriptCinematic_HoOh(void *data) {
    LegendaryCinematic *cinematic = data;
    cinematic->state = sScriptCinematicSubs_HoOh[cinematic->state](cinematic);
    if (cinematic->state == 6) {
        return FALSE;
    }
    LegendaryCinematic_DrawModels(cinematic);
    return TRUE;
}

int HoOhCinematic_Init(LegendaryCinematic *cinematic) {
    u32 variant;
    LegendaryCinematic_Init3D(cinematic);
    LegendaryCinematic_SetLights((ModelAttributes *)&cinematic->args->unk4);
    LegendaryCinematic_SetEdgeMarking(ov106_021E6E04);
    LegendaryCinematic_SetCamera(cinematic, &ov106_021E6E74);
    variant = cinematic->args->unk4E;
    LegendaryCinematic_LoadModels(cinematic, NARC_a_2_4_3, &ov106_021E6E50[variant], 1);
    LegendaryCinematic_FadeInFromWhite(cinematic, 6, 1, 3);
    return 2;
}

int HoOhCinematic_Exit(LegendaryCinematic *cinematic) {
    LegendaryCinematic_FreeModels(cinematic);
    LegendaryCinematic_Free3D(cinematic);
    return 6;
}

int HoOhCinematic_WaitFade(LegendaryCinematic *cinematic) {
    if (IsPaletteFadeFinished() == TRUE) {
        return cinematic->nextState;
    }
    return 2;
}

int HoOhCinematic_PlayFirstScene(LegendaryCinematic *cinematic) {
    u32 frame;
    if (LegendaryCinematic_AreModelsAnimating(cinematic) == FALSE && IsPaletteFadeFinished() == TRUE) {
        cinematic->frame = 0;
        return 4;
    }
    frame = cinematic->frame;
    if (frame == 0) {
        PlaySE(SEQ_SE_GS_SUZUNOONPA_APRI);
    } else if (frame == 40) {
        PlaySE(SEQ_SE_GS_HAKKOUTAI_SYUTUGEN);
    } else if (frame == 78) {
        sub_02005E44(SEQ_GS_E_HOUOU);
        PlaySE(SEQ_SE_GS_HAKKOUTAI_HIROGARI2);
    } else if (frame == 98) {
        PlaySE(SEQ_SE_GS_HAKKOUTAI_HIROGARI);
    } else if (frame == 129) {
        LegendaryCinematic_FadeOutToBlack(cinematic, 6, 1, 3);
    }
    cinematic->frame++;
    return 3;
}

int HoOhCinematic_LoadSecondScene(LegendaryCinematic *cinematic) {
    u32 variant;
    LegendaryCinematic_FreeModels(cinematic);
    LegendaryCinematic_SetCamera(cinematic, &ov106_021E6E9C);
    variant = cinematic->args->unk4E;
    LegendaryCinematic_LoadModels(cinematic, NARC_a_2_4_3, &ov106_021E6E2C[variant], 1);
    LegendaryCinematic_FadeInFromBlack(cinematic, 6, 1, 5);
    return 2;
}

int HoOhCinematic_PlaySecondScene(LegendaryCinematic *cinematic) {
    if (LegendaryCinematic_AreModelsAnimating(cinematic) == FALSE && IsPaletteFadeFinished() == TRUE) {
        cinematic->frame = 0;
        return 1;
    }
    if ((cinematic->frame & 0x3F) == 0) {
        PlaySE(SEQ_SE_GS_HOUOUHABATAKI_APRI);
    }
    if (cinematic->frame == 190) {
        LegendaryCinematic_FadeOutToWhite(cinematic, 10, 1, 1);
    }
    cinematic->frame++;
    return 5;
}

int ScriptCinematic_Lugia(void *data) {
    LegendaryCinematic *cinematic = data;
    cinematic->state = sScriptCinematicSubs_Lugia[cinematic->state](cinematic);
    if (cinematic->state == 4) {
        return FALSE;
    }
    LegendaryCinematic_DrawModels(cinematic);
    return TRUE;
}

#pragma force_active on
int LugiaCinematic_Init(LegendaryCinematic *cinematic) {
    LegendaryCinematic_Init3D(cinematic);
    LegendaryCinematic_SetCamera(cinematic, &ov106_021E6EFC);
    LegendaryCinematic_SetLights((ModelAttributes *)&cinematic->args->unk4);
    LegendaryCinematic_SetEdgeMarking(ov106_021E6EC4);
    LegendaryCinematic_LoadModels(cinematic, NARC_a_2_4_4, ov106_021E6EE4, 2);
    LegendaryCinematic_FadeInFromBlack(cinematic, 6, 1, 3);
    return 2;
}
#pragma force_active reset

int LugiaCinematic_Exit(LegendaryCinematic *cinematic) {
    LegendaryCinematic_FreeModels(cinematic);
    LegendaryCinematic_Free3D(cinematic);
    return 4;
}

int LugiaCinematic_WaitFade(LegendaryCinematic *cinematic) {
    if (IsPaletteFadeFinished() == TRUE) {
        return cinematic->nextState;
    }
    return 2;
}

#pragma force_active on
int LugiaCinematic_Play(LegendaryCinematic *cinematic) {
    if (LegendaryCinematic_AreModelsAnimating(cinematic) == FALSE && IsPaletteFadeFinished() == TRUE) {
        cinematic->frame = 0;
        StopSE(SEQ_SE_GS_TAKI_RUGIADEMO, 0);
        LegendaryCinematic_StopMotionBlur(cinematic->capture);
        return 1;
    }
    if (cinematic->frame == 0) {
        PlaySE_SetVolume(SEQ_SE_GS_TAKI_RUGIADEMO, 0);
    } else if (cinematic->frame == 115) {
        PlaySE(SEQ_SE_GS_RUGIA_TAKISHUTUGEN);
    } else if (cinematic->frame == 145) {
        PlayCry(SPECIES_LUGIA, 0);
        cinematic->capture = LegendaryCinematic_StartMotionBlur(5, 13);
    } else if (cinematic->frame == 232) {
        LegendaryCinematic_FadeOutToBlack(cinematic, 6, 1, 1);
    }
    // the waterfall sound fades in over the first ten frames
    if (cinematic->frame != 0) {
        if (cinematic->frame == 10) {
            GF_SetVolumeBySeqNo(SEQ_SE_GS_TAKI_RUGIADEMO, 30);
        } else if (cinematic->frame < 10) {
            GF_SetVolumeBySeqNo(SEQ_SE_GS_TAKI_RUGIADEMO, cinematic->frame * 0x300 / 0x100);
        }
    }
    cinematic->frame++;
    return 3;
}
#pragma force_active reset

int ScriptCinematic_Arceus(void *data) {
    LegendaryCinematic *cinematic = data;
    ArceusCinematic *arceus;
    cinematic->state = sScriptCinematicSubs_Arceus[cinematic->state](cinematic);
    if (cinematic->state == 5) {
        return FALSE;
    }
    arceus = cinematic->arceus;
    ArceusCinematic_UpdateSprites(arceus);
    SpriteSystem_DrawSprites(arceus->spriteManager);
    return TRUE;
}

void ArceusCinematic_VBlank(ArceusCinematic *arceus) {
    ArceusCinematic_TransferBgPalettes(arceus);
    DoScheduledBgGpuUpdates(arceus->bgConfig);
    SpriteSystem_TransferOam();
    *(u32 *)((u8 *)OS_IRQTable + 0x3FF8) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

void ArceusCinematic_SetBanks(void) {
    GfGfx_SetBanks(&ov106_021E6FE8);
}

#pragma force_active on
void ArceusCinematic_InitBgs(ArceusCinematic *arceus) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;

    arceus->bgConfig = BgConfig_Alloc(HEAP_ID_153);
    modes = ov106_021E6F24;
    SetBothScreensModesAndDisable(&modes);
    bg0 = ov106_021E6F74;
    InitBgFromTemplate(arceus->bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(arceus->bgConfig, GF_BG_LYR_MAIN_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 64, 0, HEAP_ID_153);
    bg1 = ov106_021E6F90;
    InitBgFromTemplate(arceus->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(arceus->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 64, 0, HEAP_ID_153);
    bg2 = ov106_021E6FAC;
    InitBgFromTemplate(arceus->bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(arceus->bgConfig, GF_BG_LYR_MAIN_2);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 64, 0, HEAP_ID_153);
    BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
}
#pragma force_active reset

void ArceusCinematic_FreeBgs(ArceusCinematic *arceus) {
    FreeBgTilemapBuffer(arceus->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(arceus->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(arceus->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(arceus->bgConfig);
}

#pragma force_active on
void ArceusCinematic_InitSprites(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion resourceCounts;

    arceus->spriteSystem = SpriteSystem_Alloc(HEAP_ID_153);
    arceus->spriteManager = SpriteManager_New(arceus->spriteSystem);
    oamManagerParam = ov106_021E6FC8;
    oamCharTransferParam = ov106_021E6F48;
    SpriteSystem_Init(arceus->spriteSystem, &oamManagerParam, &oamCharTransferParam, 32);
    resourceCounts = ov106_021E6F5C;
    SpriteSystem_InitSprites(arceus->spriteSystem, arceus->spriteManager, 1);
    SpriteSystem_InitManagerWithCapacities(arceus->spriteSystem, arceus->spriteManager, &resourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arceus->spriteSystem), 0, FX32_CONST(1024));
    SpriteSystem_LoadCellResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 205, TRUE, 51433);
    SpriteSystem_LoadAnimResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 206, TRUE, 51433);
    SpriteSystem_LoadPlttResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 207, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 51433);
    SpriteSystem_LoadCharResObjFromOpenNarc(arceus->spriteSystem, arceus->spriteManager, arceus->narc, 204, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 51433);
    arceus->sprites[0] = SpriteSystem_NewSprite(arceus->spriteSystem, arceus->spriteManager, &ov106_021E7010);
    ManagedSprite_SetDrawFlag(arceus->sprites[0], FALSE);
    ArceusCinematic_LoadArceusSprite(cinematic);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void ArceusCinematic_DeleteSprite(ArceusCinematic *arceus, int index) {
    if (arceus->sprites[index] != NULL) {
        Sprite_DeleteAndFreeResources(arceus->sprites[index]);
        arceus->sprites[index] = NULL;
    }
}
#pragma force_active reset

void ArceusCinematic_FreeSprites(ArceusCinematic *arceus) {
    ArceusCinematic_DeleteSprite(arceus, 0);
    SpriteSystem_FreeResourcesAndManager(arceus->spriteSystem, arceus->spriteManager);
    SpriteSystem_Free(arceus->spriteSystem);
}

void ArceusCinematic_UpdateSprites(ArceusCinematic *arceus) {
    if (arceus->sprites[0] != NULL) {
        ManagedSprite_TickFrame(arceus->sprites[0]);
    }
}

void ArceusCinematic_LoadObjChar(void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    GX_LoadOBJ(src, offset, size);
}

void ArceusCinematic_LoadObjPltt(void *src, u32 offset, u32 size) {
    DC_FlushRange(src, size);
    GX_LoadOBJPltt(src, offset, size);
}

int ArceusCinematic_GetOverworldModel(int species, int form, int gender) {
    int model;
    if (species <= 0 || species > NATIONAL_DEX_COUNT) {
        model = MMODEL_BABYGIRL1;
    } else {
        model = SpeciesToOverworldModelIndexOffset(species) + MMODEL_FOLLOWER_MON_BULBASAUR;
        if (OverworldModelLookupHasFemaleForm(species)) {
            if (gender == MON_FEMALE) {
                model++;
            }
        } else {
            if (form > OverworldModelLookupFormCount(species)) {
                form = 0;
            }
            model += form;
        }
    }
    return model;
}

#pragma force_active on
// The Arceus sprite uses the overworld texture of the party's lead Arceus (its form and shininess),
// or of a genderless normal Arceus when the lead is something else.
void ArceusCinematic_LoadArceusSprite(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    Party *party = SaveArray_Party_Get(cinematic->args->saveData);
    u16 gender;
    u16 form;
    int shiny;
    NNSG3dResTex *tex;
    void *file;
    u8 *src;
    void *buffer;
    u32 dest;
    u32 i;

    if (!Party_GetCount(party)) {
        gender = MON_GENDERLESS;
        form = 0;
        shiny = FALSE;
    } else {
        Pokemon *mon = Party_GetMonByIndex(party, 0);
        if (GetMonData(mon, MON_DATA_SPECIES, NULL) == SPECIES_ARCEUS) {
            gender = (u16)GetMonData(mon, MON_DATA_GENDER, NULL);
            form = (u16)GetMonData(mon, MON_DATA_FORM, NULL);
            shiny = MonIsShiny(mon);
        } else {
            gender = MON_GENDERLESS;
            form = 0;
            shiny = FALSE;
        }
    }
    file = AllocAndReadWholeNarcMemberByIdPair(NARC_data_mmodel_mmodel, ArceusCinematic_GetOverworldModel(SPECIES_ARCEUS, form, gender), HEAP_ID_153);
    tex = NNS_G3dGetTex(file);
    dest = NNS_G2dGetImageLocation(Sprite_GetImageProxy(arceus->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    src = (u8 *)tex + tex->texInfo.ofsTex;
    buffer = Heap_Alloc(HEAP_ID_153, 0x800);
    for (i = 0; i < 8; i++) {
        sub_020145B4(src, 8, 0, 0, 8, 8, buffer);
        ArceusCinematic_LoadObjChar(buffer, dest, 0x800);
        src += 0x800;
        dest += 0x800;
    }
    Heap_Free(buffer);
    dest = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(arceus->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    src = (u8 *)tex + tex->plttInfo.ofsPlttData;
    if (shiny == TRUE) {
        src += 0x20;
    }
    ArceusCinematic_LoadObjPltt(src, dest, 0x20);
    Heap_Free(file);
}

int ArceusCinematic_Init(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0);
    GfGfx_EngineBSetPlanes(0);
    G2_BlendNone();
    G2S_BlendNone();
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    cinematic->arceus = Heap_Alloc(HEAP_ID_153, sizeof(ArceusCinematic));
    MI_CpuFill8(cinematic->arceus, 0, sizeof(ArceusCinematic));
    arceus = cinematic->arceus;
    arceus->scriptPos = ov106_021E70E0;
    arceus->narc = NARC_New(NARC_a_2_4_5, HEAP_ID_153);
    ArceusCinematic_SetBanks();
    ArceusCinematic_InitBgs(arceus);
    ArceusCinematic_InitSprites(cinematic);
    Main_SetVBlankIntrCB((GFIntrCB)ArceusCinematic_VBlank, cinematic->arceus);
    return 4;
}
#pragma force_active reset

int ArceusCinematic_Exit(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    Main_SetVBlankIntrCB(NULL, NULL);
    ArceusCinematic_FreeSprites(arceus);
    ArceusCinematic_FreeBgs(arceus);
    NARC_Delete(arceus->narc);
    Heap_Free(cinematic->arceus);
    return 5;
}

int ArceusCinematic_WaitFade(LegendaryCinematic *cinematic) {
    if (IsPaletteFadeFinished() == TRUE) {
        return cinematic->nextState;
    }
    return 2;
}

#pragma force_active on
// Moves the first blend target's EV from blendStartEv to blendTargetEv over blendDuration frames
// (the second target gets the complement to 15).
int ArceusCinematic_Blend(LegendaryCinematic *cinematic) {
    struct {
        volatile int targetEv;
        volatile int frame;
    } local;
    ArceusCinematic *arceus = cinematic->arceus;
    int frame = cinematic->frame;
    int duration = arceus->blendDuration;
    int ev;

    local.frame = frame;
    if (frame == duration) {
        ev = arceus->blendTargetEv;
        G2_SetBlendAlpha(arceus->blendPlane1, arceus->blendPlane2, ev, abs(15 - ev));
        cinematic->frame = 0;
        return cinematic->nextState;
    } else {
        int targetEv = arceus->blendTargetEv;
        local.targetEv = targetEv;
        int startEv = arceus->blendStartEv;
        u32 delta = (abs(startEv - targetEv) << 8) / duration;
        delta = (local.frame * delta) >> 8;
        if (startEv > local.targetEv) {
            ev = startEv - delta;
        } else {
            ev = delta + startEv;
        }
        G2_SetBlendAlpha(arceus->blendPlane1, arceus->blendPlane2, ev, abs(15 - ev));
        cinematic->frame++;
        return 3;
    }
}

int ArceusCinematic_RunScript(LegendaryCinematic *cinematic) {
    ArceusCinematic *arceus = cinematic->arceus;
    while (TRUE) {
        const int *cmd = arceus->scriptPos;
        int opcode = *cmd;
        int result;
        if (opcode == ARCEUS_SCRIPT_END) {
            return 1;
        }
        result = ov106_021E7044[opcode](cinematic, cmd);
        if (result == ARCEUS_SCRIPT_WAIT) {
            break;
        }
        if (result == ARCEUS_SCRIPT_CONTINUE) {
            arceus->scriptPos += ov106_021E7090[*arceus->scriptPos];
            continue;
        }
        if (result == ARCEUS_SCRIPT_YIELD) {
            arceus->scriptPos += ov106_021E7090[*arceus->scriptPos];
            break;
        }
    }
    return cinematic->state;
}
#pragma force_active reset

void ArceusCinematic_LoadBgPalette(ArceusCinematic *arceus, int bgId, int fileId) {
    NNSG2dPaletteData *palette;
    void *buffer;
    ArceusBgPalette *bgPalette;

    buffer = GfGfxLoader_GetPlttDataFromOpenNarc(arceus->narc, fileId + 136, &palette, HEAP_ID_153);
    bgPalette = &arceus->bgPalettes[bgId];
    bgPalette->size = palette->szByte;
    MIi_CpuCopy32(palette->pRawData, (u32 *)bgPalette->data, bgPalette->size);
    Heap_Free(buffer);
}

void ArceusCinematic_TransferBgPalettes(ArceusCinematic *arceus) {
    u32 i;

    for (i = 0; i < ARCEUS_CINEMATIC_BG_PALETTES; i++) {
        if (arceus->bgPalettes[i].size != 0) {
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(arceus->bgPalettes[i].data, i * 0x2000, arceus->bgPalettes[i].size);
            GX_EndLoadBGExtPltt();
            arceus->bgPalettes[i].size = 0;
        }
    }
}

int ArceusScriptCmd_Wait(LegendaryCinematic *cinematic, const int *cmd) {
    u32 frame;
    GF_ASSERT(cmd[0] == 0);
    frame = cinematic->frame;
    if (frame == cmd[1]) {
        cinematic->frame = 0;
        return ARCEUS_SCRIPT_YIELD;
    }
    cinematic->frame = frame + 1;
    return ARCEUS_SCRIPT_WAIT;
}

int ArceusScriptCmd_FadeInFromBlack(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == 1);
    LegendaryCinematic_FadeInFromBlack(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_FadeOutToBlack(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == 2);
    LegendaryCinematic_FadeOutToBlack(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_FadeInFromWhite(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == 3);
    LegendaryCinematic_FadeInFromWhite(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_FadeOutToWhite(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == 4);
    LegendaryCinematic_FadeOutToWhite(cinematic, cmd[1], cmd[2], 4);
    return ARCEUS_SCRIPT_CONTINUE;
}

#pragma force_active on
int ArceusScriptCmd_WaitFade(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == 5);
    cinematic->state = 2;
    cinematic->nextState = 4;
    return ARCEUS_SCRIPT_YIELD;
}
#pragma force_active reset

int ArceusScriptCmd_LoadBg(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 6);
    GfGfxLoader_LoadCharDataFromOpenNarc(arceus->narc, cmd[2] + 68, arceus->bgConfig, (GFBgLayer)cmd[1], 0, 0, TRUE, HEAP_ID_153);
    ArceusCinematic_LoadBgPalette(arceus, cmd[1], cmd[2]);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arceus->narc, cmd[2], arceus->bgConfig, (GFBgLayer)cmd[1], 0, 0, TRUE, HEAP_ID_153);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_ToggleBg(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == 7);
    ToggleBgLayer((u8)cmd[1], (u8)cmd[2]);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_SetBgPriority(LegendaryCinematic *cinematic, const int *cmd) {
    GF_ASSERT(cmd[0] == 8);
    SetBgPriority((u8)cmd[1], (u8)cmd[2]);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_SetBlendPlane(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 9);
    if (cmd[1] == 0) {
        arceus->blendPlane1 = cmd[2];
        arceus->blendStartEv = cmd[3];
    } else {
        arceus->blendPlane2 = cmd[2];
        arceus->blendStartEv2 = cmd[3];
    }
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_StartBlend(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 10);
    arceus->blendTargetEv = cmd[1];
    arceus->blendDuration = cmd[2];
    cinematic->state = 3;
    cinematic->nextState = 4;
    return ARCEUS_SCRIPT_YIELD;
}

int ArceusScriptCmd_ResetBlend(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 11);
    G2_BlendNone();
    arceus->blendPlane1 = 0;
    arceus->blendStartEv = 0;
    arceus->blendPlane2 = 0;
    arceus->blendStartEv2 = 0;
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_SaveBgm(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 12);
    arceus->savedBgm = GF_GetCurrentPlayingBGM();
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_StopBgm(LegendaryCinematic *cinematic, const int *cmd) {
    u16 bgm;
    GF_ASSERT(cmd[0] == 13);
    bgm = GF_GetCurrentPlayingBGM();
    StopBGM(bgm, 0);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_PlayBgm(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 14);
    if (cmd[1] == -1) {
        PlayBGM(arceus->savedBgm);
    } else {
        PlayBGM((u16)cmd[1]);
    }
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_SetSpriteVisible(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 15);
    ManagedSprite_SetDrawFlag(arceus->sprites[0], cmd[1]);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_SetSpritePosition(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 16);
    ManagedSprite_SetPositionXY(arceus->sprites[0], (s16)cmd[1], (s16)cmd[2]);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_SetSpritePriority(LegendaryCinematic *cinematic, const int *cmd) {
    ArceusCinematic *arceus = cinematic->arceus;
    GF_ASSERT(cmd[0] == 17);
    ManagedSprite_SetPriority(arceus->sprites[0], cmd[1]);
    return ARCEUS_SCRIPT_CONTINUE;
}

int ArceusScriptCmd_Nop(void) {
    return ARCEUS_SCRIPT_CONTINUE;
}
