#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef void (*StateFunc)(void);

extern void WifiSetupApp_SetStateFunc(void *func);
extern void ov13_0222B8F8(int a, int b);
extern int WifiSetupFade_IsActive(int a);
extern void WifiSetupFade_Start(int a, int b, int c, int d);
extern void WifiSetupDisplay_ShowPlanes(int a, int b);
extern void WifiSetupDisplay_HidePlanes(int a, int b);
extern void WifiSetupText_FreeBgCanvas(int a);
extern void WifiSetupFade_StartDelay(int a);
extern int WifiSetupInput_IsKeyPressed(int a);
extern void ov13_0222BC84(void *func);
extern int ov13_0222BC68(void);
extern void ov13_0222BC0C(void);
extern void WifiSetupSignalIcon_Destroy(void);
extern void WifiSetupBusyIcon_Destroy(void);
extern void ov13_0223A840(void);
extern void ov13_0223AC20(void);
extern void WifiSetupSound_Play(int a);
extern void WifiSetup_PrintMessage(int a);
extern void ov13_022279B4(int a);
extern int WifiSetupButtonBar_GetSelected(void);
extern void WifiSetupButtonBar_Unlock(void);
extern void WifiSetupButtonBar_SelectIfNone(int a);
extern void WifiSetupButtonBar_Lock(void);
extern void WifiSetupButtonBar_Close(void);
extern int ov13_022272A0(void);
extern void WifiSetupBackground_LoadHeader(int a);
extern int WifiSetupMessage_Show(int a, int b, int c);
extern void WifiSetup_LoadGraphics(void *a, void *b);
extern void ov13_0222D454();
extern void ov13_0222DB6C();
extern void WifiSetupMessage_Hide(void);

extern u8 ov13_0224F514;
extern u8 ov13_0224F518;
extern u8 ov13_0224F51C;
extern u8 ov13_0224F520[];
extern u8 ov13_0224F524[];
extern u8 ov13_0224F528;

struct UnkF52C {
    u8 flag;
    u16 counter;
    void *task;
};
extern struct UnkF52C ov13_0224F52C;
extern u8 ov13_0224F534;

struct UnkOam {
    u16 attr0;
    u16 attr1;
    u16 attr2;
};

struct UnkBig {
    u8 unk0[4];
    void *unk4;
    void *unk8;
    int unkC;
    struct UnkOam *unk10[5];
    struct UnkOam *unk24[5];
    u8 unk38[4];
    void *unk3C;
    u16 unk40;
    u16 unk42[3];
    u16 unk48[4];
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
};

struct UnkF538 {
    u8 unk0;
    u16 unk2;
    struct UnkBig *big;
};
extern struct UnkF538 ov13_0224F538;

struct Str25 {
    char s[25];
};

struct Str22 {
    char s[22];
};
extern struct Str25 ov13_022453E3;
extern struct Str22 ov13_022453CD;
extern u8 ov13_02246358[];
extern u8 ov13_0224544C[];
extern u8 ov13_02245450[];
extern void ov13_0222B934(int a, int b);
extern void ov13_02238440();
extern void *WifiSetupHeap_AllocZeroed(int size, int align);
extern void ov13_0222B948(int *out, int a);
extern u8 ov13_02239D04(void *p);
extern void ov13_02230C68();
extern void ov13_022307AC(void);
extern void ov13_02230CC4(void);
extern void *WifiSetup_LocalizePath(void *a);
extern void *WifiSetupFile_Load(void *a, int b, int c);
extern void ov13_02239228(void *a);
extern void ov13_022392B4(void);
extern void ov13_0223943C(int a, int b, int c, int d, int e);
extern struct UnkOam *WifiSetupOamTemplate_AllocOam(int a, int b);
extern void WifiSetupOamTemplate_Copy(int a, int b, struct UnkOam *c);
extern void ov13_0222FF50(void);
extern void ov13_022301A4(void);
extern void ov13_02230414(void);
extern s32 FX_DivS32(s32 a, s32 b);
extern u8 ov13_02246300[];
extern u8 ov13_02246318[];
extern u8 ov13_02246330[];
extern u8 ov13_02246344[];
extern void ov13_02227E40(void);
extern void ov13_02227964(int a);
extern void WifiSetupApp_StartClient(void);
extern void *WifiSetupTask_Create(int a, void *b, int c, int d);
extern int WifiSetupApp_PollClient(void);
extern void WifiSetupTask_Destroy(int a, void *b);
extern void WifiSetupTask_DestroyLater(int a, void *b);
extern void WifiSetupApp_StopClient(int a);
extern void ov13_02237618();
extern void WifiSetupInput_Update(void);
extern void WifiSetupTask_RunQueue(int a);
extern u8 ov13_022462D8[];
extern u8 ov13_022462EC[];
extern void *ov13_0222BCA8(void);
extern int WifiSetupText_InitBgCanvas(int a, int b);
extern void WifiSetupText_DrawInRect(int a, int b, int c, int d, int e, int f, int g, void *h);
extern void WifiSetupText_MarkDirty(int a);
extern void ov13_0223BE6C(int a, int b, int c, int d, int e);
extern int ov13_0223C1DC(void);
extern void ov13_0223C198(void);
extern int ov13_0223C1F0(void);
extern void ov13_0222BC9C(void);
extern void ov13_0222BB20(void *func);
extern void WifiSetupBusyIcon_Create(int a);
extern u8 ov13_0224629C[];
extern u8 ov13_022462B0[];
extern u8 ov13_022462C4[];

