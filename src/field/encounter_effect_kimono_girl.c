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

extern void SetBothScreensBrightnessByColor();

// BgTemplates for BG1 and BG3: 512x256, 16 colors, priority 0 / 1.
const u8 gKimonoGirlEffectBg1Template[sizeof(BgTemplate)] __attribute__((aligned(4))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
};
const u8 gKimonoGirlEffectBg3Template[sizeof(BgTemplate)] __attribute__((aligned(4))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 3, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0
};

enum KimonoGirlState {
    KIMONO_GIRL_STATE_INIT,
    KIMONO_GIRL_STATE_START_FLASH,
    KIMONO_GIRL_STATE_WAIT_FLASH,
    KIMONO_GIRL_STATE_START_SCROLL_IN,
    KIMONO_GIRL_STATE_SCROLL_IN,
    KIMONO_GIRL_STATE_HOLD,
    KIMONO_GIRL_STATE_START_SCROLL_OUT,
    KIMONO_GIRL_STATE_SCROLL_OUT,
    KIMONO_GIRL_STATE_END,
};

// Two halves of a picture slide in from both sides, hold, then slide out again.
void EncounterEffect_KimonoGirl(SysTask *task, EncounterEffectTask *effect) {
    KimonoGirlEffectWork *work = effect->work;

    switch (effect->state) {
    case KIMONO_GIRL_STATE_INIT:
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
        effect->state = KIMONO_GIRL_STATE_START_FLASH;
        break;
    case KIMONO_GIRL_STATE_START_FLASH:
        EncounterEffect_StartFlash(1, -16, -16, &effect->fadeDone, 2);
        effect->state = KIMONO_GIRL_STATE_WAIT_FLASH;
        break;
    case KIMONO_GIRL_STATE_WAIT_FLASH:
        if (effect->fadeDone) {
            effect->state = KIMONO_GIRL_STATE_START_SCROLL_IN;
        }
        break;
    case KIMONO_GIRL_STATE_START_SCROLL_IN:
        EncounterEffectQuadInterp_Init(&work->scroll, FX32_CONST(128), 0, FX32_CONST(5), 18);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        work->scrolling = TRUE;
        effect->state = KIMONO_GIRL_STATE_SCROLL_IN;
        break;
    case KIMONO_GIRL_STATE_SCROLL_IN:
        if (EncounterEffectQuadInterp_Update(&work->scroll)) {
            effect->state = KIMONO_GIRL_STATE_HOLD;
        }
        break;
    case KIMONO_GIRL_STATE_HOLD:
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(effect->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 384);
        work->scrolling = FALSE;
        if (work->holdFrames++ > 5) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
            work->scrolling = TRUE;
            work->holdFrames = 0;
            effect->state = KIMONO_GIRL_STATE_START_SCROLL_OUT;
        }
        break;
    case KIMONO_GIRL_STATE_START_SCROLL_OUT:
        EncounterEffectQuadInterp_Init(&work->scroll, 0, FX32_CONST(128), FX32_CONST(5), 18);
        work->scrolling = TRUE;
        effect->state = KIMONO_GIRL_STATE_SCROLL_OUT;
        break;
    case KIMONO_GIRL_STATE_SCROLL_OUT:
        if (EncounterEffectQuadInterp_Update(&work->scroll)) {
            effect->state = KIMONO_GIRL_STATE_END;
        }
        break;
    case KIMONO_GIRL_STATE_END:
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
