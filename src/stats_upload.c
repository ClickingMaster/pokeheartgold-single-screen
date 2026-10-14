#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"
#include "dwc/stats_upload_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

#include <string.h>

// Web stats upload client of the OVY_45 network library (see dwc/stats_upload_internal.h). Its key setup and
// random number generator are at the end of ciSocketParseNextLine.c.

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

extern const HexDigits sStr_0123456789abcdef; // "0123456789abcdef"
extern const char sStr_0123456789abcdef_2[17];  // "0123456789abcdef"
extern const char sStr_Hash[];    // "&hash="
extern const char sStr_Error[];    // "error:"
extern char gStatsUploadServerRelease[];          // server addresses
extern char gStatsUploadServerDev1[];
extern char gStatsUploadServerDev2[];
extern const char sStr_Data[]; // "&data="
extern const char sFmt_Int_7[]; // "%d"
extern const char sStr_Pid[]; // "?pid="
extern const char sFmt_PidHashData[]; // "%s%s%s?pid=%d&hash=%s&data="
extern const char sStr_0000000000000000000000000000000000000000[]; // 40 '0' characters, the hash placeholder
extern const char gStatsUploadEmptyString[]; // ""

// Checks the 40 hex digit SHA-1 at the end of the response: it is computed over the base64 encoding of the rest,
// between two copies of the secret key. On success the rest becomes the body.
BOOL StatsUpload_CheckResponseHash(const char *response, int length) {
    char hash[41];
    u8 digest[20];
    HexDigits hex;
    u32 bodyLength;
    u8 *buf;
    int i;
    hex = sStr_0123456789abcdef;
    if (length <= 40) {
        return FALSE;
    }
    bodyLength = length - 40;
    buf = DWC_Alloc(7, StatsUpload_GetBase64Length(bodyLength) + 41);
    if (buf == NULL) {
        return FALSE;
    }
    *(SecretKey *)buf = *(SecretKey *)gStatsUploadSecretKey;
    B64Encode(response, buf + 20, bodyLength, 2);
    {
        int n = StatsUpload_GetBase64Length(bodyLength);
        u8 *p = buf + 20;
        p += n;
        *(SecretKey *)p = *(SecretKey *)gStatsUploadSecretKey;
    }
    MATH_CalcSHA1(digest, buf, StatsUpload_GetBase64Length(bodyLength) + 40);
    DWC_Free(7, buf, 0);
    for (i = 0; i < 20; i++) {
        hash[i * 2] = hex.chars[digest[i] >> 4];
        hash[i * 2 + 1] = hex.chars[digest[i] & 0xF];
    }
    hash[40] = 0;
    if (strncmp(response + (length - 40), hash, 40) != 0) {
        return FALSE;
    }
    gStatsUpload.body = response;
    gStatsUpload.bodyLength = bodyLength;
    return TRUE;
}

// Progress callback of the HTTP requests.
void StatsUpload_OnRequestProgress(void) {
}

