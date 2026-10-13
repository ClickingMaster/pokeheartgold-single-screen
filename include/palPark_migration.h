#ifndef POKEHEARTGOLD_PALPARK_MIGRATION_H
#define POKEHEARTGOLD_PALPARK_MIGRATION_H

#include "pokemon_types_def.h"

typedef struct MigratedPokemon {
    Pokemon pokemon[PARTY_SIZE];
    u32 migrationIds[20];
    s64 migrationTimes[20];
    u32 rtcOffsetLow;
    u32 rtcOffsetHigh;
    u8 macAddress[6];
} MigratedPokemon;

u32 MigratedPokemon_GetSize(void);
void MigratedPokemon_Init(MigratedPokemon *mon);
void MigratedPokemon_InitPokemonOnly(MigratedPokemon *mon);
void MigratedPokemon_CopyBoxPokemonToSlot(MigratedPokemon *mon, BoxPokemon *src, int idx);
void MigratedPokemon_RecordMigration(MigratedPokemon *mon, u32 id);
void MigratedPokemon_ConvertToPokemon(MigratedPokemon *mon, int idx, Pokemon *dest);
int MigratedPokemon_CountPokemon(MigratedPokemon *mon);

#endif // POKEHEARTGOLD_PALPARK_MIGRATION_H
