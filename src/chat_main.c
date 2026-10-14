#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

#include <string.h>

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern int sprintf(char *, const char *, ...);
extern int sStr_X01_2; // 0, the number sent with the CRYPT command
extern const char sStr_ciThink[];
extern const char sStr_ciSendNick[];
extern const char sStr_ciSendUser[];
extern const char sStr_ciSendLogin[];
extern const char sStr_chatInChannelA[];
extern const char sStr_chatDisconnect[];
extern const char sStr_chatEnumUsersA[];
extern const char sStr_chatConnectDoit[];
extern const char sStr_chatEnumChannelsA[];
extern const char sStr_chatEnterChannelA[];
extern const char sStr_chatLeaveChannelA[];
extern const char sStr_ciHandleDisconnect[];
extern const char sStr_chatRetryWithNickA[];
extern const char sStr_ciSendGetChannelKey[];
extern const char sStr_ciEnumUsersCallback[];
extern const char sStr_chatGetChannelKeysA[];
extern const char sStr_chatSetChannelKeysA[];
extern const char sStr_chatSetChannelModeA[];
extern const char sStr_chatGetChannelModeA[];
extern const char sStr_chatSetChannelLimitA[];
extern const char sStr_chatSendUserMessageA[];
extern const char sStr_ciProcessServerMessage[];
extern const char sStr_chatRegisterUniqueNickA[];
extern const char sStr_chatGetChannelNumUsersA[];
extern const char sStr_chatSendChannelMessageA[];
extern const char sStr_MessageNeNULL_2[];
extern const char sStr_ChatMainC[];
extern const char sStr_ChatNeNULL_4[];
extern const char sStr_Disconnected[];
extern const ChatBytes2 sStr_Star_3;
extern const char sFmt_NICK[];
extern const char sFmt_USER[];
extern const char sStr_127_Dot0_Dot0_Dot1[];
extern const char sFmt_LOGINPREAUTH[];
extern const char sFmt_LOGIN[];
extern const char sFmt_LOGIN_2[];
extern const char sStr_0_11[];
extern const char sStr_Empty_21[];
extern const char sStr_ServerAddressNeNULL[];
extern const char sStr_CallbacksNeNULL_2[];
extern const char sStr_ConnectCallbackNeNULL[];
extern const char sStr_NickNeNULL_2[];
extern const char sStr_Nick0_Ne0_3[];
extern const char sStr_StrlenNickLtMAX_NICK[];
extern const char sStr_NamespaceID_Gt0[];
extern const char sStr_UniquenickAndUniquenick0[];
extern const char sStr_PasswordAndPassword0[];
extern const char sStr_NamespaceID_Ge0[];
extern const char sStr_EmailAndEmail0[];
extern const char sStr_ProfilenickAndProfilenick0[];
extern const char sStr_AuthtokenAndAuthtoken0[];
extern const char sStr_PartnerchallengeAndPartnerchallenge0[];
extern const char sStr_preauth[];
extern const char sStr_GamenameAndGamename0[];
extern const char sStr_SecretKeyAndSecretKey0[];
extern const char sFmt_CRYPT_Des[];
extern const char sStr_USRIP_2[];
extern const char sFmt_NICK_2[];
extern const char sFmt_REGISTERNICK[];
extern const char sStr_QUIT_Later[];
extern const char sStr_CallbackAllNeNULL_OrCallbackEachNeNULL[];
extern const char sFmt_LIST[];
extern const char sStr_ChannelNeNULL_3[];
extern const char sStr_Channel0_Ne0_3[];
extern const char sFmt_JOIN[];
extern const char sFmt_PART[];
extern const char sStr_TypeEqCHAT_MESSAGE_OrTypeEqCHAT_ACTION_OrTypeEq[];
extern const char sFmt_PRIVMSG[];
extern const char sFmt_PRIVMSG_ACTION[];
extern const char sFmt_NOTICE[];
extern const char sFmt_UTM[];
extern const char sFmt_ATM[];
extern const char sStr_CallbackNeNULL_4[];
extern const char sStr_ModeNeNULL_2[];
extern const char sStr_XiXpXsXmXnXtXlXe[];
extern const char sFmt_SpaceInt[];
extern const char sFmt_MODE_2[];
extern const char sFmt_MODE_3[];
extern const char sStr_LimitGe0[];
extern const char sFmt_MODE_L[];
extern const char sFmt_MODE_L_2[];
extern const char sStr_NumUsersGe0[];
extern const char sStr_ParamNeNULL[];
extern const char sStr_DataCallbackNeNULL_3[];
extern const char sFmt_NAMES[];
extern const char sStr_UserNeNULL_3[];
extern const char sStr_User0_Ne0_3[];
extern const char sStr_StrlenUserLtMAX_USER[];
extern const char sFmt_BackslashStrBackslashStr[];
extern const char sFmt_Int_6[];
extern const char sStr_CookieAndCookie0[];
extern const char sStr_NumGe0[];
extern const char sFmt_SETCHANKEY[];
extern const char sFmt_SETCKEY[];
extern const char sStr_ChannelAndChannel0[];
extern const char sStr_NotNumOrKeys[];
extern const char sFmt_GETCHANKEY_0[];
extern const char sFmt_GETCKEY_0[];
extern const char sStr_B_UnderscoreStar_2[];

