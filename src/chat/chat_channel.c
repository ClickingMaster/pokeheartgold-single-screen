#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern const char gChatStr_ciInChannel[];
extern const char gChatStr_ciHashString[];
extern const char gChatStr_ciChannelLeft[];
extern const char gChatStr_ciSetChannelMode[];
extern const char gChatStr_ciChannelEntered[];
extern const char gChatStr_ciGetChannelMode[];
extern const char gChatStr_ciUserChangedNick[];
extern const char gChatStr_ciUserChangedMode[];
extern const char gChatStr_ciUserLeftChannel[];
extern const char gChatStr_ciCleanupChannels[];
extern const char gChatStr_ciUserTableHashFn[];
extern const char gChatStr_ciSetChannelTopic[];
extern const char gChatStr_ciChannelEntering[];
extern const char gChatStr_ciUserEnumChannels[];
extern const char gChatStr_ciSetUserBasicInfo[];
extern const char gChatStr_ciChannelListUsers[];
extern const char gChatStr_ciUserChangeNickMap[];
extern const char gChatStr_ciIsEnteringChannel[];
extern const char gChatStr_ciUserTableCompareFn[];
extern const char gChatStr_ciSetChannelPassword[];
extern const char gChatStr_ciJoinCallbackCalled[];
extern const char gChatStr_ciChannelTableHashFn[];
extern const char gChatStr_ciGetChannelNumUsers[];
extern const char gChatStr_ciUserEnteredChannel[];
extern const char gChatStr_ciGetChannelCallbacks[];
extern const char gChatStr_ciChannelListUsersMap[];
extern const char gChatStr_ciSetUserBasicInfoMap[];
extern const char gChatStr_ciUserEnumChannelsMap[];
extern const char gChatStr_ciWasJoinCallbackCalled[];
extern const char gChatStr_ciChannelTableCompareFn[];
extern const char gChatStr_ciUserTableElementFreeFn[];
extern const char gChatStr_ciChannelTableElementFreeFn[];
extern const char gChatStr_StrNeNULL[];
extern const char gChatStr_ChatChannelC[];
extern const char gChatStr_Str0_Ne0[];
extern const char gChatStr_ElemNeNULL[];
extern const char gChatStr_NumBucketsGt0[];
extern const char gChatStr_Elem1_NeNULL[];
extern const char gChatStr_Elem2_NeNULL[];
extern const char gChatStr_Str1_NeNULL[];
extern const char gChatStr_Str2_NeNULL[];
extern const char gChatStr_Str1_0_Ne0[];
extern const char gChatStr_Str2_0_Ne0[];
extern const char gChatStr_ChatNeNULL_2[];
extern const char gChatStr_chatChannel[];
extern const char gChatStr_ChannelNeNULL[];
extern const char gChatStr_Channel0_Ne0[];
extern const char gChatStr_StrlenChannelLtMAX_CHANNEL[];
extern const char gChatStr_CallbacksNeNULL[];
extern const char gChatStr_Empty_19[];
extern const char gChatStr_ClientDataNeNULL[];
extern const char gChatStr_UserNameNeNULL[];
extern const char gChatStr_UserName0_Ne0[];
extern const char gChatStr_StrlenUserNameLtMAX_NAME[];
extern const char gChatStr_DataNumUsersGe0[];
extern const char gChatStr_0_9[];
extern const char gChatStr_CallbackNeNULL_3[];
extern const char gChatStr_NameNeNULL[];
extern const char gChatStr_Name0_Ne0[];
extern const char gChatStr_StrlenNameLtMAX_NAME[];
extern const char gChatStr_ModeGe0_AndModeLe3[];
extern const char gChatStr_TableLookupChatChannelUsersChatUserNeNULL[];
extern const char gChatStr_UserNeNULL[];
extern const char gChatStr_User0_Ne0[];
extern const char gChatStr_StrlenUserLtMAX_NAME[];
extern const char gChatStr_ChannelUsersNeNULL[];
extern const char gChatStr_DataUserNeNULL[];
extern const char gChatStr_DataUserName0_Ne0[];
extern const char gChatStr_DataCallbackNeNULL_2[];
extern const char gChatStr_DataNewNickNeNULL[];
extern const char gChatStr_DataNewNick0_Ne0[];
extern const char gChatStr_StrlenDataNewNickLtMAX_NAME[];
extern const char gChatStr_DataOldNickNeNULL[];
extern const char gChatStr_DataOldNick0_Ne0[];
extern const char gChatStr_StrlenDataOldNickLtMAX_NAME[];
extern const char gChatStr_RcodeNe0[];
extern const char gChatStr_OldNickNeNULL[];
extern const char gChatStr_OldNick0_Ne0[];
extern const char gChatStr_StrlenOldNickLtMAX_NAME[];
extern const char gChatStr_NewNickNeNULL[];
extern const char gChatStr_NewNick0_Ne0[];
extern const char gChatStr_StrlenNewNickLtMAX_NAME[];
extern const char gChatStr_NickNeNULL[];
extern const char gChatStr_Nick0_Ne0[];
extern const char gChatStr_StrlenNickLtMAX_NAME[];

