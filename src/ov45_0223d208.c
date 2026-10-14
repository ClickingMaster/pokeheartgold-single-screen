#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef u32 Elem;

typedef struct {
    Elem *cur;
    Elem *first;
    Elem *mid;
    Elem *last;
} It;

int LobbyEventDequeMapIterator_Distance2(It *x, It *y);

int LobbyEventDequeMapIterator_Distance2(It *x, It *y) {
    int n;
    Elem *m = x->mid;
    n = x->cur - y->cur;
    if (x->cur >= m) {
        if (y->cur < m) {
            n -= x->last - x->first;
        }
    } else {
        if (y->cur >= m) {
            n += x->last - x->first;
        }
    }
    return n;
}