void ov13_0222DC24();
void ov13_0222DC58();
void ov13_0222DC88();
void ov13_0222DC98();
void ov13_0222DC9C();
void ov13_0222DCA0();
void ov13_0222DCC8();
void ov13_0222DD5C();
void ov13_0222DDE4();
void ov13_0222DE04();
void ov13_0222DE6C();
void ov13_0222DEA0();
void ov13_0222DECC();
void ov13_0222DEF4();
void ov13_0222DF08();
void ov13_0222DF28();
void ov13_0222DF2C();
void ov13_0222DF54();
void ov13_0222DF74();
void ov13_0222DFC0();
void ov13_0222DFF4();
void ov13_0222E014();
void ov13_0222E07C();
void ov13_0222E0B0();
void ov13_0222E0EC();
void ov13_0222E114();
void ov13_0222E128();
void ov13_0222E148();
void ov13_0222E14C();
void ov13_0222E174();
void ov13_0222E194();
void ov13_0222E1D0();
void ov13_0222E220();
void ov13_0222E268();
void ov13_0222E2E8();
void ov13_0222E33C();
void ov13_0222E368();
void ov13_0222E390();
void ov13_0222E3A4();
void ov13_0222E3DC();
void ov13_0222E3E0();
void ov13_0222E430();
void ov13_0222E450();
void ov13_0222E4B4();
void ov13_0222E56C();
void ov13_0222E59C();
void ov13_0222E608();
void ov13_0222E63C();
void ov13_0222E66C();
void ov13_0222E67C();
void ov13_0222E680();
void ov13_0222E6BC();
void ov13_0222E720();
void ov13_0222E754();
void ov13_0222E790();
void ov13_0222E810();
void ov13_0222E878();
void ov13_0222E8B4();
void ov13_0222E8E8();
void ov13_0222E8FC();
void ov13_0222E934();
void ov13_0222E938();
void ov13_0222E990();
void ov13_0222E9B0();
void ov13_0222EA14();
void ov13_0222EABC();
void ov13_0222EB8C();
void ov13_0222EC0C();
void ov13_0222EC40();
void ov13_0222EC64();
void ov13_0222EC88();
void ov13_0222ECF8();
void ov13_0222ECFC();
void ov13_0222ED00();
void ov13_0222ED5C();
void ov13_0222EDB8();
void ov13_0222EE80();
int ov13_0222EE94();
int ov13_0222F28C();
void ov13_0222EEB4();
void ov13_0222EF00();
void ov13_0222EF80();
void ov13_0222EFB4();
void ov13_0222EFE0();
void ov13_0222F004();
void ov13_0222F018();
void ov13_0222F050();
void ov13_0222F054();
void ov13_0222F080();
void ov13_0222F0B8();
void ov13_0222F138();
void ov13_0222F230();
void ov13_0222F2AC();
void ov13_0222F30C();
void ov13_0222F3B4();
void ov13_0222F408();
void ov13_0222F444();
void ov13_0222F48C();
void ov13_0222F560();
void ov13_0222F598();
void ov13_0222F59C();
void ov13_0222F5E8();
void ov13_0222F624();
void ov13_0222F688();
void ov13_0222F764();
void ov13_0222F780();
void ov13_0222F7AC();
void ov13_0222F7CC();
void ov13_0222F818();
void ov13_0222F848();
void ov13_0222F884();
void ov13_0222F8F0();
void ov13_0222F924();
void ov13_0222F948();
void ov13_0222F958();
void ov13_0222F95C();
void ov13_0222F998();
void ov13_0222FA08();
void ov13_0222FA3C();
void ov13_0222FB14();
void ov13_0222FC68();
void ov13_0222FD10();
void ov13_0222FE84();
void ov13_0222FED8();
void ov13_0222FF14();
void ov13_0222FF3C();
void ov13_0222FF50();

