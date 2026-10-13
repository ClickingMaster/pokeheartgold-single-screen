#include <string.h>

#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "assert.h"
#include "font_sprite.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "touchscreen.h"

// VS Recorder menu screen (VS_RECORDER_SCREEN_MENU): the choice rows on the touch screen, the path rows of the entries
// chosen so far, row movement between the two columns, the colour scheme arrows and the exit fade.

// moves one menu row sprite (and its label) to a target in a fixed number of frames
typedef struct VsRecorderRowMove {
    s16 x;
    s16 y;
    s16 targetX;
    s16 targetY;
    s16 stepX;
    s16 stepY;
    u8 unk_0C[0x10];
    u8 frames;
    u8 state;
    u8 unk_1E[2];
    ManagedSprite *sprite;
    TextOBJ *text;
    OrbitCursor *cursor; // follows the row, or NULL
    int *busy;           // set every frame the row moves
    int *arrived;        // set two frames before the row arrives
} VsRecorderRowMove;     // size 0x34

// VsRecorderWork.screenWork while menu rows move
typedef struct VsRecorderMenuMove {
    int blend;
    int busy;
    int started;
    int arrived;
} VsRecorderMenuMove;

extern const TouchscreenHitbox ov40_02245100; // left edge: previous colour scheme
extern const TouchscreenHitbox ov40_02245104; // right edge: next colour scheme

BOOL VsRecorder_MenuMain(VsRecorderWork *work) {
    if (System_GetTouchNew() && work->menuNoticeShown == 0) {
        VsRecorder_HideNotice(work);
    }
    if (work->mode == 0) {
        switch (VsRecorder_CheckSchemeArrowTouch()) {
        case 1:
            PlaySE(SEQ_SE_PL_BREC57);
            VsRecorder_SetSubState(work, 7);
            return FALSE;
        case 2:
            PlaySE(SEQ_SE_PL_BREC57);
            VsRecorder_SetSubState(work, 8);
            return FALSE;
        }
    }
    VsRecorder_UpdateMenuTouch(work);
    return FALSE;
}

// 1: previous colour scheme, 2: next colour scheme, 0: none
int VsRecorder_CheckSchemeArrowTouch(void) {
    if (TouchscreenHitbox_TouchHeldIsIn(&ov40_02245100)) {
        return 1;
    }
    if (TouchscreenHitbox_TouchHeldIsIn(&ov40_02245104)) {
        return 2;
    }
    return 0;
}

BOOL VsRecorder_MenuInvalid(VsRecorderWork *work) {
    GF_ASSERT(FALSE);
    work->cursorHidden = 1;
    VsRecorder_SetSubState(work, 0);
    return VsRecorder_MenuMain(work);
}

#pragma force_active on

