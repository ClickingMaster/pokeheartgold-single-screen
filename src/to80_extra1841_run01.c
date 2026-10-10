#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Root {
    char pad[0x100];
    int enabled;
} Root;

typedef struct Context {
    Root *root;
} Context;

typedef struct Lookup {
    char pad[12];
    void *data;
} Lookup;

typedef struct Node {
    char pad[0x18];
    int output;
} Node;

typedef struct Pair {
    void *a, *b;
} Pair;

typedef struct Block {
    int zero, id;
} Block;

extern char ov00_02218CC0[];
extern int ov00_02207DD0(Context *, int, Lookup **);
extern int ov00_0220657C(Context *, int, int, Node **, int, void *, void *);
extern int ov00_02202A84(Context *, Pair, Block *, Node *, int);
extern int ov00_02205E50(Context *, int, int);
extern int ov00_02200ED8(Context *, int);
extern void *ov00_021EC3A8(int);
extern void ov00_0220A890(Context *, char *);
extern void ov00_02204654(void *, Block *);
extern void ov00_022066D0(Context *, Node *);
int ov00_02205EE8(Context *, int, int, int, void *, void *);

int ov00_02205EE8(Context *c, int id, int mode, int finish, void *a, void *b) {
    struct {
        Node *node;
        Lookup *lookup;
    } frame;

    Pair pair;
    int active, output, error;
    Root *root = c->root;
    frame.node = 0;
    active = (mode == 1);
    if (!root->enabled) {
        active = 0;
    }
    if (a && active && ov00_02207DD0(c, id, &frame.lookup) && frame.lookup->data) {
        Block *block = ov00_021EC3A8(0x204);
        if (!block) {
            ov00_0220A890(c, ov00_02218CC0);
            return 1;
        }
        ov00_02204654(frame.lookup->data, block);
        block->zero = 0;
        block->id = id;
        pair.a = a;
        pair.b = b;
        error = ov00_0220657C(c, 2, 0, &frame.node, 1, a, b);
        if (error) {
            return error;
        }
        output = frame.node->output;
        error = ov00_02202A84(c, pair, block, frame.node, 0);
        if (error) {
            return error;
        }
        ov00_022066D0(c, frame.node);
    } else {
        error = ov00_0220657C(c, 2, 0, &frame.node, finish, a, b);
        if (error) {
            return error;
        }
        output = frame.node->output;
        error = ov00_02205E50(c, id, output);
        if (error) {
            return error;
        }
    }
    if (finish) {
        error = ov00_02200ED8(c, output);
        if (error) {
            return error;
        }
    }
    return 0;
}
