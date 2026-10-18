#include "field_poison_effect.h"

#include "global.h"

#include "constants/sndseq.h"

#include "field/hblank_system.h"

#include "assert.h"
#include "heap.h"
#include "scanline_effect.h"
#include "sound_playback.h"
#include "sys_task.h"
#include "sys_task_api.h"

void G3X_SetHOffset(int offset);

enum PoisonEffectState {
    POISON_EFFECT_IDLE,
    POISON_EFFECT_RUNNING,
};

enum PoisonEffectSeq {
    POISON_SEQ_START,
    POISON_SEQ_RAMP_UP,
    POISON_SEQ_RAMP_DOWN,
    POISON_SEQ_STOP,
};

// Screen shake shown when a poisoned party loses HP while walking: every HBlank shifts the 3D layer
// horizontally by an offset that alternates sign every 10 lines; the amplitude ramps up and down over 6 frames.
struct PoisonEffect {
    u32 state;                  // 0x000
    s32 counter;                // 0x004
    u32 seqNo;                  // 0x008
    u16 buffers[2][192];        // 0x00C, 0x18C
    DoubleBuffer *doubleBuffer; // 0x30C
    HBlankSystem *hBlankSystem; // 0x310
    HBlankTask *hblankTask;     // 0x314
    u32 hblankReady;            // 0x318
    SysTask *mainTask;          // 0x31C
    SysTask *vblankTask;        // 0x320
    SysTask *swapTask;          // 0x324
}; // 0x328

static void PoisonEffect_Task(SysTask *task, void *data);
static void PoisonEffect_ApplyNextLineOffset(PoisonEffect *effect);
static void PoisonEffect_FillOffsets(u16 *offsets, int num, int denom);
static void PoisonEffect_VBlankResetTask(SysTask *task, void *data);
static void PoisonEffect_HBlankCallback(HBlankTask *task, void *data);
static void PoisonEffect_OnHBlank(PoisonEffect *effect);
static void PoisonEffect_SwapBuffersTask(SysTask *task, void *data);
static void PoisonEffect_SwapBuffers(PoisonEffect *effect);
static void PoisonEffect_Stop(PoisonEffect *effect);

void *PoisonEffect_New(enum HeapID heapID, HBlankSystem *hBlankSystem) {
    PoisonEffect *effect = Heap_Alloc(heapID, sizeof(PoisonEffect));
    memset(effect, 0, sizeof(PoisonEffect));
    effect->state = POISON_EFFECT_IDLE;
    effect->doubleBuffer = DoubleBuffer_New(heapID, effect->buffers[0], effect->buffers[1]);
    effect->hBlankSystem = hBlankSystem;
    return effect;
}

void PoisonEffect_Delete(PoisonEffect *effect) {
    if (effect->state == POISON_EFFECT_RUNNING) {
        PoisonEffect_Stop(effect);
    }
    DoubleBuffer_Delete(effect->doubleBuffer);
    Heap_Free(effect);
}

void FieldSystem_DoPoisonEffect(PoisonEffect *effect) {
    GF_ASSERT(effect->state == POISON_EFFECT_IDLE);
    effect->hblankTask = HBlankSystem_StartTask(effect->hBlankSystem, PoisonEffect_HBlankCallback, effect);
    effect->mainTask = SysTask_CreateOnMainQueue(PoisonEffect_Task, effect, 1024);
    effect->vblankTask = SysTask_CreateOnVBlankQueue(PoisonEffect_VBlankResetTask, effect, 1024);
    effect->swapTask = SysTask_CreateOnVBlankQueue(PoisonEffect_SwapBuffersTask, effect, 1024);
    memset(effect->buffers[0], 0, sizeof(effect->buffers[0]));
    memset(effect->buffers[1], 0, sizeof(effect->buffers[1]));
    effect->state = POISON_EFFECT_RUNNING;
    effect->seqNo = POISON_SEQ_START;
    PlaySE(SEQ_SE_DP_DOKU2);
}

static void PoisonEffect_Task(SysTask *task, void *data) {
    PoisonEffect *effect = data;
    switch (effect->seqNo) {
    case POISON_SEQ_START:
        effect->counter = 3;
        effect->seqNo++;
        break;
    case POISON_SEQ_RAMP_UP:
        effect->counter--;
        PoisonEffect_FillOffsets(DoubleBuffer_GetCurrent(effect->doubleBuffer), 3 - effect->counter, 3);
        if (effect->counter <= 0) {
            effect->counter = 3;
            effect->seqNo++;
        }
        break;
    case POISON_SEQ_RAMP_DOWN:
        effect->counter--;
        PoisonEffect_FillOffsets(DoubleBuffer_GetCurrent(effect->doubleBuffer), effect->counter, 3);
        if (effect->counter <= 0) {
            effect->seqNo++;
        }
        break;
    case POISON_SEQ_STOP:
        PoisonEffect_Stop(effect);
        G3X_SetHOffset(0);
        break;
    }
}

static void PoisonEffect_ApplyNextLineOffset(PoisonEffect *effect) {
    s32 vcount = GX_GetVCount();
    u16 *buffer = DoubleBuffer_GetOther(effect->doubleBuffer);
    if (vcount < 192) {
        s32 line = vcount + 1;
        if (line >= 192) {
            line -= 192;
        }
        if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
            G3X_SetHOffset(buffer[line]);
        }
    }
}

static void PoisonEffect_FillOffsets(u16 *offsets, int num, int denom) {
    int toggle = 1;
    int i;
    s32 value = num * 3 / denom;
    for (i = 0; i < 192; i++) {
        if (i % 10 == 0) {
            toggle ^= 1;
        }
        if (toggle != 0) {
            offsets[i] = value;
        } else {
            offsets[i] = -value;
        }
    }
}

static void PoisonEffect_VBlankResetTask(SysTask *task, void *data) {
    PoisonEffect *effect = data;
    effect->hblankReady = 0;
    G3X_SetHOffset(0);
}

static void PoisonEffect_HBlankCallback(HBlankTask *task, void *data) {
    PoisonEffect_OnHBlank(data);
}

static void PoisonEffect_OnHBlank(PoisonEffect *effect) {
    if (effect->state == POISON_EFFECT_RUNNING && effect->hblankReady == 1) {
        PoisonEffect_ApplyNextLineOffset(effect);
    }
}

static void PoisonEffect_SwapBuffersTask(SysTask *task, void *data) {
    PoisonEffect_SwapBuffers(data);
}

static void PoisonEffect_SwapBuffers(PoisonEffect *effect) {
    if (effect->state == POISON_EFFECT_RUNNING) {
        DoubleBuffer_Swap(effect->doubleBuffer);
        effect->hblankReady = 1;
    }
}

static void PoisonEffect_Stop(PoisonEffect *effect) {
    HBlankTask_Delete(effect->hblankTask);
    effect->hblankTask = NULL;
    SysTask_Destroy(effect->mainTask);
    effect->mainTask = NULL;
    SysTask_Destroy(effect->vblankTask);
    effect->vblankTask = NULL;
    SysTask_Destroy(effect->swapTask);
    effect->swapTask = NULL;
    effect->state = POISON_EFFECT_IDLE;
}
