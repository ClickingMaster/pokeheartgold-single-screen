#include "trade_sequence.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/trade_sequence/trade_sequence_internal.h"
#include "application/trade_sequence/trade_sequence_models.h"
#include "msgdata/msg.naix"

#include "assert.h"
#include "bg_window.h"
#include "camera.h"
#include "char_tiles.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "model_box_test.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "trade_anim.h"
#include "wireless_status_icon.h"

// Trade animation: the app that plays the scenes of a trade (or of receiving a Pokemon), the helpers they share
// (palette pulses, background zoom, Pokemon pictures, sprites) and the scenes' 3D models.

#define TRADE_SEQUENCE_NUM_SCENES       7
#define TRADE_SEQUENCE_MAX_DEFERRED     32
#define TRADE_SEQUENCE_NUM_PULSE_COLORS 0x60
#define TRADE_SEQUENCE_MAX_PULSES       8

struct TradeSequence {
    TRADE_ANIM_WORK *args;    // 0x000
    u32 scene;                // 0x004
    BgConfig *bgConfig;       // 0x008
    MessageFormat *msgFormat; // 0x00C
    MsgData *msgData;         // 0x010
    String *string;           // 0x014
    SpriteList *spriteList;   // 0x018
    G2dRenderer renderer;     // 0x01C
    SysTask *drawSpritesTask; // 0x144
    BOOL receivedMonCantFlip; // 0x148
    u16 sentSpecies;          // 0x14C
    u16 sentForm;             // 0x14E
    u16 receivedSpecies;      // 0x150
    u16 receivedForm;         // 0x152
    void *sceneWork;          // 0x154
}; // size: 0x158

// Light pulses running along the link: each pulse moves one white color through a BG extended palette.
struct TradeSequencePulses {
    u16 colors[TRADE_SEQUENCE_NUM_PULSE_COLORS];    // 0x000
    SysTask *task;                                  // 0x0C0
    SysTask *pulseTasks[TRADE_SEQUENCE_MAX_PULSES]; // 0x0C4
    int *running;                                   // 0x0E4, 1 while the pulses are alive
    int stop;                                       // 0x0E8
    int dirty;                                      // 0x0EC
    int timer;                                      // 0x0F0
    int count;                                      // 0x0F4
    int nextSlot;                                   // 0x0F8
    int direction;                                  // 0x0FC
}; // size: 0x100

typedef struct TradeSequencePulse {
    TradeSequencePulses *pulses; // 0x00
    s32 colorIndex;              // 0x04
    s32 stepFrames;              // 0x08
    s32 frameCounter;            // 0x0C
    s32 slot;                    // 0x10
    s32 direction;               // 0x14
} TradeSequencePulse;            // size: 0x18

// Accelerating affine zoom of BG 2 on both screens.
struct TradeSequenceBgZoom {
    BgConfig *bgConfig; // 0x00
    int *finished;      // 0x04
    fx32 scale;         // 0x08
    fx32 targetScale;   // 0x0C
    fx32 speed;         // 0x10
    fx32 acceleration;  // 0x14
    s32 direction;      // 0x18, 0: grow, 1: shrink
    s32 centerYIndex;   // 0x1C
    SysTask *task;      // 0x20
}; // size: 0x24

typedef void *(*TradeSequenceSceneInitFunc)(TradeSequence *ts);
typedef BOOL (*TradeSequenceSceneMainFunc)(void *sceneWork, int *state);
typedef void (*TradeSequenceSceneExitFunc)(void *sceneWork);

typedef struct TradeSequenceScene {
    TradeSequenceSceneInitFunc init;
    TradeSequenceSceneMainFunc main;
    TradeSequenceSceneExitFunc exit;
    u32 tradeModes; // the trade modes (1: full trade, 2: sending only, 4: receiving only) the scene plays in
} TradeSequenceScene;

BOOL TradeSequence_BoxMonCantFlip(BoxPokemon *boxMon);
void TradeSequence_DrawSpritesTask(SysTask *task, void *data);
void TradeSequence_PulsesTask(SysTask *task, void *data);
void TradeSequence_SetPulseColor(TradeSequencePulses *pulses, int clearIndex, int setIndex, u16 color);
void TradeSequence_StartPulse(TradeSequencePulses *pulses);
void TradeSequence_DestroyPulse(TradeSequencePulses *pulses, int slot);
void TradeSequence_PulseTask(SysTask *task, void *data);
void TradeSequence_BgZoomTask(SysTask *task, void *data);
void TradeSequence_ApplyBgZoom(TradeSequenceBgZoom *zoom);
void TradeSequence_ResetDeferredFrees(void);
void TradeSequence_FlushDeferredFrees(void);