// The key that asks for the broadcast keys of a user (5 bytes with the terminator).
extern const struct {
    ChatBytes3 first;
    ChatBytes2 last;
} sStr_BackslashB_UnderscoreStar;

// Cookie buffer and counter of ciGetNextCookie.
extern char ciCookieBuffer[];
extern int ciCookieCounter;

// Assertion with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, sStr_ChatMainC, func, line); \
    }
#define ASSERT_CHAT(chat, func, line) ASSERT(chat, sStr_ChatNeNULL_4, func, line)
// A valid channel name: not NULL and not empty.
#define ASSERT_CHANNEL(name, func, line)     \
    ASSERT(name, sStr_ChannelNeNULL_3, func, line); \
    ASSERT(*name, sStr_Channel0_Ne0_3, func, line);
// A string argument of chatConnectDoit: asserted and required.
#define REQUIRE_STRING(s, message, line)                 \
    ASSERT((s) && (s)[0], message, sStr_chatConnectDoit, line); \
    if (!(s) || !(s)[0]) {                               \
        return NULL;                                     \
    }

// Queues the connected callback with a failure (the connect attempt has ended).
static inline void ConnectFailed(ChatConnection *chat) {
    chat->connecting = FALSE;
    if (chat->connectedCallback) {
        ((ChatConnectedFunc)chat->connectedCallback)(chat, FALSE, 1, chat->connectParam);
    }
}

// Calls ciThink until the request id has been answered.
#define WAIT_FOR_REQUEST(chat, id) \
    do {                           \
        ciThink(chat, id);   \
        msleep(10);         \
    } while (ciCheckForID(chat, id))

// Calls the handler of the message's command; returns whether the command is known.
int ciProcessServerMessage(ChatConnection *chat, ChatMessage *message) {
    int i;
    ASSERT(message, sStr_MessageNeNULL_2, sStr_ciProcessServerMessage, 79);
    for (i = 0; i < ciNumServerMessageHandlers; i++) {
        if (strnicmp(message->command, ciServerMessageHandlers[i].command) == 0) {
            void (*handler)(ChatConnection *, ChatMessage *) = ciServerMessageHandlers[i].handler;
            if (handler) {
                handler(chat, message);
            }
            return 1;
        }
    }
    return 0;
}

// Whether the request id is still waiting for an answer or a callback.
BOOL ciCheckForID(ChatConnection *chat, int id) {
    if (ciCheckFiltersForID(chat, id) || ciCheckCallbacksForID(chat, id)) {
        return TRUE;
    }
    return FALSE;
}

// The connection was lost: queue the disconnected callback once.
void ciHandleDisconnect(ChatConnection *chat, const char *reason) {
    const char *args;
    ASSERT_CHAT(chat, sStr_ciHandleDisconnect, 115);
    if (chat->disconnected) {
        return;
    }
    chat->connected = FALSE;
    *(volatile BOOL *)&chat->connecting = FALSE;
    chat->disconnected = TRUE;
    if (*(volatile BOOL *)&chat->connecting) {
        if (chat->connectedCallback) {
            ((ChatConnectedFunc)chat->connectedCallback)(chat, FALSE, 0, chat->connectParam);
        }
        return;
    }
    if (!chat->globalCallbacks.disconnected) {
        return;
    }
    args = reason;
    ciAddCallback_(chat, 1, chat->globalCallbacks.disconnected, (u32 *)&args, chat->globalCallbacks.param, 0, NULL, 4);
}

// Per-frame work: receive and handle the server lines, then call the queued callbacks of the request id
// (0 = all).
void ciThink(ChatConnection *chat, int id) {
    ChatMessage *message;
    u32 text;
    ASSERT_CHAT(chat, sStr_ciThink, 154);
    if (chat->socket.state == 1) {
        ciSocketThink(&chat->socket);
        message = ciSocketRecv(&chat->socket);
        while (message) {
            if (chat->globalCallbacks.raw) {
                text = (u32)message->text;
                ciAddCallback_(chat, 0, chat->globalCallbacks.raw, &text, chat->globalCallbacks.param, 0, NULL, 4);
            }
            ciProcessServerMessage(chat, message);
            message = ciSocketRecv(&chat->socket);
        }
        if (chat->socket.state == 2) {
            ciHandleDisconnect(chat, sStr_Disconnected);
        }
    }
    ciFilterThink(chat);
    ciCallCallbacks(chat, id);
}

