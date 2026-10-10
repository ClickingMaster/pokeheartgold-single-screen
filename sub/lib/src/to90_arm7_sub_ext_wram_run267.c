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
} M35599;

#define MLME    ((M35599 *)(BASE + 4 + 0x400))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void *AllocateHeapBuf(void *, u32);
extern void SetFatalErr(int);
extern void WSetMacAdrs1(void *, const void *);
extern void SendMessageToWmDirect(void *, void *);
extern u32 WL_ReadByte(const void *);
extern void WL_WriteByte(void *, u32);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
int MLME_IssueBeaconRecvIndication(u8 *frame);

int MLME_IssueBeaconRecvIndication(u8 *frame) {
    u8 *base = BASE;
    u8 *w = base + 0x344;
    u8 *p = AllocateHeapBuf(base + 0x188, H(base, 0x3e4) + 62);
    if (!p) {
        SetFatalErr(1);
        return 0;
    }
    H(p, 12) = 0x8d;
    H(p, 14) = ((int)H(w, 0xa0) + 1) / 2 + 22;
    WL_WriteByte(p + 31, H(frame, 18) & 255);
    WL_WriteByte(p + 30, H(frame, 14) & 255);
    WSetMacAdrs1(p + 46, frame + 30);
    H(p, 22) = H(w, 0xa0);
    if (H(p, 22)) {
        if (H(w, 0xa2) & 1) {
            u32 i;
            u8 *src, *dst;
            dst = p + 60;
            src = *(u8 **)(w + 0x9c) + 1;
            i = 0;
            while (i < H(p, 22)) {
                WL_WriteByte(dst, WL_ReadByte(src));
                src++;
                dst++;
                i++;
            }
        } else {
            __VENEER_MIi_CpuCopy16(*(u8 **)(w + 0x9c), p + 60, H(p, 22) + 1);
        }
    }
    SendMessageToWmDirect(BASE + 0x188, p);
    return 1;
}
