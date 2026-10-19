#include "global.h"

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

static inline const int *Minimum(const int *a, const int *b) {
    return *a < *b ? a : b;
}

static inline void Copy(LobbyEvent **first, LobbyEvent **last, LobbyEvent **dest) {
    int count = last - first;
    memmove(dest - count, first, count * sizeof(LobbyEvent *));
}

void LobbyEventDequeMap_CopyBackward(LobbyEventDequeMapIterator *, LobbyEventDequeMapIterator *, LobbyEventDequeMapIterator *, LobbyEventDequeMapIterator *);
#pragma opt_propagation off

void LobbyEventDequeMap_CopyBackward(LobbyEventDequeMapIterator *out, LobbyEventDequeMapIterator *first, LobbyEventDequeMapIterator *last, LobbyEventDequeMapIterator *dest) {
    while (first->cur != last->cur) {
        int cap, amount;
        if (last->cur == last->first) {
            last->cur = last->last;
        }
        --last->cur;
        if (dest->cur == dest->first) {
            dest->cur = dest->last;
        }
        --dest->cur;
        cap = dest->cur - (dest->cur < dest->mid ? dest->first : dest->mid);
        if ((first->cur >= first->mid) == (last->cur >= last->mid)) {
            int n = last->cur - first->cur;
            amount = *Minimum(&cap, &n);
        } else {
            int n = last->cur - (last->cur < last->mid ? last->first : last->mid);
            amount = *Minimum(&cap, &n);
        }
        last->cur -= amount;
        Copy(last->cur, last->cur + (amount + 1), dest->cur + 1);
        dest->cur -= amount;
    }
    out->cur = dest->cur;
    out->first = dest->first;
    out->mid = dest->mid;
    out->last = dest->last;
}
