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
} M35598;

#define MLME ((M35598 *)(BASE + 4 + 0x400))
extern void *AllocateHeapBuf(void *, u32);
extern void SetFatalErr(int);
extern void WSetMacAdrs1(void *, const void *);
extern void SendMessageToWmDirect(void *, void *);
int MLME_IssueBeaconSendIndication(void);

int MLME_IssueBeaconSendIndication(void) {
    u8 *p = AllocateHeapBuf(BASE + 0x188, 16);
    if (!p) {
        SetFatalErr(1);
        return 0;
    }
    *(u16 *)(p + 12) = 140;
    *(u16 *)(p + 14) = 0;
    SendMessageToWmDirect(BASE + 0x188, p);
    return 1;
}
