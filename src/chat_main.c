#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

#include <string.h>

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern int sprintf(char *, const char *, ...);
extern int ov45_0225699C; // 0, the number sent with the CRYPT command
extern const char ov45_022569A0[];
extern const char ov45_022569A8[];
extern const char ov45_022569B4[];
extern const char ov45_022569CC[];
extern const char ov45_022569D8[];
extern const char ov45_022569E8[];
extern const char ov45_022569F8[];
extern const char ov45_02256A08[];
extern const char ov45_02256A18[];
extern const char ov45_02256A2C[];
extern const char ov45_02256A40[];
extern const char ov45_02256A54[];
extern const char ov45_02256A68[];
extern const char ov45_02256A7C[];
extern const char ov45_02256A90[];
extern const char ov45_02256AA4[];
extern const char ov45_02256AB8[];
extern const char ov45_02256ACC[];
extern const char ov45_02256AE0[];
extern const char ov45_02256AF4[];
extern const char ov45_02256B0C[];
extern const char ov45_02256B24[];
extern const char ov45_02256B3C[];
extern const char ov45_02256B54[];
extern const char ov45_02256B6C[];
extern const char ov45_02256B84[];
extern const char ov45_02256B94[];
extern const char ov45_02256BA0[];
extern const char ov45_02256BB0[];
extern const ChatBytes2 ov45_02256BC0;
extern const char ov45_02256BC4[];
extern const char ov45_02256BCC[];
extern const char ov45_02256BE0[];
extern const char ov45_02256BEC[];
extern const char ov45_02256C00[];
extern const char ov45_02256C10[];
extern const char ov45_02256C28[];
extern const char ov45_02256C2C[];
extern const char ov45_02256C30[];
extern const char ov45_02256C48[];
extern const char ov45_02256C5C[];
extern const char ov45_02256C74[];
extern const char ov45_02256C84[];
extern const char ov45_02256C94[];
extern const char ov45_02256CAC[];
extern const char ov45_02256CBC[];
extern const char ov45_02256CD8[];
extern const char ov45_02256CF0[];
extern const char ov45_02256D04[];
extern const char ov45_02256D18[];
extern const char ov45_02256D38[];
extern const char ov45_02256D54[];
extern const char ov45_02256D7C[];
extern const char ov45_02256D84[];
extern const char ov45_02256D9C[];
extern const char ov45_02256DB8[];
extern const char ov45_02256DC8[];
extern const char ov45_02256DD0[];
extern const char ov45_02256DDC[];
extern const char ov45_02256DF4[];
extern const char ov45_02256E64[];
extern const char ov45_02256E94[];
extern const char ov45_02256E9C[];
extern const char ov45_02256EAC[];
extern const char ov45_02256EC0[];
extern const char ov45_02256ECC[];
extern const char ov45_02256ED8[];
extern const char ov45_02256F50[];
extern const char ov45_02256F60[];
extern const char ov45_02256F78[];
extern const char ov45_02256F88[];
extern const char ov45_02256F94[];
extern const char ov45_02256FA0[];
extern const char ov45_02256FB4[];
extern const char ov45_02256FC4[];
extern const char ov45_02256FD8[];
extern const char ov45_02256FDC[];
extern const char ov45_02256FE8[];
extern const char ov45_02256FF0[];
extern const char ov45_02256FFC[];
extern const char ov45_0225700C[];
extern const char ov45_02257018[];
extern const char ov45_02257028[];
extern const char ov45_02257038[];
extern const char ov45_02257050[];
extern const char ov45_0225705C[];
extern const char ov45_0225706C[];
extern const char ov45_0225707C[];
extern const char ov45_02257094[];
extern const char ov45_0225709C[];
extern const char ov45_022570A4[];
extern const char ov45_022570B8[];
extern const char ov45_022570C4[];
extern const char ov45_022570D4[];
extern const char ov45_022570E4[];
extern const char ov45_022570FC[];
extern const char ov45_0225710C[];
extern const char ov45_02257124[];
extern const char ov45_0225713C[];

