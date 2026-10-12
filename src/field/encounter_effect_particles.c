#include "field/encounter_effect.h"

#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "particle_system.h"
#include "scanline_effect.h"
#include "sys_task.h"
#include "sys_task_api.h"

#include "field/fieldmap.h"

// Particle mode of the leader effects: the main screen switches to 3D (BG0) over a captured image of the field
// (BG2/BG3 bitmap) so particle emitters can be drawn over it. One instance, kept in gEncounterEffectState.
// Also the HBlank BG offset wave used by several effects.

#define PARTICLE_HEAP_SIZE 0x4800

struct EncounterEffectParticles {
    u16 state;                // 0x00
    u8 setupDone;             // 0x02, set once the 3D setup is complete
    u8 setupRequested;        // 0x03, set by EncounterEffect_RequestParticleSetup
    FieldSystem *fieldSystem; // 0x04
    SPLEmitter *system;       // 0x08
    void *heap;               // 0x0C
    u32 texOffset;            // 0x10, next free texture VRAM offset
    u32 plttOffset;           // 0x14, next free palette VRAM offset
};

// particle_system_internal.h declares these with its ParticleSystem type (the third argument there is a callback).
void ParticleSystem_CreateEmitter(SPLEmitter *system, int resNo, const VecFx32 *position);
void ParticleSystem_FreeAll(void);

// NitroSDK fx_mtx22.c; lib/include's fx_mtx22.h does not declare it.
void MTX_Identity22_(MtxFx22 *mtx);

extern const VecFx32 sParticleBaseScale;
extern const GraphicsModes sParticleGraphicsModes;
extern const BgTemplate sParticleCaptureBgTemplate;
extern const GraphicsBanks sParticleGraphicsBanks;

// The leader effects declare these locally (several of them with FieldSystem *).
void EncounterEffect_StartParticleMode(FieldSystem *fieldSystem);
void EncounterEffect_RequestParticleSetup(void);
u8 EncounterEffect_IsParticleSetupDone(void);
void EncounterEffect_DrawParticles(void);
void EncounterEffect_EndParticleMode(void);
void EncounterEffect_LoadParticles(NARC *narc, int member);
void EncounterEffect_CreateEmitters(u32 count);
BOOL EncounterEffect_UpdateParticles(void);
BOOL EncounterEffect_AreParticlesDone(void);
void EncounterEffect_FreeParticles(void);
void Task_ParticleSetup_VWait(SysTask *task, void *data);
void Task_ParticleSetup_Main(SysTask *task, void *data);
void EncounterEffect_SetupParticleGraphics(FieldSystem *fieldSystem);
void EncounterEffect_FreeParticleGraphics(FieldSystem *fieldSystem);
u32 EncounterEffectParticles_AllocTexVram(u32 size, BOOL is4x4Comp);
u32 EncounterEffectParticles_AllocPlttVram(u32 size, BOOL is4Pltt);
SysTask *EncounterEffectHBlankWave_StartTask(EncounterEffectHBlankWave *wave);
void Task_EncounterEffectHBlankWave(SysTask *task, void *data);

#pragma force_active on

void EncounterEffect_StartParticleMode(FieldSystem *fieldSystem) {
    if (gEncounterEffectState.particles) {
        GF_AssertFail();
    }
    gEncounterEffectState.particles = Heap_Alloc(HEAP_ID_FIELD1, sizeof(EncounterEffectParticles));
    memset(gEncounterEffectState.particles, 0, sizeof(EncounterEffectParticles));
    gEncounterEffectState.particles->state = 0;
    gEncounterEffectState.particles->setupDone = FALSE;
    gEncounterEffectState.particles->fieldSystem = fieldSystem;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GX_ResetBankForBG();
    MI_CpuClearFast((void *)0x06840000, 0x20000); // VRAM bank D (LCDC)
    // capture the main screen into VRAM bank D
    reg_GX_DISPCAPCNT = 0xC0320C04;
    SysTask_CreateOnVWaitQueue(Task_ParticleSetup_VWait, gEncounterEffectState.particles, 1024);
    SysTask_CreateOnMainQueue(Task_ParticleSetup_Main, gEncounterEffectState.particles, 1024);
}

#pragma force_active reset

void EncounterEffect_RequestParticleSetup(void) {
    EncounterEffectParticles *particles = gEncounterEffectState.particles;
    if (!particles) {
        GF_ASSERT(FALSE);
    }
    gEncounterEffectState.particles->setupRequested = TRUE;
}

