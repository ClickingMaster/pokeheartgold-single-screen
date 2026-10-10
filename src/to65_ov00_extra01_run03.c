#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define H(p, o) (*(u16 *)((char *)(p) + (o)))
#define B(p, o) (*(u8 *)((char *)(p) + (o)))

typedef struct {
    int a, b;
    u16 c, pad;
    int d;
} Record;

extern int ArrayLength(void *);
extern int ov00_021FF368(void);
extern int ov00_021FE754(void *, void *);
extern void *ov00_021FE6EC(void *, int);
extern int ov00_0220BF70(void *);
extern void ov00_0220BF80(void *, int);
extern void ov00_0220BFD0(void *, int);
extern void ov00_0220C03C(void *, void *, int);
extern int ov00_0220D404(void *);
extern int ov00_0220CC38(void *, void *, int);
extern void ov00_0220D338(void *, int, int);
extern void ov00_0220C0C4(void *, int, int);
extern void ov00_0220CF14(void *);
extern void *ov00_021FF0C0(void *, void *);
extern void *ov00_021EC3A8(int);
extern void ov00_021EC3D8(void *);
extern void ov00_021FF818(int);
extern void ov00_021FEE8C(void *);
extern void ov00_021FE678(void *);
extern void ov00_021FF3D0(void);
extern int ov00_0220CE00(void *, int);
extern void ov00_0220D238(void *);
extern int ov00_0220C164(void *);
extern int ov00_021FF264(void *, void *, void *);
extern void ov00_021FE8D8(void *, int);
extern void *ov00_021FE5B4(int, int, void *);
extern int ov00_021FFA18(int, void *, int *);
extern int ov00_021FFA44(char *);
extern void *SOC_GetHostByName(char *);
extern u8 ov00_02219CB4[];
extern u8 ov00_02219CB8[];
extern u8 ov00_02219CC0[];
extern u8 ov00_02219C94[];
extern u8 ov00_02219D24[];
extern u8 ov00_02219D28[];
extern char ov00_02219D30[];
extern char ov00_0221B16C[];
extern char ov00_02219D38[];
extern char ov00_02219D54[];
extern int ov00_0221B664[];
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void ov00_0220F3B0(void *);
extern int ov00_0220F474(void *, int, int, void *, int);
extern void ov00_022101D4(void *);
extern int snprintf(char *, unsigned int, const char *, ...);

extern int ov00_021FF804(int, int, int);
extern void ov00_0220FC7C(void *);
extern void ov00_0220FEC4(void *);
extern int ov00_0221B0D0[];
extern int ov00_0221B634;
extern char ov00_0221B638[];
extern char ov00_02219CE4[];
extern char ov00_02219CEC[];
extern char ov00_02219CF0[];
extern char *SOC_InetNtoA(int);
extern int sprintf(char *, const char *, ...);
extern int ov00_021FF984(int, void *, int, int, void *, int);
extern void *ov00_021FF6E4(void);
extern int ov00_021FF770(void *);

typedef struct {
    int a;
    u16 c, pad;
    int d;
} Key;

extern void ov00_021FE968(void *, int);
extern void ov00_021FF01C(void *, void *);

#define SW16(x) ((((x) >> 8) & 255) | (((x) << 8) & 0xff00))
#define SW32(x) ((((x) >> 24) & 255) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | (((x) << 24) & 0xff000000))
typedef void (*Notice)(int, int, void *, int);
typedef void (*State)(int, int);
extern void ov00_02210408(void *, void *);
int ov00_0220E6C0(void *s, int a, int b);
int ov00_0220E734(void *s, int a, int n, int *out);
int ov00_0220E7F8(void *s);
int ov00_0220E86C(void *s, void *a, int n);
int ov00_0220E8E0(void *s, void *a);
int ov00_0220E940(void *s, void *a, void *b);
int ov00_0220E9CC(void *s, void *a, void *b, int n);
int ov00_0220EA44(void *s);
int ov00_0220EAA0(void *s, void *a, int n);
int ov00_0220EB14(void *s);
int ov00_0220EB70(void *s);
int ov00_0220EBCC(void *s, void *buf, int n);
int ov00_0220EC94(void *s);
int ov00_0220ECF8(void *s, int a, int b);
int ov00_0220ED84(void *s, u8 *b, int n);
int ov00_0220ED98(void *p);
int ov00_0220EDB4(void *s, int a, int b);
int ov00_0220EE00(void *s, Record *r);
int ov00_0220EE6C(void *s, void *b, int n, int flag);
u32 ov00_0220EE88(Key **p, u32 n);
int ov00_0220EEA8(Key **a, Key **b);
void ov00_0220EEDC(void **p);
void *ov00_0220EEEC(void *s, int a, int b);
void ov00_0220F144(void *s);
void ov00_0220F188(void *s, int a);
void *ov00_0220F190(void);
int ov00_0220F61C(void **p, int *a);
int ov00_0220F674(void *s);
void ov00_0220F6B0(void *s);
void ov00_0220F6E8(void *s);
void ov00_0220F958(char **s, int *n);
void *ov00_0220F9A8(int id);
void ov00_0220FA10(void *s);
void *ov00_0220FA40(void);
void ov00_0220FAD8(void *s);
void ov00_0220FB40(void);
int ov00_0220FB6C(void *s);
u16 ov00_0220FC48(int sock);
int ov00_0220FFE4(char *s);
int ov00_0221001C(char *s, char *a);
int ov00_0221005C(void);
void ov00_0221019C(int id);
void ov00_022103C4(void);
char *ov00_0220F724(int ip, int port, char *buf);
int ov00_0220FB90(int sock, int ip, int port, void *buf, int n);
int ov00_0220FBE4(void);
int ov00_022100D4(int a, int b, int c, int d, int e, int f);
void ov00_0220F3B0(void *s);
void ov00_0220FEC4(void *s);
void ov00_02210408(void *s, void *a);
void ov00_022104D4(void *s, void *p, void *a);
void ov00_02210590(void *s, void *p, void *a);
void ov00_02210658(void *s, u8 *p, void *a);
extern int ov00_0220BF44(void *, int);
extern void ov00_021FEF74(void *, void *);
int ov00_0220F1A0(void *, void **, int, int);
extern void ov00_021FF3CC(void);
extern int ov00_0220F7D4(char *, int *, u16 *);
extern void *ov00_021FED4C(int, int, int, void *, void *, void *);
extern int ov00_021FF840(int, void *, int);
int ov00_0220EF24(void **, char *, int, int, int);