// Sends the nick (login types 0 and 2) or "*".
void ciSendNick(ChatConnection *chat) {
    const char *nick;
    int error;
    ASSERT_CHAT(chat, sStr_ciSendNick, 206);
    if (!chat->loginType) {
        error = ciNickIsValid(chat->nick);
        if (error) {
            ciNickError(chat, error, chat->nick, 0, 0);
            return;
        }
        nick = chat->nick;
    } else if (chat->loginType == 2 && !chat->accountSpace) {
        error = ciNickIsValid(chat->accountNick);
        if (error) {
            ciNickError(chat, error, chat->accountNick, 0, 0);
            return;
        }
        nick = chat->accountNick;
    } else {
        nick = sStr_Star_3.bytes;
    }
    ciSocketSendf(&chat->socket, sFmt_NICK, nick);
}

void ciSendUser(ChatConnection *chat) {
    ASSERT_CHAT(chat, sStr_ciSendUser, 260);
    ciSocketSendf(&chat->socket, sFmt_USER, chat->user, sStr_127_Dot0_Dot0_Dot1, chat->serverAddress, chat->realName);
}

void ciSendNickAndUser(ChatConnection *chat) {
    ciSendUser(chat);
    ciSendNick(chat);
}

// Sends the login of login types 1 .. 3.
void ciSendLogin(ChatConnection *chat) {
    char hash[36];
    ASSERT_CHAT(chat, sStr_ciSendLogin, 280);
    if (chat->loginType == 3) {
        ciSocketSendf(&chat->socket, sFmt_LOGINPREAUTH, chat->authTicket, chat->partnerKey);
        return;
    }
    MD5Digest(chat->password, strlen(chat->password), hash);
    if (chat->loginType == 1) {
        ciSocketSendf(&chat->socket, sFmt_LOGIN, chat->accountSpace, chat->registeredNick, hash);
        return;
    }
    if (chat->loginType == 2) {
        ciSocketSendf(&chat->socket, sFmt_LOGIN_2, chat->accountSpace, hash, chat->accountNick, chat->email);
        return;
    }
    __msl_assertion_failed(sStr_0_11, sStr_ChatMainC, sStr_ciSendLogin, 318);
}