void TradeSequence_DrawModel(TradeSequenceModel *model);
void TradeSequence_InitModel(TradeSequenceModel *model);
void TradeSequence_FreeModel(TradeSequenceModel *model);
void TradeSequence_InitCamera(TradeSequenceModels *models, fx32 targetX, fx32 targetY, fx32 targetZ);
void TradeSequence_FreeCamera(TradeSequenceModels *models);

static const s32 sTradeSequenceBgZoomCenterY[] = { 144, 56 };

static const TileRect sTradeSequenceMonPicSettings = { 0, 0, 10, 10 };

static const TradeSequenceScene sTradeSequenceScenes[TRADE_SEQUENCE_NUM_SCENES] = {
    { TradeSequenceSend_Init,     TradeSequenceSend_Main,     TradeSequenceSend_Exit,     1 | 2 },
    { TradeSequenceDepart_Init,   TradeSequenceDepart_Main,   TradeSequenceDepart_Exit,   1 | 2 },
    { TradeSequenceRecede_Init,   TradeSequenceRecede_Main,   TradeSequenceRecede_Exit,   1 | 2 },
    { TradeSequenceExchange_Init, TradeSequenceExchange_Main, TradeSequenceExchange_Exit, 1     },
    { TradeSequenceApproach_Init, TradeSequenceApproach_Main, TradeSequenceApproach_Exit, 1 | 4 },
    { TradeSequenceArrive_Init,   TradeSequenceArrive_Main,   TradeSequenceArrive_Exit,   1 | 4 },
    { TradeSequenceReceive_Init,  TradeSequenceReceive_Main,  TradeSequenceReceive_Exit,  1 | 4 },
};

static s32 sTradeSequenceDeferredFreeCount;

static void *sTradeSequenceDeferredFrees[TRADE_SEQUENCE_MAX_DEFERRED];

