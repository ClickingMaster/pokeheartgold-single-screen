#include "global.h"

#include "dwc/chat_internal.h"
#include "nitrowifi/socl_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#include <string.h>

extern const char gChatStr_ciParseUser[];
extern const char gChatStr_ciSocketSend[];
extern const char gChatStr_ciParseParam[];
extern const char gChatStr_ciSocketSendf[];
extern const char gChatStr_ciSocketThink[];
extern const char gChatStr_ciParseMessage[];
extern const char gChatStr_ciSocketConnect[];
extern const char gChatStr_ciSocketThinkSend[];
extern const char gChatStr_ciSocketThinkRecv[];
extern const char gChatStr_ciSocketDisconnect[];
extern const char gChatStr_ciBufferInit[];
extern const char gChatStr_ciSocketInit[];
extern const char gChatStr_ciBufferPreAppend[];
extern const char gChatStr_ciBufferClipFront[];
extern const char gChatStr_ciSocketRecv[];
extern const char gChatStr_BufferNeNULL_2[];
extern const char gChatStr_ChatSocketC[];
extern const char gChatStr_BufferNeNULL_3[];
extern const char gChatStr_BufferSizeGe0[];
extern const char gChatStr_BufferSizeBUFFER_INC_Eq0[];
extern const char gChatStr_BufferLengthGe0[];
extern const char gChatStr_BufferLengthLeBufferSize[];
extern const char gChatStr_LenGe0_2[];
extern const char gChatStr_LenLeSHRT_MAX[];
extern const char gChatStr_LenLeBufferLength[];
extern const char gChatStr_SockNeNULL[];
extern const char gChatStr_SockNeNULL_2[];
extern const char gChatStr_SockConnectStateEqCiNotConnectedOrSock[];
extern const char gChatStr_SockInputQueueNeNULL[];
extern const char gChatStr_SockInputQueueSizeGe0[];
extern const char gChatStr_SockInputQueueSizeBUFFER_INC_Eq0[];
extern const char gChatStr_SockInputQueueLengthGe0[];
extern const char gChatStr_SockInputQueueLengthLeSockInputQueueSize[];
extern const char gChatStr_SockOutputQueueNeNULL[];
extern const char gChatStr_SockOutputQueueSizeGe0[];
extern const char gChatStr_SockOutputQueueSizeBUFFER_INC_Eq0[];
extern const char gChatStr_SockOutputQueueLengthGe0[];
extern const char gChatStr_SockOutputQueueLengthLeSockOutputQueueSize[];
extern const char gChatStr_ServerAddressNeNULL_2[];
extern const char gChatStr_PortGe0[];
extern const char gChatStr_PortLeUSHRT_MAX[];
extern const char gChatStr_SockConnectStateEqCiNotConnected[];
extern const char gChatStr_SockConnectStateEqCiConnected[];
extern const char gChatStr_0_12[];
extern const char gChatStr_PTextNeNULL[];
extern const char gChatStr_MessageNeNULL_3[];
extern const char gChatStr_SpaceColon_2[];
extern const char gChatStr_Space_4[];

// Line buffer of ciSocketSendf (4096 bytes).
extern char ciSendfBuffer[];

#define ASSERT(cond, message, func, line)                                  \
    do {                                                                   \
        if (!(cond)) {                                                     \
            __msl_assertion_failed(message, gChatStr_ChatSocketC, func, line); \
        }                                                                  \
    } while (0)
#define ASSERT_BUFFER(buffer, func, line)                                                      \
    do {                                                                                       \
        ASSERT(buffer, gChatStr_BufferNeNULL_3, func, line);                                       \
        ASSERT((buffer)->size >= 0, gChatStr_BufferSizeGe0, func, line);                           \
        ASSERT((buffer)->size % 0x2000 == 0, gChatStr_BufferSizeBUFFER_INC_Eq0, func, line);       \
        ASSERT((buffer)->length >= 0, gChatStr_BufferLengthGe0, func, line);                       \
        ASSERT((buffer)->length <= (buffer)->size, gChatStr_BufferLengthLeBufferSize, func, line); \
    } while (0)
