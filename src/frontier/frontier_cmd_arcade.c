#include "frontier/frontier_cmd_arcade.h"

#include "global.h"

#include "battle/battle_setup.h"
#include "frontier/frontier.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_trainers.h"
#include "frontier/battle_arcade_data.h"
#include "frontier/battle_arcade_setup.h"
#include "frontier/frontier_particles.h"
#include "library/spl_emitter.h"

#include "launch_application.h"
#include "overlay_42.h"
#include "particle_system.h"
#include "communication_player_state.h"
#include "battle_frontier_communication.h"

static void GameBoardArgs_Set(GAME_BOARD_ARGS *args, ArcadeContext *data);
static void BattleArcade_OnGameBoardExit(void *data);
static void BattleArcade_CreateParticleEmitter(FrontierScriptContext *ctx, ArcadeContext *arcadeCtx, u32 a2);
static void BattleArcade_ParticleEmitterCallback(SPLEmitter *emitter);
static BOOL FrontierScriptNative_WaitArcadeLink(FrontierScriptContext *ctx);

BOOL FrtCmd_ArcadeAlloc(FrontierScriptContext *ctx) {
    u32 spC = FrontierScript_ReadVar(ctx);
    u32 r4 = FrontierScript_ReadVar(ctx);
    u32 sp10 = FrontierScript_ReadVar(ctx);
    u32 r6 = FrontierScript_ReadVar(ctx);
    u32 r7 = FrontierScript_ReadVar(ctx);
    u16 *sp14 = FrontierScript_ReadVarPtr(ctx);

    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    Frontier_SetData(ctx->frontierSystem->frontierWork, BattleArcadeData_Alloc(args->saveData, spC, r4, sp10, r6, r7, sp14));

    return FALSE;
}

BOOL FrtCmd_ArcadeInit(FrontierScriptContext *ctx) {
    u32 arg1 = FrontierScript_ReadVar(ctx);
    void *arg0 = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleArcadeData_Init(arg0, arg1);
    return FALSE;
}

BOOL FrtCmd_ArcadeFree(FrontierScriptContext *ctx) {
    BattleArcadeData_Free(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

extern OverlayManagerTemplate gOverlayTemplate_BattleArcadeGameBoard;

BOOL FrtCmd_LaunchGameBoard(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    ArcadeContext *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    GAME_BOARD_ARGS *boardArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(GAME_BOARD_ARGS));
    MI_CpuFill8(boardArgs, 0, sizeof(GAME_BOARD_ARGS));
    boardArgs->saveData = args->saveData;
    GameBoardArgs_Set(boardArgs, data);
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_BattleArcadeGameBoard, boardArgs, 0, BattleArcade_OnGameBoardExit);

    return TRUE;
}

