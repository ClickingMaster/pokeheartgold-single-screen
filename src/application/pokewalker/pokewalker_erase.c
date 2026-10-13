// Pokewalker connection app, modes 1 and 2. Mode 1 shows a message and waits for A or a touch, then leaves the app.
// Mode 2 erases the Pokewalker's records: after two confirmations and the step count question it links with the
// Pokewalker, takes the Pokemon on a Stroll back into a PC box, resets the Pokewalker save data and saves.
// The state functions are in tables sMode1States (mode 1) and sEraseRecordsStates (mode 2) (asm data).

#include "global.h"

#include "application/pokewalker/pokewalker_app.h"
#include "msgdata/msg/msg_0274.h"

#include "math_util.h"
#include "message_format.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "save_arrays.h"
#include "sound_playback.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_0200FA24.h"

typedef int (*PokewalkerAppStateFunc)(PokewalkerApp *app);

extern const PokewalkerAppStateFunc sMode1States[];
extern const PokewalkerAppStateFunc sEraseRecordsStates[];

int PokewalkerApp_Mode1_Enter(PokewalkerApp *app) {
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 1, HEAP_ID_154);
    return 2;
}

int PokewalkerApp_Mode1_Update(PokewalkerApp *app) {
    app->state = sMode1States[app->state](app);
    if (app->state == 4) {
        app->nextMode = 11;
        app->state = 0;
        return 3;
    }
    return 2;
}

int PokewalkerApp_Mode1_Exit(PokewalkerApp *app) {
    return 1;
}

int PokewalkerApp_Mode1_PrintMessage(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00147);
    return 1;
}

int PokewalkerApp_Mode1_WaitMessage(PokewalkerApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == FALSE) {
        return 2;
    }
    return 1;
}

#pragma force_active on
int PokewalkerApp_Mode1_WaitInput(PokewalkerApp *app) {
    if (gSystem.newKeys & PAD_BUTTON_A || gSystem.touchNew) {
        return 3;
    }
    return 2;
}
#pragma force_active reset

int PokewalkerApp_Mode1_End(PokewalkerApp *app) {
    return 4;
}

int PokewalkerApp_EraseRecords_Enter(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    ov112_021EA64C(app);
    PokewalkerApp_HideSprite(app, 7);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_0);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_1);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    return 2;
}

int PokewalkerApp_EraseRecords_Update(PokewalkerApp *app) {
    app->state = sEraseRecordsStates[app->state](app);
    if (app->state == 35) {
        app->state = 0;
        return 3;
    }
    PokewalkerApp_UpdateParticles(app);
    return 2;
}

int PokewalkerApp_EraseRecords_Exit(PokewalkerApp *app) {
    PokewalkerApp_HideSprite(app, 8);
    ov112_021EA64C(app);
    return 1;
}

int PokewalkerApp_EraseRecords_PrintCaution(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00003);
    return 1;
}

int PokewalkerApp_EraseRecords_WaitCaution(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        PokewalkerApp_ShowYesNoMenu(app, 1);
        return 2;
    }
    return 1;
}

int PokewalkerApp_EraseRecords_HandleCautionYesNo(PokewalkerApp *app) {
    switch (PokewalkerApp_HandleTwoChoiceMenu(app)) {
    case 0:
        PokewalkerApp_HideCursor(app);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        return 3;
    case 1:
        app->nextMode = 11;
        return 34;
    default:
        return 2;
    }
}

int PokewalkerApp_EraseRecords_PrintWillErase(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00004);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    return 4;
}

int PokewalkerApp_EraseRecords_ShowEraseMenu(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        PokewalkerApp_PrintString(app, 5, app->eraseRecordsText, 4, MAKE_TEXT_COLOR(15, 1, 0));
        PokewalkerApp_LoadTilemap(app, 2, 10);
        PokewalkerApp_PrintExit(app, 1);
        PokewalkerApp_SetCursorPosition(app, 2);
        return 5;
    }
    return 4;
}

int PokewalkerApp_EraseRecords_HandleEraseMenu(PokewalkerApp *app) {
    int selection = PokewalkerApp_GetTouchedButton(1);
    if (selection != TOUCH_MENU_NO_INPUT) {
        PokewalkerApp_SelectMenuItem(app, selection, selection + 2, 6);
        PlaySE(SEQ_SE_DP_DECIDE);
        return 33;
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        if (app->menuSelection != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        PokewalkerApp_SetCursorPosition(app, 2);
        app->menuSelection = 0;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        if (app->menuSelection != 1) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        PokewalkerApp_SetCursorPosition(app, 3);
        app->menuSelection = 1;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        PlaySE(SEQ_SE_DP_DECIDE);
        selection = app->menuSelection;
        PokewalkerApp_SelectMenuItem(app, selection, selection + 2, 6);
        return 33;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_DP_SELECT);
        PokewalkerApp_SelectMenuItem(app, 1, 3, 6);
        return 33;
    }
    return 5;
}

