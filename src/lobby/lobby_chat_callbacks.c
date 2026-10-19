#include <string.h>

#include "global.h"

#include "chat_user_mode.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnInvited(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
}

void LobbyChat_OnChannelMessage(void *chat, char *channel, char *nick, void *message, int type) {
    Str nickStr;
    Str channelKey;
    Str fullName;
    Str channelName;
    int userId;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    if (type != 3) {
        return;
    }
    LobbyString_InitFromCStr(&nickStr, nick);
    userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &nickStr);
    LobbyString_Destroy(&nickStr);
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onChannelMessage(StrData(&channelName), userId, message, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
}

void LobbyChat_OnKicked(void *chat, char *channel) {
    Str channelStr;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&channelStr, channel);
    LobbyChat_RemoveChannel(gLobbyChatManager, &channelStr);
    LobbyString_Destroy(&channelStr);
}

// A user ID that is already in the channel is reported as parted before the new join.
void LobbyChat_OnUserJoined(void *chat, const char *channel, const char *nick) {
    Str oldNick;
    Str limitChannel;
    Str nickStr;
    Str oldChannel;
    Str partedKey;
    Str partedFullName;
    Str partedName;
    Str partedNick;
    Str joinedNick;
    Str joinedChannel;
    Str enumChannel;
    Str joinedKey;
    Str joinedFullName;
    Str joinedName;
    int status;
    int index;
    LobbyChannelCallbacks *hooks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&limitChannel, channel);
    status = LobbyChat_GetChannelUserLimit(gLobbyChatManager, &limitChannel);
    LobbyString_Destroy(&limitChannel);
    if (status && chatGetChannelNumUsersA(gLobbyChatManager->connection, channel) > status) {
        return;
    }
    LobbyString_InitFromCStr(&nickStr, nick);
    LobbyChat_GetUserNick(&oldNick, gLobbyChatManager, LobbyChat_GetUserIdFromNick(&nickStr));
    LobbyString_Destroy(&nickStr);
    LobbyString_InitFromCStr(&oldChannel, channel);
    status = LobbyChat_RemoveChannelUser(gLobbyChatManager, &oldNick, &oldChannel);
    LobbyString_Destroy(&oldChannel);
    if (status) {
        LobbyString_InitFromCStr(&partedKey, channel);
        hooks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &partedKey);
        LobbyString_Destroy(&partedKey);
        LobbyString_InitFromCStr(&partedFullName, channel);
        LobbyChat_StripChannelPrefix(&partedName, gLobbyChatManager, &partedFullName);
        LobbyString_InitFromCStr(&partedNick, nick);
        {
            Str *partedNickStr = &partedNick;
            const char *channelName = StrData(&partedName);
            hooks->onUserParted(channelName, LobbyChat_GetUserIdFromNick(partedNickStr), hooks->param);
        }
        LobbyString_Destroy(&partedNick);
        LobbyString_Destroy(&partedName);
        LobbyString_Destroy(&partedFullName);
    }
    LobbyString_InitFromCStr(&joinedNick, nick);
    LobbyString_InitFromCStr(&joinedChannel, channel);
    index = LobbyChat_AddChannelUser(gLobbyChatManager, &joinedNick, &joinedChannel);
    LobbyString_Destroy(&joinedChannel);
    LobbyString_Destroy(&joinedNick);
    if (index == -1) {
        LobbyString_Destroy(&oldNick);
        return;
    }
    LobbyString_InitFromCStr(&enumChannel, channel);
    status = LobbyChat_AreChannelUsersEnumerated(gLobbyChatManager, &enumChannel);
    LobbyString_Destroy(&enumChannel);
    if (status) {
        LobbyString_InitFromCStr(&joinedKey, channel);
        hooks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &joinedKey);
        LobbyString_Destroy(&joinedKey);
        LobbyString_InitFromCStr(&joinedFullName, channel);
        LobbyChat_StripChannelPrefix(&joinedName, gLobbyChatManager, &joinedFullName);
        hooks->onUserJoined(StrData(&joinedName), index, hooks->param);
        LobbyString_Destroy(&joinedName);
        LobbyString_Destroy(&joinedFullName);
    }
    LobbyString_Destroy(&oldNick);
}

void LobbyChat_OnUserParted(int chat, char *channel, char *nick) {
    Str nickStr;
    Str channelKey;
    Str fullName;
    Str channelName;
    Str userNick;
    Str userChannel;
    int userId;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&nickStr, nick);
    userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &nickStr);
    LobbyString_Destroy(&nickStr);
    if (userId == -1) {
        return;
    }
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onUserParted(StrData(&channelName), userId, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
    LobbyString_InitFromCStr(&userNick, nick);
    LobbyString_InitFromCStr(&userChannel, channel);
    LobbyChat_RemoveChannelUser(gLobbyChatManager, &userNick, &userChannel);
    LobbyString_Destroy(&userChannel);
    LobbyString_Destroy(&userNick);
}