u8 EncounterEffect_IsParticleSetupDone(void) {
    EncounterEffectParticles *particles = gEncounterEffectState.particles;
    if (!particles) {
        GF_ASSERT(FALSE);
    }
    return gEncounterEffectState.particles->setupDone;
}

void EncounterEffect_DrawParticles(void) {
    EncounterEffectParticles *particles = gEncounterEffectState.particles;
    if (particles) {
        ParticleSystem_DrawAll();
    }
}

void EncounterEffect_EndParticleMode(void) {
    EncounterEffectParticles *particles = gEncounterEffectState.particles;
    if (!particles) {
        GF_ASSERT(FALSE);
    }
    ParticleSystem_FreeAll();
    EncounterEffect_FreeParticleGraphics(gEncounterEffectState.particles->fieldSystem);
    ov01_021E631C(gEncounterEffectState.particles->fieldSystem, TRUE);
    ov01_021E6340(gEncounterEffectState.particles->fieldSystem, TRUE);
    Heap_Free(gEncounterEffectState.particles);
    gEncounterEffectState.particles = NULL;
}

#pragma force_active on

void EncounterEffect_LoadParticles(NARC *narc, int member) {
    if (gEncounterEffectState.particles == NULL) {
        GF_AssertFail();
    }
    if (gEncounterEffectState.particles->system != NULL) {
        GF_AssertFail();
    }
    gEncounterEffectState.particles->heap = Heap_Alloc(HEAP_ID_FIELD1, PARTICLE_HEAP_SIZE);
    gEncounterEffectState.particles->system = ParticleSystem_New(EncounterEffectParticles_AllocTexVram, EncounterEffectParticles_AllocPlttVram, gEncounterEffectState.particles->heap, PARTICLE_HEAP_SIZE, TRUE, HEAP_ID_FIELD1);
    if (gEncounterEffectState.particles->system == NULL) {
        GF_AssertFail();
    }
    ParticleSystem_SetPerspectiveType(gEncounterEffectState.particles->system, 1);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, FX32_CONST(900), ParticleSystem_GetCamera(gEncounterEffectState.particles->system));
    {
        void *resource = GfGfxLoader_LoadFromOpenNarc(narc, member, FALSE, HEAP_ID_FIELD1, FALSE);
        ParticleSystem_SetResource(gEncounterEffectState.particles->system, resource, 0, FALSE);
    }
}

#pragma force_active reset

void EncounterEffect_CreateEmitters(u32 count) {
    VecFx32 position = { 0, 0, 0 };
    u32 i;
    if (!gEncounterEffectState.particles) {
        GF_ASSERT(FALSE);
    }
    if (!gEncounterEffectState.particles->system) {
        GF_ASSERT(FALSE);
    }
    for (i = 0; i < count; i++) {
        ParticleSystem_CreateEmitter(gEncounterEffectState.particles->system, i, &position);
    }
}

BOOL EncounterEffect_UpdateParticles(void) {
    EncounterEffectParticles *particles = gEncounterEffectState.particles;
    SPLEmitter *system;
    if (!particles) {
        GF_ASSERT(FALSE);
    }
    system = gEncounterEffectState.particles->system;
    if (!system) {
        return TRUE;
    }
    ParticleSystem_CalcAll();
    return ParticleSystem_GetActiveEmitterCount(gEncounterEffectState.particles->system) == 0;
}

BOOL EncounterEffect_AreParticlesDone(void) {
    return ParticleSystem_GetActiveEmitterCount(gEncounterEffectState.particles->system) == 0;
}

void EncounterEffect_FreeParticles(void) {
    if (!gEncounterEffectState.particles) {
        GF_ASSERT(FALSE);
    }
    ParticleSystem_Free(gEncounterEffectState.particles->system);
    Heap_Free(gEncounterEffectState.particles->heap);
    gEncounterEffectState.particles->system = NULL;
    gEncounterEffectState.particles->heap = NULL;
}

// Waits one frame for the capture, then sets up the 3D and bitmap BGs once the effect asks for it.
void Task_ParticleSetup_VWait(SysTask *task, void *data) {
    EncounterEffectParticles *particles = data;
    switch (particles->state) {
    case 0:
        particles->state++;
        break;
    case 1:
        if (particles->setupRequested == TRUE) {
            EncounterEffect_SetupParticleGraphics(particles->fieldSystem);
            SysTask_Destroy(task);
            particles->state++;
        }
        break;
    }
}

#pragma force_active on

