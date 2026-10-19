#ifndef POKEHEARTGOLD_FRONTIER_SAVE_ACCESS_H
#define POKEHEARTGOLD_FRONTIER_SAVE_ACCESS_H

#include "save.h"
#include "save_frontier.h"

struct FrontierSpeciesRecords {
    u32 saveKey;
    u16 stats[3][495];
    u16 padding;
}; // size = 0xBA0

// The challenge types of the Battle Factory, Hall, Castle and Arcade.
enum FrontierChallengeType {
    FRONTIER_CHALLENGE_SINGLE,
    FRONTIER_CHALLENGE_DOUBLE,
    FRONTIER_CHALLENGE_MULTI,
    FRONTIER_CHALLENGE_LINK_MULTI,
};

// The field ids of the challenge records and the facility flags (*_GetField / *_SetField).
enum FactoryChallengeField {
    FACTORY_CHALLENGE_LEVEL_MODE,
    FACTORY_CHALLENGE_TYPE,
    FACTORY_CHALLENGE_BATTLE_NO,
    FACTORY_CHALLENGE_TRAINER_IDS,
    FACTORY_CHALLENGE_RENTAL_SET_IDS,
    FACTORY_CHALLENGE_RENTAL_IVS,
    FACTORY_CHALLENGE_RENTAL_PERSONALITIES,
    FACTORY_CHALLENGE_OPPONENT_SET_IDS,
    FACTORY_CHALLENGE_OPPONENT_IVS,
    FACTORY_CHALLENGE_OPPONENT_PERSONALITIES,
    FACTORY_FLAGS_HAS_STREAK,
};

enum HallChallengeField {
    HALL_CHALLENGE_TYPE,
    HALL_CHALLENGE_BATTLE_NO,
    HALL_CHALLENGE_TRAINER_IDS,
    HALL_CHALLENGE_PARTY_SLOTS,
    HALL_CHALLENGE_SET_IDS,
    HALL_FLAGS_HAS_STREAK,
};

enum CastleChallengeField {
    CASTLE_CHALLENGE_TYPE,
    CASTLE_CHALLENGE_BATTLE_NO,
    CASTLE_CHALLENGE_HP,
    CASTLE_CHALLENGE_PP,
    CASTLE_CHALLENGE_STATUS,
    CASTLE_CHALLENGE_HELD_ITEM,
    CASTLE_CHALLENGE_TRAINER_IDS,
    CASTLE_CHALLENGE_PARTY_SLOTS,
    CASTLE_CHALLENGE_SET_IDS,
    CASTLE_FLAGS_HAS_STREAK,
    CASTLE_FLAGS_ONCE,
};

enum ArcadeChallengeField {
    ARCADE_CHALLENGE_TYPE,
    ARCADE_CHALLENGE_RANDOM_FLAG,
    ARCADE_CHALLENGE_BATTLE_NO,
    ARCADE_CHALLENGE_CURSOR_SPEED,
    ARCADE_CHALLENGE_HELD_ITEMS,
    ARCADE_CHALLENGE_TRAINER_IDS,
    ARCADE_CHALLENGE_PARTY_SLOTS,
    ARCADE_CHALLENGE_SET_IDS,
    ARCADE_FLAGS_HAS_STREAK,
};

// The challenge in progress, saved so that it can be resumed. The facilities share one save region.
typedef struct FactoryChallenge {
    u8 levelMode : 1, type : 3, saved : 1, unused : 3;
    u8 battleNo;
    u16 padding;
    u16 trainerIds[14];
    u16 rentalSetIds[4];
    u8 rentalIvs[4];
    u32 rentalPersonalities[4];
    u16 opponentSetIds[4];
    u8 opponentIvs[4];
    u32 opponentPersonalities[4];
} FactoryChallenge;

typedef struct HallChallenge {
    u8 type : 3, saved : 1, unused : 4;
    u8 battleNo;
    u8 partySlots[2];
    u16 trainerIds[20];
    u16 setIds[20];
} HallChallenge;

typedef struct CastleChallenge {
    u8 type : 3, saved : 1, unused : 4;
    u8 battleNo, padding;
    u8 partySlots[3];
    u16 trainerIds[14];
    u16 hp[4];
    u8 pp[4][4];
    u16 padding_3A;
    u32 status[4];
    u16 heldItems[4];
    u16 setIds[16];
} CastleChallenge;

typedef struct ArcadeChallenge {
    u8 type : 3, saved : 1, randomFlag : 1, unused : 3;
    u8 battleNo, cursorSpeed;
    u8 partySlots[3];
    u16 heldItems[4];
    u16 trainerIds[14];
    u16 setIds[15];
} ArcadeChallenge;

