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

void LobbySort_LowerBound(It *out, It first, It last, const int *key);

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