#define REG16(a)    (*(vu16 *)(a))
#define SET_PRI3(a) REG16(a) = (u16)((REG16(a) & ~3) | 3)

void ov13_0222DC24(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222DC58);
}

void ov13_0222DC58(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_0222BC84(ov13_0222DD5C);
    WifiSetupApp_SetStateFunc(ov13_0222DC88);
}

void ov13_0222DC88(void) {
    ov13_0222DC98();
    ov13_0222DC9C();
}

void ov13_0222DC98(void) {
}

void ov13_0222DC9C(void) {
}

void ov13_0222DCA0(void) {
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222DCC8);
}

void ov13_0222DCC8(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0222BC68() == 0) {
        return;
    }
    WifiSetupSignalIcon_Destroy();
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 1);
    if (ov13_0224F514 == 0) {
        WifiSetupApp_SetStateFunc(ov13_0222E754);
    } else if (ov13_0224F514 == 2) {
        WifiSetupApp_SetStateFunc(ov13_0222E56C);
    } else {
        WifiSetupApp_SetStateFunc(ov13_0222DDE4);
    }
}

void ov13_0222DD5C(int a) {
    if (a == 2) {
        ov13_0223A840();
        ov13_0224F514 = 1;
        ov13_0223AC20();
        WifiSetupSound_Play(0x10);
    } else if (a == 3) {
        ov13_0224F514 = 2;
        ov13_0223AC20();
        WifiSetupSound_Play(0x12);
    } else {
        ov13_0224F514 = 0;
        ov13_0223AC20();
        WifiSetupSound_Play(0x12);
    }
    ov13_0222BC84(NULL);
    ov13_0222BC0C();
    WifiSetupApp_SetStateFunc(ov13_0222DCA0);
}

void ov13_0222DDE4(void) {
    ov13_0222DE04();
    WifiSetup_PrintMessage(0x1f);
    WifiSetupApp_SetStateFunc(ov13_0222DE6C);
}

void ov13_0222DE04(void) {
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222DE6C(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222DEA0);
}

void ov13_0222DEA0(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(5);
    WifiSetupApp_SetStateFunc(ov13_0222DECC);
}

void ov13_0222DECC(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222DEF4);
}

void ov13_0222DEF4(void) {
    ov13_0222DF08();
    ov13_0222DF28();
    ov13_0222DF2C();
}

void ov13_0222DF08(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222DF28(void) {
}

void ov13_0222DF2C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(ov13_0222DF54);
}

void ov13_0222DF54(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222DF74);
}

void ov13_0222DF74(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupFade_Start(3, 1, 0x3f, 0x40);
    WifiSetupFade_Start(3, 0, 0x3f, 0x40);
    WifiSetupApp_SetStateFunc(ov13_0222DFC0);
}

void ov13_0222DFC0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

void ov13_0222DFF4(void) {
    ov13_0222E014();
    WifiSetup_PrintMessage(0x1a);
    WifiSetupApp_SetStateFunc(ov13_0222E07C);
}

void ov13_0222E014(void) {
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E07C(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222E0B0);
}

void ov13_0222E0B0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(4);
    WifiSetupApp_SetStateFunc(ov13_0222E0EC);
}

void ov13_0222E0EC(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222E114);
}

void ov13_0222E114(void) {
    ov13_0222E128();
    ov13_0222E148();
    ov13_0222E14C();
}

void ov13_0222E128(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222E148(void) {
}

void ov13_0222E14C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    WifiSetupApp_SetStateFunc(ov13_0222E174);
}

void ov13_0222E174(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222E194);
}

void ov13_0222E194(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222E1D0);
}

void ov13_0222E1D0(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 1);
    WifiSetupApp_SetStateFunc(ov13_0222EEB4);
}

