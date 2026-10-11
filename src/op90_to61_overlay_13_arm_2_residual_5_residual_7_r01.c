#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef void (*InitFunc)(char *);
typedef struct { InitFunc f[5]; } InitTable;
typedef struct { u8 v[5]; } Bytes5;
typedef struct {
    u32 attr01;
    u16 attr2;
} Oam;
typedef struct {
    void *bg;
    Oam *oam;
    u8 data[12];
    u8 result;
} Work1B18;
void WifiSetupConnections_FormatIpAddress(char *buf);
void WifiSetupConnections_FormatSubnetMask(char *buf);
void WifiSetupConnections_FormatGateway(char *buf);
void WifiSetupConnections_FormatPrimaryDns(char *buf);
void WifiSetupConnections_FormatSecondaryDns(char *buf);
extern Work1B18 *gWifiSetupAddressEntry;
extern const u8 ov13_022463E0[];
void *WifiSetupHeap_AllocZeroed(u32 size, u32 align);
void WifiSetupApp_GetScreenArgs(int *out, int a);
void WifiSetupAddressEntry_LoadGraphics(void);
void WifiSetupBackground_LoadHeader(int a);
void WifiSetupMessage_Show(int a, int b, int c);
void WifiSetupBanner_Show(int a);
void *WifiSetupText_InitBgCanvas(int a, int b);
Oam *WifiSetupOamTemplate_AllocOam(int a, int b);
void WifiSetupAddressEntry_UpdateCursor(void);
void WifiSetupAddressEntry_DrawDigits(void);
void WifiSetupApp_SetStateFunc(void (*fn)(void));
void WifiSetupAddressEntry_FadeIn(void);
void ov13_02231B18(void);
extern const struct S80 { u8 pad[8]; Bytes5 vals; u8 rest[11]; } ov13_02245480;
extern const InitTable ov13_02245498;
// @fn ov13_02231B18
void ov13_02231B18(void) {
    InitTable funcs = ov13_02245498;
    Bytes5 vals = ov13_02245480.vals;
    int idx;
    Work1B18 *work;
    gWifiSetupAddressEntry = WifiSetupHeap_AllocZeroed(sizeof(Work1B18), 4);
    WifiSetupApp_GetScreenArgs(&idx, 0);
    funcs.f[idx]((char *)gWifiSetupAddressEntry->data);
    work = gWifiSetupAddressEntry;
    if (memcmp(work->data, ov13_022463E0, 3) != 0) {
        work->result = 3;
    } else {
        MI_CpuFill8(work->data, 0, 12);
        gWifiSetupAddressEntry->result = 0;
    }
    WifiSetupAddressEntry_LoadGraphics();
    WifiSetupBackground_LoadHeader(idx + 11);
    WifiSetupMessage_Show(vals.v[idx], -1, 0);
    WifiSetupBanner_Show(2);
    gWifiSetupAddressEntry->bg = WifiSetupText_InitBgCanvas(0, 0);
    gWifiSetupAddressEntry->oam = WifiSetupOamTemplate_AllocOam(0, 0x3f);
    gWifiSetupAddressEntry->oam->attr2 = (gWifiSetupAddressEntry->oam->attr2 & ~0xc00) | 0xc00;
    WifiSetupAddressEntry_UpdateCursor();
    WifiSetupAddressEntry_DrawDigits();
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_FadeIn);
}

const struct S80 ov13_02245480 = {{0x32,0x35,0x35,0,0xb,0,0x10,0},{{0x3F,0x40,0x41,0x42,0x43}},{0,0,0,0x29}};
const InitTable ov13_02245498 = {{WifiSetupConnections_FormatIpAddress, WifiSetupConnections_FormatSubnetMask, WifiSetupConnections_FormatGateway, WifiSetupConnections_FormatPrimaryDns, WifiSetupConnections_FormatSecondaryDns}};
