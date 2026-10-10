#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 wmspW[];
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MI_CpuFill8(void *, int, u32);
extern void __VENEER_MIi_CpuClear16(int, void *, u32);

void WMSP_IndicateDisconnectionFromMyself(int parent, u16 id, void *mac);

void WMSP_IndicateDisconnectionFromMyself(int parent, u16 id, void *mac) {
    u16 *p;
    u8 *s = P(wmspW + 0x1000, 0x550);
    p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    p[1] = 0;
    if (parent) {
        p[0] = 8;
        p[4] = 26;
        p[9] = 0xf001;
        p[8] = id;
        __VENEER_MI_CpuCopy8(mac, p + 5, 6);
        p[22] = H(s, 0x30);
        p[23] = H(s, 0x32);
    } else {
        p[0] = 12;
        p[4] = 26;
        p[6] = 0xf001;
        p[5] = H(s, 0x188);
        __VENEER_MI_CpuCopy8(mac, p + 8, 6);
        p[11] = H(s, 0x30);
        p[12] = H(s, 0x32);
    }
    __VENEER_WMSP_ReturnResult2Wm9(p);
}
