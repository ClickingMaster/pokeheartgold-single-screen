#ifndef POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_H
#define POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_H

#include "global.h"

#include "overlay_manager.h"

BOOL BattleFactoryApp_Init(OverlayManager *man, int *state);
BOOL BattleFactoryApp_Main(OverlayManager *man, int *state);
BOOL BattleFactoryApp_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_FRONTIER_BATTLE_FACTORY_APP_H
