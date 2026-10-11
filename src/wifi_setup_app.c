#pragma force_active on
#include "wifi_setup_app.h"

#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Access-point setup screens, application core: the setup client request, the entry point and its
// main loop, system/display start-up and shutdown, accessors of the caller's parameters, text banks
// and the session started from the screens.

// The setup key sent with the client request.
typedef struct WifiSetupRequestKey {
    u8 tag;
    u8 unused;
    u16 length;
    u8 data[256];
} WifiSetupRequestKey;

extern void ov13_0223A6E4(u8 *result);
extern void ov13_0223AAFC(void);
extern void ov13_0223AB8C(void);
extern void ov13_0222C1BC(void);
extern void ov13_0223F664(void);
extern void ov13_0223E80C(WifiSetupSession *session, WifiSetupSessionParams *params);
extern int ov13_0223EA58(void);
extern int ov13_0223E8FC(void);
extern void ov13_0223EAA0(void);
extern void ov13_0223EAD4(void *task);
extern void ov13_0223EE58(u8 *event, u8 *ok);
extern u16 *ov13_0223EE9C(void);
extern void DWC_BM_Init(void *work);
extern void GXx_SetMasterBrightness_(vu16 *reg, int brightness);

// Starts the setup client: requests settings 0 and 1 with the setup key, one connection attempt and
// one receive attempt with default timing, from this console's MAC address.
void WifiSetupApp_StartClient(void) {
    WifiSetupRequestKey key;
    u32 n;
    u16 *dst;
    const u16 *src;
    u16 a, b;
    u16 *base;
    gWifiSetupClient.request = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupRequest), 4);
    gWifiSetupClient.done = FALSE;
    MIi_CpuClear16(0, (u16 *)&key, sizeof(key));
    key.tag = 'P';
    key.length = 12;
    MI_CpuCopy8(ov13_02245320, key.data, 12);
    gWifiSetupClient.request->requestMask = 3;
    // copy the key to request->unk_002.. two halfwords at a time
    base = (u16 *)gWifiSetupClient.request;
    dst = base + 1;
    src = (const u16 *)&key;
    n = 65;
    do {
        a = src[0];
        b = src[1];
        src += 2;
        dst[0] = a;
        dst[1] = b;
        dst += 2;
    } while (--n);
    base[131] = 1; // connectRetries
    gWifiSetupClient.request->connectDelay = -1;
    gWifiSetupClient.request->receiveRetries = 1;
    gWifiSetupClient.request->receiveDelay = -1;
    gWifiSetupClient.request->receiveTimeout = -1;
    OS_GetMacAddress(gWifiSetupClient.request->unk_110);
    if (ov13_02222C1C(WifiSetupApp_ClientAlloc, WifiSetupApp_ClientFree)) {
        OS_Terminate();
    }
}

// Stops the setup client; with `apply`, the settings it received are taken over.
void WifiSetupApp_StopClient(BOOL apply) {
    ov13_02222CE0();
    if (apply && gWifiSetupClient.request->error == 0 && gWifiSetupClient.done == TRUE) {
        ov13_0223A6E4(gWifiSetupClient.request->result);
    }
    WifiSetupHeap_FreeAndClear(&ov13_0224F4C8);
}

int WifiSetupApp_PollClient(void) {
    u32 error;
    if (!ov13_02220908(gWifiSetupClient.request)) {
        gWifiSetupClient.done = TRUE;
        return WIFI_SETUP_CLIENT_DONE;
    }
    error = gWifiSetupClient.request->error;
    if (error == 1 || (u8)(error + 253) <= 2) { // 1, 3-5
        return WIFI_SETUP_CLIENT_BUSY;
    }
    return WIFI_SETUP_CLIENT_FAILED;
}

void *WifiSetupApp_ClientAlloc(u32 size) {
    return WifiSetupHeap_AllocZeroed(size, 32);
}

void WifiSetupApp_ClientFree(void *ptr) {
    WifiSetupHeap_Free(ptr);
}

// Entry point: runs the setup screens in `heap` until they exit. Returns -1 for invalid parameters.
int WifiSetupApp_Run(void *heap, s32 language, s32 flags) {
    gWifiSetupApp.heap = heap;
    if (!WifiSetupApp_SetParams(language, flags)) {
        return -1;
    }
    gWifiSetupApp.exit = FALSE;
    WifiSetupApp_InitSystem();
    WifiSetupApp_InitDisplay();
    ov13_0223AAFC();
    WifiSetupApp_SetStateFunc(ov13_0222C1BC);
    do {
        WifiSetupInput_Update();
        gWifiSetupApp.stateFunc();
        WifiSetupTask_RunQueue(0);
        WifiSetupInput_UpdateLcdPower();
        ov13_022422E0();
        OS_WaitVBlankIntr();
    } while (gWifiSetupApp.exit == FALSE);
    WifiSetupApp_Shutdown();
    return 0;
}

