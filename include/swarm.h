#ifndef POKEHEARTGOLD_SWARM_H
#define POKEHEARTGOLD_SWARM_H

#include "global.h"

BOOL IsSwarmEncounterFromRand(u32 rand, u16 mapno, u16 swarmValue);
void GetSwarmInfoFromRand(u32 rand, u16 *mapno, u16 *species);

#endif // POKEHEARTGOLD_SWARM_H