BOOL TradeSequence_Init(OverlayManager *man, int *state) {
    TradeSequence *ts;

    if (IsPaletteFadeFinished()) {
        Heap_Create(HEAP_ID_3, HEAP_ID_56, 3 << 15);
        Heap_Create(HEAP_ID_3, HEAP_ID_57, 3 << 15);
        TradeSequence_ResetDeferredFrees();
        Sound_SetSceneAndPlayBGM(3, SEQ_GS_KOUKAN, 1);
        ts = OverlayManager_CreateAndGetData(man, sizeof(TradeSequence), HEAP_ID_56);
        if (ts) {
            ts->args = OverlayManager_GetArgs(man);
            ts->scene = 0;
            ts->bgConfig = BgConfig_Alloc(HEAP_ID_56);
            ts->string = String_New(400, HEAP_ID_56);
            ts->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0179_bin, HEAP_ID_56);
            ts->msgFormat = MessageFormat_New(HEAP_ID_56);
            switch (ts->args->is_ingame) {
            case 1: // full trade
                BufferBoxMonNickname(ts->msgFormat, 0, ts->args->my_boxmon);
                BufferBoxMonNickname(ts->msgFormat, 1, ts->args->trade_boxmon);
                BufferPlayersName(ts->msgFormat, 2, ts->args->trade_profile);
                ts->receivedMonCantFlip = TradeSequence_BoxMonCantFlip(ts->args->trade_boxmon);
                break;
            case 2: // sending only (msg 0179: "will be put on offer")
                BufferBoxMonNickname(ts->msgFormat, 0, ts->args->my_boxmon);
                break;
            case 4: // receiving only (msg 0179: "was taken out")
                BufferBoxMonNickname(ts->msgFormat, 1, ts->args->trade_boxmon);
                ts->receivedMonCantFlip = TradeSequence_BoxMonCantFlip(ts->args->trade_boxmon);
                break;
            }
            NNS_G2dInitOamManagerModule();
            OamManager_Create(0, 128, 0, 32, 1, 127, 0, 32, HEAP_ID_56);
            ts->spriteList = G2dRenderer_Init(64, &ts->renderer, HEAP_ID_56);
            G2dRenderer_SetSubSurfaceCoords(&ts->renderer, 0, FX32_CONST(232));
            ts->sentSpecies = GetBoxMonData(ts->args->my_boxmon, MON_DATA_SPECIES, NULL);
            ts->receivedSpecies = GetBoxMonData(ts->args->trade_boxmon, MON_DATA_SPECIES, NULL);
            ts->sentForm = GetBoxMonData(ts->args->my_boxmon, MON_DATA_FORM, NULL);
            ts->receivedForm = GetBoxMonData(ts->args->trade_boxmon, MON_DATA_FORM, NULL);
            ts->sceneWork = NULL;
            Main_SetVBlankIntrCB(NULL, NULL);
            HBlankInterruptDisable();
            GfGfx_DisableEngineAPlanes();
            GfGfx_DisableEngineBPlanes();
            GX_SetVisiblePlane(0);
            GXS_SetVisiblePlane(0);
            ts->drawSpritesTask = SysTask_CreateOnPrintQueue(TradeSequence_DrawSpritesTask, ts, 1);
            ts->sceneWork = NULL;
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL TradeSequence_BoxMonCantFlip(BoxPokemon *boxMon) {
    u32 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    u32 form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
    if (GetMonBaseStat_HandleAlternateForm(species, form, BASE_FLIP) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL TradeSequence_Exit(OverlayManager *man, int *state) {
    OSIntrMode enabled = OS_DisableInterrupts();
    TradeSequence *ts = OverlayManager_GetData(man);
    SysTask_Destroy(ts->drawSpritesTask);
    WirelessStatusIcon_Destroy();
    MessageFormat_Delete(ts->msgFormat);
    DestroyMsgData(ts->msgData);
    String_Delete(ts->string);
    Heap_Free(ts->bgConfig);
    SpriteList_Delete(ts->spriteList);
    OamManager_Free();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_56);
    Heap_Destroy(HEAP_ID_57);
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

BOOL TradeSequence_Main(OverlayManager *man, int *state) {
    TradeSequence *ts = OverlayManager_GetData(man);

    if (ts->scene < TRADE_SEQUENCE_NUM_SCENES) {
        if (ts->args->is_ingame & sTradeSequenceScenes[ts->scene].tradeModes) {
            if (ts->sceneWork == NULL) {
                ts->sceneWork = sTradeSequenceScenes[ts->scene].init(ts);
            }
            if (sTradeSequenceScenes[ts->scene].main(ts->sceneWork, state)) {
                sTradeSequenceScenes[ts->scene].exit(ts->sceneWork);
                ts->sceneWork = NULL;
                ts->scene++;
                *state = 0;
            }
        } else {
            ts->scene = ts->scene + 1;
            *state = 0;
        }
        TradeSequence_FlushDeferredFrees();
        return FALSE;
    }
    return TRUE;
}

void TradeSequence_DrawSpritesTask(SysTask *task, void *data) {
    TradeSequence *ts = data;
    vu32 *dtcm = (vu32 *)0x027E0000; // OS_SetIrqCheckFlag(OS_IE_V_BLANK) through the DTCM base address

    SpriteList_RenderAndAnimateSprites(ts->spriteList);
    OamManager_ApplyAndResetBuffers();
    dtcm[0x3FF8 / sizeof(u32)] |= 1;
}

TradeSequencePulses *TradeSequence_CreatePulses(int *running, int direction) {
    NNSG2dPaletteData *plttData;
    void *pltt;
    int i;
    TradeSequencePulses *pulses = Heap_Alloc(HEAP_ID_56, sizeof(TradeSequencePulses));

    *running = 0;
    if (pulses != NULL) {
        pltt = GfGfxLoader_GetPlttData(NARC_a_0_8_9, 6, &plttData, HEAP_ID_56);
        if (pltt != NULL) {
            MI_CpuClear16(pulses, 0x60);
            Heap_Free(pltt);
        }
        pulses->dirty = 0;
        pulses->stop = 0;
        pulses->timer = 0;
        pulses->count = 0;
        pulses->running = running;
        pulses->nextSlot = 0;
        pulses->direction = direction;
        for (i = 0; i < TRADE_SEQUENCE_MAX_PULSES; i++) {
            pulses->pulseTasks[i] = NULL;
        }
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 16, 8);
        G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 16, 8);
        pulses->task = SysTask_CreateOnVWaitQueue(TradeSequence_PulsesTask, pulses, 20);
        if (pulses->task != NULL) {
            *pulses->running = 1;
        }
    }
    return pulses;
}

void TradeSequence_StopPulses(TradeSequencePulses *pulses) {
    if (*pulses->running == 1) {
        pulses->stop = 1;
    }
}

void TradeSequence_DestroyPulses(TradeSequencePulses *pulses) {
    if (*pulses->running == 1) {
        int i;
        for (i = 0; i < TRADE_SEQUENCE_MAX_PULSES; i++) {
            if (pulses->pulseTasks[i]) {
                TradeSequence_DestroyPulse(pulses, i);
            }
        }
        *pulses->running = 0;
        SysTask_Destroy(pulses->task);
        Heap_Free(pulses);
    }
}

void TradeSequence_PulsesTask(SysTask *task, void *data) {
    TradeSequencePulses *pulses = data;

    if (pulses->stop == 0) {
        pulses->timer--;
        if (pulses->timer <= 0) {
            PlaySE(SEQ_SE_DP_KOUKAN08);
            pulses->timer = 30;
            TradeSequence_StartPulse(pulses);
        }
        if (pulses->dirty) {
            GX_BeginLoadBGExtPltt();
            GXS_BeginLoadBGExtPltt();
            DC_FlushRange(pulses, 96);
            GX_LoadBGExtPltt(pulses, 0x6000, 96);
            GXS_LoadBGExtPltt(pulses, 0x6000, 96);
            GX_EndLoadBGExtPltt();
            GXS_EndLoadBGExtPltt();
            pulses->dirty = 0;
        }
    } else {
        if (pulses->dirty) {
            GX_BeginLoadBGExtPltt();
            GXS_BeginLoadBGExtPltt();
            DC_FlushRange(pulses, 96);
            GX_LoadBGExtPltt(pulses, 0x6000, 96);
            GXS_LoadBGExtPltt(pulses, 0x6000, 96);
            GX_EndLoadBGExtPltt();
            GXS_EndLoadBGExtPltt();
            pulses->dirty = 0;
        }
        if (pulses->count == 0) {
            *pulses->running = 0;
            Heap_Free(pulses);
            SysTask_Destroy(task);
        }
    }
}

void TradeSequence_SetPulseColor(TradeSequencePulses *pulses, int clearIndex, int setIndex, u16 color) {
    if (clearIndex >= 0 && clearIndex < TRADE_SEQUENCE_NUM_PULSE_COLORS) {
        pulses->colors[clearIndex] = 0;
    }
    if (setIndex >= 0 && setIndex < TRADE_SEQUENCE_NUM_PULSE_COLORS) {
        pulses->colors[setIndex] = color;
    }
    pulses->dirty = 1;
}

void TradeSequence_StartPulse(TradeSequencePulses *pulses) {
    TradeSequencePulse *pulse;

    if (pulses->pulseTasks[pulses->nextSlot] != NULL) {
        return;
    }
    pulse = Heap_Alloc(HEAP_ID_56, sizeof(TradeSequencePulse));
    if (pulse) {
        pulse->pulses = pulses;
        pulse->colorIndex = pulses->direction == 1 ? -1 : 15;
        pulse->frameCounter = 0;
        pulse->stepFrames = 2;
        pulse->slot = pulses->nextSlot;
        pulse->direction = pulses->direction;
        pulses->pulseTasks[pulses->nextSlot] = SysTask_CreateOnMainQueue(TradeSequence_PulseTask, pulse, 10);
        if (pulses->pulseTasks[pulses->nextSlot]) {
            pulses->count++;
            pulses->nextSlot++;
            if (pulses->nextSlot >= TRADE_SEQUENCE_MAX_PULSES) {
                pulses->nextSlot = 0;
            }
        } else {
            Heap_Free(pulse);
        }
    }
}

void TradeSequence_DestroyPulse(TradeSequencePulses *pulses, int slot) {
    if (pulses->pulseTasks[slot] != NULL) {
        Heap_Free(SysTask_GetData(pulses->pulseTasks[slot]));
        SysTask_Destroy(pulses->pulseTasks[slot]);
        pulses->pulseTasks[slot] = NULL;
        pulses->count = pulses->count - 1;
    }
}

void TradeSequence_PulseTask(SysTask *task, void *data) {
    TradeSequencePulse *pulse = data;

    if (++pulse->frameCounter >= pulse->stepFrames) {
        pulse->frameCounter = 0;
        if (pulse->direction == 1) {
            if (pulse->colorIndex + 1 >= 15) {
                TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex, pulse->colorIndex + 1, 0);
                TradeSequence_DestroyPulse(pulse->pulses, pulse->slot);
                return;
            }
            TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex, pulse->colorIndex + 1, RGB_WHITE);
            pulse->colorIndex++;
            return;
        }
        if (pulse->colorIndex < 0) {
            TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex, pulse->colorIndex + 1, 0);
            TradeSequence_DestroyPulse(pulse->pulses, pulse->slot);
            return;
        }
        TradeSequence_SetPulseColor(pulse->pulses, pulse->colorIndex + 1, pulse->colorIndex, RGB_WHITE);
        pulse->colorIndex--;
    }
}

