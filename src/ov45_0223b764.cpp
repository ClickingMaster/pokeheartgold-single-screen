#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" void ov00_021EC2EC(int, void *, u32);

struct Alloc {
    void deallocate(char *p) {
        if (p) {
            ov00_021EC2EC(0, p, 0);
        }
    }
};

struct Map {
    char **ptr;
    u32 start, size, cap;
};

struct Deque {
    u32 start, size;
    Map map;
};

extern "C" void ov45_0223B764(Deque *d, u32 n);

extern "C" void ov45_0223B764(Deque *d, u32 n) {
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
        char *&slot = d->map.ptr[(d->map.start + d->map.size - 1) % d->map.cap];
        a.deallocate(slot);
        extra -= 0x80;
        slot = 0;
    } while (extra >= 0x100);
}
