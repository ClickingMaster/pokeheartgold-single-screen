#include "application/main_menu/connect_to_ranger.h"

#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/main_menu/main_menu.h"
#include "application/main_menu/main_menu_internal.h"
#include "application/main_menu/main_menu_shared.h"
#include "application/main_menu/ranger_link.h"
#include "application/main_menu/wonder_card_album.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0017.h"
#include "msgdata/msg/msg_0247.h"

#include "bg_window.h"
#include "comm_wireless.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gf_rtc.h"
#include "heap.h"
#include "main.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "render_window.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "title_screen.h"
#include "wireless_status_icon.h"

// Main menu option "Connect to Pokemon Ranger": receives the Manaphy Egg from Pokemon Ranger over DS
// wireless and stores it as a Wonder Card.

struct ConnectToRangerApp {
    enum HeapID heapID;             // 0x0000
    BgConfig *bgConfig;             // 0x0004
    SaveData *saveData;             // 0x0008
    Pokedex *pokedex;               // 0x000C
    PlayerProfile *profile;         // 0x0010
    Options *options;               // 0x0014
    Window messageWindow;           // 0x0018
    Window statusWindow;            // 0x0028
    Window playerInfoWindow;        // 0x0038
    MainMenuTextPrinter statusText; // 0x0048
    MainMenuTextPrinter titleText;  // 0x009C
    MainMenuTextPrinter playerText; // 0x00F0
    u32 nextFreeTile;               // 0x0144
    BOOL linkStarted;               // 0x0148
    int unk_14C;                    // 0x014C
    int connectResult;              // 0x0150
    int unk_154;                    // 0x0154
    BOOL linkInitialised;           // 0x0158
    int connectState;               // 0x015C
    int timer;                      // 0x0160
    int partnerIndex;               // 0x0164
    u8 filler_168[4];               // 0x0168
    u8 linkWork[0x3004];            // 0x016C
    Sprite *sprite;                 // 0x3170
    MysteryGiftSave *mysteryGift;   // 0x3174
    u8 filler_3178[4];              // 0x3178
    int textPrinterId;              // 0x317C
    WonderCard wonderCard;          // 0x3180
    WaitingIcon *waitingIcon;       // 0x34D8
}; // size: 0x34DC

enum ConnectToRangerState {
    CONNECT_TO_RANGER_STATE_SETUP,
    CONNECT_TO_RANGER_STATE_POKEDEX_REQUIRED,
    CONNECT_TO_RANGER_STATE_PRINT_SCREEN,
    CONNECT_TO_RANGER_STATE_GIFTS_FULL,
    CONNECT_TO_RANGER_STATE_TRANSFER,
    CONNECT_TO_RANGER_STATE_SHOW_CARD,
    CONNECT_TO_RANGER_STATE_WAIT_CARD_INPUT,
    CONNECT_TO_RANGER_STATE_RESET = 11,
    CONNECT_TO_RANGER_STATE_EXIT,
    CONNECT_TO_RANGER_STATE_WAIT_FADE,
};

// Connection states of ConnectToRanger_RunTransfer
enum {
    RANGER_CONNECT_START,
    RANGER_CONNECT_WAIT_LINK,
    RANGER_CONNECT_SEARCH,
    RANGER_CONNECT_WAIT_PARTNER,
    RANGER_CONNECT_CONNECT_PARTNER,
    RANGER_CONNECT_WAIT_CONNECTED = 7,
    RANGER_CONNECT_SEND_REQUEST,
    RANGER_CONNECT_WAIT_REQUEST_SENT,
    RANGER_CONNECT_WAIT_TOUCH,
    RANGER_CONNECT_SEND_PLAYER,
    RANGER_CONNECT_WAIT_PLAYER_SENT,
    RANGER_CONNECT_SAVE_DELAY,
    RANGER_CONNECT_SAVING,
    RANGER_CONNECT_RANDOM_DELAY,
    RANGER_CONNECT_SEND_SAVED,
    RANGER_CONNECT_WAIT_ACK,
    RANGER_CONNECT_FINISH_SAVE,
    RANGER_CONNECT_DONE_DELAY,
    RANGER_CONNECT_DONE,
    RANGER_CONNECT_LINK_LOST = 22,
    RANGER_CONNECT_TIMEOUT,
    RANGER_CONNECT_CANCEL = 26,
    RANGER_CONNECT_END,
    RANGER_CONNECT_ERROR,
    RANGER_CONNECT_FOUND,
    RANGER_CONNECT_WAIT_A,
};

u16 gConnectToRanger_PlayerName[8]; // player name in Pokemon Ranger's character set
u16 gConnectToRanger_TrainerId[10]; // trainer ID as five digits

extern void CommWireless_Terminate(void);
extern void WirelessStatusIcon_SetLevel(int level);

// Wireless link with Pokemon Ranger (link_status part of this overlay)

static const GraphicsModes sConnectToRangerGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

const OverlayManagerTemplate gApp_MainMenu = { MainMenuApp_Init, MainMenuApp_Main, MainMenuApp_Exit, FS_OVERLAY_ID_NONE };

const OverlayManagerTemplate gApp_MainMenu_SelectOption_ConnectToRanger = { ConnectToRanger_Init, ConnectToRanger_Main, ConnectToRanger_Exit, FS_OVERLAY_ID_NONE };

static const GraphicsBanks sConnectToRangerGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_0_B,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