BOOL FrtCmd_187(FrontierScriptContext *ctx) {
    u8 monCnt;
    Pokemon *mon;
    int i, index1, index2, partyCnt, data;

    ArcadeContext *arcadeData = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = arcadeData->battleSetup;

    arcadeData->bpGain = BattleArcade_GetWonBattlePoints(arcadeData, setup->party[0], arcadeData->battleSetup->party[2], setup->unk1B4);

    arcadeData->battleWon = IsBattleResultWin(setup->winFlag);

    if (arcadeData->eventId == 0x1b) {
        index1 = 1;
        index2 = 3;
    } else {
        index1 = 0;
        index2 = 2;
    }

    BattleArcade_CopyMonHeldItem(setup->party[index1], arcadeData->playerParty, 0, 0);
    BattleArcade_CopyMonHeldItem(setup->party[index1], arcadeData->playerParty, 1, 1);

    if (BattleCastle_IsMultiType(arcadeData->type) == 0) {
        BattleArcade_CopyMonHeldItem(setup->party[index1], arcadeData->playerParty, 2, 2);
    } else {
        BattleArcade_CopyMonHeldItem(setup->party[index2], arcadeData->playerParty, 0, 2);
        BattleArcade_CopyMonHeldItem(setup->party[index2], arcadeData->playerParty, 1, 3);
    }

    if (arcadeData->eventId == 0x11) {
        monCnt = BattleArcade_GetMonCount(arcadeData->type, 1);
        for (i = 0; i < monCnt; i++) {
            mon = Party_GetMonByIndex(arcadeData->playerParty, i);
            u32 level = GetMonData(mon, MON_DATA_LEVEL, 0);
            u32 species = GetMonData(mon, MON_DATA_SPECIES, 0);
            u32 exp = GetMonExpBySpeciesAndLevel(species, level - 3);
            SetMonData(mon, MON_DATA_EXPERIENCE, &exp);
            CalcMonLevelAndStats(mon);
        }
    }

    partyCnt = Party_GetCount(arcadeData->playerParty);
    for (i = 0; i < partyCnt; i++) {
        mon = Party_GetMonByIndex(arcadeData->playerParty, i);
        data = arcadeData->savedHp[i];
        SetMonData(mon, MON_DATA_MAX_HP, &data);
        SetMonData(mon, MON_DATA_HP, &data);
        data = arcadeData->savedAtk[i];
        SetMonData(mon, MON_DATA_ATK, &data);
        data = arcadeData->savedDef[i];
        SetMonData(mon, MON_DATA_DEF, &data);
        data = arcadeData->savedSpd[i];
        SetMonData(mon, MON_DATA_SPEED, &data);
        data = arcadeData->savedSpAtk[i];
        SetMonData(mon, MON_DATA_SP_ATK, &data);
        data = arcadeData->savedSpDef[i];
        SetMonData(mon, MON_DATA_SP_DEF, &data);
    }

    BattleSetup_Delete(setup);

    return FALSE;
}

BOOL FrtCmd_ArcadeStartBattle(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    ArcadeContext *arcadeData = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = BattleArcade_NewBattleSetup(arcadeData, args);

    arcadeData->battleSetup = setup;

    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_Battle, setup, 0, NULL);

    return TRUE;
}

static void GameBoardArgs_Set(GAME_BOARD_ARGS *args, ArcadeContext *data) {
    args->type = data->type;
    args->round = BattleArcade_GetRound(data);

    args->eventRandom = &data->eventRandom;
    args->eventId = &data->eventId;
    args->bpGain = data->bpGain;

    args->winStreak = data->winStreak;
    args->multiWinStreak = data->multiWinStreak;

    args->cursorSpeed = &data->cursorSpeed;
    args->playerParty = data->playerParty;
    args->opponentParty = data->opponentParty;
    args->work = data;

    data->weather = 0;
    args->weather = &data->weather;
    args->randomFlag = data->randomFlag;
    data->randomFlag = 0;
    data->eventNoEffect = 0;

    int partyCnt = Party_GetCount(data->playerParty);

    for (int i = 0; i < partyCnt; i++) {
        Pokemon *mon = Party_GetMonByIndex(data->playerParty, i);
        data->savedHp[i] = GetMonData(mon, MON_DATA_MAX_HP, NULL);
        data->savedAtk[i] = GetMonData(mon, MON_DATA_ATK, NULL);
        data->savedDef[i] = GetMonData(mon, MON_DATA_DEF, NULL);
        data->savedSpd[i] = GetMonData(mon, MON_DATA_SPEED, NULL);
        data->savedSpAtk[i] = GetMonData(mon, MON_DATA_SP_ATK, NULL);
        data->savedSpDef[i] = GetMonData(mon, MON_DATA_SP_DEF, NULL);
    }
}

static void BattleArcade_OnGameBoardExit(void *data) {
    GAME_BOARD_ARGS *args = data;
    BattleArcade_StoreGameBoardResult(args->work, args);
    Heap_Free(args);
}

BOOL FrtCmd_ArcadeSetPartyBeforeBattle(FrontierScriptContext *ctx) {
    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleArcade_SetPartyBeforeBattle(arcadeCtx);
    return FALSE;
}

BOOL FrtCmd_ArcadeSetPartyAfterBattle(FrontierScriptContext *ctx) {
    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleArcade_SetPartyAfterBattle(arcadeCtx);
    return FALSE;
}

