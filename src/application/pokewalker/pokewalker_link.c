// Pokewalker connection app, mode 3 = link with the Pokewalker (first part): the mode's enter/update/exit, choosing a
// course for the Stroll (pages of 5 unlocked courses), the instructions shown before connecting, the Pokemon picture
// and sub screen effects, and starting / following the IR requests. linkAction selects what is done: 0 = send a Pokemon on
// a Stroll, 1 / 2 = receive the Pokemon and gifts, 4 = send the Pokemon back.
// The state functions are in the table sLinkStates (asm data); the rest of them follow in the next file.

#include "global.h"

#include "constants/sndseq.h"

#include "application/pokewalker/pokewalker_app.h"
#include "msgdata/msg/msg_0274.h"

#include "math_util.h"
#include "message_format.h"
#include "sound_playback.h"
#include "string_util.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "unk_0200FA24.h"

typedef int (*PokewalkerAppStateFunc)(PokewalkerApp *app);

#define LINK_MOTION(app) ((PokewalkerAppMotion *)(app)->unk_1F2C0)

extern const PokewalkerAppStateFunc sLinkStates[];
extern const s32 sCourseMenuCursorPositions[];

int PokewalkerApp_Link_Enter(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    PokewalkerApp_HideSprite(app, 7);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_0);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_1);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    app->subBgScrollX = 0;
    return 2;
}

int PokewalkerApp_Link_Update(PokewalkerApp *app) {
    app->state = sLinkStates[app->state](app);
    if (app->state == 67) {
        app->state = 0;
        return 3;
    }
    PokewalkerApp_UpdateParticles(app);
    return 2;
}

int PokewalkerApp_Link_Exit(PokewalkerApp *app) {
    app->subBgScrollX = 0;
    return 1;
}

#pragma force_active on
int PokewalkerApp_Link_Start(PokewalkerApp *app) {
    if (app->linkAction == 1) {
        app->nextState = 1;
        return 19;
    } else if (app->linkAction == 2) {
        app->nextState = 22;
        return 19;
    } else if (app->linkAction == 4) {
        return 43;
    } else if (app->strollMonSpecies) {
        return 5;
    }
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_0);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_1);
    PokewalkerApp_HideSprite(app, 3);
    PokewalkerApp_HideSprite(app, 4);
    PokewalkerApp_StartMessage(app, 2, msg_0274_00011, 3);
    return 64;
}
#pragma force_active reset

int PokewalkerApp_Link_PrintPointAtCard(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00005);
    PokewalkerApp_StartSubBgScroll(app);
    return 2;
}

int PokewalkerApp_Link_WaitPointAtCard(PokewalkerApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == FALSE) {
        return 26;
    }
    return 2;
}

int PokewalkerApp_Link_FadeOutToSelectMon(PokewalkerApp *app) {
    BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_154);
    return 4;
}

int PokewalkerApp_Link_WaitFadeOutToSelectMon(PokewalkerApp *app) {
    if (IsPaletteFadeFinished() == TRUE) {
        app->nextMode = 7;
        return 66;
    }
    return 4;
}

int PokewalkerApp_Link_FadeIn(PokewalkerApp *app) {
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 1, HEAP_ID_154);
    return 6;
}

int PokewalkerApp_Link_WaitFadeIn(PokewalkerApp *app) {
    if (IsPaletteFadeFinished() == TRUE) {
        return 7;
    }
    return 6;
}

int PokewalkerApp_Link_PrintSelectCourse(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerApp_HideSprite(app, 3);
    PokewalkerApp_HideSprite(app, 4);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00012);
    return 8;
}

int PokewalkerApp_Link_WaitSelectCourse(PokewalkerApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == FALSE) {
        PokewalkerApp_RemoveWindowBank(app, 1);
        PokewalkerApp_RemoveWindowBank(app, 0);
        PokewalkerApp_AddWindowBank(app, 3);
        PokewalkerApp_AddWindowBank(app, 2);
        return 9;
    }
    return 8;
}

