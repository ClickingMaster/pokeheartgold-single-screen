#pragma force_active on
#include <string.h>

#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

// Handlers of the lines received from the chat server (the command table): messages, the user and channel
// commands, errors and invitations, the channel user list, topic and WHO replies, and nick errors. The server
// command of each handler is noted above it.

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern char *strchr(const char *, int);
extern char *strtok(char *, const char *);
extern const char ov45_02255E84[];
extern const char ov45_02255E94[];
extern const char ov45_02255EA4[];
extern const char ov45_02255ED4[];
extern const char ov45_02255EE4[];
extern const char ov45_02255EF4[];
extern const char ov45_02255F04[];
extern const char ov45_02255F14[];
extern const char ov45_02255F24[];
extern const char ov45_02255F34[];
extern const char ov45_02255F44[];
extern const char ov45_02255F54[];
extern const char ov45_02255F64[];
extern const char ov45_02255FA4[];
extern const char ov45_02255FB4[];
extern const char ov45_02255FE8[];
extern const char ov45_0225604C[];
extern const char ov45_02256088[];
extern const char ov45_022560D8[];
extern const char ov45_02256100[];
extern const char ov45_02256140[];
extern const char ov45_02256158[];
extern const char ov45_022561D0[];
extern const char ov45_02256324[];
extern const char ov45_02256340[];
extern const char ov45_022565C0[];
extern const char ov45_022565D0[];
extern const char ov45_02256690[];
extern const char ov45_022566F0[];
extern const char ov45_02256708[];
extern const char ov45_0225670C[];
extern const char ov45_02256714[];
extern const char ov45_0225671C[];
extern const char ov45_02256734[];
extern const char ov45_02256750[];
extern const char ov45_02256760[];
extern const char ov45_02256768[];
extern const char ov45_022567A0[];
extern const char ov45_022567B0[];
extern const char ov45_022567C0[];
extern const char ov45_022567D0[];
extern const char ov45_022567E4[];
extern const char ov45_022567F4[];
extern const char ov45_0225680C[];
extern const char ov45_02256824[];
extern const char ov45_02256828[];
extern const char ov45_02256838[];
extern const char ov45_02256848[];
extern const char ov45_02256860[];
extern const char ov45_02256864[];
extern const char ov45_0225687C[];
extern const char ov45_02256894[];
extern const char ov45_0225689C[];

#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, ov45_022565D0, func, line); \
    }
#define ASSERT_CHAT(chat, func, line) ASSERT(chat, ov45_022565C0, func, line)

#define NICK_LIST_GROWTH 100

