#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
int sprintf(char *, const char *, ...);

typedef struct {
    int connected;
    u8 pad[24];
    u8 sink[1];
} Context;

typedef struct {
    char bytes[17];
} Pattern;

extern const char ov45_02256BA0[];
extern const char ov45_02256B94[];
extern const char ov45_02256ACC[];
extern const char ov45_02256E9C[];
extern const char ov45_02256EAC[];
extern const char ov45_02256FB4[];
extern const char ov45_02256FC4[];
extern const char ov45_02256FD8[];
extern const char ov45_02256FDC[];
void __msl_assertion_failed(const char *, const char *, const char *, int);
void ov45_02252F74(void *, const char *, const char *, const char *);
void ov45_02250A80(Context *, const char *, const int *);

void ov45_02250A80(Context *ctx, const char *name, const int *flags) {
    char buffer[64];
    int count;
    if (!ctx) {
        __msl_assertion_failed(ov45_02256BA0, ov45_02256B94, ov45_02256ACC, 0x67f);
    }
    if (!ctx || !ctx->connected) {
        return;
    }
    if (!name) {
        __msl_assertion_failed(ov45_02256E9C, ov45_02256B94, ov45_02256ACC, 0x682);
    }
    if (!*name) {
        __msl_assertion_failed(ov45_02256EAC, ov45_02256B94, ov45_02256ACC, 0x682);
    }
    if (!flags) {
        __msl_assertion_failed(ov45_02256FB4, ov45_02256B94, ov45_02256ACC, 0x683);
    }
    memcpy(buffer, ov45_02256FC4, 17);
    if (flags[0]) {
        buffer[0] = '+';
    } else {
        buffer[0] = '-';
    }
    if (flags[1]) {
        buffer[2] = '+';
    } else {
        buffer[2] = '-';
    }
    if (flags[2]) {
        buffer[4] = '+';
    } else {
        buffer[4] = '-';
    }
    if (flags[3]) {
        buffer[6] = '+';
    } else {
        buffer[6] = '-';
    }
    if (flags[4]) {
        buffer[8] = '+';
    } else {
        buffer[8] = '-';
    }
    if (flags[5]) {
        buffer[10] = '+';
    } else {
        buffer[10] = '-';
    }
    if (flags[7] > 0) {
        buffer[12] = '+';
    } else {
        buffer[12] = '-';
    }
    if (flags[6]) {
        buffer[14] = '+';
    } else {
        buffer[14] = '-';
    }
    count = flags[7];
    if (count > 0) {
        sprintf(buffer + strlen(buffer), ov45_02256FD8, count);
    }
    ov45_02252F74(ctx->sink, ov45_02256FDC, name, buffer);
}