#define ASSERT_SOCKET(sock, func, line)                                                                                    \
    do {                                                                                                                   \
        ASSERT(sock, gChatStr_SockNeNULL_2, func, line);                                                                       \
        ASSERT((sock)->state <= ciDisconnected, gChatStr_SockConnectStateEqCiNotConnectedOrSock, func, line);                  \
        ASSERT(&(sock)->input, gChatStr_SockInputQueueNeNULL, func, line);                                                     \
        ASSERT((sock)->input.size >= 0, gChatStr_SockInputQueueSizeGe0, func, line);                                           \
        ASSERT((sock)->input.size % 0x2000 == 0, gChatStr_SockInputQueueSizeBUFFER_INC_Eq0, func, line);                       \
        ASSERT((sock)->input.length >= 0, gChatStr_SockInputQueueLengthGe0, func, line);                                       \
        ASSERT((sock)->input.length <= (sock)->input.size, gChatStr_SockInputQueueLengthLeSockInputQueueSize, func, line);     \
        ASSERT(&(sock)->output, gChatStr_SockOutputQueueNeNULL, func, line);                                                   \
        ASSERT((sock)->output.size >= 0, gChatStr_SockOutputQueueSizeGe0, func, line);                                         \
        ASSERT((sock)->output.size % 0x2000 == 0, gChatStr_SockOutputQueueSizeBUFFER_INC_Eq0, func, line);                     \
        ASSERT((sock)->output.length >= 0, gChatStr_SockOutputQueueLengthGe0, func, line);                                     \
        ASSERT((sock)->output.length <= (sock)->output.size, gChatStr_SockOutputQueueLengthLeSockOutputQueueSize, func, line); \
    } while (0)

// Frees a part of the message and clears it.
#define FREE_PART(part)        \
    do {                       \
        if (part) {            \
            DWCi_GsFree(part); \
            part = NULL;       \
        }                      \
    } while (0)

static void ciSocketSelect(int handle, BOOL *readable, BOOL *writable, BOOL *except);
static BOOL ciAddParam(const char *param, ChatMessage *message);
static BOOL ciSocketParseNextLine(ChatSocket *socket);

// Allocates an empty buffer of 0x2000 bytes.
BOOL ciBufferInit(ChatBuffer *buffer) {
    ASSERT(buffer, gChatStr_BufferNeNULL_2, gChatStr_ciBufferInit, 95);
    buffer->length = 0;
    buffer->size = 0x2000;
    buffer->data = DWCi_GsMalloc(0x2001);
    if (!buffer->data) {
        return FALSE;
    }
    buffer->data[0] = 0;
    return TRUE;
}

void ciBufferFree(ChatBuffer *buffer) {
    DWCi_GsFree(buffer->data);
}

// Makes room for length more bytes (the size grows in steps of 0x2000).
BOOL ciBufferPreAppend(ChatBuffer *buffer, int length) {
    int size;
    char *data;
    ASSERT_BUFFER(buffer, gChatStr_ciBufferPreAppend, 120);
    ASSERT(length >= 0, gChatStr_LenGe0_2, gChatStr_ciBufferPreAppend, 121);
    ASSERT(length <= 0x7FFF, gChatStr_LenLeSHRT_MAX, gChatStr_ciBufferPreAppend, 122);
    if (buffer->length + length <= buffer->size) {
        return TRUE;
    }
    size = buffer->length + length + 0x2000;
    size -= size % 0x2000;
    data = DWCi_GsRealloc(buffer->data, size + 1);
    if (!data) {
        return FALSE;
    }
    buffer->data = data;
    buffer->size = size;
    return TRUE;
}

// Removes the first length bytes.
void ciBufferClipFront(ChatBuffer *buffer, int length) {
    ASSERT_BUFFER(buffer, gChatStr_ciBufferClipFront, 151);
    ASSERT(length >= 0, gChatStr_LenGe0_2, gChatStr_ciBufferClipFront, 152);
    ASSERT(length <= buffer->length, gChatStr_LenLeBufferLength, gChatStr_ciBufferClipFront, 153);
    buffer->length -= length;
    memmove(buffer->data, buffer->data + length, buffer->length);
    buffer->data[buffer->length] = 0;
}

