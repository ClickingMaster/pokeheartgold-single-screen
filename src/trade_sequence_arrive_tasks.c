#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "trade_sequence_arrive_internal.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"

// Scene 5 of the trade animation, continued (trade_sequence_arrive.c): its 3D setup and the tasks that lower the
// ball, scroll the backgrounds and fade the ball out. A separate TU: the bss of the data asm lies between the two.

// Scrolls layer 2 of both screens down into place.
typedef struct TradeSequenceArriveScroll {
    TradeSequenceArrive *arrive; // 0x00
    BgConfig *bgConfig;          // 0x04
    int frames;                  // 0x08
    fx32 mainY;                  // 0x0C
    fx32 subY;                   // 0x10
    fx32 speed;                  // 0x14
    u8 unk_18[4];                // 0x18
    SysTask **task;              // 0x1C
    u8 unk_20[4];                // 0x20
} TradeSequenceArriveScroll;     // size: 0x24

// Moves the ball down to its final height.
typedef struct TradeSequenceArriveLower {
    SysTask **task;           // 0x00
    int frames;               // 0x04
    TradeSequenceModel *ball; // 0x08
    VecFx32 position;         // 0x0C
    fx32 step;                // 0x18
} TradeSequenceArriveLower;   // size: 0x1C

typedef struct TradeSequenceArriveAlphaFade {
    SysTask **task;             // 0x00
    fx32 alpha;                 // 0x04
    fx32 step;                  // 0x08
    fx32 target;                // 0x0C
    int frames;                 // 0x10
    TradeSequenceModel *model;  // 0x14
} TradeSequenceArriveAlphaFade; // size: 0x18

extern const GXRgb sTradeSequenceArriveEdgeColors[8];

CameraAngle sTradeSequenceArriveCameraAngle[2]; // only [0] is used

void TradeSequenceArrive_ScrollTask(SysTask *task, void *data);
void TradeSequenceArrive_LowerTask(SysTask *task, void *data);
void TradeSequenceArrive_AlphaFadeTask(SysTask *task, void *data);

void TradeSequenceArrive_Init3D(TradeSequenceArrive *arrive) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceArriveEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    arrive->models = TradeSequence_CreateModels(1, 0, 0, 0);
    arrive->ball = TradeSequence_LoadModel(arrive->models, 0, NARC_a_0_8_9, 27, 0, FX32_CONST(140), 0, TRUE);
    sTradeSequenceArriveCameraAngle[0].x = 0;
    sTradeSequenceArriveCameraAngle[0].y = 0;
    sTradeSequenceArriveCameraAngle[0].z = 0;
    TradeSequence_SetCameraAngle(arrive->models, &sTradeSequenceArriveCameraAngle[0]);
    TradeSequence_SetCameraPerspectiveType(arrive->models, 1);
    TradeSequence_SetModelScale(arrive->ball, FX32_CONST(2));
    TradeSequence_GetModelRotation(arrive->ball, &arrive->rotation);
}

void TradeSequenceArrive_Free3D(TradeSequenceArrive *arrive) {
    if (arrive->models != NULL) {
        TradeSequence_DestroyModels(arrive->models);
        arrive->models = NULL;
    }
}

#pragma force_active on

void TradeSequenceArrive_StartScroll(TradeSequenceArrive *arrive, SysTask **task) {
    TradeSequenceArriveScroll *scroll = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArriveScroll));
    if (scroll) {
        scroll->arrive = arrive;
        scroll->bgConfig = arrive->bgConfig;
        scroll->mainY = FX32_CONST(-384);
        scroll->subY = FX32_CONST(-192);
        scroll->speed = 0x9999;
        scroll->frames = 40;
        scroll->task = task;
        *task = SysTask_CreateOnVBlankQueue(TradeSequenceArrive_ScrollTask, scroll, 0);
        if (*task == NULL) {
            Heap_Free(scroll);
        }
    }
}

void TradeSequenceArrive_ScrollTask(SysTask *task, void *data) {
    TradeSequenceArriveScroll *scroll = data;

    if (scroll->frames) {
        scroll->mainY += scroll->speed;
        scroll->subY += scroll->speed;
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, scroll->mainY >> FX32_SHIFT);
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, scroll->subY >> FX32_SHIFT);
        scroll->frames--;
    } else {
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 192);
        TradeSequenceArrive_StopScroll(task);
    }
}

#pragma force_active reset

void TradeSequenceArrive_StopScroll(SysTask *task) {
    TradeSequenceArriveScroll *scroll;
    if (task != NULL) {
        scroll = SysTask_GetData(task);
        *scroll->task = NULL;
        TradeSequence_DeferFree(scroll); // may run in the V-blank task itself
        SysTask_Destroy(task);
    }
}

#pragma force_active on

void TradeSequenceArrive_StartLower(TradeSequenceArrive *arrive, SysTask **task) {
    TradeSequenceArriveLower *lower = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArriveLower));
    if (lower) {
        lower->task = task;
        lower->ball = arrive->ball;
        TradeSequence_GetModelPosition(lower->ball, &lower->position);
        lower->step = (FX32_CONST(50) - lower->position.y) / 16;
        lower->frames = 16;
        *task = SysTask_CreateOnMainQueue(TradeSequenceArrive_LowerTask, lower, 0);
        if (!*task) {
            Heap_Free(lower);
        }
    }
}

void TradeSequenceArrive_LowerTask(SysTask *task, void *data) {
    TradeSequenceArriveLower *lower = data;

    if (lower->frames) {
        lower->position.y += lower->step;
        TradeSequence_SetModelPosition(lower->ball, &lower->position);
        lower->frames--;
        return;
    }
    lower->position.y = FX32_CONST(50);
    TradeSequence_SetModelPosition(lower->ball, &lower->position);
    TradeSequenceArrive_StopLower(task);
}

#pragma force_active reset

void TradeSequenceArrive_StopLower(SysTask *task) {
    TradeSequenceArriveLower *lower;
    if (task != NULL) {
        lower = SysTask_GetData(task);
        *lower->task = NULL;
        Heap_Free(lower);
        SysTask_Destroy(task);
    }
}

#pragma force_active on

void TradeSequenceArrive_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames, SysTask **task) {
    TradeSequenceArriveAlphaFade *fade = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArriveAlphaFade));
    if (fade) {
        fade->task = task;
        fade->alpha = start << FX32_SHIFT;
        fade->target = target << FX32_SHIFT;
        fade->step = (fade->target - fade->alpha) / frames;
        fade->frames = frames;
        fade->model = model;
        TradeSequence_SetModelAlpha(model, start);
        *task = SysTask_CreateOnMainQueue(TradeSequenceArrive_AlphaFadeTask, fade, 0);
        if (!*task) {
            Heap_Free(fade);
        }
    } else {
        *task = NULL;
    }
}

#pragma force_active reset

void TradeSequenceArrive_AlphaFadeTask(SysTask *task, void *data) {
    TradeSequenceArriveAlphaFade *fade = data;

    fade->frames--;
    if (fade->frames > 0) {
        fade->alpha += fade->step;
        TradeSequence_SetModelAlpha(fade->model, fade->alpha >> FX32_SHIFT);
        return;
    }
    TradeSequence_SetModelAlpha(fade->model, fade->target >> FX32_SHIFT);
    TradeSequenceArrive_StopAlphaFade(task);
}

void TradeSequenceArrive_StopAlphaFade(SysTask *task) {
    TradeSequenceArriveAlphaFade *fade;
    if (task != NULL) {
        fade = SysTask_GetData(task);
        *fade->task = NULL;
        Heap_Free(fade);
        SysTask_Destroy(task);
    }
}
