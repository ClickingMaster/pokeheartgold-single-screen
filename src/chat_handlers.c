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
extern long atol(const char *);
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
extern const char ov45_02255FFC[];
extern const char ov45_02256024[];
extern const char ov45_02256038[];
extern const char ov45_0225604C[];
extern const char ov45_02256060[];
extern const char ov45_02256074[];
extern const char ov45_02256088[];
extern const char ov45_022560C4[];
extern const char ov45_022560D8[];
extern const char ov45_022560EC[];
extern const char ov45_02256100[];
extern const char ov45_02256114[];
extern const char ov45_02256140[];
extern const char ov45_02256158[];
extern const char ov45_02256170[];
extern const char ov45_02256188[];
extern const char ov45_022561A0[];
extern const char ov45_022561B8[];
extern const char ov45_022561D0[];
extern const char ov45_022561E8[];
extern const char ov45_02256200[];
extern const char ov45_02256218[];
extern const char ov45_02256230[];
extern const char ov45_02256248[];
extern const char ov45_02256260[];
extern const char ov45_0225627C[];
extern const char ov45_02256298[];
extern const char ov45_022562B4[];
extern const char ov45_022562D0[];
extern const char ov45_022562EC[];
extern const char ov45_02256308[];
extern const char ov45_02256324[];
extern const char ov45_02256340[];
extern const char ov45_0225635C[];
extern const char ov45_02256378[];
extern const char ov45_02256394[];
extern const char ov45_022563B0[];
extern const char ov45_022563D0[];
extern const char ov45_022563F0[];
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
extern const char ov45_022568A0[];
extern const char ov45_022568B8[];
extern const char ov45_022568C0[];
extern const char ov45_022568C4[];
extern const char ov45_022568DC[];
extern const char ov45_022568EC[];
extern const char ov45_02256904[];
extern const char ov45_0225691C[];
extern const char ov45_02256934[];
extern const char ov45_02256950[];
extern const char ov45_02256964[];
extern const char ov45_0225697C[];
extern const char ov45_02256990[];
extern const char ov45_02256998[];

#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, ov45_022565D0, func, line); \
    }
#define ASSERT_CHAT(chat, func, line) ASSERT(chat, ov45_022565C0, func, line)

#define NICK_LIST_GROWTH 100

// PRIVMSG: a message to a channel or to the own user; a CTCP ACTION becomes an action message.
void ciPrivmsgHandler(ChatConnection *chat, ChatMessage *message) {
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
        ciAddCallback_(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
        return;
    }
    {
        ChatChannelCallbacks *callbacks = ciGetChannelCallbacks(chat, target);
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
        ciAddCallback_(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
    }
}

// NOTICE
void ciNoticeHandler(ChatConnection *chat, ChatMessage *message) {
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
        ciAddCallback_(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
    } else {
        callbacks = ciGetChannelCallbacks(chat, target);
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
        ciAddCallback_(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
    }
}

// UTM
void ciUTMHandler(ChatConnection *chat, ChatMessage *message) {
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
        ciAddCallback_(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
        return;
    }
    callbacks = ciGetChannelCallbacks(chat, target);
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
    ciAddCallback_(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
}

// ATM
void ciATMHandler(ChatConnection *chat, ChatMessage *message) {
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
        ciAddCallback_(chat, 2, chat->globalCallbacks.privateMessage, (u32 *)&privateArgs, chat->globalCallbacks.param, 0, NULL, sizeof(privateArgs));
    } else {
        callbacks = ciGetChannelCallbacks(chat, target);
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
        ciAddCallback_(chat, 4, callbacks->message, (u32 *)&channelArgs, callbacks->param, 0, target, sizeof(channelArgs));
    }
}

// PING: answered with a PONG.
void ciPingHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02255ED4, 1671);
    ciSocketSendf(&chat->socket, ov45_02256714, message->trailing);
}

// NICK: a user (maybe the own one) changed the nick.
void ciNickHandler(ChatConnection *chat, ChatMessage *message) {
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
        request = ciFindFilter(chat, 1, &key);
        if (request) {
            results[0] = TRUE;
            results[1] = (u32)oldNick;
            results[2] = (u32)newNick;
            ciFinishFilter(chat, request, results);
        }
    }
    ciUserChangedNick(chat, oldNick, newNick);
}

// JOIN: a user entered a channel; for the own user this completes the enter request.
void ciJoinHandler(ChatConnection *chat, ChatMessage *message) {
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
        if (!ciIsEnteringChannel(chat, channel)) {
            return;
        }
        memset(&key, 0, sizeof(key));
        key.type = 1;
        key.name = channel;
        request = ciFindFilter(chat, 1, &key);
        if (!request) {
            return;
        }
        enter = request->data;
        ciChannelEntered(chat, channel, &enter->callbacks);
        ciSetChannelPassword(chat, channel, enter->password);
        enter->joined = TRUE;
        ciSocketSendf(&chat->socket, ov45_02256760, channel);
        return;
    }
    if (ciInChannel(chat, channel)) {
        ciUserEnteredChannel(chat, nick, channel, mode, user, host);
    }
    if (!ciWasJoinCallbackCalled(chat, channel)) {
        return;
    }
    ChatChannelCallbacks *callbacks = ciGetChannelCallbacks(chat, channel);
    if (!callbacks) {
        return;
    }
    if (callbacks->userJoined) {
        args.channel = channel;
        args.nick = nick;
        args.mode = mode;
        ciAddCallback_(chat, 6, callbacks->userJoined, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
    }
    if (!callbacks->usersChanged) {
        return;
    }
    usersChangedArg = channel;
    ciAddCallback_(chat, 12, callbacks->usersChanged, (u32 *)&usersChangedArg, callbacks->param, 0, channel, sizeof(usersChangedArg));
}

static inline BOOL IsKickParamCount(int count) {
    BOOL valid = TRUE;
    if (count != 2 && count != 3) {
        valid = FALSE;
    }
    return valid;
}

