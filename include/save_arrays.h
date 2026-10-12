#ifndef POKEHEARTGOLD_SAVE_ARRAYS_H
#define POKEHEARTGOLD_SAVE_ARRAYS_H

#include "hall_of_fame.h"
#include "mystery_gift.h"
#include "palPark_migration.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "unk_0202E41C.h"
#include "frontier_save_access.h"

typedef u32 (*SAVESIZEFN)(void);
typedef void (*SAVEINITFN)(void *);

struct SaveChunkHeader {
    int id;
    u32 block;
    SAVESIZEFN sizeFunc;
    SAVEINITFN initFunc;
};

struct ExtraSaveChunkHeader {
    int id;
    u32 sector;
    SAVESIZEFN sizeFunc;
    SAVEINITFN initFunc;
};

extern const struct SaveChunkHeader gSaveChunkHeaders[];
extern const int gNumSaveChunkHeaders;

extern const struct ExtraSaveChunkHeader gExtraSaveChunkHeaders[];
extern const int gNumExtraSaveChunkHeaders;

UnkStruct_0202E474 *sub_020270C4(SaveData *saveData);
PCStorage *SaveArray_PCStorage_Get(SaveData *saveData);
MysteryGiftSave *Save_MysteryGift_Get(SaveData *saveData);
struct MigratedPokemon *Save_MigratedPokemon_Get(SaveData *saveData);
HallOfFame *LoadHallOfFame(SaveData *saveData, enum HeapID heapID, int *ret_p);
int SaveHallOfFame(SaveData *saveData, HallOfFame *hallOfFame);
struct BattleRecord *LoadBattleRecord(SaveData *saveData, enum HeapID heapID, int *ret_p, int idx);
int SaveBattleRecord(SaveData *saveData, struct BattleRecord *data, int idx);
struct FrontierSpeciesRecords *LoadFrontierSpeciesRecords(SaveData *saveData, enum HeapID heapID, int *ret_p);
int SaveFrontierSpeciesRecords(SaveData *saveData, struct FrontierSpeciesRecords *data);
u32 PCStorage_GetSizeOfBox(void);
u32 PCStorage_GetNumBoxes(void);
u32 Save_GetPCBoxModifiedFlags(SaveData *saveData);
void Save_ResetPCBoxModifiedFlags(SaveData *saveData);
void Save_SetAllPCBoxesModified(SaveData *saveData);
void sub_020271A0(SaveData *saveData);

#endif // POKEHEARTGOLD_SAVE_ARRAYS_H
