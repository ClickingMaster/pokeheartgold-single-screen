#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern const char sStr_ciInChannel[];
extern const char sStr_ciHashString[];
extern const char sStr_ciChannelLeft[];
extern const char sStr_ciSetChannelMode[];
extern const char sStr_ciChannelEntered[];
extern const char sStr_ciGetChannelMode[];
extern const char sStr_ciUserChangedNick[];
extern const char sStr_ciUserChangedMode[];
extern const char sStr_ciUserLeftChannel[];
extern const char sStr_ciCleanupChannels[];
extern const char sStr_ciUserTableHashFn[];
extern const char sStr_ciSetChannelTopic[];
extern const char sStr_ciChannelEntering[];
extern const char sStr_ciUserEnumChannels[];
extern const char sStr_ciSetUserBasicInfo[];
extern const char sStr_ciChannelListUsers[];
extern const char sStr_ciUserChangeNickMap[];
extern const char sStr_ciIsEnteringChannel[];
extern const char sStr_ciUserTableCompareFn[];
extern const char sStr_ciSetChannelPassword[];
extern const char sStr_ciJoinCallbackCalled[];
extern const char sStr_ciChannelTableHashFn[];
extern const char sStr_ciGetChannelNumUsers[];
extern const char sStr_ciUserEnteredChannel[];
extern const char sStr_ciGetChannelCallbacks[];
extern const char sStr_ciChannelListUsersMap[];
extern const char sStr_ciSetUserBasicInfoMap[];
extern const char sStr_ciUserEnumChannelsMap[];
extern const char sStr_ciWasJoinCallbackCalled[];
extern const char sStr_ciChannelTableCompareFn[];
extern const char sStr_ciUserTableElementFreeFn[];
extern const char sStr_ciChannelTableElementFreeFn[];
extern const char sStr_StrNeNULL[];
extern const char sStr_ChatChannelC[];
extern const char sStr_Str0_Ne0[];
extern const char sStr_ElemNeNULL[];
extern const char sStr_NumBucketsGt0[];
extern const char sStr_Elem1_NeNULL[];
extern const char sStr_Elem2_NeNULL[];
extern const char sStr_Str1_NeNULL[];
extern const char sStr_Str2_NeNULL[];
extern const char sStr_Str1_0_Ne0[];
extern const char sStr_Str2_0_Ne0[];
extern const char sStr_ChatNeNULL_2[];
extern const char sStr_chatChannel[];
extern const char sStr_ChannelNeNULL[];
extern const char sStr_Channel0_Ne0[];
extern const char sStr_StrlenChannelLtMAX_CHANNEL[];
extern const char sStr_CallbacksNeNULL[];
extern const char sStr_Empty_19[];
extern const char sStr_ClientDataNeNULL[];
extern const char sStr_UserNameNeNULL[];
extern const char sStr_UserName0_Ne0[];
extern const char sStr_StrlenUserNameLtMAX_NAME[];
extern const char sStr_DataNumUsersGe0[];
extern const char sStr_0_9[];
extern const char sStr_CallbackNeNULL_3[];
extern const char sStr_NameNeNULL[];
extern const char sStr_Name0_Ne0[];
extern const char sStr_StrlenNameLtMAX_NAME[];
extern const char sStr_ModeGe0_AndModeLe3[];
extern const char sStr_TableLookupChatChannelUsersChatUserNeNULL[];
extern const char sStr_UserNeNULL[];
extern const char sStr_User0_Ne0[];
extern const char sStr_StrlenUserLtMAX_NAME[];
extern const char sStr_ChannelUsersNeNULL[];
extern const char sStr_DataUserNeNULL[];
extern const char sStr_DataUserName0_Ne0[];
extern const char sStr_DataCallbackNeNULL_2[];
extern const char sStr_DataNewNickNeNULL[];
extern const char sStr_DataNewNick0_Ne0[];
extern const char sStr_StrlenDataNewNickLtMAX_NAME[];
extern const char sStr_DataOldNickNeNULL[];
extern const char sStr_DataOldNick0_Ne0[];
extern const char sStr_StrlenDataOldNickLtMAX_NAME[];
extern const char sStr_RcodeNe0[];
extern const char sStr_OldNickNeNULL[];
extern const char sStr_OldNick0_Ne0[];
extern const char sStr_StrlenOldNickLtMAX_NAME[];
extern const char sStr_NewNickNeNULL[];
extern const char sStr_NewNick0_Ne0[];
extern const char sStr_StrlenNewNickLtMAX_NAME[];
extern const char sStr_NickNeNULL[];
extern const char sStr_Nick0_Ne0[];
extern const char sStr_StrlenNickLtMAX_NAME[];