static RangerLinkConfig sConnectToRangerLinkConfig = {
    .receiver = {
                 .gameCode = 22,
                 .version = 1,
                 .encrypted = 0,
                 .id = 21,
                 },
    .name2 = gConnectToRanger_TrainerId,
    .name1 = gConnectToRanger_PlayerName,
};

// { Ranger character, game character }
static u16 sRangerCharTable[291][2] = {
    { 0x0000, 0xFFFF },
    { 0x0021, 0x00E1 },
    { 0x0023, 0x01C0 },
    { 0x0025, 0x01D2 },
    { 0x0027, 0x00EA },
    { 0x0028, 0x00EB },
    { 0x0029, 0x01BF },
    { 0x002A, 0x01BD },
    { 0x002B, 0x01AD },
    { 0x002C, 0x01BE },
    { 0x002E, 0x01B1 },
    { 0x0030, 0x00A2 },
    { 0x0031, 0x00A3 },
    { 0x0032, 0x00A4 },
    { 0x0033, 0x00A5 },
    { 0x0034, 0x00A6 },
    { 0x0035, 0x00A7 },
    { 0x0036, 0x00A8 },
    { 0x0037, 0x00A9 },
    { 0x0038, 0x00AA },
    { 0x0039, 0x00AB },
    { 0x003A, 0x01C4 },
    { 0x003B, 0x01C5 },
    { 0x003C, 0x00EC },
    { 0x003E, 0x00ED },
    { 0x003F, 0x01AC },
    { 0x0041, 0x00AC },
    { 0x0042, 0x00AD },
    { 0x0043, 0x00AE },
    { 0x0044, 0x00AF },
    { 0x0045, 0x00B0 },
    { 0x0046, 0x00B1 },
    { 0x0047, 0x00B2 },
    { 0x0048, 0x00B3 },
    { 0x0049, 0x00B4 },
    { 0x004A, 0x00B5 },
    { 0x004B, 0x00B6 },
    { 0x004C, 0x00B7 },
    { 0x004D, 0x00B8 },
    { 0x004E, 0x00B9 },
    { 0x004F, 0x00BA },
    { 0x0050, 0x00BB },
    { 0x0051, 0x00BC },
    { 0x0052, 0x00BD },
    { 0x0053, 0x00BE },
    { 0x0054, 0x00BF },
    { 0x0055, 0x00C0 },
    { 0x0056, 0x00C1 },
    { 0x0057, 0x00C2 },
    { 0x0058, 0x00C3 },
    { 0x0059, 0x00C4 },
    { 0x005A, 0x00C5 },
    { 0x0061, 0x00C6 },
    { 0x0062, 0x00C7 },
    { 0x0063, 0x00C8 },
    { 0x0064, 0x00C9 },
    { 0x0065, 0x00CA },
    { 0x0066, 0x00CB },
    { 0x0067, 0x00CC },
    { 0x0068, 0x00CD },
    { 0x0069, 0x00CE },
    { 0x006A, 0x00CF },
    { 0x006B, 0x00D0 },
    { 0x006C, 0x00D1 },
    { 0x006D, 0x00D2 },
    { 0x006E, 0x00D3 },
    { 0x006F, 0x00D4 },
    { 0x0070, 0x00D5 },
    { 0x0071, 0x00D6 },
    { 0x0072, 0x00D7 },
    { 0x0073, 0x00D8 },
    { 0x0074, 0x00D9 },
    { 0x0075, 0x00DA },
    { 0x0076, 0x00DB },
    { 0x0077, 0x00DC },
    { 0x0078, 0x00DD },
    { 0x0079, 0x00DE },
    { 0x007A, 0x00DF },
    { 0x0081, 0x0002 },
    { 0x0082, 0x0003 },
    { 0x0083, 0x0004 },
    { 0x0084, 0x0005 },
    { 0x0085, 0x0006 },
    { 0x0086, 0x0007 },
    { 0x0087, 0x0008 },
    { 0x0088, 0x0009 },
    { 0x0089, 0x000A },
    { 0x008A, 0x000B },
    { 0x008B, 0x000C },
    { 0x008C, 0x000D },
    { 0x008D, 0x000E },
    { 0x008E, 0x000F },
    { 0x008F, 0x0010 },
    { 0x0090, 0x0011 },
    { 0x0091, 0x0012 },
    { 0x0092, 0x0013 },
    { 0x0093, 0x0014 },
    { 0x0094, 0x0015 },
    { 0x0095, 0x0016 },
    { 0x0096, 0x0017 },
    { 0x0097, 0x0018 },
    { 0x0098, 0x0019 },
    { 0x0099, 0x001A },
    { 0x009A, 0x001B },
    { 0x009B, 0x001C },
    { 0x009C, 0x001D },
    { 0x009D, 0x001E },
    { 0x009E, 0x001F },
    { 0x009F, 0x0020 },
    { 0x00A0, 0x0021 },
    { 0x00A1, 0x0022 },
    { 0x00A2, 0x0023 },
    { 0x00A3, 0x0024 },
    { 0x00A4, 0x0025 },
    { 0x00A5, 0x0026 },
    { 0x00A6, 0x0027 },
    { 0x00A7, 0x0028 },
    { 0x00A8, 0x0029 },
    { 0x00A9, 0x002A },
    { 0x00AA, 0x002B },
    { 0x00AB, 0x002C },
    { 0x00AC, 0x002D },
    { 0x00AD, 0x002E },
    { 0x00AE, 0x002F },
    { 0x00AF, 0x0030 },
    { 0x00B0, 0x0031 },
    { 0x00B1, 0x0032 },
    { 0x00B2, 0x0033 },
    { 0x00B3, 0x0034 },
    { 0x00B4, 0x0035 },
    { 0x00B5, 0x0036 },
    { 0x00B6, 0x0037 },
    { 0x00B7, 0x0038 },
    { 0x00B8, 0x0039 },
    { 0x00B9, 0x003A },
    { 0x00BA, 0x003B },
    { 0x00BB, 0x003C },
    { 0x00BC, 0x003D },
    { 0x00BD, 0x003E },
    { 0x00BE, 0x003F },
    { 0x00BF, 0x0040 },
    { 0x00C0, 0x0041 },
    { 0x00C1, 0x0042 },
    { 0x00C2, 0x0043 },
    { 0x00C3, 0x0044 },
    { 0x00C4, 0x0045 },
    { 0x00C5, 0x0046 },
    { 0x00C6, 0x0047 },
    { 0x00C7, 0x0048 },
    { 0x00C8, 0x0049 },
    { 0x00C9, 0x004A },
    { 0x00CA, 0x004B },
    { 0x00CB, 0x004C },
    { 0x00CC, 0x004D },
    { 0x00CD, 0x004E },
    { 0x00CF, 0x004F },
    { 0x00D2, 0x0050 },
    { 0x00D3, 0x0051 },
    { 0x00E1, 0x0052 },
    { 0x00E2, 0x0053 },
    { 0x00E3, 0x0054 },
    { 0x00E4, 0x0055 },
    { 0x00E5, 0x0056 },
    { 0x00E6, 0x0057 },
    { 0x00E7, 0x0058 },
    { 0x00E8, 0x0059 },
    { 0x00E9, 0x005A },
    { 0x00EA, 0x005B },
    { 0x00EB, 0x005C },
    { 0x00EC, 0x005D },
    { 0x00ED, 0x005E },
    { 0x00EE, 0x005F },
    { 0x00EF, 0x0060 },
    { 0x00F0, 0x0061 },
    { 0x00F1, 0x0062 },
    { 0x00F2, 0x0063 },
    { 0x00F3, 0x0064 },
    { 0x00F4, 0x0065 },
    { 0x00F5, 0x0066 },
    { 0x00F6, 0x0067 },
    { 0x00F7, 0x0068 },
    { 0x00F8, 0x0069 },
    { 0x00F9, 0x006A },
    { 0x00FA, 0x006B },
    { 0x00FB, 0x006C },
    { 0x00FC, 0x006D },
    { 0x00FD, 0x006E },
    { 0x00FE, 0x006F },
    { 0x00FF, 0x0070 },
    { 0x0100, 0x0071 },
    { 0x0101, 0x0072 },
    { 0x0102, 0x0073 },
    { 0x0103, 0x0074 },
    { 0x0104, 0x0075 },
    { 0x0105, 0x0076 },
    { 0x0106, 0x0077 },
    { 0x0107, 0x0078 },
    { 0x0108, 0x0079 },
    { 0x0109, 0x007A },
    { 0x010A, 0x007B },
    { 0x010B, 0x007C },
    { 0x010C, 0x007D },
    { 0x010D, 0x007E },
    { 0x010E, 0x007F },
    { 0x010F, 0x0080 },
    { 0x0110, 0x0081 },
    { 0x0111, 0x0082 },
    { 0x0112, 0x0083 },
    { 0x0113, 0x0084 },
    { 0x0114, 0x0085 },
    { 0x0115, 0x0086 },
    { 0x0116, 0x0087 },
    { 0x0117, 0x0088 },
    { 0x0118, 0x0089 },
    { 0x0119, 0x008A },
    { 0x011A, 0x008B },
    { 0x011B, 0x008C },
    { 0x011C, 0x008D },
    { 0x011D, 0x008E },
    { 0x011E, 0x008F },
    { 0x011F, 0x0090 },
    { 0x0120, 0x0091 },
    { 0x0121, 0x0092 },
    { 0x0122, 0x0093 },
    { 0x0123, 0x0094 },
    { 0x0124, 0x0095 },
    { 0x0125, 0x0096 },
    { 0x0126, 0x0097 },
    { 0x0127, 0x0098 },
    { 0x0128, 0x0099 },
    { 0x0129, 0x009A },
    { 0x012A, 0x009B },
    { 0x012B, 0x009C },
    { 0x012C, 0x009D },
    { 0x012D, 0x009E },
    { 0x012F, 0x009F },
    { 0x0132, 0x00A0 },
    { 0x0133, 0x00A1 },
    { 0x0140, 0x00EA },
    { 0x0141, 0x00EB },
    { 0x0142, 0x00E8 },
    { 0x0143, 0x00E9 },
    { 0x0146, 0x00E6 },
    { 0x0147, 0x00E5 },
    { 0x0148, 0x00EE },
    { 0x0149, 0x00EF },
    { 0x014A, 0x00E4 },
    { 0x014B, 0x0112 },
    { 0x014D, 0x011C },
    { 0x014E, 0x011E },
    { 0x014F, 0x011D },
    { 0x0150, 0x011B },
    { 0x017C, 0x019F },
    { 0x017D, 0x01A0 },
    { 0x0180, 0x015F },
    { 0x0181, 0x0160 },
    { 0x0182, 0x0161 },
    { 0x0184, 0x0163 },
    { 0x0187, 0x0166 },
    { 0x0189, 0x0168 },
    { 0x018A, 0x0169 },
    { 0x018B, 0x016A },
    { 0x018C, 0x016B },
    { 0x018D, 0x016C },
    { 0x018E, 0x016D },
    { 0x018F, 0x016E },
    { 0x0193, 0x0172 },
    { 0x0194, 0x0173 },
    { 0x0196, 0x0175 },
    { 0x019A, 0x0179 },
    { 0x019B, 0x017A },
    { 0x019C, 0x017B },
    { 0x019F, 0x017E },
    { 0x01A0, 0x017F },
    { 0x01A1, 0x0180 },
    { 0x01A2, 0x0181 },
    { 0x01A4, 0x0183 },
    { 0x01A7, 0x0186 },
    { 0x01A9, 0x0188 },
    { 0x01AA, 0x0189 },
    { 0x01AB, 0x018A },
    { 0x01AC, 0x018B },
    { 0x01AD, 0x018C },
    { 0x01AE, 0x018D },
    { 0x01AF, 0x018E },
    { 0x01B3, 0x0192 },
    { 0x01B4, 0x0193 },
    { 0x01B6, 0x0195 },
    { 0x01BA, 0x0199 },
    { 0x01BB, 0x019A },
    { 0x01BC, 0x019B },
};

