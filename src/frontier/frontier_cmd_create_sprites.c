#include "global.h"

#include "frontier/frontier_map.h"
#include "frontier/frontier_script_context.h"

#include "sprite_system.h"

// This command is compiled with MWCC 2.0/sp2 (see Makefile), so it stays in its own file.

u32 FrontierScript_ReadVar(FrontierScriptContext *ctx);
FrontierMap *Frontier_GetMap(void *frontierWork);
ManagedSprite *FrontierMap_CreateSprite(FrontierMap *map, u32 idx, u32 id);
void FrontierMap_SetSpriteAnimActive(FrontierMap *map, u32 idx, u32 on);
BOOL FrtCmd_CreateSprites(FrontierScriptContext *ctx);

static inline u8 ReadScriptByte(FrontierScriptContext *ctx) {
    return *(ctx->scriptPtr++);
}

#pragma force_active on
// Creates the extra map sprites of a list: sprite id, x, y, draw flag, animation flag, slot; ends with 0xFD13.
BOOL FrtCmd_CreateSprites(FrontierScriptContext *ctx) {
    u32 draw, y, x;
    u16 animActive;
    u32 slot;
    const u8 *saved;
    u32 id;
    ManagedSprite *sprite;
    FrontierMap *map = Frontier_GetMap(ctx->frontierSystem->frontierWork);
    u32 offset = FrontierScriptContext_ReadWord(ctx);

    saved = ctx->scriptPtr;
    ctx->scriptPtr = saved + offset;
    while (TRUE) {
        id = FrontierScript_ReadVar(ctx);
        if (id == 0xFD13) {
            break;
        }
        x = FrontierScript_ReadVar(ctx);
        y = FrontierScript_ReadVar(ctx);
        draw = FrontierScript_ReadVar(ctx);
        animActive = ReadScriptByte(ctx);
        slot = ReadScriptByte(ctx);
        sprite = FrontierMap_CreateSprite(map, slot, id);
        ManagedSprite_SetPositionXY(sprite, x, y);
        ManagedSprite_SetDrawFlag(sprite, draw);
        FrontierMap_SetSpriteAnimActive(map, slot, animActive);
    }
    ctx->scriptPtr = saved;
    return FALSE;
}