void ov00_0220F144(void *s) {
    if (W(s, 0x1c)) {
        W(s, 0x14) = 1;
        return;
    }
    ov00_021FF818(W(s, 0));
    ov00_021FEE8C(P(s, 0xc));
    ov00_021FE678(P(s, 0x10));
    ov00_021EC3D8(s);
    ov00_021FF3D0();
}

void ov00_0220F188(void *s, int a) {
    W(s, 0x20) = a;
}

void *ov00_0220F190(void) {
    return ov00_021EC3A8(0xa0);
}

int ov00_0220F1A0(void *parent, void **out, int a, int b) {
    void *s = 0;
    if (ov00_0220EEEC(parent, a, b)) {
        return 5;
    }
    s = ov00_0220F190();
    if (s) {
        memset(s, 0, 0xa0);
        W(s, 0) = a;
        H(s, 4) = b;
        P(s, 8) = parent;
        W(s, 0x1c) = ov00_021FF368();
        W(s, 0x88) = W(s, 0x1c);
        H(s, 0x64) = 0;
        H(s, 0x66) = 0;
        if (!ov00_0220BF44((char *)s + 0x44, W(parent, 0x3c))) {
            goto fail;
        }
        if (!ov00_0220BF44((char *)s + 0x50, W(parent, 0x38))) {
            goto fail;
        }
        P(s, 0x5c) = ov00_021FE5B4(0x10, 0x40, 0);
        if (!P(s, 0x5c)) {
            goto fail;
        }
        P(s, 0x60) = ov00_021FE5B4(0x10, 0x40, 0);
        if (!P(s, 0x60)) {
            goto fail;
        }
        P(s, 0x98) = ov00_021FE5B4(4, 2, 0);
        if (!P(s, 0x98)) {
            goto fail;
        }
        P(s, 0x9c) = ov00_021FE5B4(4, 2, 0);
        if (!P(s, 0x9c)) {
            goto fail;
        }
        ov00_021FEF74(P(parent, 0xc), &s);
        *out = ov00_0220EEEC(parent, a, b);
        if (*out) {
            return 0;
        }
    }
fail:
    if (s) {
        ov00_021EC3D8(P(s, 0x44));
        ov00_021EC3D8(P(s, 0x50));
        if (P(s, 0x5c)) {
            ov00_021FE678(P(s, 0x5c));
        }
        if (P(s, 0x60)) {
            ov00_021FE678(P(s, 0x60));
        }
        if (P(s, 0x98)) {
            ov00_021FE678(P(s, 0x98));
        }
        if (P(s, 0x9c)) {
            ov00_021FE678(P(s, 0x9c));
        }
        ov00_021EC3D8(s);
    }
    return 1;
}

void ov00_0220F3B0(void *s) {
    int n, i;
    if (W(s, 0x14) || W(s, 0x24)) {
        return;
    }
    if (W(s, 0xc) == 7) {
        n = ArrayLength(P(P(s, 8), 0x10));
        for (i = 0; i < n; i++) {
            if (s == *(void **)ov00_021FE6EC(P(P(s, 8), 0x10), i)) {
                ov00_021FE968(P(P(s, 8), 0x10), i);
                return;
            }
        }
    } else {
        ov00_021FF01C(P(P(s, 8), 0xc), &s);
    }
}
