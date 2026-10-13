#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern const char ov45_0225565C[];
extern const char ov45_02255668[];
extern const char ov45_02255678[];
extern const char ov45_02255688[];
extern const char ov45_0225569C[];
extern const char ov45_022556B0[];
extern const char ov45_022556C4[];
extern const char ov45_022556D8[];
extern const char ov45_022556EC[];
extern const char ov45_02255700[];
extern const char ov45_02255714[];
extern const char ov45_02255728[];
extern const char ov45_0225573C[];
extern const char ov45_02255750[];
extern const char ov45_02255764[];
extern const char ov45_02255778[];
extern const char ov45_0225578C[];
extern const char ov45_022557A0[];
extern const char ov45_022557B4[];
extern const char ov45_022557CC[];
extern const char ov45_022557E4[];
extern const char ov45_022557FC[];
extern const char ov45_02255814[];
extern const char ov45_0225582C[];
extern const char ov45_02255844[];
extern const char ov45_0225585C[];
extern const char ov45_02255874[];
extern const char ov45_0225588C[];
extern const char ov45_022558A4[];
extern const char ov45_022558BC[];
extern const char ov45_022558D4[];
extern const char ov45_022558F0[];
extern const char ov45_0225590C[];
extern const char ov45_02255918[];
extern const char ov45_02255928[];
extern const char ov45_02255938[];
extern const char ov45_02255948[];
extern const char ov45_02255958[];
extern const char ov45_02255968[];
extern const char ov45_02255978[];
extern const char ov45_02255988[];
extern const char ov45_02255998[];
extern const char ov45_022559A8[];
extern const char ov45_022559B8[];
extern const char ov45_022559C8[];
extern const char ov45_022559D4[];
extern const char ov45_022559E4[];
extern const char ov45_022559F8[];
extern const char ov45_02255A18[];
extern const char ov45_02255A2C[];
extern const char ov45_02255A30[];
extern const char ov45_02255A44[];
extern const char ov45_02255A58[];
extern const char ov45_02255A70[];
extern const char ov45_02255A90[];
extern const char ov45_02255AA4[];
extern const char ov45_02255AA8[];
extern const char ov45_02255ABC[];
extern const char ov45_02255ACC[];
extern const char ov45_02255ADC[];
extern const char ov45_02255AF4[];
extern const char ov45_02255B10[];
extern const char ov45_02255B44[];
extern const char ov45_02255B54[];
extern const char ov45_02255B64[];
extern const char ov45_02255B7C[];
extern const char ov45_02255B94[];
extern const char ov45_02255BA8[];
extern const char ov45_02255BC4[];
extern const char ov45_02255BDC[];
extern const char ov45_02255BF4[];
extern const char ov45_02255C10[];
extern const char ov45_02255C34[];
extern const char ov45_02255C4C[];
extern const char ov45_02255C68[];
extern const char ov45_02255C8C[];
extern const char ov45_02255C98[];
extern const char ov45_02255CA8[];
extern const char ov45_02255CBC[];
extern const char ov45_02255CD8[];
extern const char ov45_02255CE8[];
extern const char ov45_02255CFC[];
extern const char ov45_02255D18[];
extern const char ov45_02255D28[];
extern const char ov45_02255D38[];

// Assertion with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, ov45_02255918, func, line); \
    }
// A valid channel name: not NULL, not empty, shorter than 257.
#define ASSERT_CHANNEL(name, func, line)      \
    ASSERT(name, ov45_022559D4, func, line);  \
    ASSERT(*name, ov45_022559E4, func, line); \
    ASSERT(strlen(name) < 257, ov45_022559F8, func, line);
// A valid nick: not NULL, not empty, shorter than 128.
#define ASSERT_NICK(nick, func, line)         \
    ASSERT(nick, ov45_02255B44, func, line);  \
    ASSERT(*nick, ov45_02255B54, func, line); \
    ASSERT(strlen(nick) < 128, ov45_02255B64, func, line);

int ciHashString(const signed char *s, int count) {
    int value, c;
    ASSERT(s, ov45_0225590C, ov45_02255668, 153);
    ASSERT(*s, ov45_02255928, ov45_02255668, 153);
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
    ASSERT(name, ov45_02255938, ov45_022557FC, 0xb4);
    ASSERT(numBuckets > 0, ov45_02255948, ov45_022557FC, 0xb5);
    return ciHashString((const signed char *)name, numBuckets);
}

