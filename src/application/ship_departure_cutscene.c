#include "ship_departure_cutscene.h"

#include "global.h"

#include "constants/sndseq.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade.h"
#include "system.h"
#include "sound_playback.h"
#include "swap_3d_buffers.h"
#include "ui_util.h"

#define SHIP_CUTSCENE_NUM_ANIMS  4
#define SHIP_CUTSCENE_NUM_SCENES 1

typedef struct ShipCutsceneModel {
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *resMdl;
    NNSG3dResFileHeader *fileHeader;
    void *anmData[SHIP_CUTSCENE_NUM_ANIMS];
    NNSG3dAnmObj *anmObj[SHIP_CUTSCENE_NUM_ANIMS];
} ShipCutsceneModel;

typedef struct ShipDepartureCutscene {
    Camera *camera;
    ShipCutsceneModel models[SHIP_CUTSCENE_NUM_SCENES];
    NNSFndAllocator allocator;
    VecFx32 target;
    u8 port;
    u8 isNight;
    u8 unk_9E;
    u8 scene;
    u8 scenesPlayed;
    u8 frame;
    u8 unk_A2;
    u8 unk_A3;
    const u32 *sceneSE;
} ShipDepartureCutscene;

// NARC_a_2_3_8 members: the ship model and its four animations, per port and time of day.
typedef struct ShipCutsceneFiles {
    u32 model;
    u16 anims[SHIP_CUTSCENE_NUM_ANIMS];
} ShipCutsceneFiles;

extern const u16 sShipDepartureCutsceneClipPlanes[2][SHIP_CUTSCENE_NUM_SCENES][2];
extern const VecFx32 sShipDepartureCutsceneModelScale;
extern const GXRgb sShipDepartureCutsceneEdgeColors[8];
extern const VecFx32 sShipDepartureCutsceneCameraOffsets[2][SHIP_CUTSCENE_NUM_SCENES];
extern const MtxFx33 sShipDepartureCutsceneModelRotation;
extern const CameraParam sShipDepartureCutsceneCameraParams[2][SHIP_CUTSCENE_NUM_SCENES];
extern const GraphicsBanks sShipDepartureCutsceneGraphicsBanks;
extern const u32 sShipDepartureCutscenePort1SceneSE[SHIP_CUTSCENE_NUM_SCENES];
extern const u32 sShipDepartureCutscenePort0SceneSE[SHIP_CUTSCENE_NUM_SCENES];
extern ShipCutsceneFiles sShipDepartureCutscenePort0NightFiles;
extern ShipCutsceneFiles sShipDepartureCutscenePort0DayFiles;
extern ShipCutsceneFiles sShipDepartureCutscenePort1NightFiles;
extern ShipCutsceneFiles sShipDepartureCutscenePort1DayFiles;

void ShipDepartureCutscene_InitGraphics(void);
void ShipDepartureCutscene_InitCamera(ShipDepartureCutscene *cutscene);
void ShipDepartureCutscene_SetBanks(void);
void ShipDepartureCutscene_LoadModel(ShipDepartureCutscene *cutscene);

