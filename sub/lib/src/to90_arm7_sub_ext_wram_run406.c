#pragma force_active on
#include <nitro/mi/memory.h>
#include <nitro/os/system.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 wmspW[];
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern OSIntrMode __VENEER_OS_DisableInterrupts(void);
extern OSIntrMode __VENEER_OS_RestoreInterrupts(OSIntrMode);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MI_CpuFill8(void *, u8, u32);

typedef struct Parent38833Pending {
    u32 first, second;
} Parent38833Pending;

void WMSP_EndParent(void *r);

extern void WMSP_IndicateDisconnectionFromMyself(int parent, u16 id, void *mac);
extern void WMSP_ResetSizeVars(void);
extern void *WMSP_WL_DevIdle(void *buf);
extern u16 *WMSP_WL_MlmeDeAuthenticate(u16 *p, const void *a, u16 b);
extern u16 *WMSP_WL_MlmeReset(u16 *p, u16 a);
extern void WmspError_060035F4(u16 a, u16 b);

void WMSP_EndParent(void *r) {
    u8 mac[8], buf[512];
    int id, retry;
    u8 *s = P(wmspW + 0x1000, 0x550);
    u16 *p;
    if (H(s, 0) != 7) {
        p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        p[0] = 9;
        p[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(p);
        return;
    }
    H(s, 0xf6) = 0;
    for (id = 1; id < 16; id++) {
        u32 mask = 1u << id;
        if (H(s, 0x182) & mask) {
            __VENEER_MI_CpuCopy8(s + 0x128 + (id - 1) * 6, mac, 6);
            retry = 0;
            while (retry < 2) {
                u16 err = WMSP_WL_MlmeDeAuthenticate((u16 *)buf, mac, 3)[2];
                if (err == 0) {
                    break;
                }
                if (err != 7 && err != 12) {
                    break;
                }
                retry++;
            }
            OSIntrMode irq = __VENEER_OS_DisableInterrupts();
            if (H(s, 0x182) & mask) {
                H(s, 0x182) &= ~mask;
                H(s, 0x86) &= ~mask;
                ((Parent38833Pending *)(s + 0x738))[id].first = 0;
                ((Parent38833Pending *)(s + 0x738))[id].second = 0;
                __VENEER_OS_RestoreInterrupts(irq);
                WMSP_IndicateDisconnectionFromMyself(1, (u16)id, mac);
            } else {
                __VENEER_OS_RestoreInterrupts(irq);
            }
        }
    }
    u16 err = WMSP_WL_MlmeReset((u16 *)buf, 1)[2];
    if (err) {
        WmspError_060035F4(0, err);
        return;
    }
    H(s, 0xc2) = 0;
    H(s, 0) = 3;
    err = ((u16 *)WMSP_WL_DevIdle(buf))[2];
    if (err) {
        WmspError_060035F4(0x302, err);
        return;
    }
    H(s, 0) = 2;
    W(s, 0x198) = 0;
    H(s, 0x196) = 0;
    __VENEER_MI_CpuFill8(s + 0x19c, 0, 80);
    WMSP_ResetSizeVars();
    p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    p[0] = 9;
    p[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(p);
}