// The key that asks for the broadcast keys of a user (5 bytes with the terminator).
extern const struct {
    ChatBytes3 first;
    ChatBytes2 last;
} ov45_02257140;

// Cookie buffer and counter of ov45_022514F4.
extern char ov45_022577F0[];
extern int ov45_022577F4;

// Assertion with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, ov45_02256B94, func, line); \
    }
#define ASSERT_CHAT(chat, func, line) ASSERT(chat, ov45_02256BA0, func, line)
// A valid channel name: not NULL and not empty.
#define ASSERT_CHANNEL(name, func, line)     \
    ASSERT(name, ov45_02256E9C, func, line); \
    ASSERT(*name, ov45_02256EAC, func, line);
// A string argument of ov45_0224FABC: asserted and required.
#define REQUIRE_STRING(s, message, line)                 \
    ASSERT((s) && (s)[0], message, ov45_02256A08, line); \
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

// Calls ov45_0224F770 until the request id has been answered.
#define WAIT_FOR_REQUEST(chat, id) \
    do {                           \
        ov45_0224F770(chat, id);   \
        ov00_021FF3C0(10);         \
    } while (ov45_0224F660(chat, id))

// Calls the handler of the message's command; returns whether the command is known.
int ov45_0224F5BC(ChatConnection *chat, ChatMessage *message) {
    int i;
    ASSERT(message, ov45_02256B84, ov45_02256B24, 79);
    for (i = 0; i < ov45_02255DA0; i++) {
        if (strnicmp(message->command, ov45_02256410[i].command) == 0) {
            void (*handler)(ChatConnection *, ChatMessage *) = ov45_02256410[i].handler;
            if (handler) {
                handler(chat, message);
            }
            return 1;
        }
    }
    return 0;
}

// Whether the request id is still waiting for an answer or a callback.
BOOL ov45_0224F660(ChatConnection *chat, int id) {
    if (ov45_0224A630(chat, id) || ov45_022474FC(chat, id)) {
        return TRUE;
    }
    return FALSE;
}

// The connection was lost: queue the disconnected callback once.
void ov45_0224F69C(ChatConnection *chat, const char *reason) {
    const char *args;
    ASSERT_CHAT(chat, ov45_02256A54, 115);
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
    ov45_02244D30(chat, 1, chat->globalCallbacks.disconnected, (u32 *)&args, chat->globalCallbacks.param, 0, NULL, 4);
}

// Per-frame work: receive and handle the server lines, then call the queued callbacks of the request id
// (0 = all).
void ov45_0224F770(ChatConnection *chat, int id) {
    ChatMessage *message;
    u32 text;
    ASSERT_CHAT(chat, ov45_022569A0, 154);
    if (chat->socket.state == 1) {
        ov45_02252AA0(&chat->socket);
        message = ov45_02253B5C(&chat->socket);
        while (message) {
            if (chat->globalCallbacks.raw) {
                text = (u32)message->text;
                ov45_02244D30(chat, 0, chat->globalCallbacks.raw, &text, chat->globalCallbacks.param, 0, NULL, 4);
            }
            ov45_0224F5BC(chat, message);
            message = ov45_02253B5C(&chat->socket);
        }
        if (chat->socket.state == 2) {
            ov45_0224F69C(chat, ov45_02256BB0);
        }
    }
    ov45_0224A578(chat);
    ov45_022471A0(chat, id);
}

