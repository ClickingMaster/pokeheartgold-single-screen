#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma warn_unusedvar off
#pragma warn_uninitializedvar off

typedef struct {
    u8 *data;
    void *a, *b, *c;
    u32 *d[5], *e[5];
    void *task, *other;
    u16 max, v42[3], v48[3];
    u8 x4e[3], count, index, sel, flag, timer, dirty, flag57, flag58, flag59;
} State;

typedef struct {
    u8 sel, pad;
    u16 scroll;
    State *p;
} Root;

typedef struct {
    void *a;
    u32 *b;
    u8 text[33];
    u8 count, flag;
} Text;

typedef struct {
    void *a;
    u32 *b;
    u8 text[12];
    u8 count, flag;
} Numeric;

extern Root ov13_0224F538;
extern Text *ov13_0224F540;
extern Numeric *ov13_0224F544;
extern u8 ov13_022453CC[];
extern u8 ov13_0224546C[];
extern u8 ov13_02245470[];
extern u16 ov13_02245424[];
extern u16 ov13_02245426[];
extern u16 ov13_02245428[];
extern void ov13_0222B8E8(void (*)(void));
extern void WifiSetupFade_Start(int, int, int, int);
extern void WifiSetupDisplay_ShowPlanes(int, int);
extern void WifiSetupSound_Play(int);
extern void WifiSetupFade_StartDelay(int);
extern int WifiSetupFade_IsActive(int);
extern int WifiSetupKeyboard_GetInput(void);
extern int WifiSetupKeypad_GetInput(void);
extern int ov13_0223C1DC(void);
extern int ov13_0223C1F0(void);
extern int WifiSetupKeypad_Exists(void);
extern void WifiSetupButtonBar_Lock(void);
extern void WifiSetupButtonBar_Close(void);
extern void WifiSetupButtonBar_Unlock(void);
extern void ov13_02239584(void);
extern void WifiSetupKeyboard_Create(void);
extern void WifiSetupKeyboard_SetCanDelete(int);
extern void WifiSetupKeyboard_SetCanType(int);
extern void WifiSetupKeyboard_Close(void);
extern void ov13_0223C198(void);
extern void WifiSetupKeypad_Create(void);
extern void WifiSetupKeypad_SetCanDelete(int);
extern void WifiSetupKeypad_SetCanType(int);
extern void WifiSetupKeypad_SetCanAdvance(int);
extern void WifiSetupKeypad_Close(void);
extern void ov13_0223BE6C(int, int, int, int, int);
extern void *WifiSetupTask_Create(int, void (*)(void *), int, int);
extern void WifiSetupTask_DestroyLater(int, void *);
extern void ov13_02239578(int);
extern void ov13_0223959C(void);
extern void WifiSetupCursor_Hide(void);
extern void WifiSetupCursor_ShowPair(int, int, int, int);
extern void WifiSetupText_Clear(void *, int);
extern void WifiSetupText_MarkDirty(void *);
extern int ov13_02232168(int);
extern void ov13_0222FF3C(void);
extern void ov13_02230540(void);
extern void ov13_022305B0(void);
extern void ov13_02230B14(void *);
extern void ov13_02230BD0(void *);
extern void ov13_02230FB0(void);
extern void ov13_0223121C(void);
extern void ov13_02231298(void);
extern void ov13_022312B8(void);
extern void ov13_022312C8(void);
extern void ov13_02231488(void);
extern void ov13_022314A8(void);

extern void ov13_02231AD4(void);
extern void ov13_02231D9C(void);
extern void ov13_02231E28(void);
extern void ov13_02231E48(void);
extern void ov13_02231E58(void);
extern void ov13_022321FC(void);
extern void ov13_0223221C(void);
extern void ov13_0223224C(void);
extern void ov13_022322D8(void);
extern void ov13_02232494(void);
extern void ov13_02232770(void);

void ov13_02230520(void);
void ov13_02230540(void);
void ov13_02230C68(void);
void ov13_02230CC4(void);
void ov13_02230E5C(void);
void ov13_02230ED0(void);
void ov13_02230F84(void);
void ov13_02230FB0(void);
void ov13_022311C8(void);
void ov13_0223121C(void);
void ov13_02231298(void);
void ov13_022312B8(void);
void ov13_02231488(void);
void ov13_0223148C(void);
void ov13_022314A8(void);
void ov13_022317A8(void);
void ov13_02231810(void);
void ov13_02231AA8(void);
void ov13_02231AD4(void);
void ov13_02231D48(void);
void ov13_02231D9C(void);
void ov13_02231E28(void);
void ov13_02231E48(void);
void ov13_022321FC(void);
void ov13_02232200(void);
void ov13_0223221C(void);
void ov13_0223224C(void);
extern void ov13_02230854(int, int);
extern void ov13_022309CC(int, int);
extern void ov13_02239378(void *, int, int);
extern void ov13_0222B948(int *, int *);
extern u32 ov13_02245454[];
extern int WifiSetupKeyboard_Exists(void);
extern long atol(const char *);

