#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

#include <stdarg.h>
#include <string.h>

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern ChatHostEntry *SOC_GetHostByName(const char *name);
extern int vsprintf(char *, const char *, va_list);
extern const char ov45_02257148[];
extern const char ov45_02257154[];
extern const char ov45_02257184[];
extern const char ov45_022571A4[];
extern const char ov45_022571B4[];
extern const char ov45_022571C4[];
extern const char ov45_022571D4[];
extern const char ov45_0225720C[];
extern const char ov45_02257220[];
extern const char ov45_02257234[];
extern const char ov45_02257248[];
extern const char ov45_02257258[];
extern const char ov45_02257334[];
extern const char ov45_02257344[];
extern const char ov45_022573C4[];
extern const char ov45_022573E4[];
extern const char ov45_02257408[];
extern const char ov45_02257438[];
extern const char ov45_0225745C[];
extern const char ov45_0225749C[];
extern const char ov45_022574BC[];
extern const char ov45_022574E0[];
extern const char ov45_02257514[];
extern const char ov45_02257538[];
extern const char ov45_02257578[];
extern const char ov45_02257590[];
extern const char ov45_0225759C[];
extern const char ov45_022575B0[];
extern const char ov45_022575D8[];
extern const char ov45_022575FC[];
extern const char ov45_02257600[];
extern const char ov45_02257610[];
extern const char ov45_02257620[];
extern const char ov45_02257624[];

// Line buffer of ciSocketSendf (4096 bytes).
extern char ov45_022577F8[];

// Assertion with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, ov45_02257258, func, line); \
    }
// A valid socket: state 0 .. 2 and both buffers consistent.
#define ASSERT_SOCKET(sock, func, line)                                            \
    ASSERT(sock, ov45_02257334, func, line);                                       \
    ASSERT((sock)->state <= 2, ov45_02257344, func, line);                         \
    ASSERT(&(sock)->input, ov45_022573C4, func, line);                             \
    ASSERT((sock)->input.size >= 0, ov45_022573E4, func, line);                    \
    ASSERT((sock)->input.size % 0x2000 == 0, ov45_02257408, func, line);           \
    ASSERT((sock)->input.length >= 0, ov45_02257438, func, line);                  \
    ASSERT((sock)->input.length <= (sock)->input.size, ov45_0225745C, func, line); \
    ASSERT(&(sock)->output, ov45_0225749C, func, line);                            \
    ASSERT((sock)->output.size >= 0, ov45_022574BC, func, line);                   \
    ASSERT((sock)->output.size % 0x2000 == 0, ov45_022574E0, func, line);          \
    ASSERT((sock)->output.length >= 0, ov45_02257514, func, line);                 \
    ASSERT((sock)->output.length <= (sock)->output.size, ov45_02257538, func, line);

// Frees a part of the message and clears it.
#define FREE_PART(part)      \
    if (part) {              \
        ov00_021EC3D8(part); \
        part = NULL;         \
    }

// Connects the socket to the server (a dotted address or a host name).
BOOL ciSocketConnect(ChatSocket *socket, const char *address, int port) {
    u32 ip;
    ChatSocketAddress addr;
    ASSERT_SOCKET(socket, ov45_022571D4, 216);
    ASSERT(address, ov45_02257578, ov45_022571D4, 217);
    ASSERT(port >= 0, ov45_02257590, ov45_022571D4, 218);
    ASSERT(port <= 65535, ov45_0225759C, ov45_022571D4, 219);
    ASSERT(socket->state == 0, ov45_022575B0, ov45_022571D4, 220);
    strncpy(socket->host, address, 255);
    socket->host[254] = 0;
    ip = ov00_021FFA44(address);
    if (ip == (u32)-1) {
        ChatHostEntry *entry = SOC_GetHostByName(address);
        if (!entry) {
            return FALSE;
        }
        ip = *entry->addresses[0];
    }
    memset(&addr, 0, sizeof(addr));
    addr.family = 2;
    addr.port = ((u16)port >> 8 & 0xFF) | ((u16)port << 8 & 0xFF00);
    addr.address = ip;
    socket->handle = ov00_021FF804(2, 1, 0);
    if (socket->handle == -1) {
        return FALSE;
    }
    if (ov00_021FF8A0(socket->handle, &addr, sizeof(addr)) != -1) {
        socket->state = 1;
        return TRUE;
    }
    ov00_021FF818(socket->handle);
    return FALSE;
}