BOOL ShipDepartureCutscene_Init(OverlayManager *man, int *state) {
    u8 i;
    ShipDepartureCutsceneArgs *args;
    ShipDepartureCutscene *cutscene;

    Heap_Create(HEAP_ID_3, HEAP_ID_151, 0x31000);
    cutscene = OverlayManager_CreateAndGetData(man, sizeof(ShipDepartureCutscene), HEAP_ID_151);
    memset(cutscene, 0, sizeof(ShipDepartureCutscene));
    args = OverlayManager_GetArgs(man);
    cutscene->port = args->port;
    cutscene->isNight = args->isNight;
    cutscene->unk_9E = 0;
    cutscene->camera = Camera_New(HEAP_ID_151);
    ShipDepartureCutscene_InitGraphics();
    ShipDepartureCutscene_LoadModel(cutscene);
    ShipDepartureCutscene_InitCamera(cutscene);
    for (i = 0; i < 4; i++) {
        NNS_G3dGlbLightVector((GXLightId)i, args->modelAttributes->lightVectors[i].x, args->modelAttributes->lightVectors[i].y, args->modelAttributes->lightVectors[i].z);
        NNS_G3dGlbLightColor((GXLightId)i, args->modelAttributes->lightColors[i]);
    }
    NNS_G3dGlbMaterialColorDiffAmb(args->modelAttributes->diffuse, args->modelAttributes->ambient, args->modelAttributes->setDiffuseColorAsVertexColor);
    NNS_G3dGlbMaterialColorSpecEmi(args->modelAttributes->specular, args->modelAttributes->emission, args->modelAttributes->enableSpecularReflectShininessTable);
    NNS_G3dGlbPolygonAttr(args->modelAttributes->lightMask, args->modelAttributes->polygonMode, args->modelAttributes->cullMode, args->modelAttributes->id, args->modelAttributes->alpha, args->modelAttributes->miscFlags);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_151);
    return TRUE;
}

BOOL ShipDepartureCutscene_Main(OverlayManager *man, int *state) {
    u8 i;
    ShipCutsceneModel *model;
    ShipDepartureCutscene *cutscene;
    NNSG3dAnmObj *anmObj;
    fx32 frame;
    BOOL done;
    MtxFx33 rotation;
    VecFx32 scale;

    cutscene = OverlayManager_GetData(man);
    done = FALSE;
    model = &cutscene->models[cutscene->scene];
    switch (*state) {
    case 0:
        anmObj = model->anmObj[0];
        if (anmObj->frame + FX32_ONE == NNS_G3dAnmObjGetNumFrame(anmObj)) {
            BeginAppScreenFade(1, HEAP_ID_151);
            (*state)++;
        }
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            cutscene->scene++;
            cutscene->scenesPlayed++;
            if (cutscene->scenesPlayed >= SHIP_CUTSCENE_NUM_SCENES) {
                done = TRUE;
            } else {
                ShipDepartureCutscene_InitCamera(cutscene);
                BeginAppScreenFade(done, HEAP_ID_151);
                *state = done;
            }
        }
        break;
    }
    cutscene->frame++;
    if (cutscene->frame == 30) {
        PlaySE(cutscene->sceneSE[cutscene->scene]);
    }
    for (i = 0; i < SHIP_CUTSCENE_NUM_ANIMS; i++) {
        anmObj = model->anmObj[i];
        frame = anmObj->frame + FX32_ONE;
        if (frame < NNS_G3dAnmObjGetNumFrame(anmObj)) {
            anmObj->frame = frame;
        }
    }
    rotation = sShipDepartureCutsceneModelRotation;
    scale = sShipDepartureCutsceneModelScale;
    {
        VecFx32 translation = { 0, 0, 0 };
        Thunk_G3X_Reset();
        Camera_PushLookAtToNNSGlb();
        GF3dRender_DrawModel(&model->renderObj, &translation, &rotation, &scale);
        RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_W);
    }
    return done;
}

BOOL ShipDepartureCutscene_Exit(OverlayManager *man, int *state) {
    ShipDepartureCutscene *cutscene;
    ShipCutsceneModel *model;
    u8 i;

    cutscene = OverlayManager_GetData(man);
    model = &cutscene->models[0];
    for (i = 0; i < SHIP_CUTSCENE_NUM_ANIMS; i++) {
        NNS_G3dFreeAnmObj(&cutscene->allocator, model->anmObj[i]);
        Heap_Free(model->anmData[i]);
    }
    Heap_Free(model->fileHeader);
    Camera_Delete(cutscene->camera);
    OverlayManager_FreeData(man);
    GF3dRender_DeleteSimpleManager();
    Heap_Destroy(HEAP_ID_151);
    return TRUE;
}

