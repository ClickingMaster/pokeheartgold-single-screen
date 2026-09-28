#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

typedef void (*StateFunc)(void);

extern int ov13_02240394(int a);
extern void ov13_022403B8(int a, int b, int c, int d);
extern void ov13_02240EA8(int a, int b);
extern void ov13_02240EFC(int a, int b);
extern void ov13_02240B04(int a);
extern void ov13_022405B8(int a);
extern int ov13_02241694(int a);
extern void ov13_02227F68(void);
extern void ov13_02227028(void);
extern void ov13_0223A840(void);
extern void ov13_0223AC20(void);
extern void ov13_0223ABB8(int a);
extern void ov13_022279EC(int a);
extern void ov13_022279B4(int a);
extern int ov13_02227258(void);
extern void ov13_022272CC(void);
extern void ov13_0222726C(int a);
extern void ov13_022272E4(void);
extern void ov13_02227228(void);
extern int ov13_022272A0(void);
extern void ov13_02227DE4(int a);
extern int ov13_022277B8(int a, int b, int c);
extern void ov13_02227924(void *a, void *b);
extern void ov13_0222788C(void);

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
extern void ov13_02238440();
extern void *ov13_0224128C(int size, int align);
extern u8 ov13_02239D04(void *p);
extern void ov13_02230C68();
extern void ov13_022307AC(void);
extern void ov13_02230CC4(void);
extern void *ov13_022278C4(void *a);
extern void *ov13_0223F95C(void *a, void *b, int c);
extern void ov13_02239228(void *a);
extern void ov13_022392B4(void);
extern void ov13_0223943C(int a, int b, int c, int d, int e);
extern struct UnkOam *ov13_02240184(int a, int b);
extern void ov13_022400F0(int a, int b, struct UnkOam *c);
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
extern void ov13_0222B254(void);
extern void *ov13_02242528(int a, void *b, int c, int d);
extern int ov13_0222B3B4(void);
extern void ov13_022425F0(int a, void *b);
extern void ov13_022425D0(int a, void *b);
extern void ov13_0222B364(int a);
extern void ov13_02237618();
extern void ov13_022413E4(void);
extern void ov13_02242478(int a);
extern u8 ov13_022462D8[];
extern u8 ov13_022462EC[];
extern int ov13_022408A0(int a, int b);
extern void ov13_02240CC0(int a, int b, int c, int d, int e, int f, int g, void *h);
extern void ov13_02240E2C(int a);
extern void ov13_0223BE6C(int a, int b, int c, int d, int e);
extern int ov13_0223C1DC(void);
extern void ov13_0223C198(void);
extern int ov13_0223C1F0(void);
extern void ov13_02226F58(int a);
extern u8 ov13_0224629C[];
extern u8 ov13_022462B0[];
extern u8 ov13_022462C4[];


