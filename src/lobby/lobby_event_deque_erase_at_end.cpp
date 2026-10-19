#include <nitro/types.h>

#include "lobby_event_deque.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void DWC_Free(int, void *, u32);

struct Alloc {
    void deallocate(LobbyEvent *p) {
        if (p) {
            DWC_Free(0, p, 0);
        }
    }
};

extern "C" void LobbyEventDeque_EraseAtEnd(LobbyEventDeque *d, u32 n);

extern "C" void LobbyEventDeque_EraseAtEnd(LobbyEventDeque *d, u32 n) {
    if (n == 0) {
        return;
    }
    d->size -= n;
    u32 cap = d->map.size == 0 ? 0 : (d->map.size - 1) * 0x80;
    u32 extra = cap - (d->start + d->size);
    if (extra < 0x100) {
        return;
    }
    do {
        Alloc a = Alloc();
        --d->map.size;
        LobbyEvent *&slot = d->map.blocks[(d->map.start + d->map.size - 1) % d->map.capacity];
        a.deallocate(slot);
        extra -= 0x80;
        slot = 0;
    } while (extra >= 0x100);
}
