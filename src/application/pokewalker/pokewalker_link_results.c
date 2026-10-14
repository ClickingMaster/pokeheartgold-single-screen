// Pokewalker connection app, mode 3 = link with the Pokewalker (second part): following the receive / gift / Stroll
// requests, storing what was read (step counts, other games' Trainer House sets, event log), the error messages, the
// results ("went to the Pokewalker", "has arrived", "received gifts"), sending the Pokemon back, and the shared
// message / cursor wait states. The state functions are in the table sLinkStates (asm data).

#include "global.h"

#include "constants/maps.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/pokewalker/pokewalker_app.h"
#include "msgdata/msg/msg_0274.h"

#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "save_local_field_data.h"
#include "sound_playback.h"
#include "system.h"
#include "text.h"
#include "screen_fade_legacy.h"

// not declared in the SDK headers of this repo
void RTC_GetDateTime(RTCDate *date, RTCTime *time);

#define LINK_MOTION(app) ((PokewalkerAppMotion *)(app)->unk_1F2C0)

#pragma force_active on

// Adds the Trainer House sets of other HeartGold / SoulSilver games met by the Pokewalker (not while the player is in
// the Trainer House).
void PokewalkerApp_AddPeerTrainerHouseSets(PokewalkerApp *app) {
    int i;
    if (LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(app->saveData))->mapId == MAP_VIRIDIAN_TRAINER_HOUSE_B1F) {
        return;
    }
    for (i = 9; i >= 0; i--) {
        if (app->peerRecords[i + 1].version == VERSION_HEARTGOLD || app->peerRecords[i + 1].version == VERSION_SOULSILVER) {
            PokewalkerApp_SaveTrainerHouseData(app->saveData, &app->peerRecords[i + 1].trainerHouseData);
        }
    }
}

void PokewalkerApp_UpdateSavedSteps(PokewalkerApp *app) {
    u32 steps;
    Pokewalker_GetStepsAndWatts(app->pokewalker, &app->walkerSteps, &app->walkerWatts);
    steps = app->peerIdentity.steps;
    if (steps < app->walkerSteps) {
        Pokewalker_SetStepsAddWatts(app->pokewalker, app->walkerSteps, app->irStatus.watts);
    } else {
        Pokewalker_SetStepsAddWatts(app->pokewalker, steps, app->irStatus.watts);
    }
}

#pragma force_active reset

void PokewalkerApp_AddSavedTotal(PokewalkerApp *app) {
    Pokewalker_GetStepsAndWatts(app->pokewalker, &app->walkerSteps, &app->walkerWatts);
    Pokewalker_SetStepsAddWatts(app->pokewalker, app->walkerSteps, app->irStatus.watts);
}

