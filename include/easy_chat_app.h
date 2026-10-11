#ifndef POKEHEARTGOLD_EASY_CHAT_APP_H
#define POKEHEARTGOLD_EASY_CHAT_APP_H

#include "overlay_manager.h"

BOOL EasyChat_Init(OverlayManager *man, int *state);
BOOL EasyChat_Main(OverlayManager *man, int *state);
BOOL EasyChat_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_EASY_CHAT_APP_H
