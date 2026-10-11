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
extern char *strtok(char *, const char *);
extern char *ov00_021FF3D4(char *);
extern void *ov00_021EC3BC(void *, int);
extern void ov00_021EC3D8(void *);
extern char *strchr(const char *, int);
extern int ov45_02244D30();
extern int ov45_022474FC();
extern int ov45_02249554();
extern int ov45_0224966C();
extern int ov45_0224A630();
extern int ov45_02251A8C();
extern int ov45_02252CB4();
extern char ov45_02256024[];
extern char ov45_02256038[];
extern char ov45_02256074[];
extern char ov45_02256114[];
extern char ov45_02256170[];
extern char ov45_022561A0[];
extern char ov45_022561E8[];
extern char ov45_02256218[];
extern char ov45_02256230[];
extern char ov45_02256248[];
extern char ov45_02256260[];
extern char ov45_0225627C[];
extern char ov45_02256298[];
extern char ov45_022562EC[];
extern char ov45_02256308[];
extern char ov45_0225635C[];
extern char ov45_02256378[];
extern char ov45_02256394[];
extern char ov45_022563B0[];
extern char ov45_022563D0[];
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_022566F0[];
extern char ov45_02256708[];
extern char ov45_0225680C[];
extern char ov45_02256824[];
extern char ov45_02256848[];
extern char ov45_022568C4[];
extern char ov45_022568DC[];
extern char ov45_02256964[];
extern char ov45_02256990[];
extern char ov45_022569A8[];
extern char ov45_022569B4[];
extern char ov45_022569CC[];
extern char ov45_02256A54[];
extern char ov45_02256B24[];
extern char ov45_02256B84[];
extern char ov45_02256B94[];
extern char ov45_02256BA0[];
extern char ov45_02256BC0[];
extern char ov45_02256BC4[];
extern char ov45_02256BCC[];
extern char ov45_02256BE0[];
extern char ov45_02256BEC[];
extern char ov45_02256C00[];
extern char ov45_02256C10[];
extern char ov45_02256C28[];
void ov45_0224EEBC(u8 *p, u8 *q);
void ov45_0224EF70(u8 *p, u8 *q);
void ov45_0224F024(u8 *p, u8 *q);
void ov45_0224F0D8(u8 *p, u8 *q);
void ov45_0224F18C(u8 *p, u8 *q);
void ov45_0224F240(u8 *p, u8 *q);
void ov45_0224D924(u8 *p, u8 *q);
void ov45_0224DF50(u8 *p);
void ov45_0224E210(u8 *p);
void ov45_0224E968(u8 *p, u8 *q);
void ov45_0224EC14(u8 *p, u8 *q);
void ov45_0224EA2C(u8 *p, u8 *q);
void ov45_0224ECCC(u8 *p, u8 *q);
void ov45_0224F2F4(void);
void ov45_0224F2F8(u8 *p);
void ov45_0224F3C0(u8 *p);
void ov45_0224F428(u8 *p);
void ov45_0224F35C(u8 *p);
int ov45_0224F660(u8 *p, u8 *q);
void ov45_0224F934(u8 *p);
void ov45_0224F9A0(u8 *p);
void ov45_0224F860(u8 *p);
void ov45_0224DE70(u8 *p, u8 *q);
int ov45_0224F5BC(u8 *p, u8 *q);
void ov45_0224F69C(u8 *p, int x);
void ov45_0224F9B8(u8 *p);
void ov45_0224EAF4(u8 *p, u8 *q);
void ov45_0224E734(u8 *p, u8 *q);
void ov45_0224E5EC(u8 *p, u8 *q);

void ov45_0224DE70(u8 *p, u8 *q) {
    char buf[128], *s;
    if (!p) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02256074, 3517);
    }
    if (W(q, 36) < 1) {
        __msl_assertion_failed(ov45_022568C4, ov45_022565D0, ov45_02256074, 3523);
    }
    if (W(q, 36) < 1) {
        return;
    }
    s = strchr((char *)P(P(q, 32), (W(q, 36) - 1) * 4), 64);
    if (s && P(p, 16)) {
        int x = ov00_021FFA44(s + 1);
        (*(NameCB *)(p + 16))(p, x, buf, P(p, 24));
        strncpy((char *)p + 0x42c, buf, 128);
        p[0x4ab] = 0;
    }
    ov45_0224F9A0(p);
}

void ov45_0224DF50(u8 *p) {
    K k = { 0 };
    u8 *r, *s;
    k.type = 0;
    r = ov45_022496AC(p, 1, &k);
    if (!r) {
        return;
    }
    s = P(r, 28);
    if (!s) {
        __msl_assertion_failed(ov45_022568DC, ov45_022565D0, ov45_022561A0, 3567);
    }
    W(s, 0) = 1;
}
