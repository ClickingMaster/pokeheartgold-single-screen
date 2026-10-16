#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
void *LobbyEventVector_Data(void *);
int PPWLobbySchedule_Copy(u8 *, u8 *, u32 *);

int PPWLobbySchedule_Copy(u8 *s, u8 *out, u32 *len) {
    u32 needed = ((W(s, 0x30) - 1) << 3) + 0x18;
    u32 cap = *len;
    *len = needed;
    if (cap < needed || !out) {
        return 0;
    }
    W(out, 0) = W(s, 0x1c);
    W(out, 4) = W(s, 0x20);
    W(out, 8) = W(s, 0x24);
    B(out, 12) = B(s, 0x28);
    B(out, 13) = B(s, 0x29);
    H(out, 14) = W(s, 0x30);
    if (H(out, 14)) {
        MI_CpuCopy8(LobbyEventVector_Data(s + 0x2c), out + 16, W(s, 0x30) << 3);
    }
    return 1;
}
