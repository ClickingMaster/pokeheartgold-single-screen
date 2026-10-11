#pragma force_active on
#include "global.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_map.h"
#include "sprite_system.h"

u32 FrontierScript_ReadVar(FrontierScriptContext *ctx);
FrontierMap *sub_0209680C(void *owner);
ManagedSprite *FrontierMap_CreateSprite(FrontierMap *map, u32 idx, u32 id);
void FrontierMap_SetSpriteAnimActive(FrontierMap *map, u32 idx, u32 on);
BOOL FrtCmd_044(FrontierScriptContext *ctx);

static inline u8 ReadByte(FrontierScriptContext *ctx) {
    return *(ctx->scriptPtr++);
}

// @fn FrtCmd_044
BOOL FrtCmd_044(FrontierScriptContext *ctx) {
    u32 draw, y, x;
    u16 on; u32 idx;
    const u8 *saved;
    u32 id;
    ManagedSprite *sprite;
    FrontierMap *map = sub_0209680C(ctx->frontierSystem->unk0);
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
        on = ReadByte(ctx);
        idx = ReadByte(ctx);
        sprite = FrontierMap_CreateSprite(map, idx, id);
        ManagedSprite_SetPositionXY(sprite, x, y);
        ManagedSprite_SetDrawFlag(sprite, draw);
        FrontierMap_SetSpriteAnimActive(map, idx, on);
    }
    ctx->scriptPtr = saved;
    return FALSE;
}
