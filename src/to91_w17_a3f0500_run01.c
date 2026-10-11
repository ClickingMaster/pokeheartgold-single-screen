#pragma force_active on
#include "global.h"
#include "battle/battle_022378C0.h"
#include "battle/battle_setup.h"
#include "game_stats.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "heap.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#pragma force_active on
FS_EXTERN_OVERLAY(OVY_5);
typedef struct Result {
 BattleSetup *setup;Party *party[4];String *names[4];enum HeapID heap;
 u8 kind,multi,result,pad,flag,tail[3];
} Result;
typedef char ResultSize30[sizeof(Result)==48?1:-1];
extern void ov05_0221BA00(void*);
BOOL ov12_0223A3F0(OverlayManager *manager){
 BattleSetup *args=OverlayManager_GetArgs(manager);
 int local,i,offset,other,slot;Result *out;
 if(!(args->battleType&4)||(args->battleSpecial&16)||(args->battleType&0x80))return FALSE;
 local=(u8)Comm_GetLocalNetId();HandleLoadOverlay(FS_OVERLAY_ID(OVY_5),OVY_LOAD_ASYNC);
 out=Heap_Alloc(HEAP_ID_BATTLE,sizeof(Result));args->evolutionTaskData=(EvolutionTaskData*)out;
 MIi_CpuClearFast(0,(u32*)out,sizeof(Result));out->setup=args;
 switch(args->winFlag){
 case 1:if(!sub_02039998())GameStats_Inc(args->gameStats,22);else GameStats_Inc(args->gameStats,27);break;
 case 2:if(!sub_02039998())GameStats_Inc(args->gameStats,23);else GameStats_Inc(args->gameStats,28);break;
 case 3:case 5:if(!sub_02039998())GameStats_Inc(args->gameStats,24);else GameStats_Inc(args->gameStats,29);break;
 case 0:case 4:break;}
 if(args->battleType&8){
 for(i=0;i<4;i++){out->party[sub_020378AC(i)]=args->party[i];slot=sub_020378AC(i);out->names[slot]=PlayerProfile_GetPlayerName_NewString(args->profile[i],HEAP_ID_BATTLE);}
 out->heap=HEAP_ID_BATTLE;out->kind=2;out->multi=1;if(args->winFlag!=5)out->result=args->winFlag;else out->result=3;
 }else{
 offset=local;out->party[sub_020378AC(local)]=args->party[offset];other=local^1;out->party[sub_020378AC(other)]=args->party[other];
 slot=sub_020378AC(local);out->names[slot]=PlayerProfile_GetPlayerName_NewString(args->profile[offset],HEAP_ID_BATTLE);
 slot=sub_020378AC(other);out->names[slot]=PlayerProfile_GetPlayerName_NewString(args->profile[other],HEAP_ID_BATTLE);
 out->heap=HEAP_ID_BATTLE;out->kind=2;out->multi=0;if(args->winFlag!=5)out->result=args->winFlag;else out->result=3;
 }
 out->flag=args->unk1B2;ov05_0221BA00(out);return TRUE;
}