static u8 sConnectToRangerLinkCommand[42] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

u16 ConnectToRanger_ConvertCharToRanger(u16 c) {
    u32 i;
    for (i = 0; i < NELEMS(sRangerCharTable); i++) {
        if (sRangerCharTable[i][1] == c) {
            return sRangerCharTable[i][0];
        }
    }
    return 0x3F;
}

void ConnectToRanger_FormatNumber5Digits(u16 *dst, int value) {
    dst[0] = value / 10000 + '0';
    value %= 10000;
    dst[1] = value / 1000 + '0';
    value %= 1000;
    dst[2] = value / 100 + '0';
    value %= 100;
    dst[3] = value / 10 + '0';
    value %= 10;
    dst[4] = value + '0';
}

void ConnectToRanger_EncodePlayerData(ConnectToRangerApp *app) {
    int i;
    const u16 *src = PlayerProfile_GetNamePtr(app->profile);
    u16 *dst = gConnectToRanger_PlayerName;
    for (i = 0; i < 8; i++) {
        *dst = ConnectToRanger_ConvertCharToRanger(*src);
        src++;
        dst++;
    }
    ConnectToRanger_FormatNumber5Digits(gConnectToRanger_TrainerId, PlayerProfile_GetTrainerID_VisibleHalf(app->profile));
    gConnectToRanger_TrainerId[5] = 0;
}

