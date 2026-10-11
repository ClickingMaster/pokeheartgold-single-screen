#pragma force_active on
#pragma warn_unusedvar off
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Ov13Rect {
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} Ov13Rect;

typedef struct Ov13Buf4 {
    u8 a[4];
} Ov13Buf4;

typedef struct Ov13Buf14 {
    u8 a[14];
} Ov13Buf14;

typedef struct Ov13Buf22 {
    u8 a[22];
} Ov13Buf22;

typedef struct Ov13RectTable {
    Ov13Buf4 buf;
    Ov13Rect r[3];
} Ov13RectTable;

typedef struct Ov13Work {
    u8 pad0[4];
    u8 unk4[4];
    u8 pad8[4];
    u32 unkC;
    u32 unk10;
    u8 pad14[0x38 - 0x14];
    u32 unk38;
    u8 pad3C[0x42 - 0x3C];
    u8 unk42;
    u8 pad43;
    u8 unk44;
    u8 pad45[2];
    u8 unk47;
} Ov13Work;

typedef struct Ov13State {
    u8 state;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    u16 unk4;
    u16 pad6;
    Ov13Work *unk8;
} Ov13State;

typedef struct Ov13Save {
    u8 pad0[0x40];
    u8 unk40;
    u8 pad41[0xC0 - 0x41];
    u32 unkC0;
    u32 unkC4;
    u32 unkC8;
    u32 unkCC;
    u8 padD0[0xF0 - 0xD0];
    u32 unkF0;
    u8 unkF4;
    u8 unkF5;
    u8 unkF6;
} Ov13Save;

typedef struct Ov13Entry {
    u8 pad0[0x28];
    u8 unk28;
    u8 pad29;
} Ov13Entry;

typedef struct Ov13Counter {
    u8 unk0;
    u8 pad1;
    u16 unk2;
} Ov13Counter;

typedef struct Ov13Obj {
    u32 unk0;
    u8 pad4[0x1D - 4];
    u8 unk1D;
} Ov13Obj;

typedef struct Ov13ObjHolder {
    u32 unk0;
    Ov13Obj *unk4;
} Ov13ObjHolder;

typedef void (*Ov13Func)(void);
typedef void (*Ov13ArgFunc)(int);

extern Ov13State ov13_0224F54C;
extern u8 ov13_0224F558;
extern u8 ov13_0224F55C;
extern u8 ov13_0224F560;
extern u8 ov13_0224F564;
extern Ov13Counter ov13_0224F568;
extern Ov13ObjHolder ov13_0224F56C;
extern u8 ov13_022454F8[];
extern u8 ov13_022455DC[];
extern u8 ov13_022455E8[];
extern u8 ov13_022455F4[];
extern Ov13Rect ov13_022455A2[];
extern Ov13Rect ov13_0224558A[];
extern Ov13Buf4 ov13_022454FC; /* really an Ov13RectTable; declared as the 4-byte head so 022350FC copies it as a whole object */
#define OV13_RECT_TABLE (*(Ov13RectTable *)&ov13_022454FC)
extern Ov13Rect ov13_02245958;
extern Ov13Buf14 ov13_02245518;
extern Ov13Buf22 ov13_0224560C;
extern u8 ov13_02246504[];
extern u8 ov13_0224651C[];
extern u8 ov13_02246534[];
extern u8 ov13_02246548[];
extern u8 ov13_0224655C[];
extern u8 ov13_02246570[];
extern u8 ov13_02246584[];
extern u8 ov13_0224659C[];
extern u8 ov13_022465B4[];
extern u8 ov13_022465C8[];
extern u8 ov13_022465E0[];
extern u8 ov13_022465F8[];
extern u8 ov13_0224660C[];
extern u8 ov13_02246620[];
extern u8 ov13_02246638[];
extern u8 ov13_02246650[];

extern BOOL DWC_BACKUPlCheckAddress(void *);
extern BOOL DWC_BACKUPlCheckIp(void *, void *);