int PokewalkerApp_EraseRecords_EndEraseMenu(PokewalkerApp *app) {
    PokewalkerApp_HideCursor(app);
    ClearWindowTilemapAndCopyToVram(&app->windows[5]);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
    if (app->menuSelection == 0) {
        return 11;
    }
    app->nextMode = 11;
    return 34;
}

int PokewalkerApp_EraseRecords_PrintCarryOverSteps(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00009);
    return 8;
}

#pragma force_active on
int PokewalkerApp_EraseRecords_WaitCarryOverSteps(PokewalkerApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == FALSE) {
        return 9;
    } else {
        return 8;
    }
}
#pragma force_active reset

int PokewalkerApp_EraseRecords_ShowCarryOverStepsMenu(PokewalkerApp *app) {
    PokewalkerApp_ShowSaveEraseStepsMenu(app, 1);
    return 10;
}

#pragma force_active on
int PokewalkerApp_EraseRecords_HandleCarryOverStepsMenu(PokewalkerApp *app) {
    switch (PokewalkerApp_HandleTwoChoiceMenu(app)) {
    case 0:
        PokewalkerApp_HideCursor(app);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        app->eraseSteps = FALSE;
        return 11;
    case 1:
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        PokewalkerApp_HideCursor(app);
        app->eraseSteps = TRUE;
        return 11;
    }
    return 10;
}
#pragma force_active reset

int PokewalkerApp_EraseRecords_PrintConnect(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerApp_StartMessage(app, 2, msg_0274_00016, 13);
    return 12;
}

int PokewalkerApp_EraseRecords_WaitMessage(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->state = app->nextState;
        return app->state;
    }
    return 12;
}

int PokewalkerApp_EraseRecords_PrintPointAtCard(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    PokewalkerApp_StartSubBgScroll(app);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00005);
    return 14;
}

int PokewalkerApp_EraseRecords_WaitPointAtCard(PokewalkerApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == FALSE && app->subBgScrollX == 0x100) {
        ov112_021EA5A4(app, 2);
        return 15;
    }
    return 14;
}

int PokewalkerApp_EraseRecords_PrepareLink(PokewalkerApp *app) {
    PokewalkerIr_RequestEraseRecords(1);
    MIi_CpuClearFast(0, (u32 *)&app->irBlocks, sizeof(PokewalkerIrBlocks));
    MIi_CpuClearFast(0, (u32 *)&app->irStatus, sizeof(PokewalkerIrStatus));
    return 16;
}

int PokewalkerApp_EraseRecords_StartLink(PokewalkerApp *app) {
    PokewalkerApp_StartLink(app);
    return 17;
}

int PokewalkerApp_EraseRecords_UpdateLink(PokewalkerApp *app) {
    int nextState = 17;
    switch (PokewalkerIr_GetAndClearError()) {
    case 2:
        nextState = 18;
        break;
    case 4:
        nextState = 18;
        break;
    case 5:
        nextState = 18;
        break;
    case 7:
        nextState = 18;
        break;
    case 8:
        nextState = 18;
        break;
    case 9:
        nextState = 18;
        break;
    case 11:
        nextState = 18;
        break;
    case 12:
        nextState = 18;
        break;
    case 15:
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
        PokewalkerApp_ReturnWalkerMonToBox(app);
        Pokewalker_Reset(app->pokewalker);
        app->walkerSteps = 0;
        app->walkerWatts = 0;
        PokewalkerIr_Stop();
        return 20;
    }
    if (nextState != 17) {
        PokewalkerIr_Stop();
    }
    if (!ov112_021E7668()) {
        PokewalkerApp_ShowSubBg1(app, TRUE);
        if (PokewalkerApp_GetTouchedButton(5) == 0 || (gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B)) {
            PokewalkerIr_Stop();
            PokewalkerApp_SelectMenuItem(app, 1, 3, 18);
            PlaySE(SEQ_SE_DP_SELECT);
            return 33;
        }
    } else {
        PokewalkerApp_ShowSubBg1(app, FALSE);
    }
    return nextState;
}

