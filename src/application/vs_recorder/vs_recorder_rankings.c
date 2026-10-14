#include <string.h>

#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/vs_recorder/vs_recorder_internal.h"
#include "msgdata/msg/msg_0013.h"

#include "assert.h"
#include "bg_window.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "global_terminal_net.h"
#include "heap.h"
#include "igt.h"
#include "message_format.h"
#include "msgdata.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "sound_playback.h"
#include "text.h"
#include "touch_hitbox_controller.h"

// VS Recorder rankings screens: this week's rankings (VS_RECORDER_SCREEN_RANKINGS) and last week's results
// (VS_RECORDER_SCREEN_RANKINGS_LAST) of the Global Terminal. Starts with the box data screens' server response handlers.

// window position and size in tiles
typedef struct VsRecorderRankingsWindowRect {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
} VsRecorderRankingsWindowRect;

typedef struct VsRecorderRankingTeamCounts {
    int counts[3];
} VsRecorderRankingTeamCounts;

// launcher (vs_recorder_app_2.c): flypoint flag 27 is set
BOOL VsRecorderApp_HasVisitedBattleFrontier(VsRecorderWork *work);

extern const VsRecorderRankingTeamCounts sVsRecorderRankingTeamCounts;    // teams per kind: 16 trainer types, 12 months, 20 species
extern const TouchscreenHitbox sVsRecorderRankingsHitboxes[];            // SWITCH RANKINGS, SWITCH TEAMS, exit
extern const VsRecorderRankingsWindowRect sVsRecorderRankingsButtonLabelWindows[]; // VsRecorderRankings.buttonLabels
extern const VsRecorderRankingsWindowRect sVsRecorderRankingsThisWeekWindows[]; // VsRecorderRankings.windows, this week
extern const VsRecorderRankingsWindowRect sVsRecorderRankingsLastWeekWindows[]; // VsRecorderRankings.windows, last week
extern const VsRecorderListConfig sVsRecorderRankingsThisWeekListConfig;           // this week's ranking list
extern const VsRecorderListConfig sVsRecorderRankingsLastWeekListConfig;           // last week's results list
extern u32 sVsRecorderRankedRecordGameStats[];                                // game stat of each record (0xFFFF: none)

// ranking list entry kinds (VsRecorderListEntry.msgId)
#define RANKING_ENTRY_TRAINER_TYPE       10000
#define RANKING_ENTRY_TRAINER_TYPE_SCORE 20000
#define RANKING_ENTRY_BIRTH_MONTH        30000
#define RANKING_ENTRY_BIRTH_MONTH_SCORE  40000
#define RANKING_ENTRY_SPECIES            50000
#define RANKING_ENTRY_SPECIES_SCORE      60000

// GetSpeciesNameIntoArray called through an int prototype, as compiled (no u16 truncation of the species)
#define GetSpeciesNameIntoArrayInt ((void (*)(int, enum HeapID, u16 *))GetSpeciesNameIntoArray)

#define RANKING_SCORE_MAX 9999999999999999999ULL

// ---- box data screens: server responses

void VsRecorder_OnBoxUploaded(void *arg, GlobalTerminalNetError *error) {
}

void VsRecorder_OnBoxListReceived(void *arg, GlobalTerminalNetError *error) {
    VsRecorderWork *work = arg;
    VsRecorderBoxData *boxData = work->screenWork;

    if (error->isError == TRUE) {
        boxData->downloadCount = 0;
        return;
    }
    boxData->downloadCount = GlobalTerminalNet_GetBoxList(VsRecorder_GetNet(work), (void **)boxData->downloads, 20);
    MI_CpuCopy8(boxData->downloads, boxData->unk_3DC, sizeof(boxData->unk_3DC));
}

// ---- rankings screens