u32 Save_Frontier_sizeof(void);
void Save_Frontier_Init(void *);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
void Save_Frontier_Commit(SaveData *saveData);
void Save_Frontier_Load(SaveData *);
u16 FrontierSave_GetStat(FrontierSave *, int, int);
u32 FrontierSave_SetStat(FrontierSave *, int, int, u16);
void FrontierSave_ClearRows(FrontierSave *);
void FrontierSave_DeleteRow(FrontierSave *, int);
void FrontierSave_ClearRow(FrontierSave *, int, int);
u32 FrontierSave_AddStat(FrontierSave *, int, int, int);
u32 FrontierSave_SubStat(FrontierSave *, int, int, int);
void FrontierSave_RaiseStat(FrontierSave *frontier, u32 stat, u32 index, u16 value);
u32 FrontierSpeciesRecords_sizeof(void);
void FrontierSpeciesRecords_Init(void *);
u32 *FrontierSpeciesRecords_Load(SaveData *saveData, u32 heapId, u32 *out);
u32 FrontierSpeciesRecords_Get(SaveData *saveData, u32 *buffer, u32 type, u32 index);
BOOL FrontierSpeciesRecords_UpdateFromStat(SaveData *, int, int, int, u32, enum HeapID, u32 *, int *);

void FactoryFlags_Init(u8 *);
void FactoryFlags_SetField(u32, u32, u8, u8 *);
u32 FactoryFlags_GetField(u32, int, u8, int);
u32 Save_Frontier_GetFactoryFlags(SaveData *);
void HallChallenge_Clear(HallChallenge *);
HallChallenge *Save_Frontier_GetHallChallenge(SaveData *);
u32 HallChallenge_GetSaved(HallChallenge *);
void HallChallenge_SetSaved(HallChallenge *, int);
void HallChallenge_SetField(HallChallenge *, int, u8, int, void *);
u32 HallChallenge_GetField(HallChallenge *, int, u8, int, int);
u8 HallTypeRanks_Get(u8, u8 *);
void HallTypeRanks_Set(u8, u8 *, u8);
void HallTypeRanks_Clear(u8 *);
void HallFlags_Init(u8 *);
u32 Save_Frontier_GetHallFlags(SaveData *);
void HallFlags_SetField(u32, u32, u32, u32, void *);
u32 HallFlags_GetField(u32, int, u8, int, int);
void CastleChallenge_Clear(CastleChallenge *);
CastleChallenge *Save_Frontier_GetCastleChallenge(SaveData *);
u32 CastleChallenge_GetSaved(CastleChallenge *);
void CastleChallenge_SetSaved(CastleChallenge *, int);
void CastleChallenge_SetField(CastleChallenge *, int, u8, int, void *);
u32 CastleChallenge_GetField(CastleChallenge *, int, u8, int, int);
void CastleFlags_Init(u8 *);
u32 Save_Frontier_GetCastleFlags(SaveData *);
void CastleFlags_SetField(u32, u32, u32, u32, void *);
u32 CastleFlags_GetField(u32, u32, u8, u32, u32);
void ArcadeChallenge_Clear(ArcadeChallenge *);
ArcadeChallenge *Save_Frontier_GetArcadeChallenge(SaveData *);
u32 ArcadeChallenge_GetSaved(ArcadeChallenge *);
void ArcadeChallenge_SetSaved(ArcadeChallenge *, int);
void ArcadeChallenge_SetField(ArcadeChallenge *, int, u8, int, void *);
u32 ArcadeChallenge_GetField(ArcadeChallenge *, int, u8, int, int);
void ArcadeFlags_Init(u8 *);
void *Save_Frontier_GetArcadeFlags(SaveData *);
void ArcadeFlags_SetField(void *, u32, u32, u32, void *);
u32 ArcadeFlags_GetField(void *, u32, u8, u32, u32);

void FactoryChallenge_Clear(FactoryChallenge *);
FactoryChallenge *Save_Frontier_GetFactoryChallenge(SaveData *);
u32 FactoryChallenge_GetSaved(FactoryChallenge *);
void FactoryChallenge_SetSaved(FactoryChallenge *, int);
void FactoryChallenge_SetField(FactoryChallenge *, int, u8, void *);
u32 FactoryChallenge_GetField(FactoryChallenge *, int, u8, int);

#endif // POKEHEARTGOLD_FRONTIER_SAVE_ACCESS_H