// Sends the nick (login types 0 and 2) or "*".
void ov45_0224F860(ChatConnection *chat) {
    const char *nick;
    int error;
    ASSERT_CHAT(chat, ov45_022569A8, 206);
    if (!chat->loginType) {
        error = ov45_02251A8C(chat->nick);
        if (error) {
            ov45_02251B0C(chat, error, chat->nick, 0, 0);
            return;
        }
        nick = chat->nick;
    } else if (chat->loginType == 2 && !chat->accountSpace) {
        error = ov45_02251A8C(chat->accountNick);
        if (error) {
            ov45_02251B0C(chat, error, chat->accountNick, 0, 0);
            return;
        }
        nick = chat->accountNick;
    } else {
        nick = ov45_02256BC0.bytes;
    }
    ov45_02252F74(&chat->socket, ov45_02256BC4, nick);
}

void ov45_0224F934(ChatConnection *chat) {
    ASSERT_CHAT(chat, ov45_022569B4, 260);
    ov45_02252F74(&chat->socket, ov45_02256BCC, chat->user, ov45_02256BE0, chat->serverAddress, chat->realName);
}

void ov45_0224F9A0(ChatConnection *chat) {
    ov45_0224F934(chat);
    ov45_0224F860(chat);
}

// Sends the login of login types 1 .. 3.
void ov45_0224F9B8(ChatConnection *chat) {
    char hash[36];
    ASSERT_CHAT(chat, ov45_022569CC, 280);
    if (chat->loginType == 3) {
        ov45_02252F74(&chat->socket, ov45_02256BEC, chat->authTicket, chat->partnerKey);
        return;
    }
    ov00_021FF31C(chat->password, strlen(chat->password), hash);
    if (chat->loginType == 1) {
        ov45_02252F74(&chat->socket, ov45_02256C00, chat->accountSpace, chat->registeredNick, hash);
        return;
    }
    if (chat->loginType == 2) {
        ov45_02252F74(&chat->socket, ov45_02256C10, chat->accountSpace, hash, chat->accountNick, chat->email);
        return;
    }
    __msl_assertion_failed(ov45_02256C28, ov45_02256B94, ov45_022569CC, 318);
}

