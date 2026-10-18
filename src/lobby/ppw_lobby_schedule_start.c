#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

void *PPWLobby_GetWork(void);
int PPWLobbyPlayers_IsFull(void *);
void DWC_GetDateTime(RTCDate *, RTCTime *);
int PPWLobbySchedule_CanStart(void *);
int PPWLobbySchedule_Start(void *);

int PPWLobbySchedule_CanStart(void *p) {
    RTCTime t;
    RTCDate d;
    s64 now;
    if (W(p, 0) == 5 || W(PPWLobby_GetWork(), 0) != 8) {
        return 0;
    }
    if (*(s64 *)((u8 *)p + 16) != 0) {
        return 0;
    }
    if (W(p, 24)) {
        return 1;
    }
    if (PPWLobbyPlayers_IsFull((u8 *)PPWLobby_GetWork() + 0xe8)) {
        return 1;
    }
    DWC_GetDateTime(&d, &t);
    now = RTC_ConvertDateTimeToSecond(&d, &t);
    return *(s64 *)((u8 *)p + 8) + (s64)(u64)W(p, 28) < now;
}

int PPWLobbySchedule_Start(void *p) {
    RTCTime t;
    RTCDate d;
    if (W(p, 0) != 5 && W(PPWLobby_GetWork(), 0) == 8) {
        DWC_GetDateTime(&d, &t);
        *(s64 *)((u8 *)p + 16) = RTC_ConvertDateTimeToSecond(&d, &t);
        W(p, 0) = 5;
        W(p, 24) = 0;
        ((void (*)(int))P(PPWLobby_GetWork(), 0x30))(0);
        return 1;
    }
    W(p, 24) = 1;
    return 0;
}
