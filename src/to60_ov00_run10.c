#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#include <string.h>

int sprintf(char *, const char *, ...);
int strncmp(const char *, const char *, size_t);
int sscanf(const char *, const char *, ...);
long atol(const char *);
void __msl_assertion_failed(const char *, const char *, const char *, int);

void *ov00_021EC3A8(u32 size);
void *ov00_021EC3BC(void *p, u32 size);
void ov00_021EC3D8(void *p);
char *ov00_021FF3D4(const char *s);

typedef struct GtVar {
    const char *name;
    int type;
    int count;

    union {
        int i;
        double d;
        char *s;
    } val;
} GtVar;

typedef struct GtCtx {
    void *table;
} GtCtx;

typedef struct GtGlobal {
    int i;
    GtCtx *ctx;
    double d;
} GtGlobal;

extern GtGlobal ov00_0221B1F0;
extern char ov00_022198AC[];
extern char ov00_022198B0[];
extern char ov00_02219890[];
extern char ov00_02219888[];
extern char ov00_0221989C[];
extern char ov00_022198BC[];
extern char ov00_02219768[];
extern char ov00_02219778[];
extern char ov00_0221973C[];
extern char ov00_02219788[];

void *ov00_021FF0C0(void *table, void *key);
void ov00_021FEF74(void *table, void *elem);

extern char ov00_022196C4[];
extern char ov00_022196C8[];
extern char ov00_022196E0[];
extern char ov00_022196E8[];
extern char ov00_022196F4[];
extern char ov00_022196FC[];
extern char ov00_0221972C[];
extern char ov00_02219794[];
extern char ov00_0221979C[];
extern char ov00_022197A4[];
extern char ov00_022197B0[];
extern char ov00_022197B8[];
extern char ov00_022197C8[];
extern char ov00_022197D4[];
extern char ov00_022197E4[];
extern char ov00_02219744[];
extern char ov00_022197EC[];
extern char ov00_02219868[];
extern char ov00_02219754[];
extern char ov00_02219878[];
#define GP_PARSE_ERROR(c)                   \
    {                                       \
        ov00_0220A86C(c, 1, ov00_02219878); \
        ov00_02202990(c, 3, 1);             \
        return 3;                           \
    }
extern char ov00_02219800[];
extern char ov00_02219838[];
extern char ov00_02219850[];

typedef struct GpPair {
    int a;
    int b;
} GpPair;

int ov00_0220792C(void *c, void *a, int code, void *b);
int ov00_02202424(void *c, void *a, char *buf);
int ov00_022079A8(void *c, void *a, const char *s, int n);
int ov00_0220A11C(void *c, int *v, void *a, int n, const char *s);
int ov00_0220A4A0(const char *str, const char *key, char *out, int size);
int ov00_0220A3C8(void *c, const char *msg, int flag);
void ov00_0220A86C(void **p, int code, const char *msg);
void ov00_0220A890(void **p, const char *msg);
void ov00_0220A3BC(void *a, const char *fmt, ...);
void ov00_02202990(void *c, int a, int b);
int ov00_02202A84(void *c, GpPair s, int *p, void *o, int z);
void ov00_022066D0(void *c, void *o);
int ov00_021FF59C(int a, int b, int *x, int *y);

typedef struct GpPeer {
    u8 pad0[0x13c];
    int busy;
    int pending;
} GpPeer;

typedef struct GpOp {
    int state;
    GpPeer *peer;
    u8 pad8[0xc];
    int x14;
    int x18;
    int result;
    struct GpOp *next;
} GpOp;

typedef struct GpInfo {
    u8 pad0[0x210];
    int opCount;
    u8 pad214[0x424 - 0x214];
    GpOp *ops;
} GpInfo;

extern char ov00_02219328[];
extern char ov00_022196A4[];
extern char ov00_022193E4[];
extern char ov00_022192BC[];
int ov00_0220851C(void *c, GpOp *op);

typedef struct QrSock {
    int sock;
    int x4;
    char *key;
} QrSock;

typedef struct QrState {
    int len;
    void *arr;
    int x8;
    char *buf;
    int state;
    u8 pad14[0x10];
    int flip;
    void *x28;
    int cap;
    char vals[2][0x100];
} QrState;

typedef struct Buf256 {
    char s[0x100];
} Buf256;