// PART: a user left a channel.
void ciPartHandler(ChatConnection *chat, ChatMessage *message) {
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
    ciUserLeftChannel(c, nick, channel);
    if (!ciWasJoinCallbackCalled(c, channel)) {
        return;
    }
    callbacks = ciGetChannelCallbacks(c, channel);
    if (!callbacks) {
        return;
    }
    if (callbacks->userLeft) {
        args.channel = channel;
        args.nick = nick;
        args.reason = CHAT_LEFT_PART;
        args.message = reason;
        args.kicker = NULL;
        ciAddCallback_(c, 7, callbacks->userLeft, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
    }
    if (callbacks->usersChanged) {
        usersChangedArg = channel;
        ciAddCallback_(c, 12, callbacks->usersChanged, (u32 *)&usersChangedArg, callbacks->param, 0, channel, sizeof(usersChangedArg));
    }
}

// KICK: a user (maybe the own one) was kicked from a channel.
void ciKickHandler(ChatConnection *chat, ChatMessage *message) {
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
    ciUserLeftChannel(chat, nick, channel);
    callbacks = ciGetChannelCallbacks(chat, channel);
    if (!callbacks) {
        return;
    }
    if (!strnicmp(nick, chat->nick)) {
        if (callbacks->kicked) {
            kickedArgs.channel = channel;
            kickedArgs.kicker = kicker;
            kickedArgs.reason = reason;
            ciAddCallback_(chat, 5, callbacks->kicked, (u32 *)&kickedArgs, callbacks->param, 0, NULL, sizeof(kickedArgs));
        }
        ciChannelLeft(chat, channel);
        return;
    }
    if (!ciWasJoinCallbackCalled(chat, channel)) {
        return;
    }
    if (callbacks->userLeft) {
        args.channel = channel;
        args.nick = nick;
        args.reason = CHAT_LEFT_KICK;
        args.message = reason;
        args.kicker = kicker;
        ciAddCallback_(chat, 7, callbacks->userLeft, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
    }
    if (callbacks->usersChanged) {
        usersChangedArg = channel;
        ciAddCallback_(chat, 12, callbacks->usersChanged, (u32 *)&usersChangedArg, callbacks->param, 0, channel, sizeof(usersChangedArg));
    }
}

// Per channel of a user who quit (ciUserEnumChannels callback, param = the quit message).
void ciQuitEnumChannelsCallback(ChatConnection *chat, const char *nick, ChatChannel *channel, void *param) {
    ChatChannelCallbacks *callbacks;
    u32 args[5], usersChangedArg;
    ASSERT(nick, ov45_022567A0, ov45_02256324, 2022);
    ASSERT(*nick, ov45_022567B0, ov45_02256324, 2022);
    ASSERT(channel, ov45_022567C0, ov45_02256324, 2023);
    ASSERT(channel->name[0], ov45_022567D0, ov45_02256324, 2023);
    ASSERT(param, ov45_022567E4, ov45_02256324, 2024);
    ciUserLeftChannel(chat, nick, channel->name);
    if (!ciWasJoinCallbackCalled(chat, channel->name)) {
        return;
    }
    callbacks = ciGetChannelCallbacks(chat, channel->name);
    if (!callbacks) {
        return;
    }
    if (callbacks->userLeft) {
        args[2] = CHAT_LEFT_QUIT;
        args[0] = (u32)channel;
        args[1] = (u32)nick;
        args[3] = (u32)param;
        args[4] = 0;
        ciAddCallback_(chat, 7, callbacks->userLeft, args, callbacks->param, 0, channel->name, sizeof(args));
    }
    if (!callbacks->usersChanged) {
        return;
    }
    usersChangedArg = (u32)channel;
    ciAddCallback_(chat, 12, callbacks->usersChanged, &usersChangedArg, callbacks->param, 0, channel->name, sizeof(usersChangedArg));
}

// QUIT: a user left the server.
void ciQuitHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02255F24, 2067);
    ASSERT(message->paramCount == 1, ov45_0225671C, ov45_02255F24, 2073);
    if (message->paramCount != 1) {
        return;
    }
    ciUserEnumChannels(chat, message->nick, ciQuitEnumChannelsCallback, message->params[0]);
}

// Per channel of a user who was killed (ciUserEnumChannels callback, param = the reason).
void ciKillEnumChannelsCallback(ChatConnection *chat, const char *nick, ChatChannel *channel, void *param) {
    ChatChannelCallbacks *callbacks;
    u32 args[5], usersChangedArg;
    ASSERT(nick, ov45_022567A0, ov45_02256340, 2088);
    ASSERT(*nick, ov45_022567B0, ov45_02256340, 2088);
    ASSERT(channel, ov45_022567C0, ov45_02256340, 2089);
    ASSERT(channel->name[0], ov45_022567D0, ov45_02256340, 2089);
    ASSERT(param, ov45_022567E4, ov45_02256340, 2090);
    ciUserLeftChannel(chat, nick, channel->name);
    if (!ciWasJoinCallbackCalled(chat, channel->name)) {
        return;
    }
    callbacks = ciGetChannelCallbacks(chat, channel->name);
    if (!callbacks) {
        return;
    }
    if (callbacks->userLeft) {
        args[2] = CHAT_LEFT_KILL;
        args[0] = (u32)channel;
        args[1] = (u32)nick;
        args[3] = (u32)param;
        args[4] = 0;
        ciAddCallback_(chat, 7, callbacks->userLeft, args, callbacks->param, 0, channel->name, sizeof(args));
    }
    if (!callbacks->usersChanged) {
        return;
    }
    usersChangedArg = (u32)channel;
    ciAddCallback_(chat, 12, callbacks->usersChanged, &usersChangedArg, callbacks->param, 0, channel->name, sizeof(usersChangedArg));
}

// KILL
void ciKillHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255F34, 2137);
    if (message->paramCount != 2) {
        return;
    }
    ciUserEnumChannels(chat, message->params[0], ciKillEnumChannelsCallback, message->params[1]);
}

