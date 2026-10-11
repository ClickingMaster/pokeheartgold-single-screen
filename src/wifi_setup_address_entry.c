#pragma force_active on
#include "wifi_setup_address_entry.h"

#include "global.h"

#include "wifi_setup_app.h"
#include "wifi_setup_keypad.h"
#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Address entry screen (0x02231C8C-0x02232770). gWifiSetupAddressEntry is allocated and filled with
// the stored address by ov13_02231B18 (src/op90_to61_overlay_13_arm_2_residual_5_residual_7_r01.c),
// which then runs WifiSetupAddressEntry_FadeIn. The digits are typed on the numeric keypad; OK
// checks the address and asks for confirmation before it is stored.

typedef void (*WifiSetupAddressFunc)(u8 *digits);

// store functions per address kind, indexed by the first value of WifiSetupApp_GetScreenArgs
typedef struct WifiSetupAddressFuncs {
    WifiSetupAddressFunc funcs[5];
} WifiSetupAddressFuncs;

// largest value of a field ("255")
typedef struct WifiSetupFieldLimit {
    u8 digits[4];
} WifiSetupFieldLimit;

// gWifiSetupTextEntry (src/wifi_setup_screens.c) directly precedes gWifiSetupAddressEntry; the
// screen frees its work through it.
extern void *gWifiSetupTextEntry[];

// Screen layout (defined in src/op90_to61_overlay_13_arm_2_residual_5_residual_7_r01.c): starts
// with the field limit; the u16 at index 7 and 8 are the position of the digits.
extern const WifiSetupFieldLimit ov13_02245480;
extern WifiSetupAddressFuncs ov13_022454AC;
extern const u8 ov13_022454C0[]; // x of every digit
extern u8 ov13_022463E4[];
extern u8 ov13_022463F8[];
extern u8 ov13_02246410[];
extern u8 ov13_02246428[];
extern char ov13_02246440[];

extern void WifiSetup_ParseAddress(const char *text, u8 *address);
extern int DWC_BACKUPlCheckAddress(void *address);
extern void WifiSetupDialog_Open(int, int, int, int, int);
extern int WifiSetupDialog_GetSelected(void);
extern void WifiSetupDialog_Close(void);
extern int WifiSetupDialog_IsOpen(void);
extern void WifiSetupSettings_Init(void);
extern long atol(const char *);

void WifiSetupAddressEntry_LoadGraphics(void) {
    WifiSetup_LoadGraphics(ov13_022463E4, GX_LoadOBJPltt);
    WifiSetup_LoadGraphics(ov13_022463F8, GX_LoadBG2Char);
    WifiSetup_LoadGraphics(ov13_02246410, GX_LoadBGPltt);
    WifiSetup_LoadGraphics(ov13_02246428, GX_LoadBG2Scr);
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & ~3) | 3;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & ~3) | 3;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 2;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 3;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 3;
}

void WifiSetupAddressEntry_FadeIn(void) {
    WifiSetupFade_Start(2, 1, 1, 8);
    WifiSetupFade_Start(2, 0, 21, 8);
    WifiSetupDisplay_ShowPlanes(1, 1);
    WifiSetupDisplay_ShowPlanes(0, 21);
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_OpenKeypad);
}

void WifiSetupAddressEntry_OpenKeypad(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupKeypad_Create();
    WifiSetupSound_Play(20);
    if (gWifiSetupAddressEntry->field == 0) {
        WifiSetupKeypad_SetCanDelete(0);
        WifiSetupKeypad_SetCanAdvance(0);
    } else {
        if (WifiSetupAddressEntry_IsFieldAtLeast(26)) {
            WifiSetupKeypad_SetCanType(0);
        }
        WifiSetupKeypad_SetCanAdvance(0);
    }
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_WaitKeypad);
}

void WifiSetupAddressEntry_WaitKeypad(void) {
    if (WifiSetupKeypad_GetInput() == 31) {
        return;
    }
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_Main);
}

void WifiSetupAddressEntry_Main(void) {
    WifiSetupAddressEntry_HandleInput();
    ov13_022321FC();
}