typedef struct Buf16 {
    char s[0x10];
} Buf16;

typedef struct QrEntry {
    int type;
    int x4;
    int x8;
    int xc;
    int x10;
    int x14;
    void *cb;
} QrEntry;

extern QrSock ov00_022198D8;
extern QrState ov00_0221B200;
extern Buf256 ov00_022162E7;
extern Buf16 ov00_02216164;
extern char ov00_02219934[];
extern char ov00_02219924[];
extern char ov00_02219944[];
extern char ov00_02219948[];
extern char ov00_0221994C[];
extern char ov00_02219954[];
extern char ov00_02219958[];
extern char ov00_02219960[];
extern char ov00_02219968[];
extern char ov00_02219970[];
extern char ov00_02219974[];
extern char ov00_02219978[];
extern char ov00_02219980[];
extern char ov00_02219988[];
extern char ov00_02219990[];
extern char ov00_0221999C[];
extern char ov00_022199A8[];
extern char ov00_022199B4[];

void ov00_021FF82C(int s, int how);
void ov00_021FF818(int s);
int ov00_021FF92C(int s, char *buf, int len, int flags);
int ov00_021FF694(int s);
int ArrayLength(void *arr);
void *ov00_021FE6EC(void *arr, int idx);
void ov00_021FE968(void *arr, int idx);
void ov00_021FE678(void *arr);

void ov00_0220AE60(void);
int ov00_0220B174(int s);
int ov00_0220B60C(char *buf, int len);
void ov00_0220B798(void);
void ov00_0220B040(char *buf, int len);
char *ov00_0220B088(const char *str, const char *key);
char *ov00_0220B15C(const char *str, const char *key);
char *ov00_0220B180(char *buf, int len);
int ov00_0220B1E8(int a, int b, int c);
void ov00_0220B26C(char *buf, int len);
void ov00_0220B31C(char *buf, int len);
void ov00_0220B3A8(char *buf, int len);
void ov00_0220B48C(char *buf, int len);
void ov00_0220B524(char *buf, int len);
void ov00_0220B694(int idx, int a1, int a2, char *a3, int a4);

typedef struct DynBuf {
    char *data;
    int cap;
    int len;
} DynBuf;

typedef struct CbObj {
    u8 pad0[0x14];
    int closing;
    int x18;
    int depth;
    int (*cb20)();
    void (*cb24)(struct CbObj *);
} CbObj;

typedef struct CbData {
    int x0;
    void *ptr;
    int len;
} CbData;

typedef struct CbTarget {
    u8 pad0[0x24];
    int refs;
} CbTarget;

extern char ov00_022199C0[];
extern char ov00_022199C8[];
extern char ov00_02219A08[];
extern char ov00_02219A10[];
extern char ov00_022199D0[];
extern char ov00_022199E4[];
extern char ov00_02219A70[];
extern char ov00_02219A8C[];
extern char ov00_02219A44[];
extern char ov00_02219A98[];
extern char ov00_02219A58[];
extern char ov00_02219ABC[];
extern char ov00_02219A30[];
extern char ov00_02219AE0[];
extern char ov00_02219AF8[];
extern char ov00_02219A1C[];
extern char ov00_02219C10[];
extern char ov00_02219C18[];
extern char ov00_02219BA0[];
extern char ov00_02219C28[];
extern char ov00_02219BD4[];

int ov00_021FF368(void);
void srand(unsigned int);
int rand(void);
int abs(int);
void ov00_0220F144(CbObj *o);

