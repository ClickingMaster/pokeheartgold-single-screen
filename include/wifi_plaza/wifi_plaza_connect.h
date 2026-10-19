#ifndef POKEHEARTGOLD_WIFI_PLAZA_CONNECT_H
#define POKEHEARTGOLD_WIFI_PLAZA_CONNECT_H

#include "wifi_plaza/wifi_plaza_internal.h"

#include "overlay_manager.h"
#include "save.h"

// Arguments of the connect screen (Wi-Fi Plaza app 5).
typedef struct WiFiPlazaConnectArgs {
    SaveData *saveData;
    WiFiPlazaWork *work;
    BOOL skipPrompt; // WiFiPlazaApps.skipPrompt
    s64 *loginTime;
    BOOL connected;
} WiFiPlazaConnectArgs;

// Arguments of the disconnect screen (Wi-Fi Plaza apps 9 to 12).
typedef struct WiFiPlazaDisconnectArgs {
    SaveData *saveData;
    WiFiPlazaWork *work;
    u32 mode;
} WiFiPlazaDisconnectArgs;

BOOL WiFiPlazaConnect_Init(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_Main(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_Exit(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectInit(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectMain(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectExit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_WIFI_PLAZA_CONNECT_H
