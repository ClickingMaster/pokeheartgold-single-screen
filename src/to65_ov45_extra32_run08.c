#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, o) (*(int *)((p) + (o)))
#define P(p, o) (*(u8 **)((p) + (o)))
extern int ov00_021FFA44(char *);
extern void ov00_021FF31C(void *, int, void *);
extern int strnicmp(const char *, const char *);
extern int ov45_02255DA0;
extern char ov45_02256410[];
#define V(p, o) (*(volatile int *)((p) + (o)))
typedef void (*PairCB)(u8 *, u8 *);
typedef void (*NameCB)(u8 *, int, char *, void *);

typedef struct K {
    int type;
    void *key;
    int z;
} K;

typedef void (*CB)(u8 *, int, int, void *);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern u8 *ov45_022496AC(u8 *, int, K *);
extern void ov45_02249A0C(u8 *, u8 *, void *);
extern void ov45_02251B0C(u8 *, int, void *, int, int);
extern void ov45_02252F74(void *, const char *, ...);
extern long atol(const char *);
extern void *ov00_021EC3A8(int);
extern char *ov00_021FF3D4(char *);
extern void ov00_021EC3D8(void *);
extern u8 *ov45_02247E98(u8 *, void *);
extern void ov45_02248CE8(u8 *, void *, void (*)(), void *);
extern char *strtok(char *, const char *);
extern char *ov00_021FF3D4(char *);
extern void *ov00_021EC3BC(void *, int);
extern void ov00_021EC3D8(void *);
extern char *strchr(const char *, int);
extern int ov45_02244D30();
extern int ov45_02248558();
extern int ov45_0224861C();
extern int ov45_022486E0();
extern int ov45_02248A98();
extern int ov45_02249054();
extern int ov45_0224F69C();
extern char ov45_02255E84[];
extern char ov45_02255E94[];
extern char ov45_02255EA4[];
extern char ov45_02255ED4[];
extern char ov45_02255EE4[];
extern char ov45_02255F04[];
extern char ov45_02255F24[];
extern char ov45_02255F34[];
extern char ov45_02255F54[];
extern char ov45_02255F64[];
extern char ov45_02255FB4[];
extern char ov45_0225604C[];
extern char ov45_02256060[];
extern char ov45_022560D8[];
extern char ov45_02256100[];
extern char ov45_02256158[];
extern char ov45_02256188[];
extern char ov45_022561D0[];
extern char ov45_02256324[];
extern char ov45_02256340[];
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_022566F0[];
extern char ov45_02256708[];
extern char ov45_02256714[];
extern char ov45_0225671C[];
extern char ov45_02256734[];
extern char ov45_022567A0[];
extern char ov45_022567B0[];
extern char ov45_022567C0[];
extern char ov45_022567D0[];
extern char ov45_022567E4[];
extern char ov45_022567F4[];
extern char ov45_0225680C[];
extern char ov45_02256848[];
extern char ov45_02256860[];
extern char ov45_02256864[];
extern char ov45_02256894[];
extern char ov45_0225689C[];
void ov45_0224B748(u8 *p, u8 *q);
void ov45_0224C018(u8 *p, u8 *q);
void ov45_0224C244(u8 *p, u8 *q);
void ov45_0224C594(u8 *p, u8 *q);
void ov45_0224C2A8(u8 *p, u8 *q);
void ov45_0224C610(u8 *p, u8 *q);
void ov45_0224CBAC(u8 *p, u8 *q);
void ov45_0224CCC8(u8 *p, u8 *q);
void ov45_0224CDE0(u8 *p, u8 *q);
void ov45_0224D128(u8 *p, u8 *q);
char *ov45_0224D240(char *p, int *len);
void ov45_0224D4BC(u8 *p, u8 *q);
void ov45_0224B474(u8 *p, u8 *q);
void ov45_0224B5E0(u8 *p, u8 *q);
void ov45_0224B794(u8 *p, u8 *q);
void ov45_0224BE74(u8 *p, char *x, char *y, char *z);
void ov45_0224C0A0(u8 *p, char *x, char *y, char *z);
void ov45_0224BB4C(u8 *p, u8 *q);
void ov45_0224C9D8(u8 *p, u8 *q);
void ov45_0224D30C(u8 *p, u8 *q);

void ov45_0224D128(u8 *p, u8 *q) {
    void *x;
    u8 *r;
    if (W(q, 36) != 3) {
        __msl_assertion_failed(ov45_02256848, ov45_022565D0, ov45_02256158, 2829);
    }
    if (W(q, 36) != 3) {
        return;
    }
    x = P(P(q, 32), 4);
    K k[2] = { 0 };
    void *out[4];
    void *other[5];
    k[0].type = 10;
    k[0].key = x;
    k[1].type = 11;
    k[1].key = x;
    r = ov45_022496AC(p, 2, k);
    if (!r) {
        return;
    }
    if (W(r, 0) == 10) {
        out[0] = 0;
        out[1] = x;
        out[2] = 0;
        out[3] = 0;
        ov45_02249A0C(p, r, out);
        return;
    }
    if (W(r, 0) == 11) {
        other[0] = (void *)1;
        other[1] = x;
        other[2] = 0;
        other[3] = 0;
        other[4] = 0;
        ov45_02249A0C(p, r, other);
    }
}

char *ov45_0224D240(char *p, int *len) {
    int n;
    char *out;
    if (!p) {
        __msl_assertion_failed(ov45_02256894, ov45_022565D0, ov45_02255E84, 2888);
    }
    if (!len) {
        __msl_assertion_failed(ov45_0225689C, ov45_022565D0, ov45_02255E84, 2889);
    }
    if (!p || *p != 92) {
        return 0;
    }
    n = 0;
    while (p[n + 1] && p[n + 1] != 92) {
        n++;
    }
    out = ov00_021EC3A8(n + 1);
    if (!out) {
        return 0;
    }
    memcpy(out, p + 1, n);
    out[n] = 0;
    *len = n + 1;
    return out;
}