void ConnectToRanger_SetupGraphics(ConnectToRangerApp *app) {
    GraphicsBanks banks = sConnectToRangerGraphicsBanks;
    GraphicsModes modes = sConnectToRangerGraphicsModes;

    GfGfx_SetBanks(&banks);
    SetBothScreensModesAndDisable(&modes);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_MAIN_0, 1, 0xF000, 0);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_MAIN_1, 1, 0xF800, 0x4000);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_SUB_0, 1, 0x7800, 0);
    MainMenuUtil_InitBg(app->bgConfig, GF_BG_LYR_SUB_1, 1, 0x7000, 0x4000);
    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, app->heapID);
    LoadUserFrameGfx1(app->bgConfig, GF_BG_LYR_MAIN_0, 1, 1, 0, app->heapID);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_0, 10, 2, Options_GetFrame(app->options), app->heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_8_OFFSET, 32, app->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x1400, TRUE, app->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0x600, TRUE, app->heapID);
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 32, 24, 8);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_1);
    ConnectToRanger_LoadSubScreenBg(app);
}

void ConnectToRanger_LoadSubScreenBg(ConnectToRangerApp *app) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 32, app->heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 1, app->bgConfig, GF_BG_LYR_SUB_0, 0, 0x1400, TRUE, app->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 2, app->bgConfig, GF_BG_LYR_SUB_0, 0, 0x600, TRUE, app->heapID);
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 32, 24, 8);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_SUB_0);
}

// Shows "you need a Pokedex" until A or B is pressed. Returns FALSE once the message is closed.
BOOL ConnectToRanger_ShowPokedexRequiredMessage(ConnectToRangerApp *app) {
    MainMenuTextPrinter printer;
    if (WindowIsInUse(&app->messageWindow) == FALSE) {
        MainMenuTextPrinter_Init(&printer, &app->messageWindow, 0, NARC_msg_msg_0017_bin, 1, 1);
        MainMenuTextPrinter_SetSize(&printer, 22, 16, app->nextFreeTile);
        MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &printer, 5, 4, msg_0017_00002);
        return TRUE;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        EraseFrameAndWindow1(&app->messageWindow, FALSE);
        RemoveWindow(&app->messageWindow);
        return FALSE;
    }
    return TRUE;
}