void ov13_022307AC(void);

void ov13_022309CC(int a, int b);

void ov13_02230A58(void);

void ov13_02230B14(void *task);

void ov13_02230BD0(void *task);

void ov13_02230F54(void);

void ov13_022314D8(void);

void ov13_0223186C(void);

int ov13_02232168(int limit);
extern void ov13_022315B0(void);
void ov13_02230520(void);
void ov13_02230540(void);
void ov13_022307AC(void);
void ov13_022309CC(int a, int b);
void ov13_02230A58(void);
void ov13_02230B14(void *task);
void ov13_02230BD0(void *task);
void ov13_02230C68(void);
void ov13_02230CC4(void);
void ov13_02230E5C(void);
void ov13_02230ED0(void);
void ov13_02230F54(void);
void ov13_02230F84(void);
void ov13_02230FB0(void);
void ov13_022311C8(void);
void ov13_0223121C(void);
void ov13_02231298(void);
void ov13_022312B8(void);
void ov13_02231488(void);
void ov13_0223148C(void);
void ov13_022314A8(void);
void ov13_022314D8(void);
void ov13_022317A8(void);
void ov13_02231810(void);
void ov13_0223186C(void);
void ov13_02231AA8(void);
void ov13_02231AD4(void);
void ov13_02231D48(void);
void ov13_02231D9C(void);
void ov13_02231E28(void);
void ov13_02231E48(void);
int ov13_02232168(int limit);
void ov13_022321FC(void);
void ov13_02232200(void);
void ov13_0223221C(void);
void ov13_0223224C(void);
extern int FX_DivS32(int, int);
extern void WifiSetup_LoadGraphics(const void *, void (*)(const void *, u32, u32));
void ov13_0223110C(void);
extern u8 ov13_02246370[];
extern u8 ov13_02246384[];
extern u8 ov13_0224639C[];
extern u8 ov13_022463B4[];
void ov13_02231C8C(void);
extern u8 ov13_022463E4[];
extern u8 ov13_022463F8[];
extern u8 ov13_02246410[];
extern u8 ov13_02246428[];
extern void WifiSetupMessage_Hide(void);
extern void WifiSetupOam_Release(void *);
extern void WifiSetupText_FreeBgCanvas(int);
extern void WifiSetupDisplay_HidePlanes(int, int);
extern void ov13_0222B8F8(int, int);
extern void ov13_0222B934(int, int);
extern void WifiSetupHeap_FreeAndClear(void *);
extern u8 ov13_022463C8[];
extern void ov13_02232E04(void);
extern void ov13_0222FA3C(void);
extern void ov13_02238440(void);
void ov13_022315B0(void);
extern int WifiSetup_StrNLen(const void *, int);
extern int ov13_0223191C(void);
extern void ov13_022316B4(void);
extern void ov13_022317A8(void);
extern void WifiSetupMessage_Show(int, int, int);
int ov13_0223191C(void);
void ov13_022312C8(void);
extern u8 ov13_0224F53C[];

void ov13_02231C8C(void) {
    WifiSetup_LoadGraphics(ov13_022463E4, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(ov13_022463F8, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246410, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246428, GX_LoadBG2Scr);
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & ~3) | 3;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & ~3) | 3;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 2;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 3;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 3;
}

void ov13_02231D48(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 21, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 21);
    ov13_0222B8E8(ov13_02231D9C);
}

void ov13_02231D9C(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupKeypad_Create();
    WifiSetupSound_Play(20);
    if (ov13_0224F544->count == 0) {
        WifiSetupKeypad_SetCanDelete(0);
        WifiSetupKeypad_SetCanAdvance(0);
    } else {
        if (ov13_02232168(26)) {
            WifiSetupKeypad_SetCanType(0);
        }
        WifiSetupKeypad_SetCanAdvance(0);
    }
    ov13_0222B8E8(ov13_02231E28);
}

void ov13_02231E28(void) {
    if (WifiSetupKeypad_GetInput() == 31) {
        return;
    }
    ov13_0222B8E8(ov13_02231E48);
}

void ov13_02231E48(void) {
    ov13_02231E58();
    ov13_022321FC();
}