int ov00_0220B88C(QrState *p, int idx);
int ov00_0220B8BC(QrState *p, int idx);
int ov00_0220B8EC(QrState *p, const char *name, void *(*get)(void *, const char *, void *), int def, ...);
double ov00_0220B954(QrState *p, const char *name, void *(*get)(void *, const char *, void *), double def, ...);
char *ov00_0220B9C4(QrState *p, const char *name, char *(*get)(void *, const char *, char *), char *def, int unused);
int ov00_0220BC50(u8 *s);
GtVar *ov00_0220AE00(GtCtx *ctx, const char *name);
void *ov00_0220ADE0(GtVar *v);
void *ov00_0220AD64(GtVar *v, void *val);
int *ov00_0220AD00(int x);
double *ov00_0220AD18(double x);
char *ov00_0220AD34(char *s);
void *ov00_0220AB88(GtCtx *ctx, const char *name, void *val);
int ov00_02209FCC(void **c);
void ov00_0220A1C0(void *c, void *a, int unused, const char *str);
int ov00_0220A244(void *c, char *o, char *msg);
void ov00_0220A348(char *dst, const char *src, int n);
int ov00_0220A5A8(void *c, int a, int *out);
int ov00_0220A69C(void **c, const char *str, int *pos, char *key, char *val);
void *ov00_0220A8A4(GtCtx *ctx, const char *name, int type, void *val);
void *ov00_0220A940(GtCtx *ctx, const char *name, void *val);
void *ov00_0220A96C(GtCtx *ctx, const char *name, void *val);
void *ov00_0220AA00(GtCtx *ctx, const char *name, void *val);
void *ov00_0220AA84(GtCtx *ctx, const char *name, void *val);
void *ov00_0220AB04(GtCtx *ctx, const char *name, void *val);
void *ov00_0220AC2C(GtCtx *ctx, const char *name, void *val);
int ov00_0220AED0(void);
int ov00_0220AEF0(void);
int ov00_0220BA20(QrState *p, int n, void *(*get)(void *, const char *, void *), int def, int idx);
double ov00_0220BA7C(QrState *p, int n, void *(*get)(void *, const char *, void *), double def, int idx);
char *ov00_0220BADC(QrState *p, int n, char *(*get)(void *, const char *, char *), char *def, int idx);
int ov00_0220BB38(QrState *p, int n, void *(*get)(void *, const char *, void *), int def, int idx);
double ov00_0220BB94(QrState *p, int n, void *(*get)(void *, const char *, void *), double def, int idx);
char *ov00_0220BBF4(QrState *p, int n, char *(*get)(void *, const char *, char *), char *def, int idx);
u8 *ov00_0220BCEC(u8 *s);
char *ov00_0220BE08(char *out, char *in);
int ov00_0220BF10(u8 *a, u8 *b);
int ov00_0220BF44(DynBuf *b, int size);
int ov00_0220BF70(DynBuf *b);
void ov00_0220BF80(DynBuf *b, char c);
void ov00_0220BFD0(DynBuf *b, int v);
void ov00_0220C03C(DynBuf *b, const void *src, int n);
void ov00_0220C0C4(DynBuf *b, int pos, int n);
int ov00_0220C164(CbObj *o);
int ov00_0220C200(CbObj *o, CbTarget *t, int a2, int a3, int a4, int a5, int a6);

double *ov00_0220AD18(double x) {
    ov00_0221B1F0.d = x;
    return &ov00_0221B1F0.d;
}

char *ov00_0220AD34(char *s) {
    char *start = s;
    while (*s) {
        if (*s == '\\') {
            *s = '/';
        }
        s++;
    }
    return start;
}

void *ov00_0220AD64(GtVar *v, void *val) {
    if (v->type == 0) {
        v->val.i = *(int *)val;
    } else if (v->type == 1) {
        v->val.d = *(double *)val;
    } else if (v->type == 2) {
        if (v->val.s) {
            ov00_021EC3D8(v->val.s);
        }
        v->val.s = val ? ov00_0220AD34(ov00_021FF3D4(val)) : 0;
    }
    return ov00_0220ADE0(v);
}

void *ov00_0220ADE0(GtVar *v) {
    if (!v) {
        return 0;
    }
    if (v->type == 2) {
        return v->val.s;
    }
    return &v->val;
}

GtVar *ov00_0220AE00(GtCtx *ctx, const char *name) {
    GtVar key;
    if (!ctx) {
        ctx = ov00_0221B1F0.ctx;
    }
    if (!ctx) {
        __msl_assertion_failed(ov00_022198AC, ov00_022198B0, ov00_02219888, 0x153);
    }
    key.name = name;
    return ov00_021FF0C0(ctx->table, &key);
}

void ov00_0220AE60(void) {
    if (ov00_022198D8.sock != -1) {
        ov00_021FF82C(ov00_022198D8.sock, 2);
        ov00_021FF818(ov00_022198D8.sock);
    }
    ov00_022198D8.sock = -1;
    ov00_0220B798();
    if (ov00_0221B200.buf) {
        ov00_021EC3D8(ov00_0221B200.buf);
        ov00_0221B200.buf = 0;
        ov00_0221B200.cap = 0;
        ov00_0221B200.len = 0;
    }
}

