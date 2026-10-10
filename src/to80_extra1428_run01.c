#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *fn;
    void *arg;
} Pair;

typedef struct {
    u8 pad[0x1a0];
    void *saved;
    u8 pad2[0x274];
    int type;
} D;

typedef struct {
    D *d;
} C;

typedef struct {
    int type;
    u8 pad[8];
    Pair cb;
    u8 pad2[8];
    int value;
} T;

typedef struct {
    int value;
    void *saved;
    u32 rest[6];
} A;

typedef struct {
    int value;
    u32 rest;
} B;

typedef struct {
    int value;
    u8 rest[512];
} E;

typedef struct {
    int value;
    u32 a, b, last;
} F;

extern char ov00_02218E88[];
extern char ov00_02218E9C[];
extern char ov00_02218E60[];
extern char ov00_02218EAC[];
extern char ov00_02218EC0[];
extern char ov00_02218ED4[];
extern char ov00_02218EE4[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov00_0220A890(C *, char *);
extern void *ov00_021EC3A8(int);
extern int ov00_02202A84(C *, Pair, void *, T *, int);
int ov00_02206230(C *, T *);

int ov00_02206230(C *c, T *t) {
    D *d = c->d;
    Pair cb;
    if (!c) {
        __msl_assertion_failed(ov00_02218E88, ov00_02218E9C, ov00_02218E60, 34);
    }
    if (!c->d) {
        __msl_assertion_failed(ov00_02218EAC, ov00_02218E9C, ov00_02218E60, 35);
    }
    if (!t) {
        __msl_assertion_failed(ov00_02218EC0, ov00_02218E9C, ov00_02218E60, 36);
    }
    cb = t->cb;
    if (cb.fn) {
        switch (t->type) {
        case 0: {
            A *p = ov00_021EC3A8(sizeof(A));
            if (!p) {
                ov00_0220A890(c, ov00_02218ED4);
                return 1;
            }
            memset(p, 0, sizeof(A));
            p->value = t->value;
            if (d->type == 513) {
                p->saved = d->saved;
                d->saved = 0;
            }
            {
                int result = ov00_02202A84(c, cb, p, t, 0);
                if (result) {
                    return result;
                }
            }
            break;
        }
        case 1: {
            B *p = ov00_021EC3A8(sizeof(B));
            if (!p) {
                ov00_0220A890(c, ov00_02218ED4);
                return 1;
            }
            memset(p, 0, sizeof(B));
            p->value = t->value;
            {
                int result = ov00_02202A84(c, cb, p, t, 0);
                if (result) {
                    return result;
                }
            }
            break;
        }
        case 2: {
            E *p = ov00_021EC3A8(sizeof(E));
            if (!p) {
                ov00_0220A890(c, ov00_02218ED4);
                return 1;
            }
            memset(p, 0, sizeof(E));
            p->value = t->value;
            {
                int result = ov00_02202A84(c, cb, p, t, 0);
                if (result) {
                    return result;
                }
            }
            break;
        }
        case 3: {
            F *p = ov00_021EC3A8(sizeof(F));
            if (!p) {
                ov00_0220A890(c, ov00_02218ED4);
                return 1;
            }
            memset(p, 0, sizeof(F));
            p->value = t->value;
            p->last = 0;
            {
                int result = ov00_02202A84(c, cb, p, t, 0);
                if (result) {
                    return result;
                }
            }
            break;
        }
        case 4: {
            int *p = ov00_021EC3A8(4);
            if (!p) {
                ov00_0220A890(c, ov00_02218ED4);
                return 1;
            }
            memset(p, 0, 4);
            *p = t->value;
            {
                int result = ov00_02202A84(c, cb, p, t, 0);
                if (result) {
                    return result;
                }
            }
            break;
        }
        default:
            __msl_assertion_failed(ov00_02218EE4, ov00_02218E9C, ov00_02218E60, 107);
        }
    }
    return 0;
}