// Creates the connection and starts the login; with blocking, waits until the connect attempt has ended
// (NULL if it failed).
ChatConnection *chatConnectDoit(int loginType, const char *serverAddress, int serverPort, const char *nick, const char *user, const char *realName, int accountSpace, const char *email, const char *accountNick, const char *registeredNick, const char *password, const char *authTicket, const char *partnerKey, const char *gameName, const char *gameKey, const ChatGlobalCallbacks *callbacks, void *nickErrorCallback, void *fillInUserCallback, void *connectedCallback, void *connectParam, BOOL blocking) {
    const char *socketName = sStr_Empty_21;
    ChatConnection *chat;
    ASSERT(serverAddress != NULL, sStr_ServerAddressNeNULL, sStr_chatConnectDoit, 347);
    ASSERT(callbacks != NULL, sStr_CallbacksNeNULL_2, sStr_chatConnectDoit, 348);
    ASSERT(connectedCallback != NULL, sStr_ConnectCallbackNeNULL, sStr_chatConnectDoit, 349);
    if (loginType == 0) {
        ASSERT(nick != NULL, sStr_NickNeNULL_2, sStr_chatConnectDoit, 355);
        ASSERT(nick[0] != 0, sStr_Nick0_Ne0_3, sStr_chatConnectDoit, 355);
        ASSERT(strlen(nick) < 64, sStr_StrlenNickLtMAX_NICK, sStr_chatConnectDoit, 355);
        if (!nick || !nick[0]) {
            return NULL;
        }
        socketName = nick;
    } else if (loginType == 1) {
        ASSERT(accountSpace > 0, sStr_NamespaceID_Gt0, sStr_chatConnectDoit, 362);
        if (accountSpace <= 0) {
            return NULL;
        }
        REQUIRE_STRING(registeredNick, sStr_UniquenickAndUniquenick0, 365);
        REQUIRE_STRING(password, sStr_PasswordAndPassword0, 368);
        socketName = registeredNick;
    } else if (loginType == 2) {
        ASSERT(accountSpace >= 0, sStr_NamespaceID_Ge0, sStr_chatConnectDoit, 375);
        if (accountSpace < 0) {
            return NULL;
        }
        REQUIRE_STRING(email, sStr_EmailAndEmail0, 378);
        REQUIRE_STRING(accountNick, sStr_ProfilenickAndProfilenick0, 381);
        REQUIRE_STRING(password, sStr_PasswordAndPassword0, 384);
        socketName = accountNick;
    } else if (loginType == 3) {
        REQUIRE_STRING(authTicket, sStr_AuthtokenAndAuthtoken0, 391);
        REQUIRE_STRING(partnerKey, sStr_PartnerchallengeAndPartnerchallenge0, 394);
        socketName = sStr_preauth;
    }
    if (loginType != 0) {
        REQUIRE_STRING(gameName, sStr_GamenameAndGamename0, 401);
        REQUIRE_STRING(gameKey, sStr_SecretKeyAndSecretKey0, 404);
    }
    SocketStartUp();
    chat = DWCi_GsMalloc(sizeof(ChatConnection));
    if (!chat) {
        return NULL;
    }
    memset(chat, 0, sizeof(ChatConnection));
    chat->loginType = loginType;
    if (nick) {
        strncpy(chat->nick, nick, 64);
        chat->nick[63] = 0;
    }
    if (user) {
        strncpy(chat->user, user, 128);
        chat->user[127] = 0;
    }
    if (realName) {
        strncpy(chat->realName, realName, 128);
        chat->realName[127] = 0;
    }
    chat->accountSpace = accountSpace;
    if (email) {
        strncpy(chat->email, email, 64);
        chat->email[63] = 0;
    }
    if (accountNick) {
        strncpy(chat->accountNick, accountNick, 32);
        chat->accountNick[31] = 0;
    }
    if (registeredNick) {
        strncpy(chat->registeredNick, registeredNick, 64);
        chat->registeredNick[63] = 0;
    }
    if (password) {
        strncpy(chat->password, password, 32);
        chat->password[31] = 0;
    }
    if (authTicket) {
        strncpy(chat->authTicket, authTicket, 256);
        chat->authTicket[255] = 0;
    }
    if (partnerKey) {
        strncpy(chat->partnerKey, partnerKey, 256);
        chat->partnerKey[255] = 0;
    }
    strncpy(chat->serverAddress, serverAddress, 128);
    chat->serverAddress[127] = 0;
    chat->serverPort = serverPort;
    chat->globalCallbacks = *callbacks;
    chat->nextRequestId = 1;
    chat->connecting = TRUE;
    chat->unk_824 = 0;
    if (!ciInitChannels(chat)) {
        DWCi_GsFree(chat);
        SocketShutDown();
        return NULL;
    }
    if (!ciInitCallbacks(chat)) {
        ciCleanupChannels(chat);
        DWCi_GsFree(chat);
        SocketShutDown();
        return NULL;
    }
    if (!ciSocketInit(&chat->socket, socketName)) {
        ciCleanupCallbacks(chat);
        ciCleanupChannels(chat);
        DWCi_GsFree(chat);
        SocketShutDown();
        return NULL;
    }
    if (!ciSocketConnect(&chat->socket, serverAddress, serverPort)) {
        ciSocketDisconnect(&chat->socket);
        ciCleanupCallbacks(chat);
        ciCleanupChannels(chat);
        DWCi_GsFree(chat);
        SocketShutDown();
        return NULL;
    }
    chat->nickErrorCallback = nickErrorCallback;
    chat->fillInUserCallback = fillInUserCallback;
    chat->connectedCallback = connectedCallback;
    chat->connectParam = connectParam;
    if (gameName && gameName[0] && gameKey && gameKey[0]) {
        strncpy(chat->gameKey, gameKey, 128);
        chat->gameKey[127] = 0;
        ciSocketSendf(&chat->socket, sFmt_CRYPT_Des, sStr_X01_2, gameName);
    } else if (chat->fillInUserCallback) {
        ciSocketSend(&chat->socket, sStr_USRIP_2);
    } else {
        ciSendNickAndUser(chat);
    }
    if (blocking) {
        do {
            ciThink(chat, 0);
            msleep(10);
        } while (chat->connecting);
        if (!chat->connected) {
            chatDisconnect(chat);
            chat = NULL;
        }
    }
    return chat;
}

// Connects with a nick only (login type 0).
ChatConnection *chatConnectSpecialA(const char *serverAddress, int serverPort, const char *nick, const char *realName, const char *gameName, const char *gameKey, const ChatGlobalCallbacks *callbacks, void *nickErrorCallback, void *fillInUserCallback, void *connectedCallback, void *connectParam, BOOL blocking) {
    return chatConnectDoit(0, serverAddress, serverPort, nick, NULL, realName, 0, NULL, NULL, NULL, NULL, NULL, NULL, gameName, gameKey, callbacks, nickErrorCallback, fillInUserCallback, connectedCallback, connectParam, blocking);
}