#define ASSERT(cond, message, func, line)                                   \
    do {                                                                    \
        if (!(cond)) {                                                      \
            __msl_assertion_failed(message, gChatStr_ChatChannelC, func, line); \
        }                                                                   \
    } while (0)
#define ASSERT_CHANNEL(name, func, line)                                         \
    do {                                                                         \
        ASSERT(name, gChatStr_ChannelNeNULL, func, line);                            \
        ASSERT(*name, gChatStr_Channel0_Ne0, func, line);                            \
        ASSERT(strlen(name) < 257, gChatStr_StrlenChannelLtMAX_CHANNEL, func, line); \
    } while (0)
#define ASSERT_NICK(nick, func, line)                                      \
    do {                                                                   \
        ASSERT(nick, gChatStr_UserNeNULL, func, line);                         \
        ASSERT(*nick, gChatStr_User0_Ne0, func, line);                         \
        ASSERT(strlen(nick) < 128, gChatStr_StrlenUserLtMAX_NAME, func, line); \
    } while (0)

static ChatChannel *ciGetChannel(ChatConnection *chat, const char *channel);
static int ciEnteringChannelCompareFn(const void *a, const void *b);

int ciHashString(const signed char *s, int count) {
    int value, c;
    ASSERT(s, gChatStr_StrNeNULL, gChatStr_ciHashString, 153);
    ASSERT(*s, gChatStr_Str0_Ne0, gChatStr_ciHashString, 153);
    value = 0;
    while ((c = *s++) != 0) {
        c = (c < 0 || c >= 128) ? c : __lower_mapC[c];
        value += c;
    }
    return value % count;
}

static ChatChannel *ciGetChannel(ChatConnection *chat, const char *channel) {
    char key[sizeof(ChatChannel)];
    strncpy(key, channel, 257);
    key[256] = 0;
    return TableLookup(chat->channels, key);
}

int ciChannelTableHashFn(const void *element, int numBuckets) {
    char *name = (char *)element;
    ASSERT(name, gChatStr_ElemNeNULL, gChatStr_ciChannelTableHashFn, 0xb4);
    ASSERT(numBuckets > 0, gChatStr_NumBucketsGt0, gChatStr_ciChannelTableHashFn, 0xb5);
    return ciHashString((const signed char *)name, numBuckets);
}