struct UnkF4CC {
    u8 state;
    u8 flag1;
    u8 pad2[6];
    u32 lo : 4;
    u32 hi : 28;
    void *func;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
};
extern struct UnkF4CC ov13_0224F4CC;
struct UnkF4EC {
    u8 pad[0x60];
    void *unk60;
};
extern struct UnkF4EC *ov13_0224F4EC;
struct UnkRes {
    u32 *offsets;
    u8 *base;
    void *data;
};
extern struct UnkRes *ov13_0224F4F8;
struct UnkF4F0 {
    u8 pad[0x1E280];
    u8 unk1E280[0x18];
    void (*callback)(int);
    void *task;
    u8 unk1E2A0;
    u8 unk1E2A1;
};
extern struct UnkF4F0 *ov13_0224F4F0;
struct Unk1C {
    int unk0;
    void *unk4;
    void *unk8;
    int unkC;
    int unk10;
    int unk14;
    u8 unk18;
};
extern struct Unk1C ov13_02245F8C;
struct Unk0C {
    int state;
    int unk4;
    int unk8;
};
extern struct Unk0C *ov13_0224F4F4;
struct UnkE8 {
    u8 pad[0x20];
    int unk20;
    int unk24;
    u8 pad28[0xC0];
};
extern void GX_DispOff(void);
extern void ov13_0224230C(void);
extern void ov13_022413AC(void);
extern void ov13_02241B34(void);
extern void ov13_0223AB8C(void);
extern void ov13_0223FBB0(void);
extern void ov13_02241D4C(void);
extern void ov13_0224074C(void);
extern void ov13_02240380(void);
extern void ov13_0223F814(void);
extern void ov13_02242430(void);
extern void ov13_0224121C(void);
extern void ov13_02242154(void);
extern void *ov13_0224022C(int a, void *b, int c);
extern void ov13_022412B0(void *pp);
extern void *ov13_022402E0(void *a);
extern void ov13_0223FA50(void *a);
extern void ov13_02240288(void *a, void *b);
extern void ov13_0223E80C(void *a, struct Unk1C *b);
extern int ov13_0223EA58(void);
extern void OS_Terminate(void);
extern int ov13_0223E8FC(void);
extern void ov13_0223EAA0(void);
extern int ov13_02226D74(int a, int b, void *c, void *d, void *e, int f);
extern int ov13_02226E5C(void);
extern int ov13_02226F28(struct UnkE8 *a);
extern void ov13_0223A5D0(struct UnkE8 *a);
extern void ov13_022412F4(void *a);
extern void ov13_02242640(int a, int b);
extern void ov13_022400C8(int a);
extern void ov13_02227DD0(void);
extern void ov13_02227B54(void);
extern void ov13_02239FDC(void);
extern void ov13_02239F98(void);
extern void ov13_02227B34(void);
extern void ov13_02227D58(void);
extern void ov13_022277A4(void);
extern void ov13_02240098(int a, void *b);
extern void *ov13_02246018[];
extern u8 ov13_02246034[];
extern u8 ov13_02246044[];
extern u8 ov13_02246058[];
extern u8 ov13_0224606C[];
extern u8 ov13_02246080[];
extern u8 ov13_02246094[];
extern u8 ov13_022460AC[];
extern u8 ov13_022460C4[];
extern u8 ov13_022460DC[];
extern u8 ov13_022460F4[];
extern u8 ov13_0224610C[];
extern u8 ov13_02246124[];
extern u8 ov13_02246138[];
extern u8 ov13_0224614C[];
extern u8 ov13_02246164[];
extern u8 ov13_0224617C[];
extern void ov13_022368D8();
extern void ov13_0223B6F4(int a);
extern void ov13_02227C28(int a, int b, int c, int d);
extern u16 ov13_0224533E[];
extern u16 ov13_02245340[];
extern u16 ov13_02245342[];
extern u16 ov13_02245344[];
struct UnkF4FC {
    u8 sel;
    void *unk4;
};
extern struct UnkF4FC ov13_0224F4FC;
extern struct Str22 ov13_0224534E;
struct Unk8 {
    u8 data[8];
};
extern struct Unk8 ov13_0224532E[];
extern int ov13_022417DC(struct Unk8 *a);
extern int ov13_022416D4(int a);
extern void ov13_02227D18(void);
struct Unk2 {
    u8 data[2];
};
extern struct Unk2 ov13_0224532C;
extern void ov13_02239378(void *a, int b, int c);
extern u8 ov13_0224F504;
extern u8 ov13_02246190[];
extern u8 ov13_022461A4[];
extern u8 ov13_022461B8[];
struct UnkF508 {
    u8 sel;
    u8 unk1;
    void *unk4;
    struct UnkOam *unk8;
};
extern struct UnkF508 ov13_0224F508;
extern u16 ov13_022453B2[];
extern u16 ov13_022453B4[];
extern u16 ov13_022453B6[];
extern u16 ov13_022453B8[];
extern struct Unk8 ov13_0224539A[];
extern u8 ov13_02245364[];
extern struct Str22 ov13_0224536C;
struct Str23 {
    char s[23];
};
extern struct Str23 ov13_02245382;
extern u8 ov13_02246258[];
extern u8 ov13_02246270[];
extern u8 ov13_02246288[];
struct WiFiInfo {
    u64 id;
    u8 pad[0xC];
};
extern void DWCi_BM_GetWiFiInfo(struct WiFiInfo *info);
extern void ov13_0222728C(int a);
extern void ov13_02241D98(void *a);
extern s32 FX_ModS32(s32 a, s32 b);
extern void ov13_02227EB0(int a);
extern void ov13_0222E220();
extern void ov13_0222DC24();
extern u16 ov13_022461CC[];
extern u16 ov13_02246208[];
extern u16 ov13_02246230[];
extern int swprintf(u16 *buf, u32 n, const u16 *fmt, ...);
extern void ov13_0223EAD4(void);
extern void ov13_0223EE58(u8 *type, u8 *flag);
extern void *ov13_0223EE9C(void);