// Tries another nick after the nick error callback (NULL gives up).
void chatRetryWithNickA(ChatConnection *chat, const char *nick) {
    int error;
    ASSERT_CHAT(chat, sStr_chatRetryWithNickA, 901);
    if (chat->connected) {
        return;
    }
    if (!nick) {
        ConnectFailed(chat);
        return;
    }
    strncpy(chat->nick, nick, 64);
    chat->nick[63] = 0;
    error = ciNickIsValid(nick);
    if (error) {
        ciNickError(chat, error, nick, 0, 0);
        return;
    }
    ciSocketSendf(&chat->socket, sFmt_NICK_2, nick);
}

// Registers a nick for the login (NULL gives up).
void chatRegisterUniqueNickA(ChatConnection *chat, int accountSpace, const char *nick, const char *cdKey) {
    ASSERT_CHAT(chat, sStr_chatRegisterUniqueNickA, 957);
    if (chat->connected) {
        return;
    }
    if (!nick) {
        ConnectFailed(chat);
        return;
    }
    if (!cdKey) {
        cdKey = sStr_Empty_21;
    }
    ciSocketSendf(&chat->socket, sFmt_REGISTERNICK, accountSpace, nick, cdKey);
    strncpy(chat->registeredNick, nick, 64);
    chat->registeredNick[63] = 0;
}

// Disconnects and frees the connection.
void chatDisconnect(ChatConnection *chat) {
    ASSERT_CHAT(chat, sStr_chatDisconnect, 1007);
    ciCleanupFilters(chat);
    if (!chat->disconnected && chat->globalCallbacks.disconnected) {
        ((void (*)(ChatConnection *, const char *, void *))chat->globalCallbacks.disconnected)(chat, sStr_Empty_21, chat->globalCallbacks.param);
    }
    if (chat->connected) {
        ciSocketSend(&chat->socket, sStr_QUIT_Later);
        ciSocketThink(&chat->socket);
    }
    ciCleanupChannels(chat);
    ciCleanupCallbacks(chat);
    ciSocketDisconnect(&chat->socket);
    DWCi_GsFree(chat);
    SocketShutDown();
}

void chatThink(ChatConnection *chat) {
    ciThink(chat, 0);
}

// Lists the channels matching the filter.
void chatEnumChannelsA(ChatConnection *chat, const char *filter, void *callbackEach, void *callbackAll, void *param, BOOL blocking) {
    int id;
    ASSERT_CHAT(chat, sStr_chatEnumChannelsA, 1422);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT(callbackAll || callbackEach, sStr_CallbackAllNeNULL_OrCallbackEachNeNULL, sStr_chatEnumChannelsA, 1425);
    if (!filter) {
        filter = sStr_Empty_21;
    }
    ciSocketSendf(&chat->socket, sFmt_LIST, filter);
    id = ciAddLISTFilter(chat, callbackEach, callbackAll, param);
    if (!blocking) {
        return;
    }
    WAIT_FOR_REQUEST(chat, id);
}

// Enters a channel.
void chatEnterChannelA(ChatConnection *chat, const char *channel, const char *password, const ChatChannelCallbacks *callbacks, void *callback, void *param, BOOL blocking) {
    int id;
    ASSERT_CHAT(chat, sStr_chatEnterChannelA, 1459);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, sStr_chatEnterChannelA, 1462);
    ASSERT(callbacks, sStr_CallbacksNeNULL_2, sStr_chatEnterChannelA, 1463);
    if (!password) {
        password = sStr_Empty_21;
    }
    ciSocketSendf(&chat->socket, sFmt_JOIN, channel, password);
    id = ciAddJOINFilter(chat, channel, callback, param, callbacks, password);
    ciChannelEntering(chat, channel);
    if (!blocking) {
        return;
    }
    WAIT_FOR_REQUEST(chat, id);
}

// Leaves a channel.
void chatLeaveChannelA(ChatConnection *chat, const char *channel, const char *reason) {
    ASSERT_CHAT(chat, sStr_chatLeaveChannelA, 1499);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, sStr_chatLeaveChannelA, 1502);
    if (!reason) {
        reason = sStr_Empty_21;
    }
    ciSocketSendf(&chat->socket, sFmt_PART, channel, reason);
    ciChannelLeft(chat, channel);
}