extern int WifiSetupInput_IsTouchPressed(Ov13Rect *);
extern int WifiSetupInput_IsTouchPressedInBox(Ov13Rect *);
extern void ov13_0223959C(void);
extern void WifiSetupCursor_Hide(void);
extern void ov13_02233DCC(void);
extern void ov13_022344F0(void);
extern void ov13_02239578(int);
extern void ov13_02239584(void);
extern void WifiSetupTask_DestroyLater(int, int);
extern void WifiSetupCursor_ShowPair(int, u16, u16, u16);
extern Ov13Save *ov13_02239FF0(void);
extern void ov13_0223ABB8(int);
extern u32 WifiSetupTask_Create(int, Ov13ArgFunc, int, int);
extern void ov13_02239378(u32, int, int);
extern int ov13_0223C1DC(void);
extern void ov13_0223C198(void);
extern int ov13_0223C1F0(void);
extern void ov13_022393E0(u32);
extern void ov13_0222B8E8(Ov13Func);
extern void ov13_022332D8(void);
extern void ov13_02227E40(void);
extern void ov13_0223A4F4(void);
extern void WifiSetup_LoadGraphics(void *, void *);
extern void WifiSetupFade_Start(int, int, int, int);
extern void WifiSetupDisplay_ShowPlanes(int, int);
extern int WifiSetupFade_IsActive(int);
extern void ov13_0223BE6C(int, int, int, int, int);
extern void WifiSetupMessage_Hide(void);
extern void WifiSetupDisplay_HidePlanes(int, int);
extern void ov13_0222B8F8(int, int);
extern void WifiSetup_PrintMessage(int);
extern void WifiSetupBusyIcon_Create(int);
extern void ov13_022279B4(int);
extern int WifiSetupButtonBar_GetSelected(void);
extern void WifiSetupButtonBar_Unlock(void);
extern int WifiSetupInput_IsKeyPressed(int);
extern void WifiSetupButtonBar_SelectIfNone(int);
extern void ov13_0223AC20(void);
extern void WifiSetupButtonBar_Lock(void);
extern void WifiSetupFade_StartDelay(int);
extern void WifiSetupButtonBar_Close(void);
extern int ov13_022272A0(void);
extern void WifiSetupBusyIcon_Destroy(void);
extern void WifiSetupText_FreeBgCanvas(int);
extern void ov13_0222BFCC(void);
extern int ov13_0222BFF0(void);
extern void ov13_02237618(void);
extern void ov13_0222C0AC(void);
extern void ov13_0222B934(int, int);
extern void ov13_02238440(void);
extern void WifiSetupMessage_Show(int, int, int);
extern void ov13_02227964(int);
extern void ov13_0222BF60(void);
extern void ov13_02239B00(void);
extern void ov13_02239B68(void);
extern int ov13_02239D04(Ov13Entry **);
extern int ov13_02239C68(void);
extern void ov13_02239B34(void);
extern void ov13_0223A414(int);
extern void ov13_0222FA3C(void);
extern Ov13Obj *WifiSetupHeap_AllocZeroed(int, int);
extern void WifiSetupBackground_LoadHeader(int);
extern void ov13_02236A48(void);
extern void ov13_022370B8(void);
extern void ov13_02236B50(void);
extern void *WifiSetup_LocalizePath(void *);
extern u32 WifiSetupFile_Load(void *, int, int);

void ov13_022349DC(int arg);
void ov13_02234AA4(int arg);
void ov13_02234BCC(void);
void ov13_02234CA4(void);
int ov13_02234974(int x);
void ov13_022352A0(void);
void ov13_022352F0(void);
void ov13_02235384(void);
void ov13_022353D8(void);
void ov13_02235428(void);
void ov13_02235450(void);
void ov13_02235454(void);
void ov13_02235458(void);
void ov13_02235494(void);
void ov13_022368D8(void);
int ov13_022354F0(void);
void ov13_0223554C(void);
void ov13_022355CC(void);
void ov13_02235600(void);
void ov13_0223562C(void);
void ov13_02235654(void);
void ov13_02235674(void);
void ov13_022356AC(void);
void ov13_022356B0(void);
void ov13_022356DC(void);
void ov13_022356FC(void);
void ov13_02235770(void);
void ov13_02235838(void);
void ov13_022358BC(void);
void ov13_022358E8(void);
int ov13_02235908(void);
void ov13_02235928(void);
void ov13_02235964(void);
void ov13_022359E4(void);
void ov13_02235A18(void);
void ov13_02235A44(void);
void ov13_02235A6C(void);
void ov13_02235A8C(void);
void ov13_02235AC4(void);
void ov13_02235AC8(void);
void ov13_02235AF4(void);
void ov13_02235B14(void);
void ov13_02235B78(void);
void ov13_02235C34(void);
void ov13_02235D0C(void);
void ov13_02235D38(void);
void ov13_02235D58(void);
void ov13_02235D94(void);
void ov13_02235E14(void);
void ov13_02235E48(void);
void ov13_02235E6C(void);
void ov13_02235E7C(void);
void ov13_02235E80(void);
void ov13_02235EBC(void);
void ov13_02235F2C(void);
int ov13_02235F60(void);
void ov13_02235510(void);
void ov13_02235FDC(void);
void ov13_02236084(void);
void ov13_022360D8(void);
void ov13_02236118(void);
void ov13_02236140(void);
void ov13_02236160(void);
void ov13_02236198(void);
void ov13_0223619C(void);
void ov13_022361C8(void);
void ov13_022361E8(void);
void ov13_0223625C(void);
void ov13_02236324(void);
void ov13_022363A8(void);
void ov13_022363D4(void);
void ov13_0223645C(void);
void ov13_02236504(void);
void ov13_02236558(void);
void ov13_02236594(void);
void ov13_022365BC(void);
void ov13_022365D4(void);
void ov13_022366FC(void);
void ov13_02236720(void);
void ov13_02236724(void);
void ov13_02236750(void);
void ov13_02236770(void);
void ov13_022367C0(void);
void ov13_0223688C(void);
void ov13_022368B8(void);
void ov13_02236938(void);
void ov13_02236084(void);

