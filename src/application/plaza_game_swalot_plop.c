#include "application/plaza_game_swalot_plop.h"

#include <string.h>

#include "global.h"

#include "constants/sndseq.h"

#include "application/plaza_game.h"

#include "bg_window.h"
#include "brightness.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "font_sprite.h"
#include "geometry_math.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "model_animation.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "render_window.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "unk_0200FA24.h"
#include "swap_3d_buffers.h"
#include "communication_command.h"
#include "communication_player_state.h"
#include "unk_02035900.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"
#include "wifi_connection.h"

// Declared in no public header (touchpad internals and library code).
BOOL GF_TouchpadStartRecording(TPData *data, u32 capacity, u32 frequency);
int GF_TouchpadUpdate(SwalotPlopTouch *out, int mode, u32 threshold);
void TextOBJ_UpdatePosition(TextOBJ *object);
void sub_0200FC20(u16 color);
void GF_SndHandleSetTempoRatio(int handleNo, int ratio);
void NNS_G3dMdlSetMdlAmbAll(NNSG3dResMdl *model, GXRgb color);
int MTX_Inverse33(const MtxFx33 *src, MtxFx33 *dst);
s32 _s32_div_f(s32 a, s32 b);
fx32 FX_Sqrt(fx32 x);
void NNS_G3dAnmObjDisableID(NNSG3dAnmObj *anmObj, int id);
void NNS_G3dAnmObjEnableID(NNSG3dAnmObj *anmObj, int id);
int NNS_G3dGetResDictIdxByName(const NNSG3dResDict *dict, const NNSG3dResName *name);

// Tables in asm/plaza_game_swalot_plop_data.s.
extern const u8 _02261BE8[];
extern const u8 ov91_02261BEC[];
extern const u8 ov91_02261BF0[];
extern const u16 ov91_02261BF8[];
extern const u16 ov91_02261BFE[];
extern const u16 ov91_02261C04[3];
extern const u16 ov91_02261C0A[];
extern const u8 ov91_02261C12[];
extern const u8 ov91_02261C13[];
extern const u16 ov91_02261C1C[6];
extern const VecFx32 ov91_02261C28;
extern const u32 ov91_02261C34[];
extern const u32 ov91_02261C44[];
extern const VecFx32 ov91_02261C4C;
extern int (*const ov91_02261C58[])(SwalotPlopBody *body, u32 frame, u32 duration);
extern const ObjCharTransferTemplate ov91_02261C64;
extern const GraphicsModes ov91_02261C74;
extern const s16 ov91_02261C84[];
extern const BgTemplate ov91_02261C9C;
extern const BgTemplate ov91_02261CB8;
extern const BgTemplate ov91_02261CD4;
extern const BgTemplate ov91_02261CF0;
extern const BgTemplate ov91_02261D0C;
extern const BgTemplate ov91_02261D28;
extern const u16 ov91_02261D3C[][4];
extern const u16 ov91_02261D5C[][4];
extern const u16 ov91_02261D64[][4];
extern const GraphicsBanks ov91_02261D84;
extern const u32 ov91_02261DAC[];
extern const u8 ov91_02261F8C[];
extern const u8 ov91_02261F90[];
extern const NNSG3dResName ov91_022621AC[6][15];
extern const u8 ov91_0226274C[6];
extern const SwalotPlopCommand ov91_02262754[8];
extern u8 ov91_022627C0[];

BOOL SwalotPlop_Init(OverlayManager *man, int *state) {
    PlazaGameArgs *args = OverlayManager_GetArgs(man);
    SwalotPlopApp *app;
    Heap_Create(HEAP_ID_3, HEAP_ID_106, 3 << 17);
    app = OverlayManager_CreateAndGetData(man, sizeof(SwalotPlopApp), HEAP_ID_106);
    memset(app, 0, sizeof(SwalotPlopApp));
    PlazaGameRoster_Init(&app->roster, args->fromLobby, args->saveData, args->unk38, args);
    SwalotPlop_LoadSharedData(app, HEAP_ID_106);
    return TRUE;
}

BOOL SwalotPlop_Main(OverlayManager *man, int *state) {
    SwalotPlopApp *app = OverlayManager_GetData(man);
    PlazaGameArgs *args = OverlayManager_GetArgs(man);

    struct {
        u32 level;
        u32 score;
        u8 options[4];
        u32 scores[4];
    } msg;

    if (PlazaGameRoster_CheckConnectionLost(&app->roster) == TRUE) {
        switch (app->connectionLostState) {
        case 0:
            if (IsPaletteFadeFinished() == TRUE) {
                sub_0200FB70();
            }
            sub_0200FC20(0);
            app->connectionLostState++;
            break;
        case 1:
            if (PlazaGameRoster_ProcessConnectionLost(&app->roster) == TRUE) {
                app->connectionLostState++;
            }
            break;
        case 2:
            SwalotPlop_Abort(app, args);
            return TRUE;
        default:
            GF_AssertFail();
            return TRUE;
        }
        return FALSE;
    }

    switch (*state) {
    case 0:
        app->entry = PlazaGameEntry_NewSwalotPlop(&app->roster, HEAP_ID_106);
        (*state)++;
        break;
    case 1:
        if (PlazaGameEntry_IsDone(app->entry) == TRUE) {
            app->selection = PlazaGameEntry_GetSelection(app->entry);
            PlazaGameEntry_Free(app->entry);
            app->entry = NULL;
            (*state)++;
        }
        break;
    case 2:
        if (gSystem.heldKeys & PAD_BUTTON_A) {
            break;
        }
        Main_SetVBlankIntrCB(SwalotPlop_VBlankCallback, app);
        HBlankInterruptDisable();
        app->startReceived = FALSE;
        app->finishReceived = FALSE;
        app->scoresReceived = FALSE;
        {
            u8 *first = &app->syncCounts[0];
            u8 *counts = &app->syncCounts[0];
            *first = 0;
            counts[1] = 0;
            counts[2] = 0;
            counts[3] = 0;
        }
        GF_CreateVramTransferManager(0x20, HEAP_ID_106);
        SwalotPlop_RegisterCommands(app);
        app->canSend = TRUE;
        app->playerCount = CommPlayerInfo_CountAccepted();
        if (app->playerCount <= 1) {
            GF_AssertFail();
        }
        app->netId = Comm_GetLocalNetId();
        app->localIndex = PlazaGameRoster_GetIndexOfNetId(&app->roster, app->netId);
        if (app->netId == 0) {
            app->tally = SwalotPlopTally_New(HEAP_ID_106, 0x4B0, app->playerCount, &app->shared);
            app->syncRound = 1;
        }
        app->game = SwalotPlopGame_New(HEAP_ID_106, 0x4B0, app->playerCount, app->localIndex, &app->shared);
        WirelessStatusIcon_Create();
        if (args->unk38 != 0) {
            WifiConnection_StartVoiceChat(HEAP_ID_106);
        }
        switch (app->selection) {
        case 0:
            msg.options[0] = 0;
            msg.options[1] = 0;
            break;
        case 1:
            msg.options[0] = 0;
            msg.options[1] = 1;
            break;
        case 2:
            msg.options[0] = 1;
            msg.options[1] = 0;
            break;
        default:
            GF_AssertFail();
            break;
        }
        SwalotPlopGame_SetOptions(app->game, msg.options);
        LinkSync_StartSync(1);
        (*state)++;
        break;
    case 3:
        if (LinkSync_IsSynced(1) != 0) {
            BeginNormalPaletteFade(0, 0x1B, 0x1B, 0xFFFF, 6, 1, HEAP_ID_106);
            (*state)++;
        }
        break;
    case 4:
        SwalotPlopGame_RunIntro(app->game, 0);
        if (IsPaletteFadeFinished() != 0) {
            if (app->netId == 0) {
                if (Comm_SendCommand(22, NULL, 0) != 0) {
                    (*state)++;
                }
            } else {
                (*state)++;
            }
        }
        break;
    case 5:
        SwalotPlopGame_RunIntro(app->game, 0);
        if (app->startReceived != 0) {
            (*state)++;
        }
        break;
    case 6:
        if (SwalotPlopGame_RunIntro(app->game, 1) == 0) {
            (*state)++;
        }
        break;
    case 7:
        if (app->finishReceived != 0) {
            msg.score = SwalotPlopGame_GetScore(app->game);
            if (Comm_SendCommand(25, &msg.score, 4) != 0) {
                (*state)++;
            }
        } else {
            if (app->netId == 0) {
                u32 running = SwalotPlopTally_Update(app->tally);
                if (SwalotPlopTally_LevelChanged(app->tally) != 0) {
                    msg.level = SwalotPlopTally_GetLevel(app->tally);
                    if (SwalotPlop_SendCommand(app, 27, &msg.level, 4) == TRUE) {
                        SwalotPlopTally_ClearLevelChanged(app->tally);
                    }
                }
                if (running == 0) {
                    SwalotPlop_SendCommand(app, 23, NULL, 0);
                }
            }
            SwalotPlopGame_Update(app->game);
            SwalotPlop_SendShots(app);
            SwalotPlop_SendScoreReport(app);
        }
        break;
    case 8:
        SwalotPlopGame_RunFinish(app->game, 0);
        if (app->netId == 0) {
            if (SwalotPlopTally_HasAllScores(app->tally) == TRUE) {
                SwalotPlopTally_GetScores(app->tally, msg.scores);
                if (Comm_SendCommand(26, msg.scores, 0x10) != 0) {
                    (*state)++;
                }
            }
        } else {
            (*state)++;
        }
        break;
    case 9:
        SwalotPlopGame_RunFinish(app->game, 0);
        if (app->scoresReceived == TRUE) {
            (*state)++;
        }
        break;
    case 10:
        if (SwalotPlopGame_RunFinish(app->game, 1) == 0) {
            (*state)++;
        }
        break;
    case 11:
        BeginNormalPaletteFade(0, 0x1A, 0x1A, 0xFFFF, 6, 1, HEAP_ID_106);
        SwalotPlopGame_RunFinish(app->game, 1);
        (*state)++;
        break;
    case 12:
        SwalotPlopGame_RunFinish(app->game, 1);
        if (IsPaletteFadeFinished() != 0) {
            LinkSync_StartSync(3);
            (*state)++;
        }
        break;
    case 13:
        if (LinkSync_IsSynced(3) == 0) {
            return FALSE;
        }
        if (args->unk38 != 0) {
            WifiConnection_StopVoiceChat();
        }
        if (app->netId == 0) {
            SwalotPlopTally_Free(app->tally);
            app->tally = NULL;
        }
        SwalotPlopGame_Free(app->game);
        app->game = NULL;
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GF_DestroyVramTransferManager();
        (*state)++;
        break;
    case 14:
        PlazaGame_CalcRanks(&app->resultData, app->roster.count);
        app->results = PlazaGameResults_NewSwalotPlop(&app->roster, &app->resultData, HEAP_ID_106);
        (*state)++;
        break;
    case 15:
        if (PlazaGameResults_IsDone(app->results) == TRUE) {
            u32 keepPlaying = PlazaGameResults_GetContinue(app->results);
            PlazaGameResults_Free(app->results);
            app->results = NULL;
            if (keepPlaying == 0) {
                return TRUE;
            }
            *state = 0;
        }
        break;
    }
    return FALSE;
}

