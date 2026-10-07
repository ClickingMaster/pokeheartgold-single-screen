#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
int ov80_02237B24(int,int),ov80_02237B58(int,int),ov80_02237D5C(int),sub_0203769C(void);
void HealParty(void*);void*BattleSetup_New(int,int);void sub_02051D18(void*,int,void*,void*,void*,void*);void Party_InitWithMaxSize(void*,int);void*AllocMonZeroed(int);void*Party_GetMonByIndex(void*,int);void CopyPokemonToPokemon(void*,void*);void BattleSetup_AddMonToParty(void*,void*,int);void Heap_Free(void*);void BattleSetup_SetAllySideBattlersToPlayer(void*);
void*ov80_02229F04(void*,int,int,int);void ov80_0222A480(void*,void*,int,int,int);u32 ov80_02237E88(void*);void*sub_02034818(int);void PlayerProfile_Copy(void*,void*);
typedef struct {u8 padding[0x30];u16 id;} TrainerRow;
void*ov80_02237B8C(void*,void*);
void*ov80_02237B8C(void*p,void*ctx){int count1,own=ov80_02237B24(B(p,16),0),enemy=ov80_02237B58(B(p,16),0),index,i,count3;void *setup,*mon;u8 *stat;u8 trainer[48];HealParty(P(p,0x2c));setup=BattleSetup_New(11,ov80_02237D5C(B(p,16)));sub_02051D18(setup,0,P(ctx,8),P(ctx,0x18),P(ctx,12),P(ctx,0x1c));W(setup,0x14c)=21;W(setup,0x150)=21;Party_InitWithMaxSize(P(setup,4),own);index=sub_0203769C()==0?0:2;mon=AllocMonZeroed(11);for(count1=0;count1<own;count1++){CopyPokemonToPokemon(Party_GetMonByIndex(P(p,0x28),index),mon);BattleSetup_AddMonToParty(setup,mon,0);index++;}Heap_Free(mon);BattleSetup_SetAllySideBattlersToPlayer(setup);Heap_Free(ov80_02229F04(trainer,((TrainerRow*)((u8*)p+B(p,17)*2))->id,11,0xcc));ov80_0222A480(setup,trainer,enemy,1,11);Party_InitWithMaxSize(P(setup,8),ov80_02237B58(B(p,16),0));for(i=0,stat=(u8*)setup;i<4;i++,stat+=0x34)W(stat,0x34)=ov80_02237E88(p);mon=AllocMonZeroed(11);for(index=0;index<enemy;index++){CopyPokemonToPokemon(Party_GetMonByIndex(P(p,0x2c),index),mon);BattleSetup_AddMonToParty(setup,mon,1);}Heap_Free(mon);switch(B(p,16)){case 2:case 3:BattleSetup_SetAllySideBattlersToPlayer(setup);PlayerProfile_Copy(sub_02034818(1-sub_0203769C()),P(setup,0x100));Heap_Free(ov80_02229F04(trainer,((TrainerRow*)((u8*)p+(B(p,17)+7)*2))->id,11,0xcc));ov80_0222A480(setup,trainer,enemy,3,11);Party_InitWithMaxSize(P(setup,16),ov80_02237B58(B(p,16),0));mon=AllocMonZeroed(11);count3=0;if(enemy>0){index=enemy;do{CopyPokemonToPokemon(Party_GetMonByIndex(P(p,0x2c),index),mon);BattleSetup_AddMonToParty(setup,mon,3);index++;count3++;}while(count3<enemy);}Heap_Free(mon);break;}return setup;}