extern s32 FX_DivS32(s32, s32);
int ov13_02234700(void);
int ov13_022349B8(int x);
void ov13_02234B48(void);
void ov13_02234D54(u8 st);
void ov13_02234D84(int key);
void ov13_02235074(void);
void ov13_022350A8(void);
void ov13_022350FC(void);
BOOL ov13_022351C0(void);
void ov13_02235260(void);
void ov13_022352C0(u8 x);
void ov13_022352D0(void);
void ov13_02235F80(void);
void ov13_022363F4(void);

#define REG16(a)    (*(vu16 *)(a))
#define SETPRIO3(a) (REG16(a) = (u16)((REG16(a) & ~3) | 3))

int ov13_02234974(int x) {
    int d = FX_DivS32(ov13_0224F54C.unk4, 0x1D);
    int i;
    for (i = 0; i < 4; i++, d++) {
        if (d == x) {
            return i;
        }
    }
    return -1;
}

int ov13_022349B8(int x) {
    int d = FX_DivS32(ov13_0224F54C.unk4, 0x1D);
    return d + x;
}

void ov13_022349DC(int arg) {
    int m;
    ov13_0223959C();
    WifiSetupCursor_Hide();
    if (ov13_0224F54C.unk4 > 6) {
        ov13_0224F54C.unk4 -= 6;
    } else {
        ov13_0224F54C.unk4 = 0;
    }
    m = FX_ModS32(ov13_0224F54C.unk4, 0x1D);
    if (m == 0x17) {
        ov13_02233DCC();
        return;
    }
    if (m > 0x17) {
        ov13_0224F54C.unk4 += 0x1D - m;
        m = 0;
    }
    ov13_022344F0();
    if (m != 0) {
        return;
    }
    ov13_02239578(ov13_0224F54C.unk4 * 0x37 / 0x91);
    ov13_02239584();
    ov13_02234CA4();
    ov13_0224F54C.unk8->unk38 = 0;
    WifiSetupTask_DestroyLater(0, arg);
}

void ov13_02234AA4(int arg) {
    int m;
    ov13_0223959C();
    WifiSetupCursor_Hide();
    ov13_0224F54C.unk4 += 6;
    m = FX_ModS32(ov13_0224F54C.unk4, 0x1D);
    if (m >= 6) {
        ov13_022344F0();
        return;
    }
    ov13_0224F54C.unk4 -= m;
    ov13_02233DCC();
    ov13_02239578(ov13_0224F54C.unk4 * 0x37 / 0x91);
    ov13_02239584();
    ov13_02234CA4();
    ov13_0224F54C.unk8->unk38 = 0;
    WifiSetupTask_DestroyLater(0, arg);
}

void ov13_02234B48(void) {
    int d;
    int m;
    if (ov13_0224F54C.unk8->unk44 == 0) {
        return;
    }
    d = FX_DivS32(ov13_0224F54C.unk4, 0x1D);
    m = FX_ModS32(ov13_0224F54C.unk4, 0x1D) - 0x33;
    d = ov13_022455E8[d];
    G2_SetBG0Offset(0, m);
    G2_SetBG2Offset(0, m + d);
    ov13_0224F54C.unk8->unk44 = 0;
}

void ov13_02234BCC(void) {
    Ov13Rect rect;
    Ov13Buf14 buf = ov13_02245518;
    int idx = buf.a[ov13_0224F54C.state];
    if (idx >= 3) {
        WifiSetupCursor_ShowPair(3, ov13_022455A2[idx].x, ov13_022455A2[idx].w, ov13_022455A2[idx].y);
        return;
    }
    rect = ov13_022455A2[idx];
    rect.y += ov13_0224F54C.unk2 * 0x1D;
    WifiSetupCursor_ShowPair(1, rect.x, rect.w, rect.y);
}