int ciChannelTableCompareFn(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    ASSERT(a, gChatStr_Elem1_NeNULL, gChatStr_ciChannelTableCompareFn, 0xc1);
    ASSERT(b, gChatStr_Elem2_NeNULL, gChatStr_ciChannelTableCompareFn, 0xc2);
    ASSERT(a, gChatStr_Str1_NeNULL, gChatStr_ciChannelTableCompareFn, 0xc6);
    ASSERT(b, gChatStr_Str2_NeNULL, gChatStr_ciChannelTableCompareFn, 0xc7);
    ASSERT(a, gChatStr_Str1_NeNULL, gChatStr_ciChannelTableCompareFn, 0xc8);
    ASSERT(*a, gChatStr_Str1_0_Ne0, gChatStr_ciChannelTableCompareFn, 0xc8);
    ASSERT(b, gChatStr_Str2_NeNULL, gChatStr_ciChannelTableCompareFn, 0xc9);
    ASSERT(*b, gChatStr_Str2_0_Ne0, gChatStr_ciChannelTableCompareFn, 0xc9);
    return strnicmp(a, b);
}

void ciChannelTableElementFreeFn(void *element) {
    ChatChannel *channel = element;
    ASSERT(channel, gChatStr_ElemNeNULL, gChatStr_ciChannelTableElementFreeFn, 0xd2);
    DWCi_GsFree(channel->topic);
    if (channel->users) {
        TableFree(channel->users);
    }
}

int ciUserTableHashFn(const void *element, int numBuckets) {
    char *nick = (char *)element;
    ASSERT(nick, gChatStr_ElemNeNULL, gChatStr_ciUserTableHashFn, 0xe1);
    ASSERT(numBuckets > 0, gChatStr_NumBucketsGt0, gChatStr_ciUserTableHashFn, 0xe2);
    return ciHashString((const signed char *)nick, numBuckets);
}

int ciUserTableCompareFn(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    ASSERT(a, gChatStr_Elem1_NeNULL, gChatStr_ciUserTableCompareFn, 0xee);
    ASSERT(b, gChatStr_Elem2_NeNULL, gChatStr_ciUserTableCompareFn, 0xef);
    if (!a || !b) {
        if (!a && !b) {
            return 0;
        }
        return !a ? -1 : 1;
    }
    ASSERT(a, gChatStr_Str1_NeNULL, gChatStr_ciUserTableCompareFn, 0x102);
    ASSERT(*a, gChatStr_Str1_0_Ne0, gChatStr_ciUserTableCompareFn, 0x102);
    ASSERT(b, gChatStr_Str2_NeNULL, gChatStr_ciUserTableCompareFn, 0x103);
    ASSERT(*b, gChatStr_Str2_0_Ne0, gChatStr_ciUserTableCompareFn, 0x103);
    return strnicmp(a, b);
}

void ciUserTableElementFreeFn(void *element) {
    ASSERT(element, gChatStr_ElemNeNULL, gChatStr_ciUserTableElementFreeFn, 0x10a);
}

BOOL ciInitChannels(ChatConnection *chat) {
    chat->channels = TableNew2(sizeof(ChatChannel), 7, 2, ciChannelTableHashFn, ciChannelTableCompareFn, ciChannelTableElementFreeFn);
    if (!chat->channels) {
        return FALSE;
    }
    chat->enteringChannels = ArrayNew(sizeof(ChatChannel), 0, NULL);
    if (chat->enteringChannels) {
        return TRUE;
    }
    TableFree(chat->channels);
    return FALSE;
}

void ciCleanupChannels(ChatConnection *chat) {
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciCleanupChannels, 0x127);
    if (chat->channels) {
        TableFree(chat->channels);
    }
    if (chat->enteringChannels) {
        ArrayFree(chat->enteringChannels);
    }
}

void ciChannelEntering(ChatConnection *chat, const char *channel) {
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciChannelEntering, 0x134);

    // a zeroed channel record that only carries the name
    struct {
        u32 words[sizeof(ChatChannel) / 4];
    } entry = { 0 };

    strncpy((char *)&entry, channel, 257);
    ((char *)&entry)[256] = 0;
    ArrayAppend(chat->enteringChannels, &entry);
}