BOOL FrtCmd_ArcadeAction(FrontierScriptContext *ctx) {
    Pokemon *mon;
    Party *party;
    int i;
    PlayerProfile *profile;

    u32 action = FrontierScript_ReadByte(ctx);
    u8 var0 = FrontierScript_ReadVar(ctx);
    u8 var1 = FrontierScript_ReadVar(ctx);
    u16 *out = FrontierScript_ReadVarPtr(ctx);
    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    FrontierMap *frontierMap = FrontierSystem_GetFrontierMap(ctx->frontierSystem);

    switch (action) {
    case 2:
        arcadeCtx->type = var0;
        break;
    case 3:
        *out = arcadeCtx->gameBoardResults[var0];
        break;
    case 4:
        *out = arcadeCtx->winStreak;
        break;
    case 5:
        if (arcadeCtx->winStreak < 9999) {
            arcadeCtx->winStreak++;
        }
        break;
    case 7:
        OS_ResetSystem(0);
        break;
    case 9:
        *out = ArcadeChallenge_GetSaved(arcadeCtx->challenge);
        break;
    case 10:
        BattleArcadeData_SaveRecord(arcadeCtx, 2);
        break;
    case 12:
        *out = ov80_02238430(arcadeCtx, arcadeCtx->eventId);
        break;
    case 14:
        *out = BattleArcade_IncBattleNo(arcadeCtx);
        break;
    case 15:
        if (var0 == 0) {
            mon = Party_GetMonByIndex(arcadeCtx->playerParty, 0);
        } else {
            mon = Party_GetMonByIndex(arcadeCtx->opponentParty, 0);
        }
        *out = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        break;
    case 16:
        *out = arcadeCtx->eventId;
        break;
    case 18:
        party = SaveArray_Party_Get(args->saveData);
        for (i = 0; i < 3; i++) {
            mon = Party_GetMonByIndex(party, arcadeCtx->partySlots[i]);
            SetMonData(mon, MON_DATA_HELD_ITEM, &arcadeCtx->heldItems[i]);
        }
        break;
    case 19:
        *out = BattleArcade_GetRound(arcadeCtx);
        break;
    case 20:
        *out = BattleArcade_GetTrainerSprite(arcadeCtx, var0);
        break;
    case 21:
        ov80_022347B8(arcadeCtx);
        break;
    case 22:
        BattleArcade_CompleteRound(arcadeCtx);
        break;
    case 23:
        *out = BattleArcade_GetBattleNo(arcadeCtx);
        break;
    case 24:
        *out = arcadeCtx->unkA74;
        break;
    case 25:
        if (arcadeCtx->eventId == 0x1b) {
            VecFx32 playerMatrix;
            VecFx32 opponentMatrix;
            u8 playerMonCnt = BattleArcade_GetMonCount(arcadeCtx->type, 1);
            u8 opponentMonCnt = BattleArcade_GetOpponentMonCount(arcadeCtx->type, 1);

            for (i = 0; i < playerMonCnt; i++) {
                Sprite *playerSprite = arcadeCtx->playerMonSprites[i]->sprite;
                playerMatrix = *(Sprite_GetMatrixPtr(playerSprite));

                Sprite *opponentSprite = arcadeCtx->opponentMonSprites[i]->sprite;
                opponentMatrix = *(Sprite_GetMatrixPtr(opponentSprite));

                Sprite_SetMatrix(playerSprite, &opponentMatrix);
                Sprite_SetMatrix(opponentSprite, &playerMatrix);

                playerSprite = arcadeCtx->playerItemIcons[i]->sprite;
                playerMatrix = *(Sprite_GetMatrixPtr(playerSprite));

                opponentSprite = arcadeCtx->opponentItemIcons[i]->sprite;
                opponentMatrix = *(Sprite_GetMatrixPtr(opponentSprite));

                Sprite_SetMatrix(playerSprite, &opponentMatrix);
                Sprite_SetMatrix(opponentSprite, &playerMatrix);
            }
        }
        break;
    case 6:
        FrontierLink_RegisterCommands(arcadeCtx);
        break;
    case 28:
        *out = BattleArcade_MultiplayerCheck(arcadeCtx->type);
        break;
    case 17:
        *out = arcadeCtx->type;
        break;
    case 29:
        FrontierMap_LoadMonIconResources(frontierMap);
        break;
    case 30:
        FrontierMap_UnloadMonIconResources(frontierMap);
        break;
    case 31:
        BattleArcade_CreateMonSprites(arcadeCtx, frontierMap, var0);

        u8 playerMonCnt = BattleArcade_GetMonCount(arcadeCtx->type, 1);
        u8 opponentMonCnt = BattleArcade_GetOpponentMonCount(arcadeCtx->type, 1);

        if (var0 == 0) {
            for (i = 0; i < playerMonCnt; i++) {
                Sprite_SetPriority(arcadeCtx->playerMonSprites[i]->sprite, 2);
            }
        } else {
            for (i = 0; i < opponentMonCnt; i++) {
                Sprite_SetPriority(arcadeCtx->opponentMonSprites[i]->sprite, 2);
            }
        }
        break;
    case 32:
        BattleArcade_DeleteMonSprites(arcadeCtx, frontierMap, var0);
        break;
    case 33:
        FrontierMap_LoadItemIconResources(frontierMap);
        break;
    case 34:
        FrontierMap_UnloadItemIconResources(frontierMap);
        break;
    case 35:
        BattleArcade_CreateItemIcons(arcadeCtx, frontierMap, var0);
        break;
    case 36:
        BattleArcade_DeleteItemIcons(arcadeCtx, frontierMap, var0);
        break;
    case 11:
        BattleArcade_LoadEventBg(arcadeCtx, frontierMap);
        break;
    case 37:
        if (BattleArcade_MultiplayerCheck(arcadeCtx->type) == TRUE) {
            BattleArcade_LoadMultiBgScreen(arcadeCtx, frontierMap);
        }
        break;
    case 38:
        if (BattleArcade_GetEventColor(arcadeCtx->eventId) == 0) {
            BufferFrontierOpponentName(ctx->frontierSystem->msgFormat, var0, arcadeCtx->trainerIds[BattleArcade_GetTrainerIndex(arcadeCtx, var1)]);
        } else {
            if (BattleArcade_MultiplayerCheck(arcadeCtx->type) == FALSE) {
                profile = Save_PlayerData_GetProfile(args->saveData);
            } else {
                profile = CommPlayerInfo_GetProfile(var1);
            }
            BufferPlayersName(ctx->frontierSystem->msgFormat, var0, profile);
        }
        break;
    case 39:
        arcadeCtx->mirrorParticles = var1;
        BattleArcade_CreateParticleEmitter(ctx, arcadeCtx, var0);
        break;
    case 40:
        FrontierTrainer_LoadRecord(&arcadeCtx->trainers[0], arcadeCtx->trainerIds[arcadeCtx->battleNo], 0xb, 0xcc);
        FrontierTrainer_LoadRecord(&arcadeCtx->trainers[1], arcadeCtx->trainerIds[arcadeCtx->battleNo + 7], 0xb, 0xcc);
        break;
    case 41:
        BattleArcade_SetPlayerMonSpriteVisible(arcadeCtx, frontierMap, var0, var1);
        break;
    case 42:
        BattleArcade_SetOpponentMonSpriteVisible(arcadeCtx, frontierMap, var0, var1);
        break;
    case 43:
        BattleArcade_LoadBlendedBgExtPalette(var0, var1);
        break;
    case 44:
        if (var0 == 0) {
            PaletteData_BlendPalette(frontierMap->paletteData, PLTTBUF_MAIN_OBJ, 0, 0x100, var0, RGB_BLACK);
        } else {
            FrontierMapObject *object = FrontierMap_FindObject(ctx->frontierSystem->frontierWork, var1);
            u32 palNo = ov42_02229248(object->objectSprite);
            PaletteData_BlendPalette(frontierMap->paletteData, PLTTBUF_MAIN_OBJ, palNo * 0x10, 0x10, var0, RGB_BLACK);
        }
        break;
    case 45:
        *out = ov80_02235324(arcadeCtx);
        break;
    case 46:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        break;
    case 47:
        *out = arcadeCtx->eventNoEffect;
        break;
    case 48:
        *out = 0;

        if (arcadeCtx->type == 0) {
            if (arcadeCtx->winStreak + 1 == 21) {
                *out = 1;
            } else if (arcadeCtx->winStreak + 1 == 49) {
                *out = 2;
            }
        }
        break;
    case 49:
        FrontierMon_GenerateList(arcadeCtx->opponentMons, arcadeCtx->setIds, arcadeCtx->ivs, arcadeCtx->personalities, NULL, 4, 0xb, 0xcd);
        break;
    case 50:
        BattleArcade_SetOpponentParty(arcadeCtx);
        break;
    case 51:
        *out = arcadeCtx->unk1E;
        arcadeCtx->unk1E = 1;
        break;
    case 52:
        arcadeCtx->linkMessageCount = 0;
        break;
    }

    return FALSE;
}

