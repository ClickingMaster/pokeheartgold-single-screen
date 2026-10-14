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
extern const char sStr_ciParseUser[];
extern const char sStr_ciSocketSend[];
extern const char sStr_ciParseParam[];
extern const char sStr_ciSocketSendf[];
extern const char sStr_ciSocketThink[];
extern const char sStr_ciParseMessage[];
extern const char sStr_ciSocketConnect[];
extern const char sStr_ciSocketThinkSend[];
extern const char sStr_ciSocketThinkRecv[];
extern const char sStr_ciSocketDisconnect[];
extern const char sStr_BufferNeNULL_2[];
extern const char sStr_ChatSocketC[];
extern const char sStr_SockNeNULL_2[];
extern const char sStr_SockConnectStateEqCiNotConnectedOrSock[];
extern const char sStr_SockInputQueueNeNULL[];
extern const char sStr_SockInputQueueSizeGe0[];
extern const char sStr_SockInputQueueSizeBUFFER_INC_Eq0[];
extern const char sStr_SockInputQueueLengthGe0[];
extern const char sStr_SockInputQueueLengthLeSockInputQueueSize[];
extern const char sStr_SockOutputQueueNeNULL[];
extern const char sStr_SockOutputQueueSizeGe0[];
extern const char sStr_SockOutputQueueSizeBUFFER_INC_Eq0[];
extern const char sStr_SockOutputQueueLengthGe0[];
extern const char sStr_SockOutputQueueLengthLeSockOutputQueueSize[];
extern const char sStr_ServerAddressNeNULL_2[];
extern const char sStr_PortGe0[];
extern const char sStr_PortLeUSHRT_MAX[];
extern const char sStr_SockConnectStateEqCiNotConnected[];
extern const char sStr_SockConnectStateEqCiConnected[];
extern const char sStr_0_12[];
extern const char sStr_PTextNeNULL[];
extern const char sStr_MessageNeNULL_3[];
extern const char sStr_SpaceColon_2[];
extern const char sStr_Space_4[];

// Line buffer of ciSocketSendf (4096 bytes).
extern char ciSendfBuffer[];

// Assertion with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, sStr_ChatSocketC, func, line); \
    }
// A valid socket: state 0 .. 2 and both buffers consistent.
#define ASSERT_SOCKET(sock, func, line)                                            \
    ASSERT(sock, sStr_SockNeNULL_2, func, line);                                       \
    ASSERT((sock)->state <= 2, sStr_SockConnectStateEqCiNotConnectedOrSock, func, line);                         \
    ASSERT(&(sock)->input, sStr_SockInputQueueNeNULL, func, line);                             \
    ASSERT((sock)->input.size >= 0, sStr_SockInputQueueSizeGe0, func, line);                    \
    ASSERT((sock)->input.size % 0x2000 == 0, sStr_SockInputQueueSizeBUFFER_INC_Eq0, func, line);           \
    ASSERT((sock)->input.length >= 0, sStr_SockInputQueueLengthGe0, func, line);                  \
    ASSERT((sock)->input.length <= (sock)->input.size, sStr_SockInputQueueLengthLeSockInputQueueSize, func, line); \
    ASSERT(&(sock)->output, sStr_SockOutputQueueNeNULL, func, line);                            \
    ASSERT((sock)->output.size >= 0, sStr_SockOutputQueueSizeGe0, func, line);                   \
    ASSERT((sock)->output.size % 0x2000 == 0, sStr_SockOutputQueueSizeBUFFER_INC_Eq0, func, line);          \
    ASSERT((sock)->output.length >= 0, sStr_SockOutputQueueLengthGe0, func, line);                 \
    ASSERT((sock)->output.length <= (sock)->output.size, sStr_SockOutputQueueLengthLeSockOutputQueueSize, func, line);

// Frees a part of the message and clears it.
#define FREE_PART(part)      \
    if (part) {              \
        DWCi_GsFree(part); \
        part = NULL;         \
    }

