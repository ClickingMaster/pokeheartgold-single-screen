#ifndef POKEHEARTGOLD_FRONTIER_OPPONENTS_H
#define POKEHEARTGOLD_FRONTIER_OPPONENTS_H

#include "field_types_def.h"
#include "message_format.h"
#include "scrcmd_9.h"

MessageFormat *Frontier_NewBannedSpeciesMessageFormat(SaveData *saveData, u16 numEligiblePokemon, u16 a2, u8 a3, u8 *numLegendaryPokemonSeen);
u32 GetFrontierTrainerOverworld(u8 trainerClass);
u16 FrontierFieldSystem_ReceiveLinkAllyData(FieldSystem *fieldSystem, u16 *linkAllyData); // SetLinkAllyData?
BOOL FrontierFieldSystem_ReceiveRoundTrainers(FieldSystem *fieldSystem, u16 *trainers);
BOOL sub_0204B690(FieldSystem *fieldSystem, u16 *a1);
void FrontierFieldSystem_PackLinkAllyData(FrontierFieldSystem *frontierFsys, SaveData *saveData);
void FrontierFieldSystem_PackRoundTrainers(FrontierFieldSystem *frontierFsys);
void FrontierFieldSystem_0204B720(FrontierFieldSystem *frontierFsys, u32 a1);
u16 FrontierFieldSystem_GetRandomFrontierTrainerID(FrontierFieldSystem *frontierFsys, u8 difficultyBracket, u8 winsThisRound, u32 towerMode);
u32 FrontierFieldSystem_GenerateFrontierTrainer(FrontierFieldSystem *frontierFsys, FrontierTrainer *frontierTrainer, u16 frontierTrainerIndex, u32 numPokemon, u16 *partySpecies, u16 *partyItems, FrontierMultiBattleAllyData *allyData, enum HeapID heapID);
void FrontierFieldSystem_GenerateAllyFrontierMons(FrontierFieldSystem *frontierFsys, FrontierTrainer *frontierTrainer, u16 frontierTrainerIndex, u32 replaceItem, FrontierMultiBattleAllyData *allyData, enum HeapID heapID);

#endif // POKEHEARTGOLD_FRONTIER_OPPONENTS_H
