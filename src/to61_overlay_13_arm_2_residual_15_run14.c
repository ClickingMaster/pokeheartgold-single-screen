#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

struct Connection {
    u16 flags[7];
    u8 entries[1][30];
};

struct State {
    void (*log)(int, char *, ...);
    u8 *work;
};
extern struct Connection *ov13_0224F5BC;
extern u16 *ov13_022468D8[];
extern struct State ov13_0224F5C0;
extern u8 *ov13_0224F5C8;
extern char ov13_02247120[][1];
extern char *ov13_02247120_ptr[];
#define W      ov13_0224F5C0.work
#define U32(o) (*(u32 *)(W + (o)))
#define U16(o) (*(u16 *)(W + (o)))
#define PTR(o) (*(void **)(W + (o)))
#define B      ov13_0224F5C8
#define B32(o) (*(u32 *)(B + (o)))
#define B16(o) (*(u16 *)(B + (o)))
#define B8(o)  (B[(o)])
extern char ov13_02247200[];
extern char ov13_02247208[];
extern char ov13_0224720C[];
extern char ov13_02247238[];
extern char ov13_02247264[];
extern char ov13_02247284[];
extern char ov13_022472B4[];
extern char ov13_022472DC[];
extern char ov13_022472F4[];
extern char ov13_0224730C[];
extern char ov13_02247324[];
extern char ov13_0224733C[];
extern char ov13_02247358[];
extern char ov13_02247388[];
extern char ov13_022473BC[];
extern char ov13_022473E0[];
extern int WM_GetAllowedChannel(void);
extern int WM_SetParentParameter(void (*)(u16 *), void *), WM_SetWEPKey(void (*)(u16 *), int, void *), WM_StartParent(void (*)(u16 *));
extern int WM_StartMP(void (*)(u16 *), void *, u16, void *, u16, u16), WM_StartKeySharing(void *, u16), WM_EndKeySharing(void *);
extern int WM_EndMP(void (*)(u16 *)), WM_EndParent(void (*)(u16 *)), WM_Disconnect(void (*)(u16 *), u16), WM_Reset(void (*)(u16 *)), WM_End(void (*)(u16 *));
extern int WM_MeasureChannel(void (*)(u16 *), u16, u16, u16, u16);
extern int WM_Initialize(void *, void (*)(u16 *), int), WM_SetIndCallback(void (*)(u16 *));
extern void *WM_GetSharedDataAddress(void *, void *, u16);
extern void *MB_CommGetChildUser(u16);
extern int WM_StepDataSharing(void *, void *, void *);
extern int WM_StartDataSharing(void *, u16, u16, u16, int);
extern u16 WM_GetNextTgid(void);
extern u16 WM_GetDispersionBeaconPeriod(void);
extern void MB_End(void);
extern void ov13_0223C9D4(void *);
extern void ov13_0223CEA0(void);
extern void ov13_0223CA14(u32, u16);
extern void *ov13_0223A7F4(u32, u32);
extern void DWCi_BACKUPlRead(void *);
void ov13_0223D6A8(u32);
void ov13_0223D724(u32);
int ov13_0223D7F4(void), ov13_0223D890(void), ov13_0223DA64(void), ov13_0223DC28(void), ov13_0223DD18(void), ov13_0223DE18(void), ov13_0223DE74(void), ov13_0223E3A0(void), ov13_0223E7BC(void);
void ov13_0223E698(void), ov13_0223E6B4(void), ov13_0223F3BC(void);
u16 ov13_0223E014(u16), ov13_0223E1A8(void);
int ov13_0223E188(void (*)(u16 *), u16);
s16 ov13_0223E220(u16);
void ov13_0223D8F4(u16 *), ov13_0223DAFC(u16 *);
void ov13_0223D448(int a);
u16 ov13_0223D45C(void);
u16 ov13_0223D470(int a);
int ov13_0223D4C8(int a);
void *ov13_0223D590(int a);
u16 ov13_0223D5C4(u8 *a);
void ov13_0223D66C(u8 *a);
void ov13_0223D6A8(u32 a);
void ov13_0223D724(u32 a);
int ov13_0223D744(void);
int ov13_0223DC28(void);
int ov13_0223DC64(void);
int ov13_0223DC94(void);
int ov13_0223DD18(void);
int ov13_0223DD64(void);
int ov13_0223DDB4(void);
int ov13_0223DE18(void);
int ov13_0223DE74(void);
void ov13_0223D858(u16 *a);
void ov13_0223DD40(u16 *a);
void ov13_0223DDE4(u16 *a);
void ov13_0223DE50(u16 *a);
void ov13_0223DEA4(u16 *a);
void ov13_0223DED8(u16 *a);
void ov13_0223D788(u16 *a);
int ov13_0223D7F4(void);
int ov13_0223D890(void);
int ov13_0223DA64(void);
void ov13_0223DCC4(u16 *a);
void ov13_0223DF00(u32 a);
u16 ov13_0223DF14(void);
u32 ov13_0223DF28(void);
int ov13_0223E188(void (*cb)(u16 *), u16 ch);
void ov13_0223E380(u16 *a);
int ov13_0223E3A0(void);
void ov13_0223E3EC(u16 *a);
int ov13_0223E308(void);
void ov13_0223E5B4(void *a);
void *ov13_0223E5C8(u16 a);
int ov13_0223E5F0(void *a);
void ov13_0223E698(void);
int ov13_0223E7BC(void);
void ov13_0223E9D8(void);
int ov13_0223EA58(void);
int ov13_0223EAA0(void);
void ov13_0223EE58(u8 *a, u8 *b);
void *ov13_0223EE9C(void);
void ov13_0223EF54(void);
void ov13_0223F130(void);
void ov13_0223F338(void);
int ov13_0223F350(void);
int ov13_0223F380(u8 *a);
void ov13_0223F3BC(void);
u16 ov13_0223F64C(void);

