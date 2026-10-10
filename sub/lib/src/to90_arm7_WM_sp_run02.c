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
void *WMSP_WlRequest(void *request);

void *WMSP_WlRequest(void *request) {
    u8 *reply;
    __VENEER_OS_SendMessage(wmspW, request, 1);
    __VENEER_OS_ReceiveMessage(wmspW + 0x58, &reply, 1);
    if (H(reply + H(reply, 0xe) * 2, 0x14) == 14) {
        u16 *r = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        r[0] = 0x80;
        r[1] = 0x13;
        r[2] = 0x18;
        __VENEER_WMSP_ReturnResult2Wm9(r);
        __VENEER_SND_BeginSleep();
        __VENEER_OS_Terminate();
    }
    return reply;
}