// Closes the socket and frees its buffers and the last message.
void ciSocketDisconnect(ChatSocket *socket) {
    int i;
    ASSERT_SOCKET(socket, ov45_02257234, 283);
    if (socket->handle != -1) {
        ov00_021FF82C(socket->handle, 2);
        ov00_021FF818(socket->handle);
    }
    socket->state = 2;
    ov45_02251C40(&socket->input);
    ov45_02251C40(&socket->output);
    ov00_021EC3D8(socket->message.text);
    ov00_021EC3D8(socket->message.prefix);
    ov00_021EC3D8(socket->message.nick);
    ov00_021EC3D8(socket->message.user);
    ov00_021EC3D8(socket->message.host);
    ov00_021EC3D8(socket->message.command);
    ov00_021EC3D8(socket->message.middle);
    ov00_021EC3D8(socket->message.trailing);
    for (i = 0; i < socket->message.paramCount; i++) {
        ov00_021EC3D8(socket->message.params[i]);
    }
    ov00_021EC3D8(socket->message.params);
}

// Readiness of the socket handle (each output may be NULL).
void ciSocketSelect(int handle, BOOL *readable, BOOL *writable, BOOL *except) {
    int r = 0, w = 0, e = 0;
    ov00_021FF59C(handle, &r, &w, &e);
    if (readable) {
        *readable = r;
    }
    if (writable) {
        *writable = w;
    }
    if (except) {
        *except = e;
    }
}

// Sends as much of the output buffer as the socket takes (1024 bytes at a time).
void ciSocketThinkSend(ChatSocket *socket) {
    ASSERT_SOCKET(socket, ov45_0225720C, 340);
    ASSERT(socket->state == 1, ov45_022575D8, ov45_0225720C, 341);
    while (socket->output.length > 0) {
        BOOL writable;
        int sent;
        ciSocketSelect(socket->handle, NULL, &writable, NULL);
        if (!writable) {
            return;
        }
        sent = ov00_021FF970(socket->handle, socket->output.data, socket->output.length < 1024 ? socket->output.length : 1024, 0);
        if (sent != 0 && sent != -1) {
            ov45_02251DB4(&socket->output, sent);
        } else {
            return;
        }
    }
}

// Appends everything the socket has received to the input buffer (decrypted); state 2 when it was closed.
void ciSocketThinkRecv(ChatSocket *socket) {
    BOOL readable;
    int received, length;
    u8 *data;
    ASSERT_SOCKET(socket, ov45_02257220, 378);
    ASSERT(socket->state == 1, ov45_022575D8, ov45_02257220, 379);
    for (;;) {
        ciSocketSelect(socket->handle, &readable, NULL, NULL);
        if (!readable) {
            return;
        }
        if (!ov45_02251C50(&socket->input, 4096)) {
            return;
        }
        data = (u8 *)socket->input.data;
        length = socket->input.length;
        received = ov00_021FF92C(socket->handle, data + length, 4096, 0);
        if (received <= 0) {
            socket->state = 2;
            return;
        }
        if (socket->encrypted) {
            gs_crypt(data + length, received, &socket->inputCipher);
        }
        socket->input.length += received;
        socket->input.data[socket->input.length] = 0;
    }
}

// Sends and receives.
void ciSocketThink(ChatSocket *socket) {
    ASSERT_SOCKET(socket, ov45_022571B4, 446);
    ASSERT(socket->state == 1, ov45_022575D8, ov45_022571B4, 447);
    if (socket->state == 2) {
        return;
    }
    ciSocketThinkSend(socket);
    ciSocketThinkRecv(socket);
}

