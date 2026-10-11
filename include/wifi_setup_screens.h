#ifndef POKEHEARTGOLD_WIFI_SETUP_SCREENS_H
#define POKEHEARTGOLD_WIFI_SETUP_SCREENS_H

#include "global.h"

#include "wifi_setup_ui.h"

// Access-point setup screens (src/wifi_setup_screens.c): the list screen and the text entry screen
// shared with the screens that follow them.

// One row of the list screen (0x2A bytes).
typedef struct WifiSetupListRow {
    u8 text[40];
    u8 kind;
    u8 unk_29;
} WifiSetupListRow;

// Work of the list screen (allocated by the screen, gWifiSetupListScreen.work).
typedef struct WifiSetupListWork {
    union {
        WifiSetupListRow *rows; // 0x00
        u8 *rowData;
    };

    void *unk_04;           // 0x04
    void *unk_08;           // 0x08
    void *buffer;           // 0x0C
    GXOamAttr *rowOams[5];  // 0x10
    GXOamAttr *iconOams[5]; // 0x24
    void *task;             // 0x38
    void *unk_3C;           // 0x3C
    u16 total;              // 0x40, scroll range
    u16 unk_42[3];          // 0x42, character numbers per row kind
    u16 unk_48[4];          // 0x48
    s8 selected;            // 0x50
    u8 count;               // 0x51, number of rows
    u8 index;               // 0x52
    u8 unk_53;              // 0x53
    u8 unk_54;              // 0x54
    u8 delay;               // 0x55
    u8 unk_56;              // 0x56
    u8 unk_57;              // 0x57
    u8 unk_58;              // 0x58
    u8 unk_59;              // 0x59
} WifiSetupListWork;

typedef struct WifiSetupListScreen {
    u8 selected;
    u8 unk_01;
    u16 position; // scroll position
    WifiSetupListWork *work;
} WifiSetupListScreen;

// Text entry screen: a 32-character text and its length.
typedef struct WifiSetupTextEntry {
    WifiSetupCanvas *canvas;
    GXOamAttr *cursor;
    u8 text[33];
    u8 length;
    u8 flag;
} WifiSetupTextEntry;

extern WifiSetupListScreen gWifiSetupListScreen;
extern WifiSetupTextEntry *gWifiSetupTextEntry;