// PRIVMSG: a message to a channel or to the own user; a CTCP ACTION becomes an action message.
void ov45_0224B0FC(ChatConnection *chat, ChatMessage *message) {
    ChatPrivateMessageArgs privateArgs;
    ChatChannelMessageArgs channelArgs;
    const char *ctcp;
    char *target;
    char *nick;
    int action = 0;
    char *text;
    int length;
    ASSERT_CHAT(chat, ov45_02255FE8, 0x57C);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255FE8, 0x582);
    if (message->paramCount != 2) {
        return;
    }
    target = message->params[0];
    text = message->params[1];
    nick = message->nick;
    length = strlen(text);
    ctcp = ov45_02256708;
    if ((s8)text[0] == 1 && (((s8)text[1] >= 'A' && (s8)text[1] <= 'Z') || ((s8)text[1] >= 'a' && (s8)text[1] <= 'z')) && (s8)text[length - 1] == 1) {
        char *space;
        text[length - 1] = 0;
        space = strchr(text, ' ');
        if (space) {
            *space = 0;
            ctcp = text + 1;
            text = space + 1;
        }
    }
    if (!strcmp(ctcp, ov45_0225670C)) {
        action = 1;
    } else if ((s8)*ctcp) {
        return;
    }
    if (!strnicmp(target, chat->nick)) {
        if (!chat->globalCallbacks.privateMessage) {
            return;
        }
        if (action) {
            privateArgs.type = CHAT_MESSAGE_ACTION;
        } else {
            privateArgs.type = CHAT_MESSAGE_NORMAL;
        }
        privateArgs.nick = nick;
        privateArgs.message = text;
        ov45_02244D30(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
        return;
    }
    {
        ChatChannelCallbacks *callbacks = ov45_02247E98(chat, target);
        if (!callbacks || !callbacks->message) {
            return;
        }
        if (action) {
            channelArgs.type = CHAT_MESSAGE_ACTION;
        } else {
            channelArgs.type = CHAT_MESSAGE_NORMAL;
        }
        channelArgs.channel = target;
        channelArgs.sender = nick;
        channelArgs.message = text;
        ov45_02244D30(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
    }
}

// NOTICE
void ov45_0224B310(ChatConnection *chat, ChatMessage *message) {
    char *target, *text;
    ChatPrivateMessageArgs privateArgs;
    ChatChannelMessageArgs channelArgs;
    ChatChannelCallbacks *callbacks;
    ASSERT_CHAT(chat, ov45_02255FA4, 1514);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255FA4, 1520);
    if (message->paramCount != 2) {
        return;
    }
    target = message->params[0];
    text = message->params[1];
    if (strnicmp(target, chat->nick) == 0) {
        if (!chat->globalCallbacks.privateMessage) {
            return;
        }
        if (message->nick) {
            privateArgs.nick = message->nick;
        } else {
            privateArgs.nick = NULL;
        }
        privateArgs.message = text;
        privateArgs.type = CHAT_MESSAGE_NOTICE;
        ov45_02244D30(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
    } else {
        callbacks = ov45_02247E98(chat, target);
        if (!callbacks || !callbacks->message) {
            return;
        }
        channelArgs.channel = target;
        if (message->nick) {
            channelArgs.sender = message->nick;
        } else {
            channelArgs.sender = NULL;
        }
        channelArgs.message = text;
        channelArgs.type = CHAT_MESSAGE_NOTICE;
        ov45_02244D30(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
    }
}

// UTM
void ov45_0224B474(ChatConnection *chat, ChatMessage *message) {
    char *target, *text;
    ChatChannelCallbacks *callbacks;
    ChatPrivateMessageArgs privateArgs;
    ChatChannelMessageArgs channelArgs;
    ASSERT_CHAT(chat, ov45_02255E94, 1567);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255E94, 1573);
    if (message->paramCount != 2) {
        return;
    }
    target = message->params[0];
    text = message->params[1];
    if (!strnicmp(target, chat->nick)) {
        if (!chat->globalCallbacks.privateMessage) {
            return;
        }
        if (message->nick) {
            privateArgs.nick = message->nick;
        } else {
            privateArgs.nick = NULL;
        }
        privateArgs.type = CHAT_MESSAGE_UTM;
        privateArgs.message = text;
        ov45_02244D30(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
        return;
    }
    callbacks = ov45_02247E98(chat, target);
    if (!callbacks || !callbacks->message) {
        return;
    }
    channelArgs.channel = target;
    if (message->nick) {
        channelArgs.sender = message->nick;
    } else {
        channelArgs.sender = NULL;
    }
    channelArgs.type = CHAT_MESSAGE_UTM;
    channelArgs.message = text;
    ov45_02244D30(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
}

// ATM
void ov45_0224B5E0(ChatConnection *chat, ChatMessage *message) {
    ChatPrivateMessageArgs privateArgs;
    ChatChannelMessageArgs channelArgs;
    char *target;
    char *text;
    ChatChannelCallbacks *callbacks;
    ASSERT_CHAT(chat, ov45_02255EA4, 1620);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255EA4, 1626);
    if (message->paramCount != 2) {
        return;
    }
    target = message->params[0];
    text = message->params[1];
    if (strnicmp(target, chat->nick) == 0) {
        if (!chat->globalCallbacks.privateMessage) {
            return;
        }
        if (message->nick) {
            privateArgs.nick = message->nick;
        } else {
            privateArgs.nick = NULL;
        }
        privateArgs.type = CHAT_MESSAGE_ATM;
        privateArgs.message = text;
        ov45_02244D30(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
    } else {
        callbacks = ov45_02247E98(chat, target);
        if (!callbacks || !callbacks->message) {
            return;
        }
        channelArgs.channel = target;
        if (message->nick) {
            channelArgs.sender = message->nick;
        } else {
            channelArgs.sender = NULL;
        }
        channelArgs.message = text;
        channelArgs.type = CHAT_MESSAGE_ATM;
        ov45_02244D30(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
    }
}

// PING: answered with a PONG.
void ov45_0224B748(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02255ED4, 1671);
    ov45_02252F74(&chat->socket, ov45_02256714, message->trailing);
}

// NICK: a user (maybe the own one) changed the nick.
void ov45_0224B794(ChatConnection *chat, ChatMessage *message) {
    char *oldNick, *newNick;
    ChatRequest *request;
    ASSERT_CHAT(chat, ov45_02255EE4, 1684);
    ASSERT(message->paramCount == 1, ov45_0225671C, ov45_02255EE4, 1690);
    if (message->paramCount != 1) {
        return;
    }
    oldNick = message->nick;
    newNick = message->params[0];
    if (!strnicmp(oldNick, chat->nick)) {
        ASSERT(strlen(newNick) < sizeof(chat->nick), ov45_02256734, ov45_02255EE4, 1706);
        strncpy(chat->nick, newNick, sizeof(chat->nick));
        chat->nick[sizeof(chat->nick) - 1] = 0;
        ChatRequestKey key = { 0 };
        u32 results[3];
        key.type = 9;
        key.name = oldNick;
        key.name2 = newNick;
        request = ov45_022496AC(chat, 1, &key);
        if (request) {
            results[0] = TRUE;
            results[1] = (u32)oldNick;
            results[2] = (u32)newNick;
            ov45_02249A0C(chat, request, results);
        }
    }
    ov45_02249054(chat, oldNick, newNick);
}

// JOIN: a user entered a channel; for the own user this completes the enter request.
void ov45_0224B8D0(ChatConnection *chat, ChatMessage *message) {
    char *channel;
    char *nick;
    char *user, *host;
    ChatEnterData *enter;
    int mode;
    ChatRequestKey key;
    ChatUserJoinedArgs args;
    char *usersChangedArg;
    ChatRequest *request;
    ASSERT_CHAT(chat, ov45_02255EF4, 1750);
    ASSERT(message->paramCount == 1, ov45_0225671C, ov45_02255EF4, 1756);
    if (message->paramCount != 1) {
        return;
    }
    nick = message->nick;
    channel = *message->params;
    user = message->user;
    host = message->host;
    if (*nick == '@') {
        nick++;
        mode = 2;
        ASSERT(*nick, ov45_02256750, ov45_02255EF4, 1771);
    } else if (*nick == '+') {
        nick++;
        mode = 1;
        ASSERT(*nick, ov45_02256750, ov45_02255EF4, 1777);
    } else {
        mode = 0;
    }
    if (!strcmp(nick, chat->nick)) {
        if (!ov45_02247AE0(chat, channel)) {
            return;
        }
        memset(&key, 0, sizeof(key));
        key.type = 1;
        key.name = channel;
        request = ov45_022496AC(chat, 1, &key);
        if (!request) {
            return;
        }
        enter = request->data;
        ov45_02247B94(chat, channel, &enter->callbacks);
        ov45_02248454(chat, channel, enter->password);
        enter->joined = TRUE;
        ov45_02252F74(&chat->socket, ov45_02256760, channel);
        return;
    }
    if (ov45_02248238(chat, channel)) {
        ov45_02248888(chat, nick, channel, mode, user, host);
    }
    if (!ov45_0224861C(chat, channel)) {
        return;
    }
    ChatChannelCallbacks *callbacks = ov45_02247E98(chat, channel);
    if (!callbacks) {
        return;
    }
    if (callbacks->userJoined) {
        args.channel = channel;
        args.nick = nick;
        args.mode = mode;
        ov45_02244D30(chat, 6, callbacks->userJoined, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
    }
    if (!callbacks->usersChanged) {
        return;
    }
    usersChangedArg = channel;
    ov45_02244D30(chat, 12, callbacks->usersChanged, (u32 *)&usersChangedArg, callbacks->param, 0, channel, sizeof(usersChangedArg));
}

static inline BOOL IsKickParamCount(int count) {
    BOOL valid = TRUE;
    if (count != 2 && count != 3) {
        valid = FALSE;
    }
    return valid;
}

// PART: a user left a channel.
void ov45_0224BB4C(ChatConnection *chat, ChatMessage *message) {
    ChatMessage *msg = message;
    ChatUserLeftArgs args;
    char *usersChangedArg;
    char *nick, *channel;
    ChatChannelCallbacks *callbacks;
    ChatConnection *c = chat;
    const char *reason;
    ASSERT_CHAT(c, ov45_02255F04, 0x752);
    nick = msg->nick;
    channel = msg->params[0];
    reason = msg->paramCount > 1 ? msg->params[1] : ov45_02256708;
    if (!strcmp(nick, c->nick)) {
        return;
    }
    ov45_02248A98(c, nick, channel);
    if (!ov45_0224861C(c, channel)) {
        return;
    }
    callbacks = ov45_02247E98(c, channel);
    if (!callbacks) {
        return;
    }
    if (callbacks->userLeft) {
        args.channel = channel;
        args.nick = nick;
        args.reason = CHAT_LEFT_PART;
        args.message = reason;
        args.kicker = NULL;
        ov45_02244D30(c, 7, callbacks->userLeft, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
    }
    if (callbacks->usersChanged) {
        usersChangedArg = channel;
        ov45_02244D30(c, 12, callbacks->usersChanged, (u32 *)&usersChangedArg, callbacks->param, 0, channel, sizeof(usersChangedArg));
    }
}

// KICK: a user (maybe the own one) was kicked from a channel.
void ov45_0224BC8C(ChatConnection *chat, ChatMessage *message) {
    ChatKickedArgs kickedArgs;
    ChatUserLeftArgs args;
    char *usersChangedArg;
    char *channel, *kicker, *nick;
    const char *reason;
    char **params;
    int count;
    ChatChannelCallbacks *callbacks;
    ASSERT_CHAT(chat, ov45_02255F14, 0x794);
    ASSERT(IsKickParamCount(message->paramCount), ov45_02256768, ov45_02255F14, 0x79A);
    count = message->paramCount;
    if (count != 2 && count != 3) {
        return;
    }
    params = message->params;
    channel = params[0];
    nick = params[1];
    kicker = message->nick;
    reason = count == 3 ? params[2] : ov45_02256708;
    ov45_02248A98(chat, nick, channel);
    callbacks = ov45_02247E98(chat, channel);
    if (!callbacks) {
        return;
    }
    if (!strnicmp(nick, chat->nick)) {
        if (callbacks->kicked) {
            kickedArgs.channel = channel;
            kickedArgs.kicker = kicker;
            kickedArgs.reason = reason;
            ov45_02244D30(chat, 5, callbacks->kicked, (u32 *)&kickedArgs, callbacks->param, 0, NULL, sizeof(kickedArgs));
        }
        ov45_02247D84(chat, channel);
        return;
    }
    if (!ov45_0224861C(chat, channel)) {
        return;
    }
    if (callbacks->userLeft) {
        args.channel = channel;
        args.nick = nick;
        args.reason = CHAT_LEFT_KICK;
        args.message = reason;
        args.kicker = kicker;
        ov45_02244D30(chat, 7, callbacks->userLeft, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
    }
    if (callbacks->usersChanged) {
        usersChangedArg = channel;
        ov45_02244D30(chat, 12, callbacks->usersChanged, (u32 *)&usersChangedArg, callbacks->param, 0, channel, sizeof(usersChangedArg));
    }
}

// Per channel of a user who quit (ov45_02248CE8 callback, param = the quit message).
void ov45_0224BE74(ChatConnection *chat, const char *nick, ChatChannel *channel, void *param) {
    ChatChannelCallbacks *callbacks;
    u32 args[5], usersChangedArg;
    ASSERT(nick, ov45_022567A0, ov45_02256324, 2022);
    ASSERT(*nick, ov45_022567B0, ov45_02256324, 2022);
    ASSERT(channel, ov45_022567C0, ov45_02256324, 2023);
    ASSERT(channel->name[0], ov45_022567D0, ov45_02256324, 2023);
    ASSERT(param, ov45_022567E4, ov45_02256324, 2024);
    ov45_02248A98(chat, nick, channel->name);
    if (!ov45_0224861C(chat, channel->name)) {
        return;
    }
    callbacks = ov45_02247E98(chat, channel->name);
    if (!callbacks) {
        return;
    }
    if (callbacks->userLeft) {
        args[2] = CHAT_LEFT_QUIT;
        args[0] = (u32)channel;
        args[1] = (u32)nick;
        args[3] = (u32)param;
        args[4] = 0;
        ov45_02244D30(chat, 7, callbacks->userLeft, args, callbacks->param, 0, channel->name, sizeof(args));
    }
    if (!callbacks->usersChanged) {
        return;
    }
    usersChangedArg = (u32)channel;
    ov45_02244D30(chat, 12, callbacks->usersChanged, &usersChangedArg, callbacks->param, 0, channel->name, sizeof(usersChangedArg));
}

// QUIT: a user left the server.
void ov45_0224C018(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02255F24, 2067);
    ASSERT(message->paramCount == 1, ov45_0225671C, ov45_02255F24, 2073);
    if (message->paramCount != 1) {
        return;
    }
    ov45_02248CE8(chat, message->nick, ov45_0224BE74, message->params[0]);
}

// Per channel of a user who was killed (ov45_02248CE8 callback, param = the reason).
void ov45_0224C0A0(ChatConnection *chat, const char *nick, ChatChannel *channel, void *param) {
    ChatChannelCallbacks *callbacks;
    u32 args[5], usersChangedArg;
    ASSERT(nick, ov45_022567A0, ov45_02256340, 2088);
    ASSERT(*nick, ov45_022567B0, ov45_02256340, 2088);
    ASSERT(channel, ov45_022567C0, ov45_02256340, 2089);
    ASSERT(channel->name[0], ov45_022567D0, ov45_02256340, 2089);
    ASSERT(param, ov45_022567E4, ov45_02256340, 2090);
    ov45_02248A98(chat, nick, channel->name);
    if (!ov45_0224861C(chat, channel->name)) {
        return;
    }
    callbacks = ov45_02247E98(chat, channel->name);
    if (!callbacks) {
        return;
    }
    if (callbacks->userLeft) {
        args[2] = CHAT_LEFT_KILL;
        args[0] = (u32)channel;
        args[1] = (u32)nick;
        args[3] = (u32)param;
        args[4] = 0;
        ov45_02244D30(chat, 7, callbacks->userLeft, args, callbacks->param, 0, channel->name, sizeof(args));
    }
    if (!callbacks->usersChanged) {
        return;
    }
    usersChangedArg = (u32)channel;
    ov45_02244D30(chat, 12, callbacks->usersChanged, &usersChangedArg, callbacks->param, 0, channel->name, sizeof(usersChangedArg));
}

// KILL
void ov45_0224C244(ChatConnection *chat, ChatMessage *message) {
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255F34, 2137);
    if (message->paramCount != 2) {
        return;
    }
    ov45_02248CE8(chat, message->params[0], ov45_0224C0A0, message->params[1]);
}

// TOPIC: the topic of a channel changed.
void ov45_0224C2A8(ChatConnection *chat, ChatMessage *message) {
    char *channel, *topic;
    ChatChannelCallbacks *callbacks;
    ChatTopicArgs args;
    ASSERT_CHAT(chat, ov45_02255F54, 2154);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255F54, 2160);
    if (message->paramCount != 2) {
        return;
    }
    channel = message->params[0];
    topic = message->params[1];
    ov45_022486E0(chat, channel, topic);
    callbacks = ov45_02247E98(chat, channel);
    if (!callbacks || !callbacks->topicChanged) {
        return;
    }
    args.channel = channel;
    args.topic = topic;
    ov45_02244D30(chat, 9, callbacks->topicChanged, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
}

// MODE: the mode of a channel or of its users changed.
void ov45_0224C388(ChatConnection *chat, ChatMessage *message) {
    ChatModeChange *changes, *change;
    char *channel, *modes;
    ChatChannelMode mode;
    ASSERT(message->paramCount >= 2, ov45_022567F4, ov45_02255F44, 0x893);
    if (message->paramCount < 2) {
        return;
    }
    channel = message->params[0];
    modes = message->params[1];
    if (ov45_02248238(chat, channel) == 0) {
        return;
    }
    changes = ov45_0224AD24(modes, (const char **)message->params + 2, message->paramCount - 2);
    if (changes == NULL) {
        return;
    }
    change = changes;
    while (change->type != CHAT_MODE_END) {
        switch (change->type) {
        case CHAT_MODE_KEY:
            if (change->set) {
                ov45_02248454(chat, channel, change->arg);
            } else {
                ov45_02248454(chat, channel, NULL);
            }
            break;
        case CHAT_MODE_OP:
            if (change->arg != NULL) {
                ov45_02249194(chat, change->arg, channel, 2, change->set);
            }
            break;
        case CHAT_MODE_VOICE:
            if (change->arg != NULL) {
                ov45_02249194(chat, change->arg, channel, 1, change->set);
            }
            break;
        case CHAT_MODE_BAN:
        case CHAT_MODE_INVITE_ONLY:
        case CHAT_MODE_LIMIT:
        case CHAT_MODE_PRIVATE:
        case CHAT_MODE_SECRET:
        case CHAT_MODE_MODERATED:
        case CHAT_MODE_NO_EXTERNAL_MESSAGES:
        case CHAT_MODE_ONLY_OPS_CHANGE_TOPIC:
        case CHAT_MODE_U:
        case CHAT_MODE_W:
        case CHAT_MODE_E:
            break;
        default:
            __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02255F44, 0x8DC);
            break;
        }
        change++;
    }
    if (ov45_02248288(chat, channel, &mode) == 0) {
        ov00_021EC3D8(changes);
        return;
    }
    ov45_0224AFD8(&mode, changes);
    ov45_02248374(chat, channel, &mode);
    ov00_021EC3D8(changes);
}

// ERROR: the server closes the connection.
void ov45_0224C594(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02255F64, 2289);
    ASSERT(message->paramCount == 1, ov45_0225671C, ov45_02255F64, 2295);
    if (message->paramCount != 1) {
        return;
    }
    ov45_0224F69C(chat, message->params[0]);
}

// INVITE: the own user was invited into a channel.
void ov45_0224C610(ChatConnection *chat, ChatMessage *message) {
    char *channel, *nick;
    ChatInviteArgs args;
    ASSERT_CHAT(chat, ov45_02255FB4, 2308);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255FB4, 2314);
    if (message->paramCount != 2) {
        return;
    }
    nick = message->nick;
    channel = message->params[1];
    if (!chat->globalCallbacks.unk_0C) {
        return;
    }
    args.channel = channel;
    args.nick = nick;
    ov45_02244D30(chat, 3, chat->globalCallbacks.unk_0C, (u32 *)&args, chat->globalCallbacks.param, 0, NULL, sizeof(args));
}

// 353: a part of the user list of a channel (nicks with "@" for operators and "+" for voiced users).
void ov45_0224C6DC(ChatConnection *chat, ChatMessage *message) {
    ChatRequest *request;
    char *channel;
    ChatNickListData *list = NULL;
    ASSERT(message->paramCount == 4, ov45_0225680C, ov45_02256088, 2351);
    if (message->paramCount != 4) {
        return;
    }
    channel = message->params[2];
    char *nicks = message->params[3];
    ChatRequestKey keys[4] = { 0 };
    keys[0].type = 1;
    keys[0].name = channel;
    keys[1].type = 15;
    keys[1].name = channel;
    keys[2].type = 3;
    keys[2].name = channel;
    keys[3].type = 3;
    request = ov45_022496AC(chat, 4, keys);
    if (!request) {
        return;
    }
    if (request->type != 1) {
        list = request->data;
    }
    char *nick = strtok(nicks, ov45_02256824);
    if (!nick) {
        return;
    }
    do {
        ASSERT(*nick, ov45_02256828, ov45_02256088, 2385);
        int mode;
        if (*nick == '@') {
            ASSERT(nick[1], ov45_02256838, ov45_02256088, 2391);
            mode = 2;
            nick++;
        } else if (*nick == '+') {
            ASSERT(nick[1], ov45_02256838, ov45_02256088, 2397);
            mode = 1;
            nick++;
        } else {
            mode = 0;
        }
        if (request->type != 1) {
            if (list->count == list->capacity) {
                char **newNicks = ov00_021EC3BC(list->nicks, (list->capacity + NICK_LIST_GROWTH) * sizeof(char *));
                if (!newNicks) {
                    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2413);
                    return;
                }
                list->nicks = newNicks;
                int *newModes = ov00_021EC3BC(list->modes, (list->capacity + NICK_LIST_GROWTH) * sizeof(int));
                if (!newModes) {
                    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2420);
                    return;
                }
                list->modes = newModes;
                list->capacity += NICK_LIST_GROWTH;
            }
            u32 length = strlen(nick);
            char *copy = ov00_021EC3A8(length + 1);
            if (!copy) {
                __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2433);
                return;
            }
            memcpy(copy, nick, length + 1);
            list->nicks[list->count] = copy;
            list->modes[list->count] = mode;
            list->count++;
        }
        if (request->type == 1 || request->type == 15) {
            ov45_02248888(chat, nick, channel, mode, NULL, NULL);
        }
        nick = strtok(NULL, ov45_02256824);
    } while (nick);
}