// Queues a line (CR LF appended, encrypted when the socket is).
BOOL ciSocketSend(ChatSocket *socket, const char *line) {
    int length;
    u8 *data;
    int start;
    ASSERT_SOCKET(socket, ov45_02257154, 472);
    ASSERT(socket->state == 1, ov45_022575D8, ov45_02257154, 473);
    ASSERT(line, ov45_02257248, ov45_02257154, 474);
    if (socket->state == 2) {
        return TRUE;
    }
    length = strlen(line);
    if (!ov45_02251C50(&socket->output, length + 2)) {
        return FALSE;
    }
    data = (u8 *)socket->output.data;
    start = socket->output.length;
    memcpy(data + start, line, length);
    socket->output.length += length;
    {
        int i = socket->output.length++;
        socket->output.data[i] = '\r';
    }
    {
        int i = socket->output.length++;
        socket->output.data[i] = '\n';
    }
    if (socket->encrypted) {
        gs_crypt(data + start, length + 2, &socket->outputCipher);
    }
    return TRUE;
}

// Queues a formatted line.
BOOL ciSocketSendf(ChatSocket *socket, const char *format, ...) {
    va_list args;
    int length;
    ASSERT_SOCKET(socket, ov45_022571A4, 531);
    ASSERT(socket->state == 1, ov45_022575D8, ov45_022571A4, 532);
    if (socket->state == 2) {
        return TRUE;
    }
    va_start(args, format);
    length = vsprintf(ov45_022577F8, format, args);
    if (length != -1) {
        ov45_022577F8[length] = 0;
    } else {
        ov45_022577F8[4095] = 0;
    }
    return ciSocketSend(socket, ov45_022577F8);
}

// Splits the prefix of a message into nick "!" user "@" host (heap copies in the message).
BOOL ciParseUser(char *prefix, ChatMessage *message) {
    char *user = NULL, *host = NULL, *p;
    int nickLength = 0, userLength = 0, hostLength = 0;
    if (!prefix || !*(volatile char *)prefix) {
        __msl_assertion_failed(ov45_022575FC, ov45_02257258, ov45_02257148, 562);
        return FALSE;
    }
    p = prefix;
    while (*p) {
        if (*(volatile char *)p != '!') {
            p++;
            nickLength++;
        } else {
            user = ++p;
            while (*p) {
                if (*p != '@') {
                    p++;
                    userLength++;
                } else {
                    host = ++p;
                    while (*p) {
                        p++;
                        hostLength++;
                    }
                }
            }
        }
    }
    if (nickLength) {
        message->nick = ov00_021EC3A8(nickLength + 1);
        if (message->nick) {
            memcpy(message->nick, prefix, nickLength);
            message->nick[nickLength] = 0;
        }
    } else {
        message->nick = NULL;
    }
    if (userLength) {
        message->user = ov00_021EC3A8(userLength + 1);
        if (message->user) {
            memcpy(message->user, user, userLength);
            message->user[userLength] = 0;
        }
    } else {
        message->user = NULL;
    }
    if (hostLength) {
        message->host = ov00_021EC3A8(hostLength + 1);
        if (message->host) {
            memcpy(message->host, host, hostLength);
            message->host[hostLength] = 0;
        }
    } else {
        message->host = NULL;
    }
    return TRUE;
}

// Appends a heap copy of param to the message's parameters.
BOOL ciAddParam(const char *param, ChatMessage *message) {
    char **params;
    char *copy;
    int i;
    params = ov00_021EC3BC(message->params, (message->paramCount + 1) * sizeof(char *));
    if (!params) {
        return FALSE;
    }
    message->params = params;
    copy = ov00_021EC3A8(strlen(param) + 1);
    if (!copy) {
        return FALSE;
    }
    strcpy(copy, param);
    i = message->paramCount++;
    message->params[i] = copy;
    return TRUE;
}

// Splits the parameters of a message: words separated by spaces, then the part after " :".
BOOL ciParseParam(const char *params, ChatMessage *message) {
    char *trailing;
    char *word;
    char *copy;
    ASSERT(params, ov45_02257600, ov45_02257184, 673);
    ASSERT(message, ov45_02257610, ov45_02257184, 674);
    copy = ov00_021EC3A8(strlen(params) + 1);
    if (!copy) {
        return FALSE;
    }
    strcpy(copy, params);
    if (copy[0] == ':') {
        copy[0] = 0;
        trailing = copy + 1;
    } else {
        trailing = strstr(copy, ov45_02257620);
        if (trailing) {
            *trailing = 0;
            trailing += 2;
        }
    }
    word = strtok(copy, ov45_02257624);
    if (word) {
        do {
            if (!ciAddParam(word, message)) {
                ov00_021EC3D8(copy);
                return FALSE;
            }
            word = strtok(NULL, ov45_02257624);
        } while (word);
    }
    if (trailing && !ciAddParam(trailing, message)) {
        ov00_021EC3D8(copy);
        return FALSE;
    }
    ov00_021EC3D8(copy);
    return TRUE;
}

