#include "global.h"
#ifndef POKEHEARTGOLD_OVERLAY_80_022340E8
#define POKEHEARTGOLD_OVERLAY_80_022340E8

#include "frontier/frontier_map.h"
#include "frontier/overlay_80_02229EE0.h"

#include "battle_arcade_game_board.h"
#include "save.h"

typedef struct ArcadeContext {
    u32 unk0[2];
    u32 unk8;
    BattleSetup *battleSetup;
    u8 type;
    u8 unk11;
    u8 randomFlag;
    u8 unk13;
    u32 weather;
    u16 winStreak;
    u16 unk1A;
    u8 cursorSpeed;
    u8 bpGain;
    u8 unk1E;
    u8 unk1F;
    u16 unk20;
    u16 unk22;
    u32 unk24;
    BOOL battleWon;
    u8 unk2C[0x3];
    u8 unkF;
    ManagedSprite *unk30[4];
    ManagedSprite *unk40[4];
    ManagedSprite *unk50[4];
    ManagedSprite *unk60[4];
    Party *playerParty;
    Party *opponentParty;
    u16 unk74[14];
    u32 savedHp[4];
    u32 savedAtk[4];
    u32 savedDef[4];
    u32 savedSpd[4];
    u32 savedSpAtk[4];
    u32 savedSpDef[4];
    FrontierTrainerData unkF4[2];
    u16 unk314[4];
    u8 unk31C[4];
    u32 unk320[4];
    void *unk330[1];
    u8 unk334[0xDE];
    u16 unk412[0x3];
    u16 unk418[0x32D];
    u8 unkA72;
    u8 unkA73;
    u8 unkA74;
    u8 unkA75;
    u8 unkA76;
    u8 unkA77;
    u16 multiWinStreak;
    u8 unkA7A;
    u8 unkA7B;
    u8 unkA7C;
} ArcadeContext;

u32 BattleArcadeData_Alloc(SaveData *saveData, u32 a1, u8 a2, u8 a3, u16 a4, u16 a5, u16 *a6);
void BattleArcadeData_Init(void *a0, u32 a1);
void BattleArcadeData_Free(void *a0);
u8 BattleArcade_GetWonBattlePoints(ArcadeContext *ctx, Party *playerParty, Party *opponentParty, int a3);
void ov80_02235364(Party *party, Party *playerParty, int a2, int a3);
int ov80_02237D8C(u8 a0);
void ov80_02234550(void *a0, GAME_BOARD_ARGS *args);
void BattleArcade_SetPartyBeforeBattle(ArcadeContext *ctx);
void BattleArcade_SetPartyAfterBattle(ArcadeContext *ctx);
void ov80_02234588(ArcadeContext *ctx, u32 a1);
u32 ov80_02234764(ArcadeContext *ctx);
u32 ov80_02234774(ArcadeContext *ctx, u8 a0);
void ov80_022347B8(ArcadeContext *ctx);
void ov80_022347C4(ArcadeContext *ctx);
u32 ov80_02234770(ArcadeContext *ctx);
void ov80_02234A74(ArcadeContext *ctx, FrontierMap *map, u32 a2);
void ov80_02234B24(ArcadeContext *ctx, FrontierMap *map, u32 a2);
void ov80_02234BEC(ArcadeContext *ctx, FrontierMap *map, u32 a2);
void ov80_02234CB0(ArcadeContext *ctx, FrontierMap *map, u32 a2);
void ov80_02234968(ArcadeContext *ctx, FrontierMap *map);
void ov80_02234A38(ArcadeContext *ctx, FrontierMap *map);
u32 ov80_022347A8(ArcadeContext *ctx, u32 a1);
void ov80_02234B7C(ArcadeContext *ctx, FrontierMap *map, u32 a2, u32 a3);
void ov80_02234BB4(ArcadeContext *ctx, FrontierMap *map, u32 a2, u32 a3);
void ov80_02234DC4(u32 a0, u32 a1);
u32 ov80_02235324(ArcadeContext *ctx);
u16 ov80_02234E50(ArcadeContext *ctx, u32 a1, u32 a2);
void ov80_02234E98(ArcadeContext *ctx, u8 a1);
void ov80_02234D04(ArcadeContext *ctx, FrontierMap *map, u32 a0, u32 a1, u32 a2);

#endif

