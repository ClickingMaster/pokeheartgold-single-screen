#ifndef POKEHEARTGOLD_FRONTIER_CMD_HALL_H
#define POKEHEARTGOLD_FRONTIER_CMD_HALL_H

#include "frontier/frontier_script_context.h"

BOOL FrtCmd_HallAlloc(FrontierScriptContext *ctx);
BOOL FrtCmd_HallInit(FrontierScriptContext *ctx);
BOOL FrtCmd_HallFree(FrontierScriptContext *ctx);
BOOL FrtCmd_HallLaunchApp(FrontierScriptContext *ctx);
BOOL FrtCmd_HallEndBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_HallStartBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_HallAction(FrontierScriptContext *ctx);
BOOL FrtCmd_HallGetBattleResult(FrontierScriptContext *ctx);
BOOL FrtCmd_HallSendLinkMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_HallWaitLink(FrontierScriptContext *ctx);
BOOL FrtCmd_HallPrintTrainerMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_HallUpdateSpeciesRecord(FrontierScriptContext *ctx);
BOOL FrtCmd_Dummy199(FrontierScriptContext *ctx);

#endif // POKEHEARTGOLD_FRONTIER_CMD_HALL_H