// Parses one line into socket->message: [":" prefix] command [middle] [":" trailing].
BOOL ciParseMessage(ChatSocket *socket, char *line) {
    int prefixLength = 0, commandLength = 0, middleLength = 0, trailingLength = 0, length;
    char *start;
    if (!line || !*line) {
        __msl_assertion_failed(ov45_022575FC, ov45_02257258, ov45_022571C4, 740);
        return FALSE;
    }
    length = strlen(line);
    socket->message.text = ov00_021EC3A8(length + 1);
    if (!socket->message.text) {
        return FALSE;
    }
    memcpy(socket->message.text, line, length);
    socket->message.text[length] = 0;
    while (*line == '\n' || *line == '\r') {
        line++;
    }
    if (*line == ':') {
        line++;
        socket->message.prefix = line;
        if (*line) {
            while (*line != ' ' && *line) {
                prefixLength++;
                line++;
            }
        }
    }
    while (*line == ' ') {
        line++;
    }
    if (*line) {
        socket->message.command = line;
        while (*line != ' ' && *line) {
            commandLength++;
            line++;
        }
    }
    while (*line == ' ') {
        line++;
    }
    if (*line != ':' && *line) {
        socket->message.middle = line;
        while (*line != ' ' && *line) {
            middleLength++;
            line++;
        }
    }
    while (*line == ' ') {
        line++;
    }
    if (*line) {
        socket->message.trailing = line;
        while (*line) {
            line++;
            trailingLength++;
        }
    }
    if (prefixLength) {
        start = socket->message.prefix;
        socket->message.prefix = ov00_021EC3A8(prefixLength + 1);
        if (socket->message.prefix) {
            memcpy(socket->message.prefix, start, prefixLength);
            socket->message.prefix[prefixLength] = 0;
        }
        if (!ciParseUser(socket->message.prefix, &socket->message)) {
            FREE_PART(socket->message.text);
            FREE_PART(socket->message.prefix);
            return FALSE;
        }
    } else {
        socket->message.prefix = NULL;
        socket->message.nick = NULL;
        socket->message.user = NULL;
        socket->message.host = NULL;
    }
    if (middleLength) {
        if (!ciParseParam(socket->message.middle, &socket->message)) {
            FREE_PART(socket->message.text);
            FREE_PART(socket->message.prefix);
            FREE_PART(socket->message.nick);
            FREE_PART(socket->message.user);
            FREE_PART(socket->message.host);
            return FALSE;
        }
    } else if (trailingLength) {
        if (!ciParseParam(socket->message.trailing, &socket->message)) {
            FREE_PART(socket->message.text);
            FREE_PART(socket->message.prefix);
            FREE_PART(socket->message.nick);
            FREE_PART(socket->message.user);
            FREE_PART(socket->message.host);
            return FALSE;
        }
    } else {
        socket->message.params = NULL;
        socket->message.paramCount = 0;
    }
    if (trailingLength) {
        start = socket->message.trailing;
        socket->message.trailing = ov00_021EC3A8(trailingLength + 1);
        if (socket->message.trailing) {
            memcpy(socket->message.trailing, start, trailingLength);
            socket->message.trailing[trailingLength] = 0;
        }
    } else {
        socket->message.trailing = NULL;
    }
    if (commandLength) {
        start = socket->message.command;
        socket->message.command = ov00_021EC3A8(commandLength + 1);
        if (socket->message.command) {
            memcpy(socket->message.command, start, commandLength);
            socket->message.command[commandLength] = 0;
        }
    } else {
        socket->message.command = NULL;
    }
    if (middleLength) {
        start = socket->message.middle;
        socket->message.middle = ov00_021EC3A8(middleLength + 1);
        if (socket->message.middle) {
            memcpy(socket->message.middle, start, middleLength);
            socket->message.middle[middleLength] = 0;
        }
    } else {
        socket->message.middle = NULL;
    }
    return TRUE;
}