// Assertion with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, sStr_ChatChannelC, func, line); \
    }
// A valid channel name: not NULL, not empty, shorter than 257.
#define ASSERT_CHANNEL(name, func, line)      \
    ASSERT(name, sStr_ChannelNeNULL, func, line);  \
    ASSERT(*name, sStr_Channel0_Ne0, func, line); \
    ASSERT(strlen(name) < 257, sStr_StrlenChannelLtMAX_CHANNEL, func, line);
// A valid nick: not NULL, not empty, shorter than 128.
#define ASSERT_NICK(nick, func, line)         \
    ASSERT(nick, sStr_UserNeNULL, func, line);  \
    ASSERT(*nick, sStr_User0_Ne0, func, line); \
    ASSERT(strlen(nick) < 128, sStr_StrlenUserLtMAX_NAME, func, line);

int ciHashString(const signed char *s, int count) {
    int value, c;
    ASSERT(s, sStr_StrNeNULL, sStr_ciHashString, 153);
    ASSERT(*s, sStr_Str0_Ne0, sStr_ciHashString, 153);
    value = 0;
    while ((c = *s++) != 0) {
        c = (c < 0 || c >= 128) ? c : __lower_mapC[c];
        value += c;
    }
    return value % count;
}

ChatChannel *ciGetChannel(ChatConnection *chat, const char *channel) {
    char key[sizeof(ChatChannel)];
    strncpy(key, channel, 257);
    key[256] = 0;
    return TableLookup(chat->channels, key);
}

int ciChannelTableHashFn(const void *element, int numBuckets) {
    char *name = (char *)element;
    ASSERT(name, sStr_ElemNeNULL, sStr_ciChannelTableHashFn, 0xb4);
    ASSERT(numBuckets > 0, sStr_NumBucketsGt0, sStr_ciChannelTableHashFn, 0xb5);
    return ciHashString((const signed char *)name, numBuckets);
}

int ciChannelTableCompareFn(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    ASSERT(a, sStr_Elem1_NeNULL, sStr_ciChannelTableCompareFn, 0xc1);
    ASSERT(b, sStr_Elem2_NeNULL, sStr_ciChannelTableCompareFn, 0xc2);
    ASSERT(a, sStr_Str1_NeNULL, sStr_ciChannelTableCompareFn, 0xc6);
    ASSERT(b, sStr_Str2_NeNULL, sStr_ciChannelTableCompareFn, 0xc7);
    ASSERT(a, sStr_Str1_NeNULL, sStr_ciChannelTableCompareFn, 0xc8);
    ASSERT(*a, sStr_Str1_0_Ne0, sStr_ciChannelTableCompareFn, 0xc8);
    ASSERT(b, sStr_Str2_NeNULL, sStr_ciChannelTableCompareFn, 0xc9);
    ASSERT(*b, sStr_Str2_0_Ne0, sStr_ciChannelTableCompareFn, 0xc9);
    return strnicmp(a, b);
}

void ciChannelTableElementFreeFn(void *element) {
    ChatChannel *channel = element;
    ASSERT(channel, sStr_ElemNeNULL, sStr_ciChannelTableElementFreeFn, 0xd2);
    DWCi_GsFree(channel->topic);
    if (channel->users) {
        TableFree(channel->users);
    }
}