void Task_ParticleSetup_Main(SysTask *task, void *data) {
    EncounterEffectParticles *particles = data;
    if (particles->state == 2) {
        ParticleSystem_ZeroAll();
        VecFx32 position = { 0, 0, 0 };
        VecFx32 scale = sParticleBaseScale;
        MtxFx33 rotation;
        MTX_Identity33_(&rotation);
        NNS_G3dGlbSetBaseTrans(&position);
        NNS_G3dGlbSetBaseScale(&scale);
        NNS_G3dGlbSetBaseRot(&rotation);
        ov01_021E631C(particles->fieldSystem, FALSE);
        ov01_021E6340(particles->fieldSystem, FALSE);
        SetBgPriority(GF_BG_LYR_MAIN_0, 0);
        particles->setupDone = TRUE;
        SysTask_Destroy(task);
    }
}

// 3D on BG0, the captured field image as a 256-colour bitmap on BG3 behind it; BG2 is cleared.
void EncounterEffect_SetupParticleGraphics(FieldSystem *fieldSystem) {
    GraphicsBanks banks = sParticleGraphicsBanks;
    GraphicsModes modes;
    MtxFx22 matrix;
    BgTemplate template;
    GfGfx_SetBanks(&banks);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    modes = sParticleGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    Thunk_ov01_021E6138(fieldSystem->bgConfig);
    reg_G2_BG3CNT = (reg_G2_BG3CNT & 0x43) | 0x4884;
    SetBgPriority(GF_BG_LYR_MAIN_3, 3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    MTX_Identity22_(&matrix);
    G2_SetBG3Affine(&matrix, 0, 0, 0, 0);
    template = sParticleCaptureBgTemplate;
    InitBgFromTemplate(fieldSystem->bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_FIELD1);
    BgClearTilemapBufferAndCommit(fieldSystem->bgConfig, GF_BG_LYR_MAIN_2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

#pragma force_active reset

void EncounterEffect_FreeParticleGraphics(FieldSystem *fieldSystem) {
    FreeBgTilemapBuffer(fieldSystem->bgConfig, GF_BG_LYR_MAIN_2);
    Thunk_BgConfig_Init(fieldSystem->bgConfig);
}

#pragma force_active on

u32 EncounterEffectParticles_AllocTexVram(u32 size, BOOL is4x4Comp) {
    EncounterEffectParticles *particles;
    u32 offset, next;
    if (!gEncounterEffectState.particles) {
        GF_AssertFail();
    }
    if (is4x4Comp) {
        GF_AssertFail();
    }
    particles = gEncounterEffectState.particles;
    offset = particles->texOffset;
    next = offset + size;
    particles->texOffset = next;
    return offset;
}

u32 EncounterEffectParticles_AllocPlttVram(u32 size, BOOL is4Pltt) {
    EncounterEffectParticles *particles;
    u32 offset, next;
    if (!gEncounterEffectState.particles) {
        GF_AssertFail();
    }
    if (is4Pltt) {
        GF_AssertFail();
    }
    particles = gEncounterEffectState.particles;
    offset = particles->plttOffset;
    next = offset + size;
    particles->plttOffset = next;
    return offset;
}

#pragma force_active reset

SysTask *EncounterEffectHBlankWave_StartTask(EncounterEffectHBlankWave *wave) {
    return SysTask_CreateOnVBlankQueue(Task_EncounterEffectHBlankWave, wave, 1024);
}

void Task_EncounterEffectHBlankWave(SysTask *task, void *data) {
    EncounterEffectHBlankWave *wave = data;
    if (wave->frames >= 2) {
        ScanlineEffect_SwapBuffers(wave->effect);
        wave->frames = 0;
    }
    ScanlineEffect_RestartDma(wave->effect);
    wave->frames++;
}

void EncounterEffectHBlankWave_Init(EncounterEffectHBlankWave *wave, int heapID) {
    wave->effect = ScanlineEffect_New((enum HeapID)heapID);
    wave->frames = 0;
    wave->task = EncounterEffectHBlankWave_StartTask(wave);
}

void EncounterEffectHBlankWave_Free(EncounterEffectHBlankWave *wave) {
    SysTask_Destroy(wave->task);
    ScanlineEffect_Stop(wave->effect);
    ScanlineEffect_Delete(wave->effect);
}

void EncounterEffectHBlankWave_Start(EncounterEffectHBlankWave *wave, u8 firstLine, u8 lastLine, u32 step, fx32 amplitude, s16 speed, void *destination, u32 fill, u32 priority) {
    ScanlineEffect_StartWave(wave->effect, firstLine, lastLine, step, amplitude, speed, destination, fill, priority);
}
