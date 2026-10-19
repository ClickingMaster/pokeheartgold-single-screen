#include "field/overlay116_scene_internal.h"

static void LegendaryEffect_ApplyCameraKey(FieldSystem *fieldSystem, const Ov116CameraKey *key);
static void LegendaryEffect_RunBirdSequence(SysTask *sysTask, EncounterEffectTask *task, const Ov116Sequence *seq);

static const Ov116Sequence sLugiaSequence = { 0x28, 5, 8, 0x3C, 0x100, -0x226000, 0x800, 5, 13 };
static const Ov116Sequence sHoOhSequence = { 0x28, 5, 8, 0x3C, 0x100, -0x294000, 0x800, 5, 13 };

enum LegendaryEffectState {
    LEGENDARY_STATE_HIDE_BGS,
    LEGENDARY_STATE_START_FLASH,
    LEGENDARY_STATE_WAIT_FLASH,
    LEGENDARY_STATE_START_PAN,
    LEGENDARY_STATE_PAN,
    LEGENDARY_STATE_START_FADE,
    LEGENDARY_STATE_WAIT_FADE,
    LEGENDARY_STATE_END,
};

enum LegendaryBirdEffectState {
    LEGENDARY_BIRD_STATE_HIDE_BGS,
    LEGENDARY_BIRD_STATE_START_FLASH,
    LEGENDARY_BIRD_STATE_WAIT_FLASH,
    LEGENDARY_BIRD_STATE_START_BLUR,
    LEGENDARY_BIRD_STATE_PERSPECTIVE,
    LEGENDARY_BIRD_STATE_START_ZOOM,
    LEGENDARY_BIRD_STATE_ZOOM,
    LEGENDARY_BIRD_STATE_START_FADE,
    LEGENDARY_BIRD_STATE_WAIT_FADE,
    LEGENDARY_BIRD_STATE_END,
};

static const Ov116CameraKey sPanKeys[] = {
    { 0x29AEC1, 0xD602, 0,      0x05C1, 4 },
    { 0x29AEC1, 0xCF02, 0xFF00, 0x0601, 4 },
    { 0x29AEC1, 0xE602, 0x1000, 0x0691, 4 },
    { 0x29AEC1, 0xD602, 0x0A00, 0x0711, 3 },
    { 0x29AEC1, 0xE102, 0xF000, 0x0780, 3 },
    { 0x29AEC1, 0xC602, 0,      0x0751, 3 },
    { 0x29AEC1, 0xE002, 0xF000, 0x0800, 3 },
    { 0x29AEC1, 0xD602, 0,      0x0802, 3 },
    { 0x29AEC1, 0xD002, 0x1000, 0x0800, 3 },
    { 0x29AEC1, 0xD902, 0xF500, 0x0751, 3 },
    { 0x29AEC1, 0xD002, 0x0A00, 0x04C1, 2 },
    { 0x29AEC1, 0xE002, 0xF000, 0x03C1, 2 },
    { 0x29AEC1, 0xD002, 0xF000, 0x0650, 1 },
    { 0x29AEC1, 0xE002, 0xA000, 0x0241, 1 },
    { 0x29AEC1, 0xE1A2, 0x0500, 0x0500, 1 },
    { 0x29AEC1, 0xD602, 0,      0x0241, 1 },
};

static void LegendaryEffect_ApplyCameraKey(FieldSystem *fieldSystem, const Ov116CameraKey *key) {
    CameraAngle angle;
    Camera_SetPerspectiveAngle(key->perspective, fieldSystem->camera);
    Camera_SetDistance(key->distance, fieldSystem->camera);
    angle.x = key->angleX;
    angle.y = key->angleY;
    angle.z = 0;
    Camera_SetAnglePos(&angle, fieldSystem->camera);
}