// Creates the connection and starts the login; with blocking, waits until the connect attempt has ended
// (NULL if it failed).
ChatConnection *ov45_0224FABC(int loginType, const char *serverAddress, int serverPort, const char *nick, const char *user, const char *realName, int accountSpace, const char *email, const char *accountNick, const char *registeredNick, const char *password, const char *authTicket, const char *partnerKey, const char *gameName, const char *gameKey, const ChatGlobalCallbacks *callbacks, void *nickErrorCallback, void *userAddressCallback, void *connectedCallback, void *connectParam, BOOL blocking) {
    const char *socketName = ov45_02256C2C;
    ChatConnection *chat;
    ASSERT(serverAddress != NULL, ov45_02256C30, ov45_02256A08, 347);
    ASSERT(callbacks != NULL, ov45_02256C48, ov45_02256A08, 348);
    ASSERT(connectedCallback != NULL, ov45_02256C5C, ov45_02256A08, 349);
    if (loginType == 0) {
        ASSERT(nick != NULL, ov45_02256C74, ov45_02256A08, 355);
        ASSERT(nick[0] != 0, ov45_02256C84, ov45_02256A08, 355);
        ASSERT(strlen(nick) < 64, ov45_02256C94, ov45_02256A08, 355);
        if (!nick || !nick[0]) {
            return NULL;
        }
        socketName = nick;
    } else if (loginType == 1) {
        ASSERT(accountSpace > 0, ov45_02256CAC, ov45_02256A08, 362);
        if (accountSpace <= 0) {
            return NULL;
        }
        REQUIRE_STRING(registeredNick, ov45_02256CBC, 365);
        REQUIRE_STRING(password, ov45_02256CD8, 368);
        socketName = registeredNick;
    } else if (loginType == 2) {
        ASSERT(accountSpace >= 0, ov45_02256CF0, ov45_02256A08, 375);
        if (accountSpace < 0) {
            return NULL;
        }
        REQUIRE_STRING(email, ov45_02256D04, 378);
        REQUIRE_STRING(accountNick, ov45_02256D18, 381);
        REQUIRE_STRING(password, ov45_02256CD8, 384);
        socketName = accountNick;
    } else if (loginType == 3) {
        REQUIRE_STRING(authTicket, ov45_02256D38, 391);
        REQUIRE_STRING(partnerKey, ov45_02256D54, 394);
        socketName = ov45_02256D7C;
    }
    if (loginType != 0) {
        REQUIRE_STRING(gameName, ov45_02256D84, 401);
        REQUIRE_STRING(gameKey, ov45_02256D9C, 404);
    }
    ov00_021FF3CC();
    chat = ov00_021EC3A8(sizeof(ChatConnection));
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
    if (!ov45_02247968(chat)) {
        ov00_021EC3D8(chat);
        ov00_021FF3D0();
        return NULL;
    }
    if (!ov45_02244B9C(chat)) {
        ov45_022479F0(chat);
        ov00_021EC3D8(chat);
        ov00_021FF3D0();
        return NULL;
    }
    if (!ov45_02251EF0(&chat->socket, socketName)) {
        ov45_02244BCC(chat);
        ov45_022479F0(chat);
        ov00_021EC3D8(chat);
        ov00_021FF3D0();
        return NULL;
    }
    if (!ov45_02251F6C(&chat->socket, serverAddress, serverPort)) {
        ov45_022522B4(&chat->socket);
        ov45_02244BCC(chat);
        ov45_022479F0(chat);
        ov00_021EC3D8(chat);
        ov00_021FF3D0();
        return NULL;
    }
    chat->nickErrorCallback = nickErrorCallback;
    chat->userAddressCallback = userAddressCallback;
    chat->connectedCallback = connectedCallback;
    chat->connectParam = connectParam;
    if (gameName && gameName[0] && gameKey && gameKey[0]) {
        strncpy(chat->gameKey, gameKey, 128);
        chat->gameKey[127] = 0;
        ov45_02252F74(&chat->socket, ov45_02256DB8, ov45_0225699C, gameName);
    } else if (chat->userAddressCallback) {
        ov45_02252CB4(&chat->socket, ov45_02256DC8);
    } else {
        ov45_0224F9A0(chat);
    }
    if (blocking) {
        do {
            ov45_0224F770(chat, 0);
            ov00_021FF3C0(10);
        } while (chat->connecting);
        if (!chat->connected) {
            ov45_022504E0(chat);
            chat = NULL;
        }
    }
    return chat;
}

// Connects with a nick only (login type 0).
ChatConnection *ov45_022502D0(const char *serverAddress, int serverPort, const char *nick, const char *realName, const char *gameName, const char *gameKey, const ChatGlobalCallbacks *callbacks, void *nickErrorCallback, void *userAddressCallback, void *connectedCallback, void *connectParam, BOOL blocking) {
    return ov45_0224FABC(0, serverAddress, serverPort, nick, NULL, realName, 0, NULL, NULL, NULL, NULL, NULL, NULL, gameName, gameKey, callbacks, nickErrorCallback, userAddressCallback, connectedCallback, connectParam, blocking);
}

// Tries another nick after the nick error callback (NULL gives up).
void ov45_0225035C(ChatConnection *chat, const char *nick) {
    int error;
    ASSERT_CHAT(chat, ov45_02256A68, 901);
    if (chat->connected) {
        return;
    }
    if (!nick) {
        ConnectFailed(chat);
        return;
    }
    strncpy(chat->nick, nick, 64);
    chat->nick[63] = 0;
    error = ov45_02251A8C(nick);
    if (error) {
        ov45_02251B0C(chat, error, nick, 0, 0);
        return;
    }
    ov45_02252F74(&chat->socket, ov45_02256DD0, nick);
}

