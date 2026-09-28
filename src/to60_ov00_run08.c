#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <string.h>
#include <stdarg.h>
// clang-format on

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
#define GP_PARSE_ERROR(c) { ov00_0220A86C(c, 1, ov00_02219878); ov00_02202990(c, 3, 1); return 3; }
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

// @fn ov00_02209FCC
int ov00_02209FCC(void **c) {
    GpInfo *info = *c;
    int n = 0;
    GpOp **arr;
    GpOp *op;
    int i;
    int r;
    if (info->opCount > 0) {
        arr = ov00_021EC3A8(info->opCount * 4);
        if (!arr) {
            ov00_0220A890(c, ov00_02219328);
            return 1;
        }
        for (op = info->ops; op; op = op->next) {
            if (op->state == 3 && op->x14 != 5 && op->peer->busy == 0) {
                if (!(n < info->opCount)) __msl_assertion_failed(ov00_022196A4, ov00_022193E4, ov00_022192BC, 0x563);
                arr[n++] = op;
                op->peer->busy = 1;
            }
        }
        for (i = 0; i < n; i++) {
            r = ov00_0220851C(c, arr[i]);
            if (r) arr[i]->result = r;
        }
        for (i = 0; i < n; i++) {
            GpPeer *peer = arr[i]->peer;
            peer->busy = 0;
            if (peer->pending) ov00_022066D0(c, arr[i]);
        }
        ov00_021EC3D8(arr);
    }
    return 0;
}

// @fn ov00_0220A11C
int ov00_0220A11C(void *c, int *v, void *a, int n, const char *s) {
    char buf[0x20];
    int err;
    if (!s) s = ov00_022196C4;
    err = ov00_0220792C(c, a, 0xc9, v);
    if (err) return err;
    sprintf(buf, ov00_022196C8, 1, n);
    err = ov00_02202424(c, a, buf);
    if (err) return err;
    err = ov00_022079A8(c, a, s, -1);
    if (err) return err;
    return 0;
}

// @fn ov00_0220A1C0
void ov00_0220A1C0(void *c, void *a, int unused, const char *str) {
    char buf[0x40];
    int v[3];
    if (!ov00_0220A4A0(str, ov00_022196E0, buf, 0x40)) return;
    if (sscanf(buf, ov00_022196E8, &v[0], &v[1], &v[2]) != 3) return;
    ov00_0220A11C(c, v, a, 2, 0);
}

// @fn ov00_0220A244
int ov00_0220A244(void *c, char *o, char *msg) {
    GpPair s;
    int *p;
    int err;
    if (ov00_0220A3C8(c, msg, 1)) return 4;
    if (strncmp(msg, ov00_022196F4, 4) != 0) {
        ov00_0220A86C(c, 1, ov00_022196FC);
        ov00_02202990(c, 3, 1);
        return 3;
    }
    s = *(GpPair *)(o + 0xc);
    if (s.a) {
        p = ov00_021EC3A8(4);
        if (!p) {
            ov00_0220A890(c, ov00_0221972C);
            return 1;
        }
        *p = 0;
        err = ov00_02202A84(c, s, p, o, 0);
        if (err) return err;
    }
    ov00_022066D0(c, o);
    return 0;
}

// @fn ov00_0220A348
void ov00_0220A348(char *dst, const char *src, int n) {
    if (!dst) __msl_assertion_failed(ov00_02219768, ov00_02219778, ov00_0221973C, 0x2f);
    if (!src) __msl_assertion_failed(ov00_02219788, ov00_02219778, ov00_0221973C, 0x30);
    strncpy(dst, src, n);
    *(dst + n - 1) = 0;
}

// @fn ov00_0220A3BC
void ov00_0220A3BC(void *a, const char *fmt, ...) {
}

// @fn ov00_0220A3C8
int ov00_0220A3C8(void *c, const char *msg, int flag) {
    char buf[0x10];
    char *info = *(char **)c;
    if (strncmp(msg, ov00_02219794, 7) == 0) {
        if (ov00_0220A4A0(msg, ov00_0221979C, buf, 0x10)) *(int *)(info + 0x418) = atol(buf);
        if (!ov00_0220A4A0(msg, ov00_022197A4, info, 0x100)) info[0] = 0;
        if (flag) ov00_02202990(c, 4, (strstr(msg, ov00_022197B0) != NULL ? 1 : 0) ? 1 : 0);
        return 1;
    }
    return 0;
}

// @fn ov00_0220A4A0
int ov00_0220A4A0(const char *str, const char *key, char *out, int size) {
    char term;
    const char *p;
    int i;
    char ch;
    if (!str) __msl_assertion_failed(ov00_022197B8, ov00_02219778, ov00_02219744, 0x10d);
    if (!key) __msl_assertion_failed(ov00_022197C8, ov00_02219778, ov00_02219744, 0x10e);
    if (!out) __msl_assertion_failed(ov00_022197D4, ov00_02219778, ov00_02219744, 0x10f);
    if (!(size > 0)) __msl_assertion_failed(ov00_022197E4, ov00_02219778, ov00_02219744, 0x110);
    term = key[0];
    p = strstr(str, key);
    if (!p) return 0;
    p += strlen(key);
    for (i = 0; i < size - 1 && (ch = p[i]) != 0 && ch != term; i++) out[i] = ch;
    out[i] = 0;
    return 1;
}

// @fn ov00_0220A5A8
int ov00_0220A5A8(void *c, int a, int *out) {
    int x = 0;
    int y = 0;
    int r = ov00_021FF59C(a, 0, &x, &y);
    if (r == -1) {
        ov00_0220A3BC(c, ov00_022197EC);
        ov00_0220A86C(c, 5, ov00_02219800);
        ov00_02202990(c, 3, 1);
        return 3;
    }
    if (r > 0) {
        if (y) {
            ov00_0220A3BC(c, ov00_02219838);
            *out = 4;
            return 0;
        }
        if (x) {
            ov00_0220A3BC(c, ov00_02219850);
            *out = 3;
            return 0;
        }
    }
    *out = 0;
    return 0;
}
