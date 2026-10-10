#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Node {
    struct Node *next;
} Node;

typedef struct {
    volatile int length;
    u8 p[12];
    void *a, *b;
    u8 q[4];
    Node *head;
} W;

extern void (*ov38_0222621C)(void *);
extern int ov38_02222210(W *, int, int, int *);
extern int ov38_02222350(W *, int, int, void *, int);
extern int ov38_022222E0(W *, int, int);
void ov38_02222E04(W *);
int ov38_02222E7C(W *, void *, int *);

void ov38_02222E04(W *w) {
    Node *n = w->head;
    while (n) {
        Node *next = n->next;
        ov38_0222621C(n);
        w->head = next;
        n = w->head;
    }
    if (w->a) {
        ov38_0222621C(w->a);
    }
    if (w->b) {
        ov38_0222621C(w->b);
    }
    ov38_0222621C(w);
}

int ov38_02222E7C(W *w, void *value, int *out) {
    int end, current, next, n;
    current = ov38_02222210(w, 12, w->length, &end);
    n = 0;
    while (current > 0) {
        next = ov38_02222210(w, current, w->length, &end);
        if (end > 0 && ov38_02222350(w, current, end, value, 0) == 0) {
            if (end + 1 < w->length) {
                n = ov38_02222210(w, end + 1, w->length, 0);
                if (n <= 0) {
                    n = w->length;
                } else {
                    if (n < 2) {
                        return -1;
                    }
                    n -= 2;
                }
                {
                    int answer = ov38_022222E0(w, end + 1, n);
                    if (answer < 0) {
                        answer = n;
                    }
                    *out = answer;
                    return n - answer;
                }
            }
            return 0;
        }
        current = next;
    }
    return -1;
}