// TOPIC: the topic of a channel changed.
void ciTopicHandler(ChatConnection *chat, ChatMessage *message) {
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
    ciSetChannelTopic(chat, channel, topic);
    callbacks = ciGetChannelCallbacks(chat, channel);
    if (!callbacks || !callbacks->topicChanged) {
        return;
    }
    args.channel = channel;
    args.topic = topic;
    ciAddCallback_(chat, 9, callbacks->topicChanged, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
}

// MODE: the mode of a channel or of its users changed.
void ciModeHandler(ChatConnection *chat, ChatMessage *message) {
    ChatModeChange *changes, *change;
    char *channel, *modes;
    ChatChannelMode mode;
    ASSERT(message->paramCount >= 2, ov45_022567F4, ov45_02255F44, 0x893);
    if (message->paramCount < 2) {
        return;
    }
    channel = message->params[0];
    modes = message->params[1];
    if (ciInChannel(chat, channel) == 0) {
        return;
    }
    changes = ciParseMode(modes, (const char **)message->params + 2, message->paramCount - 2);
    if (changes == NULL) {
        return;
    }
    change = changes;
    while (change->type != CHAT_MODE_END) {
        switch (change->type) {
        case CHAT_MODE_KEY:
            if (change->set) {
                ciSetChannelPassword(chat, channel, change->arg);
            } else {
                ciSetChannelPassword(chat, channel, NULL);
            }
            break;
        case CHAT_MODE_OP:
            if (change->arg != NULL) {
                ciUserChangedMode(chat, change->arg, channel, 2, change->set);
            }
            break;
        case CHAT_MODE_VOICE:
            if (change->arg != NULL) {
                ciUserChangedMode(chat, change->arg, channel, 1, change->set);
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
    if (ciGetChannelMode(chat, channel, &mode) == 0) {
        DWCi_GsFree(changes);
        return;
    }
    ciApplyChangesToMode(&mode, changes);
    ciSetChannelMode(chat, channel, &mode);
    DWCi_GsFree(changes);
}

// ERROR: the server closes the connection.
void ciErrorHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02255F64, 2289);
    ASSERT(message->paramCount == 1, ov45_0225671C, ov45_02255F64, 2295);
    if (message->paramCount != 1) {
        return;
    }
    ciHandleDisconnect(chat, message->params[0]);
}

// INVITE: the own user was invited into a channel.
void ciInviteHandler(ChatConnection *chat, ChatMessage *message) {
    char *channel, *nick;
    ChatInviteArgs args;
    ASSERT_CHAT(chat, ov45_02255FB4, 2308);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02255FB4, 2314);
    if (message->paramCount != 2) {
        return;
    }
    nick = message->nick;
    channel = message->params[1];
    if (!chat->globalCallbacks.invited) {
        return;
    }
    args.channel = channel;
    args.nick = nick;
    ciAddCallback_(chat, 3, chat->globalCallbacks.invited, (u32 *)&args, chat->globalCallbacks.param, 0, NULL, sizeof(args));
}

// 353: a part of the user list of a channel (nicks with "@" for operators and "+" for voiced users).
void ciNameReplyHandler(ChatConnection *chat, ChatMessage *message) {
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
    request = ciFindFilter(chat, 4, keys);
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
                char **newNicks = DWCi_GsRealloc(list->nicks, (list->capacity + NICK_LIST_GROWTH) * sizeof(char *));
                if (!newNicks) {
                    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2413);
                    return;
                }
                list->nicks = newNicks;
                int *newModes = DWCi_GsRealloc(list->modes, (list->capacity + NICK_LIST_GROWTH) * sizeof(int));
                if (!newModes) {
                    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256088, 2420);
                    return;
                }
                list->modes = newModes;
                list->capacity += NICK_LIST_GROWTH;
            }
            u32 length = strlen(nick);
            char *copy = DWCi_GsMalloc(length + 1);
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
            ciUserEnteredChannel(chat, nick, channel, mode, NULL, NULL);
        }
        nick = strtok(NULL, ov45_02256824);
    } while (nick);
}

// 366: end of the user list of a channel ("*" = all channels).
void ciEndOfNamesHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey keys[4];

    struct {
        BOOL success;
        int result; // ChatEnterResult
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
    request = ciFindFilter(chat, 4, keys);
    if (!request) {
        return;
    }
    if (request->type == 1) {
        enterResult.success = TRUE;
        enterResult.result = CHAT_ENTER_SUCCESS;
        enterResult.channel = channel;
        if (request->callback == NULL) {
            ciJoinCallbackCalled(chat, channel);
        }
        ciFinishFilter(chat, request, (u32 *)&enterResult);
    } else if (request->type == 15) {
        ChatNickListData *list = request->data;
        nickListResult.channel = channel;
        nickListResult.count = list->count;
        nickListResult.nicks = list->nicks;
        nickListResult.modes = list->modes;
        ciFinishFilter(chat, request, (u32 *)&nickListResult);
    } else if (request->type == 3) {
        ChatNickListData *list = request->data;
        usersResult.success = TRUE;
        usersResult.channel = channel;
        usersResult.count = list->count;
        usersResult.nicks = list->nicks;
        usersResult.modes = list->modes;
        ciFinishFilter(chat, request, (u32 *)&usersResult);
    }
}

// 332: the topic of a channel (answer to a topic request, or sent when the channel is entered).
void ciRplTopicHandler(ChatConnection *chat, ChatMessage *message) {
    char *channel, *topic;
    ChatRequest *request;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_0225604C, 2565);
    if (message->paramCount != 3) {
        return;
    }
    channel = message->params[1];
    topic = message->params[2];
    ciSetChannelTopic(chat, channel, topic);
    ChatRequestKey key = { 0 };
    u32 results[3];
    ChatTopicArgs args;
    key.type = 2;
    key.name = channel;
    request = ciFindFilter(chat, 1, &key);
    if (request) {
        results[0] = TRUE;
        results[1] = (u32)channel;
        results[2] = (u32)topic;
        ciFinishFilter(chat, request, results);
        return;
    }
    ChatChannelCallbacks *callbacks = ciGetChannelCallbacks(chat, channel);
    if (!callbacks || !callbacks->topicChanged) {
        return;
    }
    args.channel = channel;
    args.topic = topic;
    ciAddCallback_(chat, 9, callbacks->topicChanged, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
}

// 331: a channel has no topic.
void ciRplNoTopicHandler(ChatConnection *chat, ChatMessage *message) {
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
    request = ciFindFilter(chat, 1, &key);
    if (request) {
        results[0] = TRUE;
        results[1] = (u32)channel;
        results[2] = (u32)ov45_02256708;
        ciFinishFilter(chat, request, results);
        return;
    }
    ChatChannelCallbacks *callbacks = ciGetChannelCallbacks(chat, channel);
    if (!callbacks || !callbacks->topicChanged) {
        return;
    }
    args.channel = channel;
    args.topic = ov45_02256708;
    ciAddCallback_(chat, 9, callbacks->topicChanged, (u32 *)&args, callbacks->param, 0, channel, sizeof(args));
}

// 433: the nick is already in use (fails a nick change, or reported while connecting).
void ciErrNickInUseHandler(ChatConnection *chat, ChatMessage *message) {
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
    request = ciFindFilter(chat, 1, &key);
    if (request) {
        results[0] = FALSE;
        results[1] = (u32)oldNick;
        results[2] = (u32)newNick;
        ciFinishFilter(chat, request, results);
        return;
    }
    ASSERT(chat->connecting, ov45_02256864, ov45_022561D0, 2707);
    if (!chat->connecting) {
        return;
    }
    ov45_02251B0C(chat, 1, chat->nick, 0, 0);
}

// 352: one user of a WHO reply (channel, user, host, server, nick, flags "H@+", ...).
void ciRplWhoReplyHandler(ChatConnection *chat, ChatMessage *message) {
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
    ciSetUserBasicInfo(chat, nick, user, host);
    {
        memset(keys, 0, sizeof(keys));
        keys[0].type = 6;
        keys[1].type = 10;
        keys[2].type = 11;
        keys[0].name = nick;
        keys[0].name2 = channel;
        keys[1].name = nick;
        keys[2].name = channel;
        request = ciFindFilter(chat, 3, keys);
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
            ciFinishFilter(chat, request, modeResult);
        } else if (request->type == 10) {
            addressResult[0] = TRUE;
            addressResult[1] = (u32)nick;
            addressResult[2] = (u32)user;
            addressResult[3] = (u32)host;
            ciAddCallback_(chat, 21, request->callback, addressResult, request->param, request->id, NULL, sizeof(addressResult));
            request->callback = NULL;
        } else if (request->type == 11 && request->callback != NULL) {
            channelResult[0] = TRUE;
            channelResult[1] = (u32)request->name;
            channelResult[2] = (u32)nick;
            channelResult[3] = (u32)user;
            channelResult[4] = (u32)host;
            ciAddCallback_(chat, 22, request->callback, channelResult, request->param, request->id, NULL, sizeof(channelResult));
        }
    }
}