static int ciEnteringChannelCompareFn(const void *a, const void *b) {
    return strnicmp((const char *)a, (const char *)b);
}

BOOL ciIsEnteringChannel(ChatConnection *chat, const char *channel) {
    int i, n;
    char *name;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciIsEnteringChannel, 0x14d);
    n = ArrayLength(chat->enteringChannels);
    for (i = 0; i < n; i++) {
        name = ArrayNth(chat->enteringChannels, i);
        ASSERT(name, gChatStr_chatChannel, gChatStr_ciIsEnteringChannel, 0x153);
        if (!strnicmp(name, channel)) {
            return TRUE;
        }
    }
    return FALSE;
}

void ciChannelEntered(ChatConnection *chat, const char *name, const ChatChannelCallbacks *callbacks) {
    char *topic;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciChannelEntered, 0x160);
    ASSERT_CHANNEL(name, gChatStr_ciChannelEntered, 0x162);
    ASSERT(callbacks, gChatStr_CallbacksNeNULL, gChatStr_ciChannelEntered, 0x163);
    topic = DWCi_GsMalloc(2);
    if (!topic) {
        return;
    }
    *topic = *(const u8 *)gChatStr_Empty_19;
    {
        ChatChannel channel = { 0 };
        channel.callbacks = *callbacks;
        strncpy(channel.name, name, 257);
        channel.name[256] = 0;
        channel.users = TableNew2(sizeof(ChatChannelUser), 61, 2, ciUserTableHashFn, ciUserTableCompareFn, ciUserTableElementFreeFn);
        if (!channel.users) {
            return;
        }
        channel.hasMode = FALSE;
        channel.topic = topic;
        channel.joined = FALSE;
        channel.key[0] = 0;
        {
            int index = ArraySearch(chat->enteringChannels, &channel, ciEnteringChannelCompareFn, 0, 0);
            if (index != -1) {
                ArrayRemoveAt(chat->enteringChannels, index);
            }
        }
        TableEnter(chat->channels, &channel);
    }
}

void ciChannelLeft(ChatConnection *chat, const char *name) {
    ChatChannel channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciChannelLeft, 0x189);
    ASSERT_CHANNEL(name, gChatStr_ciChannelLeft, 0x18b);
    strncpy(channel.name, name, 257);
    channel.name[256] = 0;
    {
        int index = ArraySearch(chat->enteringChannels, &channel, ciEnteringChannelCompareFn, 0, 0);
        if (index != -1) {
            ArrayRemoveAt(chat->enteringChannels, index);
            return;
        }
    }
    TableRemove(chat->channels, &channel);
}

ChatChannelCallbacks *ciGetChannelCallbacks(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciGetChannelCallbacks, 0x1a9);
    ASSERT_CHANNEL(name, gChatStr_ciGetChannelCallbacks, 0x1ab);
    channel = ciGetChannel(chat, name);
    if (channel == NULL) {
        return NULL;
    }
    return &channel->callbacks;
}

