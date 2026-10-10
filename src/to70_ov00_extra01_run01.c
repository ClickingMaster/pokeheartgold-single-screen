#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))
typedef void *(*Alloc)(char *, int);
typedef void (*Free)(char *, void *, int);
#define F(p, o) (*(Free *)((char *)(p) + (o)))

typedef struct {
    Alloc alloc;
    Free free;
    void *other;
} Cbs;

typedef struct {
    int a, b;
} Pair;

typedef struct {
    char *name, *value;
} Entry;

typedef struct {
    char pad[0x1a38];
    Entry entries[32];
} Dict;

extern void ov00_021E9754(void *);
extern void ov00_021E9210(void *);
extern void ov00_021E7DD4(void);
extern void ov00_021E7D48(void);
extern void ov00_021EA660(void *);
extern u32 ov00_021EBF90(char *, u32, char *, u32);
extern int ov00_021EBBEC(void);
extern int ov00_021E9F2C(void *, int);
extern int ov00_021EB5B0(char *);
extern char ov00_02216B64[];
extern char ov00_02216B6C[];
extern char ov00_02216B78[];
extern char ov00_02216B84[];
extern char ov00_02216B8C[];
extern char ov00_02216B9C[];
extern int errno;
extern RTCResult RTC_GetDateTime(RTCDate *, RTCTime *);
extern char ov00_02216AF0[];
extern char ov00_02216AC0[];
extern char ov00_0221A734[];
extern char ov00_02216BA8[];
extern u32 ov00_021EBE08(u8 *, u32, char *, u32);
extern long atol(const char *);
extern char ov00_0221A6B8[];
extern char ov00_0221A748[];
extern char ov00_0221A74C[];
extern char ov00_022168A4[];
extern char ov00_022168C0[];
extern char ov00_02216904[];
extern char ov00_02216918[];
extern char ov00_02216928[];
extern char ov00_02216938[];
extern char ov00_02216954[];
extern char ov00_02216974[];
extern char ov00_02216B10[];
extern char ov00_02216B20[];
extern char *ov00_021EA1CC(void *p, char *s);
extern u32 ov00_021EA224(void *p, char *s, char *out, u32 n);
extern int ov00_021EA278(void *p, char *s, char *out, int n);
extern void ov00_021EA2B8(void *p, void *arr, int n);
extern int ov00_021EA340(Cbs *cb);
extern void ov00_021EA420(void);
extern void ov00_021EA518(void);
extern int ov00_021EA580(void);
extern int ov00_021EA5C0(void);
extern void ov00_021EA5D8(void);
extern void ov00_021EB56C(int n);
extern int ov00_021EB7CC(Alloc a, Free b);
extern void ov00_021EB864(void);
extern int ov00_021EBB18(void);
extern int ov00_021EC0FC(int *p);
extern int ov00_021EC11C(int *p, int *q);
extern void ov00_021EC210(void);
extern int ov00_021EC230(void);
extern void ov00_021EC24C(int a, int b);

char *ov00_021EA1CC(void *p, char *s) {
    int i;
    for (i = 0; i < 32; i++) {
        if (!((Dict *)p)->entries[i].name) {
            break;
        }
        if (!strcmp(s, ((Dict *)p)->entries[i].name)) {
            return ((Dict *)p)->entries[i].value;
        }
    }
    return 0;
}

u32 ov00_021EA224(void *p, char *s, char *out, u32 n) {
    char *v = ov00_021EA1CC(p, s);
    u32 r;
    if (!v) {
        return 0;
    }
    r = ov00_021EBF90(v, strlen(v), out, n);
    if (r == 0xffffffff || r >= n) {
        return r;
    }
    out[r] = 0;
    return r;
}

int ov00_021EA278(void *p, char *s, char *out, int n) {
    char *v = ov00_021EA1CC(p, s);
    if (!v) {
        return 0;
    }
    if ((int)strlen(v) >= n) {
        return 0;
    }
    strcpy(out, v);
    return 1;
}

