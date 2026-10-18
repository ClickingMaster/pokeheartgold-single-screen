#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

void LobbyEventDeque_EraseAtBegin(void *p, u32 n) {
    volatile u8 a;
    if (!n) {
        return;
    }
    W(p, 4) -= n;
    W(p, 0) += n;
    while (W(p, 0) >= 256) {
        void *ptr = ((void **)P(p, 8))[W(p, 12)];
        if (ptr) {
            DWC_Free(0, ptr, 0);
        }
        a = 0;
        {
            u32 x = *(volatile u32 *)((u8 *)p + 12) + 1;
            W(p, 12) = x;
            if (x == W(p, 20)) {
                W(p, 12) = 0;
            }
        }
        W(p, 16)
        --;
        W(p, 0) -= 128;
    }
    if (!W(p, 4)) {
        W(p, 0) = 64;
    }
}

void LobbyEventDeque_LengthError(const char *s) {
    printf(s);
    abort();
}