// Clears the socket and allocates its two buffers (name is not used).
BOOL ciSocketInit(ChatSocket *socket, const char *name) {
    ASSERT(socket, gChatStr_SockNeNULL, gChatStr_ciSocketInit, 177);
    memset(socket, 0, sizeof(ChatSocket));
    socket->handle = INVALID_SOCKET;
    if (!ciBufferInit(&socket->input)) {
        return FALSE;
    }
    if (ciBufferInit(&socket->output)) {
        return TRUE;
    }
    ciBufferFree(&socket->input);
    return FALSE;
}

// Connects the socket to the server (a dotted address or a host name).
BOOL ciSocketConnect(ChatSocket *socket, const char *address, int port) {
    u32 ip;
    ChatSocketAddress addr;
    ASSERT_SOCKET(socket, gChatStr_ciSocketConnect, 216);
    ASSERT(address, gChatStr_ServerAddressNeNULL_2, gChatStr_ciSocketConnect, 217);
    ASSERT(port >= 0, gChatStr_PortGe0, gChatStr_ciSocketConnect, 218);
    ASSERT(port <= 65535, gChatStr_PortLeUSHRT_MAX, gChatStr_ciSocketConnect, 219);
    ASSERT(socket->state == ciNotConnected, gChatStr_SockConnectStateEqCiNotConnected, gChatStr_ciSocketConnect, 220);
    strncpy(socket->host, address, 255);
    socket->host[254] = 0;
    ip = gsSocketInetAddr(address);
    if (ip == (u32)-1) {
        SocHostEnt *entry = SOC_GetHostByName(address);
        if (!entry) {
            return FALSE;
        }
        ip = *(u32 *)entry->addrList[0];
    }
    memset(&addr, 0, sizeof(addr));
    addr.family = AF_INET;
    addr.port = ((u16)port >> 8 & 0xFF) | ((u16)port << 8 & 0xFF00);
    addr.address = ip;
    socket->handle = gsSocketCreate(AF_INET, SOCK_STREAM, 0);
    if (socket->handle == INVALID_SOCKET) {
        return FALSE;
    }
    if (gsSocketConnect(socket->handle, &addr, sizeof(addr)) != SOCKET_ERROR) {
        socket->state = ciConnected;
        return TRUE;
    }
    gsSocketClose(socket->handle);
    return FALSE;
}