TradeSequenceBgZoom *TradeSequence_StartBgZoom(BgConfig *bgConfig, fx32 scale, fx32 targetScale, fx32 speed, fx32 acceleration, int centerYIndex, int *finished) {
    TradeSequenceBgZoom *zoom = Heap_Alloc(HEAP_ID_56, sizeof(TradeSequenceBgZoom));

    *finished = 1;
    if (zoom != NULL) {
        zoom->bgConfig = bgConfig;
        zoom->scale = scale;
        zoom->targetScale = targetScale;
        zoom->speed = speed;
        zoom->acceleration = acceleration;
        zoom->direction = scale < targetScale ? 0 : 1;
        zoom->centerYIndex = centerYIndex;
        zoom->finished = finished;
        *finished = 0;
        zoom->task = SysTask_CreateOnVBlankQueue(TradeSequence_BgZoomTask, zoom, 0);
        if (zoom->task == NULL) {
            *finished = 1;
            Heap_Free(zoom);
        }
    }
    return zoom;
}

void TradeSequence_BgZoomTask(SysTask *task, void *data) {
    TradeSequenceBgZoom *zoom = data;
    s32 amount = ((zoom->scale * zoom->speed) >> FX32_SHIFT) / 32;
    s32 scale = zoom->scale;

    switch (zoom->direction) {
    case 0: {
        s32 target = zoom->targetScale;
        scale += amount;
        if (scale < target) {
            zoom->scale = scale;
        } else {
            zoom->scale = target;
            *zoom->finished = 1;
        }
    } break;
    case 1: {
        s32 target = zoom->targetScale;
        scale -= amount;
        if (scale > target) {
            zoom->scale = scale;
        } else {
            zoom->scale = target;
            *zoom->finished = 1;
        }
    } break;
    }
    TradeSequence_ApplyBgZoom(zoom);
    if (*zoom->finished != 0) {
        TradeSequence_DeferFree(zoom);
        SysTask_Destroy(task);
    } else {
        zoom->speed += zoom->acceleration;
    }
}

