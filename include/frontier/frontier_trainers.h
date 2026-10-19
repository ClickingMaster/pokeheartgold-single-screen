#ifndef POKEHEARTGOLD_FRONTIER_TRAINERS_H
#define POKEHEARTGOLD_FRONTIER_TRAINERS_H

#include "global.h"

#include "battle/battle_setup.h"
#include "frontier/frontier_trainer_record.h"

#include "filesystem.h"
#include "heap.h"
#include "message_format.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "save.h"
#include "scrcmd_9.h"
#include "sprite.h"

typedef struct FrontierTrainerData {
    u8 unk0[0x18];
    u16 introMessage[4];
    u8 unk1C[0xF0];
} FrontierTrainerData;

void FrontierData_ReadMember(void *, s32, NarcId);
void FrontierMon_ToPokemon(FrontierMon *, Pokemon *, int);
u16 FrontierTrainer_GetClassSprite(u8);
void Frontier_AddRentalMonToParty(SaveData *, Party *, Pokemon *);
void Frontier_SetSpriteAnim(Sprite *, int);
void Frontier_SetSpritePosition(Sprite *, int, int, BOOL);
int Frontier_GetHpGaugeFrame(u16, u16);
void FrontierTrainer_LoadRecord(FrontierTrainerData *record, u16 trainerId, u32 heapID, u32 narcId);
void FrontierTrainer_SetBattleTrainer(BattleSetup *, FrontierTrainerRecord *, int, int, enum HeapID);
u32 FrontierMon_GenerateWithRandomOtId(FrontierMon *, u16, int, u8, int, enum HeapID, NarcId);
void FrontierMon_GenerateList(void *mons, u16 *setIds, u8 *ivs, u32 *pids, void *outPids, u32 count, u32 heapID, u32 narcId);
void Frontier_BufferPartnerName(MessageFormat *, int);
int Frontier_GetPlayerSpriteId(PlayerProfile *);
void Frontier_RecordLinkPlayerLocations(SaveData *);

#endif
