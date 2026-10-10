#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 p[0x28];
    s8 a[31], b[21], c[51], d[31], e[31];
    u8 p2[0x130 - 0xcd];
    void *value;
    int number;
} Entry;

extern char ov00_02219418[];
extern void ov00_0220A890(void *, char *);
extern void ov00_0220A348(void *, void *, int);
extern void ov00_021FF408(void *);
extern int ov00_022081F0(void *, Entry **, int);
extern int ov00_022082A0(void *, Entry *, void *, void *, void *);

int ov00_02208330(void *w, s8 *a, s8 *b, s8 *c, s8 *d, s8 *e, void *value, int n, void *f, void *g, void *h);

int ov00_02208330(void *w, s8 *a, s8 *b, s8 *c, s8 *d, s8 *e, void *value, int n, void *f, void *g, void *h) {
    Entry *p;
    int result;
    if ((!a || !*a) && (!c || !*c) && (!d || !*d) && (!e || !*e) && !value && (!b || !*b)) {
        ov00_0220A890(w, ov00_02219418);
        return 2;
    }
    result = ov00_022081F0(w, &p, 1);
    if (result != 0) {
        return result;
    }
    if (a == 0) {
        p->a[0] = 0;
    } else {
        ov00_0220A348(p->a, a, 31);
    }
    if (b == 0) {
        p->b[0] = 0;
    } else {
        ov00_0220A348(p->b, b, 21);
    }
    if (c == 0) {
        p->c[0] = 0;
    } else {
        ov00_0220A348(p->c, c, 51);
    }
    ov00_021FF408(p->c);
    if (d == 0) {
        p->d[0] = 0;
    } else {
        ov00_0220A348(p->d, d, 31);
    }
    if (e == 0) {
        p->e[0] = 0;
    } else {
        ov00_0220A348(p->e, e, 31);
    }
    p->value = value;
    if (n < 0) {
        n = 0;
    }
    p->number = n;
    result = ov00_022082A0(w, p, f, g, h);
    if (result == 0) {
        return 0;
    }
    return result;
}