void ciChannelListUsersMap(void *element, void *arg) {
    ChatChannelUser *user = element;
    ChatUserList *list = arg;
    void *p;
    ASSERT(user, gChatStr_ElemNeNULL, gChatStr_ciChannelListUsersMap, 444);
    ASSERT(list, gChatStr_ClientDataNeNULL, gChatStr_ciChannelListUsersMap, 445);
    ASSERT(user, gChatStr_UserNameNeNULL, gChatStr_ciChannelListUsersMap, 450);
    ASSERT(user->nick[0], gChatStr_UserName0_Ne0, gChatStr_ciChannelListUsersMap, 450);
    ASSERT(strlen(user->nick) < 128, gChatStr_StrlenUserNameLtMAX_NAME, gChatStr_ciChannelListUsersMap, 450);
    ASSERT(list->count >= 0, gChatStr_DataNumUsersGe0, gChatStr_ciChannelListUsersMap, 455);
    p = DWCi_GsRealloc(list->users, (list->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(gChatStr_0_9, gChatStr_ChatChannelC, gChatStr_ciChannelListUsersMap, 473);
        return;
    }
    list->users = p;
    p = DWCi_GsRealloc(list->modes, (list->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(gChatStr_0_9, gChatStr_ChatChannelC, gChatStr_ciChannelListUsersMap, 480);
        return;
    }
    list->modes = p;
    list->users[list->count] = user;
    list->modes[list->count] = user->mode;
    list->count++;
}

void ciChannelListUsers(ChatConnection *chat, const char *name, ChatUserListFunc callback, int param) {
    ChatUserList list;
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciChannelListUsers, 496);
    ASSERT_CHANNEL(name, gChatStr_ciChannelListUsers, 498);
    ASSERT(callback, gChatStr_CallbackNeNULL_3, gChatStr_ciChannelListUsers, 499);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    list.chat = chat;
    list.count = 0;
    list.users = NULL;
    list.modes = NULL;
    TableMap(channel->users, ciChannelListUsersMap, &list);
    callback(chat, name, list.count, list.users, list.modes, param);
    DWCi_GsFree(list.users);
    DWCi_GsFree(list.modes);
}

BOOL ciInChannel(ChatConnection *chat, const char *name) {
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciInChannel, 0x20f);
    if (ciGetChannel(chat, name) == NULL) {
        return FALSE;
    }
    return TRUE;
}

BOOL ciGetChannelMode(ChatConnection *chat, const char *name, void *mode) {
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciGetChannelMode, 0x21a);
    ASSERT_CHANNEL(name, gChatStr_ciGetChannelMode, 0x21c);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return FALSE;
    }
    if (!channel->hasMode) {
        return FALSE;
    }
    memcpy(mode, channel->mode, sizeof(channel->mode));
    return TRUE;
}

void ciSetChannelMode(ChatConnection *chat, const char *name, const void *mode) {
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciSetChannelMode, 0x233);
    ASSERT_CHANNEL(name, gChatStr_ciSetChannelMode, 0x235);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    channel->hasMode = TRUE;
    memcpy(channel->mode, mode, sizeof(channel->mode));
}

void ciSetChannelPassword(ChatConnection *chat, const char *name, const char *topic) {
    ChatChannel *channel;
    int n;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciSetChannelPassword, 0x24a);
    ASSERT_CHANNEL(name, gChatStr_ciSetChannelPassword, 0x24c);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    DWCi_GsFree(channel->topic);
    if (!topic) {
        topic = gChatStr_Empty_19;
    }
    n = strlen(topic);
    channel->topic = DWCi_GsMalloc(n + 1);
    if (!channel->topic) {
        return;
    }
    memcpy(channel->topic, topic, n + 1);
}

void ciJoinCallbackCalled(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciJoinCallbackCalled, 0x278);
    ASSERT_CHANNEL(name, gChatStr_ciJoinCallbackCalled, 0x27a);
    channel = ciGetChannel(chat, name);
    if (channel) {
        channel->joined = TRUE;
    }
}

BOOL ciWasJoinCallbackCalled(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciWasJoinCallbackCalled, 0x28a);
    ASSERT_CHANNEL(name, gChatStr_ciWasJoinCallbackCalled, 0x28c);
    channel = ciGetChannel(chat, name);
    if (channel == NULL) {
        return FALSE;
    }
    return channel->joined;
}

void ciSetChannelTopic(ChatConnection *chat, const char *name, const char *key) {
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciSetChannelTopic, 0x29a);
    ASSERT_CHANNEL(name, gChatStr_ciSetChannelTopic, 0x29c);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    strncpy(channel->key, key, 128);
    channel->key[127] = 0;
}

int ciGetChannelNumUsers(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciGetChannelNumUsers, 0x2bc);
    ASSERT_CHANNEL(name, gChatStr_ciGetChannelNumUsers, 0x2be);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return -1;
    }
    return TableCount(channel->users);
}