BOOL WifiSetupApp_SetParams(int language, u32 flags) {
    gWifiSetupApp.language = language;
    gWifiSetupApp.flags.raw = flags;
    if (language < 0 || language > 6) {
        return FALSE;
    }
    if (language == 1 && ((flags >> 4) & ~2) != 0) {
        return FALSE;
    }
    if (gWifiSetupApp.flags.bits.kind > 1) {
        return FALSE;
    }
    if (language != 0 && (gWifiSetupApp.flags.bits.options & 1)) {
        return FALSE;
    }
    if (language == 0 && !(gWifiSetupApp.flags.bits.options & 1)) {
        return FALSE;
    }
    return TRUE;
}

void WifiSetupApp_InitSystem(void) {
    void *work;
    (void)OS_DisableIrq();
    GX_DispOff();
    GXS_DispOff();
    if (!OS_IsTickAvailable()) {
        OS_Terminate();
    }
    if (!OS_IsAlarmAvailable()) {
        OS_Terminate();
    }
    GX_VBlankIntr(FALSE);
    FX_Init();
    FS_Init((u32)-1);
    TP_Init();
    RTC_Init();
    GX_DispOff();
    GXS_DispOff();
    WifiSetup_SaveVramBanks();
    WifiSetupHeap_Init(gWifiSetupApp.heap);
    WifiSetup_InstallVBlankIntr();
    WifiSetupTask_InitScheduler();
    ov13_0223F664();
    WifiSetupInput_Init();
    WifiSetupFade_Init();
    work = WifiSetupHeap_AllocZeroed(0x700, 32);
    DWC_BM_Init(work);
    WifiSetupHeap_FreeAndClear(&work);
}

// Clears the window and OBJ-window enables of a DISPCNT register and the master brightness next to it.
static inline void ResetWindows(vu32 *dispcnt) {
    u32 value = *dispcnt;
    value &= ~0x1f00;
    *dispcnt = value;
    value = *dispcnt;
    value &= ~0xe000;
    *dispcnt = value;
    GXx_SetMasterBrightness_((vu16 *)((u8 *)dispcnt + 0x6c), 0);
}

// Main screen: BG mode 0, BG VRAM bank A, OBJ bank B (1D mapping); sub screen: banks H and I.
void WifiSetupApp_InitDisplay(void) {
    GX_VBlankIntr(FALSE);
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_B);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    reg_GX_DISPCNT &= ~0x1f00;
    reg_GX_DISPCNT &= ~0xe000;
    GXx_SetMasterBrightness_(&reg_GX_MASTER_BRIGHT, 0);
    reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xffcfffef) | 0x10 | 0x200000;
    reg_G2_BG0CNT &= ~64;
    reg_G2_BG1CNT &= ~64;
    reg_G2_BG2CNT &= ~64;
    reg_G2_BG3CNT &= ~64;
    reg_G2_BG0OFS = 0;
    reg_G2_BG1OFS = 0;
    reg_G2_BG2OFS = 0;
    reg_G2_BG3OFS = 0;
    G2x_SetBlendBrightness_(&reg_G2_BLDCNT, 63, 16);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    ResetWindows(&reg_GXS_DB_DISPCNT);
    reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & 0xffcfffef) | 0x10;
    reg_G2S_DB_BG0CNT &= ~64;
    reg_G2S_DB_BG1CNT &= ~64;
    reg_G2S_DB_BG2CNT &= ~64;
    reg_G2S_DB_BG3CNT &= ~64;
    reg_G2S_DB_BG0OFS = 0;
    reg_G2S_DB_BG1OFS = 0;
    reg_G2S_DB_BG2OFS = 0;
    reg_G2S_DB_BG3OFS = 0;
    G2x_SetBlendBrightness_(&reg_G2S_DB_BLDCNT, 63, 16);
    reg_G2_BG0CNT = (reg_G2_BG0CNT & 0x43) | 0xc00;
    reg_G2_BG1CNT = (reg_G2_BG1CNT & 0x43) | 0x108 | 0xc00;
    reg_G2_BG2CNT = (reg_G2_BG2CNT & 0x43) | 0xe10;
    reg_G2_BG3CNT = (reg_G2_BG3CNT & 0x43) | 0xf10;
    reg_G2S_DB_BG0CNT = (reg_G2S_DB_BG0CNT & 0x43) | 0xc00;
    reg_G2S_DB_BG1CNT = (reg_G2S_DB_BG1CNT & 0x43) | 0xd00;
    reg_G2S_DB_BG2CNT = (reg_G2S_DB_BG2CNT & 0x43) | 0xe00;
    reg_G2S_DB_BG3CNT = (reg_G2S_DB_BG3CNT & 0x43) | 0xf00;
    reg_GX_DISPCNT &= ~0x38000000;
    reg_GX_DISPCNT &= ~0x7000000;
    reg_GX_POWCNT &= ~0x8000;
    WifiSetupText_Init();
    WifiSetupOam_Init();
    WifiSetupCharVram_Init();
    WifiSetupOamBlock_InitManager();
    GX_DispOn();
    GXS_DispOn();
    GX_VBlankIntr(TRUE);
}

