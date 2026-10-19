#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Rotation and binary search over sorted runs: the sort merges with these in place when its buffer is too small.

typedef struct {
    int key;
    int value;
} Pair;

typedef struct {
    Pair *p;
} It;

void LobbySort_Rotate(It *out, It first, It middle, It last);
void LobbySort_LowerBound(It *out, It first, It last, const int *key);
void LobbySort_UpperBound(It *out, It first, It last, const int *key);

void LobbySort_Rotate(It *out, It first, It middle, It last) {
    Pair *m, *a, *b, *result;
    Pair temp1, temp2;
    a = first.p;
    m = middle.p;
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

void LobbySort_LowerBound(It *out, It first, It last, const int *key) {
    It start = first;
    int count = last.p - start.p;
    while (count > 0) {
        int half = count / 2;
        It mid;
        mid.p = first.p + half;
        if (mid.p->key < *key) {
            first.p = mid.p + 1;
            count -= half + 1;
        } else {
            count = half;
        }
    }
    *out = first;
}

void LobbySort_UpperBound(It *out, It first, It last, const int *key) {
    It start = first;
    int count = last.p - start.p;
    while (count > 0) {
        int half = count / 2;
        It mid;
        mid.p = first.p + half;
        if (!(*key < mid.p->key)) {
            first.p = mid.p + 1;
            count -= half + 1;
        } else {
            count = half;
        }
    }
    *out = first;
}