void VsRecorder_InitRankingsList(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    if (rankings->lastWeek == FALSE) {
        rankings->listConfig = sVsRecorderRankingsThisWeekListConfig;
        rankings->valueTemplate = NULL;
    } else {
        rankings->listConfig = sVsRecorderRankingsLastWeekListConfig;
        rankings->valueTemplate = NewString_ReadMsgData(work->recordMsgData, rankings->results[rankings->ranking].recordId + 94);
    }
    rankings->listConfig.count = rankings->teamCount;
    rankings->listConfig.entries = rankings->entries[rankings->ranking];
    VsRecorder_SetVBlankPaused(work, TRUE);
    VsRecorder_ClearListWindow(&work->listWindow);
    VsRecorder_InitRankingList(&work->listWindow, work, rankings->msgData, &rankings->listConfig, rankings->ranking, rankings->team, rankings->valueTemplate);
    VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
    VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DMAIN);
    VsRecorder_SetListArrowsY(&work->listWindow, 112, 184);
    VsRecorder_SetVBlankPaused(work, FALSE);
}

#pragma force_active on

void VsRecorder_FillRankingEntries(VsRecorderWork *work);

// list entries of the shown ranking and team kind: this week the teams in order, last week with their scores
void VsRecorder_FillRankingEntries(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;
    int i;

    rankings->results = rankings->resultsBuf;
    rankings->standings = rankings->standingsBuf;
    switch (rankings->team) {
    case VS_RECORDER_RANKING_TEAM_TRAINER_TYPE:
        for (i = 0; i < 16; i++) {
            if (rankings->lastWeek == FALSE) {
                rankings->entries[rankings->ranking][i].msgId = RANKING_ENTRY_TRAINER_TYPE;
                rankings->entries[rankings->ranking][i].value = VsRecorder_GetAvatarFileId(rankings->standings[rankings->ranking].trainerTypes[i], 4);
                rankings->entries[rankings->ranking][i].time = (s64)i;
            } else {
                rankings->entries[rankings->ranking][i].msgId = RANKING_ENTRY_TRAINER_TYPE_SCORE;
                rankings->entries[rankings->ranking][i].value = VsRecorder_GetAvatarFileId(rankings->results[rankings->ranking].trainerTypes[i], 4);
                rankings->entries[rankings->ranking][i].time = rankings->results[rankings->ranking].trainerTypeScores[i];
                if (rankings->entries[rankings->ranking][i].time >= RANKING_SCORE_MAX) {
                    rankings->entries[rankings->ranking][i].time = RANKING_SCORE_MAX;
                }
            }
        }
        break;
    case VS_RECORDER_RANKING_TEAM_BIRTH_MONTH:
        for (i = 0; i < 12; i++) {
            if (rankings->lastWeek == FALSE) {
                rankings->entries[rankings->ranking][i].msgId = RANKING_ENTRY_BIRTH_MONTH;
                rankings->entries[rankings->ranking][i].value = rankings->standings[rankings->ranking].birthMonths[i];
                rankings->entries[rankings->ranking][i].time = (s64)i;
            } else {
                rankings->entries[rankings->ranking][i].msgId = RANKING_ENTRY_BIRTH_MONTH_SCORE;
                rankings->entries[rankings->ranking][i].value = rankings->results[rankings->ranking].birthMonths[i];
                rankings->entries[rankings->ranking][i].time = rankings->results[rankings->ranking].birthMonthScores[i];
                if (rankings->entries[rankings->ranking][i].time >= RANKING_SCORE_MAX) {
                    rankings->entries[rankings->ranking][i].time = RANKING_SCORE_MAX;
                }
            }
        }
        break;
    case VS_RECORDER_RANKING_TEAM_SPECIES:
        for (i = 0; i < 20; i++) {
            if (rankings->lastWeek == FALSE) {
                rankings->entries[rankings->ranking][i].msgId = RANKING_ENTRY_SPECIES;
                rankings->entries[rankings->ranking][i].value = rankings->standings[rankings->ranking].species[i];
                rankings->entries[rankings->ranking][i].time = (s64)i;
            } else {
                rankings->entries[rankings->ranking][i].msgId = RANKING_ENTRY_SPECIES_SCORE;
                rankings->entries[rankings->ranking][i].value = rankings->results[rankings->ranking].species[i];
                rankings->entries[rankings->ranking][i].time = rankings->results[rankings->ranking].speciesScores[i];
                if (rankings->entries[rankings->ranking][i].time >= RANKING_SCORE_MAX) {
                    rankings->entries[rankings->ranking][i].time = RANKING_SCORE_MAX;
                }
            }
        }
        break;
    }
}