void ov00_021EA2B8(void *p, void *arr, int n) {
    Free cb = F(p, 0x1014);
    int i;
    char *name, *value;
    i = 0;
    if (n <= 0) {
        return;
    }
    name = ov00_022168A4;
    value = ov00_022168C0;
    do {
        if (((Entry *)arr)[i].name) {
            cb(name, ((Entry *)arr)[i].name, 0);
            ((Entry *)arr)[i].name = 0;
        }
        if (((Entry *)arr)[i].value) {
            cb(value, ((Entry *)arr)[i].value, 0);
            ((Entry *)arr)[i].value = 0;
        }
    } while (++i < n);
}

int ov00_021EA340(Cbs *cb) {
    Alloc alloc = cb->alloc;
    if (P(ov00_0221A6B8, 0)) {
        return 4;
    }
    P(ov00_0221A6B8, 0) = alloc(ov00_02216904, 0x11f4);
    if (!P(ov00_0221A6B8, 0)) {
        return 4;
    }
    MI_CpuFill8(P(ov00_0221A6B8, 0), 0, 0x11f4);
    W(P(ov00_0221A6B8, 0), 0x1004) = -99999;
    *(Cbs *)A(P(ov00_0221A6B8, 0), 0x1108) = *cb;
    if (P(ov00_0221A6B8, 20)) {
        return 4;
    }
    P(ov00_0221A6B8, 20) = alloc(ov00_02216918, 0x1c14);
    if (!P(ov00_0221A6B8, 20)) {
        return 4;
    }
    W(ov00_0221A6B8, 4) = 0;
    OS_InitMutex(A(P(ov00_0221A6B8, 0), 0x11dc));
    ov00_021EA5D8();
    return 0;
}

void ov00_021EA420(void) {
    Free cb = F(P(ov00_0221A6B8, 0), 0x110c);
    if (P(ov00_0221A6B8, 20)) {
        ov00_021E9754(P(ov00_0221A6B8, 20));
        cb(ov00_02216928, P(ov00_0221A6B8, 20), 0);
        P(ov00_0221A6B8, 20) = 0;
    }
    ov00_021E7DD4();
    if (!P(ov00_0221A6B8, 0)) {
        return;
    }
    if (P(P(ov00_0221A6B8, 0), 0x1114)) {
        cb(ov00_02216938, P(P(ov00_0221A6B8, 0), 0x1114), 0);
        P(P(ov00_0221A6B8, 0), 0x1114) = 0;
    }
    if (P(P(ov00_0221A6B8, 0), 0x1118)) {
        cb(ov00_02216954, P(P(ov00_0221A6B8, 0), 0x1118), 0);
        P(P(ov00_0221A6B8, 0), 0x1118) = 0;
    }
    cb(ov00_02216974, P(ov00_0221A6B8, 0), 0);
    P(ov00_0221A6B8, 0) = 0;
}

void ov00_021EA518(void) {
    if (!P(ov00_0221A6B8, 0)) {
        return;
    }
    if (P(ov00_0221A6B8, 20)) {
        ov00_021E9210(P(ov00_0221A6B8, 20));
    }
    ov00_021E7D48();
    if (W(P(ov00_0221A6B8, 0), 0x1188)) {
        OS_JoinThread(A(P(ov00_0221A6B8, 0), 0x111c));
    }
    W(P(ov00_0221A6B8, 0), 0x1004) = -7;
}

int ov00_021EA580(void) {
    int r;
    OS_LockMutex(A(P(ov00_0221A6B8, 0), 0x11dc));
    r = W(P(ov00_0221A6B8, 0), 0x1000);
    OS_UnlockMutex(A(P(ov00_0221A6B8, 0), 0x11dc));
    return r;
}

int ov00_021EA5C0(void) {
    return W(P(ov00_0221A6B8, 0), 0x1004);
}

void ov00_021EA5D8(void) {
    if (W(P(ov00_0221A6B8, 0), 0x1188) && !OS_IsThreadTerminated(A(P(ov00_0221A6B8, 0), 0x111c))) {
        return;
    }
    OS_CreateThread(A(P(ov00_0221A6B8, 0), 0x111c), ov00_021EA660, P(ov00_0221A6B8, 0), A(P(ov00_0221A6B8, 0), 0x1000), 0x1000, 16);
    OS_WakeupThreadDirect(A(P(ov00_0221A6B8, 0), 0x111c));
}
