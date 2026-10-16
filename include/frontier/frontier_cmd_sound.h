#ifndef POKEHEARTGOLD_FRONTIER_CMD_SOUND_H
#define POKEHEARTGOLD_FRONTIER_CMD_SOUND_H

#include "frontier/frontier_script_context.h"

BOOL FrtCmd_PlaySE(FrontierScriptContext *ctx);
BOOL FrtCmd_StopSE(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitSE(FrontierScriptContext *ctx);
BOOL FrtCmd_PlayFanfare(FrontierScriptContext *ctx);
BOOL FrtCmd_WaitFanfare(FrontierScriptContext *ctx);
BOOL FrtCmd_PlayBGM(FrontierScriptContext *ctx);

#endif // POKEHEARTGOLD_FRONTIER_CMD_SOUND_H