// Completion callback of the HTTP requests: the challenge (state 5) or the upload response (state 7).
void StatsUpload_OnRequestCompleted(char *response, int length, int error) {
    gStatsUpload.request = -1;
    if (gStatsUpload.state == STATS_UPLOAD_FAILED) {
        return;
    }
    if (error == 0) {
        switch (gStatsUpload.state) {
        case STATS_UPLOAD_WAIT_CHALLENGE:
            if (length == 32) {
                struct {
                    SecretKey key;
                    char challenge[32];
                } block;

                char hex[17];
                u8 *digest = (u8 *)gStatsUpload.hash + 20;
                int i, j;
                memcpy(hex, sStr_0123456789abcdef_2, 17);
                memcpy(&block.key, gStatsUploadSecretKey, 20);
                memcpy(block.challenge, response, 32);
                MATH_CalcSHA1(digest, &block, 52);
                for (i = 0, j = 0; i < 20; i++, j += 2) {
                    gStatsUpload.hash[j] = hex[digest[i] >> 4];
                    gStatsUpload.hash[j + 1] = hex[digest[i] & 15];
                }
                *(gStatsUpload.hash - strlen(sStr_Hash)) = '&';
                strncpy(gStatsUploadLastHash, gStatsUpload.hash, 41);
                gStatsUpload.state = STATS_UPLOAD_SEND;
            } else {
                StatsUpload_SetFailed();
            }
            DWCi_GsFree(response);
            break;
        case STATS_UPLOAD_WAIT_RESPONSE:
            if (strncmp(response, sStr_Error, strlen(sStr_Error)) == 0) {
                DWCi_GsFree(response);
                StatsUpload_SetFailed();
                return;
            }
            if (gStatsUpload.response) {
                DWCi_GsFree(gStatsUpload.response);
                gStatsUpload.response = NULL;
            }
            if (!StatsUpload_CheckResponseHash(response, length)) {
                DWCi_GsFree(response);
                StatsUpload_SetFailed();
                return;
            }
            if (gStatsUpload.callback && !gStatsUpload.callback(response, length)) {
                DWCi_GsFree(response);
                StatsUpload_SetFailed();
                return;
            }
            gStatsUpload.response = response;
            gStatsUpload.responseLength = length;
            gStatsUpload.state = STATS_UPLOAD_DONE;
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
    buf = DWC_Alloc(7, length + 4);
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
    StatsUpload_SeedRandom(sum);
    for (i = 0; i < length; i++) {
        buf[i + 4] ^= StatsUpload_RandomByte();
    }
    sum ^= gStatsUpload.checksumKey;
    buf[0] = sum >> 24;
    buf[1] = sum >> 16;
    buf[2] = sum >> 8;
    buf[3] = sum;
    B64Encode(buf, out, length + 4, 2);
    DWC_Free(7, buf, 0);
    out[StatsUpload_GetBase64Length(length + 4)] = 0;
    return 0;
}

// Starts the client: server 0 = the game stats server, 1 and 2 = development servers. key is the 20 character
// secret key, the four random number generator constants as 8 hex digits each and the game name.
void StatsUpload_Start(int server, char *key) {
    char secretKey[21];
    char hex[9] = { 0 };
    u32 multiplier, increment, modulus, checksumKey;
    gStatsUpload.request = -1;
    gStatsUpload.url = NULL;
    gStatsUpload.hash = NULL;
    gStatsUpload.data = NULL;
    gStatsUpload.response = NULL;
    gStatsUpload.responseLength = 0;
    switch (server) {
    case 0:
        gStatsUploadServer = gStatsUploadServerRelease;
        break;
    case 1:
        gStatsUploadServer = gStatsUploadServerDev1;
        break;
    case 2:
        gStatsUploadServer = gStatsUploadServerDev2;
        break;
    }
    strncpy(secretKey, key, 20);
    secretKey[20] = 0;
    multiplier = strtoul(strncpy(hex, key + 20, 8), NULL, 16);
    increment = strtoul(strncpy(hex, key + 28, 8), NULL, 16);
    modulus = strtoul(strncpy(hex, key + 36, 8), NULL, 16);
    checksumKey = strtoul(strncpy(hex, key + 44, 8), NULL, 16);
    StatsUpload_SetKey(key + 52, secretKey, multiplier, increment, modulus, checksumKey);
    DWC_InitGHTTP(0);
    gStatsUpload.state = STATS_UPLOAD_READY;
    gStatsUpload.active = TRUE;
}

void StatsUpload_SetFailed(void) {
    if (!DWCi_IsError()) {
        DWCi_SetError(6, -90040);
    }
    gStatsUpload.state = STATS_UPLOAD_FAILED;
}

// Advances the upload; returns the state.
int StatsUpload_Process(void) {
    int request;
    switch ((u32)gStatsUpload.state) {
    case STATS_UPLOAD_NONE:
    case STATS_UPLOAD_STOPPED:
    case STATS_UPLOAD_READY:
    case STATS_UPLOAD_DONE:
        break;
    case STATS_UPLOAD_FAILED:
        StatsUpload_SetFailed();
        break;
    case STATS_UPLOAD_GET_CHALLENGE:
        request = DWC_GetGHTTPDataEx(gStatsUpload.url, 0, 0, StatsUpload_OnRequestProgress, StatsUpload_OnRequestCompleted, 0);
        gStatsUpload.request = request;
        if (request >= 0) {
            gStatsUpload.state = STATS_UPLOAD_WAIT_CHALLENGE;
        } else {
            StatsUpload_SetFailed();
        }
        break;
    case STATS_UPLOAD_WAIT_CHALLENGE:
        if (!DWC_ProcessGHTTP()) {
            StatsUpload_SetFailed();
        }
        break;
    case STATS_UPLOAD_SEND:
        request = DWC_GetGHTTPDataEx(gStatsUpload.url, 0, 0, StatsUpload_OnRequestProgress, StatsUpload_OnRequestCompleted, 0);
        gStatsUpload.request = request;
        if (request >= 0) {
            gStatsUpload.state = STATS_UPLOAD_WAIT_RESPONSE;
        } else {
            StatsUpload_SetFailed();
        }
        break;
    case STATS_UPLOAD_WAIT_RESPONSE:
        if (!DWC_ProcessGHTTP()) {
            StatsUpload_SetFailed();
        }
        break;
    }
    return gStatsUpload.state;
}

// Stops the client and frees the URL and the last response.
void StatsUpload_Stop(void) {
    if (!gStatsUpload.active) {
        return;
    }
    if (gStatsUpload.url) {
        DWC_Free(7, gStatsUpload.url, 0);
        gStatsUpload.url = NULL;
    }
    if (gStatsUpload.response) {
        DWCi_GsFree(gStatsUpload.response);
        gStatsUpload.responseLength = 0;
        gStatsUpload.response = NULL;
    }
    DWC_ShutdownGHTTP();
    gStatsUpload.state = STATS_UPLOAD_STOPPED;
    gStatsUpload.active = FALSE;
}

const char *StatsUpload_GetResponseBody(int *length) {
    *length = gStatsUpload.bodyLength;
    return gStatsUpload.body;
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

    if (gStatsUpload.keyReady != TRUE) {
        return 3;
    }
    if (gStatsUpload.url) {
        DWC_Free(7, gStatsUpload.url, 0);
        gStatsUpload.url = NULL;
    }
    dataLength = StatsUpload_GetBase64Length(size + 12);
    dataTagLength = strlen(sStr_Data);
    hashTagLength = strlen(sStr_Hash);
    pidLength = snprintf(pidText, 16, sFmt_Int_7, pid);
    pidTagLength = strlen(sStr_Pid);
    nameLength = strlen(name);
    serverLength = strlen(gStatsUploadServer);
    gameNameLength = strlen(gStatsUploadGameName);
    gStatsUpload.url = DWC_Alloc(7, dataLength + (dataTagLength + (hashTagLength + (pidLength + (pidTagLength + (nameLength + (serverLength + gameNameLength)))) + 41)));
    if (!gStatsUpload.url) {
        return 2;
    }
    sprintf(gStatsUpload.url, sFmt_PidHashData, gStatsUploadServer, gStatsUploadGameName, name, pid, sStr_0000000000000000000000000000000000000000, gStatsUploadEmptyString);
    {
        char *url = gStatsUpload.url;
        gStatsUpload.data = url + strlen(url);
    }
    gStatsUpload.hash = gStatsUpload.data - strlen(sStr_Data) - 40;
    header.pid = pid;
    header.size = size;
    if (StatsUpload_EncodeData(gStatsUpload.data, (const u8 *)&header, 8, data, size) == 2) {
        DWC_Free(7, gStatsUpload.url, 0);
        gStatsUpload.url = NULL;
        return 2;
    }
    if (useLastHash) {
        memcpy(gStatsUpload.hash, gStatsUploadLastHash, 40);
        gStatsUpload.state = STATS_UPLOAD_SEND;
    } else {
        *(gStatsUpload.hash - strlen(sStr_Hash)) = 0;
        gStatsUpload.state = STATS_UPLOAD_GET_CHALLENGE;
    }
    gStatsUpload.callback = callback;
    return 0;
}

int StatsUpload_Upload(char *name, int pid, const void *data, int size, StatsUploadResponseFunc callback) {
    return StatsUpload_PrepareUpload(name, pid, data, size, callback, FALSE);
}

int StatsUpload_UploadWithLastHash(char *name, int pid, const void *data, int size, StatsUploadResponseFunc callback) {
    return StatsUpload_PrepareUpload(name, pid, data, size, callback, TRUE);
}