void WifiSetupClient_Start(void);
void WifiSetupClient_Stop(void);
int WifiSetupClient_GetResult(void);
void WifiSetupClient_StoreResult(void);
void WifiSetupClient_OnStatus(s32 *status);
void *WifiSetupClient_Alloc(u32 size);
void WifiSetupClient_Free(void *p);
void ov13_0222C10C(void);
void ov13_0222C148(void);
void ov13_0222C1BC(void);
void ov13_0222C39C(void);
void ov13_0222C3D8(void);
void ov13_0222C44C(void);
void ov13_0222C4C8(void);
void ov13_0222C5B4(void);
void ov13_0222C608(void);
void ov13_0222C644(void);
void ov13_0222C66C(void);
void ov13_0222C680(void);
void ov13_0222C7AC(void);
void ov13_0222C7B0(void);
void ov13_0222C800(void);
void ov13_0222C820(void);
void ov13_0222C86C(void);
void ov13_0222C914(int dir);
void ov13_0222C984(void);
void ov13_0222C9C4(void);
void ov13_0222CA0C(void);
void ov13_0222CA8C(void);
void ov13_0222CAE0(void);
void ov13_0222CB1C(void);
void ov13_0222CB44(void);
void ov13_0222CB58(void);
void ov13_0222CB90(void);
void ov13_0222CB94(void);
void ov13_0222CBFC(void);
void ov13_0222CC1C(void);
void ov13_0222CC80(void);
void ov13_0222CD38(void);
void ov13_0222CD88(void);
void ov13_0222CDA8(void);
void ov13_0222CDCC(void);
void ov13_0222CE4C(void);
void ov13_0222CE80(void);
void ov13_0222CEAC(void);
void ov13_0222CED4(void);
void ov13_0222CEE8(void);
void ov13_0222CF08(void);
void ov13_0222CF0C(void);
void ov13_0222CF34(void);
void ov13_0222CF54(void);
void ov13_0222CFA0(void);
void ov13_0222CFD4(void);
void ov13_0222D008(void);
void ov13_0222D088(void);
void ov13_0222D25C(void);
void ov13_0222D2B0(void);
void ov13_0222D2EC(void);
void ov13_0222D314(void);
void ov13_0222D328(void);
void ov13_0222D348(void);
void ov13_0222D34C(void);
void ov13_0222D374(void);
void ov13_0222D394(void);
void ov13_0222D3E4(void);
void ov13_0222D454(void);
void ov13_0222D51C(void);
void ov13_0222D6B8(void);
void ov13_0222D70C(void);
void ov13_0222D748(void);
void ov13_0222D770(void);
void ov13_0222D784(void);
void ov13_0222D878(void);
void ov13_0222D87C(void);
void ov13_0222D920(void);
void ov13_0222D940(void);
void ov13_0222D9A4(void);
void ov13_0222DAB0(int dir);
void ov13_0222DB40(void);
void ov13_0222DB6C(void);
void ov13_0222DBA4(void);
void ov13_0222DC24(void);
void ov13_0222DC58(void);
void ov13_0222DC88(void);
void ov13_0222DC98(void);
void ov13_0222DC9C(void);
void ov13_0222DCA0(void);
void ov13_0222DCC8(void);
void ov13_0222DD5C(int a);
void ov13_0222DDE4(void);
void ov13_0222DE04(void);
void ov13_0222DE6C(void);
void ov13_0222DEA0(void);
void ov13_0222DECC(void);
void ov13_0222DEF4(void);
void ov13_0222DF08(void);
void ov13_0222DF28(void);
void ov13_0222DF2C(void);
void ov13_0222DF54(void);
void ov13_0222DF74(void);
void ov13_0222DFC0(void);
void ov13_0222DFF4(void);
void ov13_0222E014(void);
void ov13_0222E07C(void);
void ov13_0222E0B0(void);
void ov13_0222E0EC(void);
void ov13_0222E114(void);
void ov13_0222E128(void);
void ov13_0222E148(void);
void ov13_0222E14C(void);
void ov13_0222E174(void);
void ov13_0222E194(void);
void ov13_0222E1D0(void);
void ov13_0222E220(void);
void ov13_0222E268(void);
void ov13_0222E2E8(void);
void ov13_0222E33C(void);
void ov13_0222E368(void);
void ov13_0222E390(void);
void ov13_0222E3A4(void);
void ov13_0222E3DC(void);
void ov13_0222E3E0(void);
void ov13_0222E430(void);
void ov13_0222E450(void);
void ov13_0222E4B4(void);
void ov13_0222E56C(void);
void ov13_0222E59C(void);
void ov13_0222E608(void);
void ov13_0222E63C(void);
void ov13_0222E66C(void);
void ov13_0222E67C(void);
void ov13_0222E680(void);
void ov13_0222E6BC(void);
void ov13_0222E720(void);
void ov13_0222E754(void);
void ov13_0222E790(void);
void ov13_0222E810(void);
void ov13_0222E878(void);
void ov13_0222E8B4(void);
void ov13_0222E8E8(void);
void ov13_0222E8FC(void);
void ov13_0222E934(void);
void ov13_0222E938(void);
void ov13_0222E990(void);
void ov13_0222E9B0(void);
void ov13_0222EA14(void);
void ov13_0222EABC(void);
void ov13_0222EB8C(void);
void ov13_0222EC0C(void);
void ov13_0222EC40(void);
void ov13_0222EC64(void);
void ov13_0222EC88(void);
void ov13_0222ECF8(void);
void ov13_0222ECFC(void);
void ov13_0222ED00(void);
void ov13_0222ED5C(void);
void ov13_0222EDB8(void);
void ov13_0222EE80(void);
int ov13_0222EE94(void);
void ov13_0222EEB4(void);
void ov13_0222EF00(void);
void ov13_0222EF80(void);
void ov13_0222EFB4(void);
void ov13_0222EFE0(void);
void ov13_0222F004(void);
void ov13_0222F018(void);
void ov13_0222F050(void);
void ov13_0222F054(void);
void ov13_0222F080(void);
void ov13_0222F0B8(void);
void ov13_0222F138(void);
void ov13_0222F230(int a);
int ov13_0222F28C(void);
void ov13_0222F2AC(void);
void ov13_0222F30C(void);
void ov13_0222F3B4(void);
void ov13_0222F408(void);
void ov13_0222F444(void);
void ov13_0222F48C(void);
void ov13_0222F560(void);
void ov13_0222F598(void);
void ov13_0222F59C(void);
void ov13_0222F5E8(void);
void ov13_0222F624(void);
void ov13_0222F688(void);
void ov13_0222F764(void);
void ov13_0222F780(void);
void ov13_0222F7AC(void);
void ov13_0222F7CC(void);
void ov13_0222F818(void);
void ov13_0222F848(void);
void ov13_0222F884(void);
void ov13_0222F8F0(void);
void ov13_0222F924(void);
void ov13_0222F948(void);
void ov13_0222F958(void);
void ov13_0222F95C(void);
void ov13_0222F998(void);
void ov13_0222FA08(void);
void WifiSetupListScreen_Init(void);
void WifiSetupListScreen_LoadGraphics(void);
void ov13_0222FC68(void);
void ov13_0222FD10(void);
void ov13_0222FE84(void);
void ov13_0222FED8(void);
void ov13_0222FF14(void);
void ov13_0222FF3C(void);
void ov13_0222FF50(void);
void ov13_022301A4(void);
void ov13_02230414(void);
void ov13_02230520(void);
void ov13_02230540(void);
void ov13_022305B0(void);
void ov13_022307AC(void);
void ov13_02230854(int index, int row);
void ov13_022309CC(int a, int b);
void ov13_02230A58(void);
void ov13_02230B14(void *task);
void ov13_02230BD0(void *task);
void ov13_02230C68(void);
void ov13_02230CC4(void);
void ov13_02230D10(int input);
void ov13_02230E5C(void);
void ov13_02230ED0(void);
void ov13_02230F54(void);
void ov13_02230F84(void);
void ov13_02230FB0(void);
void WifiSetupTextEntry_Init(void);
void WifiSetupTextEntry_LoadGraphics(void);
void ov13_022311C8(void);
void ov13_0223121C(void);
void ov13_02231298(void);
void ov13_022312B8(void);
void ov13_022312C8(void);
void ov13_02231488(void);
void ov13_0223148C(void);
void ov13_022314A8(void);
void ov13_022314D8(void);
void ov13_022315B0(void);
void ov13_022316B4(void);
void ov13_022317A8(void);
void ov13_02231810(void);
void ov13_0223186C(void);
int ov13_0223191C(void);
void ov13_02231AA8(void);
void ov13_02231AD4(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_SCREENS_H