void ov13_0222B898();
void ov13_0222B8E8();
void ov13_0222B8F8();
void ov13_0222B90C();
void ov13_0222B934();
void ov13_0222B948();
u8 ov13_0222B970();
u32 ov13_0222B980();
BOOL ov13_0222B998();
void ov13_0222B9B4();
u8 ov13_0222B9C8();
void ov13_0222BA08();
void ov13_0222BA44();
struct UnkRes *ov13_0222BA58();
void ov13_0222BAB0();
u8 *ov13_0222BADC(struct UnkRes *res, int idx);
void ov13_0222BAF4(struct UnkRes *res, int idx, int pos, int ch);
void ov13_0222BB20();
void ov13_0222BC0C();
void ov13_0222BC2C();
BOOL ov13_0222BC68();
void ov13_0222BC84();
void ov13_0222BC9C();
u8 *ov13_0222BCA8();
void ov13_0222BCC0();
void ov13_0222BF60();
void ov13_0222BFCC();
int ov13_0222BFF0();
void ov13_0222C0AC();
void ov13_0222C0D8();
void *ov13_0222C0F0();
void ov13_0222C100();
void ov13_0222C10C();
void ov13_0222C148();
void ov13_0222C1BC();
void ov13_0222C39C();
void ov13_0222C3D8();
void ov13_0222C44C();
void ov13_0222C4C8();
void ov13_0222C5B4();
void ov13_0222C608();
void ov13_0222C644();
void ov13_0222C66C();
void ov13_0222C680();
void ov13_0222C7AC();
void ov13_0222C7B0();
void ov13_0222C800();
void ov13_0222C820();
void ov13_0222C86C();
void ov13_0222C914();
void ov13_0222C984();
void ov13_0222C9C4();
void ov13_0222CA0C();
void ov13_0222CA8C();
void ov13_0222CAE0();
void ov13_0222CB1C();
void ov13_0222CB44();
void ov13_0222CB58();
void ov13_0222CB90();
void ov13_0222CB94();
void ov13_0222CBFC();
void ov13_0222CC1C();
void ov13_0222CC80();
void ov13_0222CD38();
void ov13_0222CD88();
void ov13_0222CDA8();
void ov13_0222CDCC();
void ov13_0222CE4C();
void ov13_0222CE80();
void ov13_0222CEAC();
void ov13_0222CED4();
void ov13_0222CEE8();
void ov13_0222CF08();
void ov13_0222CF0C();
void ov13_0222CF34();
void ov13_0222CF54();
void ov13_0222CFA0();
void ov13_0222CFD4();
void ov13_0222D008();
void ov13_0222D088();
void ov13_0222D25C();
void ov13_0222D2B0();
void ov13_0222D2EC();
void ov13_0222D314();
void ov13_0222D328();
void ov13_0222D348();
void ov13_0222D34C();
void ov13_0222D374();
void ov13_0222D394();
void ov13_0222D3E4();
void ov13_0222D454();
void ov13_0222D51C();
void ov13_0222D6B8();
void ov13_0222D70C();
void ov13_0222D748();
void ov13_0222D770();
void ov13_0222D784();
void ov13_0222D878();
void ov13_0222D87C();
void ov13_0222D920();
void ov13_0222D940();
void ov13_0222D9A4();
void ov13_0222DAB0();
void ov13_0222DB40();
void ov13_0222DB6C();
void ov13_0222DBA4();

#define REG16(a) (*(vu16 *)(a))
#define SET_PRI3(a) REG16(a) = (u16)((REG16(a) & ~3) | 3)


// @fn ov13_0222BF60
void ov13_0222BF60(void) {
    ov13_0224F4F4 = ov13_0224128C(0xc, -4);
    if (ov13_02226D74(0xf, 0x40, ov13_0222C0D8, ov13_0222C0F0, ov13_0222C100, 0x800) != 1) {
        OS_Terminate();
    }
    OS_Sleep(10);
}

// @fn ov13_0222BFCC
void ov13_0222BFCC(void) {
    if (ov13_02226E5C() != 1) {
        OS_Terminate();
    }
    ov13_022412B0(&ov13_0224F4F4);
}

// @fn ov13_0222BFF0
int ov13_0222BFF0(void) {
    struct UnkE8 buf;
    switch (ov13_0224F4F4->state) {
    case 0:
    case 1:
    case 3:
    case 5:
        return 0;
    case 2:
        return 1;
    case 4:
        return 2;
    case 6:
        if (ov13_02226F28(&buf) != 1) {
            OS_Terminate();
        }
        if (buf.unk20 >= 0 && buf.unk20 <= 3 && buf.unk24 == 1) {
            return 3;
        }
        return 5;
    case 7:
        return 4;
    }
}

// @fn ov13_0222C0AC
void ov13_0222C0AC(void) {
    struct UnkE8 buf;
    if (ov13_02226F28(&buf) != 1) {
        OS_Terminate();
    }
    ov13_0223A5D0(&buf);
}

// @fn ov13_0222C0D8
void ov13_0222C0D8(struct Unk0C *src) {
    *ov13_0224F4F4 = *src;
}

// @fn ov13_0222C0F0
void *ov13_0222C0F0(int size) {
    return ov13_0224128C(size, 0x20);
}