void ov13_02234CA4(void) {
    Ov13Save *save;
    int n;
    u8 st = ov13_0224F54C.state;
    if ((u8)(st + 245) <= 2) {
        ov13_02234BCC();
        return;
    }
    save = ov13_02239FF0();
    n = ov13_0224F54C.unk2 + FX_DivS32(ov13_0224F54C.unk4, 0x1D);
    switch (n) {
    case 2:
        if (save->unkF5) {
            ov13_0224F54C.state = 2;
        } else {
            ov13_0224F54C.state = 3;
        }
        break;
    case 6:
        if (save->unkF6) {
            ov13_0224F54C.state = 7;
        } else {
            ov13_0224F54C.state = 8;
        }
        break;
    default:
        ov13_0224F54C.state = ov13_022455DC[n];
        break;
    }
    ov13_02234BCC();
}

void ov13_02234D54(u8 st) {
    ov13_0224F54C.state = st;
    ov13_0224F54C.unk2 = ov13_02234974(ov13_022455F4[st]);
    ov13_02234BCC();
}

void ov13_02234D84(int key) {
    Ov13Save *save = ov13_02239FF0();
    int ret = 0;
    u8 st = ov13_0224F54C.state;
    if (st == 8 && save->unkF5 == 0 && (key == 0 || key == 2)) {
        return;
    }
    switch (st) {
    case 0:
        if (key == 1) {
            ov13_0224F54C.state = 11;
        } else if (key == 3) {
            ov13_0224F54C.unk2++;
        } else {
            ret = 2;
        }
        break;
    case 10:
        if (key == 1) {
            ov13_0224F54C.unk2--;
        } else if (key == 3) {
            ov13_0224F54C.state = ov13_0224F54C.unk8->unk42;
        } else {
            ret = 2;
        }
        break;
    case 11:
        if (key == 1) {
            if (ov13_0224F54C.unk8->unk47) {
                return;
            }
            ov13_0223ABB8(9);
            ov13_0224F54C.unk8->unk47 = 1;
            return;
        } else if (key == 3) {
            ov13_0224F54C.state = 0;
            ov13_0224F54C.unk2 = 0;
            ov13_0224F54C.unk4 = 0;
            ov13_02233DCC();
            ov13_02239578(0);
        } else {
            ret = 2;
        }
        break;
    case 12:
    case 13:
        ov13_0224F54C.unk8->unk42 = st;
        if (key == 1) {
            ov13_0224F54C.state = 10;
            ov13_0224F54C.unk2 = 3;
            ov13_0224F54C.unk4 = 0x91;
            ov13_02233DCC();
            ov13_02239578(0x37);
        } else if (key == 3) {
            if (ov13_0224F54C.unk8->unk47) {
                return;
            }
            ov13_0223ABB8(9);
            ov13_0224F54C.unk8->unk47 = 1;
            return;
        } else if (st == 12) {
            ov13_0224F54C.state = 13;
        } else {
            ov13_0224F54C.state = 12;
        }
        break;
    default:
        if (key == 1) {
            if (ov13_0224F54C.unk2 != 0) {
                ov13_0224F54C.unk2--;
                break;
            }
            ov13_0223ABB8(0x13);
            ov13_0224F54C.unk8->unk38 = WifiSetupTask_Create(0, ov13_022349DC, 0, 0x78);
            return;
        }
        if (key == 3) {
            if (ov13_0224F54C.unk2 < 3) {
                ov13_0224F54C.unk2++;
                break;
            }
            ov13_0223ABB8(0x13);
            ov13_0224F54C.unk8->unk38 = WifiSetupTask_Create(0, ov13_02234AA4, 0, 0x78);
            return;
        }
        ret = 2;
        if (st == 2) {
            ov13_0224F54C.state = 3;
        } else if (st == 3) {
            ov13_0224F54C.state = 2;
        } else if (st == 7) {
            ov13_0224F54C.state = 8;
        } else if (st == 8) {
            ov13_0224F54C.state = 7;
        } else {
            break;
        }
        ov13_0223ABB8(8);
        ov13_02234BCC();
        break;
    }
    if (ret == 2) {
        return;
    }
    ov13_0223ABB8(8);
    if (ret != 0) {
        return;
    }
    ov13_02234CA4();
}