static void BattleArcade_CreateParticleEmitter(FrontierScriptContext *ctx, ArcadeContext *arcadeCtx, u32 a2) {
    FrontierMap *map = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    ParticleSystem_CreateEmitterWithParam(FrontierParticles_GetSystem(map->particleManager, 0), a2, BattleArcade_ParticleEmitterCallback, arcadeCtx);
}

extern const VecFx32 ov80_0223BE6C; //{-1, 0, 0}
static void BattleArcade_ParticleEmitterCallback(SPLEmitter *emitter) {
    VecFx16 axis;
    ArcadeContext *ctx = ParticleSystem_GetEmitterParam();
    const VecFx32 pos = ov80_0223BE6C;

    if (ctx->mirrorParticles == 1) {
        ParticleEmitter_GetAxis(emitter, &axis);
        axis.x *= -1;
        SPL_SetEmitterAxis(emitter, &axis);
        SPL_SetEmitterPosition(emitter, &pos);
    }
}

BOOL FrtCmd_ArcadeGetBattleResult(FrontierScriptContext *ctx) {
    u16 *var = FrontierScript_ReadVarPtr(ctx);

    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);
    *var = arcadeCtx->battleWon;

    return FALSE;
}

BOOL FrtCmd_ArcadeSendBuffer(FrontierScriptContext *ctx) {
    u32 unk0 = FrontierScript_ReadVar(ctx);
    u32 unk1 = FrontierScript_ReadVar(ctx);
    u16 *ret = FrontierScript_ReadVarPtr(ctx);

    ArcadeContext *arcadeContxt = Frontier_GetData(ctx->frontierSystem->frontierWork);
    *ret = BattleArcade_SendBuffer(arcadeContxt, unk0, unk1);

    return TRUE;
}

