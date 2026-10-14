#include "frontier_script_commands_misc_internal.h"

#include "constants/game_stats.h"
#include "constants/sndseq.h"

BOOL FrtCmd_StopBGM(FrontierScriptContext *ctx) {
    FrontierScriptContext_ReadHalfWord(ctx);
    StopBGM(GF_GetCurrentPlayingBGM(), 0);
    return FALSE;
}

BOOL FrtCmd_TowerAction(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u16 command = FrontierScriptContext_ReadHalfWord(ctx);
    u32 value = FrontierScript_ReadVar(ctx);
    u16 *dst = FrontierScript_ReadVarPtr(ctx);
    u8 *data = Frontier_GetData(ctx->frontierSystem->frontierWork);

    switch (command) {
    case 2:
        ResetSystem();
        break;
    case 0x2E:
        *dst = FrontierFieldSystem_AwardTowerBattlePoints(data);
        GameStats_Add(Save_GameStats_Get(args->saveData), GAME_STAT_BATTLE_POINTS_RECEIVED, *dst);
        break;
    case 0x37:
        *dst = ov80_0223558C(data, (u8)value);
        break;
    case 0x21:
        *dst = FrontierFieldSystem_GetFrontierBattleNumber(data);
        break;
    case 0x22:
        *dst = BattleTower_IncBattleNumber(data);
        break;
    case 0x23:
        *dst = FrontierFieldSystem_HasWonSet(data);
        break;
    case 0x24:
        *dst = Overlay80Counter_GetSumCapped(data);
        break;
    case 0x2B:
        *dst = FrontierFieldSystem_GetBattleTowerMode(data);
        break;
    case 0x28:
        BattleTower_GenerateOpponents(data, args->saveData);
        break;
    case 0x29:
        *dst = BattleTower_GetTrainerSprite(data, value);
        break;
    case 0x2C:
        BattleTower_SetPalmerDefeated(data, value);
        break;
    case 0x25:
        FrontierFieldSystem_RecordSetLost(data, args->saveData);
        break;
    case 0x26:
        FrontierFieldSystem_RecordSetWon(data, args->saveData);
        break;
    case 0x27:
        FrontierFieldSystem_SaveChallenge(data);
        break;
    case 0x3A:
        MI_CpuFill8(data + 0x884, 0, 0x46);
        break;
    case 0x3B:
        data[0x8D6] = 1;
        break;
    case 0x64:
        if (data == NULL) {
            *dst = 1;
        } else {
            *dst = 0;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

u32 ov80_0223558C(void *data, u32 mode) {
    u8 *p = data;
    if (mode == 2) {
        return ((u32)p[0x10] << 24) >> 29;
    }
    if (mode == 1) {
        if (p[0xF] == 2) {
            return ov80_0223C034[((u32)p[0x10] << 24) >> 29];
        }
        if (p[0x12] != 0) {
            return 0x61;
        }
        return 0;
    }
    if (p[0x11] != 0) {
        return 0x61;
    }
    return 0;
}

BOOL FrtCmd_TowerPrintTrainerMessage(FrontierScriptContext *ctx) {
    u8 arg;
    u8 *data;
    Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    arg = FrontierScriptContext_ReadByte(ctx);
    data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (data == NULL) {
        return FALSE;
    }
    FrontierScript_PrintMessageEntryFromBank(ctx, data + 0x90 + arg * 0x110, BattleTower_GetTrainerMessageBank(data[0xF]));
    return TRUE;
}

BOOL FrtCmd_TowerFreeData(FrontierScriptContext *ctx) {
    FrontierFieldSystem_Free(Frontier_GetData(ctx->frontierSystem->frontierWork));
    return FALSE;
}

BOOL FrtCmd_TowerStartBattle(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    u8 *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    BattleSetup *setup = BattleTower_NewBattleSetup(data, args);
    *(void **)(data + 0x8D0) = setup;
    Sound_SetSceneAndPlayBGM(5, SEQ_GS_VS_TRAINER, 1);
    Frontier_LaunchApplication(ctx->frontierSystem->frontierWork, &gOverlayTemplate_Battle, setup, 0, 0);
    return TRUE;
}

BOOL FrtCmd_TowerEndBattle(FrontierScriptContext *ctx) {
    u8 *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    u16 *dst = FrontierScript_ReadVarPtr(ctx);
    BattleSetup *setup = *(void **)(data + 0x8D0);
    *(u32 *)(data + 0x8CC) = IsBattleResultWin(setup->winFlag);
    *dst = *(u32 *)(data + 0x8CC);
    BattleSetup_Delete(setup);
    return FALSE;
}

BOOL FrtCmd_TowerSendLinkMessage(FrontierScriptContext *ctx) {
    u32 kind;
    u32 value;
    u8 *data;
    u16 *dst;
    Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    kind = FrontierScript_ReadVar(ctx);
    value = FrontierScript_ReadVar(ctx);
    dst = FrontierScript_ReadVarPtr(ctx);
    data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    if (kind == 2) {
        BattleTowerSetup_SetLinkFlag(data, value);
    } else {
        GF_AssertFail();
    }
    if (LinkSync_SendPacket(Comm_GetLocalNetId(), data + 0x83E) == 1) {
        *dst = 1;
    } else {
        *dst = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_TowerWaitLink(FrontierScriptContext *ctx) {
    u16 first = FrontierScriptContext_ReadHalfWord(ctx);
    u16 second = FrontierScriptContext_ReadHalfWord(ctx);
    ctx->data[0] = first;
    ctx->data[1] = second;
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitPartnerPacket);
    return TRUE;
}

BOOL FrontierScriptNative_WaitPartnerPacket(FrontierScriptContext *ctx) {
    FrontierLaunchArgs *args = Frontier_GetLaunchArgs(ctx->frontierSystem->frontierWork);
    void *data = Frontier_GetData(ctx->frontierSystem->frontierWork);
    return FrontierScript_ReadPartnerPacket(ctx, data, args->saveData, ctx->data[0], ctx->data[1]) == 1;
}

BOOL FrontierScript_ReadPartnerPacket(FrontierScriptContext *ctx, void *data, void *fieldSystem, u32 mode, u16 value) {
    void *remote = LinkSync_GetPacket(1 - Comm_GetLocalNetId());
    u16 *dst;
    if (remote == NULL) {
        return FALSE;
    }
    dst = FrontierScript_GetVarPtr(ctx, value);
    if (mode == 2) {
        *dst = BattleTowerSetup_IsLinkFlagSetByEither(data, remote);
    } else {
        GF_AssertFail();
    }
    return TRUE;
}