// Puts the Pokemon on the Pokewalker back into a PC box (preferably the one it came from); remembers the box,
// or -1 if there was no Pokemon.
void PokewalkerApp_ReturnWalkerMonToBox(PokewalkerApp *app) {
    u16 unk0;
    u16 box2;
    int box = -1;
    int slot = 0;
    PCStorage *storage = SaveArray_PCStorage_Get(app->saveData);
    BoxPokemon *boxMon = &app->returnedMon;
    Pokewalker_GetStrollState(app->pokewalker, &unk0, &box2);
    if (Pokewalker_TryGetBoxMon(app->pokewalker, boxMon)) {
        box = box2;
        PCStorage_FindFirstEmptySlot(storage, &box, &slot);
        PCStorage_PlaceMonInBoxByIndexPair(storage, box, slot, boxMon);
        app->returnedMonBox = box;
    } else {
        app->returnedMonBox = -1;
    }
}

int PokewalkerApp_EraseRecords_PrintCanceled(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00033);
    PokewalkerApp_HideCursor(app);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    return 19;
}

int PokewalkerApp_EraseRecords_WaitCanceled(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->nextMode = 11;
        return 34;
    }
    return 19;
}

int PokewalkerApp_EraseRecords_PrintInitialized(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0, 64, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_1);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00031);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    return 21;
}

int PokewalkerApp_EraseRecords_WaitInitialized(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->nextMode = 0;
        return 22;
    }
    return 21;
}

#pragma force_active on
int PokewalkerApp_EraseRecords_PrintComeBack(PokewalkerApp *app) {
    if (app->returnedMonBox >= 0) {
        BufferBoxMonNickname(app->msgFormat, 1, &app->returnedMon);
        app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00026);
        return 23;
    }
    return 29;
}
#pragma force_active reset

int PokewalkerApp_EraseRecords_WaitComeBack(PokewalkerApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == FALSE) {
        return 24;
    }
    return 23;
}

int PokewalkerApp_EraseRecords_ShowReturnedMon(PokewalkerApp *app) {
    app->timer = 0;
    PokewalkerApp_HideSprite(app, 8);
    PokewalkerApp_LoadMonPicture(&app->returnedMon, app->monSpriteBuffer, &app->monPicTemplate, 0);
    return 25;
}

#pragma force_active on
int PokewalkerApp_EraseRecords_AnimateReturnedMon(PokewalkerApp *app) {
    app->timer++;
    if (app->timer > 180) {
        PokewalkerApp_ShowSprite(app, 8);
        return 26;
    }
    if ((u32)app->timer > MTRandom() % 180) {
        PokewalkerApp_SpawnFallingParticle(app);
    }
    return 25;
}
#pragma force_active reset

int PokewalkerApp_EraseRecords_AfterAnimation(PokewalkerApp *app) {
    return 27;
}

int PokewalkerApp_EraseRecords_PrintArrived(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00027);
    return 28;
}

int PokewalkerApp_EraseRecords_WaitArrived(PokewalkerApp *app) {
    if (TextPrinterCheckActive(app->textPrinterId) == FALSE) {
        return 29;
    }
    return 28;
}

int PokewalkerApp_EraseRecords_PrintSaving(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00013);
    return 30;
}

int PokewalkerApp_EraseRecords_WaitSaving(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        PokewalkerApp_CreateWaitingIcon(app);
        return 31;
    }
    return 30;
}

int PokewalkerApp_EraseRecords_Save(PokewalkerApp *app) {
    SaveGameNormal(app->saveData);
    Save_ClearStatusFlags(app->saveData);
    return 32;
}

int PokewalkerApp_EraseRecords_EndSave(PokewalkerApp *app) {
    PokewalkerApp_DeleteWaitingIcon(app);
    app->nextMode = 0;
    return 34;
}

int PokewalkerApp_EraseRecords_WaitCursor(PokewalkerApp *app) {
    if (!ManagedSprite_IsAnimated(app->sprites[0])) {
        app->state = app->nextState;
        return app->state;
    }
    return 33;
}

int PokewalkerApp_EraseRecords_End(PokewalkerApp *app) {
    PokewalkerApp_HideCursor(app);
    return 35;
}

void PokewalkerApp_SelectMenuItem(PokewalkerApp *app, int selection, int cursorPosition, int nextState) {
    app->menuSelection = selection;
    PokewalkerApp_SetCursorPosition(app, cursorPosition);
    PokewalkerApp_PressCursor(app);
    app->nextState = nextState;
}

void PokewalkerApp_StartMessage(PokewalkerApp *app, int window, int msgId, int nextState) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, window, msgId);
    app->nextState = nextState;
}
