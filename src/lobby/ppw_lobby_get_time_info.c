#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

#pragma warn_uninitializedvar off
#define V(p, o) (*(volatile u32 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern u8 *volatile gPPWLobbyWork;
extern void *LobbyString_InitCopy(void *, void *);
int PPW_LobbyGetTimeInfo(u64 *out);
int PPW_LobbyGetSchedule(void *p, u32 *size);

void *PPWLobbyChannel_GetName(u8 *p, u8 *q) {
    return LobbyString_InitCopy(p, q + 4);
}

int PPW_LobbyGetTimeInfo(u64 *out) {
    u8 *root = gPPWLobbyWork;
    if (!root) {
        return 4;
    }
    if (root) {
        if (W(root, 0) == 10) {
            return 7;
        }
    }
    if (root) {
        if (W(gPPWLobbyWork, 0) != 8) {
            return 4;
        }
    }
    RTCDate date;
    RTCTime time;
    DWC_GetDateTime(&date, &time);
    out[0] = RTC_ConvertDateTimeToSecond(&date, &time);
    u32 a = V(root, 0x14c), b = V(root, 0x150);
    W(out, 8) = a;
    W(out, 12) = b;
    u32 c = V(root, 0x154), d = V(root, 0x158);
    W(out, 16) = c;
    W(out, 20) = d;
    return 0;
}

int PPW_LobbyGetSchedule(void *p, u32 *size) {
    u8 *root = gPPWLobbyWork;
    if (!root) {
        return 4;
    }
    if (root) {
        if (W(root, 0) == 10) {
            return 7;
        }
    }
    if (root) {
        if (W(gPPWLobbyWork, 0) != 8) {
            return 4;
        }
    }
    return PPWLobbySchedule_Copy(root + 0x144, p, size) ? 0 : 3;
}