int PokewalkerApp_GetCoursePageCount(PokewalkerApp *app) {
    return ((int)app->numCourses - 1) / 5 + 1;
}

#pragma force_active on
void PokewalkerApp_PrintCoursePage(PokewalkerApp *app) {
    int i;
    String *string;
    for (i = 0; i < 5; i++) {
        s8 course = app->courseList[i + app->coursePage * 5];
        if (course >= 0) {
            string = NewString_ReadMsgData(app->msgData, course + msg_0274_00091);
        } else {
            string = NewString_ReadMsgData(app->msgData, msg_0274_00118);
        }
        PokewalkerApp_PrintString(app, i, string, 4, MAKE_TEXT_COLOR(15, 1, 0));
        String_Delete(string);
    }
    BufferIntegerAsString(app->msgFormat, 10, app->coursePage + 1, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferIntegerAsString(app->msgFormat, 11, PokewalkerApp_GetCoursePageCount(app), 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(app->msgFormat, app->msgData, msg_0274_00081, HEAP_ID_154);
    PokewalkerApp_PrintString(app, 7, string, 0, MAKE_TEXT_COLOR(1, 14, 0));
    String_Delete(string);
    PokewalkerApp_PrintCourseInfo(app, app->coursePage, app->menuSelection);
}
#pragma force_active reset

// Course name and description of the selected course on the sub screen.
void PokewalkerApp_PrintCourseInfo(PokewalkerApp *app, int page, int selection) {
    String *string;
    if (selection == 5 || PokewalkerApp_GetSelectedCourse(app) == -1) {
        string = NewString_ReadMsgData(app->msgData, msg_0274_00118);
        PokewalkerApp_PrintSubString(app, 0, string, 0, MAKE_TEXT_COLOR(1, 2, 0));
        String_Delete(string);
        FillWindowPixelBuffer(&app->subWindows[1], 0);
        CopyWindowToVram(&app->subWindows[1]);
    } else {
        string = NewString_ReadMsgData(app->msgData, PokewalkerApp_GetSelectedCourse(app) + msg_0274_00091);
        PokewalkerApp_PrintSubString(app, 0, string, 0, MAKE_TEXT_COLOR(1, 2, 0));
        String_Delete(string);
        string = NewString_ReadMsgData(app->msgData, PokewalkerApp_GetSelectedCourse(app) + msg_0274_00119);
        PokewalkerApp_PrintSubString(app, 1, string, 0, MAKE_TEXT_COLOR(1, 2, 0));
        String_Delete(string);
    }
}

#pragma force_active on
int PokewalkerApp_ListUnlockedCourses(PokewalkerApp *app) {
    int course, count;
    app->coursePage = 0;
    app->menuSelection = 0;
    MI_CpuFill8(app->courseList, -1, sizeof(app->courseList));
    count = 0;
    course = count;
    do {
        if (Pokewalker_CourseIsUnlocked(app->pokewalker, course)) {
            app->courseList[count] = course;
            count++;
        }
        course++;
    } while (course < 27);
    return count;
}
#pragma force_active reset

int PokewalkerApp_Link_ShowCourseMenu(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
    PokewalkerApp_LoadTilemap(app, 5, 17);
    PokewalkerApp_LoadTilemap(app, 2, 23);
    app->numCourses = PokewalkerApp_ListUnlockedCourses(app);
    PokewalkerApp_PrintCoursePage(app);
    PokewalkerApp_PrintExit(app, 6);
    PokewalkerApp_UpdatePageArrows(app, app->coursePage);
    PokewalkerApp_SetCursorPosition(app, sCourseMenuCursorPositions[app->menuSelection]);
    return 10;
}

// Sprites 3 / 4 = arrows to the previous / next page.
void PokewalkerApp_UpdatePageArrows(PokewalkerApp *app, int page) {
    int numPages = PokewalkerApp_GetCoursePageCount(app);
    if (app->numCourses > 5) {
        if (page == 0) {
            PokewalkerApp_HideSprite(app, 3);
            PokewalkerApp_ShowSprite(app, 4);
        } else if (page > 0 && page < numPages - 1) {
            PokewalkerApp_ShowSprite(app, 3);
            PokewalkerApp_ShowSprite(app, 4);
        } else if (page == numPages - 1) {
            PokewalkerApp_ShowSprite(app, 3);
            PokewalkerApp_HideSprite(app, 4);
        }
    } else {
        PokewalkerApp_HideSprite(app, 3);
        PokewalkerApp_HideSprite(app, 4);
    }
}

int PokewalkerApp_GetSelectedCourse(PokewalkerApp *app) {
    return app->courseList[app->coursePage * 5 + app->menuSelection];
}

int PokewalkerApp_Link_HandleCourseMenu(PokewalkerApp *app) {
    u32 button = PokewalkerApp_GetTouchedButton(4);
    if (button != -1) {
        if (button <= 4) {
            app->menuSelection = button;
            if (PokewalkerApp_GetSelectedCourse(app) >= 0) {
                PokewalkerApp_SelectMenuItem(app, button, sCourseMenuCursorPositions[button], 11);
                PlaySE(SEQ_SE_DP_DECIDE);
                PokewalkerApp_PrintCourseInfo(app, app->coursePage, app->menuSelection);
                return 65;
            }
            PokewalkerApp_PrintCourseInfo(app, app->coursePage, app->menuSelection);
            PlaySE(SEQ_SE_DP_SELECT);
            PokewalkerApp_SetCursorPosition(app, sCourseMenuCursorPositions[app->menuSelection]);
        } else if (button == 6) {
            PokewalkerApp_ChangeCoursePage(app, -1);
            PokewalkerApp_UpdatePageArrows(app, app->coursePage);
            PokewalkerApp_SetSpriteAnim(app, 3, 2);
        } else if (button == 7) {
            PokewalkerApp_ChangeCoursePage(app, 1);
            PokewalkerApp_UpdatePageArrows(app, app->coursePage);
            PokewalkerApp_SetSpriteAnim(app, 4, 3);
        } else {
            PokewalkerApp_SelectMenuItem(app, button, sCourseMenuCursorPositions[button], 11);
            PlaySE(SEQ_SE_DP_DECIDE);
            return 65;
        }
        return 10;
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        if (app->menuSelection > 0) {
            app->menuSelection--;
            PlaySE(SEQ_SE_DP_SELECT);
        }
        PokewalkerApp_SetCursorPosition(app, sCourseMenuCursorPositions[app->menuSelection]);
        PokewalkerApp_PrintCourseInfo(app, app->coursePage, app->menuSelection);
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        if (app->menuSelection < 5) {
            app->menuSelection++;
            PlaySE(SEQ_SE_DP_SELECT);
        }
        PokewalkerApp_SetCursorPosition(app, sCourseMenuCursorPositions[app->menuSelection]);
        PokewalkerApp_PrintCourseInfo(app, app->coursePage, app->menuSelection);
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        PokewalkerApp_ChangeCoursePage(app, -1);
        PokewalkerApp_UpdatePageArrows(app, app->coursePage);
        PokewalkerApp_SetSpriteAnim(app, 3, 2);
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        PokewalkerApp_ChangeCoursePage(app, 1);
        PokewalkerApp_UpdatePageArrows(app, app->coursePage);
        PokewalkerApp_SetSpriteAnim(app, 4, 3);
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        if (app->menuSelection > 4) {
            PokewalkerApp_SelectMenuItem(app, app->menuSelection, sCourseMenuCursorPositions[app->menuSelection], 11);
            PlaySE(SEQ_SE_DP_DECIDE);
            return 65;
        }
        if (PokewalkerApp_GetSelectedCourse(app) >= 0) {
            PokewalkerApp_SelectMenuItem(app, app->menuSelection, sCourseMenuCursorPositions[app->menuSelection], 11);
            PlaySE(SEQ_SE_DP_DECIDE);
            return 65;
        }
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PokewalkerApp_SelectMenuItem(app, 5, 3, 11);
        PlaySE(SEQ_SE_DP_SELECT);
        return 65;
    }
    return 10;
}

int PokewalkerApp_Link_EndCourseMenu(PokewalkerApp *app) {
    PokewalkerApp_HideCursor(app);
    switch (app->menuSelection) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        PokewalkerApp_SetCursorPosition(app, sCourseMenuCursorPositions[app->menuSelection]);
        if (PokewalkerApp_GetSelectedCourse(app) < 0) {
            break;
        }
        PokewalkerApp_HideCursor(app);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
        app->courseId = PokewalkerApp_GetSelectedCourse(app);
        return 12;
    case 5: // Exit
        PokewalkerApp_RemoveWindowBank(app, 2);
        PokewalkerApp_RemoveWindowBank(app, 3);
        PokewalkerApp_AddWindowBank(app, 1);
        PokewalkerApp_AddWindowBank(app, 0);
        app->strollMonSpecies = SPECIES_NONE;
        PokewalkerApp_HideCursor(app);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
        return 0;
    default:
        GF_ASSERT(app->menuSelection < 6);
        break;
    }
    return 10;
}

void PokewalkerApp_ChangeCoursePage(PokewalkerApp *app, int direction) {
    if (direction < 0) {
        if (app->menuSelection != 5 && (s8)app->coursePage > 0) {
            app->coursePage--;
            app->menuSelection = 0;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    } else {
        if (app->menuSelection != 5) {
            int page = (s8)app->coursePage;
            if (page < PokewalkerApp_GetCoursePageCount(app) - 1) {
                app->coursePage++;
                app->menuSelection = 0;
                PlaySE(SEQ_SE_DP_SELECT);
            }
        }
    }
    PokewalkerApp_SetCursorPosition(app, sCourseMenuCursorPositions[app->menuSelection]);
    PokewalkerApp_PrintCoursePage(app);
}

int PokewalkerApp_Link_PrintConfirmCourse(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerApp_HideSprite(app, 3);
    PokewalkerApp_HideSprite(app, 4);
    PokewalkerApp_RemoveWindowBank(app, 2);
    PokewalkerApp_RemoveWindowBank(app, 3);
    PokewalkerApp_AddWindowBank(app, 1);
    PokewalkerApp_AddWindowBank(app, 0);
    BufferPokewalkerCourseName(app->msgFormat, 0, (u8)app->courseId);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00021);
    return 13;
}

int PokewalkerApp_Link_WaitConfirmCourse(PokewalkerApp *app) {
    return !TextPrinterCheckActive(app->textPrinterId) ? 14 : 13;
}

int PokewalkerApp_Link_ShowConfirmCourseYesNo(PokewalkerApp *app) {
    PokewalkerApp_ShowYesNoMenu(app, 0);
    return 15;
}

int PokewalkerApp_Link_HandleConfirmCourseYesNo(PokewalkerApp *app) {
    switch (PokewalkerApp_HandleTwoChoiceMenu(app)) {
    case 0:
        PokewalkerApp_HideCursor(app);
        if (ov112_021E9338(app) == TRUE) {
            app->nextState = 20;
            return 18;
        }
        app->nextState = 16;
        return 18;
    case 1:
        PokewalkerApp_HideCursor(app);
        return 7;
    default:
        return 15;
    }
}

int PokewalkerApp_Link_PrintInstructions(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerApp_HideSprite(app, 3);
    PokewalkerApp_HideSprite(app, 4);
    PokewalkerApp_StartSubBgScroll(app);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00007);
    return 17;
}