// @fn ov13_0222C100
void ov13_0222C100(void *p) {
    ov13_022412F4(p);
}

// @fn ov13_0222C10C
void ov13_0222C10C(void) {
    ov13_022403B8(3, 1, 0x3f, 0x14);
    ov13_022403B8(3, 0, 0x3f, 0x14);
    ov13_0222B8E8(ov13_0222C148);
}

// @fn ov13_0222C148
void ov13_0222C148(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_02242640(0, 0);
    ov13_02242640(1, 0);
    ov13_022400C8(1);
    ov13_022400C8(0);
    ov13_02227DD0();
    ov13_02227B54();
    ov13_0222BAB0(ov13_0224F4F8);
    ov13_0222BA44();
    ov13_02239FDC();
    ov13_0222B9B4();
}

// @fn ov13_0222C1BC
void ov13_0222C1BC(void) {
    ov13_02239F98();
    ov13_0222BA08();
    ov13_02227B34();
    ov13_02227D58();
    ov13_022277A4();
    if (ov13_0222B970() == 1 && ov13_0222B998(2) != 0) {
        ov13_0224F4F8 = ov13_0222BA58(ov13_02246034);
    } else {
        ov13_0224F4F8 = ov13_0222BA58(ov13_02246018[ov13_0222B970()]);
    }
    ov13_02240098(1, ov13_022278C4(ov13_02246044));
    ov13_02240098(0, ov13_022278C4(ov13_02246058));
    ov13_02227924(ov13_0224606C, GXS_LoadBG1Char);
    ov13_02227924(ov13_02246080, GXS_LoadBGPltt);
    ov13_02227924(ov13_02246094, GXS_LoadOBJ);
    ov13_02227924(ov13_022460AC, GXS_LoadOBJPltt);
    ov13_02227924(ov13_022460C4, GX_LoadBG2Char);
    ov13_02227924(ov13_022460DC, GX_LoadBGPltt);
    ov13_02227924(ov13_022460F4, GX_LoadOBJ);
    ov13_02227924(ov13_0224610C, GX_LoadOBJPltt);
    switch (ov13_0222B980()) {
    case 0:
        ov13_02227924(ov13_02246124, GXS_LoadBG1Scr);
        break;
    case 1:
        ov13_02227924(ov13_02246138, GXS_LoadBG1Scr);
        break;
    }
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000A);
    ov13_02240EA8(1, 2);
    ov13_02240EA8(0, 2);
    ov13_0222B8E8(ov13_0222C39C);
}

// @fn ov13_0222C39C
void ov13_0222C39C(void) {
    ov13_022403B8(2, 1, 2, 0x14);
    ov13_022403B8(2, 0, 2, 0x14);
    ov13_0222B8E8(ov13_0222C3D8);
}

// @fn ov13_0222C3D8
void ov13_0222C3D8(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    switch (ov13_0222B980()) {
    case 0:
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_0222C44C);
        break;
    case 1:
        ov13_0222B8F8(1, 1);
        ov13_0222B8E8(ov13_022368D8);
        break;
    }
}

// @fn ov13_0222C44C
void ov13_0222C44C(void) {
    int i;
    ov13_0222C4C8();
    ov13_02227DE4(0);
    ov13_022277B8(0x2e, -1, 0);
    ov13_0223B6F4(4);
    i = ov13_0224F4FC.sel * 4;
    ov13_02227C28(ov13_0224533E[i], ov13_02245342[i], ov13_02245340[i], ov13_02245344[i]);
    ov13_0222B8E8(ov13_0222C5B4);
}

// @fn ov13_0222C4C8
void ov13_0222C4C8(void) {
    struct Str22 name;
    name = ov13_0224534E;
    ov13_02227924(ov13_0224614C, GX_LoadBG2Char);
    ov13_02227924(ov13_02246164, GX_LoadBGPltt);
    ov13_02227924(ov13_0224617C, GX_LoadBG2Scr);
    ov13_0224F4FC.unk4 = ov13_0223F95C(ov13_022278C4(&name), NULL, 4);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

// @fn ov13_0222C5B4
void ov13_0222C5B4(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x14, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x14);
    ov13_0222B8E8(ov13_0222C608);
}

// @fn ov13_0222C608
void ov13_0222C608(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_022279B4(0);
    ov13_0222B8E8(ov13_0222C644);
}

// @fn ov13_0222C644
void ov13_0222C644(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_0222C66C);
}

// @fn ov13_0222C66C
void ov13_0222C66C(void) {
    ov13_0222C680();
    ov13_0222C7AC();
    ov13_0222C7B0();
}

