#ifndef POKEHEARTGOLD_LEGENDARY_CINEMATIC_INTERNAL_H
#define POKEHEARTGOLD_LEGENDARY_CINEMATIC_INTERNAL_H

#include "global.h"

#include "camera.h"
#include "legendary_cinematic_manager.h"
#include "sprite_system.h"
#include "sys_task.h"

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
BOOL LegendaryCinematic_AreModelsAnimating(LegendaryCinematic *cinematic);
LegendaryCinematicCapture *LegendaryCinematic_StartMotionBlur(int eva, int evb);
void LegendaryCinematic_StopMotionBlur(LegendaryCinematicCapture *capture);

#endif // POKEHEARTGOLD_LEGENDARY_CINEMATIC_INTERNAL_H