extern int WM_GetLinkLevel(void);
int ov13_0223E43C(u32, u16, u16);
void ov13_0223F400(u16, u16, void *);
void ov13_0223F568(void);
extern int ov13_0223CF80(void);
extern void ov13_0223CFE8(void);
extern void ov13_0223D0E0(void);
extern void ov13_0223CB34(void *, u16);
void ov13_0223D448(int a);
u16 ov13_0223D45C(void);
u16 ov13_0223D470(int a);
int ov13_0223D4C8(int a);
void *ov13_0223D590(int a);
u16 ov13_0223D5C4(u8 *a);
void ov13_0223D66C(u8 *a);
void ov13_0223D6A8(u32 a);
void ov13_0223D724(u32 a);
int ov13_0223D744(void);
void ov13_0223D788(u16 *a);
int ov13_0223D7F4(void);
void ov13_0223D858(u16 *a);
int ov13_0223D890(void);
int ov13_0223DA64(void);
int ov13_0223DC28(void);
int ov13_0223DC64(void);
int ov13_0223DC94(void);
void ov13_0223DCC4(u16 *a);
int ov13_0223DD18(void);
void ov13_0223DD40(u16 *a);
int ov13_0223DD64(void);
int ov13_0223DDB4(void);
void ov13_0223DDE4(u16 *a);
int ov13_0223DE18(void);
void ov13_0223DE50(u16 *a);
int ov13_0223DE74(void);
void ov13_0223DEA4(u16 *a);
void ov13_0223DED8(u16 *a);
void ov13_0223DF00(u32 a);
u16 ov13_0223DF14(void);
u32 ov13_0223DF28(void);
int ov13_0223E188(void (*cb)(u16 *), u16 ch);
int ov13_0223E308(void);
void ov13_0223E380(u16 *a);
int ov13_0223E3A0(void);
void ov13_0223E3EC(u16 *a);
void ov13_0223E5B4(void *a);
void *ov13_0223E5C8(u16 a);
int ov13_0223E5F0(void *a);
void ov13_0223E698(void);
int ov13_0223E7BC(void);
void ov13_0223E9D8(void);
int ov13_0223EA58(void);
int ov13_0223EAA0(void);
void ov13_0223EE58(u8 *a, u8 *b);
void *ov13_0223EE9C(void);
void ov13_0223EF54(void);
void ov13_0223F130(void);
void ov13_0223F338(void);
int ov13_0223F350(void);
int ov13_0223F380(u8 *a);
void ov13_0223F3BC(void);
u16 ov13_0223F64C(void);
void ov13_0223D8F4(u16 *a);
void ov13_0223DAFC(u16 *a);
int ov13_0223DF3C(void);
u16 ov13_0223E014(u16 channel);
void ov13_0223E0B0(u16 *a);
u16 ov13_0223E1A8(void);
s16 ov13_0223E220(u16 mask);
void ov13_0223E6B4(void);
int ov13_0223E8FC(void);
void ov13_0223EEAC(void);
void ov13_0223EF78(void);
void ov13_0223D448(int a);
u16 ov13_0223D45C(void);
u16 ov13_0223D470(int a);
int ov13_0223D4C8(int a);
void *ov13_0223D590(int a);
u16 ov13_0223D5C4(u8 *a);
void ov13_0223D66C(u8 *a);
void ov13_0223D6A8(u32 a);
void ov13_0223D724(u32 a);
int ov13_0223D744(void);
void ov13_0223D788(u16 *a);
int ov13_0223D7F4(void);
void ov13_0223D858(u16 *a);
int ov13_0223D890(void);
void ov13_0223D8F4(u16 *a);
int ov13_0223DA64(void);
void ov13_0223DAFC(u16 *a);
int ov13_0223DC28(void);
int ov13_0223DC64(void);
int ov13_0223DC94(void);
void ov13_0223DCC4(u16 *a);
int ov13_0223DD18(void);
void ov13_0223DD40(u16 *a);
int ov13_0223DD64(void);
int ov13_0223DDB4(void);
void ov13_0223DDE4(u16 *a);
int ov13_0223DE18(void);
void ov13_0223DE50(u16 *a);
int ov13_0223DE74(void);
void ov13_0223DEA4(u16 *a);
void ov13_0223DED8(u16 *a);
void ov13_0223DF00(u32 a);
u16 ov13_0223DF14(void);
u32 ov13_0223DF28(void);
int ov13_0223DF3C(void);
u16 ov13_0223E014(u16 channel);
void ov13_0223E0B0(u16 *a);
int ov13_0223E188(void (*cb)(u16 *), u16 ch);
u16 ov13_0223E1A8(void);
s16 ov13_0223E220(u16 mask);
int ov13_0223E308(void);
void ov13_0223E380(u16 *a);
int ov13_0223E3A0(void);
void ov13_0223E3EC(u16 *a);
void ov13_0223E5B4(void *a);
void *ov13_0223E5C8(u16 a);
int ov13_0223E5F0(void *a);
void ov13_0223E698(void);
void ov13_0223E6B4(void);
int ov13_0223E7BC(void);
int ov13_0223E8FC(void);
void ov13_0223E9D8(void);
int ov13_0223EA58(void);
int ov13_0223EAA0(void);
void ov13_0223EE58(u8 *a, u8 *b);
void *ov13_0223EE9C(void);
void ov13_0223EEAC(void);
void ov13_0223EF54(void);
void ov13_0223EF78(void);
void ov13_0223F130(void);
void ov13_0223F338(void);
int ov13_0223F350(void);
int ov13_0223F380(u8 *a);
void ov13_0223F3BC(void);
u16 ov13_0223F64C(void);
int ov13_0223E43C(u32 a, u16 b, u16 c);
void ov13_0223E80C(u8 *a, u32 *config);
void ov13_0223F400(u16 a, u16 b, void *data);
void ov13_0223F568(void);
void ov13_0223F15C(void);