void TradeSequence_ApplyBgZoom(TradeSequenceBgZoom *zoom) {
    MtxFx22 matrix;
    matrix._00 = zoom->scale;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = zoom->scale;
    SetBgAffine(zoom->bgConfig, GF_BG_LYR_MAIN_2, &matrix, 128, sTradeSequenceBgZoomCenterY[zoom->centerYIndex]);
    SetBgAffine(zoom->bgConfig, GF_BG_LYR_SUB_2, &matrix, 128, sTradeSequenceBgZoomCenterY[zoom->centerYIndex]);
}

void TradeSequence_LoadMonPicToBg(TradeSequence *ts, u32 whichMon, u32 bgId, u32 palette, BOOL large) {
    u32 size = large ? 6400 : 3200;
    void *buffer = Heap_AllocAtEnd(HEAP_ID_56, size);

    struct {
        TileRect settings;
        PokepicTemplate template;
    } local;

    u16 species;
    BOOL lock;
    BoxPokemon *boxMon;
    u32 personality;

    if (buffer) {
        local.settings = sTradeSequenceMonPicSettings;
        if (whichMon == 0) {
            boxMon = ts->args->my_boxmon;
        } else {
            boxMon = ts->args->trade_boxmon;
        }
        lock = AcquireBoxMonLock(boxMon);
        GetBoxmonSpriteCharAndPlttNarcIds(&local.template, boxMon, 2, FALSE);
        personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL);
        species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        if (large) {
            local.settings.w *= 2;
        }
        if (GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL) == TRUE && species == SPECIES_SPINDA) {
            species = SPECIES_EGG;
        }
        CharTiles_LoadMonRectAsObjFromNarc((NarcId)local.template.narcID, local.template.charDataID, HEAP_ID_56, &local.settings, buffer, personality, large, 2, species);
        DC_FlushRange(buffer, size);
        BG_LoadCharTilesData(ts->bgConfig, (u8)bgId, buffer, size, 0);
        ReleaseBoxMonLock(boxMon, lock);
        Heap_Free(buffer);
    }
    GfGfxLoader_GXLoadPal((NarcId)local.template.narcID, local.template.palDataID, bgId >= GF_BG_LYR_SUB_0 ? GF_PAL_LOCATION_SUB_BG : GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(palette * 32), 32, HEAP_ID_56);
}