int ciUserTableHashFn(const void *element, int numBuckets) {
    char *nick = (char *)element;
    ASSERT(nick, sStr_ElemNeNULL, sStr_ciUserTableHashFn, 0xe1);
    ASSERT(numBuckets > 0, sStr_NumBucketsGt0, sStr_ciUserTableHashFn, 0xe2);
    return ciHashString((const signed char *)nick, numBuckets);
}

int ciUserTableCompareFn(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    ASSERT(a, sStr_Elem1_NeNULL, sStr_ciUserTableCompareFn, 0xee);
    ASSERT(b, sStr_Elem2_NeNULL, sStr_ciUserTableCompareFn, 0xef);
    if (!a || !b) {
        if (!a && !b) {
            return 0;
        }
        return !a ? -1 : 1;
    }
    ASSERT(a, sStr_Str1_NeNULL, sStr_ciUserTableCompareFn, 0x102);
    ASSERT(*a, sStr_Str1_0_Ne0, sStr_ciUserTableCompareFn, 0x102);
    ASSERT(b, sStr_Str2_NeNULL, sStr_ciUserTableCompareFn, 0x103);
    ASSERT(*b, sStr_Str2_0_Ne0, sStr_ciUserTableCompareFn, 0x103);
    return strnicmp(a, b);
}

void ciUserTableElementFreeFn(void *element) {
    ASSERT(element, sStr_ElemNeNULL, sStr_ciUserTableElementFreeFn, 0x10a);
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciCleanupChannels, 0x127);
    if (chat->channels) {
        TableFree(chat->channels);
    }
    if (chat->enteringChannels) {
        ArrayFree(chat->enteringChannels);
    }
}

void ciChannelEntering(ChatConnection *chat, const char *channel) {
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciChannelEntering, 0x134);

    // a zeroed channel record that only carries the name
    struct {
        u32 words[sizeof(ChatChannel) / 4];
    } entry = { 0 };

    strncpy((char *)&entry, channel, 257);
    ((char *)&entry)[256] = 0;
    ArrayAppend(chat->enteringChannels, &entry);
}

int ciEnteringChannelCompareFn(const void *a, const void *b) {
    return strnicmp((const char *)a, (const char *)b);
}

BOOL ciIsEnteringChannel(ChatConnection *chat, const char *channel) {
    int i, n;
    char *name;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciIsEnteringChannel, 0x14d);
    n = ArrayLength(chat->enteringChannels);
    for (i = 0; i < n; i++) {
        name = ArrayNth(chat->enteringChannels, i);
        ASSERT(name, sStr_chatChannel, sStr_ciIsEnteringChannel, 0x153);
        if (!strnicmp(name, channel)) {
            return TRUE;
        }
    }
    return FALSE;
}