BOOL PokewalkerApp_IsWalkerMonValid(PokewalkerApp *app) {
    u16 species;
    if (app->peerIdentity.hasWalkerMon == 1) {
        species = app->walkerMon.species;
        if (species == SPECIES_NONE || species > NATIONAL_DEX_COUNT) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL PokewalkerApp_IsGiftMonValid(PokewalkerApp *app) {
    u16 species;
    if (app->irBlocks.flags.hasGiftMon == 1) {
        species = app->irBlocks.giftMon.mon.species;
        if (species == SPECIES_NONE || species > NATIONAL_DEX_COUNT) {
            return FALSE;
        }
    }
    return TRUE;
}

int PokewalkerApp_Link_UpdateReceive(PokewalkerApp *app) {
    int result;

    PokewalkerIr_GetChunkProgress();
    result = PokewalkerIr_GetTotalProgress();
    if (result != 0) {
        PokewalkerApp_SpawnFallingParticle(app);
    }
    PokewalkerApp_UpdateConnectionAnim(app);
    PokewalkerApp_PlayTransferSE(result);

    result = PokewalkerIr_GetAndClearError();
    switch (result) {
    case 2:
        if (app->linkConnected == 1) {
            app->linkError = 42;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
        app->linkError = result;
        break;
    case 15: // done
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
        if (PokewalkerApp_IsWalkerMonValid(app) && app->peerIdentity.hasWalkerMon == 1) {
            GetSpeciesNameIntoArray(app->walkerMon.species, HEAP_ID_154, app->walkerMonNickname);
        }
        PokewalkerApp_InitMotion(app, LINK_MOTION(app), 1);
        PokewalkerApp_AddPeerTrainerHouseSets(app);
        PokewalkerApp_UpdateSavedSteps(app);
        PokewalkerApp_AddStrollToEventLog(app);
        app->strollSteps = app->peerIdentity.steps - app->walkerSteps;
        return 37;
    }

    if (app->linkError != 0) {
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
        app->nextState = 60;
        PokewalkerIr_Stop();
        return 32;
    }
    if (!PokewalkerIrStatus_IsConnected()) {
        PokewalkerApp_ShowSubBg1(app, TRUE);
        if (PokewalkerApp_GetTouchedButton(5) == 0 || (gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B)) {
            app->linkError = 33;
            app->nextState = 66;
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
    return 27;
}

#pragma force_active on

// Adds the Stroll to the event log (the last record) and, if the Pokewalker brought a gift Pokemon, that too.
void PokewalkerApp_AddStrollToEventLog(PokewalkerApp *app) {
    PokewalkerEventLogParams params = { 0 };
    PokewalkerEventLogMon giftMon;
    params.nickname = app->walkerMonNickname;
    params.mon.species = app->walkerMon.species;
    params.mon.form = app->walkerMon.form;
    params.mon.gender = app->walkerMon.gender;
    params.mon.isShiny = app->walkerMon.isShiny;
    u32 steps = app->peerIdentity.steps;
    u32 savedSteps = app->walkerSteps;
    if (steps < savedSteps) {
        params.steps = 0;
    } else {
        params.steps = steps - savedSteps;
    }
    PokewalkerEventLog_InitStrollEndRecord(&app->irStatus.records[23], &params);
    if (PokewalkerEventLog_HasGiftMon(app->irStatus.records)) {
        giftMon.species = app->irBlocks.giftMon.mon.species;
        giftMon.form = app->irBlocks.giftMon.mon.form;
        giftMon.gender = app->irBlocks.giftMon.mon.gender;
        giftMon.isShiny = app->irBlocks.giftMon.mon.isShiny;
        PokewalkerEventLog_SetGiftMon(app->irStatus.records, &giftMon);
    }
}

#pragma force_active reset

// Request 0x60: reads the Pokewalker's status, blocks and the other games' records.
int PokewalkerApp_Link_StartReceiveGifts(PokewalkerApp *app) {
    MIi_CpuClearFast(0, (u32 *)&app->irBlocks, sizeof(PokewalkerIrBlocks));
    PokewalkerIr_RequestReceiveGifts(&app->irStatus, app->peerRecords);
    PokewalkerApp_StartLink(app);
    PokewalkerApp_ResetLinkError(app);
    return 29;
}

#pragma force_active on
int PokewalkerApp_Link_UpdateReceiveGifts(PokewalkerApp *app) {
    int progress, error;
    RTCDate date;
    RTCTime time;

    PokewalkerIr_GetChunkProgress();
    progress = PokewalkerIr_GetTotalProgress();
    if (progress) {
        PokewalkerApp_SpawnFallingParticle(app);
    }
    PokewalkerApp_PlayTransferSE(progress);
    PokewalkerApp_UpdateConnectionAnim(app);
    error = PokewalkerIr_GetAndClearError();
    switch (error) {
    case 2:
        if (app->linkConnected == 1) {
            app->linkError = 42;
        }
        break;
    case 4:
    case 5:
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
        RTC_GetDateTime(&date, &time);
        RTC_ConvertDateTimeToSecond(&date, &time);
        PokewalkerApp_AddPeerTrainerHouseSets(app);
        PokewalkerApp_AddSavedTotal(app);
        PokewalkerApp_ShowSprite(app, 6);
        return 41;
    }
    if (app->linkError) {
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
        app->nextState = 61;
        PokewalkerIr_Stop();
        return 32;
    }
    if (!PokewalkerIrStatus_IsConnected()) {
        PokewalkerApp_ShowSubBg1(app, TRUE);
        if (!PokewalkerApp_GetTouchedButton(5) || (gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B)) {
            PokewalkerIr_Stop();
            app->linkError = 33;
            app->nextState = 66;
            PokewalkerApp_SelectMenuItem(app, 3, 3, 33);
            PlaySE(SEQ_SE_DP_DECIDE);
            return 65;
        }
    } else {
        if (!app->linkConnected) {
            PokewalkerApp_ShowMessageBox(app, 2, msg_0274_00006);
            app->linkConnected = 1;
        }
        PokewalkerApp_ShowSubBg1(app, FALSE);
    }
    return 29;
}

// Request 0x52: sends the Stroll data again (the Pokewalker is already registered).
int PokewalkerApp_Link_StartSendStroll(PokewalkerApp *app) {
    PokewalkerApp_BuildSendStrollData(app);
    PokewalkerIr_RequestSendStroll((u8 *)&app->strollData, (u8 *)&app->unk_F1D0, (u8 *)&app->irStatus, (u8 *)&app->irBlocks);
    PokewalkerApp_StartLink(app);
    if (app->strollPartyMon) {
        PokewalkerApp_LoadMonPicture(app->strollPartyMon, app->monSpriteBuffer, &app->monPicTemplate, 0);
    } else {
        PokewalkerApp_LoadMonPicture(app->strollMon, app->monSpriteBuffer, &app->monPicTemplate, 0);
    }
    ManagedSprite_SetOamMode(app->sprites[8], GX_OAM_MODE_XLU);
    PokewalkerApp_ShowSprite(app, 8);
    ManagedSprite_SetAnim(app->sprites[8], 25);
    ManagedSprite_SetAnimationFrame(app->sprites[8], 0);
    app->motionStarted = 0;
    PokewalkerApp_ShowConnectionSprites(app, 2);
    PokewalkerApp_ResetLinkError(app);
    return 31;
}
#pragma force_active reset

// The walker icon (sprite 8) follows the transfer progress.
void PokewalkerApp_SetWalkerIconFrame(PokewalkerApp *app, int progress) {
    if (progress == 0) {
        ManagedSprite_SetAnimationFrame(app->sprites[8], 0);
    } else if (progress > 30) {
        ManagedSprite_SetAnimationFrame(app->sprites[8], (progress - 30) / 4);
    }
}

#pragma force_active on
int PokewalkerApp_Link_UpdateSendStroll(PokewalkerApp *app) {
    u32 progress, error;
    int total;
    PokewalkerIr_GetChunkProgress();
    total = PokewalkerIr_GetTotalProgress();
    progress = PokewalkerApp_UpdateLinkProgress(app, (u8)total);
    if (progress) {
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
    case 5:
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
        Pokewalker_GetStepsAndWatts(app->pokewalker, &app->walkerSteps, &app->walkerWatts);
        PokewalkerApp_UpdateSavedSteps(app);
        Pokewalker_IncrementLinkCount(app->pokewalker);
        PokewalkerApp_HideSprite(app, 8);
        PokewalkerApp_InitMotion(app, LINK_MOTION(app), 0);
        return 35;
    }
    if (app->linkError) {
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
        app->nextState = 59;
        PokewalkerIr_Stop();
        return 32;
    }
    if (!PokewalkerIrStatus_IsConnected()) {
        PokewalkerApp_ShowSubBg1(app, TRUE);
        if (!PokewalkerApp_GetTouchedButton(5) || (gSystem.newKeys & PAD_BUTTON_A) || (gSystem.newKeys & PAD_BUTTON_B)) {
            app->linkError = 33;
            app->nextState = 66;
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
    return 31;
}

// Message shown for a link error (PokewalkerIr_GetAndClearError, 33 = canceled, 42 = connection lost).
int PokewalkerApp_GetLinkErrorMessage(int error) {
    switch (error) {
    case 4:
        return msg_0274_00034;
    case 5:
        return msg_0274_00036;
    case 7:
        return msg_0274_00037;
    case 6:
        return msg_0274_00043;
    case 8:
    case 9:
        return msg_0274_00038;
    case 3:
    case 11:
    case 12:
        return msg_0274_00039;
    case 13:
        return msg_0274_00035;
    case 14:
        return msg_0274_00042;
    case 33:
    case 42:
        break;
    }
    return error;
}
#pragma force_active reset

int PokewalkerApp_Link_PrintError(PokewalkerApp *app) {
    PokewalkerApp_HideCursor(app);
    app->linkError = PokewalkerApp_GetLinkErrorMessage(app->linkError);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, app->linkError);
    return 34;
}

int PokewalkerApp_Link_PrintCanceled(PokewalkerApp *app) {
    PokewalkerApp_HideCursor(app);
    app->linkError = PokewalkerApp_GetLinkErrorMessage(app->linkError);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, app->linkError);
    app->nextState = 66;
    return 34;
}

int PokewalkerApp_Link_RetryInstructions(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00007);
    return 17;
}

int PokewalkerApp_Link_RetryPointAtCardForStroll(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00005);
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    return 21;
}

int PokewalkerApp_Link_RetryPointAtCard(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00005);
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    return 2;
}

int PokewalkerApp_Link_RetryPointAtCardForGifts(PokewalkerApp *app) {
    BgClearTilemapBufferAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00005);
    PokewalkerApp_LoadTilemap(app, 2, 14);
    PokewalkerApp_PrintExit(app, 1);
    return 23;
}

int PokewalkerApp_Link_WaitError(PokewalkerApp *app) {
    int next;
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        app->nextMode = 0;
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
        BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
        next = app->nextState;
        app->state = next;
        return next;
    }
    return 34;
}