void ov13_0222E220(void) {
    ov13_0224F518 = 0;
    ov13_0222E268();
    WifiSetupBackground_LoadHeader(0x13);
    WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetup_PrintMessage(0x1b);
    WifiSetupApp_SetStateFunc(ov13_0222E2E8);
}

void ov13_0222E268(void) {
    WifiSetup_LoadGraphics(ov13_0224629C, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E2E8(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222E33C);
}

void ov13_0222E33C(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(3);
    WifiSetupApp_SetStateFunc(ov13_0222E368);
}

void ov13_0222E368(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222E390);
}

void ov13_0222E390(void) {
    ov13_0222E3A4();
    ov13_0222E3DC();
    ov13_0222E3E0();
}

void ov13_0222E3A4(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222E3DC(void) {
}

void ov13_0222E3E0(void) {
    switch (WifiSetupButtonBar_GetSelected()) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        WifiSetupSound_Play(6);
        ov13_0224F518 = 1;
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222E430);
}

void ov13_0222E430(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222E450);
}

void ov13_0222E450(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F518 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222E4B4);
}

void ov13_0222E4B4(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F518 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F518 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F518 == 0) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222DFF4);
    }
}

void ov13_0222E56C(void) {
    ov13_0224F51C = 0;
    ov13_0222E59C();
    WifiSetup_PrintMessage(0x21);
    WifiSetupApp_SetStateFunc(ov13_0222E608);
}

void ov13_0222E59C(void) {
    WifiSetup_LoadGraphics(ov13_022462B0, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E608(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222E63C);
}

void ov13_0222E63C(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0222BC68() == 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222E66C);
}

void ov13_0222E66C(void) {
    ov13_0222E720();
    ov13_0222E67C();
}

void ov13_0222E67C(void) {
}

void ov13_0222E680(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222E6BC);
}

void ov13_0222E6BC(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 1);
    WifiSetupApp_SetStateFunc(ov13_0222D454);
}

void ov13_0222E720(void) {
    if (++ov13_0224F51C < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222E680);
}

void ov13_0222E754(void) {
    ov13_0222E790();
    WifiSetup_PrintMessage(0x20);
    ov13_0224F520[0] = WifiSetupMessage_Show(0x3c, -1, 0);
    WifiSetupApp_SetStateFunc(ov13_0222E810);
}

void ov13_0222E790(void) {
    WifiSetup_LoadGraphics(ov13_022462C4, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222E810(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    if (ov13_0224F520[0] != 0) {
        WifiSetupFade_Start(2, 1, 1, 8);
        WifiSetupDisplay_ShowPlanes(1, 1);
    }
    WifiSetupApp_SetStateFunc(ov13_0222E878);
}

void ov13_0222E878(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(3);
    WifiSetupApp_SetStateFunc(ov13_0222E8B4);
}

void ov13_0222E8B4(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    if (ov13_0222BC68() == 0) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    WifiSetupApp_SetStateFunc(ov13_0222E8E8);
}

void ov13_0222E8E8(void) {
    ov13_0222E8FC();
    ov13_0222E934();
    ov13_0222E938();
}

void ov13_0222E8FC(void) {
    if (WifiSetupInput_IsKeyPressed(1) != 0) {
        WifiSetupButtonBar_SelectIfNone(1);
    }
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222E934(void) {
}

void ov13_0222E938(void) {
    int r = WifiSetupButtonBar_GetSelected();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        ov13_0224F520[1] = 1;
        WifiSetupSound_Play(6);
    } else {
        ov13_0224F520[1] = 0;
        WifiSetupSound_Play(7);
    }
    WifiSetupApp_SetStateFunc(ov13_0222E990);
}

void ov13_0222E990(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222E9B0);
}

void ov13_0222E9B0(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F520[1] == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222EA14);
}

void ov13_0222EA14(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F520[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F520[1] == 0) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222EEB4);
    }
}

void ov13_0222EABC(void) {
    u16 buf[11];
    u8 *src = ov13_0222BCA8();
    int win = WifiSetupText_InitBgCanvas(0, 0);
    ov13_0224F524[1] = 0;
    ov13_0224F524[0] = 0;
    ov13_0222EB8C();
    MIi_CpuClear16(0, buf, sizeof(buf));
    MIi_CpuCopy16((u16 *)(src + 2), buf, src[1] * 2);
    WifiSetupText_DrawInRect(win, 8, 0x35, 0xf0, 0x1c, 2, 0x480, buf);
    WifiSetupText_MarkDirty(win);
    ov13_0222BC84(ov13_0222EE80);
    ov13_0223BE6C(0x1d, 4, 0, -1, 0);
    WifiSetupApp_SetStateFunc(ov13_0222EC0C);
}