#pragma force_active reset

// SWITCH RANKINGS / SWITCH TEAMS
void VsRecorder_CreateRankingsButtonLabels(VsRecorderWork *work) {
    Window *window;
    const VsRecorderRankingsWindowRect *rect = sVsRecorderRankingsButtonLabelWindows;
    int baseTile = 1;
    int i;

    for (i = 0, window = ((VsRecorderRankings *)work->screenWork)->buttonLabels; i < 2; i++) {
        String *str;
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, rect->x, rect->y, rect->width, rect->height, 14, baseTile);
        baseTile += rect->width * rect->height;
        FillWindowPixelBuffer(window, 0);
        str = NewString_ReadMsgData(work->msgData, i + 80); // SWITCH RANKINGS, SWITCH TEAMS
        AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
        window++;
        rect++;
    }
}

void VsRecorder_RemoveRankingsButtonLabels(VsRecorderWork *work) {
    int i;
    Window *window = ((VsRecorderRankings *)work->screenWork)->buttonLabels;

    for (i = 0; i < 2; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
}

void VsRecorder_CreateRankingsWindows(VsRecorderWork *work) {
    Window *window;
    const VsRecorderRankingsWindowRect *rect = sVsRecorderRankingsThisWeekWindows;
    const VsRecorderRankingsWindowRect *lastWeekRect = sVsRecorderRankingsLastWeekWindows;
    int baseTile = 1;
    VsRecorderRankings *rankings = work->screenWork;
    int i;
    String *str;

    for (i = 0, window = rankings->windows; i < 2; i++) {
        InitWindow(window);
        if (rankings->lastWeek == FALSE) {
            AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, rect->x, rect->y, rect->width, rect->height, 14, baseTile);
            baseTile += rect->width * rect->height;
        } else {
            AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, lastWeekRect->x, lastWeekRect->y, lastWeekRect->width, lastWeekRect->height, 14, baseTile);
            baseTile += lastWeekRect->width * lastWeekRect->height;
        }
        FillWindowPixelBuffer(window, 0);
        str = NewString_ReadMsgData(work->msgData, msg_0013_00067);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
        window++;
        rect++;
        lastWeekRect++;
    }
}

