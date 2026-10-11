#ifndef POKEHEARTGOLD_TRADE_SEQUENCE_H
#define POKEHEARTGOLD_TRADE_SEQUENCE_H

#include "overlay_manager.h"

BOOL TradeSequence_Init(OverlayManager *man, int *state);
BOOL TradeSequence_Main(OverlayManager *man, int *state);
BOOL TradeSequence_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_TRADE_SEQUENCE_H
