#ifndef POKEHEARTGOLD_FRONTIER_CMD_ARCADE
#define POKEHEARTGOLD_FRONTIER_CMD_ARCADE

#include "frontier/frontier_script_context.h"
#include "frontier/frontier_script_commands.h"
#include "frontier/battle_arcade_data.h"

#include "battle_arcade_game_board.h"

// Defined in battle_arcade_data.c with u8 slot0, u16 slot1 and u16 slot2; this caller passes full words.
u32 BattleArcadeData_Alloc(SaveData *saveData, u32 resume, u8 type, u32 slot0, u32 slot1, u32 slot2, u16 *resultVar);

BOOL FrtCmd_ArcadeAlloc(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeInit(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeFree(FrontierScriptContext *ctx);
BOOL FrtCmd_LaunchGameBoard(FrontierScriptContext *ctx);
BOOL FrtCmd_187(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeStartBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeSetPartyBeforeBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeSetPartyAfterBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeAction(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeGetBattleResult(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeSendBuffer(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeReceiveBuffer(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadePrintMsg(FrontierScriptContext *ctx);
BOOL FrtCmd_ArcadeSetEvent(FrontierScriptContext *ctx);
BOOL FrtCmd_198(FrontierScriptContext *ctx);

#endif