int PokewalkerApp_Link_WaitInstructions(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId) && app->monPicAlpha == 31 && app->subBgScrollX == 256) {
        PokewalkerApp_LoadTilemap(app, 2, 14);
        PokewalkerApp_PrintExit(app, 1);
        return 24;
    }
    return 17;
}

int PokewalkerApp_Link_PrintWillConnectWithMon(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerApp_HideSprite(app, 3);
    PokewalkerApp_HideSprite(app, 4);
    PokewalkerApp_FadeInMonPicture(app);
    PokewalkerApp_StartMessage(app, 2, msg_0274_00016, app->nextState);
    return 64;
}

int PokewalkerApp_Link_PrintWillConnect(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerApp_StartMessage(app, 2, msg_0274_00016, app->nextState);
    return 64;
}

int PokewalkerApp_Link_PrintPointAtCardForStroll(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00005);
    PokewalkerApp_StartSubBgScroll(app);
    return 21;
}

int PokewalkerApp_Link_WaitPointAtCardForStroll(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId) && app->monPicAlpha == 31 && app->subBgScrollX == 256) {
        return 30;
    }
    return 21;
}

void PokewalkerApp_ShowMonPicture(PokewalkerApp *app) {
    PokewalkerApp_FadeInMonPicture(app);
}

