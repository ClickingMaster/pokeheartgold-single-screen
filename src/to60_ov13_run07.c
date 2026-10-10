#pragma force_active on
#pragma warn_unusedvar off
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef void (*Ov13Task)(void);

typedef struct Ov13Menu {
    void *unk0;
    void *a[3];
    void *b[3];
    u8 unk1C;
    u8 unk1D;
} Ov13Menu;

typedef struct Ov13MenuState {
    u8 sel;
    Ov13Menu *p;
} Ov13MenuState;

typedef struct Ov13SubState {
    s8 a;
    s8 b;
    u8 c;
    void *p;
} Ov13SubState;

typedef struct Ov13Profile {
    u8 pad0[0xE7];
    u8 unkE7;
    u8 padE8[0xC];
    u8 unkF4;
} Ov13Profile;

typedef struct Ov13Pos2 {
    u16 x;
    u16 y;
} Ov13Pos2;

typedef struct Ov13Pos4 {
    u16 a;
    u16 b;
    u16 c;
    u16 d;
} Ov13Pos4;

typedef struct Ov13Bytes4 {
    u8 v[4];
} Ov13Bytes4;

typedef struct Ov13Bytes22 {
    u8 v[22];
} Ov13Bytes22;

typedef struct Ov13Words3 {
    u32 v[3];
} Ov13Words3;

typedef struct Ov13Param {
    u8 v[10];
    u8 unkA;
    u8 unkB;
} Ov13Param;

extern Ov13MenuState ov13_0224F56C;
extern void *ov13_0224F570;
extern Ov13SubState ov13_0224F574;
extern u8 ov13_0224F57C;
extern u8 ov13_0224F580;
extern u8 ov13_0224F584;
extern u8 ov13_0224F588;
extern u8 ov13_0224F58C;
extern u8 ov13_02245600[];
extern Ov13Bytes4 ov13_02245604[2];
extern Ov13Pos2 ov13_02245622[];
extern Ov13Pos4 ov13_0224563A[];
extern Ov13Pos4 ov13_02245672[];
extern s8 ov13_022456AC[];
extern Ov13Bytes4 ov13_022456B4;
extern Ov13Bytes22 ov13_022456B8;
extern s8 ov13_022456CE[][4][4];
extern Ov13Pos4 ov13_022456EE[][4];
extern Ov13Pos4 ov13_0224572E[][4];
extern s8 ov13_02245770[];
extern Ov13Param ov13_02245778;
extern Ov13Words3 ov13_02245784;
extern u8 ov13_02246664[];
extern u8 ov13_0224667C[];
extern u8 ov13_02246694[];
extern u8 ov13_022466AC[];
extern u8 ov13_022466C0[];
extern u8 ov13_022466D8[];
extern u8 ov13_022466F0[];
extern u8 ov13_02246704[];
extern u8 ov13_02246718[];
extern u8 ov13_0224672C[];
extern u8 ov13_02246740[];
extern u8 ov13_02246758[];
extern u8 ov13_02246770[];
extern u8 ov13_02246784[];
extern u8 ov13_0224679C[];
extern u8 ov13_022467B4[];

