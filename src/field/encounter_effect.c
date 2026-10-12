#include "field/encounter_effect.h"

#include <nitro/os/tick.h>

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"

FS_EXTERN_OVERLAY(OVY_114);
FS_EXTERN_OVERLAY(OVY_115);
FS_EXTERN_OVERLAY(OVY_116);
FS_EXTERN_OVERLAY(OVY_117);
FS_EXTERN_OVERLAY(OVY_118);
FS_EXTERN_OVERLAY(OVY_119);
FS_EXTERN_OVERLAY(OVY_120);

// Master brightness fade of one screen (1: main, otherwise sub).
typedef struct BrightnessInterp {
    EncounterEffectInterp interp;
    int screen;
} BrightnessInterp;

// The screen flashes count times between 0 and brightness while the other screen fades to otherBrightness.
typedef struct EncounterEffectFlash {
    u32 state;                  // 0x00
    int count;                  // 0x04
    int flashes;                // 0x08
    int screen;                 // 0x0C
    int brightness;             // 0x10
    int otherBrightness;        // 0x14
    BrightnessInterp flash;     // 0x18
    BrightnessInterp otherFade; // 0x30
    BOOL *done;                 // 0x48
} EncounterEffectFlash;

// NitroSDK gx.c; the SDK headers in lib/include only declare the getter.
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

extern void (*const sEncounterEffectFuncs[])(SysTask *task, EncounterEffectTask *effect);

void Task_EncounterEffectFlash(SysTask *task, void *data);
void EncounterEffect_SetMasterBrightness(int screen, s32 brightness);
void BrightnessInterp_Init(BrightnessInterp *interp, s32 start, s32 end, int screen, s32 duration);
BOOL BrightnessInterp_Update(BrightnessInterp *interp);
void Task_ApplyBrightness(SysTask *task, void *data);

void Task_RunEncounterEffect(SysTask *task, EncounterEffectTask *effect) {
    sEncounterEffectFuncs[gEncounterEffectState.effect](task, effect);
    gEncounterEffectState.frameCount++;
    OS_GetTick();
    OS_SetTick(0);
}

void EncounterEffect_LoadOverlay(s32 effect) {
    if (effect >= 12 && effect <= 33) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_115), OVY_LOAD_ASYNC);
        return;
    }
    if ((u32)(effect - 34) <= 2) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_116), OVY_LOAD_ASYNC);
        return;
    }
    if (effect >= 39 && effect <= 44) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_117), OVY_LOAD_ASYNC);
        return;
    }
    if (effect == 45) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_118), OVY_LOAD_ASYNC);
        return;
    }
    if (effect >= 0 && effect <= 5) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_120), OVY_LOAD_ASYNC);
        return;
    }
    if (effect >= 6 && effect <= 11) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_119), OVY_LOAD_ASYNC);
        return;
    }
    if (effect == 46) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_120), OVY_LOAD_ASYNC);
        return;
    }
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_114), OVY_LOAD_ASYNC);
}

void EncounterEffect_UnloadOverlay(s32 effect) {
    if (effect >= 12 && effect <= 33) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_115));
        return;
    }
    if ((u32)(effect - 34) <= 2) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_116));
        return;
    }
    if (effect >= 39 && effect <= 44) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_117));
        return;
    }
    if (effect == 45) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_118));
        return;
    }
    if (effect >= 0 && effect <= 5) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_120));
        return;
    }
    if (effect >= 6 && effect <= 11) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_119));
        return;
    }
    if (effect == 46) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_120));
        return;
    }
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_114));
}

void EncounterEffect_Start(s32 effect, FieldSystem *fieldSystem, BOOL *done) {
    EncounterEffectTask *effectTask;
    SysTask *task;
    BOOL *taskDone;
    gEncounterEffectState.effect = effect;
    gEncounterEffectState.frameCount = 0;
    task = CreateSysTaskAndEnvironment((SysTaskFunc)Task_RunEncounterEffect, sizeof(EncounterEffectTask), 5, HEAP_ID_FIELD1);
    effectTask = SysTask_GetData(task);
    effectTask->fieldSystem = fieldSystem;
    effectTask->done = done;
    effectTask->narc = NARC_New(NARC_a_1_0_9, HEAP_ID_FIELD1);
    taskDone = effectTask->done;
    if (taskDone) {
        *taskDone = FALSE;
    }
    effectTask->unk_18 = FALSE;
}

void EncounterEffect_Finish(EncounterEffectTask *effect, SysTask *task) {
    NARC_Delete(effect->narc);
    Heap_FreeExplicit(HEAP_ID_FIELD1, effect->work);
    DestroySysTaskAndEnvironment(task);
}

void EncounterEffect_StartFlash(int screen, int brightness, int otherBrightness, BOOL *done, int count) {
    EncounterEffectFlash *flash = Heap_Alloc(HEAP_ID_FIELD1, sizeof(EncounterEffectFlash));
    memset(flash, 0, sizeof(EncounterEffectFlash));
    SysTask_CreateOnMainQueue(Task_EncounterEffectFlash, flash, 5);
    flash->done = done;
    if (done) {
        *done = FALSE;
    }
    flash->screen = screen;
    flash->brightness = brightness;
    flash->otherBrightness = otherBrightness;
    flash->count = count;
}

