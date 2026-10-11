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
extern u8 *gWifiSetupKeyboard;
extern u8 *gWifiSetupKeypad;
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
void WifiSetupKeyboard_SetRowY(int, int, int);
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
void WifiSetupKeypad_SetRowY(int, int);
void WifiSetupKeypad_HandleTouchPress(void);
void WifiSetupKeypad_HandleTouchRelease(void);
void WifiSetupKeypad_HandleTouchHold(void);
void WifiSetupKeypad_HandleButtons(void);
void WifiSetupSound_Play(int);
void ov13_02222CE0(void);
int ov13_02220908(void *);
void ov13_0223A6E4(void *);

void WifiSetupKeyboard_SlideOutButtonsTask(void *a);
void WifiSetupKeyboard_SlideOutRow3Task(void *a);
void WifiSetupKeyboard_SlideOutRow2Task(void *a);
void WifiSetupKeyboard_SlideOutRow1Task(void *a);
void WifiSetupKeyboard_SlideOutRow0Task(void *a);
void WifiSetupKeypad_SlideOutButtonsTask(void *a);
void WifiSetupKeypad_SlideOutRow3Task(void *a);
void WifiSetupKeypad_SlideOutRow2Task(void *a);
void WifiSetupKeypad_SlideOutRow1Task(void *a);
void WifiSetupKeypad_SlideOutRow0Task(void *a);
void WifiSetupKeypad_SlideInRow0Task(void *a);
void WifiSetupKeypad_SlideInRow1Task(void *a);
void WifiSetupKeypad_SlideInRow2Task(void *a);
void WifiSetupKeypad_SlideInRow3Task(void *a);
void WifiSetupKeypad_Close(void);
int WifiSetupKeypad_GetInput(void);
void WifiSetupKeypad_SetCanDelete(int a);
void WifiSetupKeypad_SetCanType(int a);
void WifiSetupKeypad_SetCanAdvance(int a);
int WifiSetupKeypad_Exists(void);
void WifiSetupKeypad_SlideInButtonsTask(void *a);
void WifiSetupKeypad_InputTask(void *a);
void WifiSetupKeypad_DestroyTask(void *a);
void WifiSetupKeyboard_DestroyTask(void *a);
void WifiSetupKeypad_UpdateCursor(void);
void WifiSetupKeypad_MoveCursor(int a);
void ov13_0222B364(int a);
int ov13_0222B3B4(void);
void *ov13_0222B414(u32 a);
void ov13_0222B424(void *a);
int ov13_0222B4BC(int a, u32 b);

void WifiSetupKeypad_SetKeyHighlight(int, int);
void WifiSetupKeypad_SetHoveredKey(int);

struct Sprite {
    u32 bits;
    u16 attr;
};

void WifiSetupKeyboard_SetKeyHighlight(int a, int b);
void WifiSetupKeypad_SetKeyHighlight(int a, int b);
void WifiSetupKeyboard_UpdateCursor(void);
extern u16 ov13_02244FA4[];
void WifiSetupKeyboard_SetPage(int);
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
