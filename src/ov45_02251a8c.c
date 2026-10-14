#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

#include <string.h>

// The last two functions of the chat main module (assertions name "chatMain.c") and the line buffer functions of
// the socket module ("chatSocket.c"), which share this object.

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern char *strchr(const char *, int);
extern const char sStr_ciNickError[];
extern const char sStr_ChatMainC[];
extern const char sStr_ChatNeNULL_4[];
extern const char sStr_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv_3[];
extern const char sStr_ciBufferInit[];
extern const char sStr_ciSocketInit[];
extern const char sStr_ciBufferPreAppend[];
extern const char sStr_ciBufferClipFront[];
extern const char sStr_BufferNeNULL_2[];
extern const char sStr_ChatSocketC[];
extern const char sStr_BufferNeNULL_3[];
extern const char sStr_BufferSizeGe0[];
extern const char sStr_BufferSizeBUFFER_INC_Eq0[];
extern const char sStr_BufferLengthGe0[];
extern const char sStr_BufferLengthLeBufferSize[];
extern const char sStr_LenGe0_2[];
extern const char sStr_LenLeSHRT_MAX[];
extern const char sStr_LenLeBufferLength[];
extern const char sStr_SockNeNULL[];

// Assertion of the socket module with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, sStr_ChatSocketC, func, line); \
    }
// A valid buffer: size a multiple of 0x2000, 0 <= length <= size.
#define ASSERT_BUFFER(buffer, func, line)                            \
    ASSERT(buffer, sStr_BufferNeNULL_3, func, line);                       \
    ASSERT((buffer)->size >= 0, sStr_BufferSizeGe0, func, line);          \
    ASSERT((buffer)->size % 0x2000 == 0, sStr_BufferSizeBUFFER_INC_Eq0, func, line); \
    ASSERT((buffer)->length >= 0, sStr_BufferLengthGe0, func, line);        \
    ASSERT((buffer)->length <= (buffer)->size, sStr_BufferLengthLeBufferSize, func, line);

// Checks a nick: 0 = valid, 6 = too long, 2 = empty, bad first character or a character outside sStr_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv_3.
int ciNickIsValid(const char *nick) {
    if (strlen(nick) >= 21) {
        return 6;
    }
    if (!nick || !*nick) {
        return 2;
    }
    if (*nick == '@' || *nick == '#' || *nick == '+' || *nick == ':') {
        return 2;
    }
    while (*nick) {
        if (!strchr(sStr_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv_3, *nick++)) {
            return 2;
        }
    }
    return 0;
}

// The nick was refused: queue the nick error callback, or end the connect attempt without one.
void ciNickError(ChatConnection *chat, int type, const char *nick, int unk_0C, int unk_10) {
    if (!chat) {
        __msl_assertion_failed(sStr_ChatNeNULL_4, sStr_ChatMainC, sStr_ciNickError, 3017);
    }
    if (chat->nickErrorCallback) {
        ChatNickErrorArgs args = { 0 };
        args.unk_0C = unk_10;
        args.type = type;
        args.nick = nick;
        args.unk_08 = unk_0C;
        ciAddCallback_(chat, 25, chat->nickErrorCallback, (u32 *)&args, chat->connectParam, 0, NULL, sizeof(args));
        return;
    }
    chat->connecting = FALSE;
    if (chat->connectedCallback) {
        ((ChatConnectedFunc)chat->connectedCallback)(chat, FALSE, 1, chat->connectParam);
    }
}

// Allocates an empty buffer of 0x2000 bytes.
BOOL ciBufferInit(ChatBuffer *buffer) {
    ASSERT(buffer, sStr_BufferNeNULL_2, sStr_ciBufferInit, 95);
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
    ASSERT_BUFFER(buffer, sStr_ciBufferPreAppend, 120);
    ASSERT(length >= 0, sStr_LenGe0_2, sStr_ciBufferPreAppend, 121);
    ASSERT(length <= 0x7FFF, sStr_LenLeSHRT_MAX, sStr_ciBufferPreAppend, 122);
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
    ASSERT_BUFFER(buffer, sStr_ciBufferClipFront, 151);
    ASSERT(length >= 0, sStr_LenGe0_2, sStr_ciBufferClipFront, 152);
    ASSERT(length <= buffer->length, sStr_LenLeBufferLength, sStr_ciBufferClipFront, 153);
    buffer->length -= length;
    memmove(buffer->data, buffer->data + length, buffer->length);
    buffer->data[buffer->length] = 0;
}

// Clears the socket and allocates its two buffers (name is not used).
BOOL ciSocketInit(ChatSocket *socket, const char *name) {
    ASSERT(socket, sStr_SockNeNULL, sStr_ciSocketInit, 177);
    memset(socket, 0, sizeof(ChatSocket));
    socket->handle = -1;
    if (!ciBufferInit(&socket->input)) {
        return FALSE;
    }
    if (ciBufferInit(&socket->output)) {
        return TRUE;
    }
    ciBufferFree(&socket->input);
    return FALSE;
}