// Closes the socket and frees its buffers and the last message.
void ciSocketDisconnect(ChatSocket *socket) {
    int i;
    ASSERT_SOCKET(socket, gChatStr_ciSocketDisconnect, 283);
    if (socket->handle != INVALID_SOCKET) {
        gsSocketShutdown(socket->handle, 2);
        gsSocketClose(socket->handle);
    }
    socket->state = ciDisconnected;
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
static void ciSocketSelect(int handle, BOOL *readable, BOOL *writable, BOOL *except) {
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
    ASSERT_SOCKET(socket, gChatStr_ciSocketThinkSend, 340);
    ASSERT(socket->state == ciConnected, gChatStr_SockConnectStateEqCiConnected, gChatStr_ciSocketThinkSend, 341);
    while (socket->output.length > 0) {
        BOOL writable;
        int sent;
        ciSocketSelect(socket->handle, NULL, &writable, NULL);
        if (!writable) {
            return;
        }
        sent = gsSocketSend(socket->handle, socket->output.data, socket->output.length < 1024 ? socket->output.length : 1024, 0);
        if (sent != 0 && sent != SOCKET_ERROR) {
            ciBufferClipFront(&socket->output, sent);
        } else {
            return;
        }
    }
}

// Appends everything the socket has received to the input buffer (decrypted); ciDisconnected when it was closed.
void ciSocketThinkRecv(ChatSocket *socket) {
    BOOL readable;
    int received, length;
    u8 *data;
    ASSERT_SOCKET(socket, gChatStr_ciSocketThinkRecv, 378);
    ASSERT(socket->state == ciConnected, gChatStr_SockConnectStateEqCiConnected, gChatStr_ciSocketThinkRecv, 379);
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
            socket->state = ciDisconnected;
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
    ASSERT_SOCKET(socket, gChatStr_ciSocketThink, 446);
    ASSERT(socket->state == ciConnected, gChatStr_SockConnectStateEqCiConnected, gChatStr_ciSocketThink, 447);
    if (socket->state == ciDisconnected) {
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
    ASSERT_SOCKET(socket, gChatStr_ciSocketSend, 472);
    ASSERT(socket->state == ciConnected, gChatStr_SockConnectStateEqCiConnected, gChatStr_ciSocketSend, 473);
    ASSERT(line, gChatStr_BufferNeNULL_2, gChatStr_ciSocketSend, 474);
    if (socket->state == ciDisconnected) {
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
    ASSERT_SOCKET(socket, gChatStr_ciSocketSendf, 531);
    ASSERT(socket->state == ciConnected, gChatStr_SockConnectStateEqCiConnected, gChatStr_ciSocketSendf, 532);
    if (socket->state == ciDisconnected) {
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
        __msl_assertion_failed(gChatStr_0_12, gChatStr_ChatSocketC, gChatStr_ciParseUser, 562);
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
static BOOL ciAddParam(const char *param, ChatMessage *message) {
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
    ASSERT(params, gChatStr_PTextNeNULL, gChatStr_ciParseParam, 673);
    ASSERT(message, gChatStr_MessageNeNULL_3, gChatStr_ciParseParam, 674);
    copy = DWCi_GsMalloc(strlen(params) + 1);
    if (!copy) {
        return FALSE;
    }
    strcpy(copy, params);
    if (copy[0] == ':') {
        copy[0] = 0;
        trailing = copy + 1;
    } else {
        trailing = strstr(copy, gChatStr_SpaceColon_2);
        if (trailing) {
            *trailing = 0;
            trailing += 2;
        }
    }
    word = strtok(copy, gChatStr_Space_4);
    if (word) {
        do {
            if (!ciAddParam(word, message)) {
                DWCi_GsFree(copy);
                return FALSE;
            }
            word = strtok(NULL, gChatStr_Space_4);
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
        __msl_assertion_failed(gChatStr_0_12, gChatStr_ChatSocketC, gChatStr_ciParseMessage, 740);
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

// Parses the first complete line of the input buffer into socket->message and removes it from the buffer.
static BOOL ciSocketParseNextLine(ChatSocket *socket) {
    char *line = socket->input.data, *end, *last;
    char saved;
    int i;
    if (*line) {
        while (*line == '\r' || *line == '\n') {
            line++;
        }
        if (*line) {
            end = line;
            last = line;
            while (*end != '\n' && *end != '\r' && *end) {
                if (*end != ' ') {
                    last = end;
                }
                end++;
            }
            if (*end) {
                saved = last[1];
                last[1] = 0;
                FREE_PART(socket->message.text);
                FREE_PART(socket->message.prefix);
                FREE_PART(socket->message.nick);
                FREE_PART(socket->message.user);
                FREE_PART(socket->message.host);
                FREE_PART(socket->message.command);
                FREE_PART(socket->message.middle);
                FREE_PART(socket->message.trailing);
                for (i = 0; i < socket->message.paramCount; i++) {
                    FREE_PART(socket->message.params[i]);
                }
                FREE_PART(socket->message.params);
                socket->message.paramCount = 0;
                memset(&socket->message, 0, sizeof(ChatMessage));
                if (!ciParseMessage(socket, line)) {
                    memset(&socket->message, 0, sizeof(ChatMessage));
                    return FALSE;
                }
                last[1] = saved;
                ciBufferClipFront(&socket->input, end - socket->input.data);
                return TRUE;
            }
        }
    }
    return FALSE;
}

// Returns the next line received from the server, or NULL.
ChatMessage *ciSocketRecv(ChatSocket *socket) {
    ASSERT_SOCKET(socket, gChatStr_ciSocketRecv, 1001);
    if (!socket->input.length) {
        return NULL;
    }
    if (ciSocketParseNextLine(socket)) {
        return &socket->message;
    }
    return NULL;
}
