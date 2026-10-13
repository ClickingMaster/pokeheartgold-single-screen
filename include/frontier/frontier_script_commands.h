#ifndef POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_H
#define POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_H

#include "frontier/frontier_script_context.h"

#include "message_format.h"

u8 inline FrontierScript_ReadByte(FrontierScriptContext *ctx) {
    return *(ctx->scriptPtr++);
}

typedef BOOL (*FrontierFunction)(FrontierScriptContext *);

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u32 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierMap_LoadMonIconResources(FrontierMap *map);
void FrontierMap_UnloadMonIconResources(FrontierMap *map);
void FrontierMap_LoadItemIconResources(FrontierMap *map);
void FrontierMap_UnloadItemIconResources(FrontierMap *map);
void FrontierScript_GetVar(FrontierScriptContext *ctx, u16 a1);
void FrontierScript_PrintMessageEntry(FrontierScriptContext *ctx, u16 *a1);

#endif