// Registers a nick for the login (NULL gives up).
void ov45_02250424(ChatConnection *chat, int accountSpace, const char *nick, const char *cdKey) {
    ASSERT_CHAT(chat, ov45_02256B3C, 957);
    if (chat->connected) {
        return;
    }
    if (!nick) {
        ConnectFailed(chat);
        return;
    }
    if (!cdKey) {
        cdKey = ov45_02256C2C;
    }
    ov45_02252F74(&chat->socket, ov45_02256DDC, accountSpace, nick, cdKey);
    strncpy(chat->registeredNick, nick, 64);
    chat->registeredNick[63] = 0;
}

// Disconnects and frees the connection.
void ov45_022504E0(ChatConnection *chat) {
    ASSERT_CHAT(chat, ov45_022569E8, 1007);
    ov45_0224A6B4(chat);
    if (!chat->disconnected && chat->globalCallbacks.disconnected) {
        ((void (*)(ChatConnection *, const char *, void *))chat->globalCallbacks.disconnected)(chat, ov45_02256C2C, chat->globalCallbacks.param);
    }
    if (chat->connected) {
        ov45_02252CB4(&chat->socket, ov45_02256DF4);
        ov45_02252AA0(&chat->socket);
    }
    ov45_022479F0(chat);
    ov45_02244BCC(chat);
    ov45_022522B4(&chat->socket);
    ov00_021EC3D8(chat);
    ov00_021FF3D0();
}

void ov45_02250590(ChatConnection *chat) {
    ov45_0224F770(chat, 0);
}

// Lists the channels matching the filter.
void ov45_022505A0(ChatConnection *chat, const char *filter, void *callbackAll, void *callbackEach, void *param, BOOL blocking) {
    int id;
    ASSERT_CHAT(chat, ov45_02256A18, 1422);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT(callbackEach || callbackAll, ov45_02256E64, ov45_02256A18, 1425);
    if (!filter) {
        filter = ov45_02256C2C;
    }
    ov45_02252F74(&chat->socket, ov45_02256E94, filter);
    id = ov45_0224A7F0(chat, callbackAll, callbackEach, param);
    if (!blocking) {
        return;
    }
    WAIT_FOR_REQUEST(chat, id);
}

// Enters a channel.
void ov45_02250688(ChatConnection *chat, const char *channel, const char *password, const ChatChannelCallbacks *callbacks, void *callback, void *param, BOOL blocking) {
    int id;
    ASSERT_CHAT(chat, ov45_02256A2C, 1459);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, ov45_02256A2C, 1462);
    ASSERT(callbacks, ov45_02256C48, ov45_02256A2C, 1463);
    if (!password) {
        password = ov45_02256C2C;
    }
    ov45_02252F74(&chat->socket, ov45_02256EC0, channel, password);
    id = ov45_0224A874(chat, channel, callback, param, callbacks, password);
    ov45_02247A44(chat, channel);
    if (!blocking) {
        return;
    }
    WAIT_FOR_REQUEST(chat, id);
}

// Leaves a channel.
void ov45_022507D8(ChatConnection *chat, const char *channel, const char *reason) {
    ASSERT_CHAT(chat, ov45_02256A40, 1499);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, ov45_02256A40, 1502);
    if (!reason) {
        reason = ov45_02256C2C;
    }
    ov45_02252F74(&chat->socket, ov45_02256ECC, channel, reason);
    ov45_02247D84(chat, channel);
}