void ov13_0222B8E8(Ov13Task task);
void ov13_0222B8F8(int a, int b);
void ov13_0222B934(int a, int b);
void ov13_0222B948(int a, int *out);
int ov13_0222B970(void);
int ov13_0222B980(void);
int ov13_0222B998(int a);
int ov13_02227258(void);
void ov13_0222726C(int a);
void ov13_0222728C(int a);
int ov13_022272A0(void);
void ov13_022272CC(void);
void ov13_022272E4(void);
void ov13_02227228(void);
void ov13_022277B8(int a, int b, int c);
void ov13_0222788C(void);
void *ov13_022278C4(void *a);
void ov13_02227924(void *data, void *load);
void ov13_02227964(int a);
void ov13_022279B4(int a);
void ov13_022279EC(int a);
void ov13_02227A60(void *a, int b);
void ov13_02227B6C(int a, int b, int c, int d);
void ov13_02227C28(int a, int b, int c, int d);
void ov13_02227D18(void);
void ov13_02227DE4(int a);
void ov13_02227E40(void);
void ov13_02227EB0(int a);
void ov13_02227F68(void);
void ov13_02227028(void);
void ov13_02226F58(int a);
void ov13_0222C10C(void);
void ov13_0222C44C(void);
void ov13_0222F2AC(void);
void ov13_022327E0(void);
void ov13_02232DF4(void);
void ov13_02232E04(void);
void ov13_022352C0(int a);
void ov13_02235F80(void);
void ov13_022363F4(void);
void ov13_022368D8(void);
void ov13_02238D88(void);
void ov13_0223902C(void);
void ov13_02239378(void *a, int b, int c);
void ov13_02239B34(void);
Ov13Profile *ov13_02239FF0(void);
int ov13_0223A3FC(int a);
void ov13_0223A414(void);
void ov13_0223A4F4(void);
u8 *ov13_0223A7F4(void);
void ov13_0223A804(int a);
void ov13_0223ABB8(int a);
void ov13_0223AC20(void);
void ov13_0223B1A0(void);
int ov13_0223B2A8(void);
int ov13_0223B6C8(void);
void ov13_0223B788(Ov13Task task);
void ov13_0223BE6C(int a, int b, int c, int d, int e);
void ov13_0223C198(void);
int ov13_0223C1DC(void);
int ov13_0223C1F0(void);
void *ov13_0223F95C(void *a, int b, int c);
void ov13_0223FA50(void *a);
void ov13_0223FD60(void *a);
int ov13_0223FDE0(void *a, int b);
void ov13_0223FE98(void *a, int b, int c, int d);
void ov13_0223FF18(void *a, int b, int c, int d);
void ov13_02240008(void *a, int b, int c);
void ov13_022400F0(int a, int b, int c);
void *ov13_022401B0(int a, int b, int c);
int ov13_02240394(int a);
void ov13_022403B8(int a, int b, int c, int d);
void ov13_022405B8(int a);
void ov13_02240B04(int a);
void ov13_02240EA8(int a, int b);
void ov13_02240EFC(int a, int b);
void ov13_0224128C(void *a, int b);
void ov13_022412B0(void *a);
void ov13_022412F4(void *a);
int ov13_02241694(int a);
int ov13_022416D4(int a);
int ov13_022417DC(void *a);
void ov13_02242528(int a, void *b, int c, int d);
void ov13_022425D0(int a, void *b);
int ov00_021FA75C(void *a);
int ov00_021FA904(void);
void ov00_021FA9D4(void);
int ov00_021FAAC8(void);
void ov00_021FAC20(int a, void *b);
void *DWCi_BACKUPlGetWifi(void);

void ov13_02236BA4(void);
void ov13_02236BE0(void);
void ov13_02236C14(void);
void ov13_02236C28(void);
void ov13_02236D1C(void);
void ov13_02236D20(void);
void ov13_02236E54(void);
void ov13_02236E90(void);
void ov13_02236F14(void);
void ov13_022370B8(void);
void ov13_02237134(int dir);
void ov13_02237328(void);
void ov13_022373B8(void);
void ov13_02237524(void);
void ov13_02237548(void);
void ov13_02237618(void);
void ov13_0223773C(void);
void ov13_02237828(void);
void ov13_0223787C(void);
void ov13_022378B8(void);
void ov13_022378E0(void);
void ov13_022378F4(void);
void ov13_02237A78(void);
void ov13_02237A7C(void);
void ov13_02237AD0(void);
void ov13_02237AF0(void);
void ov13_02237B54(void);
void ov13_02237C84(int dir);
void ov13_02237D78(void);
void ov13_02237DD0(void);
void ov13_02237E5C(void);
void ov13_02237F04(void);
void ov13_02237F9C(void);
void ov13_02238008(void);
void ov13_0223803C(void);
void ov13_02238050(void);
void ov13_02238054(void);
void ov13_02238058(void);
void ov13_0223805C(void);
void ov13_02238084(void);
void ov13_022380A8(void);
void ov13_0223813C(void *arg);
void ov13_022381CC(void);
void ov13_022381FC(void);
void ov13_02238268(void);
void ov13_0223829C(void);
void ov13_022382C0(void);
void ov13_022382D0(void);
void ov13_022382D4(void);
void ov13_0223832C(void);
void ov13_022383D0(void);
void ov13_02238440(void);
void ov13_02238498(void);
void ov13_02238504(void);
void ov13_02238558(void);
void ov13_022385E0(void);
void ov13_0223861C(void);
void ov13_02238620(void);
void ov13_02238624(void);
void ov13_02238658(void);
void ov13_022386D4(void);
void ov13_02238754(void);
void ov13_02238788(void);
void ov13_022387AC(void);
void ov13_022387BC(void);
void ov13_022387C0(void);
void ov13_022387FC(void);
void ov13_0223886C(void);
void ov13_022388A0(void);
void ov13_022388F0(void);
void ov13_02238998(void);
void ov13_022389EC(void);
void ov13_02238A28(void);
void ov13_02238A50(void);
void ov13_02238A64(void);
void ov13_02238A9C(void);
void ov13_02238AA0(void);
void ov13_02238AF0(void);
void ov13_02238B10(void);
void ov13_02238B74(void);
void ov13_02238C4C(void);
void ov13_02238CE0(void);

void ov13_02236A48(void);
void ov13_02236B50(void);
void ov13_022381A8(void *a, void *b);
void ov13_022381BC(void *a, void *b);
void ov13_02238694(void);
BOOL ov13_02238C2C(void);

