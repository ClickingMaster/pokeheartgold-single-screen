#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void WMSP_DisconnectCore(void *, int, int);
void WMSP_AutoDisconnect(void *arg);

void WMSP_AutoDisconnect(void *arg) {
    WMSP_DisconnectCore(arg, 1, 0);
}
