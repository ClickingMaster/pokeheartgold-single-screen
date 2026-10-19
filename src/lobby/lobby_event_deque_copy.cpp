#include <nitro/types.h>

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <size_t.h>

inline void *operator new(std::size_t, void *p) {
    return p;
}

template <class T>
inline const T &Min(const T &a, const T &b) {
    return b < a ? b : a;
}

struct FalseType {};

extern "C" LobbyEventDequeIterator LobbyEventDeque_UninitializedCopy(LobbyEventVectorIterator, LobbyEventVectorIterator, LobbyEventDequeIterator, FalseType, u32 *);
extern "C" LobbyEventDequeIterator LobbyEventDeque_Copy(LobbyEventVectorIterator, LobbyEventVectorIterator, LobbyEventDequeIterator);

inline LobbyEvent *UninitCopy(LobbyEventVectorIterator first, LobbyEventVectorIterator last, LobbyEvent *out, u32 *count) {
    for (; first != last; ++first.p, ++out) {
        new (out) LobbyEvent(*first.p);
        ++*count;
    }
    return out;
}

inline LobbyEvent *CopyRange(const LobbyEventVectorIterator &f, const LobbyEventVectorIterator &l, LobbyEvent *out) {
    LobbyEvent *p = f.base();
    LobbyEvent *o = out;
    LobbyEvent *e = l.base();
    for (; p < e; ++p, ++o) {
        *o = *p;
    }
    return o;
}

inline LobbyEvent *Copy(LobbyEventVectorIterator first, LobbyEventVectorIterator last, LobbyEvent *out) {
    return CopyRange(first, last, out);
}

extern "C" LobbyEventDequeIterator LobbyEventDeque_Copy(LobbyEventVectorIterator first, LobbyEventVectorIterator last, LobbyEventDequeIterator result) {
    if (first != last) {
        int n = last - first;
        for (;;) {
            int m;
            LobbyEventVectorIterator mid;
            int room = result.SegmentEnd() - result.cur;
            m = Min(n, room);
            mid = first + m;
            Copy(first, mid, result.cur);
            n -= m;
            result += m;
            if (n == 0) {
                break;
            }
            first = mid;
        }
    }
    return result;
}