// 315: end of a WHO reply.
void ciRplEndOfWhoHandler(ChatConnection *chat, ChatMessage *message) {
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
    request = ciFindFilter(chat, 2, keys);
    if (!request) {
        return;
    }
    if (request->type == 10) {
        addressResult[0] = FALSE;
        addressResult[1] = (u32)name;
        addressResult[2] = 0;
        addressResult[3] = 0;
        ciFinishFilter(chat, request, addressResult);
        return;
    }
    if (request->type == 11) {
        channelResult[0] = TRUE;
        channelResult[1] = (u32)name;
        channelResult[2] = 0;
        channelResult[3] = 0;
        channelResult[4] = 0;
        ciFinishFilter(chat, request, channelResult);
    }
}

// Returns a heap copy of the field after the backslash at text (up to the next backslash or the end) and its
// length with the backslash in *length; NULL when text does not start with a backslash.
char *ciParseValue(const char *text, int *length) {
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
    field = DWCi_GsMalloc(n + 1);
    if (!field) {
        return NULL;
    }
    memcpy(field, text + 1, n);
    field[n] = 0;
    *length = n + 1;
    return field;
}

// 700: the keys of a user (answer to a key request of request type 12; the values are fields introduced by
// backslashes). For a request about a whole channel every user is reported through the callback, and 701 ends it.
void ciRplGetKeyHandler(ChatConnection *chat, ChatMessage *message) {
    char *nick;
    ChatRequest *request;
    char *cursor, *cookie;
    ChatKeysData *data;
    int count;
    char **values;
    int i;
    ASSERT(message->paramCount == 4, ov45_0225680C, ov45_02256060, 2934);
    if (message->paramCount != 4) {
        return;
    }
    nick = message->params[1];
    cookie = message->params[2];
    cursor = message->params[3];
    {
        ChatRequestKey key = { 0, 0, 0 };
        u32 results[5]; // success, nick, count, keys, values
        int length;
        key.type = 12;
        key.name = cookie;
        request = ciFindFilter(chat, 1, &key);
        if (!request) {
            return;
        }
        data = request->data;
        count = data->count;
        values = DWCi_GsMalloc(count * 4);
        if (!values) {
            return;
        }
        for (i = 0; i < count; i++) {
            char *value = ciParseValue(cursor, &length);
            if (value) {
                values[i] = value;
                cursor += length;
            } else {
                values[i] = goastrdup(ov45_02256708);
            }
        }
        results[0] = TRUE;
        results[1] = (u32)nick;
        results[2] = count;
        results[3] = (u32)data->keys;
        results[4] = (u32)values;
        if (!data->arg.channel) {
            ciFinishFilter(chat, request, results);
        } else {
            ciAddCallback_(chat, 29, request->callback, results, request->param, request->id, NULL, sizeof(results));
        }
        for (i = 0; i < count; i++) {
            DWCi_GsFree(values[i]);
        }
        DWCi_GsFree(values);
    }
}

// 701: end of the keys of the users of a channel (request type 12).
void ciRplEndGetKeyHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequest *request;
    ChatKeysData *data;
    char *cookie;
    ASSERT(message->paramCount == 4, ov45_0225680C, ov45_02256188, 3020);
    if (message->paramCount != 4) {
        return;
    }
    cookie = message->params[2];
    ChatRequestKey key = { 0 };
    u32 results[5]; // success, nick, count, keys, values
    key.type = 12;
    key.name = cookie;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    data = request->data;
    results[1] = 0;
    results[0] = TRUE;
    results[2] = data->count;
    results[3] = (u32)data->keys;
    results[4] = 0;
    ciFinishFilter(chat, request, results);
}

// 702: the keys of a user in a channel (request type 13). The cookie "BCAST" instead reports changed broadcast
// keys ("b_" prefix) to the channel's broadcast key callback.
void ciRplGetCKeyHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey key;
    ChatRequest *request;
    ChatBroadcastKeyArgs change;
    u32 results[6]; // success, channel, nick, count, keys, values
    char **params;
    char *cookie;
    char *nick;
    int count;
    char *channel;
    char *cursor;
    ChatChannelCallbacks *callbacks;
    ChatKeysData *data;
    char **values;
    int i;
    int length;
    char *name; // also the name of a broadcast key
    char *keyValue;
    char *value;
    char saved;
    u32 size;
    char **newKeys;
    char **newValues;
    ASSERT(message->paramCount == 5, ov45_022568A0, ov45_022560C4, 3068);
    if (message->paramCount != 5) {
        return;
    }
    params = message->params;
    cookie = params[3];
    channel = params[1];
    nick = params[2];
    cursor = params[4];
    if (strcmp(cookie, ov45_022568B8) == 0) {
        callbacks = ciGetChannelCallbacks(chat, channel);
        if (callbacks == NULL || callbacks->broadcastKeyChanged == NULL) {
            return;
        }
        memset(&change, 0, sizeof(change));
        change.channel = channel;
        change.nick = nick;
        if (*cursor == 0) {
            return;
        }
        do {
            name = strstr(cursor, ov45_022568C0);
            cursor = name;
            while (*cursor != 0 && *cursor != '\\') {
                cursor++;
            }
            if (*cursor == 0) {
                return;
            }
            *cursor++ = 0;
            value = cursor;
            while (*cursor != 0 && *cursor != '\\') {
                cursor++;
            }
            saved = *cursor;
            *cursor = 0;
            change.key = name;
            change.value = value;
            ciAddCallback_(chat, 28, callbacks->broadcastKeyChanged, (u32 *)&change, callbacks->param, 0, channel, sizeof(change));
            *cursor = saved;
        } while (*cursor != 0);
        return;
    }
    memset(&key, 0, sizeof(key));
    key.type = 13;
    key.name = cookie;
    request = ciFindFilter(chat, 1, &key);
    if (request == NULL) {
        return;
    }
    data = request->data;
    count = data->count;
    values = DWCi_GsMalloc(count * 4);
    if (values == NULL) {
        return;
    }
    for (i = 0; i < count; i++) {
        values[i] = ciParseValue(cursor, &length);
        if (values[i] != NULL) {
            cursor += length;
        }
    }
    if (data->broadcastKeys) {
        // the broadcast keys follow as backslash separated name / value pairs
        while ((name = ciParseValue(cursor, &length)) != NULL) {
            cursor += length;
            keyValue = ciParseValue(cursor, &length);
            if (keyValue != NULL) {
                size = (count + 1) * 4;
                cursor += length;
                newKeys = DWCi_GsRealloc(data->keys, size);
                if (newKeys != NULL) {
                    data->keys = newKeys;
                    newValues = DWCi_GsRealloc(values, size);
                    if (newValues != NULL) {
                        values = newValues;
                        data->keys[count] = name;
                        values[count] = keyValue;
                        count++;
                    } else {
                        DWCi_GsFree(name);
                        DWCi_GsFree(keyValue);
                    }
                } else {
                    DWCi_GsFree(name);
                    DWCi_GsFree(keyValue);
                }
            } else {
                DWCi_GsFree(name);
                break;
            }
        }
        data->count = count;
    }
    results[0] = TRUE;
    results[1] = (u32)channel;
    results[2] = (u32)nick;
    results[3] = count;
    results[4] = (u32)data->keys;
    results[5] = (u32)values;
    if (!data->arg.allUsers) {
        ciFinishFilter(chat, request, results);
    } else {
        ciAddCallback_(chat, 30, request->callback, results, request->param, request->id, NULL, sizeof(results));
    }
    for (i = 0; i < count; i++) {
        DWCi_GsFree(values[i]);
    }
    DWCi_GsFree(values);
}