void ConnectToRanger_PrintScreen(ConnectToRangerApp *app) {
    MainMenuTextPrinter_Init(&app->statusText, &app->statusWindow, 0, NARC_msg_msg_0247_bin, 10, 2);
    MainMenuTextPrinter_SetSize(&app->statusText, 27, 4, app->nextFreeTile);
    MainMenuTextPrinter_SetFrameAndFont(&app->statusText, 1, 1);
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00072);
    app->nextFreeTile += MainMenuTextPrinter_GetTileCount(&app->statusText);

    MainMenuTextPrinter_Init(&app->titleText, &app->messageWindow, 0, NARC_msg_msg_0247_bin, 1, 1);
    MainMenuTextPrinter_SetSize(&app->titleText, 30, 4, app->nextFreeTile);
    MainMenuTextPrinter_SetFrameAndFont(&app->titleText, 0, 1);
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->titleText, 1, 1, msg_0247_00011);
    app->nextFreeTile += MainMenuTextPrinter_GetTileCount(&app->titleText);

    MainMenuUtil_InitObjTransfer();
    MainMenuUtil_InitSprites();
    MainMenuUtil_LoadSpriteResources(NARC_a_1_1_3, 14, 11, 13, 12, 0);
    app->sprite = MainMenuUtil_CreateSprite(0, app->sprite, 128, 76, 1);
    ConnectToRanger_PrintPlayerInfo(app);
}

// The "Application No." shown to the player is derived from the console's MAC address.
typedef struct RangerApplicationNo {
    u32 value;
} RangerApplicationNo;

static inline RangerApplicationNo GetApplicationNumber(u8 *macAddress) {
    RangerApplicationNo applicationNo;
    applicationNo.value = RangerLink_ReadMacAddressTail(macAddress);
    return applicationNo;
}

void ConnectToRanger_PrintPlayerInfo(ConnectToRangerApp *app) {
    MessageFormat *msgFormat = MessageFormat_New(app->heapID);
    u32 high, low;
    u8 macAddress[6];
    RangerApplicationNo applicationNo;

    MainMenuTextPrinter_Init(&app->playerText, &app->playerInfoWindow, 0, NARC_msg_msg_0247_bin, 1, 1);
    MainMenuTextPrinter_SetSize(&app->playerText, 26, 4, app->nextFreeTile);
    MainMenuTextPrinter_SetFrameAndFont(&app->playerText, 0, 1);
    app->playerText.msgFormat = msgFormat;
    BufferPlayersName(msgFormat, 0, app->profile);
    BufferIntegerAsString(msgFormat, 1, PlayerProfile_GetTrainerID_VisibleHalf(app->profile), 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->playerText, 3, 13, msg_0247_00068);
    app->playerText.clearWindow = FALSE;
    MainMenuTextPrinter_SetTextPos(&app->playerText, 0, GetFontAttribute(0, 1));
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->playerText, -1, -1, msg_0247_00070);

    OS_GetMacAddress(macAddress);
    applicationNo = GetApplicationNumber(macAddress);
    RangerLink_SplitDecimal(applicationNo.value, &high, &low);
    BufferIntegerAsString(msgFormat, 0, high, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferIntegerAsString(msgFormat, 1, low, 4, PRINTING_MODE_LEADING_ZEROS, TRUE);
    app->playerText.rightAlign = TRUE;
    MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->playerText, -1, -1, msg_0247_00071);
    MessageFormat_Delete(msgFormat);
}

void ConnectToRanger_FreeGraphics(OverlayManager *manager) {
    ConnectToRangerApp *app = OverlayManager_GetData(manager);
    MainMenuUtil_FreeSprites();
    if (WindowIsInUse(&app->messageWindow) == TRUE) {
        ClearWindowTilemapAndCopyToVram(&app->messageWindow);
        RemoveWindow(&app->messageWindow);
        ClearWindowTilemapAndCopyToVram(&app->statusWindow);
        RemoveWindow(&app->statusWindow);
        ClearWindowTilemapAndCopyToVram(&app->playerInfoWindow);
        RemoveWindow(&app->playerInfoWindow);
    }
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1);
    Heap_Free(app->bgConfig);
}

BOOL ConnectToRanger_Init(OverlayManager *manager, int *state) {
    ConnectToRangerApp *app = MainMenuUtil_CreateAppData(manager, HEAP_ID_83, sizeof(ConnectToRangerApp), 0x20000);
    app->heapID = HEAP_ID_83;
    app->bgConfig = BgConfig_Alloc(HEAP_ID_83);
    app->saveData = ((MainOverlayArgs *)OverlayManager_GetArgs(manager))->saveData;
    app->profile = Save_PlayerData_GetProfile(app->saveData);
    app->pokedex = Save_Pokedex_Get(app->saveData);
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    MainMenuUtil_InitWork(app->heapID);
    app->mysteryGift = Save_MysteryGift_Get(app->saveData);
    app->unk_14C = 9;
    app->nextFreeTile = 40;
    app->unk_154 = 0;
    app->connectState = RANGER_CONNECT_START;
    app->linkInitialised = FALSE;
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570);
    Sound_SetSceneAndPlayBGM(9, SEQ_GS_WIFI_PRESENT, 1);
    return TRUE;
}