void Task_EncounterEffectFlash(SysTask *task, void *data) {
    EncounterEffectFlash *flash = data;
    switch (flash->state) {
    case 0:
        if (flash->screen == 1) {
            BrightnessInterp_Init(&flash->otherFade, 0, flash->otherBrightness, 2, 8);
        } else if (flash->screen == 2) {
            BrightnessInterp_Init(&flash->otherFade, 0, flash->otherBrightness, 1, 8);
        }
        ++flash->state;
        break;
    case 1:
        BrightnessInterp_Init(&flash->flash, 0, flash->brightness, flash->screen, 3);
        ++flash->state;
        break;
    case 2:
        if (BrightnessInterp_Update(&flash->flash)) {
            ++flash->state;
        }
        break;
    case 3:
        BrightnessInterp_Init(&flash->flash, flash->brightness, 0, flash->screen, 3);
        ++flash->state;
        break;
    case 4:
        if (BrightnessInterp_Update(&flash->flash)) {
            ++flash->flashes;
            if (flash->flashes == flash->count) {
                flash->state = 5;
            } else {
                flash->state = 1;
            }
        }
        break;
    case 5:
        flash->state = 0;
        flash->flashes = 0;
        if (flash->done != NULL) {
            *flash->done = TRUE;
        }
        SysTask_Destroy(task);
        Heap_Free(flash);
        return;
    }
    BrightnessInterp_Update(&flash->otherFade);
}

BOOL ov01_021EFE30(EncounterEffectTask *effect) {
    return effect->unk_18;
}

void EncounterEffectInterp_Init(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration) {
    interp->value = start;
    interp->start = start;
    interp->delta = end - start;
    interp->duration = duration;
    interp->frame = 0;
}

BOOL EncounterEffectInterp_Update(EncounterEffectInterp *interp) {
    s32 value = interp->delta * interp->frame / interp->duration;
    s32 frame;
    s32 duration;
    value += interp->start;
    interp->value = value;
    frame = interp->frame;
    duration = *(volatile s32 *)&interp->duration; // reloads duration after the division
    frame++;
    if (frame <= duration) {
        interp->frame = frame;
        return FALSE;
    }
    interp->frame = duration;
    return TRUE;
}

void EncounterEffectInterp_InitFx(EncounterEffectInterp *interp, s32 start, s32 end, s32 duration) {
    interp->value = start;
    interp->start = start;
    interp->delta = end - start;
    interp->duration = duration;
    interp->frame = 0;
}

BOOL EncounterEffectInterp_UpdateFx(EncounterEffectInterp *interp) {
    s64 product = (s64)interp->delta * (interp->frame << FX32_SHIFT);
    s32 frame;
    s32 duration;
    s32 value = FX_Div((s32)((product + FX32_HALF) >> FX32_SHIFT), interp->duration << FX32_SHIFT);
    value += interp->start;
    interp->value = value;
    frame = interp->frame;
    duration = *(volatile s32 *)&interp->duration;
    frame++;
    if (frame <= duration) {
        interp->frame = frame;
        return FALSE;
    }
    interp->frame = duration;
    return TRUE;
}

void EncounterEffectQuadInterp_Init(EncounterEffectQuadInterp *interp, s32 start, s32 end, s32 velocity, s32 duration) {
    s32 linearTotal = (s32)(((s64)velocity * (duration << FX32_SHIFT) + FX32_HALF) >> FX32_SHIFT);
    s32 remaining = end - start - linearTotal;
    s32 numerator = (s32)((((s64)remaining << 13) + FX32_HALF) >> FX32_SHIFT);
    s32 denominator = (duration * duration) << FX32_SHIFT;
    s32 acceleration = FX_Div(numerator, denominator);
    interp->value = start;
    interp->start = start;
    interp->velocity = velocity;
    interp->acceleration = acceleration;
    interp->frame = 0;
    interp->duration = duration;
}

BOOL EncounterEffectQuadInterp_Update(EncounterEffectQuadInterp *interp) {
    s32 frame = interp->frame;
    s32 linear = (s32)(((s64)interp->velocity * (frame << FX32_SHIFT) + FX32_HALF) >> FX32_SHIFT);
    s32 square = (frame * frame) << FX32_SHIFT;
    s32 quad = (s32)(((s64)interp->acceleration * square + FX32_HALF) >> FX32_SHIFT);
    interp->value = linear + FX_Div(quad, FX32_CONST(2)) + interp->start;
    frame = *(volatile s32 *)&interp->frame;
    if (frame + 1 <= interp->duration) {
        interp->frame = frame + 1;
        return FALSE;
    }
    interp->frame = interp->duration;
    return TRUE;
}

void EncounterEffect_SetMasterBrightness(int screen, s32 brightness) {
    if (screen == 1) {
        GXx_SetMasterBrightness_(&reg_GX_MASTER_BRIGHT, brightness);
    } else {
        GXx_SetMasterBrightness_(&reg_GXS_DB_MASTER_BRIGHT, brightness);
    }
}

void BrightnessInterp_Init(BrightnessInterp *interp, s32 start, s32 end, int screen, s32 duration) {
    interp->screen = screen;
    EncounterEffectInterp_Init(&interp->interp, start, end, duration);
}

BOOL BrightnessInterp_Update(BrightnessInterp *interp) {
    BOOL finished = EncounterEffectInterp_Update(&interp->interp);
    SysTask_CreateOnVWaitQueue(Task_ApplyBrightness, interp, 10);
    return finished;
}

void Task_ApplyBrightness(SysTask *task, void *data) {
    BrightnessInterp *interp = data;
    EncounterEffect_SetMasterBrightness(interp->screen, interp->interp.value);
    SysTask_Destroy(task);
}