void ciUserEnteredChannel(ChatConnection *chat, const char *nick, const char *name, int mode, const char *user, const char *address) {
    ChatChannel *channel;
    ASSERT(chat != NULL, gChatStr_ChatNeNULL_2, gChatStr_ciUserEnteredChannel, 720);
    ASSERT(nick != NULL, gChatStr_NameNeNULL, gChatStr_ciUserEnteredChannel, 722);
    ASSERT(*nick != 0, gChatStr_Name0_Ne0, gChatStr_ciUserEnteredChannel, 722);
    ASSERT(strlen(nick) < 128, gChatStr_StrlenNameLtMAX_NAME, gChatStr_ciUserEnteredChannel, 722);
    ASSERT(name != NULL, gChatStr_ChannelNeNULL, gChatStr_ciUserEnteredChannel, 723);
    ASSERT(*name != 0, gChatStr_Channel0_Ne0, gChatStr_ciUserEnteredChannel, 723);
    ASSERT(mode >= 0 && mode <= 3, gChatStr_ModeGe0_AndModeLe3, gChatStr_ciUserEnteredChannel, 724);
    channel = ciGetChannel(chat, name);
    if (channel == NULL) {
        return;
    }
    {
        ChatChannelUser entry = { 0 };
        strncpy(entry.nick, nick, 128);
        entry.nick[127] = 0;
        if (user != NULL && address != NULL) {
            strncpy(entry.user, user, 24);
            entry.user[23] = 0;
            strncpy(entry.address, address, 64);
            entry.address[63] = 0;
            entry.hasUserInfo = TRUE;
        } else {
            entry.hasUserInfo = FALSE;
        }
        entry.mode = mode;
        TableEnter(channel->users, &entry);
        ASSERT(TableLookup(channel->users, &entry) != NULL, gChatStr_TableLookupChatChannelUsersChatUserNeNULL, gChatStr_ciUserEnteredChannel, 759);
    }
}

void ciUserLeftChannel(ChatConnection *chat, const char *nick, const char *name) {
    char key[sizeof(ChatChannelUser)];
    ChatChannel *channel;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciUserLeftChannel, 0x2fe);
    ASSERT_NICK(nick, gChatStr_ciUserLeftChannel, 0x300);
    ASSERT(name, gChatStr_ChannelNeNULL, gChatStr_ciUserLeftChannel, 0x301);
    ASSERT(*name, gChatStr_Channel0_Ne0, gChatStr_ciUserLeftChannel, 0x301);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    strncpy(key, nick, 128);
    key[127] = 0;
    TableRemove(channel->users, key);
}

void ciUserEnumChannelsMap(void *element, void *arg) {
    ChatChannel *channel = element;
    ChatUserSearch *search = arg;
    ASSERT(channel, gChatStr_ElemNeNULL, gChatStr_ciUserEnumChannelsMap, 0x318);
    ASSERT(search, gChatStr_ClientDataNeNULL, gChatStr_ciUserEnumChannelsMap, 0x319);
    ASSERT(channel->users, gChatStr_ChannelUsersNeNULL, gChatStr_ciUserEnumChannelsMap, 0x31e);
    ASSERT(search->nick, gChatStr_DataUserNeNULL, gChatStr_ciUserEnumChannelsMap, 0x323);
    ASSERT(*search->nick, gChatStr_DataUserName0_Ne0, gChatStr_ciUserEnumChannelsMap, 0x324);
    ASSERT(search->callback, gChatStr_DataCallbackNeNULL_2, gChatStr_ciUserEnumChannelsMap, 0x325);
    if (!TableLookup(channel->users, search->nick)) {
        return;
    }
    search->callback(search->chat, search->nick, channel, search->param);
}

