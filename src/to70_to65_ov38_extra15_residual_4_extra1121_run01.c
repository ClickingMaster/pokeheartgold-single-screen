#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern char *strchr(const char *, int);
extern u32 ov00_021FF368(void);
extern void *ov00_021EC3A8(u32);
extern void ov00_021EC3D8(void *);
extern int ov38_0221CB7C(void *, void *, int *);
extern int ghiProcessIncomingFileData(void *, void *, int);

typedef struct {
    u8 p[0x38];
    int error;
    u8 p2[0xfc - 0x3c];
    int done, received, total;
    int other108, other10c, chunked;
    char header[12];
    int used, remaining, state;
    u8 p3[0x158 - 0x12c];
    u32 timeout;
} Work;

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern char ov38_02225B68[];
extern char ov38_02225B74[];
extern char ov38_02225B2C[];
extern char ov38_02225CB0[];
extern char ov38_02225CC4[];
extern char ov38_02225BB0[];
extern void ghiAppendToChunkHeaderBuffer(void *, const char *, int);
extern int ghiParseChunkSize(void *);
extern int ov38_0221F624(void *, const char *, int);
void ov38_0221FEA8(Work *);

int ghiProcessIncomingFileData(void *arg, void *bytes, int length) {
    Work *w = arg;
    char *data = bytes;
    char *line;
    int amount;
    if (!w) {
        __msl_assertion_failed(ov38_02225B68, ov38_02225B74, ov38_02225B2C, 0x35d);
    }
    if (!data) {
        __msl_assertion_failed(ov38_02225CB0, ov38_02225B74, ov38_02225B2C, 0x35e);
    }
    if (length <= 0) {
        __msl_assertion_failed(ov38_02225CC4, ov38_02225B74, ov38_02225B2C, 0x35f);
    }
    if (w->chunked) {
        while (length > 0) {
            if (w->state == 0) {
                line = strchr(data, 10);
                if (line) {
                    ghiAppendToChunkHeaderBuffer(w, data, line - data);
                    amount = line + 1 - data;
                    length -= amount;
                    data = line + 1;
                    w->remaining = ghiParseChunkSize(w);
                    if (w->remaining == -1) {
                        w->done = 1;
                        w->error = 7;
                        return 0;
                    }
                    if (w->remaining == 0) {
                        w->state = 3;
                    } else {
                        w->state = 1;
                    }
                } else {
                    ghiAppendToChunkHeaderBuffer(w, data, length);
                    return 1;
                }
            } else if (w->state == 1) {
                amount = w->remaining;
                if (amount >= length) {
                    amount = length;
                }
                if (!ov38_0221F624(w, data, amount)) {
                    return 0;
                }
                w->remaining -= amount;
                data += amount;
                length -= amount;
                if (w->remaining == 0) {
                    w->state = 2;
                }
            } else if (w->state == 2) {
                line = strchr(data, 10);
                if (!line) {
                    return 1;
                }
                w->header[0] = 0;
                w->used = 0;
                w->remaining = 0;
                w->state = 0;
                amount = line + 1 - data;
                data = line + 1;
                length -= amount;
            } else if (w->state == 3) {
                w->done = 1;
                return 1;
            } else {
                __msl_assertion_failed(ov38_02225BB0, ov38_02225B74, ov38_02225B2C, 0x3e3);
                return 0;
            }
        }
        return 1;
    }
    return ov38_0221F624(w, data, length);
}
