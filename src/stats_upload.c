#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"
#include "dwc/stats_upload_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

#include <string.h>

// Web stats upload client of the OVY_45 network library (see dwc/stats_upload_internal.h). Its key setup and
// random number generator are at the end of ov45_02253968.c.

int snprintf(char *, u32, const char *, ...);
int sprintf(char *, const char *, ...);
int strncmp(const char *, const char *, size_t);
unsigned long strtoul(const char *, char **, int);
void MATH_CalcSHA1(void *digest, const void *data, u32 length);

typedef struct HexDigits {
    char chars[17];
} HexDigits;

typedef struct SecretKey {
    u8 bytes[20];
} SecretKey;

extern const HexDigits ov45_02254DF4; // "0123456789abcdef"
extern const char ov45_02254E05[17];  // "0123456789abcdef"
extern const char ov45_022576CC[];    // "&hash="
extern const char ov45_022576D4[];    // "error:"
extern char ov45_022576DC[];          // server addresses
extern char ov45_02257704[];
extern char ov45_02257728[];
extern const char ov45_02257750[]; // "&data="
extern const char ov45_02257758[]; // "%d"
extern const char ov45_0225775C[]; // "?pid="
extern const char ov45_02257764[]; // "%s%s%s?pid=%d&hash=%s&data="
extern const char ov45_02257780[]; // 40 '0' characters, the hash placeholder
extern const char ov45_022577AC[]; // ""

// Checks the 40 hex digit SHA-1 at the end of the response: it is computed over the base64 encoding of the rest,
// between two copies of the secret key. On success the rest becomes the body.
BOOL StatsUpload_CheckResponseHash(const char *response, int length) {
    char hash[41];
    u8 digest[20];
    HexDigits hex;
    u32 bodyLength;
    u8 *buf;
    int i;
    hex = ov45_02254DF4;
    if (length <= 40) {
        return FALSE;
    }
    bodyLength = length - 40;
    buf = ov00_021EC2A8(7, ov45_02253D50(bodyLength) + 41);
    if (buf == NULL) {
        return FALSE;
    }
    *(SecretKey *)buf = *(SecretKey *)ov45_0225765C;
    B64Encode(response, buf + 20, bodyLength, 2);
    {
        int n = ov45_02253D50(bodyLength);
        u8 *p = buf + 20;
        p += n;
        *(SecretKey *)p = *(SecretKey *)ov45_0225765C;
    }
    MATH_CalcSHA1(digest, buf, ov45_02253D50(bodyLength) + 40);
    ov00_021EC2EC(7, buf, 0);
    for (i = 0; i < 20; i++) {
        hash[i * 2] = hex.chars[digest[i] >> 4];
        hash[i * 2 + 1] = hex.chars[digest[i] & 0xF];
    }
    hash[40] = 0;
    if (strncmp(response + (length - 40), hash, 40) != 0) {
        return FALSE;
    }
    ov45_0225762C.body = response;
    ov45_0225762C.bodyLength = bodyLength;
    return TRUE;
}

// Progress callback of the HTTP requests.
void StatsUpload_OnRequestProgress(void) {
}

// Completion callback of the HTTP requests: the challenge (state 5) or the upload response (state 7).
void StatsUpload_OnRequestCompleted(char *response, int length, int error) {
    ov45_0225762C.request = -1;
    if (ov45_0225762C.state == STATS_UPLOAD_FAILED) {
        return;
    }
    if (error == 0) {
        switch (ov45_0225762C.state) {
        case STATS_UPLOAD_WAIT_CHALLENGE:
            if (length == 32) {
                struct {
                    SecretKey key;
                    char challenge[32];
                } block;

                char hex[17];
                u8 *digest = (u8 *)ov45_0225762C.hash + 20;
                int i, j;
                memcpy(hex, ov45_02254E05, 17);
                memcpy(&block.key, ov45_0225765C, 20);
                memcpy(block.challenge, response, 32);
                MATH_CalcSHA1(digest, &block, 52);
                for (i = 0, j = 0; i < 20; i++, j += 2) {
                    ov45_0225762C.hash[j] = hex[digest[i] >> 4];
                    ov45_0225762C.hash[j + 1] = hex[digest[i] & 15];
                }
                *(ov45_0225762C.hash - strlen(ov45_022576CC)) = '&';
                strncpy(ov45_022576A0, ov45_0225762C.hash, 41);
                ov45_0225762C.state = STATS_UPLOAD_SEND;
            } else {
                StatsUpload_SetFailed();
            }
            ov00_021EC3D8(response);
            break;
        case STATS_UPLOAD_WAIT_RESPONSE:
            if (strncmp(response, ov45_022576D4, strlen(ov45_022576D4)) == 0) {
                ov00_021EC3D8(response);
                StatsUpload_SetFailed();
                return;
            }
            if (ov45_0225762C.response) {
                ov00_021EC3D8(ov45_0225762C.response);
                ov45_0225762C.response = NULL;
            }
            if (!StatsUpload_CheckResponseHash(response, length)) {
                ov00_021EC3D8(response);
                StatsUpload_SetFailed();
                return;
            }
            if (ov45_0225762C.callback && !ov45_0225762C.callback(response, length)) {
                ov00_021EC3D8(response);
                StatsUpload_SetFailed();
                return;
            }
            ov45_0225762C.response = response;
            ov45_0225762C.responseLength = length;
            ov45_0225762C.state = STATS_UPLOAD_DONE;
            break;
        }
    } else {
        StatsUpload_SetFailed();
    }
}