void ov13_0222EB8C(void) {
    WifiSetup_LoadGraphics(ov13_022462D8, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222EC0C(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222EC40);
}

void ov13_0222EC40(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222EC64);
}

void ov13_0222EC64(void) {
    if (ov13_0223C1DC() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222EC88);
}

void ov13_0222EC88(void) {
    ov13_0222ECF8();
    ov13_0222ECFC();
    switch (ov13_0223C1DC()) {
    case 0:
        ov13_0224F524[1] = 0;
        WifiSetupSound_Play(7);
        break;
    case 1:
        ov13_0224F524[1] = 1;
        WifiSetupSound_Play(0xe);
        break;
    default:
        return;
    }
    ov13_0222BC84(NULL);
    ov13_0223C198();
    WifiSetupApp_SetStateFunc(ov13_0222ED00);
}

void ov13_0222ECF8(void) {
}

void ov13_0222ECFC(void) {
}

void ov13_0222ED00(void) {
    if (ov13_0223C1F0() != 0) {
        return;
    }
    if (ov13_0224F524[1] == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222ED5C);
}

void ov13_0222ED5C(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F524[0] == 0 && ov13_0224F524[1] == 1) {
        ov13_0222BC9C();
    } else {
        ov13_0222BC0C();
    }
    WifiSetupApp_SetStateFunc(ov13_0222EDB8);
}

void ov13_0222EDB8(void) {
    if (ov13_0224F524[0] != 0 || ov13_0224F524[1] == 0) {
        if (ov13_0222BC68() == 0) {
            return;
        }
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F524[1] == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    ov13_0222B8F8(0, 1);
    if (ov13_0224F524[0] != 0) {
        WifiSetupApp_SetStateFunc(ov13_0222E754);
    } else if (ov13_0224F524[1] == 0) {
        WifiSetupDisplay_HidePlanes(1, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else {
        WifiSetupApp_SetStateFunc(ov13_0222DB6C);
    }
}

void ov13_0222EE80(void) {
    ov13_0224F524[0] = 1;
}

int ov13_0222EE94(void) {
    return ((*(vu16 *)0x027FFFA8 & 0x8000) >> 15) ? TRUE : FALSE;
}

void ov13_0222EEB4(void) {
    ov13_0222BB20(ov13_0222F230);
    ov13_0224F528 = 0;
    ov13_0222EF00();
    WifiSetup_PrintMessage(0x1c);
    WifiSetupBusyIcon_Create(0);
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(ov13_0222EF80);
}

void ov13_0222EF00(void) {
    WifiSetup_LoadGraphics(ov13_022462EC, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222EF80(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222EFB4);
}

void ov13_0222EFB4(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(1);
    WifiSetupApp_SetStateFunc(ov13_0222EFE0);
}

void ov13_0222EFE0(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F004);
}

void ov13_0222F004(void) {
    ov13_0222F018();
    ov13_0222F050();
    ov13_0222F054();
}

void ov13_0222F018(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_0222EE94() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222F050(void) {
}

void ov13_0222F054(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    ov13_0223AC20();
    WifiSetupSound_Play(7);
    WifiSetupApp_SetStateFunc(ov13_0222F080);
}

void ov13_0222F080(void) {
    if (ov13_0224F528 == 0) {
        ov13_0222BC0C();
    }
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222F0B8);
}

void ov13_0222F0B8(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (ov13_0222BC68() == 0) {
            return;
        }
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F528 == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222F138);
}

void ov13_0222F138(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    if (ov13_0224F528 == 0) {
        if (ov13_0222BC68() == 0) {
            return;
        }
    }
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F528 == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F528 == 0) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222D454);
    } else if (ov13_0224F528 == 2) {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222E754);
    } else {
        ov13_0222B8F8(0, 1);
        WifiSetupApp_SetStateFunc(ov13_0222EABC);
    }
}

