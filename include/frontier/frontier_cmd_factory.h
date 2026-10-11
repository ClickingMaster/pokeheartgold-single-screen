#ifndef POKEHEARTGOLD_FRONTIER_CMD_FACTORY_H
#define POKEHEARTGOLD_FRONTIER_CMD_FACTORY_H

#include "frontier/frontier_script_context.h"

BOOL FrtCmd_FactoryAlloc(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryInit(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryFree(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryLaunchRentalApp(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryEndBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryStartBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryLaunchTradeApp(FrontierScriptContext *ctx);
BOOL FrtCmd_FactorySetRentalParty(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryGenerateOpponent(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryTradeMon(FrontierScriptContext *ctx);
BOOL FrtCmd_FactorySetOpponentParty(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryAction(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryGetBattleResult(FrontierScriptContext *ctx);
BOOL FrtCmd_FactorySendBuffer(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryReceiveBuffer(FrontierScriptContext *ctx);
BOOL FrtCmd_FactoryPrintMsg(FrontierScriptContext *ctx);

#endif // POKEHEARTGOLD_FRONTIER_CMD_FACTORY_H