void VsRecorder_RemoveRankingsWindows(VsRecorderWork *work) {
    int i;
    Window *window = ((VsRecorderRankings *)work->screenWork)->windows;

    for (i = 0; i < 2; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
}

// the player's own value of a ranked record (recordId - 1)
u32 VsRecorder_GetRankedRecordValue(VsRecorderWork *work, int record) {
    if (record >= 87) {
        record = 0;
        GF_AssertFail();
    }
    if (sVsRecorderRankedRecordGameStats[record] == 0xFFFF) {
        GF_AssertFail();
    }
    return GameStats_GetCapped(Save_GameStats_Get(work->saveData), sVsRecorderRankedRecordGameStats[record]);
}

// the ranking's record (or LAST WEEK'S RESULTS!), the team kind and the player's own team
void VsRecorder_PrintRankingsInfo(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;
    MessageFormat *msgFormat;
    String *template;
    u16 speciesName[0x100];
    int isEgg;
    int month;
    int avatar;
    String *team;
    String *name;
    String *str;
    int species;

    str = String_New(255, HEAP_ID_109);
    msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
    FillWindowPixelBuffer(&rankings->windows[0], 0);
    if (rankings->lastWeek == FALSE) {
        template = NewString_ReadMsgData(work->recordMsgData, rankings->standings[rankings->ranking].recordId - 1);
    } else {
        template = NewString_ReadMsgData(work->recordMsgData, rankings->results[rankings->ranking].recordId - 1);
    }
    AddTextPrinterParameterizedWithColor(&rankings->windows[0], 0, template, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(&rankings->windows[0]);
    String_Delete(template);
    FillWindowPixelBuffer(&rankings->windows[1], 0);
    if (rankings->lastWeek == FALSE) {
        template = NewString_ReadMsgData(work->msgData, rankings->team + 82); // Team: Trainer type / Birth month / Favorite Pokemon
    } else {
        template = NewString_ReadMsgData(work->msgData, msg_0013_00091); // LAST WEEK'S RESULTS!
    }
    AddTextPrinterParameterizedWithColor(&rankings->windows[1], 0, template, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(&rankings->windows[1]);
    String_Delete(template);
    name = FrontierCard_NewNameString((const u16 *)work->videoOwners[0], HEAP_ID_109);
    VsRecorder_SanitizeName(work, name);
    switch (rankings->team) {
    case VS_RECORDER_RANKING_TEAM_TRAINER_TYPE:
        avatar = FrontierCard_GetAvatar(work->videoOwners[0]);
        template = NewString_ReadMsgData(work->msgData, msg_0013_00085);
        team = NewString_ReadMsgData(rankings->msgData, VsRecorder_GetAvatarFileId(avatar, 4));
        BufferString(msgFormat, 0, team, 0, 1, 2);
        break;
    case VS_RECORDER_RANKING_TEAM_BIRTH_MONTH:
        month = FrontierCard_GetBirthdayMonth(work->videoOwners[0]);
        template = NewString_ReadMsgData(work->msgData, msg_0013_00086);
        team = String_New(255, HEAP_ID_109);
        BufferMonthNameAbbr(msgFormat, 0, month);
        break;
    case VS_RECORDER_RANKING_TEAM_SPECIES:
        species = FrontierCard_GetSpecies(work->videoOwners[0]);
        isEgg = FrontierCard_IsEgg(work->videoOwners[0]);
        template = NewString_ReadMsgData(work->msgData, msg_0013_00087);
        if (isEgg != 0) {
            species = SPECIES_EGG;
        }
        if (species != SPECIES_NONE) {
            team = String_New(255, HEAP_ID_109);
            GetSpeciesNameIntoArrayInt(species, HEAP_ID_109, speciesName);
            CopyU16ArrayToString(team, speciesName);
        } else {
            team = NewString_ReadMsgData(work->msgData, msg_0013_00021); // Unregistered
        }
        BufferString(msgFormat, 0, team, 0, 1, 2);
        break;
    }
    BufferString(msgFormat, 1, name, 0, 1, 2);
    StringExpandPlaceholders(msgFormat, str, template);
    AddTextPrinterParameterizedWithColor(&rankings->windows[1], 0, str, 0, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(&rankings->windows[1]);
    String_Delete(template);
    String_Delete(team);
    String_Delete(str);
    String_Delete(name);
    MessageFormat_ResetBuffers(msgFormat);
    MessageFormat_Delete(msgFormat);
}

void VsRecorder_SetRankingsSendButtonHidden(VsRecorderWork *work, BOOL hidden) {
    VsRecorderRankings *rankings = work->screenWork;

    if (hidden == FALSE) {
        ManagedSprite_SetDrawFlag(rankings->sendButton.sprite, TRUE);
        TextOBJ_SetSpritesDrawFlag(rankings->sendButton.text, TRUE);
        ManagedSprite_SetPositionXY(rankings->exitButton.sprite, 128, 232);
    } else {
        ManagedSprite_SetDrawFlag(rankings->sendButton.sprite, FALSE);
        TextOBJ_SetSpritesDrawFlag(rankings->sendButton.text, FALSE);
        ManagedSprite_SetPositionXY(rankings->exitButton.sprite, 80, 232);
    }
    TextOBJ_SetPosition(rankings->sendButton.text, 36, -8);
    TextOBJ_SetPosition(rankings->exitButton.text, 36, -8);
}

void VsRecorder_CreateRankingsButtons(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    VsRecorder_LoadButtonGfx(work, NNS_G2D_VRAM_TYPE_2DSUB);
    rankings->sendButton.sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    rankings->exitButton.sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&rankings->sendButton, (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&rankings->exitButton, (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_SetTextSpriteMsg(&rankings->sendButton, (VsRecorderGfx *)&work->narc, 53); // SEND
    VsRecorder_SetTextSpriteMsg(&rankings->exitButton, (VsRecorderGfx *)&work->narc, 3);  // EXIT
    ManagedSprite_SetPositionXY(rankings->sendButton.sprite, 32, 232);
    ManagedSprite_SetPositionXY(rankings->exitButton.sprite, 128, 232);
    TextOBJ_SetPosition(rankings->sendButton.text, 36, -8);
    TextOBJ_SetPosition(rankings->exitButton.text, 36, -8);
    TextOBJ_SetSpritesDrawFlag(rankings->sendButton.text, FALSE);
    TextOBJ_SetSpritesDrawFlag(rankings->exitButton.text, TRUE);
    VsRecorder_SetRankingsSendButtonHidden(work, TRUE);
}

void VsRecorder_FreeRankingsButtons(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    VsRecorder_FreeTextSprite(&rankings->sendButton);
    VsRecorder_FreeTextSprite(&rankings->exitButton);
    Sprite_DeleteAndFreeResources(rankings->sendButton.sprite);
    Sprite_DeleteAndFreeResources(rankings->exitButton.sprite);
    VsRecorder_FreeButtonGfx(work);
}

void VsRecorder_UpdateRankingTeamCount(VsRecorderRankings *rankings) {
    VsRecorderRankingTeamCounts teamCounts = sVsRecorderRankingTeamCounts;
    rankings->teamCount = teamCounts.counts[rankings->team];
}

void VsRecorder_OnRankingsTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderRankings *rankings = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0: // SWITCH RANKINGS
        VsRecorder_OnTouchConfirm(work);
        rankings->ranking = rankings->ranking + 1;
        rankings->ranking = rankings->ranking % rankings->rankingCount;
        VsRecorder_SetSubState(work, 4);
        break;
    case 1: // SWITCH TEAMS
        VsRecorder_OnTouchConfirm(work);
        rankings->team = rankings->team + 1;
        rankings->team = rankings->team % rankings->teamKindCount;
        VsRecorder_SetSubState(work, 4);
        break;
    default:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 7);
        break;
    }
}

// substate 0
BOOL VsRecorder_RankingsInit(VsRecorderWork *work) {
    VsRecorderRankings *rankings = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderRankings));
    memset(rankings, 0, sizeof(VsRecorderRankings));
    work->screenWork = rankings;
    rankings->lastWeek = work->selection;
    rankings->rankingCount = 3;
    if (!VsRecorderApp_HasVisitedBattleFrontier(work)) {
        rankings->rankingCount = rankings->rankingCount - 1;
    }
    rankings->teamKindCount = 3;
    VsRecorder_UpdateRankingTeamCount(rankings);
    rankings->msgData = work->msgData;
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
    VsRecorder_InitBgBlend(&rankings->eva, &rankings->evb, 0);
    rankings->hitboxController = TouchHitboxController_Create(sVsRecorderRankingsHitboxes, 3, VsRecorder_OnRankingsTouch, work, HEAP_ID_109);
    VsRecorder_SetSubState(work, 1);
    return FALSE;
}

// substate 1: fade out the menu and flip the screens
BOOL VsRecorder_RankingsEnter(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    switch (work->step) {
    case 0:
        if (VsRecorder_StepFade(&rankings->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, rankings->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, rankings->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, rankings->fade, work->blendColor);
        break;
    case 1:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeMenuRows(work);
        VsRecorder_CreateRankingsButtons(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        SetBgPriority(GF_BG_LYR_MAIN_0, 1);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_SUB_0, 1);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_2, 0);
        SetBgPriority(GF_BG_LYR_SUB_3, 2);
        VsRecorder_StartFlipScreens(work, 0);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            VsRecorder_SetSubState(work, 2);
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

BOOL VsRecorder_RankingsDownload(VsRecorderWork *work);

// substate 2: receive the rankings; back to the menu if they cannot be received or are incomplete
BOOL VsRecorder_RankingsDownload(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;
    GlobalTerminalNetError *error;

    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case 0:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        VsRecorder_ShowTopMessage(work, msg_0013_00279); // Receiving ranking data...
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 1:
        if (GlobalTerminalNet_RequestRankingsHeader(VsRecorder_GetNet(work)) == TRUE) {
            work->step++;
        }
        break;
    case 2:
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_HideTopMessage(work);
            VsRecorder_ShowMessageScreen(work, 4, error->code, error->type);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            work->step = 5;
            break;
        }
        rankings->recordIds = rankings->recordIdsBuf;
        rankings->playHours = GetIGTHours(Save_PlayerData_GetIGTAddr(work->saveData));
        rankings->playMinutes = GetIGTMinutes(Save_PlayerData_GetIGTAddr(work->saveData));
        rankings->playSeconds = GetIGTSeconds(Save_PlayerData_GetIGTAddr(work->saveData));
        rankings->scores[0].recordId = rankings->recordIds[0];
        rankings->scores[1].recordId = rankings->recordIds[1];
        rankings->scores[2].recordId = rankings->recordIds[2];
        rankings->scores[0].value = VsRecorder_GetRankedRecordValue(work, rankings->scores[0].recordId - 1);
        rankings->scores[1].value = VsRecorder_GetRankedRecordValue(work, rankings->scores[1].recordId - 1);
        rankings->scores[2].value = VsRecorder_GetRankedRecordValue(work, rankings->scores[2].recordId - 1);
        StopSE(SEQ_SE_PL_BREC80, 0);
        work->step++;
        break;
    case 3:
        if (GlobalTerminalNet_RequestRankings(VsRecorder_GetNet(work), (const GlobalTerminalNetRankingsSource *)work->videoOwners[0], (const GlobalTerminalNetPair *)rankings->scores) == TRUE) {
            work->step++;
        }
        break;
    case 4:
        VsRecorder_HideTopMessage(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, 5, error->code, error->type);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            work->step = 5;
            break;
        }
        {
            int missing = FALSE;
            if (rankings->lastWeek == FALSE) {
                VsRecorderRankingStandings *standings = rankings->standings;
                if (!standings[0].recordId || !standings[1].recordId || !standings[2].recordId) {
                    missing = TRUE;
                }
            } else {
                VsRecorderRankingResults *results = rankings->results;
                if (!results[0].recordId || !results[1].recordId || !results[2].recordId) {
                    missing = TRUE;
                }
            }
            if (missing) {
                VsRecorder_ShowMessageScreen(work, 5, 0, 0);
                work->messageScreen.msgId = 300; // There is no ranking data...
                OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
                work->step = 5;
            } else {
                StopSE(SEQ_SE_PL_BREC80, 0);
                PlaySE(SEQ_SE_PL_BREC12);
                work->step = 0xFF;
            }
        }
        break;
    case 5:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeRankingsButtons(work);
        TouchHitboxController_Destroy(rankings->hitboxController);
        VsRecorder_ResetFade(&rankings->fade);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    case 6:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&rankings->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&rankings->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, 16, work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(rankings);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, rankings->fade, work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, rankings->fade, work->blendColor);
            }
        }
        break;
    default:
        VsRecorder_FillRankingEntries(work);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        VsRecorder_SetSubState(work, 5);
        break;
    }
    return FALSE;
}