// Writes the base64 encoding of [checksum][first][second] to out (NUL terminated). The data bytes are xored
// with the random number generator seeded with their sum; the checksum (big endian) is that sum xored with
// checksumKey. Returns 0, or 2 when out of memory.
int StatsUpload_EncodeData(char *out, const u8 *first, int firstLength, const u8 *second, volatile int secondLength) {
    u8 *buf;
    u32 sum = 0;
    int length = secondLength + firstLength;
    int i;
    buf = ov00_021EC2A8(7, length + 4);
    if (!buf) {
        return 2;
    }
    for (i = 0; i < firstLength; i++) {
        buf[i + 4] = first[i];
    }
    {
        // (written this way for the matching code: the index lives in first, which is no longer needed, and
        // secondLength is volatile)
        int n = secondLength;
        first = NULL;
        if (n > 0) {
            u8 *dest = buf + firstLength;
            do {
                (dest + (u32)first)[4] = second[(u32)first];
                first = (const u8 *)((u32)first + 1);
            } while ((s32)first < n);
        }
    }
    for (i = 0; i < length; i++) {
        sum += buf[i + 4];
    }
    ov45_02253D88(sum);
    for (i = 0; i < length; i++) {
        buf[i + 4] ^= ov45_02253D9C();
    }
    sum ^= ov45_0225762C.checksumKey;
    buf[0] = sum >> 24;
    buf[1] = sum >> 16;
    buf[2] = sum >> 8;
    buf[3] = sum;
    B64Encode(buf, out, length + 4, 2);
    ov00_021EC2EC(7, buf, 0);
    out[ov45_02253D50(length + 4)] = 0;
    return 0;
}

// Starts the client: server 0 = the game stats server, 1 and 2 = development servers. key is the 20 character
// secret key, the four random number generator constants as 8 hex digits each and the game name.
void StatsUpload_Start(int server, char *key) {
    char secretKey[21];
    char hex[9] = { 0 };
    u32 multiplier, increment, modulus, checksumKey;
    ov45_0225762C.request = -1;
    ov45_0225762C.url = NULL;
    ov45_0225762C.hash = NULL;
    ov45_0225762C.data = NULL;
    ov45_0225762C.response = NULL;
    ov45_0225762C.responseLength = 0;
    switch (server) {
    case 0:
        ov45_02257628 = ov45_022576DC;
        break;
    case 1:
        ov45_02257628 = ov45_02257704;
        break;
    case 2:
        ov45_02257628 = ov45_02257728;
        break;
    }
    strncpy(secretKey, key, 20);
    secretKey[20] = 0;
    multiplier = strtoul(strncpy(hex, key + 20, 8), NULL, 16);
    increment = strtoul(strncpy(hex, key + 28, 8), NULL, 16);
    modulus = strtoul(strncpy(hex, key + 36, 8), NULL, 16);
    checksumKey = strtoul(strncpy(hex, key + 44, 8), NULL, 16);
    ov45_02253DDC(key + 52, secretKey, multiplier, increment, modulus, checksumKey);
    ov00_021FA0B4(0);
    ov45_0225762C.state = STATS_UPLOAD_READY;
    ov45_0225762C.active = TRUE;
}

void StatsUpload_SetFailed(void) {
    if (!ov00_021EC230()) {
        ov00_021EC24C(6, -90040);
    }
    ov45_0225762C.state = STATS_UPLOAD_FAILED;
}