void ciChannelEntered(ChatConnection *chat, const char *name, const ChatChannelCallbacks *callbacks) {
    char *topic;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciChannelEntered, 0x160);
    ASSERT_CHANNEL(name, sStr_ciChannelEntered, 0x162);
    ASSERT(callbacks, sStr_CallbacksNeNULL, sStr_ciChannelEntered, 0x163);
    topic = DWCi_GsMalloc(2);
    if (!topic) {
        return;
    }
    *topic = *(const u8 *)sStr_Empty_19;
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciChannelLeft, 0x189);
    ASSERT_CHANNEL(name, sStr_ciChannelLeft, 0x18b);
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciGetChannelCallbacks, 0x1a9);
    ASSERT_CHANNEL(name, sStr_ciGetChannelCallbacks, 0x1ab);
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
    ASSERT(user, sStr_ElemNeNULL, sStr_ciChannelListUsersMap, 444);
    ASSERT(list, sStr_ClientDataNeNULL, sStr_ciChannelListUsersMap, 445);
    ASSERT(user, sStr_UserNameNeNULL, sStr_ciChannelListUsersMap, 450);
    ASSERT(user->nick[0], sStr_UserName0_Ne0, sStr_ciChannelListUsersMap, 450);
    ASSERT(strlen(user->nick) < 128, sStr_StrlenUserNameLtMAX_NAME, sStr_ciChannelListUsersMap, 450);
    ASSERT(list->count >= 0, sStr_DataNumUsersGe0, sStr_ciChannelListUsersMap, 455);
    p = DWCi_GsRealloc(list->users, (list->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(sStr_0_9, sStr_ChatChannelC, sStr_ciChannelListUsersMap, 473);
        return;
    }
    list->users = p;
    p = DWCi_GsRealloc(list->modes, (list->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(sStr_0_9, sStr_ChatChannelC, sStr_ciChannelListUsersMap, 480);
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciChannelListUsers, 496);
    ASSERT_CHANNEL(name, sStr_ciChannelListUsers, 498);
    ASSERT(callback, sStr_CallbackNeNULL_3, sStr_ciChannelListUsers, 499);
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciInChannel, 0x20f);
    if (ciGetChannel(chat, name) == NULL) {
        return FALSE;
    }
    return TRUE;
}

BOOL ciGetChannelMode(ChatConnection *chat, const char *name, void *mode) {
    ChatChannel *channel;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciGetChannelMode, 0x21a);
    ASSERT_CHANNEL(name, sStr_ciGetChannelMode, 0x21c);
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciSetChannelMode, 0x233);
    ASSERT_CHANNEL(name, sStr_ciSetChannelMode, 0x235);
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciSetChannelPassword, 0x24a);
    ASSERT_CHANNEL(name, sStr_ciSetChannelPassword, 0x24c);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    DWCi_GsFree(channel->topic);
    if (!topic) {
        topic = sStr_Empty_19;
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciJoinCallbackCalled, 0x278);
    ASSERT_CHANNEL(name, sStr_ciJoinCallbackCalled, 0x27a);
    channel = ciGetChannel(chat, name);
    if (channel) {
        channel->joined = TRUE;
    }
}

BOOL ciWasJoinCallbackCalled(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciWasJoinCallbackCalled, 0x28a);
    ASSERT_CHANNEL(name, sStr_ciWasJoinCallbackCalled, 0x28c);
    channel = ciGetChannel(chat, name);
    if (channel == NULL) {
        return FALSE;
    }
    return channel->joined;
}

void ciSetChannelTopic(ChatConnection *chat, const char *name, const char *key) {
    ChatChannel *channel;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciSetChannelTopic, 0x29a);
    ASSERT_CHANNEL(name, sStr_ciSetChannelTopic, 0x29c);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    strncpy(channel->key, key, 128);
    channel->key[127] = 0;
}

int ciGetChannelNumUsers(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciGetChannelNumUsers, 0x2bc);
    ASSERT_CHANNEL(name, sStr_ciGetChannelNumUsers, 0x2be);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return -1;
    }
    return TableCount(channel->users);
}

void ciUserEnteredChannel(ChatConnection *chat, const char *nick, const char *name, int mode, const char *user, const char *address) {
    ChatChannel *channel;
    ASSERT(chat != NULL, sStr_ChatNeNULL_2, sStr_ciUserEnteredChannel, 720);
    ASSERT(nick != NULL, sStr_NameNeNULL, sStr_ciUserEnteredChannel, 722);
    ASSERT(*nick != 0, sStr_Name0_Ne0, sStr_ciUserEnteredChannel, 722);
    ASSERT(strlen(nick) < 128, sStr_StrlenNameLtMAX_NAME, sStr_ciUserEnteredChannel, 722);
    ASSERT(name != NULL, sStr_ChannelNeNULL, sStr_ciUserEnteredChannel, 723);
    ASSERT(*name != 0, sStr_Channel0_Ne0, sStr_ciUserEnteredChannel, 723);
    ASSERT(mode >= 0 && mode <= 3, sStr_ModeGe0_AndModeLe3, sStr_ciUserEnteredChannel, 724);
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
        ASSERT(TableLookup(channel->users, &entry) != NULL, sStr_TableLookupChatChannelUsersChatUserNeNULL, sStr_ciUserEnteredChannel, 759);
    }
}

