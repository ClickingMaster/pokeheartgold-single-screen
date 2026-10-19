#ifndef POKEHEARTGOLD_OVERLAY104_SCENE_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY104_SCENE_INTERNAL_H

#include "global.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "system.h"
#include "sound_playback.h"
#include "swap_3d_buffers.h"
#include "ui_util.h"

#define HEAPID(n) ((enum HeapID)(n))

typedef struct MagnetTrainCutsceneAnmHeader {
    u32 unk00;
    u16 numFrames;
} MagnetTrainCutsceneAnmHeader;

typedef struct MagnetTrainCutsceneAnmObj {
    fx32 frame;
    u32 unk04;
    const MagnetTrainCutsceneAnmHeader *anm;
} MagnetTrainCutsceneAnmObj;

typedef struct MagnetTrainCutsceneModel {
    NNSG3dRenderObj obj;
    NNSG3dResMdl *mdl;
    NNSG3dResFileHeader *file;
    void *anmFile[2];
    void *anmObj[2];
} MagnetTrainCutsceneModel;

typedef struct MagnetTrainCutsceneWork {
    Camera *camera;
    MagnetTrainCutsceneModel models[3];
    NNSFndAllocator allocator;
    VecFx32 target;
    u8 variant;
    u8 gender;
    u8 isNight;
    u8 unk167;
    u8 step;
    u8 counter;
    const u32 *seTable;
} MagnetTrainCutsceneWork;

typedef struct MagnetTrainCutsceneLighting {
    VecFx16 lightVec[4];
    GXRgb lightColor[4];
    GXRgb diffuse;
    GXRgb ambient;
    GXRgb specular;
    GXRgb emission;
    BOOL diffAmbFlag;
    BOOL specEmiFlag;
    u32 polygonAttr[6];
} MagnetTrainCutsceneLighting;

typedef struct MagnetTrainCutsceneArgs {
    u8 variant;
    u8 gender;
    u8 isNight;
    u8 padding_03;
    const MagnetTrainCutsceneLighting *lighting;
} MagnetTrainCutsceneArgs;

typedef struct MagnetTrainCutsceneModelIds {
    u32 modelFileId;
    u16 anmFileIds[2];
} MagnetTrainCutsceneModelIds;

#endif