int ciChannelTableCompareFn(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    ASSERT(a, ov45_02255958, ov45_022558BC, 0xc1);
    ASSERT(b, ov45_02255968, ov45_022558BC, 0xc2);
    ASSERT(a, ov45_02255978, ov45_022558BC, 0xc6);
    ASSERT(b, ov45_02255988, ov45_022558BC, 0xc7);
    ASSERT(a, ov45_02255978, ov45_022558BC, 0xc8);
    ASSERT(*a, ov45_02255998, ov45_022558BC, 0xc8);
    ASSERT(b, ov45_02255988, ov45_022558BC, 0xc9);
    ASSERT(*b, ov45_022559A8, ov45_022558BC, 0xc9);
    return strnicmp(a, b);
}

void ciChannelTableElementFreeFn(void *element) {
    ChatChannel *channel = element;
    ASSERT(channel, ov45_02255938, ov45_022558F0, 0xd2);
    DWCi_GsFree(channel->topic);
    if (channel->users) {
        TableFree(channel->users);
    }
}

int ciUserTableHashFn(const void *element, int numBuckets) {
    char *nick = (char *)element;
    ASSERT(nick, ov45_02255938, ov45_02255714, 0xe1);
    ASSERT(numBuckets > 0, ov45_02255948, ov45_02255714, 0xe2);
    return ciHashString((const signed char *)nick, numBuckets);
}

int ciUserTableCompareFn(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    ASSERT(a, ov45_02255958, ov45_022557B4, 0xee);
    ASSERT(b, ov45_02255968, ov45_022557B4, 0xef);
    if (!a || !b) {
        if (!a && !b) {
            return 0;
        }
        return !a ? -1 : 1;
    }
    ASSERT(a, ov45_02255978, ov45_022557B4, 0x102);
    ASSERT(*a, ov45_02255998, ov45_022557B4, 0x102);
    ASSERT(b, ov45_02255988, ov45_022557B4, 0x103);
    ASSERT(*b, ov45_022559A8, ov45_022557B4, 0x103);
    return strnicmp(a, b);
}

void ciUserTableElementFreeFn(void *element) {
    ASSERT(element, ov45_02255938, ov45_022558D4, 0x10a);
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
    ASSERT(chat, ov45_022559B8, ov45_02255700, 0x127);
    if (chat->channels) {
        TableFree(chat->channels);
    }
    if (chat->enteringChannels) {
        ArrayFree(chat->enteringChannels);
    }
}

void ciChannelEntering(ChatConnection *chat, const char *channel) {
    ASSERT(chat, ov45_022559B8, ov45_0225573C, 0x134);

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
    ASSERT(chat, ov45_022559B8, ov45_022557A0, 0x14d);
    n = ArrayLength(chat->enteringChannels);
    for (i = 0; i < n; i++) {
        name = ArrayNth(chat->enteringChannels, i);
        ASSERT(name, ov45_022559C8, ov45_022557A0, 0x153);
        if (!strnicmp(name, channel)) {
            return TRUE;
        }
    }
    return FALSE;
}