void ov13_02235074(void) {
    ov13_02239378(ov13_0224F54C.unk8->unk10, ov13_022454F8[ov13_0224F54C.state - 11], ov13_022454F8[ov13_0224F54C.state - 11]);
}

void ov13_022350A8(void) {
    int i;
    for (i = 0; i < 4; i++) {
        if (ov13_0224F54C.unk8->unk4[i] != 0) {
            ov13_0224F54C.unk8->unk4[i]--;
            if (ov13_0224F54C.unk8->unk4[i] == 0) {
                ov13_02233DCC();
            }
        }
    }
}

void ov13_022350FC(void) {
    Ov13Buf4 buf = ov13_022454FC;
    int i;
    for (i = 0; i < 4; i++) {
        if (ov13_0224F54C.state == buf.a[i]) {
            ov13_0224F54C.unk8->unk4[i] = 0x14;
            if (i & 1) {
                ov13_0224F54C.unk8->unk4[i - 1] = 0;
                return;
            }
            ov13_0224F54C.unk8->unk4[i + 1] = 0;
            return;
        }
    }
}

BOOL ov13_022351C0(void) {
    Ov13Save *save = ov13_02239FF0();
    if (save->unk40 == 0) {
        return FALSE;
    }
    if (save->unkF6 == 0 && !DWC_BACKUPlCheckAddress(&save->unkC8) && !DWC_BACKUPlCheckAddress(&save->unkCC)) {
        return FALSE;
    }
    if (save->unkF5 == 0) {
        if (!DWC_BACKUPlCheckAddress(&save->unkC0)) {
            return FALSE;
        }
        if (!DWC_BACKUPlCheckAddress(&save->unkC4)) {
            return FALSE;
        }
        if (!DWC_BACKUPlCheckIp(&save->unkC0, &save->unkF0)) {
            return FALSE;
        }
    }
    return TRUE;
}

void ov13_02235260(void) {
    if (ov13_0223C1DC()) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0223C198();
    ov13_022393E0(ov13_0224F54C.unk8->unkC);
    ov13_0222B8E8(ov13_022352A0);
}

void ov13_022352A0(void) {
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_0222B8E8(ov13_022332D8);
}

void ov13_022352C0(u8 x) {
    ov13_0224F54C.unk1 = x;
}

void ov13_022352D0(void) {
    ov13_022352F0();
    ov13_02227E40();
    ov13_0223A4F4();
    ov13_0222B8E8(ov13_02235384);
}

void ov13_022352F0(void) {
    WifiSetup_LoadGraphics(ov13_02246504, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_0224651C, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246534, GX_LoadBG2Scr);
    SETPRIO3(0x04001008);
    SETPRIO3(0x0400100A);
    SETPRIO3(0x0400000A);
    SETPRIO3(0x0400000C);
}

void ov13_02235384(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x14, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x14);
    ov13_0222B8E8(ov13_022353D8);
}

void ov13_022353D8(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    ov13_0223BE6C(0x44, 5, 1, -1, 0);
    ov13_0222B8E8(ov13_02235428);
}

void ov13_02235428(void) {
    ov13_02235450();
    ov13_02235454();
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_0222B8E8(ov13_02235458);
}

void ov13_02235450(void) {
}

void ov13_02235454(void) {
}

void ov13_02235458(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x14, 8);
    ov13_0222B8E8(ov13_02235494);
}

void ov13_02235494(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x14);
    ov13_0222B8F8(2, 1);
    ov13_0222B8E8(ov13_022368D8);
}

int ov13_022354F0(void) {
    return ((REG16(0x027FFFA8) & 0x8000) >> 15) ? 1 : 0;
}

void ov13_02235510(void) {
    ov13_0224F558 = 0;
    ov13_0223554C();
    ov13_02227E40();
    WifiSetup_PrintMessage(0x25);
    WifiSetupBusyIcon_Create(1);
    ov13_0222B8E8(ov13_022355CC);
}

void ov13_0223554C(void) {
    WifiSetup_LoadGraphics(ov13_02246548, GX_LoadBG2Scr);
    SETPRIO3(0x04001008);
    SETPRIO3(0x0400100A);
    SETPRIO3(0x04000008);
    SETPRIO3(0x0400000A);
    SETPRIO3(0x0400000C);
}

void ov13_022355CC(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    ov13_0222B8E8(ov13_02235600);
}

void ov13_02235600(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    ov13_022279B4(1);
    ov13_0222B8E8(ov13_0223562C);
}

void ov13_0223562C(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    ov13_0222B8E8(ov13_02235654);
}

void ov13_02235654(void) {
    OS_Sleep(10);
    ov13_02235838();
    ov13_02235674();
    ov13_022356AC();
    ov13_022356B0();
}