void TradeSequence_LoadMonPicTilemap(TradeSequence *ts, int whichMon, int bgId, int palette, int x, int y) {
    NNSG2dScreenData *screen;
    void *buffer;
    int member;

    if (whichMon == 0) {
        member = 0x10A;
    } else if (ts->receivedMonCantFlip) {
        member = 0x10B;
    } else {
        member = 0x10A;
    }
    buffer = GfGfxLoader_GetScrnData(NARC_a_0_0_7, member, FALSE, &screen, HEAP_ID_56);
    if (buffer != NULL) {
        CopyToBgTilemapRect(ts->bgConfig, bgId, x, y, 10, 10, screen->rawData, 0, 0, 32, 32);
        BgTilemapRectChangePalette(ts->bgConfig, bgId, x, y, 10, 10, palette);
        Heap_Free(buffer);
    }
}

void TradeSequence_LoadCellAnim(TradeSequenceCellAnim *cellAnim, NarcId narcId, s32 cellBankId, s32 animBankId) {
    cellAnim->cellBankFile = GfGfxLoader_GetCellBank(narcId, cellBankId, TRUE, &cellAnim->cellBank, HEAP_ID_57);
    cellAnim->animBankFile = GfGfxLoader_GetAnimBank(narcId, animBankId, TRUE, &cellAnim->animBank, HEAP_ID_57);
}

void TradeSequence_FreeCellAnim(TradeSequenceCellAnim *cellAnim) {
    if (cellAnim->cellBankFile != NULL) {
        Heap_Free(cellAnim->cellBankFile);
        cellAnim->cellBankFile = NULL;
    }
    if (cellAnim->animBankFile != NULL) {
        Heap_Free(cellAnim->animBankFile);
        cellAnim->animBankFile = NULL;
    }
}

void TradeSequence_InitSpriteHeader(SpriteResourcesHeader *header, TradeSequenceCellAnim *cellAnim, const NNSG2dImageProxy *imageProxy, const NNSG2dImagePaletteProxy *plttProxy, u32 priority) {
    header->imageProxy = imageProxy;
    header->plttProxy = plttProxy;
    header->cellData = cellAnim->cellBank;
    header->cellAnim = cellAnim->animBank;
    header->priority = priority;
    header->charData = NULL;
    header->multiCellData = NULL;
    header->multiCellAnim = NULL;
    header->flag = 0;
}

Sprite *TradeSequence_CreateSprite(TradeSequence *ts, const SpriteResourcesHeader *header, int x, int y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen) {
    SimpleSpriteTemplate template;
    Sprite *sprite;

    template.spriteList = ts->spriteList;
    template.header = header;
    template.position.x = x << FX32_SHIFT;
    template.position.y = y << FX32_SHIFT;
    template.position.z = 0;
    template.priority = priority;
    template.whichScreen = whichScreen;
    template.heapID = HEAP_ID_56;
    sprite = Sprite_Create(&template);
    if (sprite != NULL) {
        Sprite_SetAnimActiveFlag(sprite, TRUE);
        Sprite_SetAnimSpeed(sprite, FX32_ONE);
    }
    return sprite;
}

BgConfig *TradeSequence_GetBgConfig(TradeSequence *ts) {
    return ts->bgConfig;
}

MessageFormat *TradeSequence_GetMsgFormat(TradeSequence *ts) {
    return ts->msgFormat;
}

MsgData *TradeSequence_GetMsgData(TradeSequence *ts) {
    return ts->msgData;
}

BoxPokemon *TradeSequence_GetSentBoxMon(TradeSequence *ts) {
    return ts->args->my_boxmon;
}

BoxPokemon *TradeSequence_GetReceivedBoxMon(TradeSequence *ts) {
    return ts->args->trade_boxmon;
}

int TradeSequence_GetTimeOfDay(TradeSequence *ts) {
    return ts->args->time_of_day;
}

u16 TradeSequence_GetSentSpecies(TradeSequence *ts) {
    return ts->sentSpecies;
}

u16 TradeSequence_GetSentForm(TradeSequence *ts) {
    return ts->sentForm;
}

u16 TradeSequence_GetReceivedSpecies(TradeSequence *ts) {
    return ts->receivedSpecies;
}

u16 TradeSequence_GetReceivedForm(TradeSequence *ts) {
    return ts->receivedForm;
}

u32 TradeSequence_GetFrameType(TradeSequence *ts) {
    return Options_GetFrame(ts->args->options);
}

int TradeSequence_GetTradeMode(TradeSequence *ts) {
    return ts->args->is_ingame;
}

