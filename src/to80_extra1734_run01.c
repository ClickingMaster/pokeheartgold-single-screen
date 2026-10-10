#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    u32 unknown;
    char *data;
    u32 capacity;
    int length, position;
} Buffer;

typedef struct {
    u8 p[0xc];
    int type, state;
    u32 p1;
    char *host;
    u32 p2;
    u16 port;
    u8 p3[0x38 - 0x22];
    int error;
    u8 p4[0x48 - 0x3c];
    int socket, socketError;
    u8 p5[0x74 - 0x50];
    Buffer buffer;
    u8 p6[0xec - 0x88];
    int status;
    u32 p7;
    int start, headerLength, failed;
    u32 p8;
    int contentLength;
    char *redirect;
    int redirects, chunked;
    u8 chunkState;
    u8 p9[11];
    u32 chunkSize, chunkRead, chunkTotal;
} Request;

typedef struct {
    char bytes[11];
} MaxString;

extern const char ov38_02225CCC[];
extern const char ov38_02225CD4[];
extern const char ov38_02225CD8[];
extern const char ov38_02225CE4[];
extern const char ov38_02225CF4[];
extern const char ov38_02225D04[];
extern const MaxString ov38_02224E6C;
extern const u16 __msl_digit[];
extern void *ov00_021EC3A8(u32);
extern void ov00_021EC3D8(void *);
extern char *ov00_021FF3D4(const char *);
extern int ov00_021FFA60(int);
extern int ov38_0221CB7C(Request *, void *, int *);
extern int ghiAppendDataToBuffer(Buffer *, const void *, int);
extern void ghiResetBuffer(Buffer *);
extern void ghiCallProgressCallback(Request *, const void *, int);
extern void ghiProcessIncomingFileData(Request *, void *, int);
int sprintf(char *, const char *, ...);
long atol(const char *);
int strncmp(const char *, const char *, u32);
void ov38_0221FA38(Request *);

static inline int SPACE(int c) {
    return (c < 0 || c >= 128) ? 0 : __msl_digit[c] & 256;
}

#pragma force_active on

void ov38_0221FA38(Request *r) {
    char *receive;
    int size, status;
    char *base;
    int position;
    char *p, *body;
    int remaining, group;
    char *lengthHeader;
    MaxString maximum;
    char *value, *end;
    int n;
    receive = ov00_021EC3A8(4096);
    if (!receive) {
        r->failed = 1;
        r->error = 1;
        return;
    }
    size = 4096;
    status = ov38_0221CB7C(r, receive, &size);
    if (status == 3 || (status == 1 && r->buffer.position == r->buffer.length)) {
        goto finish;
    }
    if (status == 0 && !ghiAppendDataToBuffer(&r->buffer, receive, size)) {
        goto finish;
    }
    position = r->buffer.position;
    base = r->buffer.data;
    r->start = position;
    p = strstr(base + position, ov38_02225CCC);
    if (!p) {
        p = strstr(base + position, ov38_02225CD4);
    }
    if (p) {
        int oldLength;
        char *headerBase;
        char *headerEnd = p + 2;
        headerEnd[0] = 0;
        headerBase = r->buffer.data;
        oldLength = r->buffer.length;
        r->buffer.length = headerEnd - headerBase;
        body = p + 4;
        remaining = oldLength - (body - headerBase);
        r->headerLength = headerEnd - r->buffer.data;
        r->buffer.position = r->headerLength;
        group = r->status / 100;
        if (group == 1) {
            if (remaining) {
                memmove(r->buffer.data, body, remaining + 1);
                r->buffer.length = remaining;
                r->buffer.position = 0;
            } else {
                ghiResetBuffer(&r->buffer);
            }
            r->state = 6;
            ghiCallProgressCallback(r, 0, 0);
            goto finish;
        }
        if (group == 3) {
            if (r->redirects > 10) {
                r->failed = 1;
                r->error = 11;
                goto finish;
            }
            p = strstr(base + position, ov38_02225CD8);
            if (p) {
                value = p + 9;
                while (SPACE(*value)) {
                    ++value;
                }
                end = value;
                while (*end && !SPACE(*end)) {
                    ++end;
                }
                *end = 0;
                if (*value == '/') {
                    n = strlen(r->host);
                    n = (n + 14) + strlen(value);
                    r->redirect = ov00_021EC3A8(n);
                    if (!r->redirect) {
                        r->failed = 1;
                        r->error = 1;
                    }
                    sprintf(r->redirect, ov38_02225CE4, r->host, r->port, value);
                } else {
                    r->redirect = ov00_021FF3D4(value);
                    if (!r->redirect) {
                        r->failed = 1;
                        r->error = 1;
                    }
                }
                goto finish;
            }
        }
        lengthHeader = strstr(base + position, ov38_02225CF4);
        if (lengthHeader) {
            maximum = ov38_02224E6C;
            value = lengthHeader + 16;
            end = value;
            n = strlen(maximum.bytes);
            while (end && *end && *end != '\n' && *end != '\r' && *end != ' ') {
                ++end;
            }
            {
                int tokenLength = end - value;
                if (tokenLength > n) {
                    r->failed = 1;
                    r->error = 16;
                    goto finish;
                }
                if (n == tokenLength && strncmp(value, maximum.bytes, tokenLength) >= 0) {
                    r->failed = 1;
                    r->error = 16;
                    goto finish;
                }
            }
            r->contentLength = atol(value);
        }
        r->chunked = strstr(base + position, ov38_02225D04) != 0;
        if (r->chunked) {
            r->chunkState = 0;
            r->chunkSize = 0;
            r->chunkRead = 0;
            r->chunkTotal = 0;
        }
        if (r->type == 3 || r->type == 4) {
            r->failed = 1;
            goto finish;
        }
        r->state = 8;
        if (lengthHeader && r->contentLength == 0) {
            r->failed = 1;
            goto finish;
        }
        if (remaining > 0) {
            ghiProcessIncomingFileData(r, body, remaining);
        }
    } else if (status == 2) {
        r->failed = 1;
        r->error = 7;
        r->socketError = ov00_021FFA60(r->socket);
    }
finish:
    ov00_021EC3D8(receive);
}