// 366: end of the user list of a channel ("*" = all channels).
void ov45_0224C9D8(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey keys[4];

    struct {
        BOOL success;
        int unk_04;
        char *channel;
    } enterResult;

    struct {
        char *channel;
        int count;
        char **nicks;
        int *modes;
    } nickListResult;

    struct {
        BOOL success;
        char *channel;
        int count;
        char **nicks;
        int *modes;
    } usersResult;

    ChatRequest *request;
    char *channel;
    ASSERT_CHAT(chat, ov45_022560D8, 0x99F);
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_022560D8, 0x9A5);
    if (message->paramCount != 3) {
        return;
    }
    channel = message->params[1];
    if (strcmp(channel, ov45_02256860) == 0) {
        channel = NULL;
    }
    memset(keys, 0, sizeof(keys));
    keys[0].type = 1;
    keys[0].name = channel;
    keys[1].type = 15;
    keys[1].name = channel;
    keys[2].type = 3;
    keys[2].name = channel;
    keys[3].type = 3;
    request = ov45_022496AC(chat, 4, keys);
    if (!request) {
        return;
    }
    if (request->type == 1) {
        enterResult.success = TRUE;
        enterResult.unk_04 = 0;
        enterResult.channel = channel;
        if (request->callback == NULL) {
            ov45_02248558(chat, channel);
        }
        ov45_02249A0C(chat, request, (u32 *)&enterResult);
    } else if (request->type == 15) {
        ChatNickListData *list = request->data;
        nickListResult.channel = channel;
        nickListResult.count = list->count;
        nickListResult.nicks = list->nicks;
        nickListResult.modes = list->modes;
        ov45_02249A0C(chat, request, (u32 *)&nickListResult);
    } else if (request->type == 3) {
        ChatNickListData *list = request->data;
        usersResult.success = TRUE;
        usersResult.channel = channel;
        usersResult.count = list->count;
        usersResult.nicks = list->nicks;
        usersResult.modes = list->modes;
        ov45_02249A0C(chat, request, (u32 *)&usersResult);
    }
}