// Shows the picture of the Pokemon going on the Stroll (sprite 8), alpha-blended in over BG 3.
void PokewalkerApp_FadeInMonPicture(PokewalkerApp *app) {
    PokewalkerApp_LoadMonPicture(app->strollMon, app->monSpriteBuffer, &app->monPicTemplate, 0);
    ManagedSprite_SetOamMode(app->sprites[8], GX_OAM_MODE_XLU);
    PokewalkerApp_ShowSprite(app, 8);
    app->monPicAlpha = 0;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG3, 0, 31);
    app->monFadeTask = SysTask_CreateOnMainQueue((SysTaskFunc)PokewalkerApp_FadeInMonPictureTask, app, 2);
}

void PokewalkerApp_FadeInMonPictureTask(SysTask *task, PokewalkerApp *app) {
    app->monPicAlpha += 2;
    if (app->monPicAlpha >= 31) {
        app->monPicAlpha = 31;
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG3, app->monPicAlpha, 31 - app->monPicAlpha);
    if (app->monPicAlpha == 31) {
        SysTask_Destroy(task);
    }
}

void PokewalkerApp_ScrollSubBgTask(SysTask *task, void *data) {
    u16 *scrollX = &((PokewalkerApp *)data)->subBgScrollX;
    *scrollX += 16;
    if (*scrollX > 256) {
        *scrollX = 256;
        SysTask_Destroy(task);
    }
}

