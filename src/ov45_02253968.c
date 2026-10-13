#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"
#include "dwc/stats_upload_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

#include <string.h>

// The message reading functions of the chat socket module (assertions name "chatSocket.c") and the key setup and
// random number generator of the stats upload client (stats_upload.c), which share this object.

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern const char ov45_02257164[];
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

// Assertion of the socket module with the message, the name of the checking function and the line.
#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, ov45_02257258, func, line); \
    }

// Frees a part of the message and clears it.
#define FREE_PART(part)      \
    if (part) {              \
        DWCi_GsFree(part); \
        part = NULL;         \
    }

// Parses the first complete line of the input buffer into socket->message and removes it from the buffer.
BOOL ov45_02253968(ChatSocket *socket) {
    char *line = socket->input.data, *end, *last;
    char saved;
    int i;
    if (!*line) {
        goto fail;
    }
    while (*line == '\r' || *line == '\n') {
        line++;
    }
    if (!*line) {
        goto fail;
    }
    end = line;
    last = line;
    while (*end != '\n' && *end != '\r' && *end) {
        if (*end != ' ') {
            last = end;
        }
        end++;
    }
    if (!*end) {
        goto fail;
    }
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
fail:
    return FALSE;
}

// Returns the next line received from the server, or NULL.
ChatMessage *ciSocketRecv(ChatSocket *socket) {
    ASSERT(socket, ov45_02257334, ov45_02257164, 1001);
    ASSERT(socket->state <= 2, ov45_02257344, ov45_02257164, 1001);
    ASSERT(&socket->input, ov45_022573C4, ov45_02257164, 1001);
    ASSERT(socket->input.size >= 0, ov45_022573E4, ov45_02257164, 1001);
    ASSERT(socket->input.size % 0x2000 == 0, ov45_02257408, ov45_02257164, 1001);
    ASSERT(socket->input.length >= 0, ov45_02257438, ov45_02257164, 1001);
    ASSERT(socket->input.length <= socket->input.size, ov45_0225745C, ov45_02257164, 1001);
    ASSERT(&socket->output, ov45_0225749C, ov45_02257164, 1001);
    ASSERT(socket->output.size >= 0, ov45_022574BC, ov45_02257164, 1001);
    ASSERT(socket->output.size % 0x2000 == 0, ov45_022574E0, ov45_02257164, 1001);
    ASSERT(socket->output.length >= 0, ov45_02257514, ov45_02257164, 1001);
    ASSERT(socket->output.length <= socket->output.size, ov45_02257538, ov45_02257164, 1001);
    if (!socket->input.length) {
        return NULL;
    }
    if (ov45_02253968(socket)) {
        return &socket->message;
    }
    return NULL;
}

// Length of the base64 encoding of length bytes.
u32 StatsUpload_GetBase64Length(u32 length) {
    return 4 * (length / 3 + (length % 3 != 0));
}

void StatsUpload_SeedRandom(u32 seed) {
    ov45_022587F8 = seed | (seed << 16);
}

// Next byte of the random number generator.
int StatsUpload_RandomByte(void) {
    ov45_022587F8 = (gStatsUpload.multiplier * ov45_022587F8 + gStatsUpload.increment) % gStatsUpload.modulus;
    return ((int)ov45_022587F8 >> 16) & 0xFF;
}

// Sets the game name (< 32 characters), the 20 character secret key and the random number generator constants
// (multiplier 8n + 5, odd increment, power of two modulus) used by the uploads.
BOOL StatsUpload_SetKey(char *gameName, char *secretKey, u32 multiplier, u32 increment, u32 modulus, u32 checksumKey) {
    int i, bits;
    u32 count;
    u8 *dst;
    gStatsUpload.keyReady = FALSE;
    if (strlen(gameName) >= 32) {
        return FALSE;
    }
    if (strlen(secretKey) != 20) {
        return FALSE;
    }
    if ((multiplier - 5) & 7) {
        return FALSE;
    }
    if (!(increment & 1)) {
        return FALSE;
    }
    bits = 0;
    for (i = 0; i < 32; i++) {
        if (((modulus >> i) & 1) == 1) {
            bits++;
        }
    }
    if (bits != 1) {
        return FALSE;
    }
    strncpy(gStatsUploadGameName, gameName, 32);
    count = 10;
    dst = gStatsUploadSecretKey;
    {
        do {
            u8 x = ((u8 *)secretKey)[0], y = ((u8 *)secretKey)[1];
            secretKey += 2;
            dst[0] = x;
            dst[1] = y;
            dst += 2;
        } while (--count != 0);
    }
    gStatsUpload.multiplier = multiplier;
    gStatsUpload.increment = increment;
    gStatsUpload.modulus = modulus;
    gStatsUpload.checksumKey = checksumKey;
    gStatsUpload.keyReady = TRUE;
    return TRUE;
}