// Sends a message of the given type (0 .. 4) to a channel and queues the own channel message callback.
void chatSendChannelMessageA(ChatConnection *chat, const char *channel, const char *message, u32 type) {
    ChatChannelCallbacks *callbacks;
    ChatChannelMessageArgs args;
    ASSERT_CHAT(chat, sStr_chatSendChannelMessageA, 1532);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, sStr_chatSendChannelMessageA, 1535);
    ASSERT(type <= 4, sStr_TypeEqCHAT_MESSAGE_OrTypeEqCHAT_ACTION_OrTypeEq, sStr_chatSendChannelMessageA, 1536);
    if (!message || !*message) {
        return;
    }
    if (type == 0) {
        ciSocketSendf(&chat->socket, sFmt_PRIVMSG, channel, message);
    } else if (type == 1) {
        ciSocketSendf(&chat->socket, sFmt_PRIVMSG_ACTION, channel, message);
    } else if (type == 2) {
        ciSocketSendf(&chat->socket, sFmt_NOTICE, channel, message);
    } else if (type == 3) {
        ciSocketSendf(&chat->socket, sFmt_UTM, channel, message);
    } else if (type == 4) {
        ciSocketSendf(&chat->socket, sFmt_ATM, channel, message);
    } else {
        return;
    }
    callbacks = ciGetChannelCallbacks(chat, channel);
    if (!callbacks) {
        return;
    }
    args.sender = chat->nick;
    args.channel = channel;
    args.message = message;
    args.type = type;
    ciAddCallback_(chat, 4, callbacks->message, (u32 *)&args, callbacks->param, 0, (char *)channel, sizeof(args));
}

// Sets the mode of a channel ("+i-p..." from the eight mode fields, then the limit).
void chatSetChannelModeA(ChatConnection *chat, const char *channel, const ChatChannelMode *mode) {
    char buffer[64];
    int limit;
    ASSERT_CHAT(chat, sStr_chatSetChannelModeA, 1663);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, sStr_chatSetChannelModeA, 1666);
    ASSERT(mode, sStr_ModeNeNULL_2, sStr_chatSetChannelModeA, 1667);
    memcpy(buffer, sStr_XiXpXsXmXnXtXlXe, 17);
    if (mode->inviteOnly) {
        buffer[0] = '+';
    } else {
        buffer[0] = '-';
    }
    if (mode->private) {
        buffer[2] = '+';
    } else {
        buffer[2] = '-';
    }
    if (mode->secret) {
        buffer[4] = '+';
    } else {
        buffer[4] = '-';
    }
    if (mode->moderated) {
        buffer[6] = '+';
    } else {
        buffer[6] = '-';
    }
    if (mode->noExternalMessages) {
        buffer[8] = '+';
    } else {
        buffer[8] = '-';
    }
    if (mode->onlyOpsChangeTopic) {
        buffer[10] = '+';
    } else {
        buffer[10] = '-';
    }
    if (mode->limit > 0) {
        buffer[12] = '+';
    } else {
        buffer[12] = '-';
    }
    if (mode->opsObeyChannelLimit) {
        buffer[14] = '+';
    } else {
        buffer[14] = '-';
    }
    limit = mode->limit;
    if (limit > 0) {
        sprintf(buffer + strlen(buffer), sFmt_SpaceInt, limit);
    }
    ciSocketSendf(&chat->socket, sFmt_MODE_2, channel, buffer);
}

// Gets the mode of a channel: from the channel table when it is known, else from the server.
void chatGetChannelModeA(ChatConnection *chat, const char *channel, void *callback, void *param, BOOL blocking) {
    ChatChannelMode mode;
    ChatChannelModeArgs args;
    int id;
    ASSERT_CHAT(chat, sStr_chatGetChannelModeA, 1730);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, sStr_chatGetChannelModeA, 1733);
    ASSERT(callback, sStr_CallbackNeNULL_4, sStr_chatGetChannelModeA, 1734);
    if (ciInChannel(chat, channel) && ciGetChannelMode(chat, channel, &mode)) {
        id = ciGetNextID(chat);
        args.success = TRUE;
        args.mode = &mode;
        args.channel = channel;
        ciAddCallback_(chat, 17, callback, (u32 *)&args, param, id, NULL, sizeof(args));
        if (!blocking) {
            return;
        }
        WAIT_FOR_REQUEST(chat, id);
    } else {
        ciSocketSendf(&chat->socket, sFmt_MODE_3, channel);
        id = ciAddCMODEFilter(chat, channel, callback, param);
        if (!blocking) {
            return;
        }
        WAIT_FOR_REQUEST(chat, id);
    }
}

// Sets (limit > 0) or removes the user limit of a channel.
void chatSetChannelLimitA(ChatConnection *chat, const char *channel, int limit) {
    ASSERT_CHAT(chat, sStr_chatSetChannelLimitA, 1870);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, sStr_chatSetChannelLimitA, 1873);
    ASSERT(limit >= 0, sStr_LimitGe0, sStr_chatSetChannelLimitA, 1874);
    if (limit) {
        ciSocketSendf(&chat->socket, sFmt_MODE_L, channel, limit);
    } else {
        ciSocketSendf(&chat->socket, sFmt_MODE_L_2, channel);
    }
}

