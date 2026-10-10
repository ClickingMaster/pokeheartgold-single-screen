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
} M35595;

#define MLME    ((M35595 *)(BASE + 4 + 0x400))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void *AllocateHeapBuf(void *, u32);
extern void SetFatalErr(int);
extern void WSetMacAdrs1(void *, const void *);
extern void SendMessageToWmDirect(void *, void *);
extern u32 WL_ReadByte(const void *);
extern void WL_WriteByte(void *, u32);
int MLME_IssueReAssIndication(const void *mac, u32 aid, const u8 *ssid);

int MLME_IssueReAssIndication(const void *mac, u32 aid, const u8 *ssid) {
    u8 *p = AllocateHeapBuf(BASE + 0x188, 58);
    u32 i;
    if (!p) {
        SetFatalErr(1);
        return 0;
    }
    H(p, 12) = 135;
    H(p, 14) = 21;
    WSetMacAdrs1(p + 16, mac);
    H(p, 22) = aid;
    H(p, 24) = WL_ReadByte(ssid + 1);
    ssid += 2;
    i = 0;
    while (i < H(p, 24)) {
        if (i >= 32) {
            break;
        }
        WL_WriteByte(p + 26 + i, WL_ReadByte(ssid + i));
        i++;
    }
    while (i < 32) {
        WL_WriteByte(p + 26 + i, 0);
        i++;
    }
    SendMessageToWmDirect(BASE + 0x188, p);
    return 1;
}