// @fn ov13_0222C680
void ov13_0222C680(void) {
    u32 i;
    struct Unk8 *entry = ov13_0224532E;
    for (i = 0; i < 2; i++, entry++) {
        if (ov13_022417DC(entry) != 0) {
            int idx;
            ov13_0222726C(1);
            ov13_0224F4FC.sel = i;
            idx = ov13_0224F4FC.sel * 4;
            ov13_02227C28(ov13_0224533E[idx], ov13_02245342[idx], ov13_02245340[idx], ov13_02245344[idx]);
            return;
        }
    }
    if (ov13_02241694(1) != 0) {
        ov13_0222726C(1);
        return;
    }
    if (ov13_02241694(2) != 0) {
        ov13_0222726C(0);
        return;
    }
    if (ov13_022416D4(0x40) != 0) {
        ov13_0222C914(1);
        return;
    }
    if (ov13_022416D4(0x80) != 0) {
        ov13_0222C914(3);
        return;
    }
    if (ov13_022416D4(0x20) != 0) {
        ov13_0222C914(0);
        return;
    }
    if (ov13_022416D4(0x10) != 0) {
        ov13_0222C914(2);
    }
}

// @fn ov13_0222C7AC
void ov13_0222C7AC(void) {
}

// @fn ov13_0222C7B0
void ov13_0222C7B0(void) {
    switch (ov13_02227258()) {
    case 0:
        ov13_0223ABB8(7);
        ov13_0222B8E8(ov13_0222C10C);
        break;
    case 1:
        ov13_0223ABB8(6);
        ov13_0222C984();
        ov13_0222B8E8(ov13_0222C800);
        break;
    }
}

// @fn ov13_0222C800
void ov13_0222C800(void) {
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_0222C820);
}

// @fn ov13_0222C820
void ov13_0222C820(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    ov13_022403B8(3, 1, 1, 8);
    ov13_022403B8(3, 0, 0x16, 8);
    ov13_0222B8E8(ov13_0222C86C);
}

// @fn ov13_0222C86C
void ov13_0222C86C(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_02227D18();
    ov13_0222788C();
    ov13_0223FA50(ov13_0224F4FC.unk4);
    ov13_02240EFC(1, 1);
    ov13_02240EFC(0, 0x14);
    switch (ov13_0224F4FC.sel) {
    case 0:
        ov13_0222B8F8(1, 0);
        ov13_0222B8E8(ov13_022368D8);
        break;
    case 1:
        ov13_0222B8F8(0, 0);
        ov13_0222B8E8(ov13_0222D454);
        break;
    }
}

// @fn ov13_0222C914
void ov13_0222C914(int dir) {
    int idx;
    if (dir == 1 || dir == 3) {
        return;
    }
    ov13_0224F4FC.sel ^= 1;
    ov13_0223ABB8(8);
    idx = ov13_0224F4FC.sel * 4;
    ov13_02227C28(ov13_0224533E[idx], ov13_02245342[idx], ov13_02245340[idx], ov13_02245344[idx]);
}

// @fn ov13_0222C984
void ov13_0222C984(void) {
    struct Unk2 tbl = ov13_0224532C;
    ov13_02239378(ov13_0224F4FC.unk4, tbl.data[ov13_0224F4FC.sel], tbl.data[ov13_0224F4FC.sel]);
}

// @fn ov13_0222C9C4
void ov13_0222C9C4(void) {
    ov13_0224F504 = 0;
    ov13_0222CA0C();
    ov13_02227DE4(0x12);
    ov13_022277B8(0x3b, -1, 0);
    ov13_022279EC(0x17);
    ov13_0222B8E8(ov13_0222CA8C);
}

// @fn ov13_0222CA0C
void ov13_0222CA0C(void) {
    ov13_02227924(ov13_02246190, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

// @fn ov13_0222CA8C
void ov13_0222CA8C(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x15, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x15);
    ov13_0222B8E8(ov13_0222CAE0);
}

// @fn ov13_0222CAE0
void ov13_0222CAE0(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_022279B4(3);
    ov13_0222B8E8(ov13_0222CB1C);
}

// @fn ov13_0222CB1C
void ov13_0222CB1C(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_0222CB44);
}

// @fn ov13_0222CB44
void ov13_0222CB44(void) {
    ov13_0222CB58();
    ov13_0222CB90();
    ov13_0222CB94();
}

// @fn ov13_0222CB58
void ov13_0222CB58(void) {
    if (ov13_02241694(1) != 0) {
        ov13_0222726C(1);
    }
    if (ov13_02241694(2) != 0) {
        ov13_0222726C(0);
    }
}

// @fn ov13_0222CB90
void ov13_0222CB90(void) {
}