// Number of users in a joined channel, -1 if unknown.
int chatGetChannelNumUsersA(ChatConnection *chat, const char *channel) {
    ASSERT_CHAT(chat, sStr_chatGetChannelNumUsersA, 2006);
    if (!chat->connected) {
        return -1;
    }
    ASSERT_CHANNEL(channel, sStr_chatGetChannelNumUsersA, 2010);
    if (!channel || !*channel) {
        return -1;
    }
    if (!ciInChannel(chat, channel)) {
        return -1;
    }
    return ciGetChannelNumUsers(chat, channel);
}

// Whether the channel has been joined.
BOOL chatInChannelA(ChatConnection *chat, const char *channel) {
    ASSERT_CHAT(chat, sStr_chatInChannelA, 2036);
    if (!chat->connected) {
        return FALSE;
    }
    ASSERT_CHANNEL(channel, sStr_chatInChannelA, 2040);
    if (!channel || !*channel) {
        return FALSE;
    }
    return ciInChannel(chat, channel);
}

// ChatUserListFunc of chatEnumUsersA: passes the user list of a joined channel to the enum users callback.
void ciEnumUsersCallback(ChatConnection *chat, const char *channel, int count, ChatChannelUser **users, int *modes, ChatEnumUsers *volatile enumUsers) {
    ASSERT_CHAT(chat, sStr_ciEnumUsersCallback, 2066);
    ASSERT_CHANNEL(channel, sStr_ciEnumUsersCallback, 2070);
    ASSERT(count >= 0, sStr_NumUsersGe0, sStr_ciEnumUsersCallback, 2071);
    ASSERT(enumUsers, sStr_ParamNeNULL, sStr_ciEnumUsersCallback, 2087);
    {
        ChatEnumUsers *data = enumUsers;
        ASSERT(data->callback, sStr_DataCallbackNeNULL_3, sStr_ciEnumUsersCallback, 2092);
        data->callback(chat, TRUE, channel, count, users, modes, data->param);
    }
}

// Lists the users of a channel: from the channel table when it is joined, else from the server.
void chatEnumUsersA(ChatConnection *chat, const char *channel, ChatEnumUsersFunc callback, void *param, BOOL blocking) {
    ChatEnumUsers enumUsers;
    int id;
    ASSERT_CHAT(chat, sStr_chatEnumUsersA, 2119);
    if (chat == NULL || !chat->connected) {
        return;
    }
    ASSERT(callback, sStr_CallbackNeNULL_4, sStr_chatEnumUsersA, 2123);
    if (channel == NULL) {
        channel = sStr_Empty_21;
    }
    if (*channel && ciInChannel(chat, channel)) {
        enumUsers.callback = callback;
        enumUsers.param = param;
        ciChannelListUsers(chat, channel, (ChatUserListFunc)ciEnumUsersCallback, (int)&enumUsers);
        return;
    }
    ciSocketSendf(&chat->socket, sFmt_NAMES, channel);
    if (*channel == 0) {
        channel = NULL;
    }
    id = ciAddNAMESFilter(chat, channel, callback, param);
    if (blocking) {
        WAIT_FOR_REQUEST(chat, id);
    }
}

// Sends a message of the given type (0 .. 4) to a user.
void chatSendUserMessageA(ChatConnection *chat, const char *nick, const char *message, u32 type) {
    ASSERT_CHAT(chat, sStr_chatSendUserMessageA, 2185);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT(nick, sStr_UserNeNULL_3, sStr_chatSendUserMessageA, 2188);
    ASSERT(*nick, sStr_User0_Ne0_3, sStr_chatSendUserMessageA, 2188);
    ASSERT(strlen(nick) < 128, sStr_StrlenUserLtMAX_USER, sStr_chatSendUserMessageA, 2188);
    ASSERT(type <= 4, sStr_TypeEqCHAT_MESSAGE_OrTypeEqCHAT_ACTION_OrTypeEq, sStr_chatSendUserMessageA, 2189);
    if (!message || !*message) {
        return;
    }
    if (type == 0) {
        ciSocketSendf(&chat->socket, sFmt_PRIVMSG, nick, message);
        return;
    }
    if (type == 1) {
        ciSocketSendf(&chat->socket, sFmt_PRIVMSG_ACTION, nick, message);
        return;
    }
    if (type == 2) {
        ciSocketSendf(&chat->socket, sFmt_NOTICE, nick, message);
        return;
    }
    if (type == 3) {
        ciSocketSendf(&chat->socket, sFmt_UTM, nick, message);
        return;
    }
    if (type == 4) {
        ciSocketSendf(&chat->socket, sFmt_ATM, nick, message);
        return;
    }
}

// Next request cookie, "000" .. "999".
char *ciGetNextCookie(void) {
    sprintf(ciCookieBuffer, sFmt_Int_6, ciCookieCounter++);
    ciCookieCounter %= 1000;
    return ciCookieBuffer;
}

