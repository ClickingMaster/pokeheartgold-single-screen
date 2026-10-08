#pragma force_active on
#include "global.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_map.h"
#include "sprite_system.h"

u32 FrontierScript_ReadVar(FrontierScriptContext *ctx);
FrontierMap *sub_0209680C(void *owner);
ManagedSprite *ov80_0223968C(FrontierMap *map, u32 idx, u32 id);
void ov80_02239708(FrontierMap *map, u32 idx, u32 on);
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
        sprite = ov80_0223968C(map, idx, id);
        ManagedSprite_SetPositionXY(sprite, x, y);
        ManagedSprite_SetDrawFlag(sprite, draw);
        ov80_02239708(map, idx, on);
    }
    ctx->scriptPtr = saved;
    return FALSE;
}