void WifiSetupAddressEntry_HandleInput(void) {
    int key = WifiSetupKeypad_GetInput();
    switch (key) {
    case WIFI_SETUP_KEYPAD_INPUT_NONE:
        return;
    case WIFI_SETUP_KEYPAD_INPUT_DELETE:
        if (gWifiSetupAddressEntry->field != 0 || gWifiSetupAddressEntry->digits[2] != 0) {
            WifiSetupSound_Play(3);
            if (gWifiSetupAddressEntry->digits[gWifiSetupAddressEntry->field * 3 + 2] == 0) {
                gWifiSetupAddressEntry->field--;
            }
            MI_CpuFill8(gWifiSetupAddressEntry->digits + gWifiSetupAddressEntry->field * 3, 0, 3);
            if (gWifiSetupAddressEntry->field == 0 && gWifiSetupAddressEntry->digits[2] == 0) {
                WifiSetupKeypad_SetCanDelete(0);
            }
            WifiSetupKeypad_SetCanType(1);
            WifiSetupKeypad_SetCanAdvance(0);
        }
        break;
    case WIFI_SETUP_KEYPAD_INPUT_NEXT:
        if (gWifiSetupAddressEntry->field < 3 && gWifiSetupAddressEntry->digits[gWifiSetupAddressEntry->field * 3 + 2] != 0) {
            WifiSetupSound_Play(1);
            gWifiSetupAddressEntry->field++;
            WifiSetupKeypad_SetCanAdvance(0);
        }
        break;
    case WIFI_SETUP_KEYPAD_INPUT_BACK:
        gWifiSetupAddressEntry->result = WIFI_SETUP_ADDRESS_CANCELLED;
        WifiSetupSound_Play(7);
        WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_StartClose);
        return;
    case WIFI_SETUP_KEYPAD_INPUT_OK:
        if (WifiSetupAddressEntry_IsValid()) {
            WifiSetupSound_Play(6);
            gWifiSetupAddressEntry->result = WIFI_SETUP_ADDRESS_VALID;
        } else {
            gWifiSetupAddressEntry->result = WIFI_SETUP_ADDRESS_INVALID;
            WifiSetupSound_Play(9);
        }
        gWifiSetupAddressEntry->field = 3;
        gWifiSetupAddressEntry->cursor->attr01 = (gWifiSetupAddressEntry->cursor->attr01 & 0xc1fffcff) | 0x200; // hide
        WifiSetupAddressEntry_UpdateCursor();
        WifiSetupAddressEntry_PadFields();
        WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_StartClose);
        return;
    default: // digit
        if (gWifiSetupAddressEntry->field == 3 && WifiSetupAddressEntry_IsFieldAtLeast(26)) {
            break;
        }
        WifiSetupSound_Play(1);
        {
            u8 *entry = gWifiSetupAddressEntry->digits;
            int index = gWifiSetupAddressEntry->field * 3;
            // the digits of a field shift in from the right
            if (entry[index + 2] == 0) {
                entry[index + 2] = key;
            } else if (entry[index + 1] == 0) {
                entry[index + 1] = entry[index + 2];
                entry[index + 2] = key;
                if (WifiSetupAddressEntry_IsFieldAtLeast(26) && gWifiSetupAddressEntry->field < 3) {
                    gWifiSetupAddressEntry->field++;
                }
            } else {
                entry[index] = entry[index + 1];
                entry[index + 1] = entry[index + 2];
                entry[index + 2] = key;
                if (gWifiSetupAddressEntry->field < 3) {
                    gWifiSetupAddressEntry->field++;
                }
            }
        }
        WifiSetupKeypad_SetCanDelete(1);
        if (gWifiSetupAddressEntry->field < 3) {
            WifiSetupKeypad_SetCanAdvance(1);
        } else {
            WifiSetupKeypad_SetCanAdvance(0);
        }
        if (gWifiSetupAddressEntry->field == 3 && WifiSetupAddressEntry_IsFieldAtLeast(26)) {
            WifiSetupKeypad_SetCanType(0);
        }
        break;
    }
    WifiSetupAddressEntry_DrawDigits();
    WifiSetupAddressEntry_UpdateCursor();
}

// Whether the current field is full (three digits) or its value is at least limit, so that no
// further digit can be typed into it.
int WifiSetupAddressEntry_IsFieldAtLeast(int limit) {
    u8 buf[4], *p;
    int i;
    u8 *text = gWifiSetupAddressEntry->digits + gWifiSetupAddressEntry->field * 3;
    if (text[0] != 0 && text[0] != ' ') {
        return 1;
    }
    MI_CpuCopy8(text, buf, 3);
    buf[3] = 0;
    p = buf;
    for (i = 0; i < 3; i++) {
        if (*p != 0) {
            break;
        }
        *p++ = ' ';
    }
    return atol((char *)buf) >= limit;
}

void ov13_022321FC(void) {
}

void WifiSetupAddressEntry_StartClose(void) {
    WifiSetupFade_StartDelay(8);
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_CloseKeypad);
}

void WifiSetupAddressEntry_CloseKeypad(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    WifiSetupKeypad_Close();
    WifiSetupSound_Play(21);
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_WaitKeypadClosed);
}

void WifiSetupAddressEntry_WaitKeypadClosed(void) {
    if (WifiSetupKeypad_Exists()) {
        return;
    }
    if (gWifiSetupAddressEntry->result == WIFI_SETUP_ADDRESS_CANCELLED) {
        WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_Exit);
        return;
    }
    if (gWifiSetupAddressEntry->result == WIFI_SETUP_ADDRESS_INVALID) {
        WifiSetupDialog_Open(6, 3, 1, -1, 0);
        WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_WaitInvalidMessage);
        return;
    }
    WifiSetupDialog_Open(71, 2, 1, -1, 0);
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_WaitConfirm);
}