// 703: end of the keys of the users of a channel (request type 13).
void ciRplEndGetCKeyHandler(ChatConnection *chat, ChatMessage *message) {
    char *channel, *cookie;
    ChatRequest *request;
    ChatKeysData *data;
    ASSERT(message->paramCount == 4, ov45_0225680C, ov45_022561E8, 3248);
    if (message->paramCount != 4) {
        return;
    }
    channel = message->params[1];
    cookie = message->params[2];
    ChatRequestKey key = { 0 };
    u32 results[6]; // success, channel, nick, count, keys, values
    key.type = 13;
    key.name = cookie;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    data = request->data;
    results[1] = (u32)channel;
    results[2] = 0;
    results[0] = TRUE;
    results[3] = data->count;
    results[4] = (u32)data->keys;
    results[5] = 0;
    ciFinishFilter(chat, request, results);
}

// 704: the keys of a channel (request type 14; without requested keys, all name / value pairs of the reply are
// returned). The cookie "BCAST" reports changed broadcast keys of the channel like 702.
void ciRplGetChanKeyHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequest *request;
    char **params;
    char *channel;
    char *cookie, *cursor;
    int length;
    ChatChannelCallbacks *callbacks;
    ChatRequestKey key;
    ChatBroadcastKeyArgs change;
    u32 results[6]; // success, channel, nick, count, keys, values
    ASSERT(message->paramCount == 4, ov45_0225680C, ov45_02256200, 3297);
    if (message->paramCount != 4) {
        return;
    }
    params = message->params;
    channel = params[1];
    cookie = params[2];
    cursor = params[3];
    if (strcmp(cookie, ov45_022568B8) == 0) {
        callbacks = ciGetChannelCallbacks(chat, channel);
        if (!callbacks || !callbacks->broadcastKeyChanged) {
            return;
        }
        memset(&change, 0, sizeof(change));
        change.channel = channel;
        change.nick = NULL;
        if (!*cursor) {
            return;
        }
        do {
            char *name = strstr(cursor, ov45_022568C0);
            char *value;
            s8 saved;
            cursor = name;
            while (*cursor && *cursor != '\\') {
                cursor++;
            }
            if (!*cursor) {
                return;
            }
            *cursor++ = 0;
            value = cursor;
            while (*cursor && *cursor != '\\') {
                cursor++;
            }
            saved = *cursor;
            *cursor = 0;
            change.key = name;
            change.value = value;
            ciAddCallback_(chat, 28, callbacks->broadcastKeyChanged, (u32 *)&change, callbacks->param, 0, channel, sizeof(change));
            *cursor = saved;
        } while (*cursor);
        return;
    }
    memset(&key, 0, sizeof(key));
    key.type = 14;
    key.name = cookie;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    {
        ChatKeysData *data = request->data;
        int count = data->count, i;
        char **values;
        char *name, *value;
        if (count) {
            values = DWCi_GsMalloc(count * 4);
            if (!values) {
                return;
            }
            for (i = 0; i < count; i++) {
                values[i] = ciParseValue(cursor, &length);
                if (values[i]) {
                    cursor += length;
                }
            }
            if (data->arg.broadcastKeys) {
                name = ciParseValue(cursor, &length);
                while (name) {
                    char **newKeys, **newValues;
                    u32 size;
                    cursor += length;
                    value = ciParseValue(cursor, &length);
                    if (value) {
                        size = (count + 1) * 4;
                        cursor += length;
                        newKeys = DWCi_GsRealloc(data->keys, size);
                        if (newKeys) {
                            data->keys = newKeys;
                            newValues = DWCi_GsRealloc(values, size);
                            if (newValues) {
                                values = newValues;
                                data->keys[count] = name;
                                values[count] = value;
                                count++;
                            } else {
                                DWCi_GsFree(name);
                                DWCi_GsFree(value);
                            }
                        } else {
                            DWCi_GsFree(name);
                            DWCi_GsFree(value);
                        }
                    } else {
                        DWCi_GsFree(name);
                        break;
                    }
                    name = ciParseValue(cursor, &length);
                }
                data->count = count;
            }
        } else {
            char **keys = NULL;
            values = NULL;
            count = 0;
            while (1) {
                char **newKeys, **newValues;
                u32 size;
                name = ciParseValue(cursor, &length);
                if (!name) {
                    break;
                }
                cursor += length;
                value = ciParseValue(cursor, &length);
                if (!value) {
                    DWCi_GsFree(name);
                    break;
                }
                size = (count + 1) * 4;
                cursor += length;
                newKeys = DWCi_GsRealloc(keys, size);
                newValues = DWCi_GsRealloc(values, size);
                if (!newKeys || !newValues) {
                    // (the arrays are still used after this cleanup)
                    DWCi_GsFree(name);
                    DWCi_GsFree(value);
                    while (count--) {
                        DWCi_GsFree(keys[count]);
                        DWCi_GsFree(values[count]);
                    }
                    if (newKeys) {
                        DWCi_GsFree(newKeys);
                    } else {
                        DWCi_GsFree(keys);
                    }
                    if (newValues) {
                        DWCi_GsFree(newValues);
                    } else {
                        DWCi_GsFree(values);
                    }
                }
                newKeys[count] = name;
                newValues[count] = value;
                keys = newKeys;
                values = newValues;
                count++;
            }
            data->count = count;
            data->keys = keys;
        }
        results[0] = TRUE;
        results[1] = (u32)channel;
        results[2] = 0;
        results[3] = count;
        results[4] = (u32)data->keys;
        results[5] = (u32)values;
        ciFinishFilter(chat, request, results);
        for (i = 0; i < count; i++) {
            DWCi_GsFree(values[i]);
        }
        DWCi_GsFree(values);
    }
}

