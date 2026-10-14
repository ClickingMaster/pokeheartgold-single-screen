#include "global.h"

#include "field/encounter_effect.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"

typedef struct KimonoGirlEffectWork {
    EncounterEffectQuadInterp scroll;
    u32 holdFrames;
    BOOL scrolling;
} KimonoGirlEffectWork;

void EncounterEffect_LoadBgGraphicsMirrored(NARC *narc, int member1, int member2, int member3, int palette, int paletteCount, BgConfig *bgConfig, int bgId1, int bgId2);
extern void SetBothScreensBrightnessByColor(); // called without its color argument here

// BgTemplates for BG1 and BG3 (512x256, 16 colors; screen base 0x0000 / 0x1000, char base 0x4000 /
// 0x8000, priority 0 / 1). Kept as byte arrays: MWCC places char arrays in their own .rodata sections.
const u8 gKimonoGirlEffectBg1Template[sizeof(BgTemplate)] __attribute__((aligned(4))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
};
const u8 gKimonoGirlEffectBg3Template[sizeof(BgTemplate)] __attribute__((aligned(4))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 3, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0
};

// Two halves of a picture slide in from both sides, hold, then slide out again.
void EncounterEffect_KimonoGirl(SysTask *task, EncounterEffectTask *effect) {
    KimonoGirlEffectWork *work = effect->work;

    switch (effect->state) {
    case 0:
        effect->work = Heap_Alloc(HEAP_ID_FIELD1, sizeof(KimonoGirlEffectWork));
        memset(effect->work, 0, sizeof(KimonoGirlEffectWork));
        work = effect->work;
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFFE0FF) | 0x1500;
        FreeBgTilemapBuffer(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
        InitBgFromTemplate(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, (const BgTemplate *)gKimonoGirlEffectBg1Template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1);
        FreeBgTilemapBuffer(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        InitBgFromTemplate(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, (const BgTemplate *)gKimonoGirlEffectBg3Template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        EncounterEffect_LoadBgGraphicsMirrored(effect->narc, 168, 167, 166, 0, 0, effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, GF_BG_LYR_MAIN_3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(8, 0);
        BG_SetMaskColor(GF_BG_LYR_MAIN_1, 0);
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 128);
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 256);
        GfGfx_EngineASetPlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ);
        effect->state = 1;
        break;
    case 1:
        EncounterEffect_StartFlash(1, -16, -16, &effect->fadeDone, 2);
        effect->state = 2;
        break;
    case 2:
        if (effect->fadeDone) {
            effect->state = 3;
        }
        break;
    case 3:
        EncounterEffectQuadInterp_Init(&work->scroll, FX32_CONST(128), 0, FX32_CONST(5), 18);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        work->scrolling = TRUE;
        effect->state = 4;
        break;
    case 4:
        if (EncounterEffectQuadInterp_Update(&work->scroll)) {
            effect->state = 5;
        }
        break;
    case 5:
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 384);
        work->scrolling = FALSE;
        if (work->holdFrames++ > 5) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
            work->scrolling = TRUE;
            work->holdFrames = 0;
            effect->state = 6;
        }
        break;
    case 6:
        EncounterEffectQuadInterp_Init(&work->scroll, 0, FX32_CONST(128), FX32_CONST(5), 18);
        work->scrolling = TRUE;
        effect->state = 7;
        break;
    case 7:
        if (EncounterEffectQuadInterp_Update(&work->scroll)) {
            effect->state = 8;
        }
        break;
    case 8:
        work->scrolling = FALSE;
        SetBothScreensBrightnessByColor();
        if (effect->done) {
            *effect->done = TRUE;
        }
        EncounterEffect_Finish(effect, task);
        return;
    }
    if (work->scrolling) {
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, work->scroll.value >> FX32_SHIFT);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 384 - (work->scroll.value >> FX32_SHIFT));
        ScheduleSetBgPosText(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    }
}