// Connects the socket to the server (a dotted address or a host name).
BOOL ciSocketConnect(ChatSocket *socket, const char *address, int port) {
    u32 ip;
    ChatSocketAddress addr;
    ASSERT_SOCKET(socket, sStr_ciSocketConnect, 216);
    ASSERT(address, sStr_ServerAddressNeNULL_2, sStr_ciSocketConnect, 217);
    ASSERT(port >= 0, sStr_PortGe0, sStr_ciSocketConnect, 218);
    ASSERT(port <= 65535, sStr_PortLeUSHRT_MAX, sStr_ciSocketConnect, 219);
    ASSERT(socket->state == 0, sStr_SockConnectStateEqCiNotConnected, sStr_ciSocketConnect, 220);
    strncpy(socket->host, address, 255);
    socket->host[254] = 0;
    ip = gsSocketInetAddr(address);
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
    socket->handle = gsSocketCreate(2, 1, 0);
    if (socket->handle == -1) {
        return FALSE;
    }
    if (gsSocketConnect(socket->handle, &addr, sizeof(addr)) != -1) {
        socket->state = 1;
        return TRUE;
    }
    gsSocketClose(socket->handle);
    return FALSE;
}

// Closes the socket and frees its buffers and the last message.
void ciSocketDisconnect(ChatSocket *socket) {
    int i;
    ASSERT_SOCKET(socket, sStr_ciSocketDisconnect, 283);
    if (socket->handle != -1) {
        gsSocketShutdown(socket->handle, 2);
        gsSocketClose(socket->handle);
    }
    socket->state = 2;
    ciBufferFree(&socket->input);
    ciBufferFree(&socket->output);
    DWCi_GsFree(socket->message.text);
    DWCi_GsFree(socket->message.prefix);
    DWCi_GsFree(socket->message.nick);
    DWCi_GsFree(socket->message.user);
    DWCi_GsFree(socket->message.host);
    DWCi_GsFree(socket->message.command);
    DWCi_GsFree(socket->message.middle);
    DWCi_GsFree(socket->message.trailing);
    for (i = 0; i < socket->message.paramCount; i++) {
        DWCi_GsFree(socket->message.params[i]);
    }
    DWCi_GsFree(socket->message.params);
}

