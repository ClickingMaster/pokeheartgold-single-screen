#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
typedef struct Node Node;

struct Node {
    int type;
    void *base, *a, *b;
    int c, d, e;
    void *data;
    u32 id;
    Node *next;
};

typedef struct {
    u8 p[0x814];
    Node *head, *tail;
} Work;

#include "to90_ov45_input_private.h"
typedef Ov45Payload84 Payload;
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_02255E6C[];
extern char ov45_022566A8[];
extern char ov45_02255F94[];
extern char ov45_022566BC[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void *ov00_021EC3A8(u32);
extern void *ov00_021FF368(void);
extern void *ov00_021FF3D4(void *);
extern void ov00_021EC3D8(void *);
extern u32 ov45_0224A5E4(Work *);
u32 ov45_0224A708(Work *, int, void *, void *, int, int, int, void *);
u32 ov45_0224A7F0(Work *, int, int, int);

u32 ov45_0224A978(Work *, void *, int, int);

u32 ov45_0224A708(Work *w, int type, void *a, void *b, int c, int d, int e, void *data) {
    Node *n;
    if (!w) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255E6C, 860);
    }
    n = ov00_021EC3A8(40);
    if (!n) {
        return 0;
    }
    memset(n, 0, 40);
    n->type = type;
    n->base = (u8 *)ov00_021FF368() + 0xea60;
    n->c = c;
    n->d = d;
    n->e = e;
    n->data = data;
    n->a = a ? ov00_021FF3D4(a) : 0;
    n->b = b ? ov00_021FF3D4(b) : 0;
    n->id = ov45_0224A5E4(w);
    if (!w->head) {
        w->head = n;
    } else {
        w->tail->next = n;
    }
    w->tail = n;
    return n->id;
}

u32 ov45_0224A7F0(Work *w, int a, int b, int c) {
    void *data = ov00_021EC3A8(20);
    if (!data) {
        return 0;
    }
    memset(data, 0, 20);
    return ov45_0224A708(w, 0, 0, 0, a, b, c, data);
}

u32 ov45_0224A874(void *parent, void *a, int b, int c, const Ov45Input48 *source, const char *text) {
    Work *w = parent;
    Payload *p;
    u32 id;
    if (!text) {
        __msl_assertion_failed(ov45_022566A8, ov45_022565D0, ov45_02255F94, 913);
    }
    if (strlen(text) >= 32) {
        __msl_assertion_failed(ov45_022566BC, ov45_022565D0, ov45_02255F94, 914);
    }
    p = ov00_021EC3A8(84);
    if (!p) {
        return 0;
    }
    memset(p, 0, 84);
    p->input = *source;
    strncpy(p->text, text, 32);
    p->text[31] = 0;
    id = ov45_0224A708(w, 1, a, 0, b, 0, c, p);
    if (!id) {
        ov00_021EC3D8(p);
    }
    return id;
}

u32 ov45_0224A978(Work *w, void *a, int b, int c) {
    void *data = ov00_021EC3A8(16);
    if (!data) {
        return 0;
    }
    memset(data, 0, 16);
    return ov45_0224A708(w, 3, a, 0, b, 0, c, data);
}