void WifiSetupApp_Shutdown(void) {
    GX_DispOff();
    GXS_DispOff();
    ov13_0224230C();
    WifiSetupInput_Free();
    WifiSetup_RestoreVBlankIntr();
    ov13_0223AB8C();
    WifiSetupOamBlock_FreeManager();
    WifiSetupOam_Free();
    WifiSetupText_Free();
    WifiSetupFade_Free();
    WifiSetupArchive_Unload();
    WifiSetupTask_FreeScheduler();
    WifiSetupHeap_Destroy();
    WifiSetup_RestoreVramBanks();
}

void WifiSetupApp_SetStateFunc(WifiSetupAppStateFunc func) {
    gWifiSetupApp.stateFunc = func;
}

void ov13_0222B8F8(int a, int b) {
    gWifiSetupApp.unk_10 = a;
    gWifiSetupApp.unk_14 = b;
}

void ov13_0222B90C(int *a, int *b) {
    if (a != NULL) {
        *a = gWifiSetupApp.unk_10;
    }
    if (b != NULL) {
        *b = gWifiSetupApp.unk_14;
    }
}

void ov13_0222B934(int a, int b) {
    gWifiSetupApp.unk_18 = a;
    gWifiSetupApp.unk_1C = b;
}

void ov13_0222B948(int *a, int *b) {
    if (a != NULL) {
        *a = gWifiSetupApp.unk_18;
    }
    if (b != NULL) {
        *b = gWifiSetupApp.unk_1C;
    }
}

u8 WifiSetupApp_GetLanguage(void) {
    return gWifiSetupApp.language;
}

u32 WifiSetupApp_GetKind(void) {
    return gWifiSetupApp.flags.bits.kind;
}

BOOL WifiSetupApp_HasOption(u32 mask) {
    return (mask & gWifiSetupApp.flags.bits.options) ? TRUE : FALSE;
}

void WifiSetupApp_Exit(void) {
    gWifiSetupApp.exit = TRUE;
}

// Character naming the language's files: '1' + language, with two exceptions.
u8 WifiSetupApp_GetLanguageCode(void) {
    u8 language = gWifiSetupApp.language;
    if (language == 6) {
        return '8';
    }
    if (language == 1 && !(gWifiSetupApp.flags.bits.options & 2)) {
        return '7';
    }
    return language + '1';
}

void WifiSetupText_InitBanks(void) {
    gWifiSetupTextBanks = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupTextBanks), 4);
    gWifiSetupTextBanks->pool = WifiSetupPool_NewWithItems(8, gWifiSetupTextBanks, sizeof(WifiSetupTextBank));
}

void WifiSetupText_FreeBanks(void) {
    WifiSetupHeap_FreeAndClear(&gWifiSetupTextBanks);
}

WifiSetupTextBank *WifiSetupText_LoadBank(void *path) {
    u32 size;
    WifiSetupTextBank *bank = WifiSetupPool_Pop(gWifiSetupTextBanks->pool);
    u8 *file = WifiSetupFile_Load(path, &size, 4);
    u8 *block;
    bank->file = file;
    block = file + 0x20;
    bank->offsets = (u32 *)(block + 0x10);
    bank->strings = block + *(u32 *)(block + 4) + 8;
    return bank;
}

void WifiSetupText_UnloadBank(WifiSetupTextBank *bank) {
    WifiSetupFile_Free(bank->file);
    WifiSetupPool_Push(gWifiSetupTextBanks->pool, bank);
}