// Sends a message of the given type (0 .. 4) to a channel and queues the own channel message callback.
void ov45_0225089C(ChatConnection *chat, const char *channel, const char *message, u32 type) {
    ChatChannelCallbacks *callbacks;
    ChatChannelMessageArgs args;
    ASSERT_CHAT(chat, ov45_02256B6C, 1532);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, ov45_02256B6C, 1535);
    ASSERT(type <= 4, ov45_02256ED8, ov45_02256B6C, 1536);
    if (!message || !*message) {
        return;
    }
    if (type == 0) {
        ov45_02252F74(&chat->socket, ov45_02256F50, channel, message);
    } else if (type == 1) {
        ov45_02252F74(&chat->socket, ov45_02256F60, channel, message);
    } else if (type == 2) {
        ov45_02252F74(&chat->socket, ov45_02256F78, channel, message);
    } else if (type == 3) {
        ov45_02252F74(&chat->socket, ov45_02256F88, channel, message);
    } else if (type == 4) {
        ov45_02252F74(&chat->socket, ov45_02256F94, channel, message);
    } else {
        return;
    }
    callbacks = ov45_02247E98(chat, channel);
    if (!callbacks) {
        return;
    }
    args.sender = chat->nick;
    args.channel = channel;
    args.message = message;
    args.type = type;
    ov45_02244D30(chat, 4, callbacks->message, (u32 *)&args, callbacks->param, 0, (char *)channel, sizeof(args));
}

// Sets the mode of a channel ("+i-p..." from the eight mode fields, then the limit).
void ov45_02250A80(ChatConnection *chat, const char *channel, const ChatChannelMode *mode) {
    char buffer[64];
    int limit;
    ASSERT_CHAT(chat, ov45_02256ACC, 1663);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, ov45_02256ACC, 1666);
    ASSERT(mode, ov45_02256FB4, ov45_02256ACC, 1667);
    memcpy(buffer, ov45_02256FC4, 17);
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
    if (mode->unk_18) {
        buffer[14] = '+';
    } else {
        buffer[14] = '-';
    }
    limit = mode->limit;
    if (limit > 0) {
        sprintf(buffer + strlen(buffer), ov45_02256FD8, limit);
    }
    ov45_02252F74(&chat->socket, ov45_02256FDC, channel, buffer);
}

// Gets the mode of a channel: from the channel table when it is known, else from the server.
void ov45_02250C80(ChatConnection *chat, const char *channel, void *callback, void *param, BOOL blocking) {
    ChatChannelMode mode;
    ChatChannelModeArgs args;
    int id;
    ASSERT_CHAT(chat, ov45_02256AE0, 1730);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, ov45_02256AE0, 1733);
    ASSERT(callback, ov45_02256FA0, ov45_02256AE0, 1734);
    if (ov45_02248238(chat, channel) && ov45_02248288(chat, channel, &mode)) {
        id = ov45_0224A5E4(chat);
        args.success = TRUE;
        args.mode = &mode;
        args.channel = channel;
        ov45_02244D30(chat, 17, callback, (u32 *)&args, param, id, NULL, sizeof(args));
        if (!blocking) {
            return;
        }
        WAIT_FOR_REQUEST(chat, id);
    } else {
        ov45_02252F74(&chat->socket, ov45_02256FE8, channel);
        id = ov45_0224A9F8(chat, channel, callback, param);
        if (!blocking) {
            return;
        }
        WAIT_FOR_REQUEST(chat, id);
    }
}

// Sets (limit > 0) or removes the user limit of a channel.
void ov45_02250E6C(ChatConnection *chat, const char *channel, int limit) {
    ASSERT_CHAT(chat, ov45_02256AF4, 1870);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT_CHANNEL(channel, ov45_02256AF4, 1873);
    ASSERT(limit >= 0, ov45_02256FF0, ov45_02256AF4, 1874);
    if (limit) {
        ov45_02252F74(&chat->socket, ov45_02256FFC, channel, limit);
    } else {
        ov45_02252F74(&chat->socket, ov45_0225700C, channel);
    }
}

// Number of users in a joined channel, -1 if unknown.
int ov45_02250F5C(ChatConnection *chat, const char *channel) {
    ASSERT_CHAT(chat, ov45_02256B54, 2006);
    if (!chat->connected) {
        return -1;
    }
    ASSERT_CHANNEL(channel, ov45_02256B54, 2010);
    if (!channel || !*channel) {
        return -1;
    }
    if (!ov45_02248238(chat, channel)) {
        return -1;
    }
    return ov45_022487BC(chat, channel);
}

