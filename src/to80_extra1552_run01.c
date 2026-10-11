#pragma force_active on
#include "global.h"
#define PTR(p,o) (*(void **)((u8*)(p)+(o)))
#define WORD(p,o) (*(u32 *)((u8*)(p)+(o)))
#define BYTE(p,o) (*((u8*)(p)+(o)))
#define HALF(p,o) (*(u16 *)((u8*)(p)+(o)))
extern int ov80_02236DD4(u8),ov80_02236DF8(u8,int),ov80_022370F4(u8),Comm_GetLocalNetId(void);
extern void HealParty(void*),*BattleSetup_New(int,int),*AllocMonZeroed(int),*Party_GetMonByIndex(void*,int),*CommPlayerInfo_GetProfile(int),*FrontierTrainer_Load(void*,u16,int,int);
extern void sub_02051D18(void*,int,void*,void*,void*,void*),Party_InitWithMaxSize(void*,int),CopyPokemonToPokemon(void*,void*),BattleSetup_AddMonToParty(void*,void*,int),Heap_Free(void*),BattleSetup_SetAllySideBattlersToPlayer(void*),FrontierTrainer_SetBattleTrainer(void*,void*,int,int,int),PlayerProfile_Copy(void*,void*);
extern u32 ov80_02237264(void*);
void *ov80_02236F24(void*,void*);
void *ov80_02236F24(void*s,void*field){int playerCount=ov80_02236DD4(BYTE(s,4)),enemyCount=ov80_02236DF8(BYTE(s,4),0);void *setup,*mon;int i,j;u8 trainer[48];
 HealParty(PTR(s,0x4d4));HealParty(PTR(s,0x4d8));setup=BattleSetup_New(11,ov80_022370F4(BYTE(s,4)));sub_02051D18(setup,0,PTR(field,8),PTR(field,0x18),PTR(field,12),PTR(field,0x1c));WORD(setup,0x14c)=19;WORD(setup,0x150)=19;Party_InitWithMaxSize(PTR(setup,4),playerCount);mon=AllocMonZeroed(11);
 for(i=0;i<playerCount;i++){CopyPokemonToPokemon(Party_GetMonByIndex(PTR(s,0x4d4),i),mon);BattleSetup_AddMonToParty(setup,mon,0);}Heap_Free(mon);BattleSetup_SetAllySideBattlersToPlayer(setup);Heap_Free(FrontierTrainer_Load(trainer,((u16*)((u8*)s+0x18))[BYTE(s,6)],11,204));FrontierTrainer_SetBattleTrainer(setup,trainer,enemyCount,1,11);Party_InitWithMaxSize(PTR(setup,8),ov80_02236DF8(BYTE(s,4),0));{int k;u8 *dst;k=0;dst=setup;do{WORD(dst,0x34)=ov80_02237264(s);k++;dst+=52;}while(k<4);}mon=AllocMonZeroed(11);
 for(i=0;i<enemyCount;i++){CopyPokemonToPokemon(Party_GetMonByIndex(PTR(s,0x4d8),i),mon);BattleSetup_AddMonToParty(setup,mon,1);}Heap_Free(mon);
 if(BYTE(s,4)==2)goto ally;if(BYTE(s,4)!=3)goto end;ally: {BattleSetup_SetAllySideBattlersToPlayer(setup);PlayerProfile_Copy(CommPlayerInfo_GetProfile(1-Comm_GetLocalNetId()),PTR(setup,0x100));Heap_Free(FrontierTrainer_Load(trainer,((u16*)((u8*)s+0x18))[BYTE(s,6)+7],11,204));FrontierTrainer_SetBattleTrainer(setup,trainer,enemyCount,3,11);Party_InitWithMaxSize(PTR(setup,16),ov80_02236DF8(BYTE(s,4),0));mon=AllocMonZeroed(11);j=0;if(enemyCount>0){i=enemyCount;do{CopyPokemonToPokemon(Party_GetMonByIndex(PTR(s,0x4d8),i),mon);BattleSetup_AddMonToParty(setup,mon,3);i++;j++;}while(j<enemyCount);}Heap_Free(mon);}
end: return setup;
}
