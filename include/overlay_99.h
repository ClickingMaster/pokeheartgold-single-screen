#ifndef POKEHEARTGOLD_OVY_99_H
#define POKEHEARTGOLD_OVY_99_H

#include "pokeathlon/pokeathlon_save.h"

#include "overlay_manager.h"
#include "player_data.h"
#include "pokedex.h"

typedef struct PokeathlonMedalsArgs {
    Pokedex *pokedex;
    void *unk4;
    BOOL natDexEnabled;
    BOOL unkC;
} PokeathlonMedalsArgs;

typedef struct PokeathlonEventRecordArgs {
    Pokeathlon_RecordsSolo *recordsSolo;
    Pokeathlon_RecordsLink *recordsLink;
    void *unk8;
    PlayerProfile *profile;
    BOOL unk10;
} PokeathlonEventRecordArgs;

typedef struct PokeathlonCourseRecordArgs {
    PokeathlonSave *pokeathlon;
    BOOL unk4;
} PokeathlonCourseRecordArgs;

typedef struct PokeathlonTrophiesArgs {
    PokeathlonSave *pokeathlon;
    PlayerProfile *profile;
} PokeathlonTrophiesArgs;

BOOL PokeathlonMedals_Init(OverlayManager *man, int *state);
BOOL PokeathlonMedals_Main(OverlayManager *man, int *state);
BOOL PokeathlonMedals_Exit(OverlayManager *man, int *state);

BOOL PokeathlonEventRecord_Init(OverlayManager *man, int *state);
BOOL PokeathlonEventRecord_Main(OverlayManager *man, int *state);
BOOL PokeathlonEventRecord_Exit(OverlayManager *man, int *state);

BOOL PokeathlonCourseRecord_Init(OverlayManager *man, int *state);
BOOL PokeathlonCourseRecord_Main(OverlayManager *man, int *state);
BOOL PokeathlonCourseRecord_Exit(OverlayManager *man, int *state);

BOOL PokeathlonTrophies_Init(OverlayManager *man, int *state);
BOOL PokeathlonTrophies_Main(OverlayManager *man, int *state);
BOOL PokeathlonTrophies_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_OVY_99_H
