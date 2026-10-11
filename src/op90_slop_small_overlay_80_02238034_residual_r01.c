#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"
#include "heap.h"
typedef struct {
    u8 pad0[0x10];
    u8 type;
    u8 trainerIdx;
    u8 pad12;
    u8 unk13;
    u32 unk14;
    u8 pad18[0x70 - 0x18];
    Party *party1;
    Party *party2;
    u16 trainerIds[14];
} ArcadeData;
typedef struct {
    u8 pad0[8];
    void *unk8;
    void *unkC;
    u8 pad10[8];
    void *unk18;
    void *unk1C;
} FieldSysX;
typedef struct {
    u8 pad0[4];
    Party *parties[4];
    u8 pad14[0x20];
    u32 trainerAI[4][13];
} BattleSetupX;
extern u8 BattleArcade_GetMonCount(u8, int);
extern u8 BattleArcade_GetOpponentMonCount(u8, int);
extern int ov80_02238344(u8);
extern BattleSetupX *BattleSetup_New(enum HeapID, u32);
extern void sub_02051D18(BattleSetupX *, int, void *, void *, void *, void *);
extern BOOL Comm_GetLocalNetId(void);
extern void BattleSetup_AddMonToParty(BattleSetupX *, Pokemon *, int);
extern void BattleSetup_SetAllySideBattlersToPlayer(BattleSetupX *);
extern void *FrontierTrainer_Load(void *, u16, enum HeapID, int);
extern void FrontierTrainer_SetBattleTrainer(BattleSetupX *, void *, int, int, enum HeapID);
extern u32 ov80_02238444(ArcadeData *);
extern void *CommPlayerInfo_GetProfile(int);
extern void HealParty(Party *);
BattleSetupX *BattleArcade_NewBattleSetup(ArcadeData *data, FieldSysX *fs);
// @fn BattleArcade_NewBattleSetup
BattleSetupX *BattleArcade_NewBattleSetup(ArcadeData *data, FieldSysX *fs) {
    u8 trainer[0x30];
    int i;
    Party *playerParty;
    Party *enemyParty;
    u8 playerCount;
    u8 enemyCount;
    BattleSetupX *setup;
    Pokemon *mon;
    int start;
    playerCount = BattleArcade_GetMonCount(data->type, 0);
    enemyCount = BattleArcade_GetOpponentMonCount(data->type, 0);
    setup = BattleSetup_New(HEAP_ID_FIELD2, ov80_02238344(data->type));
    sub_02051D18(setup, 0, fs->unk8, fs->unk18, fs->unkC, fs->unk1C);
    *(u32 *)((u8 *)setup + 0x14C) = 20;
    *(u32 *)((u8 *)setup + 0x150) = 20;
    *(u32 *)((u8 *)setup + 0x174) = data->unk14;
    enemyParty = data->party2;
    playerParty = data->party1;
    if (data->unk13 == 27) {
        playerParty = data->party2;
        enemyParty = data->party1;
    }
    Party_InitWithMaxSize(setup->parties[0], playerCount);
    if (Comm_GetLocalNetId() == FALSE) {
        start = 0;
    } else {
        start = 2;
    }
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < playerCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(playerParty, start), mon);
        BattleSetup_AddMonToParty(setup, mon, 0);
        start++;
    }
    Heap_Free(mon);
    BattleSetup_SetAllySideBattlersToPlayer(setup);
    Heap_Free(FrontierTrainer_Load(trainer, data->trainerIds[data->trainerIdx], HEAP_ID_FIELD2, 204));
    FrontierTrainer_SetBattleTrainer(setup, trainer, enemyCount, 1, HEAP_ID_FIELD2);
    Party_InitWithMaxSize(setup->parties[1], BattleArcade_GetOpponentMonCount(data->type, 0));
    for (i = 0; i < 4; i++) {
        setup->trainerAI[i][0] = ov80_02238444(data);
    }
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < enemyCount; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(enemyParty, i), mon);
        BattleSetup_AddMonToParty(setup, mon, 1);
    }
    Heap_Free(mon);
    switch (data->type) {
    case 2:
    case 3:
        BattleSetup_SetAllySideBattlersToPlayer(setup);
        PlayerProfile_Copy(CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId()), *(void **)((u8 *)setup + 0x100));
        Heap_Free(FrontierTrainer_Load(trainer, data->trainerIds[data->trainerIdx + 7], HEAP_ID_FIELD2, 204));
        FrontierTrainer_SetBattleTrainer(setup, trainer, enemyCount, 3, HEAP_ID_FIELD2);
        Party_InitWithMaxSize(setup->parties[3], BattleArcade_GetOpponentMonCount(data->type, 0));
        mon = AllocMonZeroed(HEAP_ID_FIELD2);
        for (i = 0; i < enemyCount; i++) {
            CopyPokemonToPokemon(Party_GetMonByIndex(enemyParty, enemyCount + i), mon);
            BattleSetup_AddMonToParty(setup, mon, 3);
        }
        Heap_Free(mon);
        break;
    }
    HealParty(data->party1);
    HealParty(data->party2);
    return setup;
}