#include "party.h"
#include "pokemon.h"
#include "player_avatar.h"
#include "unk_02030A98.h"
#include "save_vars_flags.h"
#include "sys_vars.h"
typedef struct {u8 kind:3,active:1,flag4:1,rest:3;u8 value1,value2,bytes[3];u16 values6[4];u16 valuese[14];u16 values2a[15];} FrontierState3;
extern u32 sub_02030F34(FrontierState3*,int,int,int,int);
extern void sub_02030E7C(void*),ov80_0222A840(void*);
extern u8 BattleArcade_GetMonCount(u8,int);
extern BOOL BattleArcade_MultiplayerCheck(u8);
typedef ArcadeContext ArcadeOwner;
typedef struct {ArcadeContext context;u16*extra;u32 trailer;} ArcadeStorage;
typedef char ActualParentA80[sizeof(ArcadeContext)==0xa80?1:-1];
typedef char ActualStorageA88[sizeof(ArcadeStorage)==0xa88?1:-1];
extern ArcadeOwner *ov80_0223DD4C;
typedef char FullSize[(sizeof(ArcadeStorage)==0xa88)?1:-1];
#pragma force_active on
static inline void AllocateArcadeOwner(void){ArcadeStorage*storage=Heap_Alloc((enum HeapID)11,sizeof(ArcadeStorage));ov80_0223DD4C=&storage->context;MI_CpuFill8(storage,0,sizeof(ArcadeStorage));}
static inline void CopyArcadeMon(Party *party,u16 i){Pokemon *mon;struct{u32 exp,item;}locals;mon=Party_GetMonByIndex(party,ov80_0223DD4C->unk2C[i]);Party_AddMon(ov80_0223DD4C->playerParty,mon);mon=Party_GetMonByIndex(ov80_0223DD4C->playerParty,i);locals.item=0;SetMonData(mon,6,&locals.item);if(GetMonData(mon,161,0)>50){locals.exp=GetMonExpBySpeciesAndLevel(GetMonData(mon,5,0),50);SetMonData(mon,8,&locals.exp);CalcMonLevelAndStats(mon);}}
// @fn BattleArcadeData_Alloc
u32 BattleArcadeData_Alloc(SaveData*save,u32 resume,u8 type,u8 first,u16 second,u16 third,u16*extra){
    u16 i;
    ArcadeOwner * d[1];
    ArcadeOwner *saved;
    u32 index;
    int count;
    FrontierState3 *record;
    void *aux;
    FrontierSave *frontier;
    Party *party;
    Pokemon *mon;

AllocateArcadeOwner();
d[0]=ov80_0223DD4C;d[0]->unk8=(u32)sub_02030E88(save);d[0]->unk0[1]=(u32)save;d[0]->unk0[0]=11;
d[0]=ov80_0223DD4C;d[0]->playerParty=SaveArray_Party_Alloc((enum HeapID)11);d[0]->opponentParty=SaveArray_Party_Alloc((enum HeapID)11);
((ArcadeStorage*)d[0])->extra=extra;d[0]->unk13=0x20;
record=(FrontierState3*)d[0]->unk8;aux=sub_02030FA0(save);
if(resume==0){u8 flag;d[0]=ov80_0223DD4C;d[0]->type=type;d[0]->unk11=0;d[0]->cursorSpeed=3;d[0]->randomFlag=0;sub_02030E7C(record);
if(ov80_0223DD4C->type==3)flag=Save_VarsFlags_GetVar4052(Save_VarsFlags_Get((SaveData*)ov80_0223DD4C->unk0[1]));else flag=sub_02030FE4(aux,8,ov80_0223DD4C->type,0,0);
if(flag==1){d[0]=ov80_0223DD4C;frontier=Save_Frontier_GetStatic((SaveData*)d[0]->unk0[1]);index=sub_0205C2C0(ov80_0223DD4C->type);d[0]->winStreak=FrontierSave_GetStat(frontier,index,sub_0205C268(sub_0205C2C0(d[0]->type)));}else ov80_0223DD4C->winStreak=0;
d[0]=ov80_0223DD4C;d[0]->unk1A=d[0]->winStreak/7;d[0]->unk24=0;d[0]->unk2C[0]=first;d[0]->unk2C[1]=second;d[0]->unk2C[2]=third;
}else{
ov80_0223DD4C->type=sub_02030F34(record,0,0,0,0);i=sub_02030F34(record,2,0,0,0);d[0]=ov80_0223DD4C;d[0]->unk11=i;d[0]->cursorSpeed=sub_02030F34(record,3,0,0,0);d[0]->randomFlag=sub_02030F34(record,1,0,0,0);
saved=(ArcadeOwner*)ov80_0223DD4C;frontier=Save_Frontier_GetStatic((SaveData*)saved->unk0[1]);index=sub_0205C2C0(ov80_0223DD4C->type);saved->winStreak=FrontierSave_GetStat(frontier,index,sub_0205C268(sub_0205C2C0(d[0]->type)));saved->unk1A=saved->winStreak/7;
for(i=0;i<3;i++){d[0]=ov80_0223DD4C;d[0]->unk2C[i]=sub_02030F34(record,6,(u8)i,0,0);}}
for(i=0;i<3;i++){d[0]=ov80_0223DD4C;mon=Party_GetMonByIndex(SaveArray_Party_Get((SaveData*)d[0]->unk0[1]),d[0]->unk2C[i]);d[0]=ov80_0223DD4C;d[0]->unk412[i]=GetMonData(mon,6,0);}
party=SaveArray_Party_Get((SaveData*)d[0]->unk0[1]);count=BattleArcade_GetMonCount(ov80_0223DD4C->type,0);
for(i=0;i<count;i++)CopyArcadeMon(party,i);
if(BattleArcade_MultiplayerCheck(ov80_0223DD4C->type)==1)ov80_0222A840((SaveData*)ov80_0223DD4C->unk0[1]);return (u32)ov80_0223DD4C;
}
