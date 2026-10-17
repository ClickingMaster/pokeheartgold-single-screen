#ifndef POKEHEARTGOLD_WIFI_PLAZA_CONNECT_H
#define POKEHEARTGOLD_WIFI_PLAZA_CONNECT_H

#include "overlay_manager.h"

BOOL WiFiPlazaConnect_Init(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_Main(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_Exit(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectInit(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectMain(OverlayManager *man, int *state);
BOOL WiFiPlazaConnect_DisconnectExit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_WIFI_PLAZA_CONNECT_H