// Readiness of the socket handle (each output may be NULL).
void ciSocketSelect(int handle, BOOL *readable, BOOL *writable, BOOL *except) {
    int r = 0, w = 0, e = 0;
    GSISocketSelect(handle, &r, &w, &e);
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
    ASSERT_SOCKET(socket, sStr_ciSocketThinkSend, 340);
    ASSERT(socket->state == 1, sStr_SockConnectStateEqCiConnected, sStr_ciSocketThinkSend, 341);
    while (socket->output.length > 0) {
        BOOL writable;
        int sent;
        ciSocketSelect(socket->handle, NULL, &writable, NULL);
        if (!writable) {
            return;
        }
        sent = gsSocketSend(socket->handle, socket->output.data, socket->output.length < 1024 ? socket->output.length : 1024, 0);
        if (sent != 0 && sent != -1) {
            ciBufferClipFront(&socket->output, sent);
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
    ASSERT_SOCKET(socket, sStr_ciSocketThinkRecv, 378);
    ASSERT(socket->state == 1, sStr_SockConnectStateEqCiConnected, sStr_ciSocketThinkRecv, 379);
    for (;;) {
        ciSocketSelect(socket->handle, &readable, NULL, NULL);
        if (!readable) {
            return;
        }
        if (!ciBufferPreAppend(&socket->input, 4096)) {
            return;
        }
        data = (u8 *)socket->input.data;
        length = socket->input.length;
        received = gsSocketRecv(socket->handle, data + length, 4096, 0);
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
    ASSERT_SOCKET(socket, sStr_ciSocketThink, 446);
    ASSERT(socket->state == 1, sStr_SockConnectStateEqCiConnected, sStr_ciSocketThink, 447);
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
    ASSERT_SOCKET(socket, sStr_ciSocketSend, 472);
    ASSERT(socket->state == 1, sStr_SockConnectStateEqCiConnected, sStr_ciSocketSend, 473);
    ASSERT(line, sStr_BufferNeNULL_2, sStr_ciSocketSend, 474);
    if (socket->state == 2) {
        return TRUE;
    }
    length = strlen(line);
    if (!ciBufferPreAppend(&socket->output, length + 2)) {
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
    ASSERT_SOCKET(socket, sStr_ciSocketSendf, 531);
    ASSERT(socket->state == 1, sStr_SockConnectStateEqCiConnected, sStr_ciSocketSendf, 532);
    if (socket->state == 2) {
        return TRUE;
    }
    va_start(args, format);
    length = vsprintf(ciSendfBuffer, format, args);
    if (length != -1) {
        ciSendfBuffer[length] = 0;
    } else {
        ciSendfBuffer[4095] = 0;
    }
    return ciSocketSend(socket, ciSendfBuffer);
}

// Splits the prefix of a message into nick "!" user "@" host (heap copies in the message).
BOOL ciParseUser(char *prefix, ChatMessage *message) {
    char *user = NULL, *host = NULL, *p;
    int nickLength = 0, userLength = 0, hostLength = 0;
    if (!prefix || !*(volatile char *)prefix) {
        __msl_assertion_failed(sStr_0_12, sStr_ChatSocketC, sStr_ciParseUser, 562);
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
        message->nick = DWCi_GsMalloc(nickLength + 1);
        if (message->nick) {
            memcpy(message->nick, prefix, nickLength);
            message->nick[nickLength] = 0;
        }
    } else {
        message->nick = NULL;
    }
    if (userLength) {
        message->user = DWCi_GsMalloc(userLength + 1);
        if (message->user) {
            memcpy(message->user, user, userLength);
            message->user[userLength] = 0;
        }
    } else {
        message->user = NULL;
    }
    if (hostLength) {
        message->host = DWCi_GsMalloc(hostLength + 1);
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
    params = DWCi_GsRealloc(message->params, (message->paramCount + 1) * sizeof(char *));
    if (!params) {
        return FALSE;
    }
    message->params = params;
    copy = DWCi_GsMalloc(strlen(param) + 1);
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
    ASSERT(params, sStr_PTextNeNULL, sStr_ciParseParam, 673);
    ASSERT(message, sStr_MessageNeNULL_3, sStr_ciParseParam, 674);
    copy = DWCi_GsMalloc(strlen(params) + 1);
    if (!copy) {
        return FALSE;
    }
    strcpy(copy, params);
    if (copy[0] == ':') {
        copy[0] = 0;
        trailing = copy + 1;
    } else {
        trailing = strstr(copy, sStr_SpaceColon_2);
        if (trailing) {
            *trailing = 0;
            trailing += 2;
        }
    }
    word = strtok(copy, sStr_Space_4);
    if (word) {
        do {
            if (!ciAddParam(word, message)) {
                DWCi_GsFree(copy);
                return FALSE;
            }
            word = strtok(NULL, sStr_Space_4);
        } while (word);
    }
    if (trailing && !ciAddParam(trailing, message)) {
        DWCi_GsFree(copy);
        return FALSE;
    }
    DWCi_GsFree(copy);
    return TRUE;
}

// Parses one line into socket->message: [":" prefix] command [middle] [":" trailing].
BOOL ciParseMessage(ChatSocket *socket, char *line) {
    int prefixLength = 0, commandLength = 0, middleLength = 0, trailingLength = 0, length;
    char *start;
    if (!line || !*line) {
        __msl_assertion_failed(sStr_0_12, sStr_ChatSocketC, sStr_ciParseMessage, 740);
        return FALSE;
    }
    length = strlen(line);
    socket->message.text = DWCi_GsMalloc(length + 1);
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
        socket->message.prefix = DWCi_GsMalloc(prefixLength + 1);
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
        socket->message.trailing = DWCi_GsMalloc(trailingLength + 1);
        if (socket->message.trailing) {
            memcpy(socket->message.trailing, start, trailingLength);
            socket->message.trailing[trailingLength] = 0;
        }
    } else {
        socket->message.trailing = NULL;
    }
    if (commandLength) {
        start = socket->message.command;
        socket->message.command = DWCi_GsMalloc(commandLength + 1);
        if (socket->message.command) {
            memcpy(socket->message.command, start, commandLength);
            socket->message.command[commandLength] = 0;
        }
    } else {
        socket->message.command = NULL;
    }
    if (middleLength) {
        start = socket->message.middle;
        socket->message.middle = DWCi_GsMalloc(middleLength + 1);
        if (socket->message.middle) {
            memcpy(socket->message.middle, start, middleLength);
            socket->message.middle[middleLength] = 0;
        }
    } else {
        socket->message.middle = NULL;
    }
    return TRUE;
}
