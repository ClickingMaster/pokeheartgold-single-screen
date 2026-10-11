#ifndef POKEHEARTGOLD_OVERLAY_80_0222BDF4_H
#define POKEHEARTGOLD_OVERLAY_80_0222BDF4_H

#include "frontier/frontier_script_context.h"

#include "message_format.h"

u8 inline FrontierScript_ReadShort(FrontierScriptContext *ctx) {
    return *(ctx->scriptPtr++);
}

typedef BOOL (*FrontierFunction)(FrontierScriptContext *);

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u32 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierMap_LoadMonIconResources(FrontierMap *map);
void FrontierMap_UnloadMonIconResources(FrontierMap *map);
void ov80_0222F33C(FrontierMap *map);
void ov80_0222F3CC(FrontierMap *map);
void FrontierScript_GetVar(FrontierScriptContext *ctx, u16 a1);
void FrontierScript_PrintMessageEntry(FrontierScriptContext *ctx, u16 *a1);

#endif
