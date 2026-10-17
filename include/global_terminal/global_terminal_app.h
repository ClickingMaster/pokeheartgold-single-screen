#ifndef POKEHEARTGOLD_GLOBAL_TERMINAL_APP_H
#define POKEHEARTGOLD_GLOBAL_TERMINAL_APP_H

#include "field_system.h"
#include "heap.h"
#include "overlay_manager.h"
#include "save.h"

BOOL GlobalTerminalApp_Init(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Main(OverlayManager *man, int *state);
BOOL GlobalTerminalApp_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_GLOBAL_TERMINAL_APP_H