BOOL VsRecorder_MenuExit(VsRecorderWork *work) {
    switch (work->step) {
    case 0:
        work->fade = 0;
        work->counter = 0;
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        VsRecorder_SetVBlankPaused(work, TRUE);
        work->step++;
        break;
    case 1:
        if (work->counter != 16) {
            work->counter += 2;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFE, (u8)work->counter, (u16)VsRecorder_GetMenuBlendColor(work));
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, (u8)work->counter, (u16)VsRecorder_GetMenuBlendColor(work));
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0x3FFE, (u8)work->counter, (u16)VsRecorder_GetMenuBlendColor(work));
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, (u8)work->counter, (u16)VsRecorder_GetMenuBlendColor(work));
        } else {
            work->counter = 16;
            work->step++;
        }
        break;
    case 2:
        if (work->counter != 0) {
            work->counter -= 4;
        } else {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_109);
            work->step++;
        }
        break;
    default:
        if (IsPaletteFadeFinished() == TRUE) {
            VsRecorder_SetVBlankPaused(work, FALSE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL VsRecorder_MenuSlideIn(VsRecorderWork *work) {
    if (VsRecorder_SlideInChoiceRows(work)) {
        if (work->cursorHidden == 1) {
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            VsRecorder_SetSubState(work, 0);
        } else {
            VsRecorder_SetSubState(work, 1);
        }
    }
    return FALSE;
}

BOOL VsRecorder_MenuShow(VsRecorderWork *work) {
    if (work->cursorHidden == 1) {
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        VsRecorder_SetSubState(work, 0);
    } else {
        VsRecorder_SetSubState(work, 1);
    }
    return FALSE;
}

void VsRecorder_RowMoveTask(SysTask *task, void *data) {
    s16 x;
    s16 y;
    VsRecorderRowMove *move = data;

    switch (move->state) {
    case 0:
        move->stepX = (move->targetX - move->x) / move->frames;
        move->stepY = (move->targetY - move->y) / move->frames;
        move->state++;
    case 1:
        VsRecorder_GetSpritePos(move->sprite, &x, &y);
        move->frames--;
        if (move->frames == 0) {
            x = move->targetX;
            y = move->targetY;
            move->state++;
        } else {
            x = x + move->stepX;
            y = y + move->stepY;
        }
        if (move->cursor != NULL) {
            OrbitCursor_SetTarget(move->cursor, (s16)(x + 16), y);
            if (move->frames == 2) {
                *move->arrived = TRUE;
            }
        }
        VsRecorder_SetSpritePos(move->sprite, x, y);
        TextOBJ_SetPosition(move->text, 36, -8);
        *move->busy = TRUE;
        break;
    default:
        Heap_Free(move);
        SysTask_Destroy(task);
        break;
    }
}

#pragma force_active on

// the touched choice moves into the path column, the other choices drop out; then the entry's action or child menu
BOOL VsRecorder_MenuSelect(VsRecorderWork *work) {
    VsRecorderRowMove *move;
    int i;
    const VsRecorderMenuEntry *menu;
    VsRecorderMenuMove *menuMove;
    VsRecorderMenuMove *newMenuMove;

    switch (work->step) {
    case 0: {
        newMenuMove = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderMenuMove));
        MI_CpuFill8(newMenuMove, 0, sizeof(VsRecorderMenuMove));
        work->screenWork = newMenuMove;
        newMenuMove->started = FALSE;
        i = 0;
        if (work->choiceCount > 0) {
            do {
                move = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderRowMove));
                memset(move, 0, sizeof(VsRecorderRowMove));
                VsRecorder_GetSpritePos(work->choiceRows[i].sprite, &move->x, &move->y);
                move->sprite = work->choiceRows[i].sprite;
                move->text = work->choiceRows[i].text;
                move->busy = &newMenuMove->busy;
                move->arrived = &newMenuMove->arrived;
                if (i == work->menuCursor) {
                    move->cursor = work->orbitCursors[0];
                    move->targetX = move->x;
                    move->targetY = -48;
                    move->frames = 8;
                } else {
                    move->cursor = NULL;
                    move->targetX = move->x;
                    move->targetY = 205 + (5 - work->choiceCount) * 16;
                    if (move->targetY >= 221) {
                        move->targetY = 221;
                    }
                    move->frames = 8;
                }
                SysTask_CreateOnMainQueue(VsRecorder_RowMoveTask, move, 0x2000);
                i++;
            } while (i < work->choiceCount);
        }
        newMenuMove->blend = 0;
        work->step++;
        break;
    }
    case 1: {
        menuMove = work->screenWork;
        if (menuMove->blend != 16) {
            menuMove->blend += 2;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, (u8)menuMove->blend, (u16)work->blendColor);
        }
        if (menuMove->arrived == TRUE) {
            i = 0;
            if (menuMove->started == FALSE) {
                menuMove->started = TRUE;
                do {
                    move = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderRowMove));
                    memset(move, 0, sizeof(VsRecorderRowMove));
                    VsRecorder_GetSpritePos(work->pathRows[i].sprite, &move->x, &move->y);
                    move->sprite = work->pathRows[i].sprite;
                    move->text = work->pathRows[i].text;
                    move->busy = &menuMove->busy;
                    move->cursor = NULL;
                    move->targetX = move->x;
                    if (work->menuDepth == i) {
                        move->targetX = move->x;
                        move->targetY = 169;
                        menu = work->menu;
                        ManagedSprite_SetAnim(work->pathRows[i].sprite, menu[work->menuCursor].anim);
                        VsRecorder_SetMenuRowText(work, &work->pathRows[work->menuDepth], work->menu[work->menuCursor].msgId, &work->menu[work->menuCursor]);
                        TextOBJ_SetSpritesDrawFlag(work->pathRows[work->menuDepth].text, TRUE);
                        ManagedSprite_SetDrawFlag(work->pathRows[i].sprite, TRUE);
                    } else {
                        move->targetX = move->x - 4;
                        move->targetY = (work->menuDepth - i) * 16 + ((4 - (work->menuDepth - i)) * 36 + 25);
                    }
                    move->frames = 4;
                    SysTask_CreateOnMainQueue(VsRecorder_RowMoveTask, move, 0x2000);
                    i++;
                } while (i <= work->menuDepth);
            }
            menuMove->busy = menuMove->arrived;
            menuMove->arrived = FALSE;
        }
        i = 0;
        if (work->menuDepth > 0) {
            do {
                // the current entry's row is not in this range, so every path row is dimmed
                if (i == work->menuDepth) {
                    ManagedSprite_SetPaletteOverrideOffset(work->pathRows[i].sprite, 1);
                } else {
                    ManagedSprite_SetPaletteOverrideOffset(work->pathRows[i].sprite, 2);
                }
                i++;
            } while (i < work->menuDepth);
        }
        if (menuMove->busy == FALSE) {
            work->step++;
        }
        menuMove->busy = FALSE;
        break;
    }
    case 2:
        menu = work->menu;
        if (menu[work->menuCursor].child == NULL) {
            VsRecorder_MenuRunAction(work);
        } else {
            VsRecorder_SetSubState(work, 5);
        }
        VsRecorder_MenuEnterChild(work);
        Heap_Free(work->screenWork);
        break;
    }
    return FALSE;
}