void ov13_02235674(void) {
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_022354F0()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_022356AC(void) {
}

void ov13_022356B0(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    ov13_0223AC20();
    ov13_0223ABB8(7);
    ov13_0222B8E8(ov13_022356DC);
}

void ov13_022356DC(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    ov13_0222B8E8(ov13_022356FC);
}

void ov13_022356FC(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (ov13_0224F558 == 0) {
        WifiSetupButtonBar_Close();
    }
    if (ov13_0224F558 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_02235770);
}

void ov13_02235770(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (ov13_0224F558 == 0) {
        if (WifiSetupFade_IsActive(1)) {
            return;
        }
    }
    if (!ov13_022272A0()) {
        return;
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F558 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F558 == 0) {
        ov13_0222BFCC();
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_02237618);
        return;
    }
    ov13_0222B8F8(0, 0);
    ov13_0222B8E8(ov13_02235928);
}

void ov13_02235838(void) {
    switch (ov13_0222BFF0()) {
    case 2:
        ov13_0224F558 = 1;
        ov13_0222B8E8(ov13_022356DC);
        break;
    case 4:
        ov13_0224F558 = 0;
        ov13_0223AC20();
        ov13_0223ABB8(9);
        ov13_0223BE6C(0xD, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_022358BC);
        break;
    }
}

void ov13_022358BC(void) {
    if (ov13_0223C1DC()) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0223C198();
    ov13_0222B8E8(ov13_022358E8);
}

void ov13_022358E8(void) {
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_0222B8E8(ov13_022356DC);
}

int ov13_02235908(void) {
    return ((REG16(0x027FFFA8) & 0x8000) >> 15) ? 1 : 0;
}

void ov13_02235928(void) {
    ov13_0224F55C = 0;
    ov13_02235964();
    ov13_02227E40();
    WifiSetup_PrintMessage(0x2A);
    WifiSetupBusyIcon_Create(2);
    ov13_0222B8E8(ov13_022359E4);
}

void ov13_02235964(void) {
    WifiSetup_LoadGraphics(ov13_0224655C, GX_LoadBG2Scr);
    SETPRIO3(0x04001008);
    SETPRIO3(0x0400100A);
    SETPRIO3(0x04000008);
    SETPRIO3(0x0400000A);
    SETPRIO3(0x0400000C);
}

void ov13_022359E4(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    ov13_0222B8E8(ov13_02235A18);
}

void ov13_02235A18(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    ov13_022279B4(1);
    ov13_0222B8E8(ov13_02235A44);
}

void ov13_02235A44(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    ov13_0222B8E8(ov13_02235A6C);
}

void ov13_02235A6C(void) {
    OS_Sleep(10);
    ov13_02235C34();
    ov13_02235A8C();
    ov13_02235AC4();
    ov13_02235AC8();
}

void ov13_02235A8C(void) {
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_02235908()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_02235AC4(void) {
}

void ov13_02235AC8(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    ov13_0223AC20();
    ov13_0223ABB8(7);
    ov13_0222B8E8(ov13_02235AF4);
}

void ov13_02235AF4(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    ov13_0222B8E8(ov13_02235B14);
}

void ov13_02235B14(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F55C == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_02235B78);
}

void ov13_02235B78(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (ov13_0224F55C == 0) {
        if (WifiSetupFade_IsActive(1)) {
            return;
        }
    }
    if (!ov13_022272A0()) {
        return;
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F55C == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    ov13_0222BFCC();
    if (ov13_0224F55C == 0) {
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_02237618);
        return;
    }
    ov13_0222B8F8(0, 0);
    ov13_0222B8E8(ov13_02235D58);
}

void ov13_02235C34(void) {
    switch (ov13_0222BFF0()) {
    case 3:
        ov13_0224F55C = 1;
        ov13_0223AC20();
        ov13_0222C0AC();
        ov13_0222B8E8(ov13_02235AF4);
        break;
    case 4:
        ov13_0224F55C = 0;
        ov13_0223AC20();
        ov13_0223ABB8(9);
        ov13_0223BE6C(0xD, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_02235D0C);
        break;
    case 5:
        ov13_0224F55C = 0;
        ov13_0223AC20();
        ov13_0223ABB8(0x12);
        ov13_0223BE6C(0xE, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_02235D0C);
        break;
    }
}

void ov13_02235D0C(void) {
    if (ov13_0223C1DC()) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0223C198();
    ov13_0222B8E8(ov13_02235D38);
}

void ov13_02235D38(void) {
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_0222B8E8(ov13_02235AF4);
}

