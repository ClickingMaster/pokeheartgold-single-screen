#ifndef POKEHEARTGOLD_FRONTIER_SAVE_ACCESS_H
#define POKEHEARTGOLD_FRONTIER_SAVE_ACCESS_H

#include "save.h"
#include "save_frontier.h"

struct FrontierSpeciesRecords {
    u32 unk_000;
    u16 stats[3][495];
    u16 padding;
}; // size = 0xBA0

void FactoryFlags_SetField(u32, u32, u8, u8 *);
int Save_Frontier_GetHallFlags(SaveData *);
int HallFlags_GetField(int, int, u8, int, int);
int Save_Frontier_GetFactoryFlags(SaveData *);
void HallFlags_SetField(u32, u32, u32, u32, u8 *);
void *Save_Frontier_GetCastleChallenge(SaveData *saveData);
u32 Save_Frontier_GetCastleFlags(SaveData *saveData);
void CastleFlags_SetField(u32, u32, u32, u32, u8 *);
u32 CastleFlags_GetField(u32, u32, u8, u32, u32);
void *Save_Frontier_GetHallChallenge(SaveData *);
void *Save_Frontier_GetArcadeChallenge(SaveData *saveData);
void *Save_Frontier_GetArcadeFlags(SaveData *saveData);
void ArcadeFlags_SetField(void *, u32, u32, u32, u8 *);
u32 ArcadeFlags_GetField(void *, u32, u8, u32, u32);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
void Save_Frontier_Load(SaveData *saveData);
u16 FrontierSave_GetStat(FrontierSave *, int, int);
void Save_Frontier_Commit(SaveData *saveData);
u32 FrontierSave_SetStat(FrontierSave *, int, int, u16);
void FrontierSave_RaiseStat(FrontierSave *a0, u32 a1, u32 a2, u16 a3);
u32 *FrontierSpeciesRecords_Load(SaveData *saveData, u32 a1, u32 *out);
u32 FrontierSpeciesRecords_Get(SaveData *saveData, u32 *a1, u32 a2, u32 a3);
u32 ArcadeChallenge_GetSaved(u32 a0);
void FrontierSave_ClearRows(FrontierSave *frontier);

#endif // POKEHEARTGOLD_FRONTIER_SAVE_ACCESS_H
