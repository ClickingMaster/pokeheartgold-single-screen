#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define B(p, o) (*(u8 *)((char *)(p) + (o)))
#define H(p, o) (*(u16 *)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))
typedef void *(*Alloc)(char *, int);
typedef void (*Free)(char *, void *, int);
extern char ov00_02216420[];
extern char ov00_0221A694[];
extern char ov00_02216474[];
extern char ov00_022166F8[];
extern char ov00_02216710[];
extern char ov00_022167B8[];
extern char ov00_022167CC[];
extern char ov00_022167E0[];
extern void ov00_021EA2B8(void *, void *, int);
extern int CPS_Resolve(void *);
extern void CPS_SocRegister(void *);
extern void CPS_SslAddRandomSeed(void *, int);
extern void ov00_021E8048(void *);
extern void ov00_021E9438(void *);
extern char ov00_0221A6B4[];
extern char ov00_0221A69C[];

typedef struct {
    char *base, *cursor, *end;
    int size;
} Buf;

extern void ov00_021E7B60(void *x);
extern void ov00_021E7C94(void);
extern void ov00_021E7D48(void);
extern void ov00_021E7DD4(void);
extern void ov00_021E7E2C(void);
extern int ov00_021E7E5C(void);
extern void ov00_021E7EA8(void *out);
extern void ov00_021E8940(int n);
extern void ov00_021E9154(void *p, int n);
extern void ov00_021E9210(void *p);
extern int ov00_021E926C(void *p);
extern void ov00_021E92E4(void *p);
extern int ov00_021E933C(void *p);
extern void ov00_021E9754(void *p);
extern int ov00_021E9B38(int n);
extern int ov00_021E9B44(void *p, Buf *b, int n);
extern void ov00_021E9B9C(void *p, void *b);
extern int ov00_021E9BDC(void *p, void *b, int n);

extern char ov00_02216464[];
extern char ov00_0221A6A0[];
extern char ov00_02216680[];
extern char ov00_02216698[];
extern char ov00_022166B0[];
extern char ov00_022166BC[];
extern char ov00_022166C4[];
extern char ov00_022166CC[];
extern char ov00_022166D0[];
extern char ov00_022166E0[];
extern char ov00_022166F4[];
extern char ov00_02216728[];
extern char ov00_0221677C[];
extern char ov00_0221679C[];
extern char ov00_022167B4[];
extern void DWCi_BM_GetWiFiInfo(void *);
extern int ov00_021E89AC(void *, void *);
extern int ov00_021E9C84(void *, void *);
extern long atol(const char *);

typedef struct {
    u32 v[18];
} Copy72;

typedef struct {
    u32 v[7];
} Copy28;

extern int ov00_021E7F38(int);
extern int ov00_021E7B70(void *settings, void *state);
extern int ov00_021E8984(void *out);
extern int ov00_021E8FA0(void *p, void *settings);
extern int ov00_021E90BC(void *p);
extern int ov00_021E9350(void *p);
extern int ov00_021E9808(void *p);
extern int ov00_021E98C8(void *p, char *name, char *value);
extern int ov00_021E9AB0(void *p, char *s);

void ov00_021E7C94(void) {
    void *p;
    OS_InitMutex((OSMutex *)A(P(ov00_0221A694, 8), 0x13d8));
    W(P(ov00_0221A694, 8), 0x13f0) = 0;
    p = P(ov00_0221A694, 8);
    if (W(p, 0x1384) && !OS_IsThreadTerminated((OSThread *)A(p, 0x1318))) {
        return;
    }
    p = P(ov00_0221A694, 8);
    OS_CreateThread((OSThread *)A(p, 0x1318), ov00_021E8048, ov00_0221A69C, A(p, 0x1000), 0x1000, 16);
    OS_WakeupThreadDirect((OSThread *)A(P(ov00_0221A694, 8), 0x1318));
}

void ov00_021E7D48(void) {
    void *p;
    if (!P(ov00_0221A694, 8)) {
        return;
    }
    OS_LockMutex((OSMutex *)A(P(ov00_0221A694, 8), 0x13d8));
    W(P(ov00_0221A694, 8), 0x13f0) = 1;
    OS_UnlockMutex((OSMutex *)A(P(ov00_0221A694, 8), 0x13d8));
    if (P(P(ov00_0221A694, 8), 0x1314)) {
        ov00_021E9210(P(P(ov00_0221A694, 8), 0x1314));
    }
    p = P(ov00_0221A694, 8);
    if (W(p, 0x1384)) {
        OS_JoinThread((OSThread *)A(p, 0x1318));
    }
}

void ov00_021E7DD4(void) {
    void *p = P(ov00_0221A694, 8);
    Free f;
    if (!p) {
        return;
    }
    f = (Free)P(p, 0x1210);
    if (P(p, 0x1314)) {
        ov00_021E9754(P(p, 0x1314));
    }
    f(ov00_02216474, P(ov00_0221A694, 8), 0);
    P(ov00_0221A694, 8) = 0;
}

void ov00_021E7E2C(void) {
    void *p = P(ov00_0221A694, 8);
    if (W(p, 0x1384)) {
        OS_JoinThread((OSThread *)A(p, 0x1318));
    }
}

int ov00_021E7E5C(void) {
    void *p;
    int r;
    if (!P(ov00_0221A694, 8)) {
        return 22;
    }
    OS_LockMutex((OSMutex *)A(P(ov00_0221A694, 8), 0x13d8));
    p = P(ov00_0221A694, 8);
    r = W(p, 0x1004);
    OS_UnlockMutex((OSMutex *)A(p, 0x13d8));
    return r;
}

void ov00_021E7EA8(void *out) {
    int n;
    if (!P(ov00_0221A694, 8)) {
        MI_CpuFill8(out, 0, 0x1c4);
    }
    MI_CpuCopy8(A(P(ov00_0221A694, 8), 0x1008), out, 0x1c4);
    n = W(out, 0);
    if (n < 20000 || n >= 30000) {
        W(out, 0) = 20998;
    }
    n = W(out, 0);
    if (n >= 20100) {
        W(out, 0) = -n;
    }
}