void ciUserLeftChannel(ChatConnection *chat, const char *nick, const char *name) {
    char key[sizeof(ChatChannelUser)];
    ChatChannel *channel;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciUserLeftChannel, 0x2fe);
    ASSERT_NICK(nick, sStr_ciUserLeftChannel, 0x300);
    ASSERT(name, sStr_ChannelNeNULL, sStr_ciUserLeftChannel, 0x301);
    ASSERT(*name, sStr_Channel0_Ne0, sStr_ciUserLeftChannel, 0x301);
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
    ASSERT(channel, sStr_ElemNeNULL, sStr_ciUserEnumChannelsMap, 0x318);
    ASSERT(search, sStr_ClientDataNeNULL, sStr_ciUserEnumChannelsMap, 0x319);
    ASSERT(channel->users, sStr_ChannelUsersNeNULL, sStr_ciUserEnumChannelsMap, 0x31e);
    ASSERT(search->nick, sStr_DataUserNeNULL, sStr_ciUserEnumChannelsMap, 0x323);
    ASSERT(*search->nick, sStr_DataUserName0_Ne0, sStr_ciUserEnumChannelsMap, 0x324);
    ASSERT(search->callback, sStr_DataCallbackNeNULL_2, sStr_ciUserEnumChannelsMap, 0x325);
    if (!TableLookup(channel->users, search->nick)) {
        return;
    }
    search->callback(search->chat, search->nick, channel, search->param);
}

void ciUserEnumChannels(ChatConnection *chat, const char *nick, ChatUserSearchFunc callback, void *param) {
    ChatUserSearch search;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciUserEnumChannels, 0x336);
    ASSERT_NICK(nick, sStr_ciUserEnumChannels, 0x338);
    ASSERT(callback, sStr_CallbackNeNULL_3, sStr_ciUserEnumChannels, 0x339);
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
    ASSERT(channel, sStr_ElemNeNULL, sStr_ciUserChangeNickMap, 0x34f);
    ASSERT(change, sStr_ClientDataNeNULL, sStr_ciUserChangeNickMap, 0x350);
    ASSERT(channel->users, sStr_ChannelUsersNeNULL, sStr_ciUserChangeNickMap, 0x355);
    ASSERT(change->newNick, sStr_DataNewNickNeNULL, sStr_ciUserChangeNickMap, 0x35a);
    ASSERT(*change->newNick, sStr_DataNewNick0_Ne0, sStr_ciUserChangeNickMap, 0x35a);
    ASSERT(strlen(change->newNick) < 128, sStr_StrlenDataNewNickLtMAX_NAME, sStr_ciUserChangeNickMap, 0x35a);
    ASSERT(change->oldNick, sStr_DataOldNickNeNULL, sStr_ciUserChangeNickMap, 0x35b);
    ASSERT(*change->oldNick, sStr_DataOldNick0_Ne0, sStr_ciUserChangeNickMap, 0x35b);
    ASSERT(strlen(change->oldNick) < 128, sStr_StrlenDataOldNickLtMAX_NAME, sStr_ciUserChangeNickMap, 0x35b);
    found = TableLookup(channel->users, change->oldNick);
    if (!found) {
        return;
    }
    memcpy(entry, found, sizeof(entry));
    ASSERT(TableRemove(channel->users, found), sStr_RcodeNe0, sStr_ciUserChangeNickMap, 0x367);
    strncpy((char *)entry, change->newNick, 128);
    entry[127] = 0;
    TableEnter(channel->users, entry);
    if (ciWasJoinCallbackCalled(change->chat, channel->name) && channel->callbacks.nickChanged) {
        args.channel = channel->name;
        args.oldNick = change->oldNick;
        args.newNick = change->newNick;
        ciAddCallback_(change->chat, 8, channel->callbacks.nickChanged, (u32 *)&args, channel->callbacks.param, 0, channel->name, sizeof(args));
    }
}