void LobbyChat_OnUserChangedNick(void *chat, void *channel, char *oldNick, char *newNick) {
    Str oldNickStr;
    Str newNickStr;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&oldNickStr, oldNick);
    LobbyString_InitFromCStr(&newNickStr, newNick);
    LobbyChat_RenameUser(gLobbyChatManager, &oldNickStr, &newNickStr);
    LobbyString_Destroy(&newNickStr);
    LobbyString_Destroy(&oldNickStr);
}

// The mode name is built and dropped again; nothing is reported to the channel.
void LobbyChat_OnUserModeChanged(int chat, int channel, int nick, int mode) {
    Str modeName;
    u32 *words;
    u32 i;
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    i = 0;
    words = (u32 *)&modeName;
    for (; i < 3; i++) {
        words[i] = 0;
    }
    switch (mode) {
    case CHAT_USER_MODE_NORMAL:
        LobbyString_AssignCStr(&modeName, gLobbyStr_normal);
        break;
    case CHAT_USER_MODE_VOICE:
        LobbyString_AssignCStr(&modeName, gLobbyStr_voice);
        break;
    case CHAT_USER_MODE_OP:
        LobbyString_AssignCStr(&modeName, gLobbyStr_ops);
        break;
    case CHAT_USER_MODE_VOICE_OP:
        LobbyString_AssignCStr(&modeName, gLobbyStr_VoiceOps);
        break;
    default:
        LobbyString_AssignCStr(&modeName, gLobbyStr_invalid);
        break;
    }
    LobbyString_Destroy(&modeName);
}

void LobbyChat_OnTopicChanged(void *chat, char *channel, void *topic) {
    Str channelKey;
    Str fullName;
    Str channelName;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onTopicChanged(StrData(&channelName), topic, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
}

void LobbyChat_OnChannelModeChanged(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
}

void LobbyChat_OnUserListUpdated(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
}

void LobbyChat_OnConnected(void *chat, int success) {
    int timerId;
    LobbyChatManager *manager;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    // stop the connect timeout
    timerId = 0;
    LobbyTimerMap_EraseKey(&gLobbyChatManager->timerMap, &timerId, 0);
    if (!success) {
        manager = gLobbyChatManager;
        manager->lastError = 3;
        manager->state = LOBBY_CHAT_STATE_ERROR;
        gLobbyChatManager->disconnected = 1;
        return;
    }
    if (gLobbyChatManager->state != LOBBY_CHAT_STATE_ERROR) {
        gLobbyChatManager->state = LOBBY_CHAT_STATE_CONNECTED;
    }
}

void LobbyChat_OnFillInUser(void *chat, void *param, char *user) {
    Str userName;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyChatManager_GetUser(&userName, gLobbyChatManager);
    strncpy(user, StrData(&userName), 127);
    LobbyString_Destroy(&userName);
}

void LobbyChatManager_GetUser(Str *out, LobbyChatManager *manager) {
    LobbyString_InitCopy(out, &manager->user);
}

// Retry with a new nick for the same user ID; errors 3 and 4 register it as a unique nick instead.
void LobbyChat_OnNickError(void *chat, int type) {
    Str a, b, c, d, e, f, g, h;
    LobbyChatManager *manager = CurrentManager();
    if (manager && manager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    if (type == 1) {
        LobbyChat_FormatNick(&a, CurrentManager()->userId);
        LobbyString_Assign(&CurrentManager()->nick, &a);
        LobbyString_Destroy(&a);
        LobbyChatManager_GetNick(&b, CurrentManager());
        chatRetryWithNickA(chat, StrData(&b));
        LobbyString_Destroy(&b);
    } else if (type == 2) {
        LobbyChat_FormatNick(&c, CurrentManager()->userId);
        LobbyString_Assign(&CurrentManager()->nick, &c);
        LobbyString_Destroy(&c);
        LobbyChatManager_GetNick(&d, CurrentManager());
        chatRetryWithNickA(chat, StrData(&d));
        LobbyString_Destroy(&d);
    } else if ((u32)(type - 3) <= 1) {
        LobbyChat_FormatNick(&e, CurrentManager()->userId);
        LobbyString_Assign(&CurrentManager()->nick, &e);
        LobbyString_Destroy(&e);
        LobbyChatManager_GetNick(&f, CurrentManager());
        chatRegisterUniqueNickA(chat, 2, StrData(&f), gLobbyStr_Empty_17);
        LobbyString_Destroy(&f);
    } else if (type == 5) {
        return;
    } else if (type == 6) {
        LobbyChat_FormatNick(&g, CurrentManager()->userId);
        LobbyString_Assign(&CurrentManager()->nick, &g);
        LobbyString_Destroy(&g);
        LobbyChatManager_GetNick(&h, CurrentManager());
        chatRetryWithNickA(chat, StrData(&h));
        LobbyString_Destroy(&h);
    }
}

void LobbyChatManager_GetNick(Str *out, LobbyChatManager *manager) {
    LobbyString_InitCopy(out, &manager->nick);
}
