#include "global.h"

#include "pokeathlon/pokeathlon_save.h"

#include "coins.h"
#include "money_box.h"
#include "scrcmd.h"
#include "script.h"
#include "script_manager.h"

extern Window *PointsBox_New(struct FieldSystem *fieldSystem, u8 type, u8 x, u8 y);

BOOL ScrCmd_ShowPointsBox(ScriptContext *ctx) {
    u8 type = ScriptReadByte(ctx);
    u16 x = ScriptGetVar(ctx);
    u16 y = ScriptGetVar(ctx);
    Window **window = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_POINTS_BOX);
    *window = PointsBox_New(ctx->fieldSystem, type, x, y);
    return FALSE;
}

BOOL ScrCmd_HidePointsBox(ScriptContext *ctx) {
    Window **window = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_POINTS_BOX);
    PointsBox_Delete(*window);
    return FALSE;
}

BOOL ScrCmd_UpdatePointsBox(ScriptContext *ctx) {
    u8 type = ScriptReadByte(ctx);
    Window **window = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_POINTS_BOX);
    PointsBox_Update(ctx->fieldSystem, *window, type);
    return FALSE;
}

BOOL ScrCmd_GetCoinAmount(ScriptContext *ctx) {
    u16 *ptr = ScriptGetVarPointer(ctx);
    *ptr = Coins_GetValue(Save_PlayerData_GetCoinsAddr(ctx->fieldSystem->saveData));
    return FALSE;
}

BOOL ScrCmd_GiveCoins(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 amount = ScriptGetVar(ctx);
    Coins_Add(Save_PlayerData_GetCoinsAddr(fieldSystem->saveData), amount);
    return FALSE;
}

BOOL ScrCmd_TakeCoins(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 amount = ScriptGetVar(ctx);
    Coins_Subtract(Save_PlayerData_GetCoinsAddr(fieldSystem->saveData), amount);
    return FALSE;
}

BOOL ScrCmd_TakeCoinsVar(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *ptr = ScriptGetVarPointer(ctx);
    Coins_Subtract(Save_PlayerData_GetCoinsAddr(fieldSystem->saveData), *ptr);
    return FALSE;
}

BOOL ScrCmd_CheckCoinsImmediate(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *ptr = ScriptGetVarPointer(ctx);
    u32 amount = ScriptReadWord(ctx);
    u16 coins = Coins_GetValue(Save_PlayerData_GetCoinsAddr(fieldSystem->saveData));
    if (coins < amount) {
        *ptr = FALSE;
    } else {
        *ptr = TRUE;
    }
    return FALSE;
}

BOOL ScrCmd_CheckCoinsVar(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *resultPtr = ScriptGetVarPointer(ctx);
    u16 *amountPtr = ScriptGetVarPointer(ctx);
    u16 coins = Coins_GetValue(Save_PlayerData_GetCoinsAddr(fieldSystem->saveData));
    if (coins < *amountPtr) {
        *resultPtr = FALSE;
    } else {
        *resultPtr = TRUE;
    }
    return FALSE;
}

BOOL ScrCmd_CheckGiveCoins(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *ptr = ScriptGetVarPointer(ctx);
    u16 amount = ScriptGetVar(ctx);
    *ptr = Coins_CanAdd(Save_PlayerData_GetCoinsAddr(fieldSystem->saveData), amount);
    return FALSE;
}

BOOL ScrCmd_GiveAthletePoints(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 amount = ScriptGetVar(ctx);
    PokeathlonSave_AddAthletePoints(Save_Pokeathlon_Get(fieldSystem->saveData), amount);
    return FALSE;
}

BOOL ScrCmd_TakeAthletePoints(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 amount = ScriptGetVar(ctx);
    PokeathlonSave_SubAthletePoints(Save_Pokeathlon_Get(fieldSystem->saveData), amount);
    return FALSE;
}

BOOL ScrCmd_CheckAthletePoints(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *ptr = ScriptGetVarPointer(ctx);
    u16 amount = ScriptGetVar(ctx);
    u32 points = PokeathlonSave_GetAthletePoints(Save_Pokeathlon_Get(fieldSystem->saveData));
    if (points < amount) {
        *ptr = FALSE;
    } else {
        *ptr = TRUE;
    }
    return FALSE;
}
