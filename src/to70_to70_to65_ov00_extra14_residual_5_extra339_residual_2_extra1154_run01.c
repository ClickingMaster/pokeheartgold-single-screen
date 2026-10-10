#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    char *base, *pos, *end;
    int capacity;
} Buf;

typedef struct {
    u8 pad[0x1008];
    int mode;
    u8 pad2[0x118];
    char *host, *path;
    u8 pad3[0x19f4 - 0x112c];
    int count;
    Buf buf;
} Ctx;

extern char ov00_02216728[];
extern char ov00_0221677C[];
extern char ov00_0221679C[];
extern char ov00_022166C4[];
extern char ov00_022167A8[];
extern char ov00_022167AC[];
int ov00_021E9B44(Ctx *, Buf *, int);
int ov00_021E9BDC(Ctx *, Buf *, int);
int ov00_021EBE08(void *, int, void *, int);
int ov00_021E9808(Ctx *);
int ov00_021E98C8(Ctx *, char *, char *);
int ov00_021E99A4(Ctx *, char *, void *, int);

int ov00_021E9808(Ctx *c) {
    Buf *b = &c->buf;
    char *format = c->mode == 0 ? ov00_02216728 : ov00_0221677C;
    int h = strlen(c->host), f = strlen(format), p = strlen(c->path);
    h += f - 4 + p;
    if (ov00_021E9B44(c, &c->buf, h + 0x400) != 1) {
        return 1;
    }
    b->pos += OS_SNPrintf(b->pos, b->capacity, format, c->path, c->host);
    return 0;
}

int ov00_021E98C8(Ctx *c, char *k, char *v) {
    Buf *b = &c->buf;
    int n = strlen(v), f = strlen(ov00_0221679C), l = strlen(k);
    char saved;
    char *p;
    int written;
    n += (f - 4) + l;
    if (n + 1 > b->end - b->pos) {
        if (!ov00_021E9BDC(c, b, n - (b->end - b->pos) + 1)) {
            return 1;
        }
    }
    p = strstr(b->base, ov00_022166C4) + 2;
    saved = *p;
    memmove(p + n, p, strlen(p) + 1);
    written = OS_SNPrintf(p, n + 1, ov00_0221679C, k, v);
    p[written] = saved;
    b->pos += n;
    return 0;
}

int ov00_021E99A4(Ctx *c, char *k, void *v, int len) {
    int n, f, l, total, remain;
    Buf *b = &c->buf;
    char *format = c->count == 0 ? ov00_022167A8 : ov00_022167AC;
    c->count++;
    n = ov00_021EBE08(v, len, 0, 0);
    f = strlen(format);
    l = strlen(k);
    total = (f - 2) + l + n;
    remain = b->end - b->pos;
    if (total > remain) {
        if (!ov00_021E9BDC(c, b, total - remain + 1)) {
            return 1;
        }
        remain = b->end - b->pos;
    }
    b->pos += OS_SNPrintf(b->pos, remain, format, k);
    if (ov00_021EBE08(v, len, b->pos, b->end - b->pos - 1) < 0) {
        return 1;
    }
    b->pos += n;
    *b->pos = 0;
    return 0;
}