void ov13_02236A48(void) {
    int k;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        k = ov13_0223A3FC(i);
        if (k == 0xFF) {
            k = 3;
        } else {
            ov13_0224F56C.p->b[i] = ov13_022401B0(0, 0x11, 1);
            j = i + 3;
            ov13_0223FF18(ov13_0224F56C.p->b[i], -1, ov13_02245622[j].x, ov13_02245622[j].y);
            ov13_02240008(ov13_0224F56C.p->b[i], -1, 3);
        }
        ov13_0224F56C.p->a[i] = ov13_022401B0(0, ov13_02245600[k], 1);
        ov13_0223FF18(ov13_0224F56C.p->a[i], -1, ov13_02245622[i].x, ov13_02245622[i].y);
        ov13_02240008(ov13_0224F56C.p->a[i], -1, 3);
    }
}

void ov13_02236B50(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x14, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x14);
    ov13_0222B8E8(ov13_02236BA4);
}

void ov13_02236BA4(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    ov13_022279B4(0);
    ov13_0222B8E8(ov13_02236BE0);
}

void ov13_02236BE0(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    if (ov13_0223B2A8() == 1) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_02236C14);
}

void ov13_02236C14(void) {
    ov13_02236C28();
    ov13_02236D1C();
    ov13_02236D20();
}

void ov13_02236C28(void) {
    u32 i;
    for (i = 0; i < 7; i++) {
        if (ov13_022417DC(&ov13_0224563A[i])) {
            ov13_0222726C(1);
            ov13_0224F56C.sel = i;
            ov13_022370B8();
            return;
        }
    }
    if (ov13_02241694(1)) {
        ov13_0222726C(1);
        return;
    }
    if (ov13_02241694(2)) {
        ov13_0222726C(0);
        return;
    }
    if (ov13_022416D4(0x40)) {
        ov13_02237134(1);
        return;
    }
    if (ov13_022416D4(0x80)) {
        ov13_02237134(3);
        return;
    }
    if (ov13_022416D4(0x20)) {
        ov13_02237134(0);
        return;
    }
    if (ov13_022416D4(0x10)) {
        ov13_02237134(2);
    }
}

void ov13_02236D1C(void) {
}

void ov13_02236D20(void) {
    u8 sel;
    int idx;
    int k;
    switch (ov13_02227258()) {
    case 0:
        switch (ov13_0222B980()) {
        case 0:
            ov13_0223ABB8(7);
            ov13_0224F56C.p->unk1D = 2;
            break;
        case 1:
            ov13_022272E4();
            ov13_0222B8E8(ov13_0222C10C);
            return;
        }
        break;
    case 1:
        ov13_0224F56C.p->unk1D = 1;
        sel = ov13_0224F56C.sel;
        if (sel >= 4) {
            idx = sel - 4;
            if (ov13_0223A3FC(idx) == 0xFF) {
                ov13_0223ABB8(9);
                ov13_0222728C(-1);
                return;
            }
            ov13_0223ABB8(6);
            k = ov13_022456AC[ov13_0222B970()];
            ov13_0223BE6C(0x46, 0, 1, k, idx + 1);
            ov13_02237328();
            ov13_022272E4();
            ov13_0222B8E8(ov13_022373B8);
            return;
        }
        if (sel <= 2) {
            ov13_0223A414();
        }
        ov13_0223ABB8(6);
        ov13_02237548();
        break;
    default:
        return;
    }
    ov13_0222B8E8(ov13_02236E54);
}

void ov13_02236E54(void) {
    if (ov13_0224F56C.p->unk1D == 2) {
        ov13_0223B1A0();
    }
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_02236E90);
}

void ov13_02236E90(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_0224F56C.p->unk1D == 1) {
        if (ov13_0224F56C.sel == 3 || ov13_02239FF0()->unkE7 != 0xFF) {
            ov13_02227228();
        }
    }
    ov13_022403B8(3, 1, 1, 8);
    ov13_022403B8(3, 0, 0x14, 8);
    ov13_0222B8E8(ov13_02236F14);
}