int PokewalkerApp_Link_PrintPointAtCardForGifts(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerApp_RemoveWindowBank(app, 2);
    PokewalkerApp_RemoveWindowBank(app, 3);
    PokewalkerApp_AddWindowBank(app, 0);
    PokewalkerApp_AddWindowBank(app, 1);
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    PokewalkerApp_HideSprite(app, 3);
    PokewalkerApp_HideSprite(app, 4);
    PokewalkerApp_StartSubBgScroll(app);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00005);
    return 23;
}

int PokewalkerApp_Link_WaitPointAtCardForGifts(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId) && app->subBgScrollX == 256) {
        ov112_021EA5A4(app, 2);
        return 28;
    }
    return 23;
}

void PokewalkerApp_ResetLinkError(PokewalkerApp *app) {
    app->linkError = 0;
    app->linkConnected = 0;
}

// Request 0x32: registers the Pokewalker and sends the upload image and the Stroll data.
int PokewalkerApp_Link_StartUpload(PokewalkerApp *app) {
    MIi_CpuClearFast(0, (u32 *)&app->irBlocks, sizeof(PokewalkerIrBlocks));
    MIi_CpuClearFast(0, (u32 *)&app->irStatus, sizeof(PokewalkerIrStatus));
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    PokewalkerApp_BuildUploadData(app);
    PokewalkerIr_RequestUpload(app->uploadImage, (u8 *)&app->strollData, (u8 *)&app->unk_F1D0);
    PokewalkerApp_StartLink(app);
    ov112_021EA5A4(app, 2);
    ManagedSprite_SetAnim(app->sprites[8], 25);
    ManagedSprite_SetAnimationFrame(app->sprites[8], 0);
    PokewalkerApp_ResetLinkError(app);
    return 25;
}