BOOL ConnectToRanger_Main(OverlayManager *manager, int *state) {
    ConnectToRangerApp *app = OverlayManager_GetData(manager);

    switch (*state) {
    case CONNECT_TO_RANGER_STATE_SETUP:
        ConnectToRanger_SetupGraphics(app);
        if (!Pokedex_IsEnabled(app->pokedex)) {
            MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_IN, CONNECT_TO_RANGER_STATE_POKEDEX_REQUIRED, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        } else {
            *state = CONNECT_TO_RANGER_STATE_PRINT_SCREEN;
        }
        break;
    case CONNECT_TO_RANGER_STATE_POKEDEX_REQUIRED:
        if (!ConnectToRanger_ShowPokedexRequiredMessage(app)) {
            MainMenuUtil_SetFadeWhite(TRUE);
            MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_OUT, CONNECT_TO_RANGER_STATE_EXIT, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        }
        break;
    case CONNECT_TO_RANGER_STATE_PRINT_SCREEN:
        ConnectToRanger_PrintScreen(app);
        if (!SaveMysteryGift_FindAvailable(app->mysteryGift)) {
            app->statusText.speed = 1;
            app->textPrinterId = MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00022);
            MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_IN, CONNECT_TO_RANGER_STATE_GIFTS_FULL, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        } else {
            app->unk_14C = 7;
            MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_IN, CONNECT_TO_RANGER_STATE_TRANSFER, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        }
        break;
    case CONNECT_TO_RANGER_STATE_GIFTS_FULL:
        if (!TextPrinterCheckActive((u8)app->textPrinterId)) {
            if (gSystem.newKeys & PAD_BUTTON_A) {
                MainMenuUtil_SetFadeWhite(TRUE);
                MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_OUT, CONNECT_TO_RANGER_STATE_EXIT, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
            }
        }
        break;
    case CONNECT_TO_RANGER_STATE_TRANSFER:
        app->connectResult = ConnectToRanger_RunTransfer(app);
        if (app->connectResult == 5) {
            MainMenuUtil_SetFadeWhite(TRUE);
            MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_OUT, CONNECT_TO_RANGER_STATE_EXIT, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        } else if (app->connectResult == 4) {
            MainMenuUtil_SetFadeWhite(TRUE);
            MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_OUT, CONNECT_TO_RANGER_STATE_EXIT, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        }
        break;
    case CONNECT_TO_RANGER_STATE_SHOW_CARD:
        ConnectToRanger_LoadSubScreenBg(app);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        WonderCardAlbum_DrawCardOnBg(app->bgConfig, &app->wonderCard, HEAP_ID_83);
        MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_IN, CONNECT_TO_RANGER_STATE_WAIT_CARD_INPUT, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        app->wonderCard.info.unk8E_2 = FALSE;
        break;
    case CONNECT_TO_RANGER_STATE_WAIT_CARD_INPUT:
        if (gSystem.newKeys) {
            MainMenuUtil_StartFade(FADE_TYPE_BRIGHTNESS_OUT, CONNECT_TO_RANGER_STATE_RESET, state, CONNECT_TO_RANGER_STATE_WAIT_FADE);
        }
        break;
    case CONNECT_TO_RANGER_STATE_RESET:
        OS_ResetSystem(0);
        break;
    case CONNECT_TO_RANGER_STATE_EXIT:
        ConnectToRanger_FreeGraphics(manager);
        return TRUE;
    case CONNECT_TO_RANGER_STATE_WAIT_FADE:
        MainMenuUtil_WaitFade(state);
        break;
    }
    MainMenuUtil_UpdateSprites();
    return FALSE;
}

BOOL ConnectToRanger_Exit(OverlayManager *manager, int *state) {
    Heap_Destroy(HEAP_ID_89);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_83);
    return TRUE;
}

// WonderCardInfo with its flag bits seen as one byte.
typedef struct WonderCardInfoFlags {
    u16 name[36];
    u32 version;
    u16 id;
    u8 flags;
} WonderCardInfoFlags;

#define WONDER_CARD_FLAG_BYTE(card) (((WonderCardInfoFlags *)&(card)->info)->flags)

// Builds the Wonder Card of the Manaphy Egg received from Pokemon Ranger, stores it and shows it.
void ConnectToRanger_StoreManaphyEggCard(ConnectToRangerApp *app) {
    WonderCard *wonderCard = &app->wonderCard;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    u32 heartGold, soulSilver, diamond, pearl, platinum;
    RTCDate date;

    MI_CpuFill8(wonderCard, 0, sizeof(WonderCard));
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0247_bin, app->heapID);
    msgFormat = MessageFormat_New(app->heapID);
    wonderCard->tag = 7;
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0247_00078, app->heapID);
    CopyStringToU16Array(string, wonderCard->info.name, NELEMS(wonderCard->info.name));
    String_Delete(string);
    heartGold = MainMenuUtil_GetVersionMaskBit(8);
    soulSilver = MainMenuUtil_GetVersionMaskBit(7);
    diamond = MainMenuUtil_GetVersionMaskBit(12);
    pearl = MainMenuUtil_GetVersionMaskBit(10);
    platinum = MainMenuUtil_GetVersionMaskBit(11);
    wonderCard->info.version = soulSilver | (pearl | platinum | diamond) | heartGold;
    wonderCard->info.id = 1;
    wonderCard->info.unique = FALSE;
    wonderCard->info.unk8E_1 = FALSE;
    WONDER_CARD_FLAG_BYTE(wonderCard) = WONDER_CARD_FLAG_BYTE(wonderCard) | 4;
    WONDER_CARD_FLAG_BYTE(wonderCard) = WONDER_CARD_FLAG_BYTE(wonderCard) | 8;
    wonderCard->info.unk8E_5 = FALSE;
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0247_00077, app->heapID);
    CopyStringToU16Array(string, wonderCard->text, NELEMS(wonderCard->text));
    String_Delete(string);
    wonderCard->shareMax = 0;
    wonderCard->monIcon[0] = SPECIES_MANAPHY;
    wonderCard->monIcon[1] = 0;
    wonderCard->monIcon[2] = 0;
    wonderCard->shareCount = 0;
    GF_RTC_CopyDate(&date);
    wonderCard->receiveDate = RTC_ConvertDateToDay(&date);
    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);
    // The card's header has the layout of a MysteryGift.
    SaveMysteryGift_TryInsertGift(app->mysteryGift, (MysteryGift *)wonderCard, 3);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    MainMenuUtil_ShowCardSubScreen(app->bgConfig, wonderCard);
}