void WifiSetupAddressEntry_Exit(void) {
    int kind;
    WifiSetupMessage_Hide();
    WifiSetupOam_Release(gWifiSetupAddressEntry->cursor);
    WifiSetupText_FreeBgCanvas(0);
    WifiSetup_LoadGraphics(ov13_02246440, GX_LoadOBJPltt);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 21);
    WifiSetupApp_GetScreenArgs(&kind, 0);
    if (kind >= 3) {
        kind++;
    }
    WifiSetupApp_SetTransition(2, 1);
    WifiSetupApp_SetScreenArgs(0, kind + 3);
    WifiSetupApp_SetStateFunc(WifiSetupSettings_Init);
    WifiSetupHeap_FreeAndClear(&gWifiSetupTextEntry[1]); // &gWifiSetupAddressEntry
}

static inline void DrawDigit(WifiSetupCanvas *canvas, u16 x, volatile u16 *rect, u16 *text) {
    WifiSetupText_DrawInRect(canvas, x, rect[1], rect[2], rect[3], 2, 0x480, text);
}

void WifiSetupAddressEntry_DrawDigits(void) {
    WifiSetupAddressEntry *entry = gWifiSetupAddressEntry;
    u16 rect[4];
    u16 text[2];
    int i;
    rect[0] = ((const u16 *)&ov13_02245480)[7];
    rect[1] = ((const u16 *)&ov13_02245480)[8];
    rect[2] = 11;
    rect[3] = 16;
    WifiSetupText_Clear(entry->canvas, 0);
    text[1] = 0;
    for (i = 0; i < 12; i++) {
        u16 x = ov13_022454C0[i];
        text[0] = gWifiSetupAddressEntry->digits[i];
        rect[0] = x;
        DrawDigit(gWifiSetupAddressEntry->canvas, x, rect, text);
    }
    WifiSetupText_MarkDirty(gWifiSetupAddressEntry->canvas);
}

static inline void SetOamPos(u32 *attr01, int x, int y) {
    *attr01 = (*attr01 & 0xFE00FF00) | ((u32)(y & 0xff)) | ((u32)(x & 0x1ff) << 16);
}

void WifiSetupAddressEntry_UpdateCursor(void) {
    int field = gWifiSetupAddressEntry->field;
    if (field > 3) {
        field = 3;
    }
    SetOamPos(&gWifiSetupAddressEntry->cursor->attr01, ov13_022454C0[field * 3 + 2], 0x28);
}

void WifiSetupAddressEntry_WaitConfirm(void) {
    gWifiSetupAddressEntry->result = WifiSetupDialog_GetSelected();
    switch (gWifiSetupAddressEntry->result) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        WifiSetupSound_Play(14);
        break;
    default:
        return;
    }
    WifiSetupDialog_Close();
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_FinishConfirm);
}

void WifiSetupAddressEntry_FinishConfirm(void) {
    WifiSetupAddressFunc store[5];
    int kind;
    *(WifiSetupAddressFuncs *)store = ov13_022454AC;
    if (WifiSetupDialog_IsOpen()) {
        return;
    }
    if (!gWifiSetupAddressEntry->result) {
        gWifiSetupAddressEntry->cursor->attr01 &= 0xc1fffcff; // show again
        WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_OpenKeypad);
        return;
    }
    WifiSetupApp_GetScreenArgs(&kind, 0);
    store[kind](gWifiSetupAddressEntry->digits);
    WifiSetupApp_SetStateFunc(WifiSetupAddressEntry_Exit);
}

// Pads every field to three characters: empty or zero digits before the last one become spaces,
// an empty last digit becomes '0'.
void WifiSetupAddressEntry_PadFields(void) {
    u8 *p;
    int i = 0;
    int j;
    int offset = i;
    int space = ' ', digit = '0', zero = i;
    do {
        p = (u8 *)gWifiSetupAddressEntry + 8 + offset; // digits
        j = zero;
        do {
            u8 c = p[j];
            if (c != digit && c != space && c != zero) {
                break;
            }
            p[j] = j == 2 ? digit : space;
            j++;
        } while (j < 3);
        i++;
        offset += 3;
    } while (i < 4);
    WifiSetupAddressEntry_DrawDigits();
}

// Every field must be at most 255. A subnet mask (kind 1) must be contiguous ones followed by
// zeros; other addresses are checked by DWC_BACKUPlCheckAddress.
int WifiSetupAddressEntry_IsValid(void) {
    WifiSetupFieldLimit limit = ov13_02245480;
    int i;
    int kind;
    u8 address[4];
    u8 *p;
    int j;
    int zero;
    for (i = 0; i < 4; i++) {
        p = &gWifiSetupAddressEntry->digits[i * 3];
        if (p[0] == ' ') {
            continue;
        }
        for (j = 0; j < 3; j++) {
            if (p[j] > limit.digits[j]) {
                return 0;
            }
            if (p[j] < limit.digits[j]) {
                break;
            }
        }
    }
    WifiSetup_ParseAddress((const char *)gWifiSetupAddressEntry->digits, address);
    WifiSetupApp_GetScreenArgs(&kind, 0);
    if (kind == 1) {
        zero = 0;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 8; j++) {
                if (zero) {
                    if (address[i] & (1 << (7 - j))) {
                        return 0;
                    }
                } else if (!(address[i] & (1 << (7 - j)))) {
                    zero = 1;
                }
            }
        }
        return 1;
    }
    return DWC_BACKUPlCheckAddress(address) != 0;
}