void TradeSequence_ResetDeferredFrees(void) {
    sTradeSequenceDeferredFreeCount = 0;
}

void TradeSequence_FlushDeferredFrees(void) {
    s32 i;

    if (sTradeSequenceDeferredFreeCount != 0) {
        for (i = 0; i < sTradeSequenceDeferredFreeCount; i++) {
            Heap_Free(sTradeSequenceDeferredFrees[i]);
        }
        sTradeSequenceDeferredFreeCount = 0;
    }
}

void TradeSequence_DeferFree(void *ptr) {
    GF_ASSERT(sTradeSequenceDeferredFreeCount < TRADE_SEQUENCE_MAX_DEFERRED);
    sTradeSequenceDeferredFrees[sTradeSequenceDeferredFreeCount++] = ptr;
}

TradeSequenceModels *TradeSequence_CreateModels(u32 count, fx32 targetX, fx32 targetY, fx32 targetZ) {
    TradeSequenceModels *models = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceModels));
    u32 i;

    if (models != NULL) {
        TradeSequence_InitCamera(models, targetX, targetY, targetZ);
        models->models = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceModel) * count);
        models->count = count;
        for (i = 0; i < count; i++) {
            TradeSequence_InitModel(&models->models[i]);
        }
    }
    return models;
}

void TradeSequence_DestroyModels(TradeSequenceModels *models) {
    u32 i;

    for (i = 0; i < models->count; i++) {
        TradeSequence_FreeModel(&models->models[i]);
    }
    Heap_Free(models->models);
    TradeSequence_FreeCamera(models);
    Heap_Free(models);
}

void TradeSequence_DrawModels(TradeSequenceModels *models) {
    u32 i;
    u32 one;

    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    Camera_PushLookAtToNNSGlb();
    for (i = 0; i < models->count; i++) {
        if (models->models[i].visible != FALSE) {
            TradeSequence_DrawModel(&models->models[i]);
        }
    }
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
}

void TradeSequence_DrawModel(TradeSequenceModel *model) {
    MtxFx33 rotation;
    MtxFx33 temp;

    MTX_Identity33_(&rotation);
    MTX_RotX33_(&temp, FX_SinIdx((u16)model->rotation.x), FX_CosIdx((u16)model->rotation.x));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotY33_(&temp, FX_SinIdx((u16)model->rotation.y), FX_CosIdx((u16)model->rotation.y));
    MTX_Concat33(&temp, &rotation, &rotation);
    MTX_RotZ33_(&temp, FX_SinIdx((u16)model->rotation.z), FX_CosIdx((u16)model->rotation.z));
    MTX_Concat33(&temp, &rotation, &rotation);
    if (model->alpha != 31) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, 0, model->alpha, 0);
    }
    GF3dRender_DrawModel(&model->renderObj, &model->translation, &rotation, &model->scale);
}

void TradeSequence_InitModel(TradeSequenceModel *model) {
    model->visible = FALSE;
    model->file = NULL;
    model->rotation.x = model->rotation.y = model->rotation.z = 0;
    model->scale.x = model->scale.y = model->scale.z = FX32_ONE;
    model->translation.x = model->translation.y = model->translation.z = 0;
}

void TradeSequence_FreeModel(TradeSequenceModel *model) {
    if (model->file != NULL) {
        Heap_Free(model->file);
        model->file = NULL;
        model->visible = FALSE;
    }
}

TradeSequenceModel *TradeSequence_LoadModel(TradeSequenceModels *models, int index, NarcId narcId, s32 fileId, fx32 x, fx32 y, fx32 z, BOOL visible) {
    TradeSequenceModel *base = models->models;
    TradeSequenceModel *model = &base[index];

    base[index].file = GfGfxLoader_LoadFromNarc(narcId, fileId, FALSE, HEAP_ID_57, TRUE);
    DC_FlushRange(base[index].file, GetNarcMemberSizeByIdPair(narcId, fileId));
    if (base[index].file) {
        model->mdlSet = NNS_G3dGetMdlSet(base[index].file);
        model->mdl = NNS_G3dGetMdlByIdx(model->mdlSet, 0);
        model->tex = NNS_G3dGetTex(model->file);
        GF3dRender_AllocAndLoadTexResources(model->tex);
        GF3dRender_BindModelSet(model->file, model->tex);
        NNS_G3dRenderObjInit(&model->renderObj, model->mdl);
        model->translation.x = x;
        model->translation.y = y;
        model->translation.z = z;
        model->alpha = 31;
        model->visible = visible;
    }
    return model;
}