// 332: the topic of a channel (answer to a topic request, or sent when the channel is entered).
void ov45_0224CBAC(ChatConnection *chat, ChatMessage *message) {
    char *channel, *topic;
    ChatRequest *request;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_0225604C, 2565);
    if (message->paramCount != 3) {
        return;
    }
    channel = message->params[1];
    topic = message->params[2];
    ov45_022486E0(chat, channel, topic);
    ChatRequestKey key = { 0 };
    u32 results[3];
    ChatTopicArgs args;
    key.type = 2;
    key.name = channel;
    request = ov45_022496AC(chat, 1, &key);
    if (request) {
        results[0] = TRUE;
        results[1] = (u32)channel;
        results[2] = (u32)topic;
        ov45_02249A0C(chat, request, results);
        return;
    }
    ChatChannelCallbacks *callbacks = ov45_02247E98(chat, channel);
    if (!callbacks || !callbacks->topicChanged) {
        return;
    }
    args.channel = channel;
    args.topic = topic;
    ov45_02244D30(chat, 9, callbacks->topicChanged, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
}

// 331: a channel has no topic.
void ov45_0224CCC8(ChatConnection *chat, ChatMessage *message) {
    char *channel;
    ChatRequest *request;
    ASSERT(message->paramCount >= 2, ov45_022567F4, ov45_02256100, 2620);
    if (message->paramCount < 2) {
        return;
    }
    channel = message->params[1];
    ChatRequestKey key = { 0 };
    u32 results[3];
    ChatTopicArgs args;
    key.type = 2;
    key.name = channel;
    request = ov45_022496AC(chat, 1, &key);
    if (request) {
        results[0] = TRUE;
        results[1] = (u32)channel;
        results[2] = (u32)ov45_02256708;
        ov45_02249A0C(chat, request, results);
        return;
    }
    ChatChannelCallbacks *callbacks = ov45_02247E98(chat, channel);
    if (!callbacks || !callbacks->topicChanged) {
        return;
    }
    args.channel = channel;
    args.topic = ov45_02256708;
    ov45_02244D30(chat, 9, callbacks->topicChanged, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
}

// 433: the nick is already in use (fails a nick change, or reported while connecting).
void ov45_0224CDE0(ChatConnection *chat, ChatMessage *message) {
    char *oldNick, *newNick;
    ChatRequest *request;
    ASSERT_CHAT(chat, ov45_022561D0, 2669);
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_022561D0, 2675);
    if (message->paramCount != 3) {
        return;
    }
    oldNick = message->params[0];
    newNick = message->params[1];
    ChatRequestKey key = { 0 };
    u32 results[3];
    key.type = 9;
    key.name = oldNick;
    key.name2 = newNick;
    request = ov45_022496AC(chat, 1, &key);
    if (request) {
        results[0] = FALSE;
        results[1] = (u32)oldNick;
        results[2] = (u32)newNick;
        ov45_02249A0C(chat, request, results);
        return;
    }
    ASSERT(chat->connecting, ov45_02256864, ov45_022561D0, 2707);
    if (!chat->connecting) {
        return;
    }
    ov45_02251B0C(chat, 1, chat->nick, 0, 0);
}

