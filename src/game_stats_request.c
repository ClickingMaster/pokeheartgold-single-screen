#include "game_stats_request.h"

#include <string.h>

#include "global.h"

typedef struct HexDigits {
    char digits[17];
} HexDigits;

typedef struct HashSalt {
    char chars[21];
} HashSalt;

extern const HexDigits gGameStatsHexDigits;
extern HashSalt gGameStatsHashSalt;
extern char gGameStatsHashParam[];
extern char gGameStatsDataParam[];
extern char gGameStatsErrorChecksum[];
extern char gGameStatsErrorPid[];
extern char gGameStatsErrorDataLength[];
extern char gGameStatsErrorTokenNotFound[];
extern char gGameStatsErrorTokenExpired[];
extern char gGameStatsErrorIncorrectHash[];
extern char gGameStatsUrlFormat[];

extern int strncmp(const char *a, const char *b, u32 n);
extern int sprintf(char *dest, const char *format, ...);

void *ov00_021EC2A8(int category, u32 size);
void ov00_021EC2EC(int category, void *ptr, u32 size);
void ov00_021FFC18(const void *src, char *dst, int len, int mode);
BOOL ov00_021FA0B4(int value);

void GameStats_SeedRandom(u32 seed) {
    gGameStatsRandState = seed | (seed << 16);
}

u8 GameStats_NextRandom(void) {
    gGameStatsRandState = (gGameStatsRandState * 0x45 + 0x1111) & 0x7FFFFFFF;
    return (s32)gGameStatsRandState >> 16;
}

// force_active matches the section flags of the original objects of these functions.
#pragma force_active on

int GameStats_EncodePayload(u32 key, const u8 *data, int len, char *out, int outSize) {
    u32 sum = 0;
    int total = len + 8;
    int b1;
    int b2;
    int b3;
    int b0;
    u8 *buf;
    int i;
    u32 x;

    if (outSize < GameStats_GetBase64Length(total) + 1) {
        return 2;
    }
    buf = ov00_021EC2A8(10, total);
    if (buf == NULL) {
        return 1;
    }
    b3 = (u8)(key >> 24);
    sum += (u8)b3;
    b2 = (u8)(key >> 16);
    sum += (u8)b2;
    b1 = (u8)(key >> 8);
    sum += (u8)b1;
    b0 = (u8)key;
    sum += (u8)b0;
    for (i = 0; i < len; i++) {
        sum += data[i];
    }
    GameStats_SeedRandom(sum);
    buf[4] = GameStats_NextRandom() ^ b0;
    buf[5] = GameStats_NextRandom() ^ b1;
    buf[6] = GameStats_NextRandom() ^ b2;
    buf[7] = GameStats_NextRandom() ^ b3;
    for (i = 0; i < len; i++) {
        buf[8 + i] = data[i] ^ GameStats_NextRandom();
    }
    x = sum ^ 0x4A3B2C1D;
    buf[0] = x >> 24;
    buf[1] = x >> 16;
    buf[2] = x >> 8;
    buf[3] = x;
    ov00_021FFC18(buf, out, len + 8, 2);
    out[GameStats_GetBase64Length(total)] = 0;
    ov00_021EC2EC(10, buf, 0);
    return 0;
}

#pragma force_active reset

// Callers pass a signed length (kept: an unsigned parameter changes their register allocation).
int GameStats_GetBase64Length(int size) {
    return ((u32)size / 3 + ((u32)size % 3 != 0)) * 4;
}

void GameStats_SetRequestError(int result) {
    switch (result) {
    case -8:
        gGameStatsRequest.error = 0;
        break;
    case -6:
        gGameStatsRequest.error = 1;
        break;
    case -5:
        gGameStatsRequest.error = 2;
        break;
    case -4:
        gGameStatsRequest.error = 3;
        break;
    case -3:
        gGameStatsRequest.error = 4;
        break;
    case -2:
        gGameStatsRequest.error = 5;
        break;
    case -1:
        gGameStatsRequest.error = 6;
        break;
    default:
        gGameStatsRequest.error = 0;
        break;
    }
}

#pragma force_active on