int PokewalkerApp_Link_PrintWentToWalker(PokewalkerApp *app) {
    BoxPokemon *boxMon;
    PokewalkerIr_Stop();
    if (!PokewalkerApp_IsGiftMonValid(app)) {
        MIi_CpuClearFast(0, (u32 *)&app->irBlocks.giftMon, sizeof(PokewalkerIrGiftMon));
        app->irBlocks.flags.hasGiftMon = FALSE;
    }
    boxMon = app->strollPartyMon;
    if (boxMon) {
        BufferBoxMonNickname(app->msgFormat, 1, boxMon);
    } else {
        BufferBoxMonNickname(app->msgFormat, 1, app->strollMon);
    }
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00024);
    PlayFanfare(SEQ_ME_KEYITEM);
    return 36;
}

int PokewalkerApp_Link_WaitWentToWalker(PokewalkerApp *app) {
    PokewalkerApp_UpdateMotion(app, LINK_MOTION(app));
    if (!TextPrinterCheckActive(app->textPrinterId) && !PokewalkerApp_AgeParticles(app) && !IsFanfarePlaying()) {
        app->nextMode = 5;
        return 66;
    }
    return 36;
}

int PokewalkerApp_Link_PrintReturned(PokewalkerApp *app) {
    String *nickname = String_New(12, HEAP_ID_154);
    int printerId;
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    if (PokewalkerApp_IsWalkerMonValid(app)) {
        CopyU16ArrayToStringN(nickname, app->walkerMonNickname, 12);
        BufferString(app->msgFormat, 1, nickname, app->walkerMon.gender, 1, 1);
        printerId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00027);
    } else {
        printerId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00032);
    }
    app->textPrinterId = printerId;
    PokewalkerIr_Stop();
    String_Delete(nickname);
    return 38;
}

