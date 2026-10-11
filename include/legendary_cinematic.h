#ifndef POKEHEARTGOLD_LEGENDARY_CINEMATIC_H
#define POKEHEARTGOLD_LEGENDARY_CINEMATIC_H

#include "global.h"

// The cinematics of overlay 106, run one frame at a time by LegendaryCinematic_Main through
// LegendaryCinematicArgs.unk4C: 0 Ho-Oh, 1 Lugia, 2 Arceus. Each takes the overlay's work data and
// returns FALSE once it has finished.
int ScriptCinematic_HoOh(void *data);
int ScriptCinematic_Lugia(void *data);
int ScriptCinematic_Arceus(void *data);

#endif // POKEHEARTGOLD_LEGENDARY_CINEMATIC_H