// @fn ov13_0222CB94
void ov13_0222CB94(void) {
    switch (ov13_02227258()) {
    case 0:
        ov13_0223ABB8(7);
        ov13_0222B8E8(ov13_0222CBFC);
        break;
    case 1:
        ov13_0223ABB8(6);
        ov13_0223BE6C(0x18, 0, 1, -1, 0);
        ov13_022272E4();
        ov13_0222B8E8(ov13_0222CD38);
        break;
    }
}

// @fn ov13_0222CBFC
void ov13_0222CBFC(void) {
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_0222CC1C);
}

// @fn ov13_0222CC1C
void ov13_0222CC1C(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    ov13_02227228();
    if (ov13_0224F504 == 0) {
        ov13_022403B8(3, 1, 1, 8);
    }
    ov13_022403B8(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_0222CC80);
}

// @fn ov13_0222CC80
void ov13_0222CC80(void) {
    if (ov13_02240394(0) != 0) {
        return;
    }
    if (ov13_0224F504 == 0) {
        if (ov13_02240394(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    ov13_02240B04(0);
    if (ov13_0224F504 == 0) {
        ov13_0222788C();
        ov13_02240EFC(1, 1);
    }
    ov13_02240EFC(0, 0x15);
    if (ov13_0224F504 == 0) {
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_0222D454);
    } else {
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_0222CDA8);
    }
}

// @fn ov13_0222CD38
void ov13_0222CD38(void) {
    int r = ov13_0223C1DC();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        ov13_0223ABB8(0xe);
        ov13_0224F504 = 1;
    } else {
        ov13_0223ABB8(7);
    }
    ov13_0223C198();
    ov13_0222B8E8(ov13_0222CD88);
}

// @fn ov13_0222CD88
void ov13_0222CD88(void) {
    if (ov13_0223C1F0() != 0) {
        return;
    }
    ov13_0222B8E8(ov13_0222CBFC);
}

// @fn ov13_0222CDA8
void ov13_0222CDA8(void) {
    ov13_0222CDCC();
    ov13_022279EC(0x19);
    ov13_0223A840();
    ov13_0222B8E8(ov13_0222CE4C);
}

// @fn ov13_0222CDCC
void ov13_0222CDCC(void) {
    ov13_02227924(ov13_022461A4, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

// @fn ov13_0222CE4C
void ov13_0222CE4C(void) {
    ov13_022403B8(2, 0, 0x15, 8);
    ov13_02240EA8(0, 0x15);
    ov13_0222B8E8(ov13_0222CE80);
}

// @fn ov13_0222CE80
void ov13_0222CE80(void) {
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_022279B4(5);
    ov13_0222B8E8(ov13_0222CEAC);
}

// @fn ov13_0222CEAC
void ov13_0222CEAC(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_0222CED4);
}

// @fn ov13_0222CED4
void ov13_0222CED4(void) {
    ov13_0222CEE8();
    ov13_0222CF08();
    ov13_0222CF0C();
}

// @fn ov13_0222CEE8
void ov13_0222CEE8(void) {
    if (ov13_02241694(1) != 0) {
        ov13_0222726C(0);
    }
}

// @fn ov13_0222CF08
void ov13_0222CF08(void) {
}

// @fn ov13_0222CF0C
void ov13_0222CF0C(void) {
    if (ov13_02227258() != 0) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0222B8E8(ov13_0222CF34);
}

// @fn ov13_0222CF34
void ov13_0222CF34(void) {
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_0222CF54);
}

// @fn ov13_0222CF54
void ov13_0222CF54(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    ov13_022403B8(3, 1, 0x3f, 0x40);
    ov13_022403B8(3, 0, 0x3f, 0x40);
    ov13_0222B8E8(ov13_0222CFA0);
}

// @fn ov13_0222CFA0
void ov13_0222CFA0(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    OS_SpinWait(0x1000000);
    PM_ForceToPowerOff();
}

// @fn ov13_0222CFD4
void ov13_0222CFD4(void) {
    ov13_0222D008();
    ov13_02227DE4(0x11);
    ov13_022277B8(0x3a, -1, 0);
    ov13_0222D088();
    ov13_0222B8E8(ov13_0222D25C);
}