// Sets keys of a channel (nick NULL or empty) or of a user in it.
void chatSetChannelKeysA(ChatConnection *chat, const char *channel, const char *nick, int count, const char **keys, const char **values) {
    char buffer[512];
    int i;
    const char *value;
    ASSERT_CHAT(chat, sStr_chatSetChannelKeysA, 2775);
    if (!chat || !chat->connected) {
        return;
    }
    if (!nick || !*nick) {
        sprintf(buffer, sFmt_SETCHANKEY, channel);
    } else {
        sprintf(buffer, sFmt_SETCKEY, channel, nick);
    }
    for (i = 0; i < count; i++) {
        value = values[i];
        if (!value) {
            value = sStr_Empty_21;
        }
        sprintf(buffer + strlen(buffer), sFmt_BackslashStrBackslashStr, keys[i], value);
    }
    ciSocketSend(&chat->socket, buffer);
}

// Sends the get keys request for a channel or a user ('\' in the keys becomes '/'); returns whether the
// broadcast keys were asked for.
BOOL ciSendGetChannelKey(ChatConnection *chat, const char *channel, const char *nick, const char *cookie, int count, const char **keys) {
    char buffer[512];
    BOOL broadcastKeys = FALSE;
    int i, length, total;
    const char *key;
    int j;
    char *p;
    ASSERT_CHAT(chat, sStr_ciSendGetChannelKey, 2841);
    ASSERT(channel != NULL && *channel != 0, sStr_ChannelAndChannel0, sStr_ciSendGetChannelKey, 2843);
    ASSERT(cookie != NULL && *cookie != 0, sStr_CookieAndCookie0, sStr_ciSendGetChannelKey, 2844);
    ASSERT(count == 0 || keys != NULL, sStr_NotNumOrKeys, sStr_ciSendGetChannelKey, 2845);
    if (nick == NULL || *nick == 0) {
        sprintf(buffer, sFmt_GETCHANKEY_0, channel, cookie);
    } else {
        sprintf(buffer, sFmt_GETCKEY_0, channel, nick, cookie);
    }
    total = strlen(buffer);
    for (i = 0; i < count; i++) {
        key = keys[i];
        if (key == NULL || *key == 0) {
            continue;
        }
        if (strcmp(key, sStr_B_UnderscoreStar_2) == 0) {
            broadcastKeys = TRUE;
            continue;
        }
        length = strlen(key);
        if (total + length + 1 >= 512) {
            continue;
        }
        buffer[total] = '\\';
        total++;
        p = buffer + total;
        memcpy(p, keys[i], length);
        for (j = total; j < total + length; j++) {
            if (*p == '\\') {
                *p = '/';
            }
            p++;
        }
        total += length;
        buffer[total] = 0;
    }
    if (broadcastKeys && total + 4 < 512) {
        {
            memcpy(buffer + total, &sStr_BackslashB_UnderscoreStar.first, 3);
            *(ChatBytes2 *)(buffer + total + 3) = sStr_BackslashB_UnderscoreStar.last;
        }
        total += 4;
    }
    if (count == 0 && (nick == NULL || *nick == 0)) {
        memcpy(buffer + total, &sStr_Star_3, 2);
    }
    ciSocketSend(&chat->socket, buffer);
    return broadcastKeys;
}

// Gets keys of a channel (nick NULL or empty), of a user in it or of all its users (nick "*").
void chatGetChannelKeysA(ChatConnection *chat, const char *channel, const char *nick, int count, const char **keys, void *callback, void *param, BOOL blocking) {
    char *cookie;
    BOOL broadcastKeys;
    int id;
    ASSERT_CHAT(chat, sStr_chatGetChannelKeysA, 2927);
    if (chat == NULL || !chat->connected) {
        return;
    }
    ASSERT(count >= 0, sStr_NumGe0, sStr_chatGetChannelKeysA, 2930);
    ASSERT(count == 0 || keys != NULL, sStr_NotNumOrKeys, sStr_chatGetChannelKeysA, 2931);
    cookie = ciGetNextCookie();
    broadcastKeys = ciSendGetChannelKey(chat, channel, nick, cookie, count, keys);
    if (nick == NULL || *nick == 0) {
        id = ciAddGETCHANKEYFilter(chat, cookie, count, keys, broadcastKeys, callback, param);
    } else {
        id = ciAddGETCKEYFilter(chat, cookie, count, keys, strcmp(nick, sStr_Star_3.bytes) == 0, broadcastKeys, callback, param);
    }
    if (blocking) {
        do {
            ciThink(chat, id);
            msleep(10);
        } while (ciCheckForID(chat, id) != 0);
    }
}