struct ArchiveHolder {
    void *tables;
    u8 work[132];
    FSArchive arc;
    u16 lock;
};
extern struct ArchiveHolder *ov13_0224F5CC;
extern char ov13_02247404[];
extern char ov13_0224593C[];
extern char ov13_0224741C[];
extern FSResult ov13_0223F8A0(FSFile *, FSCommandType);
extern FSResult ov13_0223F904(FSArchive *, void *, u32, u32);
extern FSResult ov13_0223F954(FSArchive *, const void *, u32, u32);
extern void *ov13_0224128C(u32, int);
extern void *ov13_0224022C(u32, void *, int);
void ov13_0223D448(int a);
u16 ov13_0223D45C(void);
u16 ov13_0223D470(int a);
int ov13_0223D4C8(int a);
void *ov13_0223D590(int a);
u16 ov13_0223D5C4(u8 *a);
void ov13_0223D66C(u8 *a);
void ov13_0223D6A8(u32 a);
void ov13_0223D724(u32 a);
int ov13_0223D744(void);
void ov13_0223D788(u16 *a);
int ov13_0223D7F4(void);
void ov13_0223D858(u16 *a);
int ov13_0223D890(void);
void ov13_0223D8F4(u16 *a);
int ov13_0223DA64(void);
void ov13_0223DAFC(u16 *a);
int ov13_0223DC28(void);
int ov13_0223DC64(void);
int ov13_0223DC94(void);
void ov13_0223DCC4(u16 *a);
int ov13_0223DD18(void);
void ov13_0223DD40(u16 *a);
int ov13_0223DD64(void);
int ov13_0223DDB4(void);
void ov13_0223DDE4(u16 *a);
int ov13_0223DE18(void);
void ov13_0223DE50(u16 *a);
int ov13_0223DE74(void);
void ov13_0223DEA4(u16 *a);
void ov13_0223DED8(u16 *a);
void ov13_0223DF00(u32 a);
u16 ov13_0223DF14(void);
u32 ov13_0223DF28(void);
int ov13_0223DF3C(void);
u16 ov13_0223E014(u16 channel);
void ov13_0223E0B0(u16 *a);
int ov13_0223E188(void (*cb)(u16 *), u16 ch);
u16 ov13_0223E1A8(void);
s16 ov13_0223E220(u16 mask);
int ov13_0223E308(void);
void ov13_0223E380(u16 *a);
int ov13_0223E3A0(void);
void ov13_0223E3EC(u16 *a);
int ov13_0223E43C(u32 a, u16 b, u16 c);
void ov13_0223E5B4(void *a);
void *ov13_0223E5C8(u16 a);
int ov13_0223E5F0(void *a);
void ov13_0223E698(void);
void ov13_0223E6B4(void);
int ov13_0223E7BC(void);
void ov13_0223E80C(u8 *a, u32 *config);
int ov13_0223E8FC(void);
void ov13_0223E9D8(void);
int ov13_0223EA58(void);
int ov13_0223EAA0(void);
void ov13_0223EE58(u8 *a, u8 *b);
void *ov13_0223EE9C(void);
void ov13_0223EEAC(void);
void ov13_0223EF54(void);
void ov13_0223EF78(void);
void ov13_0223F130(void);
void ov13_0223F15C(void);
void ov13_0223F338(void);
int ov13_0223F350(void);
int ov13_0223F380(u8 *a);
void ov13_0223F3BC(void);
void ov13_0223F400(u16 a, u16 b, void *data);
void ov13_0223F568(void);
u16 ov13_0223F64C(void);
void ov13_0223F664(void);

