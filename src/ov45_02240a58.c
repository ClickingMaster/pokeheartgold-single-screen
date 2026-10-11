#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define U(p, n) (*(unsigned *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

typedef union {
    struct {
        u32 flag : 1;
        u32 pad : 31;
        u32 len;
        char *data;
    } b;

    struct {
        char head;
        char inlinebuf[11];
    } s;
} Str;

extern void *ov45_022577DC;
extern char ov45_022552DC[];
extern char ov45_022552E4[];
extern char ov45_022552EC[];
extern char ov45_022552F0[];
extern char ov45_022552FC[];
void ov45_02237B58(Str *, char *);
void ov45_02237BC8(Str *);
void ov45_02237A24(void *, void *);
void ov45_022376BC(void *);
void ov45_02232D8C(void *, int *, int);
void ov45_02242858(void *);
void ov00_021EC2EC(int, void *, int);
void ov45_022436E0(void *, Str *, Str *);
void ov45_0224335C(void *, Str *);
void ov45_0223DAD4(Str *, char *);
void *ov45_02242C3C(void *, Str *);
void ov45_022428D8(Str *, void *, Str *);
void ov45_022428C8(Str *, void *, Str *);
int ov45_02251024(void *, char *);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void *ov45_02242394(unsigned);
void *ov45_0223EA34(void *, unsigned, char *);
void ov45_022421E4(void *, unsigned, void *);
char *strncpy(char *, const char *, size_t);
int ov45_022435F8(void *, Str *);
void *ov45_0223EC64(void *p);
void ov45_0223EC80(void *p, unsigned n);
void *ov45_0223ECB0(void *p);
void ov45_0223ED00(void);
void ov45_0223F1C8(void);
void ov45_0223F8F4(void);
void ov45_0223F910(void);
void ov45_0223ED1C(void);
void ov45_0223F2D8(void *a, char *s);
void ov45_0223F6F4(void *a, void *b, char *c, char *d);
void ov45_0223F768(void *a, void *b, void *c, unsigned mode);
void ov45_0223F83C(void *a, char *s, void *b);
void ov45_0223F92C(void *a, int n);
void ov45_0223F9C4(void *a, void *b, char *out);
void ov45_0223FA2C(void *out, void *p);
void ov45_0223FC5C(void *out, void *p);
void ov45_02240F6C(void *out, void *p);
void ov45_02240F7C(void *out, void *p);
int ov45_0223FD6C(void *p);
void *ov45_0223FD74(void *p);
void ov45_0223FD88(void);
void *ov45_0223FDE8(void *c, void *key, void *node, void *best);
void ov45_0223FD8C(void **out, void *tree, void *key);
void ov45_0223FE20(void *a, int n);
void ov45_0223FFE8(void *p, u32 *first, u32 *last);
int ov45_02240850(void);
int ov45_02240A58(void);
void *ov45_02240A74(void *p);
int ov45_02240A80(char *s);
void ov45_0223F1E4(void *a, char *s, char *name, void *arg, int mode);
void ov45_02240620(void *a, char *s, char *name, void *arg, void *extra);

int ov45_02240A58(void) {
    return ov45_022577DC ? W(ov45_022577DC, 0x58) : 0;
}

void *ov45_02240A74(void *p) {
    return (u8 *)p + 0xa410;
}

int ov45_02240A80(char *s) {
    Str x, y;
    int r;
    if (!ov45_022577DC) {
        return 0;
    }
    ov45_02237B58(&x, s);
    ov45_022428C8(&y, ov45_022577DC, &x);
    r = ov45_02251024(P(ov45_022577DC, 0), !y.b.flag ? y.s.inlinebuf : y.b.data) == 1;
    ov45_02237BC8(&y);
    ov45_02237BC8(&x);
    return r;
}
