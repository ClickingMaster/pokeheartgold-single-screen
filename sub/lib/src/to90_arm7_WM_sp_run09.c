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
u32 WMSP_GetLinkLevel(u32 r);

u32 WMSP_GetLinkLevel(u32 r) {
    if ((*(u8 **)(W + 0x54c))[0x53] == 8) {
        if (r < 22) {
            return 0;
        }
        if (r < 28) {
            return 1;
        }
        if (r < 34) {
            return 2;
        }
        return 3;
    } else {
        if (r < 8) {
            return 0;
        }
        if (r < 14) {
            return 1;
        }
        if (r < 20) {
            return 2;
        }
        return 3;
    }
}
