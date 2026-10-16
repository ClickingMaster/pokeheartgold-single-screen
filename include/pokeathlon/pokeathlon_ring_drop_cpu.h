#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_CPU_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_CPU_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

// The computer participants of Ring Drop (PokeathlonRingDrop.cpu).
typedef struct PokeathlonRingDropCpu PokeathlonRingDropCpu;
typedef struct PokeathlonRingDropCpuPlayer PokeathlonRingDropCpuPlayer;

PokeathlonRingDropCpu *PokeathlonRingDropCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonRingDropCpu_Free(PokeathlonRingDropCpu *cpu);
void PokeathlonRingDropCpu_Update(PokeathlonRingDropCpu *cpu);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_CPU_H