// Whether the channel has been joined.
BOOL ov45_02251024(ChatConnection *chat, const char *channel) {
    ASSERT_CHAT(chat, ov45_022569D8, 2036);
    if (!chat->connected) {
        return FALSE;
    }
    ASSERT_CHANNEL(channel, ov45_022569D8, 2040);
    if (!channel || !*channel) {
        return FALSE;
    }
    return ov45_02248238(chat, channel);
}

// ChatUserListFunc of ov45_02251208: passes the user list of a joined channel to the enum users callback.
void ov45_022510D4(ChatConnection *chat, const char *channel, int count, ChatChannelUser **users, int *modes, ChatEnumUsers *volatile enumUsers) {
    ASSERT_CHAT(chat, ov45_02256A90, 2066);
    ASSERT_CHANNEL(channel, ov45_02256A90, 2070);
    ASSERT(count >= 0, ov45_02257018, ov45_02256A90, 2071);
    ASSERT(enumUsers, ov45_02257028, ov45_02256A90, 2087);
    {
        ChatEnumUsers *data = enumUsers;
        ASSERT(data->callback, ov45_02257038, ov45_02256A90, 2092);
        data->callback(chat, TRUE, channel, count, users, modes, data->param);
    }
}

// Lists the users of a channel: from the channel table when it is joined, else from the server.
void ov45_02251208(ChatConnection *chat, const char *channel, ChatEnumUsersFunc callback, void *param, BOOL blocking) {
    ChatEnumUsers enumUsers;
    int id;
    ASSERT_CHAT(chat, ov45_022569F8, 2119);
    if (chat == NULL || !chat->connected) {
        return;
    }
    ASSERT(callback, ov45_02256FA0, ov45_022569F8, 2123);
    if (channel == NULL) {
        channel = ov45_02256C2C;
    }
    if (*channel && ov45_02248238(chat, channel)) {
        enumUsers.callback = callback;
        enumUsers.param = param;
        ov45_022480EC(chat, channel, (ChatUserListFunc)ov45_022510D4, (int)&enumUsers);
        return;
    }
    ov45_02252F74(&chat->socket, ov45_02257050, channel);
    if (*channel == 0) {
        channel = NULL;
    }
    id = ov45_0224A978(chat, channel, callback, param);
    if (blocking) {
        WAIT_FOR_REQUEST(chat, id);
    }
}

// Sends a message of the given type (0 .. 4) to a user.
void ov45_02251350(ChatConnection *chat, const char *nick, const char *message, u32 type) {
    ASSERT_CHAT(chat, ov45_02256B0C, 2185);
    if (!chat || !chat->connected) {
        return;
    }
    ASSERT(nick, ov45_0225705C, ov45_02256B0C, 2188);
    ASSERT(*nick, ov45_0225706C, ov45_02256B0C, 2188);
    ASSERT(strlen(nick) < 128, ov45_0225707C, ov45_02256B0C, 2188);
    ASSERT(type <= 4, ov45_02256ED8, ov45_02256B0C, 2189);
    if (!message || !*message) {
        return;
    }
    if (type == 0) {
        ov45_02252F74(&chat->socket, ov45_02256F50, nick, message);
        return;
    }
    if (type == 1) {
        ov45_02252F74(&chat->socket, ov45_02256F60, nick, message);
        return;
    }
    if (type == 2) {
        ov45_02252F74(&chat->socket, ov45_02256F78, nick, message);
        return;
    }
    if (type == 3) {
        ov45_02252F74(&chat->socket, ov45_02256F88, nick, message);
        return;
    }
    if (type == 4) {
        ov45_02252F74(&chat->socket, ov45_02256F94, nick, message);
        return;
    }
}

// Next request cookie, "000" .. "999".
char *ov45_022514F4(void) {
    sprintf(ov45_022577F0, ov45_0225709C, ov45_022577F4++);
    ov45_022577F4 %= 1000;
    return ov45_022577F0;
}