void ov13_02235D58(void) {
    ov13_0224F560 = 0;
    ov13_02235D94();
    ov13_02227E40();
    WifiSetup_PrintMessage(0x26);
    ov13_0223ABB8(0x10);
    ov13_0222B8E8(ov13_02235E14);
}

void ov13_02235D94(void) {
    WifiSetup_LoadGraphics(ov13_02246570, GX_LoadBG2Scr);
    SETPRIO3(0x04001008);
    SETPRIO3(0x0400100A);
    SETPRIO3(0x04000008);
    SETPRIO3(0x0400000A);
    SETPRIO3(0x0400000C);
}

void ov13_02235E14(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    ov13_0222B8E8(ov13_02235E48);
}

void ov13_02235E48(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    ov13_0222B8E8(ov13_02235E6C);
}

void ov13_02235E6C(void) {
    ov13_02235F2C();
    ov13_02235E7C();
}

void ov13_02235E7C(void) {
}

void ov13_02235E80(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_02235EBC);
}

void ov13_02235EBC(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 0);
    ov13_0222B934(0, 1);
    ov13_0222B8E8(ov13_02238440);
}

void ov13_02235F2C(void) {
    if (++ov13_0224F560 < 0x78) {
        return;
    }
    ov13_0222B8E8(ov13_02235E80);
}

int ov13_02235F60(void) {
    return ((REG16(0x027FFFA8) & 0x8000) >> 15) ? 1 : 0;
}

void ov13_02235F80(void) {
    ov13_0224F564 = 0;
    ov13_02235FDC();
    ov13_02227E40();
    WifiSetupMessage_Show(0x37, -1, 0);
    ov13_02227964(2);
    WifiSetup_PrintMessage(0x24);
    WifiSetupBusyIcon_Create(0);
    ov13_0223ABB8(0xB);
    ov13_0222B8E8(ov13_02236084);
}

void ov13_02235FDC(void) {
    WifiSetup_LoadGraphics(ov13_02246584, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_0224659C, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022465B4, GX_LoadBG2Scr);
    SETPRIO3(0x04001008);
    SETPRIO3(0x0400100A);
    SETPRIO3(0x04000008);
    SETPRIO3(0x0400000A);
    SETPRIO3(0x0400000C);
}

void ov13_02236084(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    ov13_0222B8E8(ov13_022360D8);
}

void ov13_022360D8(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    ov13_0222BF60();
    ov13_022279B4(1);
    ov13_0222B8E8(ov13_02236118);
}

void ov13_02236118(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    ov13_0222B8E8(ov13_02236140);
}

void ov13_02236140(void) {
    OS_Sleep(10);
    ov13_02236324();
    ov13_02236160();
    ov13_02236198();
    ov13_0223619C();
}

void ov13_02236160(void) {
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_02235F60()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_02236198(void) {
}

void ov13_0223619C(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    ov13_0223AC20();
    ov13_0223ABB8(7);
    ov13_0222B8E8(ov13_022361C8);
}

void ov13_022361C8(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    ov13_0222B8E8(ov13_022361E8);
}

void ov13_022361E8(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (ov13_0224F564 == 0) {
        WifiSetupButtonBar_Close();
    }
    if (ov13_0224F564 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x14, 8);
    ov13_0222B8E8(ov13_0223625C);
}

void ov13_0223625C(void) {
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (ov13_0224F564 == 0) {
        if (WifiSetupFade_IsActive(1)) {
            return;
        }
    }
    if (!ov13_022272A0()) {
        return;
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F564 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F564 == 0) {
        ov13_0222BFCC();
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_02237618);
        return;
    }
    ov13_0222B8F8(0, 0);
    ov13_0222B8E8(ov13_02235510);
}

void ov13_02236324(void) {
    switch (ov13_0222BFF0()) {
    case 1:
        ov13_0224F564 = 1;
        ov13_0222B8E8(ov13_022361C8);
        break;
    case 4:
        ov13_0224F564 = 0;
        ov13_0223AC20();
        ov13_0223ABB8(9);
        ov13_0223BE6C(0xD, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_022363A8);
        break;
    }
}

void ov13_022363A8(void) {
    if (ov13_0223C1DC()) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0223C198();
    ov13_0222B8E8(ov13_022363D4);
}

void ov13_022363D4(void) {
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_0222B8E8(ov13_022361C8);
}