int ov00_0220AED0(void) {
    return ov00_022198D8.sock != -1;
}

int ov00_0220AEF0(void) {
    int n;
    int used;
    if (ov00_022198D8.sock == -1) {
        return 0;
    }
    if (ov00_0221B200.state != 5) {
        return 0;
    }
    while (ov00_0220B174(ov00_022198D8.sock)) {
        if (ov00_0221B200.cap - ov00_0221B200.len < 0x80) {
            if (ov00_0221B200.cap < 0x100) {
                ov00_0221B200.cap = 0x100;
            } else {
                ov00_0221B200.cap = ov00_0221B200.cap * 2;
            }
            ov00_0221B200.buf = ov00_021EC3BC(ov00_0221B200.buf, ov00_0221B200.cap + 1);
            if (!ov00_0221B200.buf) {
                return 0;
            }
        }
        n = ov00_021FF92C(ov00_022198D8.sock, ov00_0221B200.buf + ov00_0221B200.len, ov00_0221B200.cap - ov00_0221B200.len, 0);
        if (n <= 0) {
            ov00_0220AE60();
            return 0;
        }
        ov00_0221B200.len += n;
        ov00_0221B200.buf[ov00_0221B200.len] = 0;
        used = ov00_0220B60C(ov00_0221B200.buf, ov00_0221B200.len);
        if (used == ov00_0221B200.len) {
            ov00_0221B200.len = 0;
        } else {
            memmove(ov00_0221B200.buf, ov00_0221B200.buf + used, ov00_0221B200.len - used);
            ov00_0221B200.len -= used;
        }
    }
    if (ov00_022198D8.sock == -1) {
        return 0;
    }
    return 1;
}

void ov00_0220B040(char *buf, int len) {
    int i;
    char *k;
    k = ov00_022198D8.key;
    for (i = 0; i < len; i++) {
        buf[i] ^= *k;
        k++;
        if (*k == 0) {
            k = ov00_022198D8.key;
        }
    }
}

char *ov00_0220B088(const char *str, const char *key) {
    Buf256 b = ov00_022162E7;
    const char *p;
    char *ret;
    char *dst;
    ov00_0221B200.flip ^= 1;
    strcat(b.s, key);
    strcat(b.s, ov00_02219948);
    p = strstr(str, b.s);
    if (!p) {
        return 0;
    }
    p += strlen(b.s);
    ret = ov00_0221B200.vals[ov00_0221B200.flip];
    dst = ret;
    while (*p != 0 && *p != '\\') {
        *dst++ = *p++;
    }
    *dst = 0;
    return ret;
}

char *ov00_0220B15C(const char *str, const char *key) {
    char *r = ov00_0220B088(str, key);
    if (!r) {
        r = ov00_02219944;
    }
    return r;
}

int ov00_0220B174(int s) {
    return ov00_021FF694(s);
}

char *ov00_0220B180(char *buf, int len) {
    char *p;
    for (p = buf; p - buf < len - 6; p++) {
        if (p[0] == '\\' && p[1] == 'f' && p[2] == 'i' && p[3] == 'n' && p[4] == 'a' && p[5] == 'l' && p[6] == '\\') {
            return p;
        }
    }
    return 0;
}

int ov00_0220B1E8(int a, int b, int c) {
    int i;
    if (!ov00_0221B200.arr) {
        return -1;
    }
    for (i = 0; i < ArrayLength(ov00_0221B200.arr); i++) {
        QrEntry *e = ov00_021FE6EC(ov00_0221B200.arr, i);
        if (e->type == a && e->x4 == b && e->x8 == c) {
            return i;
        }
    }
    return -1;
}

void ov00_0220B26C(char *buf, int len) {
    int v;
    int id;
    int idx;
    char *s;
    v = atol(ov00_0220B15C(buf, ov00_0221994C));
    id = atol(ov00_0220B15C(buf, ov00_02219954));
    s = ov00_0220B15C(buf, ov00_02219958);
    idx = ov00_0220B1E8(0, id, 0);
    if (idx == -1) {
        return;
    }
    ((QrEntry *)ov00_021FE6EC(ov00_0221B200.arr, idx))->x8 = v;
    ov00_0220B694(idx, v > 0, 0, s, 0);
}