void ciUserChangedNick(ChatConnection *chat, const char *oldNick, const char *newNick) {
    ChatNickChange change;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciUserChangedNick, 0x386);
    ASSERT(oldNick, sStr_OldNickNeNULL, sStr_ciUserChangedNick, 0x388);
    ASSERT(*oldNick, sStr_OldNick0_Ne0, sStr_ciUserChangedNick, 0x388);
    ASSERT(strlen(oldNick) < 128, sStr_StrlenOldNickLtMAX_NAME, sStr_ciUserChangedNick, 0x388);
    ASSERT(newNick, sStr_NewNickNeNULL, sStr_ciUserChangedNick, 0x389);
    ASSERT(*newNick, sStr_NewNick0_Ne0, sStr_ciUserChangedNick, 0x389);
    ASSERT(strlen(newNick) < 128, sStr_StrlenNewNickLtMAX_NAME, sStr_ciUserChangedNick, 0x389);
    change.chat = chat;
    change.oldNick = oldNick;
    change.newNick = newNick;
    TableMap(chat->channels, ciUserChangeNickMap, &change);
}

void ciUserChangedMode(ChatConnection *chat, const char *nick, const char *name, int mode, BOOL set) {
    // the callback arguments and the two lookup keys share one stack block
    struct {
        ChatUserModeArgs args;
        char nickKey[128];
        char unused_8c[96];
        char channelKey[257];
        char unused_1ed[225];
    } frame;

    ChatChannel *channel;
    ChatChannelUser *user;
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciUserChangedMode, 923);
    ASSERT_NICK(nick, sStr_ciUserChangedMode, 925);
    ASSERT(name, sStr_ChannelNeNULL, sStr_ciUserChangedMode, 926);
    ASSERT(*name, sStr_Channel0_Ne0, sStr_ciUserChangedMode, 926);
    ASSERT(mode >= 0 && mode <= 3, sStr_ModeGe0_AndModeLe3, sStr_ciUserChangedMode, 927);
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
    ciAddCallback_(chat, 11, channel->callbacks.userModeChanged, (u32 *)&frame.args, channel->callbacks.param, 0, (char *)name, sizeof(frame.args));
}

void ciSetUserBasicInfoMap(void *element, void *arg) {
    ChatChannel *channel = element;
    ChatUserInfo *info = arg;
    ChatChannelUser *user;
    ASSERT(channel, sStr_ElemNeNULL, sStr_ciSetUserBasicInfoMap, 0x41a);
    ASSERT(info, sStr_ClientDataNeNULL, sStr_ciSetUserBasicInfoMap, 0x41b);
    ASSERT(channel->users, sStr_ChannelUsersNeNULL, sStr_ciSetUserBasicInfoMap, 0x424);
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
    ASSERT(chat, sStr_ChatNeNULL_2, sStr_ciSetUserBasicInfo, 1085);
    ASSERT(nick, sStr_NickNeNULL, sStr_ciSetUserBasicInfo, 1087);
    ASSERT(*nick != 0, sStr_Nick0_Ne0, sStr_ciSetUserBasicInfo, 1087);
    ASSERT(strlen(nick) < 128, sStr_StrlenNickLtMAX_NAME, sStr_ciSetUserBasicInfo, 1087);
    strncpy(buffer, nick, 128);
    buffer[127] = 0;
    info.user = user;
    info.address = address;
    info.nick = buffer;
    TableMap(chat->channels, ciSetUserBasicInfoMap, &info);
}
