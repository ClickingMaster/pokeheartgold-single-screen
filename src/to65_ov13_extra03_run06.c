#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma warn_unusedvar off
#pragma warn_uninitializedvar off
#define B(p, n) (((u8 *)(p))[n])
#define S(p, n) (((s8 *)(p))[n])
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))
extern u8 *ov13_0224F4BC;
extern u8 *ov13_0224F4C0;
extern u8 ov13_0224F4C4[];
extern u8 ov13_0224F4C8[];
extern u8 ov13_0224F4CC[];
extern volatile u8 ov13_02244F4C[];
extern volatile u8 ov13_02244F50[];
extern volatile u8 ov13_0224521C[];
extern volatile u8 ov13_02245210[];
extern u16 ov13_02245060[];
extern u16 ov13_02245062[];
extern u16 ov13_022452A2[];
extern u16 ov13_022452A4[];
extern s8 ov13_022452DA[];
void ov13_022290F0(int, int, int);
void *WifiSetupOamBlock_GetOam(void *, int);
void WifiSetupOamBlock_Free(void *);
void WifiSetupOamBlock_SetModeAndPalette(void *, int, int, int);
void WifiSetupOamBlock_SetPosition(void *, int, int, int);
void WifiSetupOamBlock_SetPriority(void *, int, int);
void WifiSetupOamTemplate_Copy(int, int, void *);
void WifiSetupOamBlock_GetPosition(void *, int, int *, int *);
void WifiSetupText_FreeObjCanvas(void *);
void WifiSetupOam_Release(void *);
void *WifiSetupHeap_AllocZeroed(u32, int);
void WifiSetupHeap_Free(void *);
void WifiSetupHeap_FreeAndClear(void *);
void WifiSetupTask_SetFunc(void *, void (*)(void *));
void WifiSetupTask_DestroyLater(int, void *);
void ov13_0222AAFC(int, int);
void ov13_0222A378(void);
void ov13_0222A554(void);
void ov13_0222A724(void);
void ov13_0222A960(void);
void ov13_0223ABB8(int);
void ov13_02222CE0(void);
int ov13_02220908(void *);
void ov13_0223A6E4(void *);

void ov13_022299CC(void *a);
void ov13_02229A50(void *a);
void ov13_02229AC4(void *a);
void ov13_02229B38(void *a);
void ov13_02229BAC(void *a);
void ov13_0222AF5C(void *a);
void ov13_0222AFD4(void *a);
void ov13_0222B044(void *a);
void ov13_0222B0B4(void *a);
void ov13_0222B124(void *a);
void ov13_0222A0DC(void *a);
void ov13_0222A160(void *a);
void ov13_0222A1E4(void *a);
void ov13_0222A268(void *a);
void ov13_0222A03C(void);
int ov13_0222A070(void);
void ov13_0222A084(int a);
void ov13_0222A098(int a);
void ov13_0222A0AC(int a);
int ov13_0222A0C0(void);
void ov13_0222A2EC(void *a);
void ov13_0222A360(void *a);
void ov13_0222B194(void *a);
void ov13_02229C20(void *a);
void ov13_0222AE24(void);
void ov13_0222AEB0(int a);
void ov13_0222B364(int a);
int ov13_0222B3B4(void);
void *ov13_0222B414(u32 a);
void ov13_0222B424(void *a);
int ov13_0222B4BC(int a, u32 b);

void ov13_0222AD5C(int, int);
void ov13_0222A914(int);

struct Sprite {
    u32 bits;
    u16 attr;
};

void ov13_022293F0(int a, int b);
void ov13_0222AD5C(int a, int b);
void ov13_022294B8(void);
extern u16 ov13_02244FA4[];
void ov13_0222936C(int);
int ov13_0222B430(void *, int, u32);
void ov13_0222B558(void);
void ov13_0222B620(void);
void ov13_0223AAFC(void);
void ov13_0222B8E8(void (*)(void));
void ov13_0222C1BC(void);
void WifiSetupInput_Update(void);
void WifiSetupTask_RunQueue(int);
void WifiSetupInput_UpdateLcdPower(void);
void ov13_022422E0(void);
void ov13_0222B898(void);

void ov13_0222B364(int a) {
    ov13_02222CE0();
    if (a && B(P(ov13_0224F4C4, 4), 0x116) == 0 && B(ov13_0224F4C4, 0) == 1) {
        ov13_0223A6E4((u8 *)P(ov13_0224F4C4, 4) + 0x117);
    }
    WifiSetupHeap_FreeAndClear(ov13_0224F4C8);
}

int ov13_0222B3B4(void) {
    u32 v;
    if (!ov13_02220908(P(ov13_0224F4C4, 4))) {
        B(ov13_0224F4C4, 0) = 1;
        return 1;
    }
    v = B(P(ov13_0224F4C4, 4), 0x116);
    if (v == 1 || (u8)(v + 253) <= 2) {
        return 0;
    }
    return 2;
}

void *ov13_0222B414(u32 a) {
    return WifiSetupHeap_AllocZeroed(a, 32);
}

void ov13_0222B424(void *a) {
    WifiSetupHeap_Free(a);
}

int ov13_0222B430(void *a, int b, u32 c) {
    P(ov13_0224F4CC, 4) = a;
    if (!ov13_0222B4BC(b, c)) {
        return -1;
    }
    B(ov13_0224F4CC, 1) = 0;
    ov13_0222B558();
    ov13_0222B620();
    ov13_0223AAFC();
    ov13_0222B8E8(ov13_0222C1BC);
    do {
        WifiSetupInput_Update();
        ((void (*)(void))P(ov13_0224F4CC, 12))();
        WifiSetupTask_RunQueue(0);
        WifiSetupInput_UpdateLcdPower();
        ov13_022422E0();
        OS_WaitVBlankIntr();
    } while (B(ov13_0224F4CC, 1) == 0);
    ov13_0222B898();
    return 0;
}
