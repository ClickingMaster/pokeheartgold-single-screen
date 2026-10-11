#include "global.h"

#include "frontier/frontier_script_context.h"

#include "sound.h"
#include "sound_playback.h"
#include "sound_system.h"

// Sound script commands: sound effects, fanfares and the background music.

// Defined in frontier_script_commands.c.
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);

BOOL FrtCmd_085(FrontierScriptContext *ctx);
BOOL FrtCmd_086(FrontierScriptContext *ctx);
BOOL FrtCmd_087(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitSE(FrontierScriptContext *ctx);
BOOL FrtCmd_088(FrontierScriptContext *ctx);
BOOL FrtCmd_089(FrontierScriptContext *ctx);
BOOL FrontierScriptNative_WaitFanfare(FrontierScriptContext *ctx);
BOOL FrtCmd_090(FrontierScriptContext *ctx);

BOOL FrtCmd_085(FrontierScriptContext *ctx) {
    PlaySE(FrontierScript_ReadVar(ctx));
    return FALSE;
}

BOOL FrtCmd_086(FrontierScriptContext *ctx) {
    StopSE(FrontierScript_ReadVar(ctx), 0);
    return FALSE;
}

BOOL FrtCmd_087(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitSE);
    return TRUE;
}

BOOL FrontierScriptNative_WaitSE(FrontierScriptContext *ctx) {
    if (!IsSEPlaying(ctx->data[0])) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_088(FrontierScriptContext *ctx) {
    PlayFanfare(FrontierScriptContext_ReadHalfWord(ctx));
    return FALSE;
}

BOOL FrtCmd_089(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitFanfare);
    return TRUE;
}

BOOL FrontierScriptNative_WaitFanfare(FrontierScriptContext *ctx) {
    if (!IsFanfarePlaying()) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_090(FrontierScriptContext *ctx) {
    u32 seqNo = FrontierScriptContext_ReadHalfWord(ctx);
    Sound_SetFieldBGM(seqNo);
    PlayBGM(seqNo);
    return FALSE;
}