// Whether the Pokemon stored in the save data for the Stroll is the one the Pokewalker has.
BOOL PokewalkerApp_IsWalkerMonInSave(PokewalkerApp *app) {
    BOOL result = TRUE;
    Pokemon *mon;
    u32 species;
    u32 form;
    mon = AllocMonZeroed(HEAP_ID_154);
    if (Pokewalker_TryGetBoxMon(app->pokewalker, (BoxPokemon *)mon) == FALSE) {
        result = FALSE;
    }
    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    form = GetMonData(mon, MON_DATA_FORM, NULL);
    if (app->walkerMon.species != species || app->walkerMon.form != form) {
        result = FALSE;
    }
    Heap_Free(mon);
    return result;
}

int PokewalkerApp_Link_WaitReturned(PokewalkerApp *app) {
    PokewalkerApp_UpdateMotion(app, LINK_MOTION(app));
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        if (!IsFanfarePlaying()) {
            if (!PokewalkerApp_IsWalkerMonValid(app)) {
                MIi_CpuClearFast(0, (u32 *)&app->walkerMon, sizeof(PokewalkerMon));
                app->peerIdentity.hasWalkerMon = 0;
            }
            if (!PokewalkerApp_IsGiftMonValid(app)) {
                MIi_CpuClearFast(0, (u32 *)&app->irBlocks.giftMon, sizeof(PokewalkerIrGiftMon));
                app->irBlocks.flags.hasGiftMon = FALSE;
            }
            if (PokewalkerApp_IsWalkerMonInSave(app) || app->irBlocks.flags.hasGiftMon || app->irBlocks.flags.hasGiftItem || app->peerIdentity.hasWalkerMon) {
                app->nextMode = 6;
                return 66;
            }
            return 39;
        }
    }
    return 38;
}

