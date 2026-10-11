#ifndef POKEHEARTGOLD_WIFI_SETUP_ADDRESS_ENTRY_H
#define POKEHEARTGOLD_WIFI_SETUP_ADDRESS_ENTRY_H

#include "global.h"

#include "wifi_setup_ui.h"

// Address entry screen of the access-point setup (ARM code in overlay 13): an IPv4 address or
// subnet mask typed on the numeric keypad as four fields of three digits, and the error screen that
// shows a connection error code with its message.

#define WIFI_SETUP_ADDRESS_NUM_FIELDS 4
#define WIFI_SETUP_ADDRESS_FIELD_LEN  3

// result of the address entry
#define WIFI_SETUP_ADDRESS_CANCELLED 0
#define WIFI_SETUP_ADDRESS_VALID     1
#define WIFI_SETUP_ADDRESS_INVALID   2

typedef struct WifiSetupAddressEntry {
    WifiSetupCanvas *canvas; // 0x00, text of the digits
    GXOamAttr *cursor;       // 0x04, marks the field being typed
    u8 digits[12];           // 0x08, four fields of three characters ('0'-'9', ' ' or 0)
    u8 field;                // 0x14, field being typed (0-3)
    u8 result;               // 0x15, WIFI_SETUP_ADDRESS_*; answer of the confirmation dialog
} WifiSetupAddressEntry;

extern WifiSetupAddressEntry *gWifiSetupAddressEntry;
extern int gWifiSetupErrorCode;

// src/wifi_setup_address_entry.c
void WifiSetupAddressEntry_LoadGraphics(void);
void WifiSetupAddressEntry_FadeIn(void);
void WifiSetupAddressEntry_OpenKeypad(void);
void WifiSetupAddressEntry_WaitKeypad(void);
void WifiSetupAddressEntry_Main(void);
void WifiSetupAddressEntry_HandleInput(void);
int WifiSetupAddressEntry_IsFieldAtLeast(int limit);
void ov13_022321FC(void);
void WifiSetupAddressEntry_StartClose(void);
void WifiSetupAddressEntry_CloseKeypad(void);
void WifiSetupAddressEntry_WaitKeypadClosed(void);
void WifiSetupAddressEntry_Exit(void);
void WifiSetupAddressEntry_DrawDigits(void);
void WifiSetupAddressEntry_UpdateCursor(void);
void WifiSetupAddressEntry_WaitConfirm(void);
void WifiSetupAddressEntry_FinishConfirm(void);
void WifiSetupAddressEntry_PadFields(void);
int WifiSetupAddressEntry_IsValid(void);

// src/wifi_setup_error_screen.c
void WifiSetupAddressEntry_WaitInvalidMessage(void);
void WifiSetupAddressEntry_FinishInvalidMessage(void);
void WifiSetupErrorScreen_Init(void);
void WifiSetupErrorScreen_LoadGraphics(void);
void WifiSetupErrorScreen_DrawMessage(void);
void WifiSetupErrorScreen_FadeIn(void);
void WifiSetupErrorScreen_WaitFadeIn(void);
void WifiSetupErrorScreen_WaitButtonBar(void);
void WifiSetupErrorScreen_Main(void);
void WifiSetupErrorScreen_HandleKeys(void);
void ov13_02232CA4(void);
void WifiSetupErrorScreen_HandleButton(void);
void WifiSetupErrorScreen_StartClose(void);
void WifiSetupErrorScreen_CloseButtonBar(void);
void WifiSetupErrorScreen_Exit(void);
void WifiSetupErrorScreen_SetCode(int code);

#endif // POKEHEARTGOLD_WIFI_SETUP_ADDRESS_ENTRY_H
