#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 x[3];
    int method, state;
    const char *url, *host;
    u32 pad1c;
    u16 port, pad22;
    const char *path, *headers;
    u32 pad2c, pad30;
    int close;
    u8 gap[0x50 - 0x38];
    u32 buffer[7];
    u8 rest[0x13c - 0x6c];
    int post;
    u8 gap2[12];
    u32 length;
    u8 gap3[12];
    int secure;
} Request;

extern const char ov38_02225BB4[];
extern const char ov38_02225BBC[];
extern const char ov38_02225BC4[];
extern const char ov38_02225BCC[];
extern const char ov38_02225BD8[];
extern const char ov38_02225BE0[];
extern const char ov38_02225BE8[];
extern const char ov38_02225BEC[];
extern const char ov38_02225BF8[];
extern const char ov38_02225C08[];
extern const char ov38_02225C14[];
extern const char ov38_02225C20[];
extern const char ov38_02225C28[];
extern const char ov38_02225C2C[];
extern const char ov38_02225C3C[];
extern int ov38_02226028;
extern void ghiAppendDataToBuffer(void *, const char *, int);
extern void ghiAppendCharToBuffer(void *, int);
extern void ov38_0221C5EC(void *, const char *, const char *);
extern void ov38_0221C6CC(void *, int);
extern void ghiResetBuffer(void *);
extern void ghiCallProgressCallback(void *, int, int);
extern const char *ghiPostGetContentType(void *);
extern int ov38_0221C748(void *);
extern int sprintf(char *, const char *, ...);
void ov38_0221F024(Request *);

void ov38_0221F024(Request *r) {
    u32 *buffer;
    char text[16];
    if (r->buffer[3] == 0) {
        buffer = r->buffer;
        ghiAppendDataToBuffer(buffer, r->post ? ov38_02225BB4 : r->method == 3 ? ov38_02225BBC
                                                                               : ov38_02225BC4,
            0);
        if (r->secure || ov38_02226028) {
            ghiAppendDataToBuffer(buffer, r->url, 0);
        } else {
            ghiAppendDataToBuffer(buffer, r->path, 0);
        }
        ghiAppendDataToBuffer(buffer, ov38_02225BCC, 0);
        if (r->port == 80) {
            ov38_0221C5EC(buffer, ov38_02225BD8, r->host);
        } else {
            ghiAppendDataToBuffer(buffer, ov38_02225BE0, 0);
            ghiAppendDataToBuffer(buffer, r->host, 0);
            ghiAppendCharToBuffer(buffer, 58);
            ov38_0221C6CC(buffer, r->port);
            ghiAppendDataToBuffer(buffer, ov38_02225BE8, 2);
        }
        if (r->headers == 0 || strstr(r->headers, ov38_02225BEC) == 0) {
            ov38_0221C5EC(buffer, ov38_02225BEC, ov38_02225BF8);
        }
        if (r->close) {
            ov38_0221C5EC(buffer, ov38_02225C08, ov38_02225C14);
        } else {
            ov38_0221C5EC(buffer, ov38_02225C08, ov38_02225C20);
        }
        if (r->post) {
            sprintf(text, ov38_02225C28, r->length);
            ov38_0221C5EC(buffer, ov38_02225C2C, text);
            ov38_0221C5EC(buffer, ov38_02225C3C, ghiPostGetContentType(r));
        }
        if (r->headers) {
            ghiAppendDataToBuffer(buffer, r->headers, 0);
        }
        ghiAppendDataToBuffer(buffer, ov38_02225BE8, 2);
        if (buffer != r->buffer) {
            ghiAppendDataToBuffer(r->buffer, (const char *)buffer[1], buffer[3]);
        }
    }
    if (!ov38_0221C748(r)) {
        return;
    }
    if ((int)r->buffer[4] < (int)r->buffer[3]) {
        return;
    }
    ghiResetBuffer(r->buffer);
    if (r->post) {
        r->state = 4;
    } else {
        r->state = 5;
    }
    ghiCallProgressCallback(r, 0, 0);
}