void ciChannelEntered(ChatConnection *chat, const char *name, const ChatChannelCallbacks *callbacks) {
    char *topic;
    ASSERT(chat, ov45_022559B8, ov45_0225569C, 0x160);
    ASSERT_CHANNEL(name, ov45_0225569C, 0x162);
    ASSERT(callbacks, ov45_02255A18, ov45_0225569C, 0x163);
    topic = DWCi_GsMalloc(2);
    if (!topic) {
        return;
    }
    *topic = *(const u8 *)ov45_02255A2C;
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
    ASSERT(chat, ov45_022559B8, ov45_02255678, 0x189);
    ASSERT_CHANNEL(name, ov45_02255678, 0x18b);
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
    ASSERT(chat, ov45_022559B8, ov45_02255844, 0x1a9);
    ASSERT_CHANNEL(name, ov45_02255844, 0x1ab);
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
    ASSERT(user, ov45_02255938, ov45_0225585C, 444);
    ASSERT(list, ov45_02255A30, ov45_0225585C, 445);
    ASSERT(user, ov45_02255A44, ov45_0225585C, 450);
    ASSERT(user->nick[0], ov45_02255A58, ov45_0225585C, 450);
    ASSERT(strlen(user->nick) < 128, ov45_02255A70, ov45_0225585C, 450);
    ASSERT(list->count >= 0, ov45_02255A90, ov45_0225585C, 455);
    p = DWCi_GsRealloc(list->users, (list->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(ov45_02255AA4, ov45_02255918, ov45_0225585C, 473);
        return;
    }
    list->users = p;
    p = DWCi_GsRealloc(list->modes, (list->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(ov45_02255AA4, ov45_02255918, ov45_0225585C, 480);
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
    ASSERT(chat, ov45_022559B8, ov45_02255778, 496);
    ASSERT_CHANNEL(name, ov45_02255778, 498);
    ASSERT(callback, ov45_02255AA8, ov45_02255778, 499);
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
    ASSERT(chat, ov45_022559B8, ov45_0225565C, 0x20f);
    if (ciGetChannel(chat, name) == NULL) {
        return FALSE;
    }
    return TRUE;
}

BOOL ciGetChannelMode(ChatConnection *chat, const char *name, void *mode) {
    ChatChannel *channel;
    ASSERT(chat, ov45_022559B8, ov45_022556B0, 0x21a);
    ASSERT_CHANNEL(name, ov45_022556B0, 0x21c);
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
    ASSERT(chat, ov45_022559B8, ov45_02255688, 0x233);
    ASSERT_CHANNEL(name, ov45_02255688, 0x235);
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
    ASSERT(chat, ov45_022559B8, ov45_022557CC, 0x24a);
    ASSERT_CHANNEL(name, ov45_022557CC, 0x24c);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    DWCi_GsFree(channel->topic);
    if (!topic) {
        topic = ov45_02255A2C;
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
    ASSERT(chat, ov45_022559B8, ov45_022557E4, 0x278);
    ASSERT_CHANNEL(name, ov45_022557E4, 0x27a);
    channel = ciGetChannel(chat, name);
    if (channel) {
        channel->joined = TRUE;
    }
}

BOOL ciWasJoinCallbackCalled(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, ov45_022559B8, ov45_022558A4, 0x28a);
    ASSERT_CHANNEL(name, ov45_022558A4, 0x28c);
    channel = ciGetChannel(chat, name);
    if (channel == NULL) {
        return FALSE;
    }
    return channel->joined;
}

void ciSetChannelTopic(ChatConnection *chat, const char *name, const char *key) {
    ChatChannel *channel;
    ASSERT(chat, ov45_022559B8, ov45_02255728, 0x29a);
    ASSERT_CHANNEL(name, ov45_02255728, 0x29c);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return;
    }
    strncpy(channel->key, key, 128);
    channel->key[127] = 0;
}

int ciGetChannelNumUsers(ChatConnection *chat, const char *name) {
    ChatChannel *channel;
    ASSERT(chat, ov45_022559B8, ov45_02255814, 0x2bc);
    ASSERT_CHANNEL(name, ov45_02255814, 0x2be);
    channel = ciGetChannel(chat, name);
    if (!channel) {
        return -1;
    }
    return TableCount(channel->users);
}

void ciUserEnteredChannel(ChatConnection *chat, const char *nick, const char *name, int mode, const char *user, const char *address) {
    ChatChannel *channel;
    ASSERT(chat != NULL, ov45_022559B8, ov45_0225582C, 720);
    ASSERT(nick != NULL, ov45_02255ABC, ov45_0225582C, 722);
    ASSERT(*nick != 0, ov45_02255ACC, ov45_0225582C, 722);
    ASSERT(strlen(nick) < 128, ov45_02255ADC, ov45_0225582C, 722);
    ASSERT(name != NULL, ov45_022559D4, ov45_0225582C, 723);
    ASSERT(*name != 0, ov45_022559E4, ov45_0225582C, 723);
    ASSERT(mode >= 0 && mode <= 3, ov45_02255AF4, ov45_0225582C, 724);
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
        ASSERT(TableLookup(channel->users, &entry) != NULL, ov45_02255B10, ov45_0225582C, 759);
    }
}

void ciUserLeftChannel(ChatConnection *chat, const char *nick, const char *name) {
    char key[sizeof(ChatChannelUser)];
    ChatChannel *channel;
    ASSERT(chat, ov45_022559B8, ov45_022556EC, 0x2fe);
    ASSERT_NICK(nick, ov45_022556EC, 0x300);
    ASSERT(name, ov45_022559D4, ov45_022556EC, 0x301);
    ASSERT(*name, ov45_022559E4, ov45_022556EC, 0x301);
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
    ASSERT(channel, ov45_02255938, ov45_0225588C, 0x318);
    ASSERT(search, ov45_02255A30, ov45_0225588C, 0x319);
    ASSERT(channel->users, ov45_02255B7C, ov45_0225588C, 0x31e);
    ASSERT(search->nick, ov45_02255B94, ov45_0225588C, 0x323);
    ASSERT(*search->nick, ov45_02255BA8, ov45_0225588C, 0x324);
    ASSERT(search->callback, ov45_02255BC4, ov45_0225588C, 0x325);
    if (!TableLookup(channel->users, search->nick)) {
        return;
    }
    search->callback(search->chat, search->nick, channel, search->param);
}

void ciUserEnumChannels(ChatConnection *chat, const char *nick, ChatUserSearchFunc callback, void *param) {
    ChatUserSearch search;
    ASSERT(chat, ov45_022559B8, ov45_02255750, 0x336);
    ASSERT_NICK(nick, ov45_02255750, 0x338);
    ASSERT(callback, ov45_02255AA8, ov45_02255750, 0x339);
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
    ASSERT(channel, ov45_02255938, ov45_0225578C, 0x34f);
    ASSERT(change, ov45_02255A30, ov45_0225578C, 0x350);
    ASSERT(channel->users, ov45_02255B7C, ov45_0225578C, 0x355);
    ASSERT(change->newNick, ov45_02255BDC, ov45_0225578C, 0x35a);
    ASSERT(*change->newNick, ov45_02255BF4, ov45_0225578C, 0x35a);
    ASSERT(strlen(change->newNick) < 128, ov45_02255C10, ov45_0225578C, 0x35a);
    ASSERT(change->oldNick, ov45_02255C34, ov45_0225578C, 0x35b);
    ASSERT(*change->oldNick, ov45_02255C4C, ov45_0225578C, 0x35b);
    ASSERT(strlen(change->oldNick) < 128, ov45_02255C68, ov45_0225578C, 0x35b);
    found = TableLookup(channel->users, change->oldNick);
    if (!found) {
        return;
    }
    memcpy(entry, found, sizeof(entry));
    ASSERT(TableRemove(channel->users, found), ov45_02255C8C, ov45_0225578C, 0x367);
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
    ASSERT(chat, ov45_022559B8, ov45_022556C4, 0x386);
    ASSERT(oldNick, ov45_02255C98, ov45_022556C4, 0x388);
    ASSERT(*oldNick, ov45_02255CA8, ov45_022556C4, 0x388);
    ASSERT(strlen(oldNick) < 128, ov45_02255CBC, ov45_022556C4, 0x388);
    ASSERT(newNick, ov45_02255CD8, ov45_022556C4, 0x389);
    ASSERT(*newNick, ov45_02255CE8, ov45_022556C4, 0x389);
    ASSERT(strlen(newNick) < 128, ov45_02255CFC, ov45_022556C4, 0x389);
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
    ASSERT(chat, ov45_022559B8, ov45_022556D8, 923);
    ASSERT_NICK(nick, ov45_022556D8, 925);
    ASSERT(name, ov45_022559D4, ov45_022556D8, 926);
    ASSERT(*name, ov45_022559E4, ov45_022556D8, 926);
    ASSERT(mode >= 0 && mode <= 3, ov45_02255AF4, ov45_022556D8, 927);
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
    ASSERT(channel, ov45_02255938, ov45_02255874, 0x41a);
    ASSERT(info, ov45_02255A30, ov45_02255874, 0x41b);
    ASSERT(channel->users, ov45_02255B7C, ov45_02255874, 0x424);
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
    ASSERT(chat, ov45_022559B8, ov45_02255764, 1085);
    ASSERT(nick, ov45_02255D18, ov45_02255764, 1087);
    ASSERT(*nick != 0, ov45_02255D28, ov45_02255764, 1087);
    ASSERT(strlen(nick) < 128, ov45_02255D38, ov45_02255764, 1087);
    strncpy(buffer, nick, 128);
    buffer[127] = 0;
    info.user = user;
    info.address = address;
    info.nick = buffer;
    TableMap(chat->channels, ciSetUserBasicInfoMap, &info);
}
