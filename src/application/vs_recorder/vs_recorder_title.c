#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "touchscreen.h"

// VS Recorder title screen (VS_RECORDER_SCREEN_TITLE): fade in, drop the orbit cursors, "TOUCH HERE!", then show the
// menu and slide its choice rows in.

// one choice row sliding up along a sine curve
typedef struct VsRecorderChoiceSlide {
    int delay;
    int angle;
    s16 targetY;
    u8 unk_0A[2];
    fx32 baseX;
} VsRecorderChoiceSlide;

// VsRecorderWork.screenWork while the choice rows slide in
typedef struct VsRecorderChoiceSlideIn {
    VsRecorderChoiceSlide rows[5];
    u8 fade;
} VsRecorderChoiceSlideIn;

BOOL VsRecorder_TitleIntro(VsRecorderWork *work) {
    switch (work->step) {
    case 0:
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            VsRecorder_SetSubState(work, 1);
            break;
        }
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_109);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFE, 16, (u16)ov40_0222DAF0(work));
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xBFFF, 16, (u16)ov40_0222DAF0(work));
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0x3FFE, 16, (u16)ov40_0222DAF0(work));
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, 16, (u16)ov40_0222DAF0(work));
        sub_02087A30(work->orbitCursors[1]);
        sub_02087A30(work->orbitCursors[0]);
        work->step++;
        break;
    case 1:
        if (!IsPaletteFadeFinished()) {
            break;
        }
        OrbitCursor_SetPosition(work->orbitCursors[1], 128, 16);
        OrbitCursor_SetTarget(work->orbitCursors[1], 128, 216);
        OrbitCursor_SetVisible(work->orbitCursors[1], TRUE);
        OrbitCursor_SetActive(work->orbitCursors[1], TRUE);
        PlaySE(SEQ_SE_PL_BREC11_2);
        work->step++;
        break;
    case 2:
        work->counter++;
        if (work->counter < 25) {
            break;
        }
        work->counter = 0;
        OrbitCursor_SetPosition(work->orbitCursors[0], 128, -16);
        OrbitCursor_SetTarget(work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetActive(work->orbitCursors[0], TRUE);
        work->step++;
        break;
    case 3:
        work->counter++;
        if (work->counter < 18) {
            break;
        }
        VsRecorder_ShowTouchHere(work);
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        work->counter = 0;
        work->step++;
        break;
    case 4:
        if (work->counter < 16) {
            work->counter += 4;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xF000, (u8)work->counter, 0xFFFF);
        } else {
            work->step++;
        }
        break;
    case 5:
        if (work->counter > 0) {
            work->counter -= 4;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xF000, (u8)work->counter, 0xFFFF);
        } else {
            work->step++;
        }
        break;
    case 6:
        if (System_GetTouchHeld() == TRUE) {
            VsRecorder_HideTouchHere(work);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            sub_02087A54(work->orbitCursors[1]);
            sub_02087A54(work->orbitCursors[0]);
            work->step++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, 1);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_TitleShowMenu(VsRecorderWork *work) {
    switch (work->step) {
    case 0:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_LoadDecorGfx(work);
        VsRecorder_CreateDecorSprites(work);
        VsRecorder_LoadMenuRowGfx(work);
        VsRecorder_CreateMenuRowSprites(work);
        VsRecorder_CreateMenuRowText(work);
        VsRecorder_RefreshChoiceRows(work);
        VsRecorder_RefreshPathRows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            if (work->mode == 0) {
                GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x45, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
            } else {
                GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x37, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
            }
            OrbitCursor_SetPosition(work->orbitCursors[0], 128, 224);
            OrbitCursor_SetTarget(work->orbitCursors[0], 128, 224);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            OrbitCursor_SetActive(work->orbitCursors[0], TRUE);
            VsRecorder_SetScreenIfDone(work, work->resumeScreen, TRUE, work->screenPtr);
            break;
        }
        PlaySE(SEQ_SE_PL_BREC03);
        work->counter = 16;
        work->step++;
        break;
    case 1:
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            if (IsPaletteFadeFinished() == TRUE) {
                work->step++;
            }
            break;
        }
        if (work->counter != 0) {
            work->counter -= 2;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFE, (u8)work->counter, (u16)ov40_0222DAF0(work));
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, (u8)work->counter, (u16)ov40_0222DAF0(work));
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0x3FFE, (u8)work->counter, (u16)ov40_0222DAF0(work));
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, (u8)work->counter, (u16)ov40_0222DAF0(work));
        } else {
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, 16, (u16)work->blendColor);
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
            work->step++;
        }
        break;
    case 2:
        if (work->mode == 0) {
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x45, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
        } else {
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x37, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_109);
        }
        work->step++;
        break;
    default:
        if (work->decor[0].state >= 3) {
            VsRecorder_SetSubState(work, 2);
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_TitleFlash(VsRecorderWork *work) {
    switch (work->step) {
    case 0:
        OrbitCursor_SetPosition(work->orbitCursors[0], 128, 96);
        OrbitCursor_SetTarget(work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetActive(work->orbitCursors[0], TRUE);
        work->counter++;
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)(16 - work->counter), (u16)work->blendColor);
        if (work->counter == 2) {
            work->counter = 15;
            work->step++;
        }
        break;
    case 1:
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)work->counter, (u16)work->blendColor);
        work->step++;
        break;
    case 2:
        work->counter--;
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)work->counter, (u16)work->blendColor);
        if (work->counter == 0) {
            work->step++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, 3);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_TitleSlideInChoices(VsRecorderWork *work) {
    s16 x;
    s16 y;
    BOOL done = VsRecorder_SlideInChoiceRows(work);
    VsRecorder_GetSpritePos(work->choiceRows[0].sprite, &x, &y);
    OrbitCursor_SetTarget(work->orbitCursors[0], (s16)(x + 16), y);
    return done;
}