BOOL FrtCmd_ArcadeReceiveBuffer(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitArcadeLink);
    return TRUE;
}

static BOOL FrontierScriptNative_WaitArcadeLink(FrontierScriptContext *ctx) {
    FrontierScript_GetVar(ctx, ctx->data[0]);
    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);

    if (arcadeCtx->linkMessageCount >= 2) {
        arcadeCtx->linkMessageCount = 0;
        return TRUE;
    }

    return FALSE;
}

BOOL FrtCmd_ArcadePrintMsg(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 index = FrontierScript_ReadByte(ctx);
    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);

    if (arcadeCtx == NULL) {
        return FALSE;
    }

    u16 *msg = arcadeCtx->trainers[index].unk18;
    FrontierScript_PrintMessageEntry(ctx, msg);

    return TRUE;
}

BOOL FrtCmd_ArcadeSetEvent(FrontierScriptContext *ctx) {
    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleArcade_ApplyEvent(arcadeCtx, arcadeCtx->eventId);
    return TRUE;
}

BOOL FrtCmd_198(FrontierScriptContext *ctx) {
    FrontierMap *map = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    u32 var0 = FrontierScript_ReadVar(ctx);
    u32 var1 = FrontierScript_ReadVar(ctx);
    u32 var2 = FrontierScript_ReadVar(ctx);
    ArcadeContext *arcadeCtx = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleArcade_SetItemIconVisible(arcadeCtx, map, var0, var1, var2);
    return FALSE;
}
