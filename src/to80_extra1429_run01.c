#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Entry {
    int base;
    u8 p[12];
    int type, off;
};

struct Node {
    u8 p[12];
    int key;
    u8 q[40];
    void *array;
    struct Node *next;
};

struct State {
    u8 p[0x434];
    struct Node *head;
};

struct Work {
    struct State *s;
};

extern char ov00_02219118[];
extern char ov00_022190A8[];
extern char ov00_02218F50[];
extern char ov00_022190A4[];
extern char ov00_02219128[];
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void ov00_0220A3BC(struct Work *, char *);
extern void ov00_02201AE8(struct Work *, int, int, int);
extern void ov00_022071F4(struct Work *, struct Node *);
extern int ArrayLength(void *);
extern struct Entry *ov00_021FE6EC(void *, int);
extern void ov00_021FE968(void *, int);
void ov00_02207254(struct Work *, struct Node *);

void ov00_02207254(struct Work *w, struct Node *n) {
    struct State *s = w->s;
    struct Node *p, *next;
    int i, j;
    if (!n) {
        __msl_assertion_failed(ov00_02219118, ov00_022190A8, ov00_02218F50, 0x213);
    }
    p = s->head;
    if (p == n) {
        s->head = n->next;
    } else {
        next = p->next;
        while (next != n) {
            if (!next) {
                __msl_assertion_failed(ov00_022190A4, ov00_022190A8, ov00_02218F50, 0x225);
                ov00_0220A3BC(w, ov00_02219128);
                return;
            }
            p = next;
            next = next->next;
        }
        p->next = n->next;
    }
    if (ArrayLength(n->array)) {
        i = 0;
        j = 0;
        do {
            struct Entry *e = ov00_021FE6EC(n->array, j);
            if (e->type < 100) {
                ov00_02201AE8(w, n->key, e->type, e->base + e->off);
            }
            ov00_021FE968(n->array, i);
        } while (ArrayLength(n->array));
    }
    ov00_022071F4(w, n);
}