void GameStats_ResponseCallback(const char *response, int length, int error) {
    char text[54];
    HexDigits hexDigits;
    char *digest;
    int i;
    int j;
    int result;

    gGameStatsRequest.requestId = -1;
    if (gGameStatsRequest.state == GAME_STATS_STATE_IDLE) {
        return;
    }
    if (error == 0) {
        switch (gGameStatsRequest.state) {
        case GAME_STATS_STATE_WAIT_CHALLENGE:
            if (length == 32) {
                // The 20-byte SHA1 digest is computed in the second half of the hash text and
                // expanded to 40 hex digits in place.
                digest = gGameStatsRequest.hashText + 20;
                hexDigits = gGameStatsHexDigits;
                {
                    const u8 *src = (const u8 *)gGameStatsHashSalt.chars;
                    u8 *dst = (u8 *)text;
                    u32 count = 21;
                    do {
                        *dst++ = *src++;
                    } while (--count);
                }
                strncat(text, response, length);
                MATH_CalcSHA1(digest, text, strlen(text));
                strcat(gGameStatsRequest.url, gGameStatsHashParam);
                for (i = 0, j = 0; i < 20; i++, j += 2) {
                    gGameStatsRequest.hashText[j] = hexDigits.digits[*(u8 *)((u32)digest + i) >> 4];
                    gGameStatsRequest.hashText[j + 1] = hexDigits.digits[*(u8 *)((u32)digest + i) & 15];
                }
                gGameStatsRequest.hashText[40] = 0;
                strcat(gGameStatsRequest.url, gGameStatsDataParam);
                result = GameStats_EncodePayload(gGameStatsRequest.pid, gGameStatsRequest.payload, gGameStatsRequest.payloadSize, gGameStatsRequest.dataText, gGameStatsRequest.dataTextSize);
                switch (result) {
                case 0:
                    break;
                case 1:
                case 2:
                    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                    return;
                }
                gGameStatsRequest.state = GAME_STATS_STATE_SEND_DATA;
                return;
            }
            gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
            return;
        case GAME_STATS_STATE_WAIT_RESPONSE:
            if (length == 22) {
                if (strncmp(response, gGameStatsErrorChecksum, 22) == 0) {
                    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                    gGameStatsRequest.error = 26;
                    return;
                }
                if (strncmp(response, gGameStatsErrorPid, 22) == 0) {
                    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                    gGameStatsRequest.error = 27;
                    return;
                }
                if (strncmp(response, gGameStatsErrorDataLength, 22) == 0) {
                    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                    gGameStatsRequest.error = 28;
                    return;
                }
                if (strncmp(response, gGameStatsErrorTokenNotFound, 22) == 0) {
                    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                    gGameStatsRequest.error = 29;
                    return;
                }
                if (strncmp(response, gGameStatsErrorTokenExpired, 22) == 0) {
                    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                    gGameStatsRequest.error = 30;
                    return;
                }
                if (strncmp(response, gGameStatsErrorIncorrectHash, 22) == 0) {
                    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                    gGameStatsRequest.error = 31;
                    return;
                }
            }
            if (length <= gGameStatsRequest.responseCapacity) {
                memcpy(gGameStatsRequest.response, response, length);
                gGameStatsRequest.state = GAME_STATS_STATE_DONE;
            } else {
                memcpy(gGameStatsRequest.response, response, gGameStatsRequest.responseCapacity);
                gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
                gGameStatsRequest.error = 32;
            }
            gGameStatsRequest.responseLength = length;
            return;
        }
    } else {
        gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
        switch (error) {
        case 1:
            gGameStatsRequest.error = 8;
            return;
        case 2:
            gGameStatsRequest.error = 9;
            return;
        case 3:
            gGameStatsRequest.error = 10;
            return;
        case 4:
            gGameStatsRequest.error = 11;
            return;
        case 5:
            gGameStatsRequest.error = 12;
            return;
        case 6:
            gGameStatsRequest.error = 13;
            return;
        case 7:
            gGameStatsRequest.error = 14;
            return;
        case 8:
            gGameStatsRequest.error = 15;
            return;
        case 9:
            gGameStatsRequest.error = 16;
            return;
        case 10:
            gGameStatsRequest.error = 17;
            return;
        case 11:
            gGameStatsRequest.error = 18;
            return;
        case 12:
            gGameStatsRequest.error = 19;
            return;
        case 15:
            gGameStatsRequest.error = 22;
            return;
        case 16:
            gGameStatsRequest.error = 23;
            return;
        case 19:
            gGameStatsRequest.error = 25;
            break;
        }
    }
}

#pragma force_active reset

void GameStats_Start(void) {
    gGameStatsRequest.state = GAME_STATS_STATE_STARTED;
    gGameStatsRequest.requestId = -1;
    if (!ov00_021FA0B4(0)) {
        gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
    }
}

#pragma force_active on
#pragma opt_propagation off

int GameStats_PrepareRequest(char *url, int pid, void *payload, int payloadSize, void *response, int responseCapacity) {
    int len;
    int n;
    char *p;

    if (gGameStatsRequest.state != GAME_STATS_STATE_STARTED) {
        return 1;
    }
    gGameStatsRequest.pid = pid;
    gGameStatsRequest.payload = payload;
    gGameStatsRequest.payloadSize = payloadSize;
    gGameStatsRequest.response = response;
    gGameStatsRequest.responseCapacity = responseCapacity;
    payloadSize += 8;
    len = strlen(url);
    n = GameStats_GetBase64Length(payloadSize);
    gGameStatsRequest.url = ov00_021EC2A8(10, len + 69 + n);
    if (gGameStatsRequest.url == NULL) {
        return 2;
    }
    sprintf(gGameStatsRequest.url, gGameStatsUrlFormat, url, pid);
    p = gGameStatsRequest.url;
    len = strlen(p);
    n = strlen(gGameStatsHashParam);
    gGameStatsRequest.hashText = p + len + n;
    n = strlen(gGameStatsDataParam);
    gGameStatsRequest.dataText = gGameStatsRequest.hashText + 40 + n;
    gGameStatsRequest.dataTextSize = GameStats_GetBase64Length(payloadSize) + 1;
    gGameStatsRequest.state = GAME_STATS_STATE_PREPARED;
    return 0;
}

#pragma opt_propagation reset
#pragma force_active reset