void ov13_02236F14(void) {
    u32 i;
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    if (!ov13_0223B6C8()) {
        return;
    }
    if (!ov13_022272A0()) {
        return;
    }
    for (i = 0; i < 3; i++) {
        if (ov13_0224F56C.p->a[i] != NULL) {
            ov13_0223FD60(ov13_0224F56C.p->a[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        if (ov13_0224F56C.p->b[i] != NULL) {
            ov13_0223FD60(ov13_0224F56C.p->b[i]);
        }
    }
    ov13_02227D18();
    ov13_0222788C();
    ov13_0223FA50(ov13_0224F56C.p->unk0);
    ov13_02227924(ov13_02246664, GX_LoadOBJPltt);
    ov13_02240EFC(1, 1);
    ov13_02240EFC(0, 0x14);
    if (ov13_0224F56C.p->unk1D == 2) {
        ov13_0222B8F8(0, 0);
        ov13_0222B8E8(ov13_0222C44C);
    } else {
        switch (ov13_0224F56C.sel) {
        case 0:
        case 1:
        case 2:
            ov13_0222B8F8(2, 0);
            if (ov13_02239FF0()->unkE7 == 0xFF) {
                ov13_0222B8E8(ov13_02237618);
            } else {
                ov13_0222B934(0, 0);
                ov13_022352C0(0);
                ov13_0222B8E8(ov13_02232E04);
            }
            break;
        case 3:
            ov13_0222B8F8(2, 1);
            ov13_0222B8E8(ov13_022388A0);
            break;
        }
    }
    ov13_022412B0(&ov13_0224F570);
}

void ov13_022370B8(void) {
    u8 sel = ov13_0224F56C.sel;
    if (sel < 4) {
        ov13_02227C28(ov13_02245672[sel].a, ov13_02245672[sel].c, ov13_02245672[sel].b, ov13_02245672[sel].d);
    } else {
        ov13_02227B6C(0, ov13_02245672[sel].a, ov13_02245672[sel].c, ov13_02245672[sel].b);
    }
}

void ov13_02237134(int dir) {
    BOOL changed = TRUE;
    u8 c;
    int c4;
    switch (ov13_0224F56C.sel) {
    case 0:
        ov13_0224F56C.p->unk1C = 0;
        if (dir == 0) {
            ov13_0224F56C.sel = 2;
        } else if (dir == 2) {
            ov13_0224F56C.sel = 1;
        } else if (dir == 1) {
            ov13_0224F56C.sel = 3;
        } else {
            ov13_0224F56C.sel = 4;
        }
        break;
    case 1:
        ov13_0224F56C.p->unk1C = 1;
        if (dir == 0) {
            ov13_0224F56C.sel = 0;
        } else if (dir == 2) {
            ov13_0224F56C.sel = 2;
        } else if (dir == 1) {
            ov13_0224F56C.sel = 3;
        } else {
            ov13_0224F56C.sel = 5;
        }
        break;
    case 2:
        ov13_0224F56C.p->unk1C = 2;
        if (dir == 0) {
            ov13_0224F56C.sel = 1;
        } else if (dir == 2) {
            ov13_0224F56C.sel = 0;
        } else if (dir == 1) {
            ov13_0224F56C.sel = 3;
        } else {
            ov13_0224F56C.sel = 6;
        }
        break;
    case 3:
        c = ov13_0224F56C.p->unk1C;
        c4 = c + 4;
        if (dir == 1) {
            ov13_0224F56C.sel = c4;
        } else if (dir == 3) {
            ov13_0224F56C.sel = c;
        } else {
            changed = FALSE;
        }
        break;
    case 4:
        ov13_0224F56C.p->unk1C = 0;
        if (dir == 0) {
            ov13_0224F56C.sel = 6;
        } else if (dir == 2) {
            ov13_0224F56C.sel = 5;
        } else if (dir == 1) {
            ov13_0224F56C.sel = 0;
        } else {
            ov13_0224F56C.sel = 3;
        }
        break;
    case 5:
        ov13_0224F56C.p->unk1C = 1;
        if (dir == 0) {
            ov13_0224F56C.sel = 4;
        } else if (dir == 2) {
            ov13_0224F56C.sel = 6;
        } else if (dir == 1) {
            ov13_0224F56C.sel = 1;
        } else {
            ov13_0224F56C.sel = 3;
        }
        break;
    case 6:
        ov13_0224F56C.p->unk1C = 2;
        if (dir == 0) {
            ov13_0224F56C.sel = 5;
        } else if (dir == 2) {
            ov13_0224F56C.sel = 4;
        } else if (dir == 1) {
            ov13_0224F56C.sel = 2;
        } else {
            ov13_0224F56C.sel = 3;
        }
        break;
    }
    if (changed) {
        ov13_0223ABB8(8);
        ov13_022370B8();
    }
}

void ov13_02237328(void) {
    int idx = ov13_0224F56C.sel - 4;
    ov13_022400F0(0, 0x32, ov13_0223FDE0(ov13_0224F56C.p->b[idx], 0));
    ov13_0223FF18(ov13_0224F56C.p->b[idx], -1, ov13_02245622[idx + 3].x, ov13_02245622[idx + 3].y);
    ov13_02240008(ov13_0224F56C.p->b[idx], -1, 3);
}

void ov13_022373B8(void) {
    int idx = ov13_0224F56C.sel - 4;
    switch (ov13_0223C1DC()) {
    case 1:
        ov13_0223ABB8(0xE);
        ov13_0223A804(idx);
        ov13_022400F0(0, 0x56, ov13_0223FDE0(ov13_0224F56C.p->a[idx], 0));
        ov13_0223FF18(ov13_0224F56C.p->a[idx], -1, ov13_02245622[idx].x, ov13_02245622[idx].y);
        ov13_02240008(ov13_0224F56C.p->a[idx], -1, 3);
        ov13_0223FD60(ov13_0224F56C.p->b[idx]);
        ov13_0224F56C.p->b[idx] = NULL;
        break;
    case 0:
        ov13_0223ABB8(7);
        ov13_022400F0(0, 0x11, ov13_0223FDE0(ov13_0224F56C.p->b[idx], 0));
        ov13_0223FF18(ov13_0224F56C.p->b[idx], -1, ov13_02245622[idx + 3].x, ov13_02245622[idx + 3].y);
        ov13_02240008(ov13_0224F56C.p->b[idx], -1, 3);
        break;
    default:
        return;
    }
    ov13_0223C198();
    ov13_0222B8E8(ov13_02237524);
}

void ov13_02237524(void) {
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_02236C14);
}

void ov13_02237548(void) {
    Ov13Bytes4 a = ov13_02245604[0];
    Ov13Bytes4 b = ov13_02245604[1];
    int k;
    if (ov13_0224F56C.sel > 3) {
        return;
    }
    ov13_02239378(ov13_0224F56C.p->unk0, a.v[ov13_0224F56C.sel], a.v[ov13_0224F56C.sel]);
    if (ov13_0224F56C.sel == 3) {
        return;
    }
    k = ov13_0223A3FC(ov13_0224F56C.sel);
    if (k > 2) {
        k = 3;
    }
    ov13_0223FE98(ov13_0224F56C.p->a[ov13_0224F56C.sel], -1, 0, b.v[k]);
}

void ov13_02237618(void) {
    Ov13Profile *prof = ov13_02239FF0();
    int k;
    int a, b, c, d;
    ov13_0224F574.c = 0;
    if (ov13_0224F574.a == 0) {
        ov13_0224F574.a = 1;
    }
    if (ov13_0222B970()) {
        if (ov13_0224F574.b == 2) {
            ov13_0224F574.b = 0;
        }
        if (ov13_0224F574.a == 2) {
            ov13_0224F574.a = 1;
        }
    }
    ov13_0223773C();
    ov13_02227E40();
    k = ov13_0222B970();
    ov13_022277B8(0x32, ov13_02245770[k], prof->unkF4 + 1);
    ov13_02227964(1);
    ov13_02227C28(ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].a, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].c, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].b, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].d);
    ov13_0222B8E8(ov13_02237828);
}

