#ifndef POKEHEARTGOLD_WIFI_SETUP_ADDRESS_ENTRY_H
#define POKEHEARTGOLD_WIFI_SETUP_ADDRESS_ENTRY_H

#include "global.h"

#include "wifi_setup/wifi_setup_ui.h"

// Address entry screen of the access-point setup (an IPv4 address or subnet mask typed on the numeric keypad), and
// the error screen that shows a connection error code with its message.

#define WIFI_SETUP_ADDRESS_NUM_FIELDS 4
#define WIFI_SETUP_ADDRESS_FIELD_LEN  3

// result of the address entry
#define WIFI_SETUP_ADDRESS_CANCELLED 0
#define WIFI_SETUP_ADDRESS_VALID     1
#define WIFI_SETUP_ADDRESS_INVALID   2

typedef struct WifiSetupAddressEntry {
    WifiSetupCanvas *canvas; // text of the digits
    GXOamAttr *cursor;       // marks the field being typed
    u8 digits[12];           // four fields of three characters ('0'-'9', ' ' or 0)
    u8 field;                // field being typed (0-3)
    u8 result;               // WIFI_SETUP_ADDRESS_*; answer of the confirmation dialog
} WifiSetupAddressEntry;

extern WifiSetupAddressEntry *gWifiSetupAddressEntry;
extern int gWifiSetupErrorCode;

void WifiSetupAddressEntry_LoadGraphics(void);
void WifiSetupAddressEntry_FadeIn(void);
void WifiSetupAddressEntry_OpenKeypad(void);
void WifiSetupAddressEntry_DrawDigits(void);
void WifiSetupAddressEntry_UpdateCursor(void);

// Invalid address message
void WifiSetupAddressEntry_WaitInvalidMessage(void);
void WifiSetupErrorScreen_Init(void);
void WifiSetupErrorScreen_SetCode(int code);

#endif // POKEHEARTGOLD_WIFI_SETUP_ADDRESS_ENTRY_H
