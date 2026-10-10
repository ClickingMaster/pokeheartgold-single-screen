#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE (*(u8 **)0x0380FFF4)
extern void AddTask(int, int);

typedef struct {
    u16 state, x, result, reason, y, elapsed, step, z;
    u32 a, b;
    u8 *req, *confirm;
} M35597;

#define MLME ((M35597 *)(BASE + 4 + 0x400))
extern void *AllocateHeapBuf(void *, u32);
extern void SetFatalErr(int);
extern void WSetMacAdrs1(void *, const void *);
extern void SendMessageToWmDirect(void *, void *);
int MLME_IssueBeaconLostIndication(const void *mac);

int MLME_IssueBeaconLostIndication(const void *mac) {
    u8 *p = AllocateHeapBuf(BASE + 0x188, 22);
    if (!p) {
        SetFatalErr(1);
        return 0;
    }
    *(u16 *)(p + 12) = 139;
    *(u16 *)(p + 14) = 3;
    WSetMacAdrs1(p + 16, mac);
    SendMessageToWmDirect(BASE + 0x188, p);
    return 1;
}
