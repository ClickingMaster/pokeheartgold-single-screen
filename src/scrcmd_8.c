#include "friend_group.h"
#include "launch_application.h"
#include "math_util.h"
#include "scrcmd.h"
#include "script_manager.h"
#include "friend_group_access.h"

BOOL ScrCmd_GetStaticEncounterOutcome(ScriptContext *ctx) {
    u32 *winFlag = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_BATTLE_WIN_FLAG);
    u16 *variable = ScriptGetVarPointer(ctx);
    *variable = *winFlag;
    return TRUE;
}

BOOL ScrCmd_FriendGroupAction(ScriptContext *ctx) {
    MessageFormat **msg = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MESSAGE_FORMAT);
    SAV_FRIEND_GRP *group = Save_FriendGroup_Get(ctx->fieldSystem->saveData);
    SaveData *save = ctx->fieldSystem->saveData;

    u16 var = ScriptReadHalfword(ctx);
    switch (var) {
    case 0: {
        u16 groupIndex = ScriptGetVar(ctx);
        u16 *retPtr = ScriptGetVarPointer(ctx);
        *retPtr = Save_FriendGroup_IsGroupValid(group, groupIndex);
        return FALSE;
    }
    case 1: {
        u16 groupIndex = ScriptGetVar(ctx);
        u16 *retPtr = ScriptGetVarPointer(ctx);
        *retPtr = Save_FriendGroup_IsCurrentGroup(group, groupIndex);
        return FALSE;
    }
    case 2: {
        u16 groupId = ScriptGetVar(ctx);
        u16 fieldNo = ScriptGetVar(ctx);
        BufferGroupName(*msg, save, groupId, fieldNo, 0);
        break;
    }
    case 3: {
        u16 groupId = ScriptGetVar(ctx);
        u16 fieldNo = ScriptGetVar(ctx);
        BufferGroupName(*msg, save, groupId, fieldNo, 1);
        break;
    }
    case 4: {
        u16 *r5 = Save_FriendGroup_GetName(group, 0, 0);
        u16 *retPtr = ScriptGetVarPointer(ctx);
        CallTask_NamingScreen(ctx->taskman, NAME_SCREEN_GROUP, 0, PLAYER_NAME_LENGTH, 0, r5, retPtr);
        return TRUE;
    }
    case 5: {
        u16 groupIndex = ScriptGetVar(ctx);
        Save_FriendGroup_IsGroupValid(group, 1);
        Save_FriendGroup_CopyGroup(group, groupIndex, 1);
        return FALSE;
    }
    case 6: {
        String *str = String_New(64, HEAP_ID_FIELD3);
        PlayerProfile *profile = Save_PlayerData_GetProfile(ctx->fieldSystem->saveData);
        PlayerName_FlatToString(profile, str);
        Save_FriendGroup_SetName(group, 0, 1, str);
        Save_FriendGroup_SetLeaderGender(group, 0, PlayerProfile_GetTrainerGender(profile));
        Save_FriendGroup_SetLanguage(group, 0, 2);
        Save_FriendGroup_SetGroupId(group, 0, MTRandom());
        String_Delete(str);
        Save_FriendGroup_CopyGroup(group, 0, 1);
        break;
    }
    case 7: {
        u16 *retPtr = ScriptGetVarPointer(ctx);
        u16 i, count;
        for (count = 0, i = 0; i < FGRP_MAX; i++) {
            if (Save_FriendGroup_IsGroupValid(group, i) && Save_FriendGroup_IsCurrentGroup(group, i) != TRUE) {
                count++;
            }
        }

        if (count >= 4) {
            *retPtr = TRUE;
        } else {
            *retPtr = FALSE;
        }
    }
    }

    return FALSE;
}