// Sets keys of a channel (nick NULL or empty) or of a user in it.
void ov45_02251554(ChatConnection *chat, const char *channel, const char *nick, int count, const char **keys, const char **values) {
    char buffer[512];
    int i;
    const char *value;
    ASSERT_CHAT(chat, ov45_02256AB8, 2775);
    if (!chat || !chat->connected) {
        return;
    }
    if (!nick || !*nick) {
        sprintf(buffer, ov45_022570C4, channel);
    } else {
        sprintf(buffer, ov45_022570D4, channel, nick);
    }
    for (i = 0; i < count; i++) {
        value = values[i];
        if (!value) {
            value = ov45_02256C2C;
        }
        sprintf(buffer + strlen(buffer), ov45_02257094, keys[i], value);
    }
    ov45_02252CB4(&chat->socket, buffer);
}

// Sends the get keys request for a channel or a user ('\' in the keys becomes '/'); returns whether the
// broadcast keys were asked for.
BOOL ov45_02251650(ChatConnection *chat, const char *channel, const char *nick, const char *cookie, int count, const char **keys) {
    char buffer[512];
    BOOL broadcastKeys = FALSE;
    int i, length, total;
    const char *key;
    int j;
    char *p;
    ASSERT_CHAT(chat, ov45_02256A7C, 2841);
    ASSERT(channel != NULL && *channel != 0, ov45_022570E4, ov45_02256A7C, 2843);
    ASSERT(cookie != NULL && *cookie != 0, ov45_022570A4, ov45_02256A7C, 2844);
    ASSERT(count == 0 || keys != NULL, ov45_022570FC, ov45_02256A7C, 2845);
    if (nick == NULL || *nick == 0) {
        sprintf(buffer, ov45_0225710C, channel, cookie);
    } else {
        sprintf(buffer, ov45_02257124, channel, nick, cookie);
    }
    total = strlen(buffer);
    for (i = 0; i < count; i++) {
        key = keys[i];
        if (key == NULL || *key == 0) {
            continue;
        }
        if (strcmp(key, ov45_0225713C) == 0) {
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
            memcpy(buffer + total, &ov45_02257140.first, 3);
            *(ChatBytes2 *)(buffer + total + 3) = ov45_02257140.last;
        }
        total += 4;
    }
    if (count == 0 && (nick == NULL || *nick == 0)) {
        memcpy(buffer + total, &ov45_02256BC0, 2);
    }
    ov45_02252CB4(&chat->socket, buffer);
    return broadcastKeys;
}

// Gets keys of a channel (nick NULL or empty), of a user in it or of all its users (nick "*").
void ov45_022518F0(ChatConnection *chat, const char *channel, const char *nick, int count, const char **keys, void *callback, void *param, BOOL blocking) {
    char *cookie;
    BOOL broadcastKeys;
    int id;
    ASSERT_CHAT(chat, ov45_02256AA4, 2927);
    if (chat == NULL || !chat->connected) {
        return;
    }
    ASSERT(count >= 0, ov45_022570B8, ov45_02256AA4, 2930);
    ASSERT(count == 0 || keys != NULL, ov45_022570FC, ov45_02256AA4, 2931);
    cookie = ov45_022514F4();
    broadcastKeys = ov45_02251650(chat, channel, nick, cookie, count, keys);
    if (nick == NULL || *nick == 0) {
        id = ov45_0224ABA0(chat, cookie, count, keys, broadcastKeys, callback, param);
    } else {
        id = ov45_0224AA2C(chat, cookie, count, keys, strcmp(nick, ov45_02256BC0.bytes) == 0, broadcastKeys, callback, param);
    }
    if (blocking) {
        do {
            ov45_0224F770(chat, id);
            ov00_021FF3C0(10);
        } while (ov45_0224F660(chat, id) != 0);
    }
}
