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

int ov00_021EBB18(void) {
    if (!P(ov00_0221A748, 0)) {
        return 0;
    }
    if (!W(P(ov00_0221A748, 0), 0x1bac)) {
        return 1;
    }
    if (OS_IsThreadTerminated(A(P(ov00_0221A748, 0), 0x1b40)) == 1) {
        switch (W(P(ov00_0221A748, 0), 0x1028)) {
        case 8:
            if (!ov00_021EBBEC()) {
                ov00_021E9754(A(P(ov00_0221A748, 0), 8));
                return 4;
            }
            ov00_021E9754(A(P(ov00_0221A748, 0), 8));
            return 3;
        case 7:
            ov00_021E9754(A(P(ov00_0221A748, 0), 8));
            return 5;
        default:
            ov00_021E9754(A(P(ov00_0221A748, 0), 8));
            ov00_021EC24C(17, -24101);
            return 4;
        }
    }
    return 2;
}

int ov00_021EBBEC(void) {
    void *p = P(ov00_0221A748, 0);
    char b[4];
    char *v;
    int n;
    if (!ov00_021E9F2C(A(p, 8), 0)) {
        ov00_021EC24C(16, -25101);
        return 0;
    }
    v = ov00_021EA1CC(A(p, 8), ov00_02216B64);
    if (v) {
        ov00_021EB5B0(v);
    }
    v = ov00_021EA1CC(A(p, 8), ov00_02216B6C);
    if (!v) {
        ov00_021EC24C(16, -25101);
        return 0;
    }
    n = atol(v);
    if (errno == 34) {
        ov00_021EC24C(16, -25101);
        return 0;
    }
    if (n != 200) {
        ov00_021EC24C(16, -25000 - n);
        return 0;
    }
    if ((int)ov00_021EA224(A(p, 8), ov00_02216B78, b, 4) <= 0) {
        ov00_021EC24C(16, -25101);
        return 0;
    }
    n = atol(b);
    if (errno == 34) {
        ov00_021EC24C(16, -25101);
        return 0;
    }
    if (n >= 100) {
        ov00_021EC24C(16, -24000 - n);
        return 0;
    }
    ov00_021EA224(A(p, 8), ov00_02216B84, A(P(P(ov00_0221A748, 0), 0x1c1c), 4), 64);
    ov00_021EA224(A(p, 8), ov00_02216B8C, A(P(P(ov00_0221A748, 0), 0x1c1c), 69), 300);
    ov00_021EA224(A(p, 8), ov00_02216B9C, b, 1);
    if (b[0] == 'Y') {
        W(P(P(ov00_0221A748, 0), 0x1c1c), 0) = 1;
    } else {
        W(P(P(ov00_0221A748, 0), 0x1c1c), 0) = 0;
    }
    return 1;
}
