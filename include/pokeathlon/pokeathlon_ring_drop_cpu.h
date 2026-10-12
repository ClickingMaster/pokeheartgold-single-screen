#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_CPU_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_CPU_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

// The computer participants of Ring Drop (pokeathlon_ring_drop_cpu.c; PokeathlonRingDrop.cpu).
typedef struct PokeathlonRingDropCpu PokeathlonRingDropCpu;
typedef struct PokeathlonRingDropCpuPlayer PokeathlonRingDropCpuPlayer;

// pokeathlon_ring_drop_cpu.c
PokeathlonRingDropCpu *PokeathlonRingDropCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonRingDropCpu_Free(PokeathlonRingDropCpu *cpu);
void PokeathlonRingDropCpu_Update(PokeathlonRingDropCpu *cpu);
void PokeathlonRingDropCpu_InitPlayer(PokeathlonRingDropCpuPlayer *player, u8 participant, PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_IsNear(const VecFx32 *a, const VecFx32 *b, fx32 distance);
BOOL PokeathlonRingDrop_IsInDirection(const VecFx32 *a, const VecFx32 *b, int direction);
void PokeathlonRingDropCpu_Decide(PokeathlonRingDropCpuPlayer *player);
void PokeathlonRingDropCpu_GoSwitch(PokeathlonRingDropCpuPlayer *player);
BOOL PokeathlonRingDropCpu_ShouldSwitch(PokeathlonRingDropCpuPlayer *player);
void PokeathlonRingDropCpu_UpdatePlayer(PokeathlonRingDropCpuPlayer *player);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_CPU_H