void ov13_022363F4(void) {
    ov13_0224F568.unk2 = 0;
    ov13_0224F568.unk0 = 0;
    ov13_0223645C();
    WifiSetupMessage_Show(0x33, -1, 0);
    ov13_02227E40();
    ov13_02227964(2);
    WifiSetup_PrintMessage(0x33);
    WifiSetupBusyIcon_Create(0);
    ov13_02239B00();
    ov13_02239B68();
    ov13_0223ABB8(0xA);
    ov13_0222B8E8(ov13_02236504);
}

void ov13_0223645C(void) {
    WifiSetup_LoadGraphics(ov13_022465C8, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_022465E0, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_022465F8, GX_LoadBG2Scr);
    SETPRIO3(0x04001008);
    SETPRIO3(0x0400100A);
    SETPRIO3(0x04000008);
    SETPRIO3(0x0400000A);
    SETPRIO3(0x0400000C);
}

void ov13_02236504(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    ov13_0222B8E8(ov13_02236558);
}

void ov13_02236558(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    ov13_022279B4(1);
    ov13_0222B8E8(ov13_02236594);
}

void ov13_02236594(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    ov13_0222B8E8(ov13_022365BC);
}

void ov13_022365BC(void) {
    ov13_022366FC();
    ov13_02236720();
    ov13_02236724();
    ov13_022365D4();
}

void ov13_022365D4(void) {
    Ov13Entry *list;
    Ov13Entry *p;
    int n;
    int i;

    ov13_0224F568.unk2++;
    if (ov13_0224F568.unk2 < 300) {
        return;
    }
    ov13_0223AC20();
    n = ov13_02239D04(&list);
    if (n == 0) {
        ov13_0224F568.unk0 = 2;
        ov13_0223BE6C(0xF, 1, 1, -1, 0);
        ov13_0223ABB8(0x12);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_0223688C);
        return;
    }
    for (i = 0; i < n; i++) {
        if (list[i].unk28 != 2) {
            break;
        }
    }
    if (i == n) {
        ov13_0224F568.unk0 = 3;
        ov13_0223BE6C(0xE, 1, 1, -1, 0);
        ov13_0223ABB8(0x12);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_0223688C);
        return;
    }
    ov13_0224F568.unk0 = 1;
    ov13_0223ABB8(0xF);
    ov13_0222B8E8(ov13_02236750);
}

void ov13_022366FC(void) {
    if (!WifiSetupInput_IsKeyPressed(2)) {
        return;
    }
    ov13_0223AC20();
    WifiSetupButtonBar_SelectIfNone(0);
}

void ov13_02236720(void) {
}

void ov13_02236724(void) {
    if (WifiSetupButtonBar_GetSelected()) {
        return;
    }
    ov13_0223AC20();
    ov13_0223ABB8(7);
    ov13_0222B8E8(ov13_02236750);
}

void ov13_02236750(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    ov13_0222B8E8(ov13_02236770);
}

void ov13_02236770(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_022367C0);
}

void ov13_022367C0(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    if (!ov13_022272A0()) {
        return;
    }
    while (!ov13_02239C68()) {
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F568.unk0 != 1) {
        ov13_02239B34();
        ov13_0223A414(ov13_02239FF0()->unkF4);
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_02237618);
        return;
    }
    ov13_0222B8F8(0, 1);
    ov13_0222B934(0, 0);
    ov13_0222B8E8(ov13_0222FA3C);
}

void ov13_0223688C(void) {
    if (ov13_0223C1DC()) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0223C198();
    ov13_0222B8E8(ov13_022368B8);
}

void ov13_022368B8(void) {
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_0222B8E8(ov13_02236750);
}

void ov13_022368D8(void) {
    Ov13Obj *obj = WifiSetupHeap_AllocZeroed(0x20, 4);
    ov13_0224F56C.unk4 = obj;
    obj->unk1D = 0;
    ov13_02236938();
    WifiSetupBackground_LoadHeader(1);
    WifiSetupMessage_Show(0x2F, -1, 0);
    ov13_02227964(0);
    ov13_02236A48();
    ov13_022370B8();
    ov13_0222B8E8(ov13_02236B50);
}

void ov13_02236938(void) {
    Ov13Buf22 buf = ov13_0224560C;
    WifiSetup_LoadGraphics(ov13_0224660C, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(ov13_02246620, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246638, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246650, GX_LoadBG2Scr);
    ov13_0224F56C.unk4->unk0 = WifiSetupFile_Load(WifiSetup_LocalizePath(&buf), 0, 4);
    WifiSetupDisplay_ShowPlanes(1, 0x10);
    SETPRIO3(0x04001008);
    SETPRIO3(0x0400100A);
    SETPRIO3(0x0400000A);
    SETPRIO3(0x0400000C);
}