#pragma force_active on

// slides the choice rows up from the bottom of the touch screen while their palettes fade in; TRUE when all are in place
BOOL VsRecorder_SlideInChoiceRows(VsRecorderWork *work) {
    fx32 posY;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    VsRecorderChoiceSlideIn *data;
    int i;
    int j;
    int done;
    fx32 y;
    fx32 x;

    switch (work->step) {
    case 0:
        data = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderChoiceSlideIn));
        MI_CpuFill8(data, 0, sizeof(VsRecorderChoiceSlideIn));
        work->screenWork = data;
        data->fade = 16;
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, data->fade, work->blendColor);
        for (i = 0; i < work->choiceCount; i++) {
            data->rows[i].delay = i * 4;
            data->rows[i].angle = 90;
            data->rows[i].targetY = 25 + i * 36 + (5 - work->choiceCount) * 16;
            VsRecorder_SetSpritePos(work->choiceRows[i].sprite, 42, (5 - work->choiceCount) * 16 + 169);
            ManagedSprite_GetSpritePositionFxXY(work->choiceRows[i].sprite, &data->rows[i].baseX, &posY);
            ManagedSprite_SetPositonFxXY(work->choiceRows[i].sprite, data->rows[i].baseX, ((5 - work->choiceCount) * 16 + 169) * FX32_ONE);
            TextOBJ_SetPosition(work->choiceRows[i].text, 36, -8);
            TextOBJ_SetSpritesDrawFlag(work->choiceRows[i].text, TRUE);
        }
        work->step++;
        break;
    case 1:
        data = work->screenWork;
        if (data->fade != 0) {
            data->fade--;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, data->fade, work->blendColor);
        }
        done = 0;
        for (j = 0; j < work->choiceCount; j++) {
            if (data->rows[j].delay != 0) {
                data->rows[j].delay--;
                continue;
            }
            VsRecorder_GetSpritePos(work->choiceRows[j].sprite, &x1, &y1);
            if (y1 == data->rows[j].targetY) {
                done++;
                continue;
            }
            if (y1 - 8 > data->rows[j].targetY) {
                y1 -= 8;
            } else {
                y1 = data->rows[j].targetY;
            }
            y = y1 * FX32_ONE;
            x = data->rows[j].baseX + GF_SinDeg(0xFFFF * data->rows[j].angle / 360) * 16;
            data->rows[j].angle -= 4;
            ManagedSprite_SetPositonFxXY(work->choiceRows[j].sprite, x, y);
            TextOBJ_SetPosition(work->choiceRows[j].text, 36, -8);
        }
        if (done == work->choiceCount) {
            work->step++;
        }
        break;
    default:
        data = work->screenWork;
        VsRecorder_GetSpritePos(work->choiceRows[work->menuCursor].sprite, &x2, &y2);
        OrbitCursor_SetPosition(work->orbitCursors[0], x2 + 16, y2);
        OrbitCursor_SetTarget(work->orbitCursors[0], (s16)(x2 + 16), y2);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        VsRecorder_UpdateMenuHitboxes(work);
        Heap_Free(data);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset
