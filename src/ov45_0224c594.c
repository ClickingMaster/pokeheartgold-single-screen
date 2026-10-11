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

void ov45_0224C594(u8 *p, u8 *q) {
    if (!p) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255F64, 2289);
    }
    if (W(q, 36) != 1) {
        __msl_assertion_failed(ov45_0225671C, ov45_022565D0, ov45_02255F64, 2295);
    }
    if (W(q, 36) != 1) {
        return;
    }
    ov45_0224F69C(p, P(P(q, 32), 0));
}

void ov45_0224C610(u8 *p, u8 *q) {
    void *x, *y;
    void *out[2];
    if (!p) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255FB4, 2308);
    }
    if (W(q, 36) != 2) {
        __msl_assertion_failed(ov45_022566F0, ov45_022565D0, ov45_02255FB4, 2314);
    }
    if (W(q, 36) != 2) {
        return;
    }
    y = P(q, 8);
    x = P(P(q, 32), 4);
    if (!P(p, 0x804)) {
        return;
    }
    out[0] = x;
    out[1] = y;
    ov45_02244D30(p, 3, P(p, 0x804), out, W(p, 0x808), 0, 0, 8);
}