void ov13_0223773C(void) {
    Ov13Bytes22 buf = ov13_022456B8;
    ov13_02227924(ov13_0224667C, GX_LoadBG2Char);
    ov13_02227924(ov13_02246694, GX_LoadBGPltt);
    ov13_02227924(ov13_022466AC, GX_LoadBG2Scr);
    ov13_0224F574.p = ov13_0223F95C(ov13_022278C4(&buf), 0, 4);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void ov13_02237828(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x14, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x14);
    ov13_0222B8E8(ov13_0223787C);
}

void ov13_0223787C(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    ov13_022279B4(0);
    ov13_0222B8E8(ov13_022378B8);
}

void ov13_022378B8(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_022378E0);
}

void ov13_022378E0(void) {
    ov13_022378F4();
    ov13_02237A78();
    ov13_02237A7C();
}

void ov13_022378F4(void) {
    u32 i;
    int a, b, c, d;
    for (i = 0; i < 4; i++) {
        if (ov13_022417DC(&ov13_022456EE[ov13_0222B998(1)][i])) {
            ov13_0222726C(1);
            ov13_0224F574.b = i;
            ov13_02227C28(ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].a, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].c, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].b, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].d);
            return;
        }
    }
    if (ov13_02241694(1)) {
        ov13_0222726C(1);
        return;
    }
    if (ov13_02241694(2)) {
        ov13_0222726C(0);
        return;
    }
    if (ov13_022416D4(0x40)) {
        ov13_02237C84(1);
        return;
    }
    if (ov13_022416D4(0x80)) {
        ov13_02237C84(3);
        return;
    }
    if (ov13_022416D4(0x20)) {
        ov13_02237C84(0);
        return;
    }
    if (ov13_022416D4(0x10)) {
        ov13_02237C84(2);
    }
}

void ov13_02237A78(void) {
}

void ov13_02237A7C(void) {
    switch (ov13_02227258()) {
    case 0:
        ov13_0223ABB8(7);
        break;
    case 1:
        ov13_0223ABB8(6);
        ov13_02237D78();
        ov13_0224F574.c = 1;
        break;
    default:
        return;
    }
    ov13_0222B8E8(ov13_02237AD0);
}

void ov13_02237AD0(void) {
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_02237AF0);
}

void ov13_02237AF0(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_0224F574.c) {
        ov13_02227228();
    }
    ov13_022403B8(3, 1, 1, 8);
    ov13_022403B8(3, 0, 0x14, 8);
    ov13_0222B8E8(ov13_02237B54);
}