void EncounterEffect_Legendary(SysTask *sysTask, EncounterEffectTask *task) {
    Ov116PanWork *work = task->work;
    switch (task->state) {
    case LEGENDARY_STATE_HIDE_BGS:
        task->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov116PanWork));
        memset(task->work, 0, sizeof(Ov116PanWork));
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);
        task->state++;
        break;
    case LEGENDARY_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, 0x10, 0x10, &task->fadeDone, 1);
        task->state++;
        break;
    case LEGENDARY_STATE_WAIT_FLASH:
        if (task->fadeDone != 0) {
            task->state++;
        }
        break;
    case LEGENDARY_STATE_START_PAN:
        work->motionBlur = FieldMotionBlur_Start(3, 0xF);
        work->index = 0;
        work->delay = sPanKeys[work->index].delay;
        task->state++;
        break;
    case LEGENDARY_STATE_PAN:
        if (--work->delay < 0) {
            LegendaryEffect_ApplyCameraKey(task->fieldSystem, &sPanKeys[work->index]);
            work->index++;
            if (work->index >= 16) {
                task->state++;
            } else {
                work->delay = sPanKeys[work->index].delay;
            }
        }
        break;
    case LEGENDARY_STATE_START_FADE:
        BeginNormalPaletteFade(3, 0, 0, 0x7FFF, 10, 1, HEAP_ID_FIELD1);
        task->state++;
        break;
    case LEGENDARY_STATE_WAIT_FADE:
        if (IsPaletteFadeFinished()) {
            task->fadeDone = 0;
            task->state++;
        }
        break;
    case LEGENDARY_STATE_END:
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, 0x7FFF);
        FieldMotionBlur_Stop(&work->motionBlur);
        *(vu16 *)0x04000050 = 0;
        if (task->done != NULL) {
            *task->done = 1;
        }
        EncounterEffect_Finish(task, sysTask);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, 0x7FFF);
        break;
    }
}

static void LegendaryEffect_RunBirdSequence(SysTask *sysTask, EncounterEffectTask *task, const Ov116Sequence *seq) {
    Ov116MoveWork *work = task->work;
    int done;
    switch (task->state) {
    case LEGENDARY_BIRD_STATE_HIDE_BGS:
        task->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov116MoveWork));
        memset(task->work, 0, sizeof(Ov116MoveWork));
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);
        task->state++;
        break;
    case LEGENDARY_BIRD_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, 0x10, 0x10, &task->fadeDone, 1);
        task->state++;
        break;
    case LEGENDARY_BIRD_STATE_WAIT_FLASH:
        if (task->fadeDone != 0) {
            task->state++;
        }
        break;
    case LEGENDARY_BIRD_STATE_START_BLUR: {
        s32 start;
        work->motionBlur = FieldMotionBlur_Start(seq->angleX, seq->angleY);
        start = Camera_GetPerspectiveAngle(task->fieldSystem->camera);
        EncounterEffectInterp_Init(&work->perspective, start, start + seq->perspectiveDelta, seq->perspectiveDuration);
        task->state++;
        break;
    }
    case LEGENDARY_BIRD_STATE_PERSPECTIVE:
        done = EncounterEffectInterp_Update(&work->perspective);
        Camera_SetPerspectiveAngle((u16)work->perspective.value, task->fieldSystem->camera);
        if (done == 1) {
            task->state++;
            work->delay = seq->delay;
        }
        break;
    case LEGENDARY_BIRD_STATE_START_ZOOM: {
        s32 start;
        if (--work->delay < 0) {
            start = Camera_GetDistance(task->fieldSystem->camera);
            EncounterEffectQuadInterp_Init(&work->distance, start, start + seq->distanceDelta, seq->distanceDuration, seq->duration);
            task->state++;
        }
        break;
    }
    case LEGENDARY_BIRD_STATE_ZOOM:
        done = EncounterEffectQuadInterp_Update(&work->distance);
        Camera_SetDistance(work->distance.value, task->fieldSystem->camera);
        if (done == 1) {
            task->state++;
        }
        break;
    case LEGENDARY_BIRD_STATE_START_FADE:
        BeginNormalPaletteFade(3, 0, 0, 0x7FFF, seq->fadeFrames, 1, HEAP_ID_FIELD1);
        task->state++;
        break;
    case LEGENDARY_BIRD_STATE_WAIT_FADE:
        if (IsPaletteFadeFinished()) {
            task->fadeDone = 0;
            task->state++;
        }
        break;
    case LEGENDARY_BIRD_STATE_END:
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, 0x7FFF);
        FieldMotionBlur_Stop(&work->motionBlur);
        *(vu16 *)0x04000050 = 0;
        if (task->done != NULL) {
            *task->done = 1;
        }
        EncounterEffect_Finish(task, sysTask);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, 0x7FFF);
        break;
    }
}

void EncounterEffect_HoOh(SysTask *sysTask, EncounterEffectTask *task) {
    LegendaryEffect_RunBirdSequence(sysTask, task, &sHoOhSequence);
}

void EncounterEffect_Lugia(SysTask *sysTask, EncounterEffectTask *task) {
    LegendaryEffect_RunBirdSequence(sysTask, task, &sLugiaSequence);
}
