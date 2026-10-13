#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
void *ov45_02238C08(void *);
int PPWLobbySchedule_Copy(u8 *, u8 *, u32 *);
void ov45_02238C1C(u32 *, u8 *);
extern void LobbyCharVector_Grow(void *, u32);
extern void *LobbyByteVector_Data(void *);

void *ov45_02238C08(void *p) {
    if (W(p, 4) == 0) {
        return 0;
    }
    return (void *)W(p, 0);
}

void ov45_02238C1C(u32 *v, u8 *p) {
    u32 len;
    len = 0;
    PPWLobbySchedule_Copy(p, 0, &len);
    {
        u32 n = len;
        u32 zero = 0;
        v[0] = zero;
        v[1] = zero;
        v[2] = zero;
        LobbyCharVector_Grow(v, n);
    }
    PPWLobbySchedule_Copy(p, LobbyByteVector_Data(v), &len);
}
