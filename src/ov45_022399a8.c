#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    int key;
    int value;
} Pair;

typedef struct {
    Pair *p;
} It;

void ov45_022399A8(It *out, It first, It middle, It last);

void ov45_022399A8(It *out, It first, It middle, It last) {
    Pair *m, *a, *b, *result;
    Pair temp1, temp2;
    a = ((volatile It *)&first)->p;
    m = ((volatile It *)&middle)->p;
    if (a == m) {
        *out = last;
        return;
    }
    if (m == last.p) {
        out->p = ((volatile It *)&first)->p;
        return;
    }
    b = ((volatile It *)&middle)->p;
    for (;;) {
        temp1 = *a;
        a->key = b->key;
        a->value = b->value;
        b->key = temp1.key;
        b->value = temp1.value;
        ++b;
        ++a;
        if (b == last.p) {
            break;
        }
        if (a == m) {
            m = b;
        }
    }
    result = a;
    if (a != m) {
        b = m;
        for (;;) {
            temp2 = *a;
            a->key = b->key;
            a->value = b->value;
            b->key = temp2.key;
            b->value = temp2.value;
            ++b;
            ++a;
            if (b == last.p) {
                if (a == m) {
                    break;
                }
                b = m;
            } else if (a == m) {
                m = b;
            }
        }
    }
    out->p = result;
}
