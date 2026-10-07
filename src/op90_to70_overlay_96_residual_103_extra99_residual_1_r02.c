#pragma force_active on
#include "global.h"

typedef struct Group {
    void *a[2];
    void *b[2];
    void *c;
    void *d;
    u32 flags;
} Group;

typedef struct Large {
    void *obj;
    u8 pad[24];
} Large;

typedef struct Big {
    void *owner;
    Group g[2];
    Large large[12];
} Big;

void *ov96_0220ED34(void *a, void *b, int c, int d, int e, int f);
void *ov96_0220ECA4(void *a, void *b, int c, int d, int e, int f, int g);
void ov96_0221007C(Big *w, void *a, void *b, void *owner);

// @fn ov96_0221007C
void ov96_0221007C(Big *w, void *a, void *b, void *owner) {
    int i, j;
    int flag;
    Group *g;

    w->owner = owner;
    for (i = 0; i < 12; i++) {
        w->large[i].obj = ov96_0220ED34(a, b, 0, 0, 5, 5);
    }
    for (i = 0, g = w->g; i < 2; g++, i++) {
        for (j = 0; j < 2; j++) {
            if (j == 1) {
                flag = 1;
            } else {
                flag = 0;
            }
            g->a[j] = ov96_0220ECA4(a, b, 0, 0, 0, 3, flag);
            g->b[j] = ov96_0220ECA4(a, b, 0, 0, 2, 20, flag);
        }
        g->c = ov96_0220ED34(a, b, 0, 0, 0, 1);
        g->d = ov96_0220ED34(a, b, 0, 0, 4, 2);
    }
}