void ov13_0223F130(void) {
    ov13_0223F3BC();
    ov13_0223E5B4(ov13_0223F380);
    B8(0xa90) = 8;
}

void ov13_0223F15C(void) {
    switch (ov13_0223DF28()) {
    case 1:
        ov13_0223E43C(4, B16(0x648) + 1, B16(0x64a));
        return;
    case 4:
    case 5:
    case 6:
        ov13_0223F400(0, B16(0x200), (void *)(B32(0xaa4) + (B16(0x200) % 16) * 64));
        ov13_0223F568();
        if (B8(0xa90) == 27) {
            return;
        }
        if (B32(0x204) > 480) {
            B8(0xa90) = 27;
            return;
        }
        if (ov13_0223F64C() == 16 || ov13_0223F64C() == 32) {
            ov13_0223F64C();
            B8(0xa90) = 10;
            return;
        }
        if (ov13_0223F64C() == 64) {
            B8(0xa90) = 11;
            return;
        }
        if (ov13_0223F64C() == 255) {
            B8(0xa90) = 27;
            return;
        }
        if (ov13_0223F64C() == 80) {
            B8(0xa90) = 21;
            return;
        }
        if (ov13_0223F64C() == 96) {
            B8(0xa90) = 24;
            return;
        }
        if (ov13_0223F64C() == 112) {
            B8(0xa90) = 27;
            return;
        }
        if (ov13_0223F64C() == 0) {
            B8(0xa90) = 8;
            return;
        }
        if (ov13_0223F64C() == 189) {
            B8(0xa90) = 9;
            return;
        }
        B8(0xa90) = 31;
        return;
    default:
        return;
    }
}

void ov13_0223F338(void) {
    B8(0xa90) = 12;
}

int ov13_0223F350(void) {
    if (ov13_0223DF28() != 1) {
        return 0;
    }
    ov13_0223E7BC();
    B8(0xa90) = 1;
    return 1;
}

int ov13_0223F380(u8 *a) {
    u16 n = ov13_0223D5C4(a + 10);
    if (n == 0) {
        return 0;
    }
    ((void **)(B + 0xaa0))[n - 1] = ov13_0223D590(n);
    return 1;
}

void ov13_0223F3BC(void) {
    MI_CpuFill8(B + 0x100, 0, 256);
    MI_CpuFill8(B, 0, 256);
    *(u8 **)(B + 0xab0) = B;
}

void ov13_0223F400(u16 a, u16 b, void *data) {
    u16 i;
    if (B8(0xa93) == 1) {
        *(u16 *)B32(0xab0) = a;
        *(u16 *)(B32(0xab0) + 2) = b;
        MI_CpuCopy8(data, (void *)(B32(0xab0) + 4), 64);
    } else {
        B32(0x204)
        ++;
        *(u16 *)B32(0xab0) = 188;
        *(u8 *)(B32(0xab0) + 4) = B8(0xa92);
    }
    if (ov13_0223DF28() != 5) {
        return;
    }
    if (ov13_0223E5F0(B) == 0) {
        B32(0x204) += 4;
        return;
    }
    if (B8(0xa93) == 0) {
        B32(0x204)
        ++;
    } else {
        B32(0x204) = 0;
        if (ov13_0223DF14() != 3) {
            B8(0xa90) = 27;
            return;
        }
    }
    for (i = 0; i < 2; i++) {
        void *p = ov13_0223E5C8(i);
        if (p != 0) {
            MI_CpuCopy8(p, B + 0x100 + i * 68, 68);
            ((u32 *)(B + 0x208))[i] = 1;
        } else {
            ((u32 *)(B + 0x208))[i] = 0;
        }
    }
}