// 352: one user of a WHO reply (channel, user, host, server, nick, flags "H@+", ...).
void ov45_0224CF0C(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey keys[3];
    u32 modeResult[4];
    u32 addressResult[4];
    u32 channelResult[5];
    char *channel, *nick, *user, *host;
    ChatRequest *request;
    ASSERT(message->paramCount == 8, ov45_0225687C, ov45_02256140, 2726);
    if (message->paramCount != 8) {
        return;
    }
    user = message->params[2];
    host = message->params[3];
    nick = message->params[5];
    channel = message->params[1];
    ov45_02249460(chat, nick, user, host);
    {
        memset(keys, 0, sizeof(keys));
        keys[0].type = 6;
        keys[1].type = 10;
        keys[2].type = 11;
        keys[0].name = nick;
        keys[0].name2 = channel;
        keys[1].name = nick;
        keys[2].name = channel;
        request = ov45_022496AC(chat, 3, keys);
        if (request == NULL) {
            return;
        }
        if (request->type == 6) {
            u32 mode;
            const char *flags;
            flags = message->params[6];
            mode = 0;
            if (strchr(flags, '@') != NULL) {
                mode |= 2;
            }
            if (strchr(flags, '+') != NULL) {
                mode |= 1;
            }
            modeResult[0] = TRUE;
            modeResult[1] = (u32)channel;
            modeResult[2] = (u32)nick;
            modeResult[3] = mode;
            ov45_02249A0C(chat, request, modeResult);
        } else if (request->type == 10) {
            addressResult[0] = TRUE;
            addressResult[1] = (u32)nick;
            addressResult[2] = (u32)user;
            addressResult[3] = (u32)host;
            ov45_02244D30(chat, 21, request->callback, addressResult, request->param, request->id, NULL, sizeof(addressResult));
            request->callback = NULL;
        } else if (request->type == 11 && request->callback != NULL) {
            channelResult[0] = TRUE;
            channelResult[1] = (u32)request->name;
            channelResult[2] = (u32)nick;
            channelResult[3] = (u32)user;
            channelResult[4] = (u32)host;
            ov45_02244D30(chat, 22, request->callback, channelResult, request->param, request->id, NULL, sizeof(channelResult));
        }
    }
}

