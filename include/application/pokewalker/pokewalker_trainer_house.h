#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_TRAINER_HOUSE_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_TRAINER_HOUSE_H

#include "party.h"
#include "save.h"
#include "save_trainer_house.h"

// The player's Trainer House set as exchanged in the Pokewalker connection app (built at app start), with a CRC.
typedef struct PokewalkerTrainerHouseData {
    TrainerHouseSet set;
    u8 unk_180[0x72];
    u16 crc;                  // 0x1F2, GF_CalcCRC16 of everything before it
} PokewalkerTrainerHouseData; // size=0x1F4

// dst: PokewalkerTrainerHouseData (pokewalker_app.h declares it with void * as well)
void PokewalkerApp_BuildTrainerHouseData(SaveData *saveData, void *dst);
void PokewalkerApp_SaveTrainerHouseData(SaveData *saveData, PokewalkerTrainerHouseData *data);
void PokewalkerApp_FillTrainerHouseTrainer(TrainerHouseTrainer *trainer, SaveData *saveData);
void PokewalkerApp_FillTrainerHouseParty(TrainerHouseMon *mons, Party *party);
int PokewalkerApp_CountTrainerHouseSets(TrainerHouse *trainerHouse);
int PokewalkerApp_FindTrainerHouseSet(TrainerHouse *trainerHouse, TrainerHouseTrainer *trainer, BOOL *found);
void PokewalkerApp_GetTrainerHouseDailyFlags(SaveData *saveData, u8 *flags);
void PokewalkerApp_SetTrainerHouseDailyFlags(SaveData *saveData, const u8 *flags);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_TRAINER_HOUSE_H
