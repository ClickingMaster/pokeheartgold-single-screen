#ifndef POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_H
#define POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_H

#include "frontier/frontier_script_context.h"

#include "message_format.h"

u8 inline FrontierScript_ReadByte(FrontierScriptContext *ctx) {
    return *(ctx->scriptPtr++);
}

typedef BOOL (*FrontierFunction)(FrontierScriptContext *);

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierMap_LoadMonIconResources(FrontierMap *map);
void FrontierMap_UnloadMonIconResources(FrontierMap *map);
void FrontierMap_LoadItemIconResources(FrontierMap *map);
void FrontierMap_UnloadItemIconResources(FrontierMap *map);
u16 FrontierScript_GetVar(FrontierScriptContext *ctx, u16 index);
void FrontierScript_PrintMessageEntry(FrontierScriptContext *ctx, const u16 *entry);

#endif
