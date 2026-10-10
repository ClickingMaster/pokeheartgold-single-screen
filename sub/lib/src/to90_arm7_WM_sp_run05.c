#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u8 wmspW[];
extern u8 wmspIndicateThread[];
extern u8 wmspRequestThread[];
#define W       ((u8 *)(wmspW + 0x1000))
#define P       (*(u8 **)(W + 0x550))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern BOOL __VENEER_OS_SendMessage(void *, void *, int);
extern BOOL __VENEER_OS_ReceiveMessage(void *, void *, int);
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_SND_BeginSleep(void);
extern void __VENEER_OS_Terminate(void);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern BOOL __VENEER_OS_DisableScheduler(void);
extern BOOL __VENEER_OS_EnableScheduler(void);
extern BOOL __VENEER_OS_SetThreadPriority(void *, u32);
extern void *WL_GetThreadStruct(void);
extern u32 WMSP_GetLinkLevel(u32);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
void WMSP_CopyParentParam(u8 *, const u8 *);

void WMSP_CopyParentParam(u8 *d, const u8 *s) {
    u16 length;
    U(d, 4) = U(s, 8);
    H(d, 8) = H(s, 12);
    d[11] = (H(s, 14) ? 1 : 0) | (H(s, 18) ? 2 : 0) | (H(s, 20) ? 4 : 0);
    d[10] = H(s, 4);
    H(d, 0) = 1;
    d[2] = 1;
    d[3] = 0;
    H(d, 12) = H(s, 52);
    if (H(s, 18) && H(s, 54) >= 8) {
        length = 8;
    } else {
        length = H(s, 54);
    }
    H(d, 14) = length;
    if (d[10]) {
        __VENEER_MI_CpuCopy8(*(void *const *)s, d + 16, (d[10] + 1) & ~1);
    }
}