// @fn ov13_0222D008
void ov13_0222D008(void) {
    ov13_02227924(ov13_022461B8, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

// @fn ov13_0222D088
void ov13_0222D088(void) {
    struct WiFiInfo info;
    u32 parts[4];
    u16 buf[0x14];
    u8 mac[6];
    u64 id;
    int i;
    int win = ov13_022408A0(0, 0);
    OS_GetMacAddress(mac);
    swprintf(buf, 0x14, ov13_022461CC, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    ov13_02240CC0(win, 8, 0x40, 0xf0, 0x1c, 2, 0x480, buf);
    DWCi_BM_GetWiFiInfo(&info);
    if (info.id != 0) {
        id = info.id;
        parts[3] = (id % 10) * 1000;
        id /= 10;
        for (i = 0; i < 3; i++) {
            parts[2 - i] = id % 10000;
            id /= 10000;
        }
        swprintf(buf, 0x14, ov13_02246208, parts[0], parts[1], parts[2], parts[3]);
    } else {
        swprintf(buf, 0x14, ov13_02246230);
    }
    ov13_02240CC0(win, 8, 0x78, 0xf0, 0x1c, 2, 0x480, buf);
    ov13_02240E2C(win);
}

// @fn ov13_0222D25C
void ov13_0222D25C(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x15, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x15);
    ov13_0222B8E8(ov13_0222D2B0);
}

// @fn ov13_0222D2B0
void ov13_0222D2B0(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_022279B4(2);
    ov13_0222B8E8(ov13_0222D2EC);
}

// @fn ov13_0222D2EC
void ov13_0222D2EC(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_0222D314);
}

// @fn ov13_0222D314
void ov13_0222D314(void) {
    ov13_0222D328();
    ov13_0222D348();
    ov13_0222D34C();
}

// @fn ov13_0222D328
void ov13_0222D328(void) {
    if (ov13_02241694(2) != 0) {
        ov13_0222726C(0);
    }
}

// @fn ov13_0222D348
void ov13_0222D348(void) {
}

// @fn ov13_0222D34C
void ov13_0222D34C(void) {
    if (ov13_02227258() != 0) {
        return;
    }
    ov13_0223ABB8(7);
    ov13_0222B8E8(ov13_0222D374);
}

// @fn ov13_0222D374
void ov13_0222D374(void) {
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_0222D394);
}

// @fn ov13_0222D394
void ov13_0222D394(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    ov13_02227228();
    ov13_022403B8(3, 1, 1, 8);
    ov13_022403B8(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_0222D3E4);
}

// @fn ov13_0222D3E4
void ov13_0222D3E4(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    ov13_02240B04(0);
    ov13_0222788C();
    ov13_02240EFC(1, 1);
    ov13_02240EFC(0, 0x15);
    ov13_0222B8F8(0, 1);
    ov13_0222B8E8(ov13_0222D454);
}

// @fn ov13_0222D454
void ov13_0222D454(void) {
    int idx;
    ov13_0224F508.unk1 = 0;
    ov13_0222D51C();
    ov13_02227DE4(0x10);
    ov13_0223B6F4(3);
    ov13_022277B8(0x39, -1, 0);
    ov13_0224F508.unk8 = ov13_02240184(0, 0x5b);
    *(u32 *)ov13_0224F508.unk8 = (*(u32 *)ov13_0224F508.unk8 & 0xFE00FF00) | 0x84 | 0xE00000;
    ov13_0224F508.unk8->attr2 = (ov13_0224F508.unk8->attr2 & ~0xc00) | 0xc00;
    idx = ov13_0224F508.sel * 4;
    ov13_02227C28(ov13_022453B2[idx], ov13_022453B6[idx], ov13_022453B4[idx], ov13_022453B8[idx]);
    ov13_0222B8E8(ov13_0222D6B8);
}

// @fn ov13_0222D51C
void ov13_0222D51C(void) {
    struct Str22 a;
    struct Str23 b;
    struct WiFiInfo info;
    u8 *src;
    u8 *dst;
    int i;
    u8 *pal;
    a = ov13_0224536C;
    b = ov13_02245382;
    ov13_02227924(ov13_02246258, GX_LoadBG2Char);
    ov13_02227924(ov13_02246270, GX_LoadBG2Scr);
    ov13_0224F508.unk4 = ov13_0223F95C(ov13_022278C4(&b), NULL, 4);
    pal = ov13_0223F95C(ov13_022278C4(&a), NULL, 4);
    DWCi_BM_GetWiFiInfo(&info);
    if (info.id == 0) {
        src = pal + 0xc0;
        dst = pal + 0x40;
        for (i = 0; i < 2; i++) {
            MI_CpuCopy8(src, dst, 0x20);
            src += 0x20;
            dst += 0x20;
        }
    }
    DC_FlushRange(pal, 0x200);
    GX_LoadBGPltt(pal, 0, 0x200);
    ov13_0223FA50(pal);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}

// @fn ov13_0222D6B8
void ov13_0222D6B8(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x14, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x14);
    ov13_0222B8E8(ov13_0222D70C);
}

// @fn ov13_0222D70C
void ov13_0222D70C(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    ov13_022279B4(0);
    ov13_0222B8E8(ov13_0222D748);
}

