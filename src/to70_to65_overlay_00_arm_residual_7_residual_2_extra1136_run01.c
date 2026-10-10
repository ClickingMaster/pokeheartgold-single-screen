#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Node {
    u8 pad[0x1c];
    int state;
    struct Node *next;
} Node;

typedef struct {
    u8 pad[0x1d8];
    u32 state;
    u8 pad2[0x240];
    u32 pending;
    u32 pad3;
    Node *nodes;
} Data;

typedef struct {
    Data *data;
} C;

extern char ov00_0221811C[];
extern char ov00_02218214[];
extern char ov00_02218000[];
extern char ov00_0221821C[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov00_021FF3C0(int);
extern void ov00_02206230(C *, Node *);
extern void ov00_022066D0(C *, Node *);
extern void ov00_02204284(C *, int);
extern int ov00_0220411C(C *);
extern int ov00_02206720(C *, Node **, int);
extern int ov00_02200BC0(C *);
extern int ov00_022073E8(C *);
extern int ov00_02209FCC(C *);
extern int ov00_02202D20(C *, int);
int ov00_02200ED8(C *, int);

int ov00_02200ED8(C *c, int wait) {
    Data *d = c->data;
    int result = 0;
    Node *node;
    if (d->state > 4) {
        __msl_assertion_failed(ov00_0221811C, ov00_02218214, ov00_02218000, 0x1b6);
    }
    if (d->state == 1) {
        int again;
        do {
            result = ov00_0220411C(c);
            if (result == 0 && wait && d->state == 1) {
                again = 1;
            } else {
                again = 0;
            }
            if (again) {
                ov00_021FF3C0(10);
            }
        } while (again);
        if (result) {
            if (ov00_02206720(c, &node, 1)) {
                node->state = 4;
            } else {
                __msl_assertion_failed(ov00_0221821C, ov00_02218214, ov00_02218000, 0x1dc);
            }
        }
    }
    if (d->state == 2 || d->state == 3) {
        if (!result) {
            result = ov00_02200BC0(c);
        }
        if (!result) {
            result = ov00_022073E8(c);
        }
    }
    if (!result) {
        result = ov00_02209FCC(c);
    }
    node = d->nodes;
    while (node) {
        if (node->state == 0) {
            node = node->next;
        } else {
            ov00_02206230(c, node);
            {
                Node *old = node;
                node = node->next;
                ov00_022066D0(c, old);
            }
        }
    }
    {
        int status = ov00_02202D20(c, wait);
        if (status) {
            return status;
        }
    }
    if (d->pending) {
        ov00_02204284(c, 0);
    }
    return result;
}
