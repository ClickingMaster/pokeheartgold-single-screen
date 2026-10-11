#ifndef POKEHEARTGOLD_MOVE_RELEARNER_APP_H
#define POKEHEARTGOLD_MOVE_RELEARNER_APP_H

#include "overlay_manager.h"

BOOL MoveRelearner_Init(OverlayManager *man, int *state);
BOOL MoveRelearner_Main(OverlayManager *man, int *state);
BOOL MoveRelearner_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_MOVE_RELEARNER_APP_H