void ov13_0222F230(int a) {
    ov13_0223AC20();
    if (a == 0) {
        ov13_0224F528 = 1;
        WifiSetupSound_Play(0x10);
    } else {
        ov13_0224F528 = 2;
        WifiSetupSound_Play(0x12);
    }
    ov13_0222BC84(NULL);
    WifiSetupApp_SetStateFunc(ov13_0222F080);
}

int ov13_0222F28C(void) {
    return ((*(vu16 *)0x027FFFA8 & 0x8000) >> 15) ? TRUE : FALSE;
}

void ov13_0222F2AC(void) {
    ov13_0224F52C.counter = 0;
    ov13_0222F30C();
    ov13_02227E40();
    WifiSetupMessage_Show(0x36, -1, 0);
    ov13_02227964(2);
    WifiSetup_PrintMessage(0x22);
    WifiSetupBusyIcon_Create(0);
    WifiSetupApp_StartClient();
    WifiSetupSound_Play(0xb);
    WifiSetupApp_SetStateFunc(ov13_0222F3B4);
}

void ov13_0222F30C(void) {
    WifiSetup_LoadGraphics(ov13_02246300, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246318, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246330, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222F3B4(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222F408);
}

void ov13_0222F408(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(1);
    WifiSetupApp_SetStateFunc(ov13_0222F444);
}

void ov13_0222F444(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    ov13_0224F52C.task = WifiSetupTask_Create(1, ov13_0222F764, 0, 0x78);
    WifiSetupApp_SetStateFunc(ov13_0222F48C);
}

void ov13_0222F48C(void) {
    ov13_0222F560();
    ov13_0222F598();
    ov13_0222F59C();
    switch (WifiSetupApp_PollClient()) {
    case 0:
        return;
    case 1:
        if (ov13_0224F52C.task == NULL) {
            return;
        }
        ov13_0224F52C.flag = 1;
        WifiSetupTask_Destroy(1, ov13_0224F52C.task);
        ov13_0224F52C.task = NULL;
        WifiSetupApp_SetStateFunc(ov13_0222F7CC);
        break;
    case 2:
        if (ov13_0224F52C.task == NULL) {
            return;
        }
        ov13_0223AC20();
        ov13_0223BE6C(0xc, 1, 1, -1, 0);
        WifiSetupSound_Play(9);
        WifiSetupButtonBar_Lock();
        WifiSetupTask_Destroy(1, ov13_0224F52C.task);
        ov13_0224F52C.task = NULL;
        WifiSetupApp_SetStateFunc(ov13_0222F780);
        break;
    }
}

void ov13_0222F560(void) {
    if (WifiSetupInput_IsKeyPressed(2) != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_0222F28C() != 0) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_0222F598(void) {
}

void ov13_0222F59C(void) {
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    if (ov13_0224F52C.task == NULL) {
        return;
    }
    WifiSetupTask_DestroyLater(1, ov13_0224F52C.task);
    ov13_0224F52C.task = NULL;
    WifiSetupButtonBar_Lock();
    WifiSetupApp_SetStateFunc(ov13_0222F818);
}

void ov13_0222F5E8(void) {
    WifiSetupButtonBar_Lock();
    if (ov13_0224F52C.task != NULL) {
        WifiSetupTask_Destroy(1, ov13_0224F52C.task);
    }
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(ov13_0222F624);
}

void ov13_0222F624(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F52C.flag == 0) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222F688);
}

void ov13_0222F688(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F52C.flag == 0) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    WifiSetupApp_StopClient(ov13_0224F52C.flag != 0 ? 1 : 0);
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F52C.flag == 0) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F52C.flag == 0) {
        ov13_0222B8F8(2, 1);
        WifiSetupApp_SetStateFunc(ov13_02237618);
    } else {
        ov13_0222B8F8(0, 0);
        WifiSetupApp_SetStateFunc(ov13_0222F848);
    }
}

void ov13_0222F764(void) {
    WifiSetupInput_Update();
    WifiSetupTask_RunQueue(0);
    ov13_0222F560();
    ov13_0222F59C();
}

void ov13_0222F780(void) {
    if (ov13_0223C1DC() != 0) {
        return;
    }
    WifiSetupSound_Play(6);
    ov13_0223C198();
    WifiSetupApp_SetStateFunc(ov13_0222F7AC);
}

void ov13_0222F7AC(void) {
    if (ov13_0223C1F0() != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F5E8);
}

