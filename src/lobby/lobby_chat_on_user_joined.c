#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef union String {
    struct {
        u32 tag : 1;
        u32 rest : 31;
        char *other;
        char *ptr;
    } large;

    struct {
        char tag;
        char chars[11];
    } small;
} String;

typedef struct Hooks {
    void (*onUserJoined)(char *, int, void *);
    void (*onUserParted)(char *, char *, void *);
    int pad[3];
    void *arg;
} Hooks;

extern u8 *gLobbyChatManager;
void LobbyString_InitFromCStr(String *, const char *);
void LobbyString_Destroy(String *);
int LobbyChat_GetChannelUserLimit(void *, String *);
int chatGetChannelNumUsersA(int, const char *);
char *LobbyChat_GetUserIdFromNick(String *);
void LobbyChat_GetUserNick(String *, void *, char *);
int LobbyChat_RemoveChannelUser(void *, String *, String *);
Hooks *LobbyChat_GetChannelCallbacks(void *, String *);
void LobbyChat_StripChannelPrefix(String *, void *, String *);
int LobbyChat_AddChannelUser(void *, String *, String *);
int LobbyChat_AreChannelUsersEnumerated(void *, String *);
void LobbyChat_OnUserJoined(void *, const char *, const char *);

static inline char *getStringData(String *s) {
    if ((BOOL)s->large.tag == FALSE) {
        return s->small.chars;
    }
    return s->large.ptr;
}

// A user ID that is already in the channel is reported as parted before the new join.
void LobbyChat_OnUserJoined(void *chat, const char *channel, const char *nick) {
    String oldNick;
    String limitChannel;
    String nickStr;
    String oldChannel;
    String partedKey;
    String partedFullName;
    String partedName;
    String partedNick;
    String joinedNick;
    String joinedChannel;
    String enumChannel;
    String joinedKey;
    String joinedFullName;
    String joinedName;
    int status;
    int index;
    Hooks *hooks;
    if (gLobbyChatManager && *(int *)(gLobbyChatManager + 4) == 5) {
        return;
    }
    LobbyString_InitFromCStr(&limitChannel, channel);
    status = LobbyChat_GetChannelUserLimit(gLobbyChatManager, &limitChannel);
    LobbyString_Destroy(&limitChannel);
    if (status && chatGetChannelNumUsersA(*(int *)gLobbyChatManager, channel) > status) {
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
            String *partedNickStr = &partedNick;
            char *channelName = getStringData(&partedName);
            hooks->onUserParted(channelName, LobbyChat_GetUserIdFromNick(partedNickStr), hooks->arg);
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
        hooks->onUserJoined(getStringData(&joinedName), index, hooks->arg);
        LobbyString_Destroy(&joinedName);
        LobbyString_Destroy(&joinedFullName);
    }
    LobbyString_Destroy(&oldNick);
}