int PokewalkerApp_Link_PrintReleased(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00020);
    return 40;
}

int PokewalkerApp_Link_WaitReleased(PokewalkerApp *app) {
    return !TextPrinterCheckActive(app->textPrinterId) ? 62 : 40;
}

int PokewalkerApp_Link_PrintReceivedGifts(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00029);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    PokewalkerIr_Stop();
    PlayFanfare(SEQ_ME_ITEM);
    return 42;
}

int PokewalkerApp_Link_WaitReceivedGifts(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId) && !IsFanfarePlaying()) {
        app->nextMode = 6;
        return 66;
    }
    return 42;
}

int PokewalkerApp_Link_PrintSendBackCaution(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00018);
    return 44;
}

int PokewalkerApp_Link_WaitSendBackCaution(PokewalkerApp *app) {
    return !TextPrinterCheckActive(app->textPrinterId) ? 45 : 44;
}

int PokewalkerApp_Link_ShowSendBackYesNo(PokewalkerApp *app) {
    PokewalkerApp_ShowYesNoMenu(app, 1);
    return 46;
}

#pragma force_active on
int PokewalkerApp_Link_HandleSendBackYesNo(PokewalkerApp *app) {
    switch (PokewalkerApp_HandleTwoChoiceMenu(app)) {
    case 0:
        PokewalkerApp_HideCursor(app);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
        return 47;
    case 1:
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 16, 32, 8, TILEMAP_FILL_KEEP_PAL);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        PokewalkerApp_HideCursor(app);
        app->nextMode = 0;
        return 66;
    default:
        return 46;
    }
}
#pragma force_active reset

int PokewalkerApp_Link_PrintWillSendBack(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 0, msg_0274_00019);
    return 48;
}

int PokewalkerApp_Link_WaitWillSendBack(PokewalkerApp *app) {
    return !TextPrinterCheckActive(app->textPrinterId) ? 49 : 48;
}

int PokewalkerApp_Link_ShowSendBackMenu(PokewalkerApp *app) {
    PokewalkerApp_LoadTilemap(app, 2, 10);
    PokewalkerApp_PrintString(app, 5, app->sendHomeText, 4, MAKE_TEXT_COLOR(15, 1, 0));
    PokewalkerApp_PrintExit(app, 1);
    app->menuSelection = 0;
    PokewalkerApp_SetCursorPosition(app, 2);
    return 50;
}