#pragma force_active reset

// back: the last path row returns to the choices, the earlier path rows move down, then the parent menu slides in
BOOL VsRecorder_MenuBack(VsRecorderWork *work) {
    VsRecorderMenuMove *menuMove;
    VsRecorderRowMove *move;
    int i;
    int depth;
    int y;

    switch (work->step) {
    case 0:
        menuMove = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderMenuMove));
        MI_CpuFill8(menuMove, 0, sizeof(VsRecorderMenuMove));
        work->screenWork = menuMove;
        i = 0;
        do {
            move = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderRowMove));
            memset(move, 0, sizeof(VsRecorderRowMove));
            VsRecorder_GetSpritePos(work->pathRows[i].sprite, &move->x, &move->y);
            move->sprite = work->pathRows[i].sprite;
            move->text = work->pathRows[i].text;
            move->busy = &menuMove->busy;
            move->arrived = &menuMove->arrived;
            move->cursor = NULL;
            move->targetX = move->x;
            if (work->menuDepth - 1 == i) {
                move->cursor = work->orbitCursors[0];
                move->targetX = move->x;
                y = 217;
            } else {
                move->targetX = move->x + 4;
                depth = work->menuDepth - i;
                y = (5 - depth) * 36 + 29 + depth * 16;
            }
            move->targetY = y;
            move->frames = 4;
            SysTask_CreateOnMainQueue(VsRecorder_RowMoveTask, move, 0x2000);
            i++;
        } while (i <= work->menuDepth - 1);
        work->step++;
        break;
    case 1:
        menuMove = work->screenWork;
        if (menuMove->arrived == TRUE) {
            for (i = 0; i < work->choiceCount; i++) {
                move = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderRowMove));
                memset(move, 0, sizeof(VsRecorderRowMove));
                VsRecorder_GetSpritePos(work->choiceRows[i].sprite, &move->x, &move->y);
                move->sprite = work->choiceRows[i].sprite;
                move->text = work->choiceRows[i].text;
                move->busy = &menuMove->busy;
                move->arrived = &menuMove->arrived;
                move->cursor = NULL;
                move->targetX = move->x;
                move->targetY = (5 - work->choiceCount) * 16 + 205;
                if (move->targetY >= 221) {
                    move->targetY = 221;
                }
                move->frames = 8;
                SysTask_CreateOnMainQueue(VsRecorder_RowMoveTask, move, 0x2000);
            }
            menuMove->blend = 0;
            work->step++;
            --work->menuDepth;
            for (i = 0; i < work->menuDepth; i++) {
                if (i == work->menuDepth - 1) {
                    ManagedSprite_SetPaletteOverrideOffset(work->pathRows[i].sprite, 1);
                } else {
                    ManagedSprite_SetPaletteOverrideOffset(work->pathRows[i].sprite, 2);
                }
            }
        }
        break;
    case 2:
        menuMove = work->screenWork;
        if (menuMove->blend != 16) {
            menuMove->blend = menuMove->blend + 2;
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, menuMove->blend, work->blendColor);
        }
        if (menuMove->busy == FALSE) {
            work->step++;
        }
        menuMove->busy = FALSE;
        break;
    default:
        VsRecorder_MenuRestore(work);
        Heap_Free(work->screenWork);
        VsRecorder_SetSubState(work, 5);
        break;
    }
    return FALSE;
}

// left colour scheme arrow: fade to the scheme colour, swap the palettes, fade back
BOOL VsRecorder_MenuPrevScheme(VsRecorderWork *work) {
    switch (work->step) {
    case 0:
        work->fade = 0;
        if (*(volatile u8 *)&work->colorScheme != 0) {
            work->colorScheme = work->colorScheme - 1;
        } else {
            work->colorScheme = 6;
        }
        work->blendColor = VsRecorder_GetBlendColor(work);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepFade(&work->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, work->fade, work->blendColor);
        break;
    case 2:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        VsRecorder_ReloadSchemePalettes(work, work->colorScheme);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, work->fade, work->blendColor);
        work->step++;
        break;
    case 3:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->step++;
        break;
    case 4:
        if (VsRecorder_StepFade(&work->fade, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, work->fade, work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, 0);
        break;
    }
    return FALSE;
}