void ConnectToRanger_SetTimeout(ConnectToRangerApp *app, int frames) {
    app->timer = frames;
}

// Counts down the timeout and ends the link on timeout, link loss (if checkLink) or a press of cancelKeys.
BOOL ConnectToRanger_CheckTimeoutOrCancel(ConnectToRangerApp *app, u32 *connectState, BOOL checkLink, u32 cancelKeys) {
    app->timer--;
    if (app->timer == 0) {
        RangerLink_RequestEnd();
        *connectState = RANGER_CONNECT_TIMEOUT;
        return FALSE;
    }
    if (checkLink && !RangerLink_IsConnected()) {
        RangerLink_RequestEnd();
        *connectState = RANGER_CONNECT_LINK_LOST;
        return FALSE;
    }
    if (cancelKeys && (gSystem.newKeys & cancelKeys)) {
        RangerLink_RequestEnd();
        *connectState = RANGER_CONNECT_CANCEL;
        return TRUE;
    }
    return FALSE;
}

#pragma push
#pragma opt_lifetimes off
#pragma opt_propagation off
#pragma opt_common_subs off

int ConnectToRanger_RunTransfer(ConnectToRangerApp *app) {
    s32 stateOffset = (s32) & ((ConnectToRangerApp *)NULL)->connectState;
    s32 linkInitialisedOffset = stateOffset - 4;
#define CONNECT_STATE (*(u32 *)((u8 *)app + stateOffset))

    if (*(BOOL *)((u8 *)app + linkInitialisedOffset) == TRUE) {
        WirelessStatusIcon_SetLevel(3 - RangerLink_GetLinkLevel());
        RangerLink_Update();
        RangerLink_SetChannel(1);
    }

    switch (CONNECT_STATE) {
    case RANGER_CONNECT_START:
        ConnectToRanger_EncodePlayerData(app);
        CommWireless_StartUp();
        app->linkStarted = TRUE;
        CONNECT_STATE = RANGER_CONNECT_WAIT_LINK;
        break;
    case RANGER_CONNECT_WAIT_LINK:
        if (CommWireless_IsStartedUp()) {
            RangerLink_Init(&sConnectToRangerLinkConfig, app->linkWork);
            app->linkInitialised = TRUE;
            WirelessStatusIcon_Create();
            CONNECT_STATE = RANGER_CONNECT_SEARCH;
        }
        break;
    case RANGER_CONNECT_SEARCH:
        RangerLink_StartChild();
        ConnectToRanger_SetTimeout(app, 1600);
        app->waitingIcon = WaitingIcon_New(&app->statusWindow, 10);
        CONNECT_STATE = RANGER_CONNECT_WAIT_PARTNER;
        break;
    case RANGER_CONNECT_WAIT_PARTNER: {
        int index = RangerLink_CountPeers();
        if (index) {
            app->partnerIndex = index - 1;
            ConnectToRanger_SetTimeout(app, 1800);
            CONNECT_STATE = RANGER_CONNECT_CONNECT_PARTNER;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, FALSE, PAD_BUTTON_B);
        break;
    }
    case RANGER_CONNECT_CONNECT_PARTNER:
        if (RangerLink_CanSelectParent(app->partnerIndex)) {
            RangerLink_SelectParent(app->partnerIndex);
            CONNECT_STATE = RANGER_CONNECT_WAIT_CONNECTED;
            app->timer = 1800;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, FALSE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_WAIT_CONNECTED:
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, FALSE, PAD_BUTTON_B);
        if (RangerLink_IsConnected()) {
            CONNECT_STATE = RANGER_CONNECT_FOUND;
        }
        break;
    case RANGER_CONNECT_FOUND:
        MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00031);
        Sprite_SetDrawFlag(app->sprite, FALSE);
        WaitingIcon_Delete(app->waitingIcon);
        app->waitingIcon = NULL;
        PlaySE(SEQ_SE_DP_SELECT);
        app->timer = 1800;
        CONNECT_STATE = RANGER_CONNECT_WAIT_A;
        break;
    case RANGER_CONNECT_WAIT_A:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            CONNECT_STATE = RANGER_CONNECT_SEND_REQUEST;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_SEND_REQUEST:
        RangerLink_StartSendList();
        CONNECT_STATE = RANGER_CONNECT_WAIT_REQUEST_SENT;
        app->timer = 1800;
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_WAIT_REQUEST_SENT:
        if (RangerLink_IsSendDone()) {
            RangerLink_StartSendNames();
            CONNECT_STATE = RANGER_CONNECT_WAIT_TOUCH;
            app->timer = 3200;
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00035);
            PlaySE(SEQ_SE_DP_SELECT);
            app->waitingIcon = NULL;
            return 1;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_WAIT_TOUCH:
        if (RangerLink_IsSendDone()) {
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00073);
            Sprite_SetDrawFlag(app->sprite, TRUE);
            ConnectToRanger_PrintPlayerInfo(app);
            CONNECT_STATE = RANGER_CONNECT_SEND_PLAYER;
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_SEND_PLAYER:
        RangerLink_StartCommand(sConnectToRangerLinkCommand, 42, 1);
        CONNECT_STATE = RANGER_CONNECT_WAIT_PLAYER_SENT;
        app->timer = 3200;
        break;
    case RANGER_CONNECT_WAIT_PLAYER_SENT:
        if (RangerLink_IsCommandDone()) {
            CONNECT_STATE = RANGER_CONNECT_SAVE_DELAY;
            app->timer = 60;
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00073);
        }
        ConnectToRanger_CheckTimeoutOrCancel(app, &CONNECT_STATE, TRUE, PAD_BUTTON_B);
        break;
    case RANGER_CONNECT_SAVE_DELAY:
        if (--app->timer == 0) {
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00074);
            app->waitingIcon = WaitingIcon_New(&app->statusWindow, 10);
            ConnectToRanger_StoreManaphyEggCard(app);
            MainMenuUtil_StartSave(app->saveData);
            CONNECT_STATE = RANGER_CONNECT_SAVING;
        }
        break;
    case RANGER_CONNECT_SAVING:
        if (MainMenuUtil_RunSave() == 1) {
            CONNECT_STATE = RANGER_CONNECT_RANDOM_DELAY;
            app->timer = (s32)OS_GetTickLo() % 54 + 6;
        }
        break;
    case RANGER_CONNECT_RANDOM_DELAY:
        if (--app->timer == 0) {
            CONNECT_STATE = RANGER_CONNECT_SEND_SAVED;
        }
        break;
    case RANGER_CONNECT_SEND_SAVED:
        RangerLink_StartCommand(sConnectToRangerLinkCommand, 42, 2);
        CONNECT_STATE = RANGER_CONNECT_WAIT_ACK;
        app->timer = 1200;
        break;
    case RANGER_CONNECT_WAIT_ACK:
        if (RangerLink_IsCommandAcknowledged()) {
            MainMenuUtil_ResumeSave();
            CONNECT_STATE = RANGER_CONNECT_FINISH_SAVE;
            app->timer = 800;
        } else {
            if (--app->timer == 0) {
                RangerLink_RequestEnd();
                CONNECT_STATE = RANGER_CONNECT_TIMEOUT;
                app->timer = 120;
            }
            if (!RangerLink_IsConnected()) {
                RangerLink_RequestEnd();
                CONNECT_STATE = RANGER_CONNECT_LINK_LOST;
                app->timer = 120;
            }
        }
        break;
    case RANGER_CONNECT_FINISH_SAVE:
        if (MainMenuUtil_GetSaveState() == 3) {
            RangerLink_RequestEnd();
            CONNECT_STATE = RANGER_CONNECT_LINK_LOST;
            app->timer = 120;
        } else if (MainMenuUtil_GetSaveState() != 2) {
            MainMenuUtil_RunSave();
        } else if (RangerLink_IsCommandDone() || !RangerLink_IsConnected()) {
            CONNECT_STATE = RANGER_CONNECT_DONE_DELAY;
            app->timer = 1;
            RangerLink_RequestEnd();
            break;
        }
        if (--app->timer == 0) {
            RangerLink_RequestEnd();
            CONNECT_STATE = RANGER_CONNECT_TIMEOUT;
            app->timer = 10;
        }
        // fallthrough
    case RANGER_CONNECT_DONE_DELAY:
        if (--app->timer == 0) {
            CONNECT_STATE = RANGER_CONNECT_DONE;
            MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00075);
            Sprite_SetDrawFlag(app->sprite, FALSE);
            WaitingIcon_Delete(app->waitingIcon);
            PlaySE(SEQ_SE_DP_SELECT);
            app->waitingIcon = NULL;
        }
        break;
    case RANGER_CONNECT_DONE:
        if (RangerLink_IsFinished()) {
            if (app->linkStarted == TRUE) {
                CommWireless_Terminate();
                WirelessStatusIcon_Destroy();
                app->linkStarted = FALSE;
            }
            if (gSystem.newKeys & PAD_BUTTON_A) {
                CONNECT_STATE = RANGER_CONNECT_END;
                return 4;
            }
        }
        break;
    case RANGER_CONNECT_LINK_LOST:
        CONNECT_STATE = RANGER_CONNECT_TIMEOUT;
        // fallthrough
    case RANGER_CONNECT_TIMEOUT:
        MainMenuTextPrinter_AddWindowAndPrint(app->bgConfig, &app->statusText, 2, 19, msg_0247_00076);
        Sprite_SetDrawFlag(app->sprite, FALSE);
        if (app->waitingIcon) {
            WaitingIcon_Delete(app->waitingIcon);
        }
        app->waitingIcon = NULL;
        CONNECT_STATE = RANGER_CONNECT_ERROR;
        break;
    case RANGER_CONNECT_ERROR:
        if (RangerLink_IsFinished()) {
            if (app->linkStarted == TRUE) {
                CommWireless_Terminate();
                WirelessStatusIcon_Destroy();
                app->linkStarted = FALSE;
            }
            if (gSystem.newKeys & PAD_BUTTON_A) {
                OS_ResetSystem(0);
            }
        }
        break;
    case RANGER_CONNECT_CANCEL:
        if (RangerLink_IsFinished() && app->linkStarted == TRUE) {
            CommWireless_Terminate();
            WirelessStatusIcon_Destroy();
            app->linkStarted = FALSE;
            if (app->waitingIcon) {
                WaitingIcon_Delete(app->waitingIcon);
            }
            return 5;
        }
        break;
    }
    return 0;
}

#undef CONNECT_STATE

#pragma pop
