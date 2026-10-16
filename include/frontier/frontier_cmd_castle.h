#ifndef POKEHEARTGOLD_FRONTIER_CMD_CASTLE_H
#define POKEHEARTGOLD_FRONTIER_CMD_CASTLE_H

#include "frontier/frontier_script_context.h"

BOOL FrtCmd_CastleAlloc(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleInit(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleFree(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleLaunchPartyApp(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleLaunchEmptyApp(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleEndBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleStartBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleLaunchOpponentApp(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleSetPartyBeforeBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleGenerateOpponents(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleAction(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleGetBattleResult(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleSendLinkMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleWaitLink(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleWaitLinkChoice(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleShowPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleHidePoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastlePrintPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleGetPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleSpendPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastleAddPoints(FrontierScriptContext *ctx);
BOOL FrtCmd_CastlePrintTrainerMessage(FrontierScriptContext *ctx);

#endif // POKEHEARTGOLD_FRONTIER_CMD_CASTLE_H