void TradeSequence_GetModelPosition(TradeSequenceModel *model, VecFx32 *position) {
    *position = model->translation;
}

void TradeSequence_SetModelPosition(TradeSequenceModel *model, const VecFx32 *position) {
    model->translation = *position;
}

void TradeSequence_GetModelRotation(TradeSequenceModel *model, TradeSequenceRotation *rotation) {
    rotation->x = *(u16 *)&model->rotation.x;
    rotation->y = *(u16 *)&model->rotation.y;
    rotation->z = *(u16 *)&model->rotation.z;
}

void TradeSequence_SetModelRotation(TradeSequenceModel *model, const TradeSequenceRotation *rotation) {
    model->rotation.x = *(const u16 *)&rotation->x;
    model->rotation.y = *(const u16 *)&rotation->y;
    model->rotation.z = *(const u16 *)&rotation->z;
}

void TradeSequence_SetModelVisible(TradeSequenceModel *model, BOOL visible) {
    model->visible = visible;
}

void TradeSequence_SetModelAlpha(TradeSequenceModel *model, u32 alpha) {
    model->alpha = alpha;
    if (alpha != 31) {
        NNSi_G3dModifyPolygonAttrMask(model->mdl, FALSE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
    } else {
        NNSi_G3dModifyPolygonAttrMask(model->mdl, TRUE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
    }
}

void TradeSequence_SetModelScale(TradeSequenceModel *model, fx32 scale) {
    model->scale.x = scale;
    model->scale.y = scale;
    model->scale.z = scale;
}

BOOL TradeSequence_IsModelInView(TradeSequenceModel *model) {
    MtxFx33 matrix;
    MtxFx33 rot;
    int one;
    int result;

    MTX_Identity33(&matrix);
    MTX_RotX33(&rot, FX_SinIdx((u16)model->rotation.x), FX_CosIdx((u16)model->rotation.x));
    MTX_Concat33(&rot, &matrix, &matrix);
    MTX_RotY33(&rot, FX_SinIdx((u16)model->rotation.y), FX_CosIdx((u16)model->rotation.y));
    MTX_Concat33(&rot, &matrix, &matrix);
    MTX_RotZ33(&rot, FX_SinIdx((u16)model->rotation.z), FX_CosIdx((u16)model->rotation.z));
    MTX_Concat33(&rot, &matrix, &matrix);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    Camera_PushLookAtToNNSGlb();
    result = GF3d_ModelBoxTest(model->mdl, &model->translation, &matrix, &model->scale);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, (u32 *)&one, 1);
    return result;
}

void TradeSequence_GetCameraPos(TradeSequenceModels *models, VecFx32 *pos) {
    VecFx32 camPos;
    camPos = Camera_GetLookAtCamPos(models->camera);
    *pos = camPos;
}

void TradeSequence_SetCameraAngle(TradeSequenceModels *models, const CameraAngle *angle) {
    Camera_SetAnglePos(angle, models->camera);
}

void TradeSequence_AdjustCameraAngle(TradeSequenceModels *models, const CameraAngle *delta) {
    Camera_AdjustAngleTarget(delta, models->camera);
}

void TradeSequence_SetCameraPerspectiveType(TradeSequenceModels *models, u8 perspectiveType) {
    Camera_ApplyPerspectiveType(perspectiveType, models->camera);
}

void TradeSequence_SetCameraPerspectiveAngle(TradeSequenceModels *models, u16 perspectiveAngle) {
    Camera_SetPerspectiveAngle(perspectiveAngle, models->camera);
}

void TradeSequence_InitCamera(TradeSequenceModels *models, fx32 targetX, fx32 targetY, fx32 targetZ) {
    VecFx32 up;

    models->camera = Camera_New(HEAP_ID_57);
    models->cameraTarget.x = targetX;
    models->cameraTarget.y = targetY;
    models->cameraTarget.z = targetZ;
    models->cameraAngle.x = 0;
    models->cameraAngle.y = 0;
    models->cameraAngle.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&models->cameraTarget, FX32_CONST(300), &models->cameraAngle, 4004, 0, TRUE, models->camera);
    up.x = 0;
    up.y = FX32_ONE;
    up.z = 0;
    Camera_SetLookAtCamUp(&up, models->camera);
    Camera_SetStaticPtr(models->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(1000), models->camera);
}

void TradeSequence_FreeCamera(TradeSequenceModels *models) {
    Camera_UnsetStaticPtr();
    Camera_Delete(models->camera);
}