#pragma force_active reset

// substate 5: show the windows and the list
BOOL VsRecorder_RankingsShow(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    switch (work->step) {
    case 0:
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 33, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 34, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_CreateRankingsWindows(work);
        VsRecorder_PrintRankingsInfo(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateTouchScrollSpritesWithAnim(&work->touchScroll, work, 2);
        VsRecorder_InitRankingsList(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_CreateRankingsButtonLabels(work);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        {
            int step = work->step;
            step++;
            work->step = step;
        }
        break;
    case 1:
        VsRecorder_StepFade(&rankings->fade, 0);
        if (VsRecorder_StepBlend(&rankings->eva, &rankings->evb, FALSE, 0)) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, rankings->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, rankings->fade, work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, 6);
        break;
    }
    return FALSE;
}

// substate 6: scroll the list, SWITCH RANKINGS / SWITCH TEAMS / exit (VsRecorder_OnRankingsTouch)
BOOL VsRecorder_RankingsMain(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    VsRecorder_UpdateTouchScroll(&work->touchScroll);
    VsRecorder_ListScroll(&work->listWindow, work->touchScroll.direction);
    VsRecorder_RedrawRankingList(&work->listWindow, work, (void *)rankings->ranking, rankings->team, rankings->valueTemplate);
    TouchHitboxController_IsTriggered(rankings->hitboxController);
    return FALSE;
}

// substate 7: remove the list and fade out
BOOL VsRecorder_RankingsCloseList(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        if (rankings->valueTemplate != NULL) {
            String_Delete(rankings->valueTemplate);
        }
        VsRecorder_FreeListArrows(&work->listWindow, work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&rankings->fade, 1);
        if (VsRecorder_StepBlend(&rankings->eva, &rankings->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, rankings->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, rankings->fade, work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, 8);
        break;
    }
    return FALSE;
}

// substate 8: free the screen and go back to the menu
BOOL VsRecorder_RankingsExit(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_RemoveRankingsButtonLabels(work);
        VsRecorder_RemoveRankingsWindows(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeRankingsButtons(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        TouchHitboxController_Destroy(rankings->hitboxController);
        work->step++;
        break;
    case 1:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_ResetFade(&rankings->fade);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&rankings->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&rankings->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, 16, work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(rankings);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, rankings->fade, work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, rankings->fade, work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

// substate 3 (unused)
BOOL VsRecorder_RankingsInvalid(VsRecorderWork *work) {
    GF_AssertFail();
    return FALSE;
}

#pragma force_active on

BOOL VsRecorder_RankingsSwitch(VsRecorderWork *work);

// substate 4: SWITCH RANKINGS / SWITCH TEAMS: rebuild the list and the windows
BOOL VsRecorder_RankingsSwitch(VsRecorderWork *work) {
    VsRecorderRankings *rankings = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_FillRankingEntries(work);
        VsRecorder_UpdateRankingTeamCount(rankings);
        VsRecorder_RemoveRankingsWindows(work);
        VsRecorder_RemoveListWindow(&work->listWindow);
        if (rankings->valueTemplate) {
            String_Delete(rankings->valueTemplate);
        }
        VsRecorder_FreeListArrows(&work->listWindow, work);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&rankings->eva, &rankings->evb, TRUE, 1)) {
            work->step++;
        }
        break;
    case 2:
        if (VsRecorder_StepBlend(&rankings->eva, &rankings->evb, FALSE, 1)) {
            VsRecorder_CreateRankingsWindows(work);
            VsRecorder_PrintRankingsInfo(work);
            VsRecorder_InitRankingsList(work);
            work->step++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, 6);
        break;
    }
    return FALSE;
}

#pragma force_active reset