// 302: the address of the own user ("USRIP" answer, nick=+user@address in the last parameter). The user address
// callback can choose the user name from it before the registration is sent.
void ciRplUserIPHandler(ChatConnection *chat, ChatMessage *message) {
    char user[128], *address;
    ASSERT_CHAT(chat, ov45_02256074, 3517);
    ASSERT(message->paramCount >= 1, ov45_022568C4, ov45_02256074, 3523);
    if (message->paramCount < 1) {
        return;
    }
    address = strchr(message->params[message->paramCount - 1], '@');
    if (address && chat->userAddressCallback) {
        int ip = gsSocketInetAddr(address + 1);
        ((ChatUserAddressFunc)chat->userAddressCallback)(chat, ip, user, chat->connectParam);
        strncpy(chat->user, user, sizeof(chat->user));
        chat->user[sizeof(chat->user) - 1] = 0;
    }
    ciSendNickAndUser(chat);
}

// 321: start of the channel list (request type 0).
void ciRplListStartHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey key = { 0 };
    ChatRequest *request;
    ChatChannelListData *data;
    key.type = 0;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    data = request->data;
    ASSERT(data, ov45_022568DC, ov45_022561A0, 3567);
    data->started = TRUE;
}

// 322: one channel of the channel list (name, number of users, topic): passed to the per channel callback and
// collected for the list callback.
void ciRplListHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequest *request;
    ChatChannelListData *data;
    char *name, *topic;
    long numUsers;
    u32 length;
    int index;
    void *array;
    ASSERT(message->paramCount == 4, ov45_0225680C, ov45_02255FFC, 3585);
    if (message->paramCount != 4) {
        return;
    }
    {
        ChatRequestKey key = { 0, 0, 0 };
        ChatChannelListEntryArgs args;
        key.type = 0;
        request = ciFindFilter(chat, 1, &key);
        if (request == NULL || request->callback == NULL) {
            return;
        }
        data = request->data;
        ASSERT(data, ov45_022568DC, ov45_02255FFC, 3605);
        length = strlen(message->params[1]);
        name = DWCi_GsMalloc(length + 1);
        if (name == NULL) {
            return;
        }
        memcpy(name, message->params[1], length + 1);
        numUsers = atol(message->params[2]);
        length = strlen(message->params[3]);
        topic = DWCi_GsMalloc(length + 1);
        if (topic == NULL) {
            DWCi_GsFree(name);
            return;
        }
        memcpy(topic, message->params[3], length + 1);
        index = data->count;
        args.index = index;
        args.channel = name;
        args.topic = topic;
        args.numUsers = numUsers;
        args.success = TRUE;
        ciAddCallback_(chat, 13, request->callback, (u32 *)&args, request->param, request->id, NULL, sizeof(args));
        array = DWCi_GsRealloc(data->names, (data->count + 1) * 4);
        if (array == NULL) {
            DWCi_GsFree(name);
            DWCi_GsFree(topic);
            return;
        }
        data->names = array;
        data->names[index] = name;
        array = DWCi_GsRealloc(data->numUsers, (data->count + 1) * 4);
        if (array == NULL) {
            DWCi_GsFree(name);
            DWCi_GsFree(topic);
            return;
        }
        data->numUsers = array;
        data->numUsers[index] = numUsers;
        array = DWCi_GsRealloc(data->topics, (data->count + 1) * 4);
        if (array == NULL) {
            DWCi_GsFree(name);
            DWCi_GsFree(topic);
            return;
        }
        data->topics = array;
        data->topics[index] = topic;
        data->count++;
    }
}

// 323: end of the channel list (request type 0).
void ciRplListEndHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey key = { 0 };
    u32 results[5]; // success, count, names, topics, numUsers
    ChatRequest *request;
    ChatChannelListData *data;
    key.type = 0;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    data = request->data;
    results[0] = TRUE;
    results[1] = data->count;
    results[2] = (u32)data->names;
    results[4] = (u32)data->numUsers;
    results[3] = (u32)data->topics;
    ciFinishFilter(chat, request, results);
}

// 324: the mode of a channel (answer to a mode request, request type 5, or sent when the channel is entered).
void ciRplChannelModeIsHandler(ChatConnection *chat, ChatMessage *message) {
    char *channel;
    ChatModeChange *changes;
    ChatRequest *request;
    ASSERT(message->paramCount >= 3, ov45_022568EC, ov45_022562D0, 3731);
    if (message->paramCount < 3) {
        return;
    }
    channel = message->params[1];
    changes = ciParseMode(message->params[2], (const char **)message->params + 3, message->paramCount - 3);
    if (!changes) {
        return;
    }
    ChatChannelMode mode, oldMode;

    union {
        ChatRequestKey key;
        u32 words[3];
    } key;

    ChatChannelModeArgs result;

    union {
        ChatChannelModeChangedArgs args;
        u32 words[2];
    } changed;

    memset(&mode, 0, sizeof(mode));
    ciApplyChangesToMode(&mode, changes);
    if (!ciGetChannelMode(chat, channel, &oldMode)) {
        ChatChannelCallbacks *callbacks;
        ciSetChannelMode(chat, channel, &mode);
        callbacks = ciGetChannelCallbacks(chat, channel);
        if (callbacks && callbacks->modeChanged) {
            changed.args.channel = channel;
            changed.args.mode = &mode;
            ciAddCallback_(chat, 10, callbacks->modeChanged, changed.words, callbacks->param, 0, channel, sizeof(changed));
        }
    }
    memset(&key, 0, sizeof(key));
    key.key.type = 5;
    key.key.name = channel;
    request = ciFindFilter(chat, 1, &key.key);
    if (request) {
        result.success = TRUE;
        result.channel = channel;
        result.mode = &mode;
        ciFinishFilter(chat, request, (u32 *)&result);
    }
    DWCi_GsFree(changes);
}

// 311: the user, address and real name of a user (WHOIS reply, request type 4), or the address for a ban by nick
// (request type 7: sends the ban "*!*@address" to the channel).
void ciRplWhoisUserHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey keys[2];
    ChatRequest *request;
    char *nick;
    ASSERT_CHAT(chat, ov45_022561B8, 3797);
    ASSERT(message->paramCount == 6, ov45_02256904, ov45_022561B8, 3803);
    if (message->paramCount != 6) {
        return;
    }
    nick = message->params[1];
    memset(keys, 0, sizeof(keys));
    keys[0].type = 4;
    keys[0].name = nick;
    keys[1].type = 7;
    keys[1].name = nick;
    request = ciFindFilter(chat, 2, keys);
    if (!request) {
        return;
    }
    if (request->type == 4) {
        char *realName;
        char *address;
        ChatWhoisData *data = request->data;
        realName = message->params[5];
        address = message->params[3];
        data->user = goastrdup(message->params[2]);
        if (!data->user) {
            return;
        }
        data->realName = goastrdup(realName);
        if (!data->realName) {
            return;
        }
        data->address = goastrdup(address);
        if (!data->address) {
            return;
        }
    } else if (request->type == 7) {
        ChatBanData *data = request->data;
        ASSERT(data, ov45_022568DC, ov45_022561B8, 3869);
        ASSERT(data->channel, ov45_0225691C, ov45_022561B8, 3870);
        ASSERT(data->channel[0], ov45_02256934, ov45_022561B8, 3870);
        ciSocketSendf(&chat->socket, ov45_02256950, data->channel, message->params[3]);
        ciFinishFilter(chat, request, NULL);
    }
}

