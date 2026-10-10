#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 id;
    u16 port;
    u8 pad[14];
    u8 flags, other;
} M;

typedef struct W {
    u8 pad[0x10];
    void *p10;
    u8 pad2[0x2c];
    void *p40;
    u8 pad3[8];
    void *p4c;
    u8 pad4[0x49c];
    void *p4ec;
    u8 pad5[0x130];
    void *p620;
    u32 state;
    u32 id;
    u16 port;
    u16 pad6;
    void (*callback)(struct W *, int, M *, void *);
    void *arg;
} W;

typedef struct {
    u32 pad;
    void *array;
} C;

extern void ov00_022126A8(W *, M *, int, int);
extern void ov00_02212BA8(W *, M *);
extern void ov00_02214908(C *, void *);
extern void ov00_02212658(W *, void *);
extern int ArrayLength(void *);
void ov00_022133B8(C *, u32, M *, W *);

void ov00_022133B8(C *c, u32 event, M *m, W *w) {
    switch (event) {
    case 0:
        w->callback(w, 0, m, w->arg);
        if ((m->flags & 3) && (m->flags & 64)) {
            break;
        }
        if ((m->flags & 44) == 0 && w->state == 0) {
            int mode;
            if (m->other & 1) {
                if (w->p4c == 0 || w->p40 == 0) {
                    mode = 1;
                } else {
                    mode = 0;
                }
            } else {
                mode = 2;
            }
            ov00_022126A8(w, m, 0, mode);
        }
        break;
    case 1:
        if ((m->flags & 67) == 0) {
            w->callback(w, 2, m, w->arg);
        } else {
            w->callback(w, 1, m, w->arg);
        }
        break;
    case 2:
        if (m->flags & 44) {
            ov00_02212BA8(w, m);
        }
        w->callback(w, 3, m, w->arg);
        break;
    case 3:
        if (w->p620) {
            ov00_02214908(c, w->p620);
        }
        if (ArrayLength(c->array) != 0 && w->p10 != 0) {
            break;
        }
        w->callback(w, 4, 0, w->arg);
        break;
    case 5:
        w->callback(w, 5, 0, w->arg);
        break;
    case 6:
        ov00_02212658(w, w->p4ec);
        break;
    }
    if (m && m->id == w->id && m->port == w->port) {
        w->id = 0;
    }
}