BOOL SwalotPlop_Exit(OverlayManager *man, int *state) {
    SwalotPlopApp *app = OverlayManager_GetData(man);
    BOOL connectionLost;
    OverlayManager_GetArgs(man);
    switch (*state) {
    case 0:
        connectionLost = PlazaGameRoster_CheckConnectionLost(&app->roster);
        SwalotPlop_FreeSharedData(app);
        OverlayManager_FreeData(man);
        Heap_Destroy(HEAP_ID_106);
        CommState_SetErrorWatch(0, 1);
        if (connectionLost == TRUE) {
            return TRUE;
        }
        LinkSync_StartSync(4);
        (*state)++;
        break;
    case 1:
        if (LinkSync_IsSynced(4) != 0 || Comm_CountConnected() < CommPlayerInfo_CountAccepted()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void SwalotPlop_ReceiveStart(SwalotPlopApp *app) {
    app->startReceived = TRUE;
}

void SwalotPlop_ReceiveFinish(SwalotPlopApp *app) {
    app->finishReceived = TRUE;
}

void SwalotPlop_TallyShot(SwalotPlopApp *app, const SwalotPlopShotRecord *record, int netId) {
    int index = PlazaGameRoster_GetIndexOfNetId(&app->roster, netId);
    SwalotPlopTally_AddShot(app->tally, record, index);
}

void SwalotPlop_ReceiveShot(SwalotPlopApp *app, const SwalotPlopShotRecord *record, int netId) {
    SwalotPlopGame_AddRemoteShot(app->game, record);
}

void SwalotPlop_ReceiveFinalScore(SwalotPlopApp *app, u32 score, int netId) {
    int index = PlazaGameRoster_GetIndexOfNetId(&app->roster, netId);
    SwalotPlopTally *tally = app->tally;
    if (tally != NULL) {
        SwalotPlopTally_SetScore(tally, score, index);
    }
}

void SwalotPlop_ReceiveScores(SwalotPlopApp *app, const u32 *scores, int netId) {
    // walks the struct shifted by one element per step, as the original code does
    SwalotPlopGame_SetFinalScores(app->game, scores);
    app->scoresReceived = TRUE;
    for (int i = 0; i < 4; i++) {
        app->resultData.scores[0] = *scores;
        scores++;
        app = (SwalotPlopApp *)((u8 *)app + 4);
    }
}

void SwalotPlop_ReceiveLevel(SwalotPlopApp *app, u32 level) {
    SwalotPlopGame_SetLevel(app->game, level);
}

#pragma force_active on

void SwalotPlop_ReceiveScoreReport(SwalotPlopApp *app, u32 score, int netId) {
    int index = PlazaGameRoster_GetIndexOfNetId(&app->roster, netId);
    u32 i;
    BOOL allReported;
    SwalotPlopGame_SetScoreReport(app->game, score, index);
    app->syncCounts[index]++;
    if (app->netId == 0) {
        allReported = TRUE;
        for (i = 0; i < app->playerCount; i++) {
            if (app->syncRound > app->syncCounts[i]) {
                allReported = FALSE;
            }
        }
        if (allReported == TRUE) {
            app->syncRound++;
            Comm_SendCommand(29, NULL, 0);
        }
    }
}

#pragma force_active reset

void SwalotPlop_ReceiveSync(SwalotPlopApp *app) {
    app->canSend = TRUE;
}

void SwalotPlop_VBlankCallback(void *data) {
    SwalotPlopApp *app = data;
    SwalotPlopGame_VBlank(app->game);
}

void SwalotPlop_SendShots(SwalotPlopApp *app) {
    SwalotPlopShotRecord record;
    if (SwalotPlopGame_TakeShotReport(app->game, &record) == TRUE) {
        do {
            SwalotPlop_SendCommand(app, 24, &record, sizeof(SwalotPlopShotRecord));
        } while (SwalotPlopGame_TakeShotReport(app->game, &record) == TRUE);
    }
}

void SwalotPlop_SendScoreReport(SwalotPlopApp *app) {
    int frame = SwalotPlopGame_GetFrame(app->game);
    if (frame != 0 && frame % 30 == 0 && app->canSend == TRUE) {
        u32 score = SwalotPlopGame_GetScore(app->game);
        if (!SwalotPlop_SendCommand(app, 28, &score, 4)) {
            GF_AssertFail();
        }
        app->canSend = FALSE;
    }
}

#pragma force_active on

void SwalotPlop_LoadSharedData(SwalotPlopApp *app, int heapId) {
    u32 size;
    int i;
    SwalotPlopApp *walker;
    PlazaGameRoster *roster;
    app->shared.levelTable.levels = GfGfxLoader_LoadFromNarc_GetSizeOut((NarcId)212, 0, FALSE, (enum HeapID)heapId, FALSE, &size);
    app->shared.levelTable.count = size / sizeof(SwalotPlopLevel);
    i = 0;
    if (i < app->roster.count) {
        roster = (PlazaGameRoster *)app;
        walker = app;
        roster = (PlazaGameRoster *)((u8 *)roster + 8);
        // walks profiles[] and nameFlags[] through a pointer advanced by 4 bytes per player
        do {
            walker->shared.profiles[0] = PlazaGameRoster_GetProfile(roster, i);
            walker->shared.nameFlags[0] = ov90_022588A4(roster, i);
            i++;
            walker = (SwalotPlopApp *)((u8 *)walker + 4);
        } while (i < app->roster.count);
    }
}

#pragma force_active reset

void SwalotPlop_FreeSharedData(SwalotPlopApp *app) {
    Heap_Free(app->shared.levelTable.levels);
    app->shared.levelTable.levels = NULL;
    app->shared.levelTable.count = 0;
}

void SwalotPlop_Abort(SwalotPlopApp *app, PlazaGameArgs *args) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (app->entry != NULL) {
        PlazaGameEntry_Free(app->entry);
        app->entry = NULL;
    }
    if (app->game != NULL) {
        if (app->netId == 0) {
            SwalotPlopTally_Free(app->tally);
        }
        SwalotPlopGame_Free(app->game);
        GF_DestroyVramTransferManager();
    }
    if (args->unk38 != 0) {
        WifiConnection_StopVoiceChat();
    }
    if (app->results != NULL) {
        PlazaGameResults_Free(app->results);
        app->results = NULL;
    }
}

BOOL SwalotPlop_SendCommand(SwalotPlopApp *app, int command, void *data, int size) {
    if (app->canSend == FALSE) {
        return FALSE;
    }
    return Comm_SendCommand(command, data, size);
}

SwalotPlopTally *SwalotPlopTally_New(enum HeapID heapId, int duration, u32 playerCount, SwalotPlopShared *shared) {
    SwalotPlopTally *tally = Heap_Alloc(heapId, sizeof(SwalotPlopTally));
    memset(tally, 0, sizeof(SwalotPlopTally));
    tally->timer = duration;
    tally->playerCount = playerCount;
    tally->shared = shared;
    tally->running = TRUE;
    return tally;
}

void SwalotPlopTally_Free(SwalotPlopTally *tally) {
    Heap_Free(tally);
}

#pragma force_active on

BOOL SwalotPlopTally_Update(SwalotPlopTally *tally) {
    int i;
    u32 count, walk;
    u8 previous;
    SwalotPlopLevelTable *table;
    if (tally->timer > 0 && tally->running == TRUE) {
        tally->timer--;
    }
    if (tally->timer <= 0) {
        return FALSE;
    }
    previous = tally->level;
    table = &tally->shared->levelTable;
    count = table->count;
    i = previous;
    walk = previous;
    if (previous < count) {
        SwalotPlopLevel *level = &table->levels[previous];
        u16 hitCount = tally->hitCount;
        do {
            if (level->hitCount >= hitCount) {
                break;
            }
            i++;
            level++;
            walk = i;
        } while (i < count);
    }
    if (walk == count) {
        i--;
    }
    if (i != previous) {
        tally->level = i;
        tally->levelChanged = TRUE;
    }
    return TRUE;
}

#pragma force_active reset

u8 SwalotPlopTally_LevelChanged(SwalotPlopTally *tally) {
    return tally->levelChanged;
}

void SwalotPlopTally_ClearLevelChanged(SwalotPlopTally *tally) {
    tally->levelChanged = FALSE;
}

u8 SwalotPlopTally_GetLevel(SwalotPlopTally *tally) {
    return tally->level;
}

void SwalotPlopTally_AddShot(SwalotPlopTally *tally, const SwalotPlopShotRecord *record, int index) {
    if (record->caught != 0) {
        tally->hitCount++;
    }
}

void SwalotPlopTally_SetScore(SwalotPlopTally *tally, u32 score, int index) {
    u32 mask;
    if (score > 99999) {
        score = 99999;
    }
    mask = 1 << index;
    if (!(tally->receivedMask & mask)) {
        tally->scores[index] = score;
        tally->receivedMask |= mask;
    }
}

BOOL SwalotPlopTally_HasAllScores(SwalotPlopTally *tally) {
    if (SwalotPlopTally_CountScores(tally) >= tally->playerCount) {
        return TRUE;
    }
    return FALSE;
}

void SwalotPlopTally_GetScores(SwalotPlopTally *tally, u32 *scores) {
    *(SwalotPlopScores *)scores = *(SwalotPlopScores *)tally->scores;
}

u32 SwalotPlopTally_CountScores(SwalotPlopTally *tally) {
    s32 i;
    s32 count = 0;
    u32 mask = tally->receivedMask;
    for (i = count; i < 4; i++) {
        if ((1 << i) & mask) {
            count++;
        }
    }
    return count;
}

SwalotPlopGame *SwalotPlopGame_New(enum HeapID heapId, u32 duration, u32 playerCount, u32 localIndex, SwalotPlopShared *shared) {
    SwalotPlopGame *game = Heap_Alloc(heapId, sizeof(SwalotPlopGame));
    memset(game, 0, sizeof(SwalotPlopGame));
    game->running = TRUE;
    game->frame = 0;
    game->duration = duration;
    game->playerCount = playerCount;
    game->localIndex = localIndex;
    game->shared = shared;
    SwalotPlopBody_Init(game, &game->body);
    SwalotPlopGame_InitGfx(game, heapId);
    SwalotPlopDemo_Init(&game->demo, &game->gfx, heapId);
    SwalotPlopScoreReports_Init(&game->scoreReports);
    if (GF_TouchpadStartRecording((TPData *)ov91_022627C0, 8, 4) != TRUE) {
        GF_AssertFail();
    }
    return game;
}

void SwalotPlopGame_Free(SwalotPlopGame *game) {
    if (GF_TouchpadStopSampling() != TRUE) {
        GF_AssertFail();
    }
    SwalotPlopDemo_Free(&game->demo, &game->gfx);
    SwalotPlopGame_FreeGfx(game);
    Heap_Free(game);
}

#pragma force_active on

BOOL SwalotPlopGame_RunIntro(SwalotPlopGame *game, BOOL ready) {
    BOOL result = TRUE;
    if (game->gfx.intro.state >= 4) {
        int speed = game->options[1] ? -120 : 120;
        SwalotPlopBody_SetSpin(&game->body, (u16)(game->body.spin + speed));
    }
    SwalotPlopGfx_SetScenePos(&game->gfx, &game->body);
    SwalotPlopScene_SetSpin(&game->gfx.scene, &game->body);
    SwalotPlopScene_SetScale(&game->gfx.scene, &game->body);
    switch (game->gfx.intro.state) {
    case 0:
        SwalotPlopDemo_Start(&game->demo);
        game->gfx.intro.state++;
        break;
    case 1:
        if (SwalotPlopDemo_Update(&game->demo, game) == TRUE) {
            game->gfx.intro.state++;
            LinkSync_StartSync(2);
        }
        break;
    case 2:
        if (LinkSync_IsSynced(2)) {
            game->gfx.intro.state++;
            game->gfx.intro.counter = 65;
        }
        break;
    case 3:
        game->gfx.intro.counter--;
        if (game->gfx.intro.counter == 50) {
            PlaySE(SEQ_SE_DP_KI_GASYAN);
        }
        if (game->gfx.intro.counter == 20) {
            PlaySE(SEQ_SE_DP_HAMARU);
        }
        SwalotPlopScene_AdvanceAnimation(&game->gfx.scene, FX32_ONE);
        if (!game->gfx.intro.counter) {
            SwalotPlopIntro_StartScene(&game->gfx.intro, &game->gfx);
            game->gfx.intro.state = 4;
        }
        break;
    case 4:
        if (ready == TRUE) {
            game->gfx.intro.state = 5;
        }
        break;
    case 5:
        SwalotPlopIntro_ShowWindow(&game->gfx.intro, &game->gfx);
        PlaySE(SEQ_SE_DP_DECIDE);
        game->gfx.intro.state = 6;
        break;
    case 6:
        if (!SwalotPlopIntro_IsCountingDown(&game->gfx.intro, &game->gfx)) {
            game->gfx.intro.state = 7;
        }
        break;
    case 7:
        if (!SwalotPlopIntro_Wait(&game->gfx.intro, &game->gfx)) {
            game->gfx.intro.state = 8;
        }
        break;
    case 8:
        SwalotPlopIntro_HideWindow(&game->gfx.intro, &game->gfx);
        SwalotPlopBody_SetActive(&game->body, 1);
        result = FALSE;
        break;
    }
    SwalotPlopScene_Animate(&game->gfx.scene, &game->body, game->phase);
    SwalotPlopGame_Draw(game, &game->gfx);
    return result;
}

BOOL SwalotPlopGame_RunFinish(SwalotPlopGame *game, BOOL ready) {
    BOOL result = TRUE;
    switch (game->gfx.finish.state) {
    case 0:
        SwalotPlopGame_DropFlyingShots(game);
        SwalotPlopGame_EndFlick(game);
        StartBrightnessTransition(24, -14, 0, (GXBlendPlaneMask)0x3F, 2);
        SwalotPlopFinish_Start(&game->gfx);
        PlazaGameSign_StartFinish(game->gfx.sign);
        SwalotPlopSpeedUp_ResetTempo(&game->speedUp);
        SwalotPlopCombo_Hide(&game->gfx.combo);
        game->gfx.finish.state++;
        break;
    case 1: {
        BOOL done = TRUE;
        if (!SwalotPlopFinish_Update(&game->gfx)) {
            done = FALSE;
        }
        SwalotPlopGame_UpdateShots(game);
        SwalotPlopGame_DropEndedShots(game);
        if (SwalotPlopGame_HasActiveShots(game) == TRUE) {
            done = FALSE;
        }
        if (!SwalotPlopFinish_IsSignDone(&game->gfx)) {
            done = FALSE;
        }
        if (!IsBrightnessTransitionActive(2)) {
            done = FALSE;
        }
        if (done == TRUE && ready == TRUE) {
            game->gfx.finish.counter = 0;
            game->gfx.finish.state++;
        }
        break;
    }
    case 2:
        game->gfx.finish.counter++;
        if (game->gfx.finish.counter >= 64) {
            game->gfx.finish.state++;
        }
        break;
    case 3:
        result = FALSE;
        break;
    }
    int speed = game->options[1] ? -120 : 120;
    SwalotPlopBody_SetSpin(&game->body, (u16)(game->body.spin + speed));
    SwalotPlopScene_SetPos(&game->gfx.scene, &game->body);
    SwalotPlopScene_SetSpin(&game->gfx.scene, &game->body);
    SwalotPlopScene_Animate(&game->gfx.scene, &game->body, game->phase);
    SwalotPlopGame_Draw(game, &game->gfx);
    SwalotPlopCombo_Update(&game->gfx.combo);
    return result;
}

#pragma force_active reset

BOOL SwalotPlopGame_Update(SwalotPlopGame *game) {
    u32 phase = game->phase;
    u32 leader;
    if (phase < 2 && ov91_02261C44[phase] <= game->frame) {
        game->phase = phase + 1;
        if (game->phase == SWALOT_PLOP_PHASE_FAST) {
            SwalotPlopSpeedUp_Start(&game->speedUp);
        }
    }
    GF_TouchpadUpdate(&game->touch, 1, 0);
    SwalotPlopGame_HandleTouch(game);
    if (!SwalotPlopLevelBlend_Update(&game->levelBlend)) {
        SwalotPlopGame_ApplyLevel(game, &game->levelBlend);
    }
    if (SwalotPlopScoreReports_HasAll(&game->scoreReports, game->playerCount) == TRUE) {
        leader = SwalotPlopScoreReports_GetLeader(&game->scoreReports);
        if (leader != 0xFFFF) {
            SwalotPlopScene_SetLeader(&game->gfx.scene, leader);
        }
        SwalotPlopScoreReports_Reset(&game->scoreReports);
    }
    SwalotPlopGame_UpdateShots(game);
    SwalotPlopBody_Update(game, &game->body);
    SwalotPlopGame_UpdateGfx(game, &game->gfx);
    SwalotPlopGame_RemoveEndedShots(game);
    SwalotPlopSpeedUp_Update(&game->speedUp, &game->gfx.stage, &game->gfx.scene);
    if (game->running == TRUE) {
        game->frame++;
    }
    if ((s32)game->frame >= (s32)game->duration) {
        game->frame = game->duration;
        return TRUE;
    }
    return FALSE;
}

void SwalotPlopGame_VBlank(SwalotPlopGame *game) {
    DoScheduledBgGpuUpdates(game->gfx.bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
}

u32 SwalotPlopGame_GetFrame(SwalotPlopGame *game) {
    return game->frame;
}

void SwalotPlopGame_AddRemoteShot(SwalotPlopGame *game, const SwalotPlopShotRecord *record) {
    SwalotPlopShotParams params;
    if (game->localIndex != record->owner) {
        SwalotPlopShot_FromRecord(record, &params);
        SwalotPlopGame_AddShot(game, &params, SHOT_STATE_FALLING);
        if (record->caught != 0) {
            SwalotPlopPopups_Show(game->gfx.popups, record->owner, record->combo, game->phase);
        }
    }
}

#pragma force_active on

BOOL SwalotPlopGame_TakeShotReport(SwalotPlopGame *game, SwalotPlopShotRecord *record) {
    // some accesses go through offsetof(shots) as in the original code (register allocation)
    int i;
    for (i = 0; i < 0x60; i++) {
        if (game->shots[i].state == SHOT_STATE_REPORT) {
            SwalotPlopShot_ToRecord((SwalotPlopShotParams *)((u32)&game->shots[0].params + i * sizeof(SwalotPlopShot)), record);
            SwalotPlopShot_Clear((SwalotPlopShot *)((u32)game->shots + i * sizeof(SwalotPlopShot)));
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active reset

void SwalotPlopGame_SetFinalScores(SwalotPlopGame *game, const u32 *scores) {
    *(SwalotPlopScores *)game->finalScores = *(const SwalotPlopScores *)scores;
}

u32 SwalotPlopGame_GetScore(SwalotPlopGame *game) {
    return game->score;
}

void SwalotPlopGame_SetLevel(SwalotPlopGame *game, u32 level) {
    u16 prev = game->level;
    game->prevLevel = prev;
    game->level = level;
    SwalotPlopLevelBlend_Start(&game->levelBlend, game->shared, game->options, game->prevLevel, game->level);
}

#pragma force_active on

void SwalotPlopGame_SetScoreReport(SwalotPlopGame *game, u32 score, int index) {
    SwalotPlopScoreReports_Set(&game->scoreReports, score, index);
}

#pragma force_active reset

void SwalotPlopGame_SetOptions(SwalotPlopGame *game, const u8 *options) {
    game->options[0] = options[0];
    game->options[1] = options[1];
    game->options[2] = options[2];
    game->options[3] = options[3];
    game->optionsSet = TRUE;
    SwalotPlopLevelBlend_Init(&game->levelBlend, game->shared, game->options);
    SwalotPlopGame_ApplyLevel(game, &game->levelBlend);
}

void SwalotPlopCurve_Init(SwalotPlopCurve *curve, s32 start, s32 end, s32 slope, s32 duration) {
    s32 delta = end - start - (s32)(((s64)slope * (duration << 12) + 0x800) >> 12);
    s32 curvature = FX_Div((s32)((((s64)delta << 13) + 0x800) >> 12), (duration * duration) << 12);
    curve->value = start;
    curve->start = start;
    curve->slope = slope;
    curve->curvature = curvature;
    curve->step = 0;
    curve->duration = duration;
}

BOOL SwalotPlopCurve_Update(SwalotPlopCurve *curve) {
    s32 step = curve->step;
    s32 slopePart = (s32)(((s64)curve->slope * (step << 12) + 0x800) >> 12);
    s32 curvePart = (s32)(((s64)curve->curvature * ((step * step) << 12) + 0x800) >> 12);
    curve->value = curve->start + (slopePart + FX_Div(curvePart, 0x2000));
    s32 current = *(volatile s32 *)&curve->step;
    s32 duration = curve->duration;
    s32 next = current + 1;
    if (next <= duration) {
        curve->step = next;
        return FALSE;
    }
    curve->step = duration;
    return TRUE;
}

void SwalotPlop_GetPopupPos(int seat, int playerCount, int index, u32 *x, u32 *y) {
    *x = *(const u32 *)(ov91_02261F8C + playerCount * 0x80 + seat * 0x20 + index * 8);
    *y = *(const u32 *)(ov91_02261F90 + playerCount * 0x80 + seat * 0x20 + index * 8);
}

#pragma force_active on

void SwalotPlopGame_ThrowDemoShot(SwalotPlopGame *game, s32 x, s32 y, s32 endX, u16 endY, int state) {
    SwalotPlopShotParams params;
    params.owner = game->localIndex;
    params.caught = 0;
    params.direction.x = (u32)(s32)(s16)((s32)((u32)x - (u32)endX) / 2) << 12;
    params.direction.z = (u32)(s32)(s16)((s32)((u32)y - (u32)endY) / 2) << 12;
    params.direction.y = 0;
    if (VEC_Mag(&params.direction) > 0x1E200) {
        VEC_Normalize(&params.direction, &params.direction);
        params.direction.x = ((s64)params.direction.x * 0x1E200 + 2048) >> 12;
        params.direction.z = ((s64)params.direction.z * 0x1E200 + 2048) >> 12;
    }
    params.direction.y = 38 << 12;
    params.speed = VEC_Mag(&params.direction);
    params.speed = ((s64)params.speed * 0x99A + 2048) >> 12;
    VEC_Normalize(&params.direction, &params.direction);
    SwalotPlop_TouchToWorld((s16)endX, (s16)endY, &params.start);
    params.start.y = 0;
    SwalotPlop_RotateToSeat(&params.start, &params.start, params.owner, game->playerCount);
    SwalotPlop_RotateToSeat(&params.direction, &params.direction, params.owner, game->playerCount);
    params.big = FALSE;
    SwalotPlopGame_AddShot(game, &params, state);
}

#pragma force_active reset

u32 SwalotPlopGame_GetHitPoints(SwalotPlopGame *game) {
    u32 index;
    u32 points;
    s32 combo = game->combo;
    if (combo >= 3) {
        index = 2;
    } else {
        index = (u8)combo;
    }
    points = ov91_02261C34[index];
    if (game->phase == SWALOT_PLOP_PHASE_FASTEST) {
        points = points + points * 2;
    }
    return points;
}

#pragma force_active on

void SwalotPlopPopups_Init(SwalotPlopPopups *popups, SwalotPlopGfx *gfx, int playerCount, int localIndex, enum HeapID heapId) {
    SpriteTemplate template;
    int i;
    template.spriteList = gfx->spriteList;
    template.header = &gfx->header;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = heapId;
    for (i = 0; i < 4; i++, popups++) {
        SwalotPlopPopups_InitSeat(popups, &template, gfx, playerCount, i, localIndex, heapId);
    }
}

#pragma force_active reset

void SwalotPlopPopups_Free(SwalotPlopPopups *popups, SwalotPlopGfx *gfx) {
    int i;
    for (i = 0; i < 4; i++) {
        SwalotPlopPopups_FreeSeat(popups);
        popups++;
    }
}

#pragma force_active on

void SwalotPlopPopups_Show(SwalotPlopPopups *popups, int seat, int combo, int phase) {
    SwalotPlopPopups_ShowSeat(&popups[seat], seat, combo, phase);
}

#pragma force_active reset

void SwalotPlopPopups_Update(SwalotPlopPopups *popups) {
    int i;
    for (i = 0; i < 4; i++) {
        SwalotPlopPopups_UpdateSeat(popups);
        popups++;
    }
}

#pragma force_active on

void SwalotPlopPopups_InitSeat(SwalotPlopPopups *popups, SpriteTemplate *template, SwalotPlopGfx *gfx, int playerCount, int seat, int localIndex, enum HeapID heapId) {
    int i;

    struct {
        u32 y;
        u32 x;
    } coord;

    for (i = 0; i < 3; i++) {
        SwalotPlop_GetPopupPos(playerCount, localIndex, seat, &coord.x, &coord.y);
        template->position.x = coord.x << 12;
        template->position.y = coord.y << 12;
        popups->pos[i] = template->position;
        popups->sprites[i] = Sprite_CreateAffine(template);
        Sprite_SetAnimCtrlSeq(popups->sprites[i], 2);
        Sprite_SetDrawFlag(popups->sprites[i], FALSE);
        Sprite_SetAnimActiveFlag(popups->sprites[i], TRUE);
        Sprite_SetAnimSpeed(popups->sprites[i], 6 << 10);
        popups->timers[i] = 0;
    }
    if (localIndex == seat) {
        popups->mode = 1;
    } else {
        popups->mode = 0;
    }
}

#pragma force_active reset

void SwalotPlopPopups_FreeSeat(SwalotPlopPopups *popups) {
    int i;
    Sprite **sprite = popups->sprites;
    for (i = 0; i < 3; i++) {
        Sprite_Delete(*sprite);
        sprite++;
    }
}

#pragma force_active on

void SwalotPlopPopups_ShowSeat(SwalotPlopPopups *popups, int seat, u32 combo, int phase) {
    int index;
    BOOL found;
    u32 maxPriority;
    int i;
    u32 priority;
    int palette;
    found = FALSE;
    maxPriority = 0;
    index = -1;
    for (i = 0; i < 3; i++) {
        if (Sprite_GetDrawFlag(popups->sprites[i]) == FALSE) {
            index = i;
            found = TRUE;
        } else {
            priority = Sprite_GetDrawPriority(popups->sprites[i]);
            Sprite_SetDrawPriority(popups->sprites[i], priority + 1);
            if (found == FALSE && maxPriority <= priority + 1) {
                maxPriority = priority + 1;
                index = i;
            }
        }
    }
    GF_ASSERT(index >= 0);
    if (combo >= 3) {
        combo = 2;
    }
    if (phase != SWALOT_PLOP_PHASE_FASTEST) {
        if (popups->mode == 1) {
            combo += 2;
        } else {
            combo += 8;
        }
        palette = seat + 3;
    } else {
        if (popups->mode == 1) {
            combo += 5;
        } else {
            combo += 11;
        }
        palette = 7;
    }
    Sprite_SetAnimCtrlSeq(popups->sprites[index], combo);
    popups->timers[index] = 0;
    Sprite_SetDrawFlag(popups->sprites[index], TRUE);
    SwalotPlopPopups_SetPos(popups, index);
    Sprite_SetDrawPriority(popups->sprites[index], 0);
    Sprite_SetPaletteOverride(popups->sprites[index], palette);
}

#pragma force_active reset

void SwalotPlopPopups_UpdateSeat(SwalotPlopPopups *popups) {
    int i;
    SwalotPlopPopups *slot;
    i = 0;
    // slot i is addressed as the struct shifted by 4 * i (sprites[] and timers[] are both 4 bytes wide)
    slot = popups;
    while (i < 3) {
        if (Sprite_GetDrawFlag(slot->sprites[0]) == TRUE) {
            u32 timer = ++slot->timers[0];
            if (timer > 16) {
                SwalotPlopPopups_HideSprite(popups, i);
            } else {
                SwalotPlopPopups_SetPos(popups, i);
            }
        }
        i++;
        slot = (SwalotPlopPopups *)((u8 *)slot + 4);
    }
}

#pragma force_active on

void SwalotPlopPopups_SetPos(SwalotPlopPopups *popups, int i) {
    VecFx32 pos;
    fx32 y = FX_Div((fx32)(((s64)(popups->timers[i] << 12) * -65536 + 2048) >> 12), 65536);
    pos = popups->pos[i];
    pos.y += y;
    Sprite_SetMatrix(popups->sprites[i], &pos);
}

#pragma force_active reset

void SwalotPlopPopups_HideSprite(SwalotPlopPopups *popups, int i) {
    Sprite_SetDrawFlag(popups->sprites[i], FALSE);
}

#pragma force_active on

void SwalotPlopIntro_Init(SwalotPlopIntro *intro, SwalotPlopGfx *gfx, SwalotPlopShared *shared, u32 playerCount, u32 localIndex, NARC *narc, enum HeapID heapId) {
    u8 *bytes = (u8 *)intro;
    u32 size = sizeof(*intro);
    do {
        *bytes++ = 0;
    } while (--size);
    AddWindowParameterized(gfx->bgConfig, &intro->window, GF_BG_LYR_MAIN_1, 2, 19, 28, 4, 5, 10);
    FillWindowPixelRect(&intro->window, 15, 0, 0, 224, 32);
    String *string = String_New(128, heapId);
    ReadMsgDataIntoString(gfx->msgData, 0, string);
    AddTextPrinterParameterizedWithColor(&intro->window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(string);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 18, gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, playerCount + 18, gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 19, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 128, heapId);
    Window window;
    InitWindow(&window);
    AddWindowParameterized(gfx->bgConfig, &window, GF_BG_LYR_MAIN_2, 0, 0, 8, 2, 5, 256);
    u32 x;
    u32 baseTile = 256;
    string = String_New(128, heapId);
    u32 i;
    for (i = 0; i < playerCount; i++) {
        const u32 *table = ov91_02261DAC + localIndex * 32 + playerCount * 8 + i * 2;
        if (i != localIndex) {
            x = table[-8];
            u32 y = table[-7];
            BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_MAIN_2, (u8)(x - 1), (u8)(y - 1), 10, 4, (u8)i);
            FillWindowPixelBuffer(&window, 15);
            u32 color = shared->nameFlags[i] == 1 ? MAKE_TEXT_COLOR(5, 6, 15) : MAKE_TEXT_COLOR(1, 2, 15);
            PlayerName_FlatToString(shared->profiles[i], string);
            SetWindowX(&window, (u8)x);
            SetWindowY(&window, (u8)y);
            window.baseTile = (u16)baseTile;
            u32 width = FontID_String_GetWidth(0, string, 0);
            AddTextPrinterParameterizedWithColor(&window, 0, string, (64 - width) >> 1, 0, TEXT_SPEED_INSTANT, color, NULL);
            baseTile += 16;
        }
    }
    String_Delete(string);
    RemoveWindow(&window);
    intro->delay = 0;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active reset

void SwalotPlopIntro_Free(SwalotPlopIntro *intro, SwalotPlopGfx *gfx) {
    RemoveWindow(&intro->window);
}

void SwalotPlopIntro_ShowWindow(SwalotPlopIntro *intro, SwalotPlopGfx *gfx) {
    DrawFrameAndWindow1(&intro->window, FALSE, 1, 4);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    PlazaGameSign_StartCountdown(gfx->sign);
}

BOOL SwalotPlopIntro_IsCountingDown(SwalotPlopIntro *intro, SwalotPlopGfx *gfx) {
    if (PlazaGameSign_IsDone(gfx->sign) == TRUE) {
        return FALSE;
    }
    return TRUE;
}

void SwalotPlopIntro_HideWindow(SwalotPlopIntro *intro, SwalotPlopGfx *gfx) {
    EraseFrameAndWindow1(&intro->window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&intro->window);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void SwalotPlopIntro_StartScene(SwalotPlopIntro *intro, SwalotPlopGfx *gfx) {
    SwalotPlopScene_SetAnimation(&gfx->scene, 1);
    ModelAnimation_SetFrame(&gfx->scene.animations[1], 0xB4000);
    SwalotPlopScene_SetBaseAnimation(&gfx->scene, 1);
}

BOOL SwalotPlopIntro_Wait(SwalotPlopIntro *intro, SwalotPlopGfx *gfx) {
    intro->delay++;
    if (intro->delay < 8) {
        return TRUE;
    }
    return FALSE;
}

void SwalotPlopGfx_SetScenePos(SwalotPlopGfx *gfx, SwalotPlopBody *body) {
    SwalotPlopScene_SetPos(&gfx->scene, body);
}

void SwalotPlopFinish_Init(SwalotPlopGfx *gfx, u32 playerCount, u32 localIndex, enum HeapID heapId) {
    gfx->finish.state = 0;
    gfx->finish.counter = 0;
}

void ov91_0225DBE0(SwalotPlopGfx *gfx, u32 playerCount) {
}

void SwalotPlopFinish_Start(SwalotPlopGfx *gfx) {
    SwalotPlopScene_SetAnimation(&gfx->scene, 5);
}

BOOL SwalotPlopFinish_Update(SwalotPlopGfx *gfx) {
    gfx->finish.counter = gfx->finish.counter + 1;
    if (gfx->finish.counter >= 60) {
        if (ModelAnimation_Advance(&gfx->scene.animations[5], 0x2000) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL SwalotPlopFinish_IsSignDone(SwalotPlopGfx *gfx) {
    if (PlazaGameSign_IsDone(gfx->sign) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void SwalotPlopBody_Init(SwalotPlopGame *game, SwalotPlopBody *body) {
    VecFx32 offset;
    body->spinSpeed = 0;
    SwalotPlopBody_SetSpin(body, 0x7FFF);
    SwalotPlopBody_SetTilt(body, 0xE38);
    body->active = FALSE;
    body->mouthOpen = TRUE;
    body->pushRadius = 0xC8000;
    body->pushStrength = 123;
    body->mouthDepth = 0x50000;
    offset = ov91_02261C28;
    body->offset = offset;
    SwalotPlopBody_SetScale(body, 0x9C4);
    body->position = offset;
    body->height = body->heightMin;
}

void SwalotPlopBody_Update(SwalotPlopGame *game, SwalotPlopBody *body) {
    if (game->phase == SWALOT_PLOP_PHASE_FAST) {
        body->mouthOpen = FALSE;
    } else {
        body->mouthOpen = TRUE;
    }
    int (*getTilt)(SwalotPlopBody *, u32, u32) = ov91_02261C58[game->phase];
    int tilt = getTilt(body, game->frame, game->duration);
    SwalotPlopBody_SetTilt(body, tilt);
    body->mouthDepth = SwalotPlopBody_CalcMouthDepth(body, body->tilt);
    body->height = SwalotPlopBody_CalcHeight(body, body->tilt);
    SwalotPlopBody_SetSpin(body, (u16)(body->spin + body->spinSpeed));
}

void SwalotPlopBody_SetActive(SwalotPlopBody *body, u8 active) {
    body->active = active;
}

void SwalotPlopBody_SetTilt(SwalotPlopBody *body, int tilt) {
    body->tilt = tilt;
    MTX_Identity33_(&body->tiltMtx);
    MTX_RotX33_(&body->tiltMtx, FX_SinIdx(tilt), FX_CosIdx(tilt));
}

void SwalotPlopBody_SetSpin(SwalotPlopBody *body, u16 spin) {
    VecFx32 v;
    fx32 dot;
    body->spin = spin;
    MTX_Identity33_(&body->rotation);
    MTX_Identity33_(&body->spinMtx);
    MTX_RotY33_(&body->spinMtx, FX_SinIdx(spin), FX_CosIdx(spin));
    MTX_Concat33(&body->tiltMtx, &body->spinMtx, &body->rotation);
    MTX_MultVec33(&ov91_02261C4C, &body->rotation, &body->normal);
    v.x = 0;
    v.y = 0;
    v.z = -body->mouthDepth;
    MTX_MultVec33(&v, &body->rotation, &body->mouthPos);
    v.x = body->mouthOffsetX + body->offset.x;
    v.y = body->mouthOffsetY;
    v.z = body->offset.z + 0x50000;
    MTX_MultVec33(&v, &body->spinMtx, &v);
    VEC_Add(&v, &body->mouthPos, &body->mouthPos);
    dot = FX_Mul(body->normal.x, body->mouthPos.x) + FX_Mul(body->normal.y, body->mouthPos.y) + FX_Mul(body->normal.z, body->mouthPos.z);
    body->mouthPlane = dot;
    MTX_MultVec33(&body->offset, &body->spinMtx, &body->position);
}

int SwalotPlopBody_CheckMouth(SwalotPlopBody *body, const VecFx32 *position, VecFx32 *move, VecFx32 *push) {
    VecFx32 hit;
    VecFx32 delta;
    VecFx32 normalized;
    fx32 t;
    fx32 distance;
    fx32 strength;
    int result = 0;
    if (body->active == FALSE) {
        return result;
    }
    SwalotPlop_IntersectPlane(position, move, &body->normal, body->mouthPlane, &hit, &t);
    VEC_Subtract(&body->mouthPos, position, &delta);
    distance = VEC_Mag(&delta);
    if (t >= 0 && t <= FX32_ONE) {
        if (distance <= body->mouthDepth) {
            if (body->mouthOpen == TRUE) {
                result = 1;
            } else {
                result = 2;
            }
        } else {
            return 2;
        }
    }
    if (t > 0 && distance < body->pushRadius) {
        strength = FX_Mul(body->pushRadius - distance, body->pushStrength);
        VEC_Normalize(&delta, &normalized);
        push->x = FX_Mul(normalized.x, strength);
        push->y = FX_Mul(normalized.y, strength);
        push->z = FX_Mul(normalized.z, strength);
        if (distance - strength <= body->mouthDepth) {
            if (body->mouthOpen == TRUE) {
                result = 1;
            } else {
                result = 2;
            }
        } else {
            result = 3;
        }
    }
    return result;
}

BOOL SwalotPlopBody_IsInside(SwalotPlopBody *body, const VecFx32 *pos) {
    VecFx32 delta;
    VecFx32 center;
    VecFx32 horizontal;
    fx32 distance;
    s32 narrowing;
    s32 y;
    if (body->active == FALSE) {
        return FALSE;
    }
    if (body->position.y < pos->y && body->position.y + body->height > pos->y) {
        center.x = body->position.x;
        center.z = body->position.z;
        center.y = 0;
        horizontal.x = pos->x;
        horizontal.z = pos->z;
        horizontal.y = 0;
        VEC_Subtract(&center, &horizontal, &delta);
        distance = VEC_Mag(&delta);
        y = (pos->y - body->position.y) >> FX32_SHIFT;
        narrowing = _s32_div_f(y * body->slope, body->height >> FX32_SHIFT);
        if (distance < body->radius - narrowing) {
            return TRUE;
        }
    }
    return FALSE;
}

fx32 SwalotPlopBody_CalcMouthDepth(SwalotPlopBody *body, s32 tilt) {
    s32 degrees = 90;
    s32 t;
    degrees <<= 2;
    t = _s32_div_f(degrees * tilt, 0xFFFF);
    t = _s32_div_f((90 - t) * body->mouthDepthRange, 90);
    t += body->mouthDepthMin;
    return t;
}

fx32 SwalotPlopBody_CalcHeight(SwalotPlopBody *body, s32 tilt) {
    s32 degrees = 90;
    s32 t;
    degrees <<= 2;
    t = _s32_div_f(degrees * tilt, 0xFFFF);
    t = _s32_div_f(t * body->heightRange, 90);
    t += body->heightMin;
    return t;
}

void SwalotPlopBody_SetScale(SwalotPlopBody *body, fx32 scale) {
    s32 topRadius;
    body->scale = scale;
    body->mouthDepthRange = FX_Mul(scale, 30 << FX32_SHIFT);
    body->mouthDepthMin = FX_Mul(scale, 50 << FX32_SHIFT);
    topRadius = FX_Mul(scale, 80 << FX32_SHIFT);
    body->unkB4 = topRadius;
    body->mouthOffsetX = 0;
    body->mouthOffsetY = FX_Mul(scale, 55 << FX32_SHIFT);
    body->heightMin = FX_Mul(scale, 150 << FX32_SHIFT);
    body->heightRange = FX_Mul(scale, 60 << FX32_SHIFT);
    body->topRadius = topRadius;
    body->radius = FX_Mul(scale, 120 << FX32_SHIFT);
    body->slope = body->radius - body->topRadius;
    body->modelScale = FX_Mul(scale, 0x1800);
}

void SwalotPlopBody_SetSpinSpeed(SwalotPlopBody *body, u16 spinSpeed) {
    body->spinSpeed = spinSpeed;
}

void SwalotPlopBody_SetMouthOffset(SwalotPlopBody *body, u32 offset) {
    body->offset.z = offset;
}

int SwalotPlopBody_GetTiltNormal(SwalotPlopBody *body, u32 frame, u32 duration) {
    return 0xE38;
}

int SwalotPlopBody_GetTiltFast(SwalotPlopBody *body, u32 frame, u32 duration) {
    return 0x3FFF;
}

u16 SwalotPlopBody_GetTiltFastest(SwalotPlopBody *body, u32 frame, u32 duration) {
    s32 tilt = (s32)(((frame - 750) % 50) * 0x638E) / 50;
    if (tilt > 0x31C7) {
        tilt = 0x31C7 - tilt % 0x31C7;
    }
    return tilt + 0xE38;
}

void SwalotPlopShot_ToRecord(const SwalotPlopShotParams *params, SwalotPlopShotRecord *record) {
    record->owner = params->owner;
    record->caught = params->caught;
    record->dir[0] = params->direction.x;
    record->dir[1] = params->direction.y;
    record->dir[2] = params->direction.z;
    record->speed = (s64)((s64)params->speed * 0x28F + 0x800) >> 12;
    record->start[0] = params->start.x >> 12;
    record->start[1] = params->start.y >> 12;
    record->start[2] = params->start.z >> 12;
    record->combo = params->combo;
    record->big = params->big;
}

void SwalotPlopShot_FromRecord(const SwalotPlopShotRecord *record, SwalotPlopShotParams *params) {
    params->owner = record->owner;
    params->caught = record->caught;
    params->direction.x = record->dir[0];
    params->direction.y = record->dir[1];
    params->direction.z = record->dir[2];
    params->speed = FX_Div(record->speed, 0x28F);
    params->start.x = record->start[0] << 12;
    params->start.y = record->start[1] << 12;
    params->start.z = record->start[2] << 12;
    params->big = record->big;
}

#pragma force_active on

void SwalotPlopGame_AddShot(SwalotPlopGame *game, const SwalotPlopShotParams *params, u32 state) {
    // some accesses go through offsetof(shots) as in the original code (register allocation)
    int i;
    for (i = 0; i < 0x60; i++) {
        if (game->shots[i].state == SHOT_STATE_FREE) {
            game->shots[i].params = *params;
            ((SwalotPlopShot *)((u8 *)game + offsetof(SwalotPlopGame, shots)))[i].state = state;
            game->shots[i].position = params->start;
            game->shots[i].frames = 0;
            game->shots[i].crossedRim = FALSE;
            SwalotPlopGfx_AddShotView(&game->gfx, &game->shots[i], game->playerCount, game->localIndex);
            return;
        }
    }
    if (state == SHOT_STATE_FLYING) {
        for (i = 0; i < 0x60; i++) {
            if (game->shots[i].state == SHOT_STATE_FALLING || game->shots[i].state == SHOT_STATE_BOUNCED) {
                SwalotPlopGfx_RemoveShotView(&game->gfx, &((SwalotPlopShot *)((u8 *)game + offsetof(SwalotPlopGame, shots)))[i]);
                SwalotPlopShot_Clear(&game->shots[i]);
                game->shots[i].params = *params;
                game->shots[i].state = state;
                game->shots[i].position = params->start;
                game->shots[i].frames = 0;
                game->shots[i].crossedRim = FALSE;
                SwalotPlopGfx_AddShotView(&game->gfx, &((SwalotPlopShot *)((u8 *)game + offsetof(SwalotPlopGame, shots)))[i], game->playerCount, game->localIndex);
                return;
            }
        }
        GF_ASSERT(FALSE);
    }
}

#pragma force_active reset

void SwalotPlopShot_End(SwalotPlopGame *game, SwalotPlopShot *shot, u16 caught, u32 combo) {
    shot->remove = TRUE;
    shot->params.caught = caught;
    shot->params.combo = combo;
}

void SwalotPlopShot_Clear(SwalotPlopShot *shot) {
    memset(shot, 0, sizeof(SwalotPlopShot));
}

void SwalotPlopGame_UpdateShots(SwalotPlopGame *game) {
    // walks the struct shifted by one element per step, as the original code does
    SwalotPlopShot *shot = (SwalotPlopShot *)game;
    int i = 0;
    SwalotPlopGame *walker = game;
    shot = (SwalotPlopShot *)((u8 *)shot + offsetof(SwalotPlopGame, shots));
    for (; i < 0x60; i++, walker = (SwalotPlopGame *)((u8 *)walker + sizeof(SwalotPlopShot)), shot++) {
        u8 state = walker->shots[0].state;
        if ((u8)(state + 255) <= 1) { // flying or falling
            SwalotPlopGame_UpdateFlyingShot(game, shot);
        } else if (state == SHOT_STATE_BOUNCED) {
            SwalotPlopGame_UpdateBouncedShot(shot, game);
        } else if (state == SHOT_STATE_CAUGHT) {
            SwalotPlopGame_UpdateCaughtShot(shot, game);
        }
    }
}

void SwalotPlopGame_UpdateFlyingShot(SwalotPlopGame *game, SwalotPlopShot *shot) {
    VecFx32 move;
    VecFx32 push;
    fx32 oldY;
    int result;
    int state = shot->state;

    if (state != SHOT_STATE_FLYING && state != SHOT_STATE_FALLING) {
        return;
    }
    oldY = shot->position.y;
    SwalotPlopShot_GetMove(shot, &move);
    if (shot->state == SHOT_STATE_FLYING) {
        result = SwalotPlopBody_CheckMouth(&game->body, &shot->position, &move, &push);
    } else {
        result = 0;
    }
    SwalotPlopShot_Move(shot, &move);
    if (result == 3 || result == 1) {
        SwalotPlopShot_Move(shot, &push);
    }
    shot->frames++;
    if (result == 1) {
        game->score += SwalotPlopGame_GetHitPoints(game);
        if (game->score > 99999) {
            game->score = 99999;
        }
        switch (game->combo) {
        case 0:
            PlaySE(SEQ_SE_PL_NOMI02);
            PlaySE(SEQ_SE_PL_POINT1);
            break;
        case 1:
            PlaySE(SEQ_SE_PL_NOMI02);
            PlaySE(SEQ_SE_PL_POINT2);
            break;
        default:
            PlaySE(SEQ_SE_PL_NOMI02);
            PlaySE(SEQ_SE_PL_POINT3);
            break;
        }
        SwalotPlopShot_Catch(shot, game);
        SwalotPlopPopups_Show(game->gfx.popups, game->localIndex, game->combo, game->phase);
        if (game->combo < 99) {
            game->combo++;
        }
        if (game->combo == 3) {
            SwalotPlopCombo_Show(&game->gfx.combo, &game->gfx, game->combo);
        } else {
            SwalotPlopCombo_SetCount(&game->gfx.combo, &game->gfx, game->combo);
        }
        return;
    }
    result = 0;
    if (SwalotPlopBody_IsInside(&game->body, &shot->position) == TRUE) {
        result = 1;
        if (state == SHOT_STATE_FLYING) {
            PlaySE(SEQ_SE_DP_023);
        }
        SwalotPlopShot_Bounce(shot, game, TRUE);
    } else if (shot->position.y <= -0x32000) {
        if (!SwalotPlopShot_IsOnRim(shot)) {
            result = 1;
            if (oldY <= -0x32000) {
                SwalotPlopShot_End(game, shot, 0, game->combo);
            } else {
                SwalotPlopShot_Bounce(shot, game, FALSE);
            }
        } else if (shot->position.y <= 0x400) {
            result = 1;
            SwalotPlopShot_End(game, shot, 0, game->combo);
        }
    } else if (SwalotPlopShot_CheckOutside(shot) == TRUE) {
        result = 1;
        VEC_Subtract(&shot->position, &move, &shot->position);
        SwalotPlopShot_BounceEx(shot, game, TRUE, 0x400, FALSE);
    }
    if (result == 1 && state == SHOT_STATE_FLYING) {
        game->combo = 0;
        SwalotPlopCombo_Hide(&game->gfx.combo);
    }
}

void SwalotPlopGame_RemoveEndedShots(SwalotPlopGame *game) {
    int i = 0;
    SwalotPlopShot *shot = (SwalotPlopShot *)game;
    shot = (SwalotPlopShot *)((u8 *)shot + offsetof(SwalotPlopGame, shots));
    for (; i < 0x60; i++, shot++) {
        SwalotPlopShot_CheckRemove(shot, game);
    }
}

void SwalotPlopGame_DropEndedShots(SwalotPlopGame *game) {
    // walks the struct shifted by one element per step, as the original code does
    SwalotPlopGame *walker;
    int i;
    SwalotPlopShot *shot;
    shot = (SwalotPlopShot *)game;
    i = 0;
    walker = game;
    shot = (SwalotPlopShot *)((u8 *)shot + offsetof(SwalotPlopGame, shots));
    for (; i < 0x60; i++, walker = (SwalotPlopGame *)((u8 *)walker + sizeof(SwalotPlopShot)), shot++) {
        if (walker->shots[0].remove == TRUE) {
            walker->shots[0].state = SHOT_STATE_FALLING;
        }
        SwalotPlopShot_CheckRemove(shot, game);
    }
}

void SwalotPlopShot_CheckRemove(SwalotPlopShot *shot, SwalotPlopGame *game) {
    if (shot->remove != FALSE) {
        if (shot->state == SHOT_STATE_CAUGHT && shot->params.caught == TRUE) {
            shot->state = SHOT_STATE_REPORT;
        } else {
            SwalotPlopShot_Clear(shot);
        }
        SwalotPlopGfx_RemoveShotView(&game->gfx, shot);
    }
}

void SwalotPlopGame_DropFlyingShots(SwalotPlopGame *game) {
    // walks the struct shifted by one element per step, as the original code does
    int i;
    for (i = 0; i < 0x60; i++, game = (SwalotPlopGame *)((u8 *)game + sizeof(SwalotPlopShot))) {
        if (game->shots[0].state == SHOT_STATE_FLYING) {
            game->shots[0].state = SHOT_STATE_FALLING;
        }
    }
}

BOOL SwalotPlopGame_HasActiveShots(SwalotPlopGame *game) {
    // walks the struct shifted by one element per step, as the original code does
    int i;
    for (i = 0; i < 0x60; i++, game = (SwalotPlopGame *)((u8 *)game + sizeof(SwalotPlopShot))) {
        u8 state = game->shots[0].state;
        if (state == SHOT_STATE_FLYING || state == SHOT_STATE_FALLING || state == SHOT_STATE_BOUNCED || state == SHOT_STATE_CAUGHT) {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active on

fx32 SwalotPlopShot_GetSpeed(SwalotPlopShot *shot) {
    fx32 speed = shot->params.speed - (int)(((s64)(shot->frames << 12) * 0x670 + 0x800) >> 12);
    if (speed < 0) {
        speed = 0;
    }
    return speed;
}

void SwalotPlopShot_GetMove(SwalotPlopShot *shot, VecFx32 *move) {
    fx32 speed = SwalotPlopShot_GetSpeed(shot);
    move->x = (int)(((s64)shot->params.direction.x * speed + 0x800) >> 12);
    move->y = (int)(((s64)shot->params.direction.y * speed + 0x800) >> 12);
    move->y += (int)(((s64)(shot->frames << 12) * -0x1334 + 0x800) >> 12);
    move->z = (int)(((s64)shot->params.direction.z * speed + 0x800) >> 12);
}

#pragma force_active reset

void SwalotPlopShot_Bounce(SwalotPlopShot *shot, SwalotPlopGame *game, BOOL bounce) {
    fx32 damping;
    if (bounce == TRUE) {
        damping = 0xD9A;
    } else {
        damping = 0xCCD;
    }
    SwalotPlopShot_BounceEx(shot, game, bounce, damping, TRUE);
}

#pragma force_active on

void SwalotPlopShot_BounceEx(SwalotPlopShot *shot, SwalotPlopGame *game, BOOL reverse, fx32 damping, BOOL jitter) {
    VecFx32 move;
    if (reverse) {
        shot->params.direction.x = -shot->params.direction.x;
        shot->params.direction.z = -shot->params.direction.z;
        if (jitter) {
            u32 random = MTRandom();
            int z = shot->params.direction.z;
            int absZ = z < 0 ? -z : z;
            int x = shot->params.direction.x;
            int absX = x < 0 ? -x : x;
            if (absX <= absZ) {
                int value = (fx32)(((s64)z * FX32_ONE + 2048) >> 12);
                if ((random & 1) == 1) {
                    shot->params.direction.x = -value;
                } else {
                    shot->params.direction.x = value;
                }
            } else {
                int value = (fx32)(((s64)x * FX32_ONE + 2048) >> 12);
                if ((random & 1) == 1) {
                    shot->params.direction.z = -value;
                } else {
                    shot->params.direction.z = value;
                }
            }
        }
    }
    shot->params.speed = (fx32)(((s64)shot->params.speed * damping + 2048) >> 12);
    shot->frames = 0;
    shot->state = SHOT_STATE_BOUNCED;
    SwalotPlopShot_GetMove(shot, &move);
    SwalotPlopShot_Move(shot, &move);
    shot->frames++;
}

void SwalotPlopGame_UpdateBouncedShot(SwalotPlopShot *shot, SwalotPlopGame *game) {
    BOOL done = FALSE;
    fx32 oldY;
    VecFx32 move;
    if (shot->state == SHOT_STATE_BOUNCED) {
        oldY = shot->position.y;
        SwalotPlopShot_GetMove(shot, &move);
        SwalotPlopShot_Move(shot, &move);
        shot->frames++;
        if (SwalotPlopBody_IsInside(&game->body, &shot->position) == TRUE) {
            SwalotPlopShot_Bounce(shot, game, TRUE);
        } else if (shot->position.y <= -0x32000) {
            if (SwalotPlopShot_IsOnRim(shot) == FALSE) {
                if (oldY <= -0x32000) {
                    done = TRUE;
                } else {
                    SwalotPlopShot_Bounce(shot, game, FALSE);
                }
            } else if (shot->position.y <= -0x64000) {
                done = TRUE;
            }
            if (shot->params.speed <= 0x2000) {
                done = TRUE;
            }
        } else if (SwalotPlopShot_CheckOutside(shot) == TRUE) {
            VEC_Subtract(&shot->position, &move, &shot->position);
            SwalotPlopShot_BounceEx(shot, game, TRUE, 0x400, FALSE);
        }
        if (shot->params.speed <= FX32_ONE) {
            done = TRUE;
        }
        if (done == TRUE) {
            SwalotPlopShot_End(game, shot, 0, game->combo);
        }
    }
}

#pragma force_active reset

BOOL SwalotPlopShot_IsOnRim(SwalotPlopShot *shot) {
    fx32 distance = shot->distance;
    return distance >= 0xBC000 && distance <= 0xF0000;
}

BOOL SwalotPlopShot_IsInPool(SwalotPlopShot *shot) {
    return shot->distance <= 0x78000;
}

BOOL SwalotPlopShot_CheckOutside(SwalotPlopShot *shot) {
    fx32 distance = shot->distance;
    if (distance >= 0xF0000) {
        if (shot->crossedRim == TRUE) {
            return TRUE;
        }
    } else if (shot->crossedRim == FALSE) {
        shot->crossedRim = TRUE;
    }
    return FALSE;
}

void SwalotPlopShot_Catch(SwalotPlopShot *shot, SwalotPlopGame *game) {
    shot->state = SHOT_STATE_CAUGHT;
    shot->timer = 8;
    shot->combo = game->combo;
}

void SwalotPlopGame_UpdateCaughtShot(SwalotPlopShot *shot, SwalotPlopGame *game) {
    VecFx32 move;
    VecFx32 push;
    int result;

    SwalotPlopShot_GetMove(shot, &move);
    result = SwalotPlopBody_CheckMouth(&game->body, &shot->position, &move, &push);
    SwalotPlopShot_Move(shot, &move);
    if (result == 3 || result == 1) {
        SwalotPlopShot_Move(shot, &push);
    }
    shot->frames++;
    shot->timer--;
    if (shot->timer <= 0) {
        SwalotPlopShot_End(game, shot, 1, shot->combo);
    }
}

void SwalotPlopShot_Move(SwalotPlopShot *shot, const VecFx32 *move) {
    VecFx32 horizontal;

    VEC_Add(&shot->position, move, &shot->position);
    horizontal.y = 0;
    horizontal.x = shot->position.x;
    horizontal.z = shot->position.z;
    shot->distance = VEC_Mag(&horizontal);
}

void SwalotPlopGame_HandleTouch(SwalotPlopGame *game) {
    if (gSystem.touchHeld == 0) {
        SwalotPlopGame_Throw(game);
    } else {
        SwalotPlopGame_TrackFlick(game);
    }
}

#pragma force_active on

void SwalotPlopGame_Throw(SwalotPlopGame *game) {
    SwalotPlopPoint last;
    SwalotPlopPoint first;
    SwalotPlopShotParams params;
    BOOL tooSlow = FALSE;
    int count;
    fx32 speed;
    if (!game->flick.active) {
        game->releasedFrames++;
        return;
    }
    if (!SwalotPlopFlickQueue_PeekFirst(&game->flick, &last)) {
        GF_AssertFail();
    }
    if (!SwalotPlopFlickQueue_PeekLast(&game->flick, &first)) {
        GF_AssertFail();
    }
    count = SwalotPlopFlickQueue_Count(&game->flick);
    if (count >= 2) {
        params.owner = game->localIndex;
        params.caught = 0;
        params.direction.x = (s16)((last.x - first.x) / count) << 12;
        params.direction.z = (s16)((last.y - first.y) / count) << 12;
        params.direction.y = 0;
        speed = VEC_Mag(&params.direction);
        if (speed < 0x8000) {
            if (speed) {
                VEC_Normalize(&params.direction, &params.direction);
                params.direction.x = (s64)(params.direction.x) * 0x8000 + 0x800 >> 12;
                params.direction.z = (s64)(params.direction.z) * 0x8000 + 0x800 >> 12;
            } else {
                tooSlow = TRUE;
            }
        } else if (speed > 0x1E200) {
            VEC_Normalize(&params.direction, &params.direction);
            params.direction.x = ((s64)params.direction.x * 0x1E200 + 0x800) >> 12;
            params.direction.z = ((s64)params.direction.z * 0x1E200 + 0x800) >> 12;
        }
        if (!tooSlow) {
            params.direction.y = 0x26000;
            params.speed = VEC_Mag(&params.direction);
            params.speed = ((s64)params.speed * 0x99A + 0x800) >> 12;
            VEC_Normalize(&params.direction, &params.direction);
            SwalotPlop_TouchToWorld(first.x, first.y, &params.start);
            params.start.y = 0;
            SwalotPlop_RotateToSeat(&params.start, &params.start, params.owner, game->playerCount);
            SwalotPlop_RotateToSeat(&params.direction, &params.direction, params.owner, game->playerCount);
            if (game->combo >= 5) {
                params.big = TRUE;
            } else {
                params.big = FALSE;
            }
            SwalotPlopGame_AddShot(game, &params, SHOT_STATE_FLYING);
            PlaySE(SEQ_SE_DP_FW104);
        }
    } else {
        tooSlow = TRUE;
    }
    if (tooSlow == TRUE) {
        SwalotPlopParticles_Spawn(game->gfx.particles, first.x, first.y);
    }
    SwalotPlopGame_EndFlick(game);
    game->releasedFrames = 0;
}

void SwalotPlopGame_TrackFlick(SwalotPlopGame *game) {
    SwalotPlopFlick point;
    if (game->flick.active == FALSE && SwalotPlopGame_CanStartFlick(game) == TRUE) {
        game->flick.active = TRUE;
        SwalotPlopCursor_Show(&game->gfx.cursor);
        SwalotPlopBgFade_Step(&game->gfx.bgFade);
    }
    if (game->flick.active == TRUE) {
        u16 count = game->touch.count;
        if (count == 1) {
            point.x = game->touch.samples[0].x;
            point.y = game->touch.samples[0].y;
            SwalotPlopFlickQueue_Push(&game->flick, point);
        } else if (count >= 2) {
            point.x = game->touch.samples[0].x;
            point.y = game->touch.samples[0].y;
            SwalotPlopFlickQueue_Push(&game->flick, point);
            count = game->touch.count;
            point.x = game->touch.samples[count - 1].x;
            point.y = game->touch.samples[count - 1].y;
            SwalotPlopFlickQueue_Push(&game->flick, point);
        } else {
            point.x = gSystem.touchX;
            point.y = gSystem.touchY;
            SwalotPlopFlickQueue_Push(&game->flick, point);
        }
    }
}

#pragma force_active reset

BOOL SwalotPlopGame_CanStartFlick(SwalotPlopGame *game) {
    s32 x;
    s32 y;

    if (game->releasedFrames < 2) {
        return FALSE;
    }
    x = 128 - gSystem.touchX;
    y = -96 - gSystem.touchY;
    if ((FX_Sqrt((x * x + y * y) << FX32_SHIFT) >> FX32_SHIFT) <= 286) {
        return TRUE;
    }
    return FALSE;
}

void SwalotPlopFlickQueue_Clear(SwalotPlopFlickQueue *queue) {
    memset(queue->points, 0, sizeof(queue->points));
    queue->read = 0;
    queue->write = 0;
}

#pragma force_active on

void SwalotPlopFlickQueue_Push(SwalotPlopFlickQueue *queue, SwalotPlopFlick point) {
    SwalotPlopFlick dropped;
    if ((queue->write + 1) % 8 == queue->read) {
        SwalotPlopFlickQueue_Pop(queue, &dropped);
    }
    queue->points[queue->write] = point;
    queue->write = (queue->write + 1) % 8;
}

BOOL SwalotPlopFlickQueue_Pop(SwalotPlopFlickQueue *queue, SwalotPlopFlick *point) {
    if (queue->write == queue->read) {
        return FALSE;
    }
    *point = queue->points[queue->read];
    queue->read = (queue->read + 1) % 8;
    return TRUE;
}

#pragma force_active reset

BOOL SwalotPlopFlickQueue_PeekFirst(SwalotPlopFlickQueue *queue, SwalotPlopPoint *point) {
    u16 read = queue->read;
    if (queue->write == read) {
        return FALSE;
    }
    point->x = queue->points[read].x;
    point->y = queue->points[read].y;
    return TRUE;
}

BOOL SwalotPlopFlickQueue_PeekLast(SwalotPlopFlickQueue *queue, SwalotPlopPoint *point) {
    int last;
    if (queue->write == queue->read) {
        return FALSE;
    }
    last = queue->write - 1;
    if (last < 0) {
        last += 8;
    }
    point->x = queue->points[last].x;
    point->y = queue->points[last].y;
    return TRUE;
}

u32 SwalotPlopFlickQueue_Count(SwalotPlopFlickQueue *queue) {
    if (queue->read > queue->write) {
        return 8 - queue->read + queue->write;
    }
    return queue->write - queue->read;
}

void SwalotPlopGame_EndFlick(SwalotPlopGame *game) {
    game->flick.active = FALSE;
    SwalotPlopFlickQueue_Clear(&game->flick);
    SwalotPlopCursor_Hide(&game->gfx.cursor);
}

void SwalotPlopLevelBlend_Init(SwalotPlopLevelBlend *blend, SwalotPlopShared *shared, const u8 *options) {
    blend->step = 16;
    SwalotPlop_GetLevel(shared, options, 0, (SwalotPlopLevel *)blend);
}

void SwalotPlopLevelBlend_Start(SwalotPlopLevelBlend *blend, SwalotPlopShared *shared, const u8 *options, int from, u16 to) {
    SwalotPlopLevel start;
    SwalotPlopLevel end;
    s32 forward;
    s32 wrapped;
    SwalotPlop_GetLevel(shared, options, from, &start);
    SwalotPlop_GetLevel(shared, options, to, &end);
    *(SwalotPlopLevel *)blend->start = start;
    *(SwalotPlopLevel *)blend->current = start;
    blend->delta[0] = end.hitCount - start.hitCount;
    blend->delta[2] = end.cameraHeight - start.cameraHeight;
    blend->delta[4] = end.mouthOffset - start.mouthOffset;
    blend->delta[5] = end.scale - start.scale;
    wrapped = end.spinSpeed + 0xFFFF - start.spinSpeed;
    forward = end.spinSpeed - start.spinSpeed;
    if (forward < wrapped) {
        blend->delta[1] = forward;
    } else {
        blend->delta[1] = wrapped;
    }
    wrapped = end.cameraPitch + 0xFFFF - start.cameraPitch;
    forward = end.cameraPitch - start.cameraPitch;
    if (forward < wrapped) {
        blend->delta[3] = forward;
    } else {
        blend->delta[3] = wrapped;
    }
    blend->step = 0;
}

BOOL SwalotPlopLevelBlend_Update(SwalotPlopLevelBlend *blend) {
    s32 step = blend->step;
    if (step >= 16) {
        return TRUE;
    }
    step++;
    blend->step = step;
    blend->current[0] = blend->start[0] + blend->delta[0] * step / 16;
    blend->current[1] = blend->start[1] + blend->delta[1] * blend->step / 16;
    blend->current[2] = blend->start[2] + blend->delta[2] * blend->step / 16;
    blend->current[3] = blend->start[3] + blend->delta[3] * blend->step / 16;
    blend->current[4] = blend->start[4] + blend->delta[4] * blend->step / 16;
    blend->current[5] = blend->start[5] + blend->delta[5] * blend->step / 16;
    return FALSE;
}

void SwalotPlopGame_ApplyLevel(SwalotPlopGame *game, SwalotPlopLevelBlend *blend) {
    SwalotPlopGame_ApplyLevelParams(game, blend);
}

void SwalotPlopGame_ApplyLevelParams(SwalotPlopGame *game, SwalotPlopLevelBlend *blend) {
    SwalotPlopLevel *level = (SwalotPlopLevel *)blend->current;
    SwalotPlopBody_SetSpinSpeed(&game->body, (u16)level->spinSpeed);
    SwalotPlopGfx_SetCameraHeight(&game->gfx, level->cameraHeight);
    SwalotPlopGfx_SetCameraPitch(&game->gfx, (u16)level->cameraPitch);
    SwalotPlopBody_SetMouthOffset(&game->body, level->mouthOffset);
    SwalotPlopBody_SetScale(&game->body, level->scale);
}

void SwalotPlop_GetLevel(SwalotPlopShared *shared, const u8 *options, int index, SwalotPlopLevel *level) {
    int mirrored;
    if ((u32)index >= shared->levelTable.count) {
        GF_AssertFail();
    }
    *level = shared->levelTable.levels[index];
    level->spinSpeed = (u16)level->spinSpeed;
    level->cameraPitch = (u16)level->cameraPitch;
    if (options[0] != 0) {
        mirrored = 6 - (index + 1);
        if (mirrored < 0) {
            mirrored = 0;
        }
        level->scale = shared->levelTable.levels[mirrored].scale;
    }
    if (options[1] != 0) {
        level->spinSpeed = -level->spinSpeed;
    }
}

#pragma force_active on

void SwalotPlopDemo_Init(SwalotPlopDemo *demo, SwalotPlopGfx *gfx, enum HeapID heapId) {
    SimpleSpriteTemplate template;
    NARC *narc;
    memset(demo, 0, sizeof(SwalotPlopDemo));
    narc = NARC_New((NarcId)200, heapId);
    demo->res[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, 21, FALSE, 120, NNS_G2D_VRAM_TYPE_2DSUB, heapId);
    demo->res[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], narc, 20, FALSE, 120, NNS_G2D_VRAM_TYPE_2DSUB, 2, heapId);
    demo->res[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, 22, FALSE, 120, GF_GFX_RES_TYPE_CELL, heapId);
    demo->res[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, 23, FALSE, 120, GF_GFX_RES_TYPE_ANIM, heapId);
    if (!SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(demo->res[GF_GFX_RES_TYPE_CHAR])) {
        GF_AssertFail();
    }
    if (!SpriteTransfer_CreatePlttTransferTask(demo->res[GF_GFX_RES_TYPE_PLTT])) {
        GF_AssertFail();
    }
    sub_0200A740(demo->res[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(demo->res[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&demo->header, 120, 120, 120, 120, -1, -1, FALSE, 0, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = gfx->spriteList;
    template.header = &demo->header;
    template.position.x = 128 * FX32_ONE;
    template.position.y = 0x278000;
    template.priority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = heapId;
    demo->sprite = Sprite_Create(&template);
    Sprite_SetDrawFlag(demo->sprite, FALSE);
    NARC_Delete(narc);
}

#pragma force_active reset

void SwalotPlopDemo_Free(SwalotPlopDemo *demo, SwalotPlopGfx *gfx) {
    Sprite_Delete(demo->sprite);
    SpriteTransfer_DeleteCharTransferTask(demo->res[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(demo->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CHAR], demo->res[GF_GFX_RES_TYPE_CHAR]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_PLTT], demo->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CELL], demo->res[GF_GFX_RES_TYPE_CELL]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_ANIM], demo->res[GF_GFX_RES_TYPE_ANIM]);
    memset(demo, 0, sizeof(SwalotPlopDemo));
}

void SwalotPlopDemo_Start(SwalotPlopDemo *demo) {
    demo->active = TRUE;
    demo->state = 0;
    demo->count = 0;
    Sprite_SetDrawFlag(demo->sprite, TRUE);
}

#pragma force_active on

BOOL SwalotPlopDemo_Update(SwalotPlopDemo *demo, SwalotPlopGame *game) {
    switch (demo->state) {
    case 0:
        Sprite_SetAnimationFrame(demo->sprite, 0);
        SwalotPlopCursor_Show(&game->gfx.cursor);
        SwalotPlopCursor_SetPos(&game->gfx.cursor, 128, 177);
        demo->thrown = FALSE;
        demo->state++;
        break;
    case 1: {
        int frame;
        Sprite_UpdateAnim(demo->sprite, 2 * FX32_ONE);
        frame = Sprite_GetAnimationFrame(demo->sprite);
        switch (frame) {
        case 10:
            if (demo->thrown == FALSE) {
                SwalotPlopGame_ThrowDemoShot(game, 128, 96, 128, 64, SHOT_STATE_FALLING);
                PlaySE(SEQ_SE_DP_FW104);
                demo->thrown = TRUE;
            }
            break;
        case 11:
            demo->count++;
            if (demo->count >= 2) {
                demo->state = 3;
            } else {
                demo->state = 2;
                demo->timer = 16;
            }
            SwalotPlopCursor_Hide(&game->gfx.cursor);
            break;
        }
        SwalotPlopCursor_SetPos(&game->gfx.cursor, 128, ov91_02261C84[frame]);
        break;
    }
    case 2:
        demo->timer--;
        if (demo->timer <= 0) {
            demo->state = 0;
        }
        break;
    case 3:
        if (SwalotPlopGame_HasActiveShots(game) == FALSE) {
            Sprite_SetDrawFlag(demo->sprite, FALSE);
            return TRUE;
        }
        break;
    }
    SwalotPlopGame_UpdateShots(game);
    SwalotPlopGame_RemoveEndedShots(game);
    return FALSE;
}

#pragma force_active reset

void SwalotPlopScoreReports_Init(SwalotPlopScoreReports *reports) {
    u8 *bytes = (u8 *)reports;
    u32 size = sizeof(SwalotPlopScoreReports);
    do {
        *bytes++ = 0;
    } while (--size != 0);
    reports->leader = 0xFFFF;
}

void SwalotPlopScoreReports_Set(SwalotPlopScoreReports *reports, u32 score, u32 index) {
    reports->scores[index] = score;
    reports->mask |= 1 << index;
    if (score != 0) {
        if (reports->leader != 0xFFFF) {
            if (reports->scores[reports->leader] >= score) {
                return;
            }
            reports->leader = index;
            return;
        }
        reports->leader = index;
    }
}

void SwalotPlopScoreReports_Reset(SwalotPlopScoreReports *reports) {
    reports->mask = 0;
    reports->leader = 0xFFFF;
}

u32 SwalotPlopScoreReports_GetLeader(SwalotPlopScoreReports *reports) {
    return reports->leader;
}

BOOL SwalotPlopScoreReports_HasAll(SwalotPlopScoreReports *reports, u32 playerCount) {
    int i;
    int count = 0;
    for (i = 0; i < 4; i++) {
        if (reports->mask & (1 << i)) {
            count++;
        }
    }
    return playerCount <= count;
}

void SwalotPlopGfx_SetBanks(void) {
    GraphicsBanks banks;
    banks = ov91_02261D84;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on

void SwalotPlopGfx_Setup3D(void) {
    volatile u16 *reg = &reg_G2_BG0CNT;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    *reg = (*reg & ~3) | 2;
    reg = &reg_G3X_DISP3DCNT;
    *reg &= ~0x3002;
    *reg = (*reg & ~0x3000) | 16;
    *reg &= 0xCFFB;
    *reg = (*reg & ~0x3000) | 8;
    *reg = (*reg & ~0x3000) | 32;
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0x6B5A, 0, 0x7FFF, 63, FALSE);
    reg_G3_VIEWPORT = 0xBFFF0000;
}

void SwalotPlopGfx_Init3D(SwalotPlopGfx *gfx, int playerCount, int localIndex, enum HeapID heapId) {
    gfx->vramMan = GF_3DVramMan_Create(heapId, 0, 1, 0, 4, SwalotPlopGfx_Setup3D);
    SwalotPlopGfx_InitCamera(gfx, playerCount, localIndex, heapId);
}

#pragma force_active reset

void SwalotPlopGfx_Free3D(SwalotPlopGfx *gfx) {
    SwalotPlopGfx_FreeCamera(gfx);
    GF_3DVramMan_Delete(gfx->vramMan);
}

void SwalotPlopGame_InitGfx(SwalotPlopGame *game, enum HeapID heapId) {
    HeapExp_FndInitAllocator(&game->allocator, heapId, 0x20);
    SwalotPlopGfx_SetBanks();
    SwalotPlopGfx_Init3D(&game->gfx, game->playerCount, game->localIndex, heapId);
    SwalotPlopGfx_InitBgs(&game->gfx.bgConfig, heapId);
    SwalotPlopGfx_InitSprites(&game->gfx, heapId);
    SwalotPlopGfx_InitText(&game->gfx, heapId);
    NARC *narc = NARC_New((NarcId)0xBD, heapId);
    SwalotPlopGfx_LoadSpriteRes(&game->gfx, narc, heapId);
    SwalotPlopStage_Init(&game->gfx.stage, &game->gfx, narc, game->playerCount, game->localIndex, heapId, &game->allocator);
    SwalotPlopScene_Init(&game->gfx.scene, narc, heapId, &game->allocator);
    SwalotPlopGfx_InitShotViews(&game->gfx, narc, heapId);
    SwalotPlopCursor_Init(&game->gfx, &game->gfx.cursor, game->localIndex, heapId);
    SwalotPlopParticles_Init(&game->gfx, game->gfx.particles, game->localIndex, heapId);
    SwalotPlopGfx_LoadSubBgs(&game->gfx, narc, game->localIndex, heapId);
    SwalotPlopIntro_Init(&game->gfx.intro, &game->gfx, game->shared, game->playerCount, game->localIndex, narc, heapId);
    SwalotPlopFinish_Init(&game->gfx, game->playerCount, game->localIndex, heapId);
    SwalotPlopPopups_Init(game->gfx.popups, &game->gfx, game->playerCount, game->localIndex, heapId);
    SwalotPlopBgFade_Init(&game->gfx.bgFade, (u8)game->localIndex);
    SwalotPlopCombo_Init(&game->gfx.combo, &game->gfx, narc, heapId);
    NARC_Delete(narc);
    game->gfx.sign = PlazaGameSign_New(game->gfx.spriteList, heapId);
}

void SwalotPlopGame_FreeGfx(SwalotPlopGame *game) {
    SwalotPlopCombo_Free(&game->gfx.combo, &game->gfx);
    SwalotPlopPopups_Free(game->gfx.popups, &game->gfx);
    ov91_0225DBE0(&game->gfx, game->playerCount);
    SwalotPlopIntro_Free(&game->gfx.intro, &game->gfx);
    ov91_0225FEFC(&game->gfx);
    SwalotPlopParticles_Free(&game->gfx, game->gfx.particles);
    SwalotPlopCursor_Free(&game->gfx.cursor);
    SwalotPlopGfx_FreeShotViews(&game->gfx);
    SwalotPlopScene_Free(&game->gfx.scene, &game->allocator);
    SwalotPlopStage_Free(&game->gfx.stage, &game->gfx, &game->allocator);
    SwalotPlopGfx_FreeSpriteRes(&game->gfx);
    PlazaGameSign_Free(game->gfx.sign);
    SwalotPlopGfx_FreeBgs(&game->gfx);
    SwalotPlopGfx_FreeSprites(&game->gfx);
    SwalotPlopGfx_FreeText(&game->gfx);
    SwalotPlopGfx_Free3D(&game->gfx);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
}

void SwalotPlopGame_UpdateGfx(SwalotPlopGame *game, SwalotPlopGfx *gfx) {
    SwalotPlopScene_Update(&gfx->scene, &game->body, game->phase);
    SwalotPlopScene_Animate(&gfx->scene, &game->body, game->phase);
    SwalotPlopCursor_Update(&gfx->cursor);
    SwalotPlopGame_Draw(game, gfx);
    SwalotPlopBgFade_Update(&gfx->bgFade, gfx);
    SwalotPlopCombo_Update(&gfx->combo);
}

void SwalotPlopGame_Draw(SwalotPlopGame *game, SwalotPlopGfx *gfx) {
    u32 pop0;
    u32 pop1;
    u32 pop2;
    SwalotPlopParticles_Update(gfx->particles);
    SwalotPlopPopups_Update(gfx->popups);
    Thunk_G3X_Reset();
    SwalotPlopGfx_ApplyCamera(gfx);
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, -FX32_ONE, 0);
    NNS_G3dGlbLightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    SwalotPlopStage_Draw(&gfx->stage);
    pop0 = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &pop0, 1);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    SwalotPlopScene_Draw(&gfx->scene, &game->body);
    pop1 = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &pop1, 1);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    SwalotPlopGfx_DrawShotViews(gfx, game->playerCount);
    pop2 = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &pop2, 1);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    SpriteList_RenderAndAnimateSprites(gfx->spriteList);
}

void SwalotPlopGfx_InitBgs(BgConfig **bgConfig, enum HeapID heapId) {
    *bgConfig = BgConfig_Alloc(heapId);
    GraphicsModes modes = ov91_02261C74;
    SetBothScreensModesAndDisable(&modes);
    BgTemplate main1 = ov91_02261C9C;
    InitBgFromTemplate(*bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*bgConfig, GF_BG_LYR_MAIN_1);
    BgTemplate main2 = ov91_02261CB8;
    InitBgFromTemplate(*bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*bgConfig, GF_BG_LYR_MAIN_2);
    BgTemplate sub0 = ov91_02261CD4;
    InitBgFromTemplate(*bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*bgConfig, GF_BG_LYR_SUB_0);
    BgTemplate sub1 = ov91_02261CF0;
    InitBgFromTemplate(*bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*bgConfig, GF_BG_LYR_SUB_1);
    BgTemplate sub2 = ov91_02261D0C;
    InitBgFromTemplate(*bgConfig, GF_BG_LYR_SUB_2, &sub2, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*bgConfig, GF_BG_LYR_SUB_2);
    BgTemplate sub3 = ov91_02261D28;
    InitBgFromTemplate(*bgConfig, GF_BG_LYR_SUB_3, &sub3, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_5_OFFSET, heapId);
    LoadUserFrameGfx1(*bgConfig, GF_BG_LYR_MAIN_1, 1, 4, 0, heapId);
}

void SwalotPlopGfx_FreeBgs(SwalotPlopGfx *gfx) {
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(gfx->bgConfig);
}

void SwalotPlopGfx_InitSprites(SwalotPlopGfx *gfx, enum HeapID heapId) {
    int i;
    SwalotPlopGfx *walker;
    ObjCharTransferTemplate template;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapId);
    template = ov91_02261C64;
    template.heapID = heapId;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K);
    ObjPlttTransfer_Init(0x10, heapId);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    gfx->spriteList = G2dRenderer_Init(0x80, &gfx->renderer, heapId);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, 2 << 20);
    i = 0;
    // resMan[i] is addressed as the struct shifted by 4 * i
    walker = gfx;
    while (i < 4) {
        walker->resMan[0] = Create2DGfxResObjMan(0x10, (GfGfxResType)i, heapId);
        i++;
        walker = (SwalotPlopGfx *)((u8 *)walker + 4);
    }
    gfx->fontSystem = FontSystem_NewInit(8, heapId);
}

void SwalotPlopGfx_FreeSprites(SwalotPlopGfx *gfx) {
    // walks the struct shifted by one element per step, as the original code does
    int i;
    SwalotPlopGfx *walker;
    FontSystem_Delete(gfx->fontSystem);
    SpriteList_Delete(gfx->spriteList);
    walker = gfx;
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(walker->resMan[0]);
        walker = (SwalotPlopGfx *)((u8 *)walker + 4);
    }
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void SwalotPlopGfx_LoadSpriteRes(SwalotPlopGfx *gfx, NARC *narc, enum HeapID heapId) {
    gfx->res[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, 2, FALSE, 200, NNS_G2D_VRAM_TYPE_2DMAIN, heapId);
    gfx->res[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], narc, 3, FALSE, 200, NNS_G2D_VRAM_TYPE_2DMAIN, 8, heapId);
    gfx->res[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, 1, FALSE, 200, GF_GFX_RES_TYPE_CELL, heapId);
    gfx->res[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, 0, FALSE, 200, GF_GFX_RES_TYPE_ANIM, heapId);
    if (!SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(gfx->res[GF_GFX_RES_TYPE_CHAR])) {
        GF_AssertFail();
    }
    if (!SpriteTransfer_CreatePlttTransferTask(gfx->res[GF_GFX_RES_TYPE_PLTT])) {
        GF_AssertFail();
    }
    sub_0200A740(gfx->res[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(gfx->res[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&gfx->header, 200, 200, 200, 200, -1, -1, FALSE, 0, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

void SwalotPlopGfx_FreeSpriteRes(SwalotPlopGfx *gfx) {
    SpriteTransfer_DeleteCharTransferTask(gfx->res[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(gfx->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->res[GF_GFX_RES_TYPE_CHAR]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->res[GF_GFX_RES_TYPE_CELL]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_ANIM], gfx->res[GF_GFX_RES_TYPE_ANIM]);
}

void SwalotPlopGfx_InitText(SwalotPlopGfx *gfx, enum HeapID heapId) {
    gfx->msgFormat = MessageFormat_New(heapId);
    gfx->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 230, heapId);
    gfx->string1 = String_New(0x80, heapId);
    gfx->string2 = String_New(0x80, heapId);
}

void SwalotPlopGfx_FreeText(SwalotPlopGfx *gfx) {
    String_Delete(gfx->string2);
    String_Delete(gfx->string1);
    DestroyMsgData(gfx->msgData);
    MessageFormat_Delete(gfx->msgFormat);
}

void SwalotPlopGfx_InitCamera(SwalotPlopGfx *gfx, int playerCount, int localIndex, enum HeapID heapId) {
    CameraAngle angle;
    VecFx32 up;
    gfx->camera = Camera_New(heapId);
    gfx->cameraTarget.x = 0;
    gfx->cameraTarget.y = 0;
    gfx->cameraTarget.z = 0;
    angle.x = 0xF112;
    angle.y = ov91_02261D5C[playerCount][localIndex];
    angle.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&gfx->cameraTarget, 0x1D9000, &angle, 0xFA4, 0, TRUE, gfx->camera);
    up.x = 0;
    up.y = FX32_ONE;
    up.z = 0;
    Camera_SetLookAtCamUp(&up, gfx->camera);
    Camera_SetStaticPtr(gfx->camera);
    Camera_SetPerspectiveClippingPlane(0xC8000, 0x3E8000, gfx->camera);
}

void SwalotPlopGfx_FreeCamera(SwalotPlopGfx *gfx) {
    Camera_Delete(gfx->camera);
}

void SwalotPlopGfx_ApplyCamera(SwalotPlopGfx *gfx) {
    Camera_PushLookAtToNNSGlb();
}

#pragma force_active on

void SwalotPlopGfx_LoadSubBgs(SwalotPlopGfx *gfx, NARC *narc, int localIndex, enum HeapID heapId) {
    int palette;
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 12, gfx->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 14, gfx->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 15, gfx->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 16, gfx->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, gfx->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 13, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 384, heapId);
    BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_SUB_3, 0, 0, 32, 32, ov91_02261BEC[localIndex]);
    palette = localIndex * 2;
    BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 32, (u8)(palette + 2));
    palette = localIndex * 2 + 1;
    BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 32, 32, (u8)palette);
    BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 32, 32, (u8)palette);
    BgCommitTilemapBufferToVram(gfx->bgConfig, GF_BG_LYR_SUB_0);
    BgCommitTilemapBufferToVram(gfx->bgConfig, GF_BG_LYR_SUB_1);
    BgCommitTilemapBufferToVram(gfx->bgConfig, GF_BG_LYR_SUB_2);
    BgCommitTilemapBufferToVram(gfx->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active reset

void ov91_0225FEFC(SwalotPlopGfx *gfx) {
}

static inline const NNSG3dResName *GetResNameByIdx(const NNSG3dResDict *dict, u32 idx) {
    if (dict != NULL && idx < dict->numEntry) {
        const NNSG3dResDictEntryHeader *header = (const NNSG3dResDictEntryHeader *)((const u8 *)dict + dict->ofsEntry);
        return (const NNSG3dResName *)((const u8 *)header + header->ofsName + idx * sizeof(NNSG3dResName));
    }
    return NULL;
}

#pragma force_active on

void SwalotPlopScene_Init(SwalotPlopScene *scene, NARC *narc, enum HeapID heapId, NNSFndAllocator *allocator) {
    int i;
    int j;
    int node;
    BOOL missing;
    NNSG3dResMdl *model;
    const NNSG3dResDict *dict;
    int id;
    u8 modelId;
    ModelAnimation *special;

    memset(scene, 0, sizeof(SwalotPlopScene));
    for (i = 0; i < 3; i++) {
        ModelResource_LoadFromNarc(&scene->resources[i], narc, ov91_02261C04[i], heapId);
        ModelInstance_Init(&scene->models[i], &scene->resources[i]);
        ModelInstance_SetVisible(&scene->models[i], FALSE);
        ModelInstance_SetPosition(&scene->models[i], 0, -0x122000, 0);
        ModelInstance_SetScale(&scene->models[i], 0x1800, 0x1800, 0x1800);
    }
    for (i = 0; i < 6; i++) {
        ModelAnimation_LoadFromNarc(&scene->animations[i], &scene->resources[ov91_0226274C[i]], narc, ov91_02261C1C[i], heapId, allocator);
        ModelAnimation_SetFrame(&scene->animations[i], 0);
        node = 0;
        model = scene->resources[ov91_0226274C[i]].model;
        dict = model != NULL ? &model->nodeInfo.dict : NULL;
        missing = dict == NULL;
        while ((!missing ? GetResNameByIdx(dict, node) : NULL) != NULL) {
            NNS_G3dAnmObjDisableID(scene->animations[i].object, node);
            node++;
        }
        for (j = 0; j < 15; j++) {
            id = dict != NULL ? NNS_G3dGetResDictIdxByName(dict, &ov91_022621AC[i][j]) : -1;
            if (id != -1) {
                NNS_G3dAnmObjEnableID(scene->animations[i].object, id);
            }
        }
    }
    modelId = _02261BE8[0];
    special = &scene->special;
    ModelAnimation_LoadFromNarc(special, &scene->resources[modelId], narc, 32, heapId, allocator);
    ModelInstance_AddAnimation(&scene->models[modelId], special);
    scene->leader = 4;
    scene->bonus = 0;
    SwalotPlopScene_SetBaseAnimation(scene, 0);
    ModelInstance_AddAnimation(&scene->models[1], &scene->animations[4]);
    ModelAnimation_SetFrame(&scene->animations[4], 0);
    ModelInstance_SetVisible(&scene->models[1], TRUE);
    scene->current = 4;
}

#pragma force_active reset

void SwalotPlopScene_Free(SwalotPlopScene *scene, NNSFndAllocator *allocator) {
    int i = 0;
    ModelResource *resource = scene->resources;
    for (; i < 3; i++) {
        ModelResource_Free(resource);
        resource++;
    }
    {
        ModelAnimation *animation;
        int j;
        j = 0;
        animation = scene->animations;
        for (; j < 6; j++) {
            ModelAnimation_Free(animation, allocator);
            animation++;
        }
    }
    ModelAnimation_Free(&scene->special, allocator);
    memset(scene, 0, sizeof(SwalotPlopScene));
}

void SwalotPlopScene_Update(SwalotPlopScene *scene, SwalotPlopBody *body, u32 phase) {
    SwalotPlopScene_SetPos(scene, body);
    SwalotPlopScene_SetSpin(scene, body);
    SwalotPlopScene_SetScale(scene, body);
    switch (phase) {
    case SWALOT_PLOP_PHASE_NORMAL:
    case SWALOT_PLOP_PHASE_FASTEST:
        SwalotPlopScene_SetAnimation(scene, 1);
        SwalotPlopScene_SetTiltFrame(scene, body->tilt);
        SwalotPlopScene_SetBaseAnimation(scene, 1);
        break;
    case SWALOT_PLOP_PHASE_FAST:
        SwalotPlopScene_SetAnimation(scene, 3);
        SwalotPlopScene_SetBaseAnimation(scene, 0);
        SwalotPlopScene_AdvanceAnimation(scene, FX32_ONE);
        break;
    }
}

void SwalotPlopScene_Animate(SwalotPlopScene *scene, SwalotPlopBody *body, u32 phase) {
    ModelAnimation_AdvanceLooped(&scene->animations[0], FX32_ONE);
    SwalotPlopScene_UpdateSpecial(scene);
}

void SwalotPlopScene_SetPos(SwalotPlopScene *scene, SwalotPlopBody *body) {
    int i;
    ModelInstance *model = scene->models;
    for (i = 0; i < 3; i++, model++) {
        ModelInstance_SetPosition(model, body->position.x, body->position.y, body->position.z);
    }
}

void SwalotPlopScene_SetSpin(SwalotPlopScene *scene, SwalotPlopBody *body) {
    ModelInstance *model;
    u32 spin;
    int i;
    model = scene->models;
    spin = body->spin;
    for (i = 0; i < 3; i++, model++) {
        ModelInstance_SetRotation(model, (u16)spin, 1);
    }
}

void SwalotPlopScene_SetScale(SwalotPlopScene *scene, SwalotPlopBody *body) {
    int i;
    ModelInstance *model = scene->models;
    for (i = 0; i < 3; i++, model++) {
        fx32 scale = body->modelScale;
        ModelInstance_SetScale(model, scale, scale, scale);
    }
}

void SwalotPlopScene_Draw(SwalotPlopScene *scene, SwalotPlopBody *body) {
    int i;
    ModelInstance *model = scene->models;
    for (i = 0; i < 3; i++, model++) {
        ModelInstance_Draw(model);
    }
}

void SwalotPlopScene_SetTiltFrame(SwalotPlopScene *scene, u16 tilt) {
    u32 maxTilt = GF_DegreeToSinCosIdx(0xF222);
    u32 degrees = (90 * (tilt - 0xE38)) / maxTilt;
    s32 frame = _s32_div_f((s32)((45 << 14) * degrees), 90);
    frame = (45 << 14) - frame;
    ModelAnimation_SetFrame(&scene->animations[1], frame);
}

void SwalotPlopScene_SetAnimation(SwalotPlopScene *scene, u32 animation) {
    u16 old;
    if (animation >= 6) {
        GF_AssertFail();
    }
    old = scene->current;
    if (old != animation) {
        ModelInstance_RemoveAnimation(&scene->models[ov91_0226274C[old]], &scene->animations[old]);
        ModelInstance_AddAnimation(&scene->models[ov91_0226274C[animation]], &scene->animations[animation]);
        ModelAnimation_SetFrame(&scene->animations[animation], 0);
        ModelInstance_SetVisible(&scene->models[ov91_0226274C[scene->current]], FALSE);
        ModelInstance_SetVisible(&scene->models[ov91_0226274C[animation]], TRUE);
        scene->current = animation;
    }
}

BOOL SwalotPlopScene_AdvanceAnimation(SwalotPlopScene *scene, fx32 step) {
    return ModelAnimation_Advance(&scene->animations[scene->current], step);
}

void SwalotPlopScene_SetBaseAnimation(SwalotPlopScene *scene, int state) {
    u16 old = scene->state;
    if (old != state) {
        if (state == 1) {
            ModelInstance_AddAnimation(&scene->models[0], &scene->animations[0]);
        } else {
            ModelInstance_RemoveAnimation(&scene->models[0], &scene->animations[0]);
        }
        scene->state = state;
    }
}

void SwalotPlopScene_SetBonus(SwalotPlopScene *scene, u16 bonus) {
    scene->bonus = bonus;
}

void SwalotPlopScene_SetLeader(SwalotPlopScene *scene, u32 leader) {
    scene->leader = leader;
}

#pragma force_active on

void SwalotPlopScene_UpdateSpecial(SwalotPlopScene *scene) {
    if (scene->bonus == 1) {
        int frame = scene->frame + 0x2000;
        if (frame < 0x28000) {
            scene->frame = scene->frame + 0x2000;
        } else {
            scene->frame = FX32_ONE;
        }
    } else {
        int i = scene->leader == 4 ? 0 : scene->leader + 1;
        int start = ov91_02261C12[i * 2] << 12;
        int frame = scene->frame;
        if (frame < start) {
            scene->frame = start;
        } else {
            int next = frame + 0x2000;
            if (next < (ov91_02261C13[i * 2] << 12)) {
                scene->frame = next;
            } else {
                scene->frame = start;
            }
        }
    }
    ModelAnimation_SetFrame(&scene->special, scene->frame);
}

#pragma force_active reset

void SwalotPlop_TouchToWorld(int x, int y, VecFx32 *pos) {
    pos->x = (255 - x) * 0x960 - (0x960 << 7);
    pos->z = -0x12C000 - (y * 0x960 - (0x960 << 7));
}

void SwalotPlop_WorldToTouch(const VecFx32 *pos, SwalotPlopPoint *point) {
    point->x = 0xFF - (pos->x + 0x4B000) / 0x960;
    point->y = (-0xE1000 - pos->z) / 0x960;
}

void SwalotPlop_RotateToSeat(const VecFx32 *src, VecFx32 *dst, int seat, int playerCount) {
    MtxFx33 matrix;
    u16 angle;

    MTX_Identity33_(&matrix);
    angle = ov91_02261D3C[playerCount][seat];
    MTX_RotY33_(&matrix, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_MultVec33(src, &matrix, dst);
}

void SwalotPlop_RotateFromSeat(const VecFx32 *src, VecFx32 *dst, int seat, int playerCount) {
    MtxFx33 matrix;
    u16 angle;

    MTX_Identity33_(&matrix);
    angle = ov91_02261D3C[playerCount][seat];
    MTX_RotY33_(&matrix, FX_SinIdx(angle), FX_CosIdx(angle));
    MTX_Inverse33(&matrix, &matrix);
    MTX_MultVec33(src, &matrix, dst);
}

void SwalotPlopGfx_InitShotViews(SwalotPlopGfx *gfx, NARC *narc, enum HeapID heapId) {
    int i;

    for (i = 0; i < 5; i++) {
        ModelResource_LoadFromNarc(&gfx->shotModels[i], narc, ov91_02261BF0[i], heapId);
    }
    ModelResource_LoadFromNarc(&gfx->shadowModel, narc, 0x2E, heapId);
    NNSi_G3dModifyPolygonAttrMask(gfx->shadowModel.model, 0, 0x1F << 16);

    gfx->shotRes[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, 7, FALSE, 100, NNS_G2D_VRAM_TYPE_2DSUB, heapId);
    gfx->shotRes[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], narc, 4, FALSE, 100, NNS_G2D_VRAM_TYPE_2DSUB, 4, heapId);
    gfx->shotRes[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, 6, FALSE, 100, GF_GFX_RES_TYPE_CELL, heapId);
    gfx->shotRes[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, 5, FALSE, 100, GF_GFX_RES_TYPE_ANIM, heapId);

    if (!SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(gfx->shotRes[GF_GFX_RES_TYPE_CHAR])) {
        GF_AssertFail();
    }
    if (!SpriteTransfer_CreatePlttTransferTask(gfx->shotRes[GF_GFX_RES_TYPE_PLTT])) {
        GF_AssertFail();
    }
    sub_0200A740(gfx->shotRes[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(gfx->shotRes[GF_GFX_RES_TYPE_PLTT]);

    CreateSpriteResourcesHeader(&gfx->shotHeader, 100, 100, 100, 100, -1, -1, FALSE, 0, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);

    for (i = 0; i < 0x60; i++) {
        SwalotPlopShotView_Init(gfx, &gfx->shotViews[i], heapId);
    }
}

void SwalotPlopGfx_FreeShotViews(SwalotPlopGfx *gfx) {
    ModelResource *model;
    int i;

    for (i = 0; i < 0x60; i++) {
        SwalotPlopShotView_Free(gfx, &gfx->shotViews[i]);
    }

    SpriteTransfer_DeleteCharTransferTask(gfx->shotRes[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(gfx->shotRes[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->shotRes[GF_GFX_RES_TYPE_CHAR]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->shotRes[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->shotRes[GF_GFX_RES_TYPE_CELL]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_ANIM], gfx->shotRes[GF_GFX_RES_TYPE_ANIM]);

    model = gfx->shotModels;
    for (i = 0; i < 5; i++, model++) {
        ModelResource_Free(model);
    }
    ModelResource_Free(&gfx->shadowModel);
    memset(gfx->shotModels, 0, sizeof(gfx->shotModels) + sizeof(gfx->shadowModel) + sizeof(gfx->shotRes) + sizeof(gfx->shotHeader));
    memset(gfx->shotViews, 0, sizeof(gfx->shotViews));
}

void SwalotPlopGfx_AddShotView(SwalotPlopGfx *gfx, SwalotPlopShot *shot, u32 playerCount, u32 localIndex) {
    SwalotPlopShotView *view = SwalotPlopGfx_GetFreeShotView(gfx);
    if (shot->params.owner == localIndex) {
        SwalotPlopShotView_StartSprite(gfx, view, shot, playerCount);
    } else {
        SwalotPlopShotView_StartModel(gfx, view, shot);
    }
}

void SwalotPlopGfx_RemoveShotView(SwalotPlopGfx *gfx, SwalotPlopShot *shot) {
    // walks the struct shifted by one element per step, as the original code does
    int i;
    SwalotPlopGfx *walker;

    i = 0;
    walker = gfx;
    while (i < 0x60) {
        if (walker->shotViews[0].shot == shot) {
            SwalotPlopShotView_Clear((SwalotPlopShotView *)((u8 *)gfx + offsetof(SwalotPlopGfx, shotViews) + i * sizeof(SwalotPlopShotView)));
            return;
        }
        i++;
        walker = (SwalotPlopGfx *)((u8 *)walker + sizeof(SwalotPlopShotView));
    }
    GF_AssertFail();
}

void SwalotPlopGfx_DrawShotViews(SwalotPlopGfx *gfx, u32 playerCount) {
    int i;

    for (i = 0; i < 0x60; i++) {
        if (gfx->shotViews[i].shot != NULL) {
            SwalotPlopShotView_Draw(&gfx->shotViews[i], gfx, playerCount);
        }
    }
}

void SwalotPlopShotView_Init(SwalotPlopGfx *gfx, SwalotPlopShotView *view, enum HeapID heapId) {
    SpriteTemplate template = { 0 };
    Sprite *sprite;

    template.spriteList = gfx->spriteList;
    template.header = &gfx->shotHeader;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.drawPriority = 0x20;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = heapId;
    sprite = Sprite_CreateAffine(&template);
    view->sprite = sprite;
    Sprite_SetDrawFlag(sprite, FALSE);
    ModelInstance_Init(&view->shadow, &gfx->shadowModel);
    ModelInstance_SetVisible(&view->shadow, FALSE);
}

void SwalotPlopShotView_Free(SwalotPlopGfx *gfx, SwalotPlopShotView *view) {
    Sprite_Delete(view->sprite);
    memset(view, 0, sizeof(SwalotPlopShotView));
}

#pragma force_active on

void SwalotPlopShotView_StartSprite(SwalotPlopGfx *gfx, SwalotPlopShotView *view, SwalotPlopShot *shot, u32 playerCount) {
    view->shot = shot;
    view->state = 1;
    view->timer = 0;
    Sprite_SetPaletteOverride(view->sprite, shot->params.owner);
    SwalotPlopShotView_UpdateSprite(view, shot, playerCount);
    Sprite_SetDrawFlag(view->sprite, TRUE);
}

#pragma force_active reset

void SwalotPlopShotView_StartModel(SwalotPlopGfx *gfx, SwalotPlopShotView *view, SwalotPlopShot *shot) {
    view->shot = shot;
    view->timer = 0;
    Sprite_SetDrawFlag(view->sprite, FALSE);
    SwalotPlopShotView_SwitchToModel(view, gfx);
}

#pragma force_active on

void SwalotPlopShotView_Draw(SwalotPlopShotView *view, SwalotPlopGfx *gfx, u32 playerCount) {
    if (view->state == 1 && SwalotPlopShotView_UpdateSprite(view, view->shot, playerCount) == FALSE) {
        SwalotPlopShotView_SwitchToModel(view, gfx);
    }
    if (view->state == 0) {
        ModelResource *model = SwalotPlop_GetShotModel(view->shot, gfx->shotModels);
        if (view->shot->state == SHOT_STATE_BOUNCED) {
            NNS_G3dMdlSetMdlAmbAll(model->model, GX_RGB(31, 31, 31));
        } else {
            NNS_G3dMdlSetMdlAmbAll(model->model, GX_RGB(18, 18, 18));
        }
        SwalotPlopShotView_Spin(view);
        ModelInstance_SetPosition(&view->model, view->shot->position.x, view->shot->position.y, view->shot->position.z);
        ModelInstance_Draw(&view->model);
        if (ModelInstance_IsVisible(&view->shadow) == TRUE) {
            SwalotPlopShotView_PlaceShadow(view);
            ModelInstance_Draw(&view->shadow);
            SwalotPlopShotView_ResetPolygonAttr(view);
        }
    }
}

void SwalotPlopShotView_PlaceShadow(SwalotPlopShotView *view) {
    fx32 groundY, height, scale, value;
    fx32 factor = FX32_ONE;
    if (SwalotPlopShot_IsInPool(view->shot) == TRUE) {
        groundY = -0x12C000;
    } else if (SwalotPlopShot_IsOnRim(view->shot) == FALSE) {
        groundY = -0x33000;
    } else {
        groundY = -0x12C000;
    }
    ModelInstance_SetPosition(&view->shadow, view->shot->position.x, groundY, view->shot->position.z);
    height = view->shot->position.y - groundY;
    value = (fx32)(((s64)height * factor + factor / 2) >> 12);
    scale = FX_Div(value, 0x12C000) + FX32_ONE;
    ModelInstance_SetScale(&view->shadow, scale, scale, scale);
    value = (fx32)(((s64)height * 65536 + 2048) >> 12);
    value = FX_Div(value, 0x12C000);
    NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, 0, 24 - (value >> 12), 0);
}

#pragma force_active reset

void SwalotPlopShotView_ResetPolygonAttr(SwalotPlopShotView *view) {
    NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, 0, 31, 0);
}

void SwalotPlopShotView_Clear(SwalotPlopShotView *view) {
    view->shot = NULL;
    view->state = 0;
    Sprite_SetDrawFlag(view->sprite, FALSE);
    ModelInstance_SetVisible(&view->shadow, FALSE);
}

#pragma force_active on

SwalotPlopShotView *SwalotPlopGfx_GetFreeShotView(SwalotPlopGfx *gfx) {
    int i;
    for (i = 0; i < 0x60; i++) {
        if (gfx->shotViews[i].shot == NULL) {
            return &gfx->shotViews[i];
        }
    }
    GF_AssertFail();
    return NULL;
}

BOOL SwalotPlopShotView_UpdateSprite(SwalotPlopShotView *view, SwalotPlopShot *shot, u32 playerCount) {
    SwalotPlopPoint point;
    VecFx32 pos = view->shot->position;
    SwalotPlop_RotateFromSeat(&pos, &pos, view->shot->params.owner, playerCount);
    SwalotPlop_WorldToTouch(&pos, &point);
    pos.x = point.x << 12;
    pos.y = (point.y << 12) + (512 << FX32_SHIFT);
    pos.z = 0;
    Sprite_SetMatrix(view->sprite, &pos);
    if (point.x < 0 || point.y < 0) {
        return FALSE;
    }
    return TRUE;
}

void SwalotPlopShotView_SwitchToModel(SwalotPlopShotView *view, SwalotPlopGfx *gfx) {
    Sprite_SetDrawFlag(view->sprite, FALSE);
    view->state = 0;
    ModelInstance_Init(&view->model, SwalotPlop_GetShotModel(view->shot, gfx->shotModels));
    ModelInstance_SetVisible(&view->model, TRUE);
    if (view->shot->state == SHOT_STATE_FLYING) {
        ModelInstance_SetVisible(&view->shadow, TRUE);
    }
}

#pragma force_active reset

ModelResource *SwalotPlop_GetShotModel(SwalotPlopShot *shot, ModelResource *models) {
    u32 index;
    if (shot->params.big == TRUE) {
        index = 4;
    } else {
        index = shot->params.owner;
    }
    return &models[index];
}

static inline s64 SinCosComponent(s32 value, s32 scale) {
    return (s64)value * scale;
}

#pragma force_active on

void SwalotPlopShotView_Spin(SwalotPlopShotView *view) {
    view->timer = (s16)(view->timer - 1);
    if (view->timer < 0) {
        int speed = SwalotPlopShot_GetSpeed(view->shot);
        int angle = (FX_Atan2Idx(view->shot->params.direction.z, view->shot->params.direction.x) >> 4) * 2;
        s32 value = (u16)((s32)((u32)speed << 9) / 0x1E200) << 12;
        view->rotSpeedX = (((fx32)((SinCosComponent(FX_SinCosTable_[angle], value) + 2048) >> 12)) >> 12) + 512;
        view->rotSpeedZ = (((fx32)((SinCosComponent(FX_SinCosTable_[angle + 1], value) + 2048) >> 12)) >> 12) + 512;
        view->timer = 8;
    }
    view->rotX += view->rotSpeedX;
    view->rotZ += view->rotSpeedZ;
    ModelInstance_SetRotation(&view->model, view->rotX, 0);
    ModelInstance_SetRotation(&view->model, view->rotZ, 2);
}

void SwalotPlopCursor_Init(SwalotPlopGfx *gfx, SwalotPlopCursor *cursor, int palette, enum HeapID heapId) {
    SpriteTemplate template = { 0 };
    template.spriteList = gfx->spriteList;
    template.header = &gfx->shotHeader;
    template.heapID = heapId;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.drawPriority = 32;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    cursor->sprite = Sprite_CreateAffine(&template);
    Sprite_SetDrawFlag(cursor->sprite, FALSE);
    Sprite_SetPaletteOverride(cursor->sprite, palette);
    cursor->active = FALSE;
}

#pragma force_active reset

void SwalotPlopCursor_Free(SwalotPlopCursor *cursor) {
    Sprite_Delete(cursor->sprite);
}

void SwalotPlopCursor_Show(SwalotPlopCursor *cursor) {
    Sprite_SetDrawFlag(cursor->sprite, TRUE);
    cursor->active = TRUE;
}

#pragma force_active on

void SwalotPlopCursor_Update(SwalotPlopCursor *cursor) {
    if (cursor->active) {
        SwalotPlopCursor_SetPos(cursor, gSystem.touchX, gSystem.touchY);
    }
}

void SwalotPlopCursor_SetPos(SwalotPlopCursor *cursor, int x, int y) {
    VecFx32 pos;
    pos.x = x << 12;
    pos.y = (y << 12) + (512 << FX32_SHIFT);
    Sprite_SetMatrix(cursor->sprite, &pos);
}

#pragma force_active reset

void SwalotPlopCursor_Hide(SwalotPlopCursor *cursor) {
    Sprite_SetDrawFlag(cursor->sprite, FALSE);
    cursor->active = FALSE;
}

#pragma force_active on

void SwalotPlopParticles_Init(SwalotPlopGfx *gfx, SwalotPlopParticle *particle, int palette, enum HeapID heapId) {
    SpriteTemplate template = { 0 };
    int i;
    template.spriteList = gfx->spriteList;
    template.header = &gfx->shotHeader;
    template.scale.x = 9;
    template.scale.y = 9;
    template.scale.z = 9;
    template.drawPriority = 32;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = heapId;
    for (i = 0; i < 8; i++) {
        particle->sprite = Sprite_CreateAffine(&template);
        Sprite_SetDrawFlag(particle->sprite, FALSE);
        Sprite_SetAffineOverwriteMode(particle->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_NORMAL);
        Sprite_SetPaletteOverride(particle->sprite, palette);
        particle->active = FALSE;
        particle++;
    }
}

#pragma force_active reset

void SwalotPlopParticles_Free(SwalotPlopGfx *gfx, SwalotPlopParticle *particle) {
    int i;
    for (i = 0; i < 8; i++, particle++) {
        Sprite_Delete(particle->sprite);
    }
}

void SwalotPlopParticles_Update(SwalotPlopParticle *particles) {
    int i;
    s32 dx;
    s32 dy;
    VecFx32 scale;

    for (i = 0; i < 8; i++) {
        SwalotPlopParticle *particle = &particles[i];

        if (particle->active == TRUE) {
            if (particle->age < 16) {
                SwalotPlop_CalcArcStep(particle->age, 16, 8, 190, 125, &dx, &dy);
            } else if (particle->age < 26) {
                SwalotPlop_CalcArcStep(particle->age - 16, 10, 4, 182, 140, &dx, &dy);
            } else {
                SwalotPlop_CalcArcStep(particle->age - 26, 6, 3, 180, 160, &dx, &dy);
            }

            particle->age++;
            scale.x = FX_Div((9 - particle->age * 3 / 32) * FX32_ONE, 10 * FX32_ONE);
            scale.y = scale.x;
            Sprite_SetAffineScale(particle->sprite, &scale);

            if (particle->direction) {
                particle->position.x += dx << FX32_SHIFT;
            } else {
                particle->position.x -= dx << FX32_SHIFT;
            }
            particle->position.y += dy << FX32_SHIFT;
            Sprite_SetMatrix(particle->sprite, &particle->position);

            if (particle->position.x < -0x20000 || particle->position.x > 0x120000 || particle->position.y < 0x1E0000 || particle->position.y > 0x2E0000) {
                particle->active = FALSE;
                Sprite_SetDrawFlag(particle->sprite, FALSE);
            }
        }
    }
}

void SwalotPlopParticles_Spawn(SwalotPlopParticle *particles, s32 x, s32 y) {
    int i;
    SwalotPlopParticle *particle = NULL;

    for (i = 0; i < 8; i++) {
        if (!particles[i].active) {
            particle = &particles[i];
            break;
        }
    }

    if (particle != NULL) {
        particle->direction = x < 0x80;
        particle->position.x = x << FX32_SHIFT;
        particle->position.y = (y << FX32_SHIFT) + (512 << FX32_SHIFT);
        Sprite_SetMatrix(particle->sprite, &particle->position);
        particle->active = TRUE;
        Sprite_SetDrawFlag(particle->sprite, TRUE);
        particle->age = 0;
    }
}

static inline int FxMul(int a, int b) {
    return (int)(((s64)a * b + 2048) >> 12);
}

#pragma force_active on

void SwalotPlop_CalcArcStep(int step, int steps, int radius, int startAngle, int endAngle, int *x, int *y) {
    u16 angle = (s16)(startAngle + ((endAngle - startAngle) * step) / steps);
    int cosine = GF_CosDegNoWrap(angle);
    int r = radius << 12;
    *x = FxMul(cosine, r) >> 12;
    *y = FxMul(GF_SinDegNoWrap(angle), r) >> 12;
}

void SwalotPlopStage_Init(SwalotPlopStage *stage, SwalotPlopGfx *gfx, NARC *narc, int playerCount, int localIndex, enum HeapID heapId, NNSFndAllocator *volatile allocator) {
    int i, fileId;
    ModelInstance *model;
    ModelResource *resource;
    // keeps the original's reload of the allocator from the stack
    allocator = allocator;
    resource = stage->resources;
    model = stage->models;
    for (i = 0; i < 3; i++, resource++, model++) {
        if (i == 0) {
            fileId = 36;
        } else if (i == 1) {
            fileId = 37;
        } else {
            fileId = ov91_02261C0A[playerCount - 1];
        }
        ModelResource_LoadFromNarc(resource, narc, fileId, heapId);
        ModelInstance_Init(model, resource);
        ModelInstance_SetPosition(model, 0, -0x62000, 0);
        ModelInstance_SetScale(model, 0x1800, 0x1800, 0x1800);
        if (i == 1) {
            ModelInstance_SetVisible(model, FALSE);
        }
        if ((u32)i <= 1) {
            ModelInstance_SetRotation(model, ov91_02261D64[playerCount - 1][localIndex], 1);
        } else {
            ModelInstance_SetRotation(model, GF_DegreeToSinCosIdxNoWrap(180), 1);
        }
    }
    for (i = 0; i < 3; i++) {
        ModelAnimation_LoadFromNarc(&stage->animations[i], &stage->resources[ov91_02261BFE[i]], narc, ov91_02261BF8[i], heapId, allocator);
        ModelInstance_AddAnimation(&stage->models[ov91_02261BFE[i]], &stage->animations[i]);
    }
    stage->animSpeed = FX32_ONE;
}

#pragma force_active reset

void SwalotPlopStage_Free(SwalotPlopStage *stage, SwalotPlopGfx *gfx, NNSFndAllocator *allocator) {
    int i = 0;
    ModelAnimation *animation = stage->animations;
    for (; i < 3; i++, animation++) {
        ModelAnimation_Free(animation, allocator);
    }
    i = 0;
    ModelResource *resource = stage->resources;
    for (; i < 3; i++, resource++) {
        ModelResource_Free(resource);
    }
}

#pragma force_active on

void SwalotPlopStage_Draw(SwalotPlopStage *stage) {
    int i;
    ModelAnimation_AdvanceLooped(&stage->animations[0], stage->animSpeed);
    if (stage->mouthActive) {
        switch (stage->mouthState) {
        case 0: {
            s32 step = stage->mouthStep;
            if (stage->mouthFrame + step < 0x33000) {
                stage->mouthFrame += step;
            } else {
                stage->mouthFrame = 0;
            }
            if (stage->mouthStep + 410 <= 0x24CD) {
                stage->mouthStep += 410;
            } else {
                stage->mouthStep = 0x24CD;
                stage->mouthTimer = 24;
                stage->mouthState++;
            }
            break;
        }
        case 1: {
            s32 step = stage->mouthStep;
            if (stage->mouthFrame + step < 0x33000) {
                stage->mouthFrame += step;
            } else {
                stage->mouthFrame = 0;
            }
            stage->mouthTimer--;
            if (stage->mouthTimer <= 0) {
                stage->mouthState++;
                stage->mouthFrame = 0x33000;
            }
            break;
        }
        case 2:
            if (stage->mouthFrame + FX32_ONE < 0x64000) {
                stage->mouthFrame += FX32_ONE;
            } else {
                stage->mouthFrame = 0x33000;
            }
            break;
        default:
            GF_AssertFail();
            break;
        }
        ModelAnimation_SetFrame(&stage->animations[1], stage->mouthFrame);
        ModelAnimation_SetFrame(&stage->animations[2], stage->mouthFrame);
    }
    ModelInstance *model = stage->models;
    for (i = 0; i < 3; i++, model++) {
        ModelInstance_Draw(model);
    }
}

#pragma force_active reset

void SwalotPlopStage_StartMouth(SwalotPlopStage *stage) {
    ModelInstance_SetVisible(&stage->models[0], FALSE);
    ModelInstance_SetVisible(&stage->models[1], TRUE);
    stage->mouthFrame = 0;
    stage->mouthStep = FX32_ONE;
    stage->mouthActive = TRUE;
    stage->mouthState = 0;
}

void SwalotPlopStage_SetAnimSpeed(SwalotPlopStage *stage, fx32 speed) {
    stage->animSpeed = speed;
}

#pragma force_active on

void SwalotPlop_IntersectPlane(const VecFx32 *pos, const VecFx32 *dir, const VecFx32 *normal, fx32 planeDist, VecFx32 *hit, fx32 *t) {
    fx32 scale;
    scale = FX_Div(planeDist + (-FX_Mul(normal->x, pos->x) - FX_Mul(normal->y, pos->y) - FX_Mul(normal->z, pos->z)),
        FX_Mul(normal->z, dir->z) + (FX_Mul(normal->x, dir->x) + FX_Mul(normal->y, dir->y)));
    hit->x = pos->x + FX_Mul(dir->x, scale);
    hit->y = pos->y + FX_Mul(dir->y, scale);
    // the original writes the z result to x as well
    hit->x = pos->z + FX_Mul(dir->z, scale);
    *t = scale;
}

#pragma force_active reset

void SwalotPlopGfx_SetCameraHeight(SwalotPlopGfx *gfx, fx32 height) {
    gfx->cameraTarget.y = height;
}

void SwalotPlopGfx_SetCameraPitch(SwalotPlopGfx *gfx, u16 pitch) {
    CameraAngle angle;
    angle = Camera_GetAngle(gfx->camera);
    angle.x = pitch;
    Camera_SetAnglePos(&angle, gfx->camera);
}

void SwalotPlopBgFade_Init(SwalotPlopBgFade *fade, u8 palette) {
    u8 *bytes = (u8 *)fade;
    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
    bytes[4] = 0;
    bytes[5] = 0;
    bytes[6] = 0;
    bytes[7] = 0;
    fade->layer = 2;
    fade->palette = palette;
}

#pragma force_active on

void SwalotPlopBgFade_Update(SwalotPlopBgFade *fade, SwalotPlopGfx *gfx) {
    int layer, older, prev, alpha;
    switch (fade->state) {
    case 1:
        SwalotPlopBgFade_SetLayers(gfx, (s16)(fade->layer - 1), fade->palette);
        fade->state++;
        fade->timer = 0;
    case 2:
        if (fade->timer >= 16) {
            fade->state = 3;
            fade->timer = 0;
            SwalotPlopBgFade_SetLayers(gfx, fade->layer, fade->palette);
            G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, 1 << fade->layer, GX_BLEND_PLANEMASK_BG3, 0, 16);
            return;
        }
        fade->timer++;
        layer = fade->layer;
        older = layer - 2;
        prev = layer - 1;
        if (layer < 0) {
            layer += 3;
        }
        if (older < 0) {
            older += 3;
        }
        if (prev < 0) {
            prev += 3;
        }
        alpha = 16 - (fade->timer * 16) / 16;
        G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, 1 << layer, GX_BLEND_PLANEMASK_BG3 | ((1 << older) | (1 << prev)), alpha, 16 - alpha);
        return;
    case 3:
        if (fade->timer >= 8) {
            fade->state = 0;
            reg_G2S_DB_BLDCNT = 0;
            return;
        }
        fade->timer++;
        alpha = (fade->timer * 16) / 8;
        G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, 1 << fade->layer, GX_BLEND_PLANEMASK_BG3, alpha, 16 - alpha);
        break;
    case 0:
    default:
        break;
    }
}

#pragma force_active reset

void SwalotPlopBgFade_Step(SwalotPlopBgFade *fade) {
    fade->counter++;
    if (fade->counter >= 10) {
        fade->state = 1;
        fade->layer = (fade->layer + 1) % 3;
        fade->counter = 0;
    }
}

#pragma force_active on

void SwalotPlopBgFade_SetLayers(SwalotPlopGfx *gfx, int top, int palette) {
    int i;
    int layer;
    for (i = 0; i < 3; i++) {
        layer = top - i;
        if (layer < 0) {
            layer += 3;
        }
        SetBgPriority(layer + GF_BG_LYR_SUB_0, (u8)(2 - i));
        if (i == 0) {
            BgTilemapRectChangePalette(gfx->bgConfig, layer + GF_BG_LYR_SUB_0, 0, 0, 32, 32, palette * 2 + 2);
        } else {
            BgTilemapRectChangePalette(gfx->bgConfig, layer + GF_BG_LYR_SUB_0, 0, 0, 32, 32, palette * 2 + 1);
        }
        BgCommitTilemapBufferToVram(gfx->bgConfig, layer + GF_BG_LYR_SUB_0);
    }
}

#pragma force_active reset

void SwalotPlopCombo_Init(SwalotPlopCombo *combo, SwalotPlopGfx *gfx, NARC *narc, enum HeapID heapId) {
    SimpleSpriteTemplate spriteTemplate;
    TextOBJTemplate textTemplate;
    combo->heapId = heapId;
    combo->res[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, 10, FALSE, 140, NNS_G2D_VRAM_TYPE_2DMAIN, heapId);
    combo->res[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], narc, 11, FALSE, 140, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapId);
    combo->res[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, 9, FALSE, 140, GF_GFX_RES_TYPE_CELL, heapId);
    combo->res[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, 8, FALSE, 140, GF_GFX_RES_TYPE_ANIM, heapId);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(combo->res[GF_GFX_RES_TYPE_CHAR]));
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(combo->res[GF_GFX_RES_TYPE_PLTT]));
    sub_0200A740(combo->res[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(combo->res[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&combo->header, 140, 140, 140, 140, -1, -1, FALSE, 0, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &combo->header;
    spriteTemplate.position.x = -0x40000;
    spriteTemplate.position.y = 0x30000;
    spriteTemplate.priority = 1;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = heapId;
    combo->sprite = Sprite_Create(&spriteTemplate);
    Sprite_SetDrawFlag(combo->sprite, FALSE);
    combo->string = String_New(0x10, heapId);
    AddTextWindowTopLeftCorner(gfx->bgConfig, &combo->window, 3, 2, 0, 0);
    combo->textLayout = TextOBJLayout_New(&combo->window, heapId);
    GF_ASSERT(sub_02021AC8(TextOBJLayout_GetCharSize(combo->textLayout, NNS_G2D_VRAM_TYPE_2DMAIN), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &combo->charTransfer) == TRUE);
    combo->textPltt = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_graphic_font, 7, FALSE, 150, NNS_G2D_VRAM_TYPE_2DMAIN, 1, heapId);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(combo->textPltt));
    sub_0200A740(combo->textPltt);
    textTemplate.fontSystem = gfx->fontSystem;
    textTemplate.window = &combo->window;
    textTemplate.spriteList = gfx->spriteList;
    textTemplate.plttResourceProxy = SpriteTransfer_GetPaletteProxy(combo->textPltt, NULL);
    textTemplate.sprite = combo->sprite;
    textTemplate.offset = combo->charTransfer.offset;
    textTemplate.x = -7;
    textTemplate.y = -5;
    textTemplate.unk_20 = 0;
    textTemplate.unk_24 = 0;
    textTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    textTemplate.heapID = heapId;
    combo->textObj = TextOBJ_Create(&textTemplate, combo->textLayout);
    TextOBJ_SetSpritesDrawFlag(combo->textObj, FALSE);
    RemoveWindow(&combo->window);
    SwalotPlopCurve_Init(&combo->slideX, -0x40000, 0x20000, 0x24000, 8);
    SwalotPlopCurve_Init(&combo->slideY, 0x30000, 0x30000, 0xC000, 4);
    combo->slideFrame = 0;
    combo->bounceFrame = 4;
}

void SwalotPlopCombo_Free(SwalotPlopCombo *combo, SwalotPlopGfx *gfx) {
    TextOBJ_Destroy(combo->textObj);
    SpriteTransfer_DeletePlttTransferTask(combo->textPltt);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_PLTT], combo->textPltt);
    sub_02021B5C(&combo->charTransfer);
    TextOBJLayout_Delete(combo->textLayout);
    String_Delete(combo->string);
    Sprite_Delete(combo->sprite);
    SpriteTransfer_DeleteCharTransferTask(combo->res[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(combo->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CHAR], combo->res[GF_GFX_RES_TYPE_CHAR]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_PLTT], combo->res[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_CELL], combo->res[GF_GFX_RES_TYPE_CELL]);
    DestroySingle2DGfxResObj(gfx->resMan[GF_GFX_RES_TYPE_ANIM], combo->res[GF_GFX_RES_TYPE_ANIM]);
}

void SwalotPlopCombo_Show(SwalotPlopCombo *combo, SwalotPlopGfx *gfx, int count) {
    String16_FormatInteger(combo->string, count, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    AddTextWindowTopLeftCorner(gfx->bgConfig, &combo->window, 3, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&combo->window, 0, combo->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    TextOBJ_CopyFromBGWindow(combo->textObj, combo->textLayout, &combo->window, combo->heapId);
    RemoveWindow(&combo->window);
    TextOBJ_SetSpritesDrawFlag(combo->textObj, TRUE);
    Sprite_SetDrawFlag(combo->sprite, TRUE);
    combo->state = 1;
    combo->hideDelay = 0;
}

void SwalotPlopCombo_Hide(SwalotPlopCombo *combo) {
    u16 state = combo->state;
    if (state != 3) {
        if (state == 2) {
            combo->hideDelay = 12;
        }
        combo->state = 3;
    }
}

void SwalotPlopCombo_SetCount(SwalotPlopCombo *combo, SwalotPlopGfx *gfx, int count) {
    if (combo->state != 3) {
        String16_FormatInteger(combo->string, count, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        AddTextWindowTopLeftCorner(gfx->bgConfig, &combo->window, 3, 2, 0, 0);
        AddTextPrinterParameterizedWithColor(&combo->window, 0, combo->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
        TextOBJ_CopyFromBGWindow(combo->textObj, combo->textLayout, &combo->window, combo->heapId);
        RemoveWindow(&combo->window);
        combo->bounceFrame = 0;
    }
}

void SwalotPlopCombo_Update(SwalotPlopCombo *combo) {
    switch (combo->state) {
    case 0:
        break;
    case 1:
        if (combo->slideFrame < 8) {
            combo->slideFrame++;
        } else {
            combo->state = 2;
        }
        break;
    case 2:
        break;
    case 3:
        if (combo->hideDelay > 0) {
            combo->hideDelay--;
        } else if (combo->slideFrame > 0) {
            combo->slideFrame--;
        } else {
            combo->state = 0;
            TextOBJ_SetSpritesDrawFlag(combo->textObj, FALSE);
            Sprite_SetDrawFlag(combo->sprite, FALSE);
        }
        break;
    }

    if (combo->bounceFrame < 4) {
        combo->bounceFrame++;
        combo->slideY.step = combo->bounceFrame;
        SwalotPlopCurve_Update(&combo->slideY);
    }
    combo->slideX.step = combo->slideFrame;
    SwalotPlopCurve_Update(&combo->slideX);
    {
        VecFx32 pos;
        pos.x = combo->slideX.value;
        pos.y = combo->slideY.value;
        Sprite_SetMatrix(combo->sprite, &pos);
    }
    TextOBJ_UpdatePosition(combo->textObj);
}

void SwalotPlopSpeedUp_Start(SwalotPlopSpeedUp *speedUp) {
    speedUp->state = 0;
    speedUp->active = TRUE;
    speedUp->frame = 0;
    PlaySE(SEQ_SE_PL_PINPON);
}

#pragma force_active on

void SwalotPlopSpeedUp_Update(SwalotPlopSpeedUp *speedUp, SwalotPlopStage *stage, SwalotPlopScene *scene) {
    if (!speedUp->active) {
        return;
    }
    switch (speedUp->state) {
    case 0:
        speedUp->frame++;
        SwalotPlopStage_SetAnimSpeed(stage, speedUp->frame * 102400 / 58 + FX32_ONE);
        GF_SndHandleSetTempoRatio(7, speedUp->frame * 150 / 58 + 256);
        if (speedUp->frame == 12) {
            PlaySE(SEQ_SE_PL_BALLOON05_2);
        }
        if (speedUp->frame >= 58) {
            speedUp->state++;
        }
        break;
    case 1:
        StartBrightnessTransition(4, 16, 0, (GXBlendPlaneMask)0x3F, 1);
        speedUp->state++;
        break;
    case 2:
        if (IsBrightnessTransitionActive(1) == TRUE) {
            SwalotPlopStage_StartMouth(stage);
            SwalotPlopStage_SetAnimSpeed(stage, 2 * FX32_ONE);
            SwalotPlopScene_SetBonus(scene, 1);
            speedUp->state++;
        }
        break;
    case 3:
        StartBrightnessTransition(14, 0, 16, (GXBlendPlaneMask)0x3F, 1);
        speedUp->state++;
        break;
    case 4:
        if (IsBrightnessTransitionActive(1) == TRUE) {
            speedUp->state++;
        }
        break;
    case 5:
        speedUp->active = FALSE;
        speedUp->state = 0;
        break;
    }
}

#pragma force_active reset

void SwalotPlopSpeedUp_ResetTempo(SwalotPlopSpeedUp *speedUp) {
    GF_SndHandleSetTempoRatio(7, 0x100);
}

const SwalotPlopCommand *SwalotPlop_GetCommandTable(void) {
    return ov91_02262754;
}

int SwalotPlop_GetCommandCount(void) {
    return 8;
}

void SwalotPlop_RegisterCommands(SwalotPlopApp *app) {
    Comm_RegisterCommandTable(SwalotPlop_GetCommandTable(), SwalotPlop_GetCommandCount(), app);
}

u32 SwalotPlop_GetNoDataSize(void) {
    return 0;
}

void SwalotPlop_HandleStart(int netId, int size, void *data, void *context) {
    SwalotPlop_ReceiveStart(context);
}

void SwalotPlop_HandleFinish(int netId, int size, void *data, void *context) {
    SwalotPlop_ReceiveFinish(context);
}

void SwalotPlop_HandleShot(int netId, int size, void *data, void *context) {
    SwalotPlop_ReceiveShot(context, data, netId);
    if (!Comm_GetLocalNetId()) {
        SwalotPlop_TallyShot(context, data, netId);
    }
}

void SwalotPlop_HandleFinalScore(int netId, int size, void *data, void *context) {
    SwalotPlop_ReceiveFinalScore(context, *(u32 *)data, netId);
}

void SwalotPlop_HandleScores(int netId, int size, void *data, void *context) {
    SwalotPlop_ReceiveScores(context, data, netId);
}

void SwalotPlop_HandleScoreReport(int netId, int size, void *data, void *context) {
    SwalotPlop_ReceiveScoreReport(context, *(u32 *)data, netId);
}

void SwalotPlop_HandleSync(int netId, int size, void *data, void *context) {
    SwalotPlop_ReceiveSync(context);
}

void SwalotPlop_HandleLevel(int netId, int size, void *data, void *context) {
    u32 level = *(u32 *)data;
    if (netId == 0) {
        SwalotPlop_ReceiveLevel(context, level);
    }
}

u32 SwalotPlop_GetShotSize(void) {
    return sizeof(SwalotPlopShotRecord);
}

u32 SwalotPlop_GetScoresSize(void) {
    return 0x10;
}

u32 SwalotPlop_GetScoreSize(void) {
    return 4;
}

u32 SwalotPlop_GetLevelSize(void) {
    return 4;
}