// 319: the channels of a user (WHOIS reply, request type 4; "@" / "+" mode prefixes are dropped).
void ciRplWhoisChannelsHandler(ChatConnection *chat, ChatMessage *message) {
    char *nick;
    ChatRequest *request;
    ChatWhoisData *data;
    char *token, *name;
    char **array;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_022562EC, 3892);
    if (message->paramCount != 3) {
        return;
    }
    nick = message->params[1];
    ChatRequestKey key;
    memset(&key, 0, sizeof(key));
    key.type = 4;
    key.name = nick;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    data = request->data;
    token = strtok(message->params[2], ov45_02256824);
    if (!token) {
        return;
    }
    do {
        if (*token == '-') {
            token++;
        }
        if (*token == '@' || *token == '+') {
            token++;
        }
        name = goastrdup(token);
        if (!name) {
            return;
        }
        array = DWCi_GsRealloc(data->channels, (data->channelCount + 1) * 4);
        if (!array) {
            DWCi_GsFree(name);
            return;
        }
        data->channels = array;
        data->channels[data->channelCount] = name;
        data->channelCount++;
        token = strtok(NULL, ov45_02256824);
    } while (token);
}

// 318: end of a WHOIS reply (request type 4).
void ciRplEndOfWhoisHandler(ChatConnection *chat, ChatMessage *message) {
    char *nick;
    ChatRequest *request;
    ChatWhoisData *data;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_02256218, 3958);
    if (message->paramCount != 3) {
        return;
    }
    nick = message->params[1];
    ChatRequestKey key = { 0 };
    u32 results[7]; // success, nick, user, realName, address, channelCount, channels
    key.type = 4;
    key.name = nick;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    data = request->data;
    results[0] = data->user != NULL;
    results[1] = (u32)nick;
    results[2] = (u32)data->user;
    results[3] = (u32)data->realName;
    results[4] = (u32)data->address;
    results[5] = data->channelCount;
    results[6] = (u32)data->channels;
    ciFinishFilter(chat, request, results);
}

// 367: one ban mask of a channel's ban list (request type 8).
void ciRplBanListHandler(ChatConnection *chat, ChatMessage *message) {
    char *channel;
    char *ban, *copy;
    ChatRequest *request;
    ChatBanListData *data;
    ChatRequestKey key;
    int length;
    ASSERT(message->paramCount >= 3, ov45_02256964, ov45_022560EC, 3997);
    if (message->paramCount < 3) {
        return;
    }
    channel = message->params[1];
    ban = message->params[2];
    memset(&key, 0, sizeof(key));
    key.type = 8;
    key.name = channel;
    request = ciFindFilter(chat, 1, &key);
    if (request == NULL) {
        return;
    }
    data = request->data;
    ASSERT(data, ov45_022568DC, ov45_022560EC, 4015);
    ASSERT(data->count >= 0, ov45_0225697C, ov45_022560EC, 4016);
    copy = DWCi_GsRealloc(data->bans, (data->count + 1) * 4);
    if (copy == NULL) {
        return;
    }
    data->bans = (char **)copy;
    length = strlen(ban);
    copy = DWCi_GsMalloc(length + 1);
    if (copy == NULL) {
        return;
    }
    memcpy(copy, ban, length + 1);
    data->bans[data->count] = copy;
    data->count++;
}

// 368: end of a channel's ban list (request type 8).
void ciRplEndOfBanListHandler(ChatConnection *chat, ChatMessage *message) {
    char *channel;
    ChatRequest *request;
    ChatBanListData *data;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_0225627C, 4047);
    if (message->paramCount != 3) {
        return;
    }
    channel = message->params[1];
    ChatRequestKey key = { 0 };
    u32 results[4]; // success, channel, count, bans
    key.type = 8;
    key.name = channel;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    data = request->data;
    results[1] = (u32)channel;
    results[0] = TRUE;
    results[2] = data->count;
    results[3] = (u32)data->bans;
    ciFinishFilter(chat, request, results);
}

// 001: the registration is done (the server may have changed the nick); calls the connected callback.
void ciRplWelcomeHandler(ChatConnection *chat, ChatMessage *message) {
    char *nick;
    ASSERT_CHAT(chat, ov45_02256114, 4076);
    ASSERT(message->paramCount == 2, ov45_022566F0, ov45_02256114, 4082);
    if (message->paramCount != 2) {
        return;
    }
    nick = message->params[0];
    if (strcmp(chat->nick, nick)) {
        strncpy(chat->nick, nick, sizeof(chat->nick));
        chat->nick[sizeof(chat->nick) - 1] = 0;
    }
    chat->connecting = FALSE;
    chat->connected = TRUE;
    if (chat->connectedCallback) {
        ((ChatConnectedFunc)chat->connectedCallback)(chat, TRUE, 0, chat->connectParam);
    }
}

// 705: the keys of the connection's ciphers (decoded with the game key); then the login, the address query or
// the registration is sent.
void ciRplSecureKeyHandler(ChatConnection *chat, ChatMessage *message) {
    char *outputKey, *inputKey;
    int outputLength, inputLength;
    ASSERT_CHAT(chat, ov45_02256170, 4117);
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_02256170, 4123);
    if (message->paramCount != 3) {
        return;
    }
    outputKey = message->params[1];
    inputKey = message->params[2];
    outputLength = strlen(outputKey);
    inputLength = strlen(inputKey);
    gs_xcode_buf((s8 *)outputKey, outputLength, (s8 *)chat->gameKey);
    gs_xcode_buf((s8 *)inputKey, inputLength, (s8 *)chat->gameKey);
    gs_prepare_key((u8 *)outputKey, outputLength, &chat->socket.outputCipher);
    gs_prepare_key((u8 *)inputKey, inputLength, &chat->socket.inputCipher);
    chat->socket.encrypted = TRUE;
    if (chat->loginType) {
        ciSendLogin(chat);
    } else if (chat->userAddressCallback) {
        ciSocketSend(&chat->socket, ov45_02256990);
    } else {
        ciSendNickAndUser(chat);
    }
}

// 706: the result of the CD key check (request type 16).
void ciRplCDKeyHandler(ChatConnection *chat, ChatMessage *message) {
    int result;
    char *text;
    ChatRequest *request;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_02256024, 4174);
    if (message->paramCount != 3) {
        return;
    }
    result = atol(message->params[1]);
    text = message->params[2];
    ChatRequestKey key = { 0 };
    u32 results[2]; // result, text
    key.type = 16;
    request = ciFindFilter(chat, 1, &key);
    if (!request) {
        return;
    }
    results[0] = result;
    results[1] = (u32)text;
    ciFinishFilter(chat, request, results);
}