void ov13_02237B54(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    if (!ov13_022272A0()) {
        return;
    }
    ov13_02227D18();
    ov13_0222788C();
    ov13_0223FA50(ov13_0224F574.p);
    ov13_02240EFC(1, 1);
    ov13_02240EFC(0, 0x14);
    if (ov13_0224F574.c == 0) {
        ov13_0222B8F8(2, 0);
        ov13_0222B8E8(ov13_022368D8);
        return;
    }
    switch (ov13_0224F574.b) {
    case 0:
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_022363F4);
        break;
    case 1:
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_0222F2AC);
        break;
    case 2:
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_02235F80);
        break;
    case 3:
        ov13_0222B8F8(2, 0);
        ov13_0222B934(0, 0);
        ov13_022352C0(1);
        ov13_0222B8E8(ov13_02232E04);
        break;
    }
}

void ov13_02237C84(int dir) {
    s8 cur = ov13_0224F574.b;
    s8 next = ov13_022456CE[ov13_0222B998(1)][cur][dir];
    int a, b, c, d;
    if (next == -1) {
        return;
    }
    if (next == 0) {
        ov13_0224F574.a = cur;
    }
    if (next == -2) {
        ov13_0224F574.b = ov13_0224F574.a;
    } else {
        ov13_0224F574.b = next;
    }
    ov13_0223ABB8(8);
    ov13_02227C28(ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].a, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].c, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].b, ov13_0224572E[ov13_0222B998(1)][ov13_0224F574.b].d);
}

void ov13_02237D78(void) {
    Ov13Bytes4 t = ov13_022456B4;
    ov13_02239378(ov13_0224F574.p, t.v[ov13_0224F574.b], t.v[ov13_0224F574.b]);
}

void ov13_02237DD0(void) {
    int mode;
    ov13_0224F57C = 0;
    ov13_02237E5C();
    ov13_0222B948(0, &mode);
    if (mode == 0) {
        ov13_022277B8(0x31, -1, 0);
    }
    ov13_02227964(2);
    if (mode == 0) {
        ov13_02227E40();
    }
    ov13_022279EC(0x2C);
    ov13_02226F58(0);
    ov13_02237F04();
    ov13_02227EB0(0);
    ov13_0223ABB8(0xC);
    ov13_0222B8E8(ov13_02237F9C);
}

