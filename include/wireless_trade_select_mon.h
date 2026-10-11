#ifndef POKEHEARTGOLD_WIRELESS_TRADE_SELECT_MON_H
#define POKEHEARTGOLD_WIRELESS_TRADE_SELECT_MON_H

#include "overlay_manager.h"

// Wireless trade: both players pick the Pokemon to trade (args: WirelessTradeSelectMonArgs).
BOOL WirelessTradeSelectMon_Init(OverlayManager *man, int *state);
BOOL WirelessTradeSelectMon_Main(OverlayManager *man, int *state);
BOOL WirelessTradeSelectMon_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_WIRELESS_TRADE_SELECT_MON_H
