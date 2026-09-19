#include "to52_r6_w19_overlay80_r6_private.h"

BOOL ov80_0222C33C(FrontierScriptContext *ctx) {
    FrontierSystemR6W19 *system = (FrontierSystemR6W19 *)ctx->frontierSystem;
    u16 *value = ov80_0222BE24(ctx, ctx->data[0]);

    if (system->unk60 != NULL) {
        return FALSE;
    }
    if (*value != 0xEEEE) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_026(FrontierScriptContext *ctx) {
    FrontierSystemR6W19 *system = (FrontierSystemR6W19 *)ctx->frontierSystem;
    u8 a = FrontierScriptContext_ReadByte(ctx);
    u8 b = FrontierScriptContext_ReadByte(ctx);
    u8 c = FrontierScriptContext_ReadByte(ctx);
    u8 d = FrontierScriptContext_ReadByte(ctx);
    u16 id = FrontierScriptContext_ReadHalfWord(ctx);
    u16 *value = ov80_0222BE24(ctx, id);

    system->unk60 = ov80_0222E920(system, a, b, c, d, value, system->unk44, NULL);
    ctx->data[0] = id;
    return TRUE;
}

BOOL FrtCmd_027(FrontierScriptContext *ctx) {
    FrontierSystemR6W19 *system = (FrontierSystemR6W19 *)ctx->frontierSystem;
    u8 a = FrontierScriptContext_ReadByte(ctx);
    u8 b = FrontierScriptContext_ReadByte(ctx);
    u8 c = FrontierScriptContext_ReadByte(ctx);
    u8 d = FrontierScriptContext_ReadByte(ctx);
    u16 id = FrontierScriptContext_ReadHalfWord(ctx);
    u16 *value = ov80_0222BE24(ctx, id);

    system->unk60 = ov80_0222E920(system, a, b, c, d, value, system->unk44, ctx->msgLoader);
    ctx->data[0] = id;
    return TRUE;
}