void ov13_02237E5C(void) {
    ov13_02227924(ov13_022466C0, GX_LoadBG2Char);
    ov13_02227924(ov13_022466D8, GX_LoadBGPltt);
    ov13_02227924(ov13_022466F0, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void ov13_02237F04(void) {
    int mode;
    Ov13Param param;
    Ov13Profile *prof = ov13_02239FF0();
    MIi_CpuCopy32((const void *)&ov13_02245778, (void *)&param, sizeof(Ov13Param));
    ov13_0222B948(0, &mode);
    if (mode == 2) {
        param.unkA = 4;
    } else {
        param.unkA = prof->unkF4 + 1;
    }
    if (!ov00_021FA75C(&param)) {
        OS_Terminate();
    }
    if (mode == 0) {
        ov00_021FAC20(prof->unkF4, prof);
    }
    ov13_02242528(0, ov13_0223813C, 0, 0x78);
}

void ov13_02237F9C(void) {
    int mode;
    ov13_022403B8(2, 0, 0x15, 8);
    ov13_02240EA8(0, 0x15);
    ov13_0222B948(0, &mode);
    if (mode == 0) {
        ov13_022403B8(2, 1, 1, 8);
        ov13_02240EA8(1, 1);
    }
    ov13_0222B8E8(ov13_02238008);
}

void ov13_02238008(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    ov13_0222B8E8(ov13_0223803C);
}

void ov13_0223803C(void) {
    ov13_02238050();
    ov13_02238054();
    ov13_02238058();
}

void ov13_02238050(void) {
}

void ov13_02238054(void) {
}

void ov13_02238058(void) {
}

void ov13_0223805C(void) {
    ov13_022403B8(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_02238084);
}

void ov13_02238084(void) {
    if (ov13_02240394(0)) {
        return;
    }
    ov13_0222B8E8(ov13_022380A8);
}

void ov13_022380A8(void) {
    u8 *q = ov13_0223A7F4();
    if (!ov00_021FAAC8()) {
        return;
    }
    MI_CpuCopy8(DWCi_BACKUPlGetWifi(), q + 0xF0, 0xE);
    MI_CpuCopy8(DWCi_BACKUPlGetWifi(), q + 0x1F0, 0xE);
    ov13_02227028();
    ov13_02240B04(0);
    ov13_02240EFC(0, 0x15);
    if (ov13_0224F57C == 0) {
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_022327E0);
    } else {
        ov13_0222B8F8(0, 1);
        ov13_0222B8E8(ov13_022381CC);
    }
}

void ov13_0223813C(void *arg) {
    int r = ov00_021FA904();
    if (r == 0) {
        return;
    }
    ov13_02227F68();
    ov13_0223AC20();
    if (r > 0) {
        ov13_0224F57C = 1;
        ov13_0223ABB8(0x11);
    } else {
        ov00_021FA9D4();
        ov13_02232DF4();
        ov13_0223ABB8(0x12);
    }
    ov13_0222B8E8(ov13_0223805C);
    ov13_022425D0(0, arg);
}

void ov13_022381A8(void *a, void *b) {
    ov13_0224128C(b, 0x20);
}

void ov13_022381BC(void *a, void *b) {
    ov13_022412F4(b);
}

void ov13_022381CC(void) {
    ov13_0224F580 = 0;
    ov13_022381FC();
    ov13_022279EC(0x2D);
    ov13_0222B8E8(ov13_02238268);
}

void ov13_022381FC(void) {
    ov13_02227924(ov13_02246704, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void ov13_02238268(void) {
    ov13_022403B8(2, 0, 0x15, 8);
    ov13_02240EA8(0, 0x15);
    ov13_0222B8E8(ov13_0223829C);
}

void ov13_0223829C(void) {
    if (ov13_02240394(0)) {
        return;
    }
    ov13_0222B8E8(ov13_022382C0);
}

void ov13_022382C0(void) {
    ov13_022383D0();
    ov13_022382D0();
}

void ov13_022382D0(void) {
}

void ov13_022382D4(void) {
    int mode;
    ov13_0222B948(0, &mode);
    if (mode != 0) {
        ov13_0223B1A0();
    }
    ov13_022403B8(3, 1, 1, 8);
    ov13_022403B8(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_0223832C);
}

void ov13_0223832C(void) {
    int mode;
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    if (!ov13_0223B6C8()) {
        return;
    }
    ov13_02240B04(0);
    ov13_0222788C();
    ov13_02240EFC(1, 1);
    ov13_02240EFC(0, 0x15);
    ov13_0222B8F8(2, 1);
    ov13_0222B948(0, &mode);
    if (mode == 0) {
        ov13_0222B934(0, 0);
        ov13_0222B8E8(ov13_02232E04);
        return;
    }
    ov13_0222B8E8(ov13_0222C44C);
}

void ov13_022383D0(void) {
    int mode;
    if (++ov13_0224F580 < 0xB4) {
        return;
    }
    ov13_0222B948(0, &mode);
    switch (ov13_0222B980()) {
    case 0:
        break;
    case 1:
        if (mode != 0) {
            ov13_0222B8E8(ov13_0222C10C);
            return;
        }
        break;
    }
    ov13_0222B8E8(ov13_022382D4);
}

void ov13_02238440(void) {
    int mode;
    ov13_02239B34();
    ov13_0222B948(0, &mode);
    ov13_02238498();
    ov13_022277B8(0x31, -1, 0);
    if (mode != 2) {
        ov13_02227E40();
    }
    if (mode == 1) {
        ov13_0223A4F4();
    }
    ov13_0222B8E8(ov13_02238504);
}

void ov13_02238498(void) {
    ov13_02227924(ov13_02246718, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void ov13_02238504(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x14, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x14);
    ov13_0222B8E8(ov13_02238558);
}

void ov13_02238558(void) {
    Ov13Words3 t = ov13_02245784;
    int mode;
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    ov13_0222B948(0, &mode);
    ov13_0223BE6C(t.v[mode], 1, 1, -1, 0);
    ov13_0222B8E8(ov13_022385E0);
}

void ov13_022385E0(void) {
    ov13_0223861C();
    ov13_02238620();
    if (ov13_0223C1DC()) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0223C198();
    ov13_0223ABB8(6);
    ov13_0222B8E8(ov13_02238624);
}

void ov13_0223861C(void) {
}

void ov13_02238620(void) {
}

void ov13_02238624(void) {
    if (ov13_0223C1F0()) {
        return;
    }
    ov13_022403B8(3, 0, 0x14, 8);
    ov13_0222B8E8(ov13_02238658);
}

void ov13_02238658(void) {
    if (ov13_02240394(0)) {
        return;
    }
    ov13_02240EFC(0, 0x14);
    ov13_0222B8F8(0, 1);
    ov13_0222B8E8(ov13_02237DD0);
}

void ov13_02238694(void) {
    ov13_0224F584 = 0;
    ov13_022386D4();
    ov13_02227DE4(8);
    ov13_022279EC(0x29);
    ov13_0223ABB8(0x10);
    ov13_0222B8E8(ov13_02238754);
}

void ov13_022386D4(void) {
    ov13_02227924(ov13_0224672C, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void ov13_02238754(void) {
    ov13_022403B8(2, 0, 0x15, 8);
    ov13_02240EA8(0, 0x15);
    ov13_0222B8E8(ov13_02238788);
}

void ov13_02238788(void) {
    if (ov13_02240394(0)) {
        return;
    }
    ov13_0222B8E8(ov13_022387AC);
}

void ov13_022387AC(void) {
    ov13_0223886C();
    ov13_022387BC();
}

void ov13_022387BC(void) {
}

void ov13_022387C0(void) {
    ov13_022403B8(3, 1, 1, 8);
    ov13_022403B8(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_022387FC);
}

void ov13_022387FC(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    ov13_02240B04(0);
    ov13_0222788C();
    ov13_02240EFC(1, 1);
    ov13_02240EFC(0, 0x15);
    ov13_0222B8F8(0, 0);
    ov13_0222B934(0, 2);
    ov13_0222B8E8(ov13_02238440);
}

void ov13_0223886C(void) {
    if (++ov13_0224F584 < 0x78) {
        return;
    }
    ov13_0222B8E8(ov13_022387C0);
}

void ov13_022388A0(void) {
    ov13_0224F588 = 0;
    ov13_022388F0();
    ov13_02227DE4(8);
    ov13_022277B8(0x38, -1, 0);
    ov13_02227964(1);
    ov13_022279EC(0x27);
    ov13_0222B8E8(ov13_02238998);
}

void ov13_022388F0(void) {
    ov13_02227924(ov13_02246740, GX_LoadBG2Char);
    ov13_02227924(ov13_02246758, GX_LoadBGPltt);
    ov13_02227924(ov13_02246770, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}

void ov13_02238998(void) {
    ov13_022403B8(2, 1, 1, 8);
    ov13_022403B8(2, 0, 0x15, 8);
    ov13_02240EA8(1, 1);
    ov13_02240EA8(0, 0x15);
    ov13_0222B8E8(ov13_022389EC);
}

void ov13_022389EC(void) {
    if (ov13_02240394(1)) {
        return;
    }
    if (ov13_02240394(0)) {
        return;
    }
    ov13_022279B4(6);
    ov13_0222B8E8(ov13_02238A28);
}

void ov13_02238A28(void) {
    if (ov13_02227258() == -2) {
        return;
    }
    ov13_022272CC();
    ov13_0222B8E8(ov13_02238A50);
}

void ov13_02238A50(void) {
    ov13_02238A64();
    ov13_02238A9C();
    ov13_02238AA0();
}

void ov13_02238A64(void) {
    if (ov13_02241694(1)) {
        ov13_0222726C(1);
    }
    if (ov13_02241694(2)) {
        ov13_0222726C(0);
    }
}

void ov13_02238A9C(void) {
}

void ov13_02238AA0(void) {
    switch (ov13_02227258()) {
    case 0:
        ov13_0223ABB8(7);
        break;
    case 1:
        ov13_0223ABB8(6);
        ov13_0224F588 = 1;
        break;
    default:
        return;
    }
    ov13_0222B8E8(ov13_02238AF0);
}

void ov13_02238AF0(void) {
    ov13_022272E4();
    ov13_022405B8(8);
    ov13_0222B8E8(ov13_02238B10);
}

void ov13_02238B10(void) {
    if (ov13_02240394(1)) {
        return;
    }
    ov13_02227228();
    if (ov13_0224F588 == 0) {
        ov13_022403B8(3, 1, 1, 8);
    }
    ov13_022403B8(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_02238B74);
}

void ov13_02238B74(void) {
    if (ov13_02240394(0)) {
        return;
    }
    if (ov13_0224F588 == 0 && ov13_02240394(1)) {
        return;
    }
    if (!ov13_022272A0()) {
        return;
    }
    ov13_02240B04(0);
    if (ov13_0224F588 == 0) {
        ov13_0222788C();
        ov13_02240EFC(1, 1);
    }
    ov13_02240EFC(0, 0x15);
    if (ov13_0224F588 == 0) {
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_022368D8);
    } else {
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_02238C4C);
    }
}

BOOL ov13_02238C2C(void) {
    return PAD_DetectFold() ? TRUE : FALSE;
}

void ov13_02238C4C(void) {
    u16 name[11];
    OSOwnerInfo info;
    ov13_0224F58C = 0;
    ov13_02238CE0();
    ov13_02227DE4(8);
    ov13_02227964(2);
    OS_GetOwnerInfo(&info);
    MIi_CpuClear16(0, name, sizeof(name));
    MIi_CpuCopy16(info.nickName, name, info.nickNameLength * 2);
    ov13_02227A60(name, 0x28);
    ov13_02226F58(0);
    ov13_0223B788(ov13_0223902C);
    ov13_0223ABB8(0xB);
    ov13_0222B8E8(ov13_02238D88);
}

void ov13_02238CE0(void) {
    ov13_02227924(ov13_02246784, GX_LoadBG2Char);
    ov13_02227924(ov13_0224679C, GX_LoadBGPltt);
    ov13_02227924(ov13_022467B4, GX_LoadBG2Scr);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}
