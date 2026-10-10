#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Child {
    int id, pad;
    void *a, *b;
} Child;

typedef struct Lookup {
    int id, pad;
    Child *child;
} Lookup;

typedef struct Root {
    char pad[0x198];
    int number;
    char pad2[0x58];
    int request;
    char pad3[0x238];
    int count;
} Root;

typedef struct Context {
    Root *root;
} Context;

extern char ov00_022182FC[];
extern char ov00_02218310[];
extern char ov00_0221823C[];
extern char ov00_0221831C[];
extern char ov00_02218260[];
extern char ov00_0221832C[];
extern char ov00_02218338[];
extern char ov00_02218220[];
extern char ov00_02218344[];
extern int ov00_02207DD0(Context *, int, Lookup **);
extern int ov00_02207FB8(Lookup *);
extern void ov00_0220A890(Context *, char *), ov00_022020EC(Context *, void *, char *), ov00_0220211C(Context *, void *, int), ov00_021EC3D8(void *), ov00_02207E40(Context *, Lookup *), ov00_02201D78(void), ov00_02207F1C(Context *, void (*)(void), int);
extern void __msl_assertion_failed(char *, char *, char *, int);
int ov00_02201D9C(Context *, int);

int ov00_02201D9C(Context *c, int id) {
    Root *r = c->root;
    Lookup *l;
    if (!ov00_02207DD0(c, id, &l)) {
        ov00_0220A890(c, ov00_022182FC);
        return 2;
    }
    ov00_022020EC(c, &r->request, ov00_02218310);
    ov00_022020EC(c, &r->request, ov00_0221823C);
    ov00_0220211C(c, &r->request, r->number);
    ov00_022020EC(c, &r->request, ov00_0221831C);
    ov00_0220211C(c, &r->request, l->id);
    ov00_022020EC(c, &r->request, ov00_02218260);
    if (l->child) {
        int saved = l->child->id;
        if (saved < 0) {
            __msl_assertion_failed(ov00_0221832C, ov00_02218338, ov00_02218220, 0x1fd);
        }
        ov00_021EC3D8(l->child->a);
        l->child->a = 0;
        ov00_021EC3D8(l->child->b);
        l->child->b = 0;
        ov00_021EC3D8(l->child);
        l->child = 0;
        if (ov00_02207FB8(l)) {
            ov00_02207E40(c, l);
        }
        if (--r->count < 0) {
            __msl_assertion_failed(ov00_02218344, ov00_02218338, ov00_02218220, 0x204);
        }
        ov00_02207F1C(c, ov00_02201D78, saved);
    }
    return 0;
}
