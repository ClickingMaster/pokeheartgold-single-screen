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

void ov00_021E9B9C(void *p, void *b) {
    Free f = (Free)P(p, 0x1014);
    if (P(b, 0)) {
        f(ov00_022167CC, P(b, 0), 0);
    }
    MI_CpuFill8(b, 0, 16);
}

int ov00_021E9BDC(void *p, void *b, int n) {
    Free f = (Free)P(p, 0x1014);
    Alloc a = (Alloc)P(p, 0x1010);
    char *s;
    if (n <= 0) {
        return 0;
    }
    s = a(ov00_022167E0, W(b, 12) + n);
    if (!s) {
        return 0;
    }
    MI_CpuCopy8(P(b, 0), s, W(b, 12));
    f(ov00_022167CC, P(b, 0), 0);
    if (!s) {
        return 0;
    }
    P(b, 4) = (char *)P(b, 4) + (s - (char *)P(b, 0));
    W(b, 12) += n;
    P(b, 0) = s;
    P(b, 8) = s + W(b, 12);
    return 1;
}
