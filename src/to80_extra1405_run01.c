#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 bytes[2];
} Pair;

typedef struct {
    void *a;
    void *b;
    u8 buf[33];
    u8 val;
} G;

extern Pair ov13_02245454;
extern G *ov13_0224F540;
extern void *WifiSetupHeap_AllocZeroed(int, int);
extern void *WifiSetupText_InitBgCanvas(int, int);
extern void *WifiSetupOamTemplate_AllocOam(int, int);
extern void ov13_0222B948(int *, int *), ov13_0223A28C(void *), ov13_0223110C(void), WifiSetupBackground_LoadHeader(int), WifiSetupMessage_Show(int, int, int), ov13_02227964(int), ov13_022317A8(void), ov13_022316B4(void), ov13_022311C8(void), WifiSetupApp_SetStateFunc(void (*)(void));
extern u8 WifiSetup_StrNLen(void *, int);
void ov13_02230FD8(void);

void ov13_02230FD8(void) {
    Pair p = ov13_02245454;
    int a, b;
    ov13_0224F540 = WifiSetupHeap_AllocZeroed(44, 4);
    ov13_0222B948(&a, &b);
    if (a == 0) {
        ov13_0223A28C(ov13_0224F540->buf);
        ov13_0224F540->val = WifiSetup_StrNLen(ov13_0224F540->buf, 32);
    }
    ov13_0223110C();
    WifiSetupBackground_LoadHeader(a + 9);
    if (b == 1) {
        WifiSetupMessage_Show(53, -1, 0);
    } else {
        WifiSetupMessage_Show(p.bytes[a], -1, 0);
    }
    ov13_02227964(2);
    ov13_0224F540->a = WifiSetupText_InitBgCanvas(0, 0);
    ov13_0224F540->b = WifiSetupOamTemplate_AllocOam(0, 62);
    {
        u16 *q = ov13_0224F540->b;
        q[2] = (q[2] & ~0xc00) | 0xc00;
    }
    ov13_022317A8();
    ov13_022316B4();
    WifiSetupApp_SetStateFunc(ov13_022311C8);
}