void ciUserEnumChannels(ChatConnection *chat, const char *nick, ChatUserSearchFunc callback, void *param) {
    ChatUserSearch search;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciUserEnumChannels, 0x336);
    ASSERT_NICK(nick, gChatStr_ciUserEnumChannels, 0x338);
    ASSERT(callback, gChatStr_CallbackNeNULL_3, gChatStr_ciUserEnumChannels, 0x339);
    strncpy(search.buffer, nick, 128);
    search.buffer[127] = 0;
    search.chat = chat;
    search.callback = callback;
    search.param = param;
    search.nick = search.buffer;
    TableMap(chat->channels, ciUserEnumChannelsMap, &search);
}

void ciUserChangeNickMap(void *element, void *arg) {
    ChatChannel *channel = element;
    ChatNickChange *change = arg;
    u8 entry[sizeof(ChatChannelUser)];
    ChatNickChangeArgs args;
    void *found;
    ASSERT(channel, gChatStr_ElemNeNULL, gChatStr_ciUserChangeNickMap, 0x34f);
    ASSERT(change, gChatStr_ClientDataNeNULL, gChatStr_ciUserChangeNickMap, 0x350);
    ASSERT(channel->users, gChatStr_ChannelUsersNeNULL, gChatStr_ciUserChangeNickMap, 0x355);
    ASSERT(change->newNick, gChatStr_DataNewNickNeNULL, gChatStr_ciUserChangeNickMap, 0x35a);
    ASSERT(*change->newNick, gChatStr_DataNewNick0_Ne0, gChatStr_ciUserChangeNickMap, 0x35a);
    ASSERT(strlen(change->newNick) < 128, gChatStr_StrlenDataNewNickLtMAX_NAME, gChatStr_ciUserChangeNickMap, 0x35a);
    ASSERT(change->oldNick, gChatStr_DataOldNickNeNULL, gChatStr_ciUserChangeNickMap, 0x35b);
    ASSERT(*change->oldNick, gChatStr_DataOldNick0_Ne0, gChatStr_ciUserChangeNickMap, 0x35b);
    ASSERT(strlen(change->oldNick) < 128, gChatStr_StrlenDataOldNickLtMAX_NAME, gChatStr_ciUserChangeNickMap, 0x35b);
    found = TableLookup(channel->users, change->oldNick);
    if (!found) {
        return;
    }
    memcpy(entry, found, sizeof(entry));
    ASSERT(TableRemove(channel->users, found), gChatStr_RcodeNe0, gChatStr_ciUserChangeNickMap, 0x367);
    strncpy((char *)entry, change->newNick, 128);
    entry[127] = 0;
    TableEnter(channel->users, entry);
    if (ciWasJoinCallbackCalled(change->chat, channel->name) && channel->callbacks.nickChanged) {
        args.channel = channel->name;
        args.oldNick = change->oldNick;
        args.newNick = change->newNick;
        ciAddCallback_(change->chat, CALLBACK_USER_CHANGED_NICK, channel->callbacks.nickChanged, (u32 *)&args, channel->callbacks.param, 0, channel->name, sizeof(args));
    }
}

void ciUserChangedNick(ChatConnection *chat, const char *oldNick, const char *newNick) {
    ChatNickChange change;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciUserChangedNick, 0x386);
    ASSERT(oldNick, gChatStr_OldNickNeNULL, gChatStr_ciUserChangedNick, 0x388);
    ASSERT(*oldNick, gChatStr_OldNick0_Ne0, gChatStr_ciUserChangedNick, 0x388);
    ASSERT(strlen(oldNick) < 128, gChatStr_StrlenOldNickLtMAX_NAME, gChatStr_ciUserChangedNick, 0x388);
    ASSERT(newNick, gChatStr_NewNickNeNULL, gChatStr_ciUserChangedNick, 0x389);
    ASSERT(*newNick, gChatStr_NewNick0_Ne0, gChatStr_ciUserChangedNick, 0x389);
    ASSERT(strlen(newNick) < 128, gChatStr_StrlenNewNickLtMAX_NAME, gChatStr_ciUserChangedNick, 0x389);
    change.chat = chat;
    change.oldNick = oldNick;
    change.newNick = newNick;
    TableMap(chat->channels, ciUserChangeNickMap, &change);
}