void ov00_0220B31C(char *buf, int len) {
    int v = atol(ov00_0220B15C(buf, ov00_02219960));
    int id = atol(ov00_0220B15C(buf, ov00_02219954));
    int idx = ov00_0220B1E8(3, id, 0);
    QrEntry *e;
    if (idx == -1) {
        return;
    }
    e = ov00_021FE6EC(ov00_0221B200.arr, idx);
    e->x8 = v;
    ov00_0220B694(idx, v > 0, 0, 0, 0);
}

void ov00_0220B3A8(char *buf, int len) {
    int idx;
    int d;
    int a;
    int c;
    int b;
    int id;
    char *p;
    char *s;
    a = atol(ov00_0220B15C(buf, ov00_02219968));
    id = atol(ov00_0220B15C(buf, ov00_02219954));
    b = atol(ov00_0220B15C(buf, ov00_02219970));
    c = atol(ov00_0220B15C(buf, ov00_02219974));
    idx = ov00_0220B1E8(1, id, b);
    if (idx != -1) {
        d = atol(ov00_0220B15C(buf, ov00_02219978));
        p = strstr(buf, ov00_02219980);
        if (!p) {
            s = ov00_02219944;
            d = 0;
        } else {
            s = p + 6;
        }
        ov00_0220B694(idx, a, c, s, d);
    }
}

void ov00_0220B48C(char *buf, int len) {
    int a = atol(ov00_0220B15C(buf, ov00_02219988));
    int b = atol(ov00_0220B15C(buf, ov00_02219970));
    int id = atol(ov00_0220B15C(buf, ov00_02219954));
    int c = atol(ov00_0220B15C(buf, ov00_02219974));
    int idx = ov00_0220B1E8(2, id, b);
    if (idx == -1) {
        return;
    }
    ov00_0220B694(idx, a, c, 0, 0);
}

void ov00_0220B524(char *buf, int len) {
    buf[len] = 0;
    if (strncmp(buf, ov00_02219990, 8) == 0) {
        ov00_0220B26C(buf, len);
    } else if (strncmp(buf, ov00_0221999C, 9) == 0) {
        ov00_0220B31C(buf, len);
    } else if (strncmp(buf, ov00_0221999C, 9) == 0) {
        ov00_0220B31C(buf, len);
    } else if (strncmp(buf, ov00_022199A8, 8) == 0) {
        ov00_0220B3A8(buf, len);
    } else if (strncmp(buf, ov00_022199B4, 8) == 0) {
        ov00_0220B48C(buf, len);
    }
}

int ov00_0220B60C(char *buf, int len) {
    char *p;
    int orig;
    int n;
    orig = len;
    p = ov00_0220B180(buf, len);
    while (len > 0 && p) {
        n = p - buf;
        ov00_022198D8.key = ov00_02219924;
        ov00_0220B040(buf, n);
        ov00_0220B524(buf, n);
        len -= n + 7;
        buf = p + 7;
        if (len > 0) {
            p = ov00_0220B180(buf, len);
        }
    }
    return orig - len;
}

void ov00_0220B694(int idx, int a1, int a2, char *a3, int a4) {
    QrEntry *e;
    if (idx < 0) {
        return;
    }
    if (idx >= ArrayLength(ov00_0221B200.arr)) {
        return;
    }
    e = ov00_021FE6EC(ov00_0221B200.arr, idx);
    if (e->cb) {
        switch (e->type) {
        case 0:
            ((void (*)(int, int, int, char *, int))e->cb)(e->x4, e->x8, a1, a3, e->x14);
            break;
        case 1:
            ((void (*)(int, int, int, int, int, int, char *, int, int))e->cb)(e->x4, e->x8, e->xc, e->x10, a1, a2, a3, a4, e->x14);
            break;
        case 2:
            ((void (*)(int, int, int, int, int, int, int))e->cb)(e->x4, e->x8, e->xc, e->x10, a1, a2, e->x14);
            break;
        case 3:
            ((void (*)(int, int, int, int))e->cb)(e->x4, e->x8, a1, e->x14);
            break;
        }
    }
    ov00_021FE968(ov00_0221B200.arr, idx);
}

