#include "field_system.h"
#include "scrcmd.h"
#include "apricorn_box.h"
#include "field_time_events.h"

#include "field/apricorn_tree_anim.h"

BOOL ScrCmd_AnimApricornTree(ScriptContext *ctx) {
    u16 varId = ScriptReadHalfword(ctx);
    LocalMapObject **lastInteracted = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_LAST_INTERACTED);
    u16 *retPtr = GetVarPointer(ctx->fieldSystem, varId);
    FieldSystem_AnimApricornTree(ctx->fieldSystem, *lastInteracted, retPtr);
    return TRUE;
}

BOOL ScrCmd_ApricornTreeGetApricorn(ScriptContext *ctx) {
    u16 varId = ScriptReadHalfword(ctx);
    LocalMapObject **lastInteracted = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_LAST_INTERACTED);
    u16 *retPtr = GetVarPointer(ctx->fieldSystem, varId);
    *retPtr = FieldSystem_ApricornTree_GetApricorn(ctx->fieldSystem, *lastInteracted);
    return FALSE;
}

BOOL ScrCmd_GiveApricornFromTree(ScriptContext *ctx) {
    u16 apricornArg = ScriptGetVar(ctx);
    u8 quantity = ScriptGetVar(ctx);
    u16 *retPtr = ScriptGetVarPointer(ctx);
    SaveApricornBox *apricornBox = Save_ApricornBox_Get(ctx->fieldSystem->saveData);
    u8 apricorn = apricornArg;
    int apricornCount = ApricornBox_CountApricorn(apricornBox, apricorn);

    if ((quantity + apricornCount) <= 99) {
        ApricornBox_GiveApricorn(apricornBox, apricorn, quantity);
        *retPtr = TRUE;
    } else {
        *retPtr = FALSE;
    }
    return FALSE;
}