void ov13_0222F7CC(void) {
    ov13_0222F560();
    ov13_0222F598();
    ov13_0222F59C();
    ov13_0224F52C.counter++;
    if (ov13_0224F52C.counter < 0x438) {
        return;
    }
    ov13_0223AC20();
    WifiSetupApp_SetStateFunc(ov13_0222F5E8);
}

void ov13_0222F818(void) {
    ov13_0223AC20();
    WifiSetupSound_Play(7);
    ov13_0224F52C.flag = 0;
    WifiSetupApp_SetStateFunc(ov13_0222F5E8);
}

void ov13_0222F848(void) {
    ov13_0224F534 = 0;
    ov13_0222F884();
    ov13_02227E40();
    WifiSetup_PrintMessage(0x23);
    WifiSetupSound_Play(0x10);
    WifiSetupApp_SetStateFunc(ov13_0222F8F0);
}

void ov13_0222F884(void) {
    WifiSetup_LoadGraphics(ov13_02246344, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

void ov13_0222F8F0(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    WifiSetupApp_SetStateFunc(ov13_0222F924);
}

void ov13_0222F924(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F948);
}

void ov13_0222F948(void) {
    ov13_0222FA08();
    ov13_0222F958();
}

void ov13_0222F958(void) {
}

void ov13_0222F95C(void) {
    WifiSetupFade_Start(3, 1, 1, 8);
    WifiSetupFade_Start(3, 0, 0x15, 8);
    WifiSetupApp_SetStateFunc(ov13_0222F998);
}

void ov13_0222F998(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupMessage_Hide();
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 0x15);
    ov13_0222B8F8(0, 0);
    ov13_0222B934(0, 1);
    WifiSetupApp_SetStateFunc(ov13_02238440);
}

void ov13_0222FA08(void) {
    if (++ov13_0224F534 < 0x78) {
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_0222F95C);
}

void ov13_0222FA3C(void) {
    int result;
    ov13_0224F538.big = WifiSetupHeap_AllocZeroed(0x5c, 4);
    ov13_0222B948(&result, 0);
    if (result == 0) {
        ov13_0224F538.unk0 = 0;
        ov13_0224F538.unk2 = 0;
    }
    ov13_0224F538.big->unk51 = ov13_02239D04(ov13_0224F538.big);
    ov13_0222FB14();
    ov13_02227E40();
    WifiSetupMessage_Show(0x34, -1, 0);
    ov13_02227964(2);
    ov13_0222FC68();
    ov13_0222FD10();
    ov13_0224F538.big->unkC = WifiSetupText_InitBgCanvas(0, 0);
    ov13_0224F538.big->unk3C = WifiSetupTask_Create(1, ov13_02230C68, 0, 0x6e);
    ov13_022307AC();
    ov13_02230CC4();
    WifiSetupApp_SetStateFunc(ov13_0222FE84);
}

void ov13_0222FB14(void) {
    struct Str25 a;
    struct Str22 b;
    a = ov13_022453E3;
    b = ov13_022453CD;
    WifiSetup_LoadGraphics(ov13_02246358, GX_LoadBG3Scr);
    ov13_0224F538.big->unk4 = WifiSetupFile_Load(WifiSetup_LocalizePath(&a), 0, 4);
    ov13_02239228(ov13_0224F538.big->unk4);
    ov13_022392B4();
    ov13_0224F538.big->unk8 = WifiSetupFile_Load(WifiSetup_LocalizePath(&b), 0, 4);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    REG16(0x0400000A) = (u16)((REG16(0x0400000A) & ~3) | 2);
    SET_PRI3(0x0400000C);
    REG16(0x0400000E) = (u16)((REG16(0x0400000E) & ~3) | 2);
}

void ov13_0222FC68(void) {
    int mode;
    int val;
    struct UnkBig *big = ov13_0224F538.big;
    big->unk40 = (big->unk51 - 4) * 0x1c;
    big = ov13_0224F538.big;
    val = 0;
    if (big->unk51 <= 4) {
        mode = 0;
        big->unk53 = 0;
    } else if (big->unk51 <= 8) {
        big->unk53 = 0x1f;
        mode = 1;
    } else {
        big->unk53 = 0x37;
        mode = 2;
    }
    if (mode != 0) {
        val = FX_DivS32(ov13_0224F538.unk2 * ov13_0224F538.big->unk53, ov13_0224F538.big->unk40);
    }
    ov13_0223943C(mode, 0x55, 0xec, 0x3f, val);
}