u16 *WifiSetupText_GetString(WifiSetupTextBank *bank, int index) {
    return (u16 *)(bank->strings + bank->offsets[(u16)index]);
}

// Returns string `index` with character `pos` (if >= 0) replaced by the digit `digit`.
u16 *WifiSetupText_GetStringWithDigit(WifiSetupTextBank *bank, u32 index, int pos, u32 digit) {
    u16 *str = (u16 *)(bank->strings + bank->offsets[(u16)index]);
    if (pos >= 0) {
        digit += '0';
        str[pos] = (u16)digit;
    }
    return str;
}

void ov13_0222BB20(WifiSetupSessionCallback callback) {
    WifiSetupSessionParams params;
    gWifiSetupSession = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupSession), 32);
    gWifiSetupSession->callback = callback;
    gWifiSetupSession->unk_1E2A0 = 0;
    gWifiSetupSession->unk_1E2A1 = 0;
    params = ov13_02245F8C;
    params.unk_04 = WifiSetupText_GetString(ov13_0224F4F8, 0);
    params.unk_08 = WifiSetupText_GetString(ov13_0224F4F8, 1);
    params.languageCode = WifiSetupApp_GetLanguageCode();
    ov13_0223E80C(gWifiSetupSession, &params);
    if (ov13_0223EA58() == 0) {
        OS_Terminate();
    }
    gWifiSetupSession->task = WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_0222BCC0, NULL, 120);
}

void ov13_0222BC0C(void) {
    WifiSetupTask_Create(0, (WifiSetupTaskFunc)ov13_0222BC2C, NULL, 120);
}

void ov13_0222BC2C(void *task) {
    if (ov13_0223E8FC() == 0) {
        return;
    }
    gWifiSetupSession->unk_1E2A1 = 1;
    WifiSetupTask_DestroyLater(0, task);
}

BOOL ov13_0222BC68(void) {
    return gWifiSetupSession == NULL;
}

void ov13_0222BC84(WifiSetupSessionCallback callback) {
    gWifiSetupSession->callback = callback;
}

void ov13_0222BC9C(void) {
    ov13_0223EAA0();
}

u8 *ov13_0222BCA8(void) {
    return gWifiSetupSession->unk_1E280;
}

WifiSetupSession *gWifiSetupSession;

void ov13_0222BCC0(void *task) {
    u8 event;
    u8 ok;

    ov13_0223EAD4(task);
    if (gWifiSetupSession->unk_1E2A0 && !gWifiSetupSession->unk_1E2A1) {
        if (gWifiSetupSession->callback != NULL) {
            gWifiSetupSession->callback(0);
        }
        return;
    }
    ov13_0223EE58(&event, &ok);
    switch (event) {
    case 5:
        if (!ok) {
            return;
        }
        {
            u8 *dst = gWifiSetupSession->unk_1E280;
            u16 *src = ov13_0223EE9C();
            MIi_CpuCopy16(src, (u16 *)dst, 0x16);
        }
        if (gWifiSetupSession->callback == NULL) {
            gWifiSetupSession->unk_1E2A0 = 1;
            return;
        }
        gWifiSetupSession->callback(0);
        return;
    case 13:
        if (!ok) {
            return;
        }
        if (gWifiSetupSession->callback == NULL) {
            gWifiSetupSession->unk_1E2A0 = 1;
            return;
        }
        gWifiSetupSession->callback(1);
        return;
    case 20:
    case 23:
        if (!ok) {
            return;
        }
        if (gWifiSetupSession->callback == NULL) {
            gWifiSetupSession->unk_1E2A0 = 1;
            return;
        }
        gWifiSetupSession->callback(3);
        return;
    case 26:
    case 29:
        if (!ok) {
            return;
        }
        if (gWifiSetupSession->callback == NULL) {
            gWifiSetupSession->unk_1E2A0 = 1;
            return;
        }
        gWifiSetupSession->callback(4);
        return;
    case 12:
        if (!ok) {
            return;
        }
        if (gWifiSetupSession->callback == NULL) {
            gWifiSetupSession->unk_1E2A0 = 1;
            return;
        }
        gWifiSetupSession->callback(2);
        return;
    case 1:
    case 2:
    case 8:
    case 9:
        break;
    case 34:
        WifiSetupTask_DestroyLater(0, task);
        WifiSetupHeap_FreeAndClear(&gWifiSetupSession);
        break;
    }
}
