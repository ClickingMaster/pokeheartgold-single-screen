#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int ov45_0224739C(void *, int);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern const char ov45_0225590C[];
extern const char ov45_02255918[];
extern const char ov45_02255668[];
extern const char ov45_02255928[];
extern u8 __lower_mapC[];
int ov45_022474FC(void *, int), ov45_02247518(const signed char *, int);

typedef struct {
    int kind;
    void *callback, *data;
    int value, key;
    char *label;
} Node;

typedef struct {
    u8 p[0x820];
    void *array;
} Collection;

extern char ov45_022554F0[];
extern char ov45_02255468[];
extern char ov45_02255420[];
extern char ov45_02255458[];
extern char ov45_02255478[];
extern char ov45_02255488[];
extern char ov45_022554A4[];
extern char ov45_022554BC[];
extern char ov45_022554DC[];
extern int ArrayLength(void *);
extern Node *ov00_021FE6EC(void *, int);
int ov45_0224739C(void *, int);

static inline void assertNode(Node *n) {
    if (!n) {
        __msl_assertion_failed(ov45_02255458, ov45_02255468, ov45_02255420, 0x648);
    }
    if (n->kind < 0) {
        __msl_assertion_failed(ov45_02255478, ov45_02255468, ov45_02255420, 0x648);
    }
    if (n->kind >= 32) {
        __msl_assertion_failed(ov45_02255488, ov45_02255468, ov45_02255420, 0x648);
    }
    if (!n->callback) {
        __msl_assertion_failed(ov45_022554A4, ov45_02255468, ov45_02255420, 0x648);
    }
    if (!n->data) {
        __msl_assertion_failed(ov45_022554BC, ov45_02255468, ov45_02255420, 0x648);
    }
    if (n->key < 0) {
        __msl_assertion_failed(ov45_022554DC, ov45_02255468, ov45_02255420, 0x648);
    }
}

extern char ov45_022553E8[];
extern int ov45_02248238(Collection *, const char *);
extern int ov45_0224861C(Collection *, const char *);
extern void ov45_02244500(Node *);
extern void ov00_021FE968(void *, int);
extern void ov45_02246BA4(Collection *, Node *);
void ov45_022471A0(Collection *, int);

static inline void assertQueueNode(Node *n) {
    if (!n) {
        __msl_assertion_failed(ov45_02255458, ov45_02255468, ov45_022553E8, 0x605);
    }
    if (n->kind < 0) {
        __msl_assertion_failed(ov45_02255478, ov45_02255468, ov45_022553E8, 0x605);
    }
    if (n->kind >= 32) {
        __msl_assertion_failed(ov45_02255488, ov45_02255468, ov45_022553E8, 0x605);
    }
    if (!n->callback) {
        __msl_assertion_failed(ov45_022554A4, ov45_02255468, ov45_022553E8, 0x605);
    }
    if (!n->data) {
        __msl_assertion_failed(ov45_022554BC, ov45_02255468, ov45_022553E8, 0x605);
    }
    if (n->key < 0) {
        __msl_assertion_failed(ov45_022554DC, ov45_02255468, ov45_022553E8, 0x605);
    }
}

static inline Node *getQueueNode(Collection *c, int i) {
    Node *n = ov00_021FE6EC(c->array, i);
    assertQueueNode(n);
    return n;
}

void ov45_022471A0(Collection *c, int key) {
    Node *n;
    int i;
    Node copy;
    if (!c) {
        __msl_assertion_failed(ov45_022554F0, ov45_02255468, ov45_022553E8, 0x5fc);
    }
    i = 0;
    while (i < ArrayLength(c->array)) {
        n = ov00_021FE6EC(c->array, i);
        assertQueueNode(n);
        if (n->label && !ov45_02248238(c, n->label)) {
            ov45_02244500(n);
            ov00_021FE968(c->array, i);
        } else if ((!n->label || ov45_0224861C(c, n->label)) && (!key || n->key == key)) {
            copy = *n;
            ov00_021FE968(c->array, i);
            ov45_02246BA4(c, &copy);
            if (key) {
                return;
            }
        } else {
            ++i;
        }
    }
}

static inline Node *getSearchNode(Collection *c, int i) {
    Node *n = ov00_021FE6EC(c->array, i);
    assertNode(n);
    return n;
}