int PokewalkerApp_Link_HandleSendBackMenu(PokewalkerApp *app) {
    int button = PokewalkerApp_GetTouchedButton(1);
    if (button != -1) {
        PokewalkerApp_SelectMenuItem(app, button, button + 2, 51);
        PlaySE(SEQ_SE_DP_DECIDE);
        return 65;
    }
    if (gSystem.newKeys & PAD_KEY_UP) {
        PokewalkerApp_SetCursorPosition(app, 2);
        if (app->menuSelection != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        app->menuSelection = 0;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        PokewalkerApp_SetCursorPosition(app, 3);
        if (app->menuSelection != 1) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        app->menuSelection = 1;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        PokewalkerApp_SelectMenuItem(app, app->menuSelection, app->menuSelection + 2, 51);
        PlaySE(SEQ_SE_DP_DECIDE);
        return 65;
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        PokewalkerApp_SelectMenuItem(app, 1, 3, 51);
        PlaySE(SEQ_SE_DP_SELECT);
        return 65;
    }
    return 50;
}

int PokewalkerApp_Link_EndSendBackMenu(PokewalkerApp *app) {
    PokewalkerApp_HideCursor(app);
    if (app->menuSelection == 0) {
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
        FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
        app->strollSteps = 0;
        return 52;
    }
    app->nextMode = 11;
    return 66;
}

int PokewalkerApp_Link_PrintComeBack(PokewalkerApp *app) {
    BoxPokemon *boxMon = &app->returnedMon;
    Pokewalker_TryGetBoxMon(app->pokewalker, boxMon);
    PokewalkerApp_ShowConnectionSprites(app, 0);
    BufferBoxMonNickname(app->msgFormat, 1, boxMon);
    PokewalkerApp_LoadMonPicture(boxMon, app->monSpriteBuffer, &app->monPicTemplate, 0);
    PokewalkerApp_StartMessage(app, 2, msg_0274_00026, 53);
    return 64;
}

int PokewalkerApp_Link_StartReturnAnimation(PokewalkerApp *app) {
    app->timer = 0;
    PokewalkerApp_HideSprite(app, 8);
    return 54;
}

int PokewalkerApp_Link_AnimateReturn(PokewalkerApp *app) {
    app->timer++;
    if (app->timer > 180) {
        return 55;
    }
    if ((u32)app->timer > MTRandom() % 180) {
        PokewalkerApp_SpawnFallingParticle(app);
    }
    PokewalkerApp_PlayTransferSE(app->timer);
    return 54;
}

int PokewalkerApp_Link_AfterReturnAnimation(PokewalkerApp *app) {
    return 56;
}

int PokewalkerApp_Link_PrintArrived(PokewalkerApp *app) {
    PokewalkerApp_StartMessage(app, 2, msg_0274_00027, 57);
    return 64;
}

int PokewalkerApp_Link_EndSendBack(PokewalkerApp *app) {
    app->nextMode = 6;
    return 66;
}

int PokewalkerApp_Link_FadeOut(PokewalkerApp *app) {
    BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_154);
    return 63;
}

int PokewalkerApp_Link_WaitFadeOut(PokewalkerApp *app) {
    if (IsPaletteFadeFinished() == TRUE) {
        PokewalkerApp_HideMotionSprites(app);
        app->nextMode = 0;
        return 66;
    }
    return 63;
}

int PokewalkerApp_Link_WaitMessage(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        return app->state = app->nextState;
    }
    return 64;
}

int PokewalkerApp_Link_WaitCursor(PokewalkerApp *app) {
    if (!ManagedSprite_IsAnimated(app->sprites[0])) {
        PokewalkerApp_HideCursor(app);
        return app->state = app->nextState;
    }
    return 65;
}

int PokewalkerApp_Link_End(PokewalkerApp *app) {
    PokewalkerApp_HideConnectionSprites(app);
    PokewalkerApp_HideSprite(app, 3);
    PokewalkerApp_HideSprite(app, 4);
    PokewalkerApp_HideSprite(app, 8);
    FillBgTilemapRect(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 24, TILEMAP_FILL_KEEP_PAL);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_1);
    return 67;
}