void ciUserChangedMode(ChatConnection *chat, const char *nick, const char *name, int mode, BOOL set) {
    struct {
        ChatUserModeArgs args;
        char nickKey[128];
        char unused_8c[96];
        char channelKey[257];
        char unused_1ed[225];
    } frame;

    ChatChannel *channel;
    ChatChannelUser *user;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciUserChangedMode, 923);
    ASSERT_NICK(nick, gChatStr_ciUserChangedMode, 925);
    ASSERT(name, gChatStr_ChannelNeNULL, gChatStr_ciUserChangedMode, 926);
    ASSERT(*name, gChatStr_Channel0_Ne0, gChatStr_ciUserChangedMode, 926);
    ASSERT(mode >= 0 && mode <= 3, gChatStr_ModeGe0_AndModeLe3, gChatStr_ciUserChangedMode, 927);
    strncpy(frame.channelKey, name, 257);
    frame.channelKey[256] = 0;
    channel = TableLookup(chat->channels, frame.channelKey);
    if (!channel) {
        return;
    }
    strncpy(frame.nickKey, nick, 128);
    frame.nickKey[127] = 0;
    user = TableLookup(channel->users, frame.nickKey);
    if (!user) {
        return;
    }
    if (set) {
        user->mode |= mode;
    } else {
        user->mode &= ~mode;
    }
    if (!channel->callbacks.userModeChanged) {
        return;
    }
    frame.args.channel = name;
    frame.args.nick = nick;
    frame.args.mode = user->mode;
    ciAddCallback_(chat, CALLBACK_USER_MODE_CHANGED, channel->callbacks.userModeChanged, (u32 *)&frame.args, channel->callbacks.param, 0, (char *)name, sizeof(frame.args));
}

void ciSetUserBasicInfoMap(void *element, void *arg) {
    ChatChannel *channel = element;
    ChatUserInfo *info = arg;
    ChatChannelUser *user;
    ASSERT(channel, gChatStr_ElemNeNULL, gChatStr_ciSetUserBasicInfoMap, 0x41a);
    ASSERT(info, gChatStr_ClientDataNeNULL, gChatStr_ciSetUserBasicInfoMap, 0x41b);
    ASSERT(channel->users, gChatStr_ChannelUsersNeNULL, gChatStr_ciSetUserBasicInfoMap, 0x424);
    user = TableLookup(channel->users, info->nick);
    if (!user) {
        return;
    }
    strncpy(user->user, info->user, sizeof(user->user));
    user->user[23] = 0;
    strncpy(user->address, info->address, sizeof(user->address));
    user->address[63] = 0;
    user->hasUserInfo = TRUE;
}

void ciSetUserBasicInfo(ChatConnection *chat, const char *nick, const char *user, const char *address) {
    char buffer[sizeof(ChatChannelUser)];
    ChatUserInfo info;
    ASSERT(chat, gChatStr_ChatNeNULL_2, gChatStr_ciSetUserBasicInfo, 1085);
    ASSERT(nick, gChatStr_NickNeNULL, gChatStr_ciSetUserBasicInfo, 1087);
    ASSERT(*nick != 0, gChatStr_Nick0_Ne0, gChatStr_ciSetUserBasicInfo, 1087);
    ASSERT(strlen(nick) < 128, gChatStr_StrlenNickLtMAX_NAME, gChatStr_ciSetUserBasicInfo, 1087);
    strncpy(buffer, nick, 128);
    buffer[127] = 0;
    info.user = user;
    info.address = address;
    info.nick = buffer;
    TableMap(chat->channels, ciSetUserBasicInfoMap, &info);
}