void PokewalkerApp_StartSubBgScroll(PokewalkerApp *app) {
    PokewalkerApp_LoadSubBg1Tilemap(app);
    app->subBgScrollTask = SysTask_CreateOnMainQueue(PokewalkerApp_ScrollSubBgTask, app, 2);
}

void PokewalkerApp_PlayTransferSE(int progress) {
    if (progress != 0 && !IsSEPlaying(SEQ_SE_GS_PHC_POKEMONIDOU)) {
        PlaySE(SEQ_SE_GS_PHC_POKEMONIDOU);
    }
}

// Returns the highest transfer progress seen while the link is up (the progress restarts with each block).
int PokewalkerApp_UpdateLinkProgress(PokewalkerApp *app, int progress) {
    if (ov112_021E7668()) {
        u16 highest = app->linkProgress;
        if (highest < progress) {
            app->linkProgress = progress;
        } else {
            progress = highest;
        }
    } else {
        app->linkProgress = 0;
    }
    return progress;
}

#pragma force_active on
int PokewalkerApp_Link_UpdateUpload(PokewalkerApp *app) {
    u8 progress;
    int error;
    PokewalkerIr_GetChunkProgress();
    progress = PokewalkerApp_UpdateLinkProgress(app, PokewalkerIr_GetTotalProgress());
    if (progress > MTRandom() % 255) {
        PokewalkerApp_SpawnRisingParticle(app);
    }
    PokewalkerApp_UpdateConnectionAnim(app);
    PokewalkerApp_PlayTransferSE(progress);
    PokewalkerApp_SetWalkerIconFrame(app, progress);
    if (progress > 200) {
        if (app->motionStarted == 0) {
            PokewalkerApp_InitMotion(app, LINK_MOTION(app), 2);
            app->motionStarted = 1;
        }
    } else {
        PokewalkerApp_HideMotionSprites(app);
        app->motionStarted = 0;
    }
    error = PokewalkerIr_GetAndClearError();
    switch (error) {
    case 2:
        if (app->linkConnected == 1) {
            app->linkError = 42;
        }
        break;
    case 4:
    case 7:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
        app->linkError = error;
        break;
    case 15: // done
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
        Pokewalker_IncrementLinkCount(app->pokewalker);
        PokewalkerApp_HideSprite(app, 8);
        PokewalkerApp_InitMotion(app, LINK_MOTION(app), 0);
        return 35;
    }
    if (app->linkError) {
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
        app->nextState = 58;
        PokewalkerIr_Stop();
        return 32;
    }
    if (ov112_021E7668() == NULL) {
        PokewalkerApp_ShowSubBg1(app, TRUE);
        if (PokewalkerApp_GetTouchedButton(5) == 0 || (gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B)) {
            app->linkError = 33;
            PokewalkerIr_Stop();
            PokewalkerApp_SelectMenuItem(app, 3, 3, 33);
            PlaySE(SEQ_SE_DP_DECIDE);
            return 65;
        }
    } else {
        if (app->linkConnected == 0) {
            PokewalkerApp_ShowMessageBox(app, 2, msg_0274_00006);
            app->linkConnected = 1;
        }
        PokewalkerApp_ShowSubBg1(app, FALSE);
    }
    return 25;
}
#pragma force_active reset

// Request 0x40: reads the Pokewalker's Pokemon, status and blocks.
int PokewalkerApp_Link_StartReceive(PokewalkerApp *app) {
    MIi_CpuClearFast(0, (u32 *)app->irStatus.records, sizeof(app->irStatus.records));
    PokewalkerIr_RequestReceive((u8 *)&app->walkerMon, (u8 *)&app->irStatus, (u8 *)&app->irBlocks, (u8 *)app->peerRecords);
    PokewalkerApp_StartLink(app);
    PokewalkerApp_ResetLinkError(app);
    ov112_021EA5A4(app, 2);
    ManagedSprite_SetAnimationFrame(app->sprites[8], 0);
    return 27;
}
