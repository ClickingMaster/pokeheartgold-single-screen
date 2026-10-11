#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
extern int sprintf(char *, const char *, ...);
extern const char ov45_02256BA0[];
extern const char ov45_02256B94[];
extern const char ov45_02256A7C[];
extern const char ov45_022570E4[];
extern const char ov45_022570A4[];
extern const char ov45_022570FC[];
extern const char ov45_0225710C[];
extern const char ov45_02257124[];
extern const char ov45_0225713C[];
extern const char ov45_02256AA4[];
extern const char ov45_022570B8[];

typedef struct {
    char b[5];
} Bytes5;

typedef struct {
    char b[2];
} Bytes2;

typedef struct {
    u8 b[3];
} Bytes3;

extern const struct {
    Bytes3 first;
    Bytes2 last;
} ov45_02257140;

extern const Bytes2 ov45_02256BC0;
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02252CB4(void *, const char *);
extern char *ov45_022514F4(void);
extern int ov45_0224ABA0(void *, char *, int, const char **, int, void *, void *);
extern int ov45_0224AA2C(void *, char *, int, const char **, int, int, void *, void *);
extern void ov45_0224F770(void *, int);
extern void ov00_021FF3C0(int);
extern int ov45_0224F660(void *, int);

int ov45_02251650(void *w, const char *a, const char *b, const char *c, int count, const char **entries);
void ov45_022518F0(void *w, const char *a, const char *b, int count, const char **entries, void *cb, void *arg, int wait);

#pragma opt_lifetimes off
#pragma force_active on

int ov45_02251650(void *w, const char *a, const char *b, const char *c, int count, const char **entries) {
    char buffer[512];
    int seen = 0;
    int i, n, len;
    const char *s;
    int j;
    char *p;
    if (w == 0) {
        __msl_assertion_failed(ov45_02256BA0, ov45_02256B94, ov45_02256A7C, 0xb19);
    }
    if (a == 0 || *a == 0) {
        __msl_assertion_failed(ov45_022570E4, ov45_02256B94, ov45_02256A7C, 0xb1b);
    }
    if (c == 0 || *c == 0) {
        __msl_assertion_failed(ov45_022570A4, ov45_02256B94, ov45_02256A7C, 0xb1c);
    }
    if (count != 0 && entries == 0) {
        __msl_assertion_failed(ov45_022570FC, ov45_02256B94, ov45_02256A7C, 0xb1d);
    }
    if (b == 0 || *b == 0) {
        sprintf(buffer, ov45_0225710C, a, c);
    } else {
        sprintf(buffer, ov45_02257124, a, b, c);
    }
    len = strlen(buffer);
    for (i = 0; i < count; i++) {
        s = entries[i];
        if (s == 0 || *s == 0) {
            continue;
        }
        if (strcmp(s, ov45_0225713C) == 0) {
            seen = 1;
            continue;
        }
        n = strlen(s);
        if (len + n + 1 >= 512) {
            continue;
        }
        buffer[len] = 0x5c;
        len++;
        p = buffer + len;
        memcpy(p, entries[i], n);
        for (j = len; j < len + n; j++) {
            if (*p == 0x5c) {
                *p = 0x2f;
            }
            p++;
        }
        len += n;
        buffer[len] = 0;
    }
    if (seen && len + 4 < 512) {
        {
            memcpy(buffer + len, &ov45_02257140.first, 3);
            *(Bytes2 *)(buffer + len + 3) = ov45_02257140.last;
        }
        len += 4;
    }
    if (count == 0 && (b == 0 || *b == 0)) {
        memcpy(buffer + len, &ov45_02256BC0, 2);
    }
    ov45_02252CB4((u8 *)w + 0x1c, buffer);
    return seen;
}
