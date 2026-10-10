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

void ov13_0223D66C(u8 *a) {
    W = a;
    U32(0x40) = 0;
    U32(0x13a8) = 0;
    U32(0x13ac) = 0;
    U32(0x13b0) = 0;
}

void ov13_0223D6A8(u32 a) {
    if (ov13_0224F5C0.log) {
        ov13_0224F5C0.log(0x8000000, ov13_02247200, ((char **)ov13_02247120)[U32(0x40)]);
    }
    U32(0x40) = a;
    if (ov13_0224F5C0.log) {
        ov13_0224F5C0.log(0x8000000, ov13_02247208, ((char **)ov13_02247120)[U32(0x40)]);
    }
}

void ov13_0223D724(u32 a) {
    if ((u32)(U32(0x40) - 9) > 1) {
        U32(0x54) = a;
    }
}

int ov13_0223D744(void) {
    int r;
    ov13_0223D6A8(3);
    r = WM_SetParentParameter(ov13_0223D788, W);
    if (r == 2) {
        return 1;
    }
    ov13_0223D724(r);
    ov13_0223D6A8(9);
    return 0;
}

void ov13_0223D788(u16 *a) {
    if (a[1] != 0) {
        ov13_0223D724(a[1]);
        ov13_0223D6A8(9);
        return;
    }
    if (U32(0x13ac) != 0) {
        if (ov13_0223D7F4() == 0) {
            ov13_0223D6A8(9);
        }
    } else if (ov13_0223D890() == 0) {
        ov13_0223D6A8(9);
    }
}