// 315: end of a WHO reply.
void ov45_0224D128(ChatConnection *chat, ChatMessage *message) {
    char *name;
    ChatRequest *request;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_02256158, 2829);
    if (message->paramCount != 3) {
        return;
    }
    name = message->params[1];
    ChatRequestKey keys[2] = { 0 };
    u32 addressResult[4];
    u32 channelResult[5];
    keys[0].type = 10;
    keys[0].name = name;
    keys[1].type = 11;
    keys[1].name = name;
    request = ov45_022496AC(chat, 2, keys);
    if (!request) {
        return;
    }
    if (request->type == 10) {
        addressResult[0] = FALSE;
        addressResult[1] = (u32)name;
        addressResult[2] = 0;
        addressResult[3] = 0;
        ov45_02249A0C(chat, request, addressResult);
        return;
    }
    if (request->type == 11) {
        channelResult[0] = TRUE;
        channelResult[1] = (u32)name;
        channelResult[2] = 0;
        channelResult[3] = 0;
        channelResult[4] = 0;
        ov45_02249A0C(chat, request, channelResult);
    }
}

// Returns a heap copy of the field after the backslash at text (up to the next backslash or the end) and its
// length with the backslash in *length; NULL when text does not start with a backslash.
char *ov45_0224D240(const char *text, int *length) {
    int n;
    char *field;
    ASSERT(text, ov45_02256894, ov45_02255E84, 2888);
    ASSERT(length, ov45_0225689C, ov45_02255E84, 2889);
    if (!text || *text != '\\') {
        return NULL;
    }
    n = 0;
    while (text[n + 1] && text[n + 1] != '\\') {
        n++;
    }
    field = ov00_021EC3A8(n + 1);
    if (!field) {
        return NULL;
    }
    memcpy(field, text + 1, n);
    field[n] = 0;
    *length = n + 1;
    return field;
}