void ShipDepartureCutscene_InitGraphics(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
    reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
    ShipDepartureCutscene_SetBanks();
    GF3dRender_InitSimpleManager(HEAP_ID_151);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sShipDepartureCutsceneEdgeColors);
    GfGfx_SwapDisplay();
}

void ShipDepartureCutscene_InitCamera(ShipDepartureCutscene *cutscene) {
    u8 scene;
    u8 port;
    VecFx32 target = { 0, 0, 0 };

    cutscene->target = target;
    scene = cutscene->scene;
    port = cutscene->port;
    Camera_Init_FromTargetDistanceAndAngle(&cutscene->target, sShipDepartureCutsceneCameraParams[port][scene].distance, &sShipDepartureCutsceneCameraParams[port][scene].angle, sShipDepartureCutsceneCameraParams[port][scene].perspective, sShipDepartureCutsceneCameraParams[port][scene].perspectiveType, TRUE, cutscene->camera);
    Camera_OffsetLookAtPosAndTarget(&sShipDepartureCutsceneCameraOffsets[cutscene->port][scene], cutscene->camera);
    Camera_SetPerspectiveClippingPlane(sShipDepartureCutsceneClipPlanes[cutscene->port][scene][0] * FX32_ONE, sShipDepartureCutsceneClipPlanes[cutscene->port][scene][1] * FX32_ONE, cutscene->camera);
    Camera_SetStaticPtr(cutscene->camera);
}

void ShipDepartureCutscene_SetBanks(void) {
    GraphicsBanks banks = sShipDepartureCutsceneGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void ShipDepartureCutscene_LoadModel(ShipDepartureCutscene *cutscene) {
    u8 i;
    NNSG3dResTex *tex;
    NARC *narc;
    ShipCutsceneModel *model;
    ShipCutsceneFiles *files;
    void *anm;

    narc = NARC_New(NARC_a_2_3_8, HEAP_ID_151);
    HeapExp_FndInitAllocator(&cutscene->allocator, HEAP_ID_151, 4);
    if (cutscene->port == 0) {
        if (cutscene->isNight == FALSE) {
            files = &sShipDepartureCutscenePort0DayFiles;
        } else {
            files = &sShipDepartureCutscenePort0NightFiles;
        }
        cutscene->sceneSE = sShipDepartureCutscenePort0SceneSE;
    } else {
        if (cutscene->isNight == FALSE) {
            files = &sShipDepartureCutscenePort1DayFiles;
        } else {
            files = &sShipDepartureCutscenePort1NightFiles;
        }
        cutscene->sceneSE = sShipDepartureCutscenePort1SceneSE;
    }
    model = &cutscene->models[0];
    model->fileHeader = NARC_AllocAndReadWholeMember(narc, files->model, HEAP_ID_151);
    GF3dRender_InitObjFromHeader(&model->renderObj, &model->resMdl, &model->fileHeader);
    tex = NNS_G3dGetTex(cutscene->models[0].fileHeader);
    NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_DIFFUSE);
    NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_AMBIENT);
    NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_SPECULAR);
    NNSi_G3dModifyMatFlag(model->resMdl, FALSE, NNS_G3D_MATFLAG_EMISSION);
    for (i = 0; i < SHIP_CUTSCENE_NUM_ANIMS; i++) {
        model->anmData[i] = NARC_AllocAndReadWholeMember(narc, files->anims[i], HEAP_ID_151);
        anm = NNS_G3dGetAnmByIdx(model->anmData[i], 0);
        model->anmObj[i] = NNS_G3dAllocAnmObj(&cutscene->allocator, anm, model->resMdl);
        NNS_G3dAnmObjInit(model->anmObj[i], anm, model->resMdl, tex);
        NNS_G3dRenderObjAddAnmObj(&model->renderObj, model->anmObj[i]);
    }
    NARC_Delete(narc);
}