// 707: the login was accepted (two numbers); then the address query or the registration is sent.
void ciRplLoginHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02256038, 4201);
    ASSERT(message->paramCount >= 3, ov45_02256964, ov45_02256038, 4207);
    if (message->paramCount < 3) {
        return;
    }
    chat->unk_8AC[0] = atol(message->params[1]);
    chat->unk_8AC[1] = atol(message->params[2]);
    if (chat->userAddressCallback) {
        ciSocketSend(&chat->socket, ov45_02256990);
    } else {
        ciSendNickAndUser(chat);
    }
}

// 403: no such channel: fails an enter (type 1), mode (type 5) or channel keys (type 12) request.
void ciErrNoSuchChannelHandler(ChatConnection *chat, ChatMessage *message) {
    ChatRequestKey keys[2];
    u32 enterResult[3];
    u32 modeResult[3];
    u32 keysResult[5];
    ChatRequest *request;
    char *channel;
    ASSERT(message->paramCount == 3, ov45_02256848, ov45_022562B4, 4240);
    if (message->paramCount != 3) {
        return;
    }
    channel = message->params[1];
    memset(&keys[0], 0, sizeof(keys[0]));
    keys[0].type = 1;
    keys[0].name = channel;
    keys[1].type = 5;
    keys[1].name = channel;
    request = ciFindFilter(chat, 2, keys);
    if (request != NULL) {
        if (request->type == 1) {
            enterResult[0] = FALSE;
            enterResult[1] = CHAT_ENTER_NO_SUCH_CHANNEL;
            enterResult[2] = (u32)channel;
            ciFinishFilter(chat, request, enterResult);
            return;
        }
        if (request->type == 5) {
            modeResult[0] = FALSE;
            modeResult[1] = (u32)channel;
            modeResult[2] = 0;
            ciFinishFilter(chat, request, modeResult);
            return;
        }
    }
    request = ciFindGetKeyFilter(chat, channel);
    if (request == NULL) {
        return;
    }
    keysResult[0] = FALSE;
    keysResult[1] = 0;
    keysResult[2] = 0;
    keysResult[3] = 0;
    keysResult[4] = 0;
    ciFinishFilter(chat, request, keysResult);
}

// Fails the enter request (request type 1) of the channel in the second parameter.
#define FAIL_ENTER(chat, message, func, line, reason)                \
    {                                                                \
        char *channel;                                               \
        ChatRequest *request;                                        \
        ASSERT(message->paramCount == 3, ov45_02256848, func, line); \
        if (message->paramCount != 3) {                              \
            return;                                                  \
        }                                                            \
        channel = message->params[1];                                \
        ChatRequestKey key = { 0 };                                  \
        u32 results[3]; /* success, reason, channel */               \
        key.type = 1;                                                \
        key.name = channel;                                          \
        request = ciFindFilter(chat, 1, &key);                      \
        if (!request) {                                              \
            return;                                                  \
        }                                                            \
        results[0] = FALSE;                                          \
        results[1] = reason;                                         \
        results[2] = (u32)channel;                                   \
        ciFinishFilter(chat, request, results);                       \
    }

// 405: too many channels.
void ciErrTooManyChannelsHandler(ChatConnection *chat, ChatMessage *message) {
    FAIL_ENTER(chat, message, ov45_02256394, 4318, CHAT_ENTER_TOO_MANY_CHANNELS);
}

// 471: the channel is full.
void ciErrChannelIsFullHandler(ChatConnection *chat, ChatMessage *message) {
    FAIL_ENTER(chat, message, ov45_02256308, 4353, CHAT_ENTER_CHANNEL_FULL);
}

// 473: the channel is invite only.
void ciErrInviteOnlyChanHandler(ChatConnection *chat, ChatMessage *message) {
    FAIL_ENTER(chat, message, ov45_0225635C, 4388, CHAT_ENTER_INVITE_ONLY);
}

// 474: the own user is banned from the channel.
void ciErrBannedFromChanHandler(ChatConnection *chat, ChatMessage *message) {
    FAIL_ENTER(chat, message, ov45_02256378, 4423, CHAT_ENTER_BANNED);
}

// 475: wrong channel key (password).
void ciErrBadChannelKeyHandler(ChatConnection *chat, ChatMessage *message) {
    FAIL_ENTER(chat, message, ov45_02256298, 4458, CHAT_ENTER_BAD_PASSWORD);
}

// 476: bad channel name.
void ciErrBadChanMaskHandler(ChatConnection *chat, ChatMessage *message) {
    FAIL_ENTER(chat, message, ov45_02256230, 4493, CHAT_ENTER_BAD_CHANNEL_NAME);
}

// 401: no such nick (ignored).
void ciErrNoSuchNickHandler(ChatConnection *chat, ChatMessage *message) {
}

// 432: the nick is not valid (while connecting: nick error 2).
void ciErrErroneusNicknameHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_022563B0, 4530);
    if (!chat->connecting) {
        return;
    }
    ov45_02251B0C(chat, 2, chat->nick, 0, 0);
}

// 708: the login failed (while connecting: the connect fails with reason 2).
void ciErrLoginFailedHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02256248, 4546);
    if (!chat->connecting) {
        return;
    }
    chat->connecting = FALSE;
    if (chat->connectedCallback) {
        ((ChatConnectedFunc)chat->connectedCallback)(chat, FALSE, 2, chat->connectParam);
    }
}

// 709: (while connecting: nick error 4).
void ciErrNoUniqueNickHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_02256260, 4571);
    if (!chat->connecting) {
        return;
    }
    ov45_02251B0C(chat, 4, ov45_02256708, 0, 0);
}

// 710: (while connecting: nick error 3).
void ciErrUniqueNickExpiredHandler(ChatConnection *chat, ChatMessage *message) {
    ASSERT_CHAT(chat, ov45_022563D0, 4587);
    if (!chat->connecting) {
        return;
    }
    ov45_02251B0C(chat, 3, ov45_02256708, 0, 0);
}

// 711: the registered nick was refused, with a number of suggested nicks (backslash separated); while
// connecting: nick error 5 with the suggestions.
void ciErrRegisterNickFailedHandler(ChatConnection *chat, ChatMessage *message) {
    int count, j;
    char **suggestions, *text, *token;
    int i;
    ASSERT_CHAT(chat, ov45_022563F0, 4608);
    ASSERT(message->paramCount == 4, ov45_0225680C, ov45_022563F0, 4614);
    if (message->paramCount != 4) {
        return;
    }
    count = atol(message->params[1]);
    text = message->params[2];
    if (!chat->connecting) {
        return;
    }
    suggestions = DWCi_GsMalloc(count * 4);
    if (!suggestions) {
        return;
    }
    token = strtok(text, ov45_02256998);
    i = 0;
    while (i < count && token) {
        suggestions[i] = goastrdup(token);
        if (!suggestions[i]) {
            break;
        }
        token = strtok(NULL, ov45_02256998);
        i++;
    }
    ov45_02251B0C(chat, 5, chat->registeredNick, i, (int)suggestions);
    for (j = 0; j < i; j++) {
        DWCi_GsFree(suggestions[j]);
    }
    DWCi_GsFree(suggestions);
}