void ov00_0220B798(void) {
    int i;
    Buf16 b;
    Buf16 a;
    if (!ov00_0221B200.arr) {
        return;
    }
    i = ArrayLength(ov00_0221B200.arr) - 1;
    if (i >= 0) {
        a = ov00_02216164;
        do {
            b = a;
            ov00_022198D8.key = ov00_02219934;
            ov00_0220B040(b.s, 15);
            ov00_0220B694(i, 0, 0, b.s, 0);
        } while (--i >= 0);
    }
    ov00_021FE678(ov00_0221B200.arr);
    ov00_0221B200.arr = 0;
}

int ov00_0220B88C(QrState *p, int idx) {
    if (!p) {
        p = ov00_0221B200.x28;
    }
    if (!p) {
        return idx;
    }
    return *(int *)ov00_021FE6EC(*(void **)((char *)p + 0x20), idx);
}

int ov00_0220B8BC(QrState *p, int idx) {
    if (!p) {
        p = ov00_0221B200.x28;
    }
    if (!p) {
        return idx;
    }
    return *(int *)ov00_021FE6EC(*(void **)((char *)p + 0x1c), idx);
}

int ov00_0220B8EC(QrState *p, const char *name, void *(*get)(void *, const char *, void *), int def, ...) {
    void *r;
    if (!p) {
        p = ov00_0221B200.x28;
    }
    if (!p) {
        r = &def;
    } else {
        r = get(p->buf, name, &def);
        if (!r) {
            r = ov00_0220A8A4((GtCtx *)p->buf, name, 0, &def);
        }
    }
    return *(int *)r;
}

double ov00_0220B954(QrState *p, const char *name, void *(*get)(void *, const char *, void *), double def, ...) {
    void *r;
    if (!p) {
        p = ov00_0221B200.x28;
    }
    if (!p) {
        r = &def;
    } else {
        r = get(p->buf, name, &def);
        if (!r) {
            r = ov00_0220A8A4((GtCtx *)p->buf, name, 1, &def);
        }
    }
    return *(double *)r;
}

char *ov00_0220B9C4(QrState *p, const char *name, char *(*get)(void *, const char *, char *), char *def, int unused) {
    char *r;
    if (!p) {
        p = ov00_0221B200.x28;
    }
    if (!p) {
        return def;
    }
    r = get(p->buf, name, def);
    if (r) {
        return r;
    }
    return ov00_0220A8A4((GtCtx *)p->buf, name, 2, def);
}

int ov00_0220BA20(QrState *p, int n, void *(*get)(void *, const char *, void *), int def, int idx) {
    char buf[0x40];
    sprintf(buf, ov00_022199C0, n, ov00_0220B88C(p, idx));
    return ov00_0220B8EC(p, buf, get, def, idx);
}

double ov00_0220BA7C(QrState *p, int n, void *(*get)(void *, const char *, void *), double def, int idx) {
    char buf[0x40];
    sprintf(buf, ov00_022199C0, n, ov00_0220B88C(p, idx));
    return ov00_0220B954(p, buf, get, def, idx);
}

char *ov00_0220BADC(QrState *p, int n, char *(*get)(void *, const char *, char *), char *def, int idx) {
    char buf[0x40];
    sprintf(buf, ov00_022199C0, n, ov00_0220B88C(p, idx));
    return ov00_0220B9C4(p, buf, get, def, idx);
}

int ov00_0220BB38(QrState *p, int n, void *(*get)(void *, const char *, void *), int def, int idx) {
    char buf[0x40];
    sprintf(buf, ov00_022199C8, n, ov00_0220B8BC(p, idx));
    return ov00_0220B8EC(p, buf, get, def, idx);
}

double ov00_0220BB94(QrState *p, int n, void *(*get)(void *, const char *, void *), double def, int idx) {
    char buf[0x40];
    sprintf(buf, ov00_022199C8, n, ov00_0220B8BC(p, idx));
    return ov00_0220B954(p, buf, get, def, idx);
}

char *ov00_0220BBF4(QrState *p, int n, char *(*get)(void *, const char *, char *), char *def, int idx) {
    char buf[0x40];
    sprintf(buf, ov00_022199C8, n, ov00_0220B8BC(p, idx));
    return ov00_0220B9C4(p, buf, get, def, idx);
}