// @fn ov13_0222D748
void ov13_0222D748(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_0222D770);
}

// @fn ov13_0222D770
void ov13_0222D770(void) {
    ov13_0222D784();
    ov13_0222D878();
    ov13_0222D87C();
}

// @fn ov13_0222D784
void ov13_0222D784(void) {
    u32 i;
    struct Unk8 *entry = ov13_0224539A;
    for (i = 0; i < 3; i++, entry++) {
        if (ov13_022417DC(entry) != 0) {
            int idx;
            ov13_0222726C(1);
            ov13_0224F508.sel = i;
            idx = ov13_0224F508.sel * 4;
            ov13_02227C28(ov13_022453B2[idx], ov13_022453B6[idx], ov13_022453B4[idx], ov13_022453B8[idx]);
            return;
        }
    }
    if (ov13_02241694(1) != 0) {
        ov13_0222726C(1);
        return;
    }
    if (ov13_02241694(2) != 0) {
        ov13_0222726C(0);
        return;
    }
    if (ov13_022416D4(0x40) != 0) {
        ov13_0222DAB0(1);
        return;
    }
    if (ov13_022416D4(0x80) != 0) {
        ov13_0222DAB0(3);
    }
}

// @fn ov13_0222D878
void ov13_0222D878(void) {
}

// @fn ov13_0222D87C
void ov13_0222D87C(void) {
    struct WiFiInfo info;
    switch (ov13_02227258()) {
    case 0:
        ov13_0223ABB8(7);
        break;
    case 1:
        DWCi_BM_GetWiFiInfo(&info);
        if (ov13_0224F508.sel != 0 && info.id == 0) {
            ov13_0223ABB8(9);
            ov13_0222728C(-1);
            return;
        }
        ov13_0223ABB8(6);
        ov13_0222DB40();
        ov13_0224F508.unk1 = 1;
        break;
    default:
        return;
    }
    ov13_0222B8E8(ov13_0222D920);
}

// @fn ov13_0222D920
void ov13_0222D920(void) {
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_0222D940);
}

// @fn ov13_0222D940
void ov13_0222D940(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_0224F508.unk1 != 0) {
        ov13_02227228();
    }
    ov13_022403B8(3, 1, 1, 8);
    ov13_022403B8(3, 0, 0x14, 8);
    ov13_0222B8E8(ov13_0222D9A4);
}

// @fn ov13_0222D9A4
void ov13_0222D9A4(void) {
    if (ov13_02240394(1) != 0) {
        return;
    }
    if (ov13_02240394(0) != 0) {
        return;
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    ov13_02241D98(ov13_0224F508.unk8);
    ov13_02227D18();
    ov13_0222788C();
    ov13_0223FA50(ov13_0224F508.unk4);
    ov13_02240EFC(1, 1);
    ov13_02240EFC(0, 0x14);
    if (ov13_0224F508.unk1 == 0) {
        ov13_0222B8F8(0, 0);
        ov13_0222B8E8(ov13_0222C44C);
        return;
    }
    switch (ov13_0224F508.sel) {
    case 0:
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_0222CFD4);
        break;
    case 1:
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_0222C9C4);
        break;
    case 2:
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_0222E220);
        break;
    }
}

// @fn ov13_0222DAB0
void ov13_0222DAB0(int dir) {
    int idx;
    if (dir == 1) {
        ov13_0224F508.sel = FX_ModS32(ov13_0224F508.sel + 2, 3);
    } else {
        ov13_0224F508.sel = FX_ModS32(ov13_0224F508.sel + 1, 3);
    }
    ov13_0223ABB8(8);
    idx = ov13_0224F508.sel * 4;
    ov13_02227C28(ov13_022453B2[idx], ov13_022453B6[idx], ov13_022453B4[idx], ov13_022453B8[idx]);
}

// @fn ov13_0222DB40
void ov13_0222DB40(void) {
    ov13_02239378(ov13_0224F508.unk4, ov13_02245364[ov13_0224F508.sel], ov13_02245364[ov13_0224F508.sel]);
}

// @fn ov13_0222DB6C
void ov13_0222DB6C(void) {
    ov13_0222DBA4();
    ov13_022279EC(0x1e);
    ov13_02226F58(0);
    ov13_02227EB0(1);
    ov13_0223ABB8(0xb);
    ov13_0222B8E8(ov13_0222DC24);
}

// @fn ov13_0222DBA4
void ov13_0222DBA4(void) {
    ov13_02227924(ov13_02246288, GX_LoadBG2Scr);
    SET_PRI3(0x04001008);
    SET_PRI3(0x0400100A);
    SET_PRI3(0x04000008);
    SET_PRI3(0x0400000A);
    SET_PRI3(0x0400000C);
}
