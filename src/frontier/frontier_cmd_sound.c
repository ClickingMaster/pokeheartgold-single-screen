#include "frontier/frontier_cmd_sound.h"

#include "global.h"

#include "frontier/frontier_script_context.h"

#include "sound.h"
#include "sound_playback.h"
#include "sound_system.h"

u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);

static BOOL FrontierScriptNative_WaitSE(FrontierScriptContext *ctx);
static BOOL FrontierScriptNative_WaitFanfare(FrontierScriptContext *ctx);

BOOL FrtCmd_PlaySE(FrontierScriptContext *ctx) {
    PlaySE(FrontierScript_ReadVar(ctx));
    return FALSE;
}

BOOL FrtCmd_StopSE(FrontierScriptContext *ctx) {
    StopSE(FrontierScript_ReadVar(ctx), 0);
    return FALSE;
}

BOOL FrtCmd_WaitSE(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitSE);
    return TRUE;
}

static BOOL FrontierScriptNative_WaitSE(FrontierScriptContext *ctx) {
    if (!IsSEPlaying(ctx->data[0])) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_PlayFanfare(FrontierScriptContext *ctx) {
    PlayFanfare(FrontierScriptContext_ReadHalfWord(ctx));
    return FALSE;
}

BOOL FrtCmd_WaitFanfare(FrontierScriptContext *ctx) {
    FrontierScriptContext_Pause(ctx, FrontierScriptNative_WaitFanfare);
    return TRUE;
}

static BOOL FrontierScriptNative_WaitFanfare(FrontierScriptContext *ctx) {
    if (!IsFanfarePlaying()) {
        return TRUE;
    }
    return FALSE;
}

BOOL FrtCmd_PlayBGM(FrontierScriptContext *ctx) {
    u32 seqNo = FrontierScriptContext_ReadHalfWord(ctx);
    Sound_SetFieldBGM(seqNo);
    PlayBGM(seqNo);
    return FALSE;
}