// Advances the upload; returns the state.
int StatsUpload_Process(void) {
    int request;
    switch ((u32)ov45_0225762C.state) {
    case STATS_UPLOAD_NONE:
    case STATS_UPLOAD_STOPPED:
    case STATS_UPLOAD_READY:
    case STATS_UPLOAD_DONE:
        break;
    case STATS_UPLOAD_FAILED:
        StatsUpload_SetFailed();
        break;
    case STATS_UPLOAD_GET_CHALLENGE:
        request = ov00_021FA488(ov45_0225762C.url, 0, 0, StatsUpload_OnRequestProgress, StatsUpload_OnRequestCompleted, 0);
        ov45_0225762C.request = request;
        if (request >= 0) {
            ov45_0225762C.state = STATS_UPLOAD_WAIT_CHALLENGE;
        } else {
            StatsUpload_SetFailed();
        }
        break;
    case STATS_UPLOAD_WAIT_CHALLENGE:
        if (!ov00_021FA118()) {
            StatsUpload_SetFailed();
        }
        break;
    case STATS_UPLOAD_SEND:
        request = ov00_021FA488(ov45_0225762C.url, 0, 0, StatsUpload_OnRequestProgress, StatsUpload_OnRequestCompleted, 0);
        ov45_0225762C.request = request;
        if (request >= 0) {
            ov45_0225762C.state = STATS_UPLOAD_WAIT_RESPONSE;
        } else {
            StatsUpload_SetFailed();
        }
        break;
    case STATS_UPLOAD_WAIT_RESPONSE:
        if (!ov00_021FA118()) {
            StatsUpload_SetFailed();
        }
        break;
    }
    return ov45_0225762C.state;
}

// Stops the client and frees the URL and the last response.
void StatsUpload_Stop(void) {
    if (!ov45_0225762C.active) {
        return;
    }
    if (ov45_0225762C.url) {
        ov00_021EC2EC(7, ov45_0225762C.url, 0);
        ov45_0225762C.url = NULL;
    }
    if (ov45_0225762C.response) {
        ov00_021EC3D8(ov45_0225762C.response);
        ov45_0225762C.responseLength = 0;
        ov45_0225762C.response = NULL;
    }
    ov00_021FA0D8();
    ov45_0225762C.state = STATS_UPLOAD_STOPPED;
    ov45_0225762C.active = FALSE;
}

const char *StatsUpload_GetResponseBody(int *length) {
    *length = ov45_0225762C.bodyLength;
    return ov45_0225762C.body;
}

// Builds the upload URL for size bytes of data. With useLastHash the hash of the last challenge is used and
// the URL is sent at once, else a new challenge is fetched first. Returns 0, 2 when out of memory or 3 when
// the key is not set.
int StatsUpload_PrepareUpload(char *name, int pid, const void *data, int size, StatsUploadResponseFunc callback, BOOL useLastHash) {
    int dataLength, dataTagLength, hashTagLength, pidLength, pidTagLength, nameLength, serverLength, gameNameLength;
    char pidText[16];

    struct {
        int pid;
        int size;
    } header;

    if (ov45_0225762C.keyReady != TRUE) {
        return 3;
    }
    if (ov45_0225762C.url) {
        ov00_021EC2EC(7, ov45_0225762C.url, 0);
        ov45_0225762C.url = NULL;
    }
    dataLength = ov45_02253D50(size + 12);
    dataTagLength = strlen(ov45_02257750);
    hashTagLength = strlen(ov45_022576CC);
    pidLength = snprintf(pidText, 16, ov45_02257758, pid);
    pidTagLength = strlen(ov45_0225775C);
    nameLength = strlen(name);
    serverLength = strlen(ov45_02257628);
    gameNameLength = strlen(ov45_0225763C);
    ov45_0225762C.url = ov00_021EC2A8(7, dataLength + (dataTagLength + (hashTagLength + (pidLength + (pidTagLength + (nameLength + (serverLength + gameNameLength)))) + 41)));
    if (!ov45_0225762C.url) {
        return 2;
    }
    sprintf(ov45_0225762C.url, ov45_02257764, ov45_02257628, ov45_0225763C, name, pid, ov45_02257780, ov45_022577AC);
    {
        char *url = ov45_0225762C.url;
        ov45_0225762C.data = url + strlen(url);
    }
    ov45_0225762C.hash = ov45_0225762C.data - strlen(ov45_02257750) - 40;
    header.pid = pid;
    header.size = size;
    if (StatsUpload_EncodeData(ov45_0225762C.data, (const u8 *)&header, 8, data, size) == 2) {
        ov00_021EC2EC(7, ov45_0225762C.url, 0);
        ov45_0225762C.url = NULL;
        return 2;
    }
    if (useLastHash) {
        memcpy(ov45_0225762C.hash, ov45_022576A0, 40);
        ov45_0225762C.state = STATS_UPLOAD_SEND;
    } else {
        *(ov45_0225762C.hash - strlen(ov45_022576CC)) = 0;
        ov45_0225762C.state = STATS_UPLOAD_GET_CHALLENGE;
    }
    ov45_0225762C.callback = callback;
    return 0;
}

int StatsUpload_Upload(char *name, int pid, const void *data, int size, StatsUploadResponseFunc callback) {
    return StatsUpload_PrepareUpload(name, pid, data, size, callback, FALSE);
}

int StatsUpload_UploadWithLastHash(char *name, int pid, const void *data, int size, StatsUploadResponseFunc callback) {
    return StatsUpload_PrepareUpload(name, pid, data, size, callback, TRUE);
}
