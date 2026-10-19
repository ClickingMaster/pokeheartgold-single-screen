#include <nitro/types.h>

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline int Distance(LobbyEventVectorIterator first, LobbyEventVectorIterator last) {
    return last.p - first.p;
}

class Tag {};

template <class T>
inline const T &Min(const T &a, const T &b) {
    return a < b ? a : b;
}

extern "C" LobbyEventDequeIterator LobbyEventDeque_IteratorAt(LobbyEventDeque *, int);
extern "C" LobbyEventDequeIterator LobbyEventDeque_Copy(LobbyEventVectorIterator, LobbyEventVectorIterator, LobbyEventDequeIterator);
extern "C" void LobbyEventDeque_AppendRange(LobbyEventDeque *, LobbyEventVectorIterator *, const LobbyEventVectorIterator &, Tag);
extern "C" void LobbyEventDeque_EraseAtEnd(LobbyEventDeque *, u32, Tag);
extern "C" void LobbyEventDeque_Assign(LobbyEventDeque *, LobbyEventVectorIterator, const LobbyEventVectorIterator &, Tag);

extern "C" void LobbyEventDeque_Assign(LobbyEventDeque *d, LobbyEventVectorIterator first, const LobbyEventVectorIterator &last, Tag) {
    u32 n = Distance(first, last);
    u32 m = Min(d->size, n);
    LobbyEventVectorIterator mid = first;
    mid.p += m;
    LobbyEventDeque_Copy(first, mid, LobbyEventDeque_IteratorAt(d, 0));
    n -= m;
    if (n != 0) {
        Tag t;
        LobbyEventDeque_AppendRange(d, &mid, last, t);
    } else {
        u32 size = d->size;
        if (m < size) {
            LobbyEventDeque_EraseAtEnd(d, size - m, Tag());
        }
    }
}
