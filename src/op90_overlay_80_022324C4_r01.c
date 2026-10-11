#include "global.h"
#include "save.h"
#include "save_frontier.h"
#include "party.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "sys_vars.h"
#include "frontier_save_access.h"
typedef struct{u32 heap;void*save,*record;u32 unused;u8 type,round,b,c;u16 wins,sets;u32 state;u8 pad[4];u16 score,old;struct{u8 slots[3],yes;} selection;void*party,*other;u16 trainers[14];u8 p4c[0x36a-0x4c];u16 heldItems[3];u8 p370[0xa20-0x370];void*extra;u32 tail;} D;
extern u32 sub_0205C1F0(u8),sub_0205C218(int),sub_0205C174(u8,u8);
extern u32 CastleFlags_GetField(u32,u32,u8,u32,u32),sub_0205C1A0(u8),sub_0205C268(u32),CastleChallenge_GetField(void*,u32,u8,u32,u32);extern int ov80_02237B24(u8,int),ov80_02237D8C(u8);
extern void CastleChallenge_Clear(void*),ov80_0222A840(void*);

u32 Save_Frontier_GetCastleFlags(SaveData*);
FrontierSave *Save_Frontier_GetStatic(SaveData*);
u16 FrontierSave_GetStat(FrontierSave*,int,int);
D *ov80_022324C4(SaveData*,int,u8,int,u16,u16,void*);
u32 FrontierSave_AddStat(FrontierSave *,int,int,u16);
#pragma force_active on
#pragma force_active off
#pragma force_active on
// @fn ov80_022324C4
D *ov80_022324C4(SaveData*save,int resume,u8 type,int first,u16 second,u16 third,void*extra){
static D *ov80_0223DD48;D*d;void *record;Party *party;Pokemon *mon;FrontierSave*frontier;u32 aux;u16 i;u32 item,exp,index;int count;
ov80_0223DD48=Heap_Alloc((enum HeapID)11,0xa28);MI_CpuFill8(ov80_0223DD48,0,0xa28);
d=ov80_0223DD48;d->record=Save_Frontier_GetCastleChallenge(save);d->save=save;d->heap=11;
d=ov80_0223DD48;d->party=SaveArray_Party_Alloc((enum HeapID)11);d->other=SaveArray_Party_Alloc((enum HeapID)11);
d->extra=extra;
record=d->record;aux=Save_Frontier_GetCastleFlags(save);
if(resume==0){u8 flag;d=ov80_0223DD48;d->type=type;d->round=0;CastleChallenge_Clear(record);
if(ov80_0223DD48->type==3)flag=Save_VarsFlags_GetVar4052(Save_VarsFlags_Get(ov80_0223DD48->save));else flag=CastleFlags_GetField(aux,9,ov80_0223DD48->type,0,0);
if(flag==1){frontier=Save_Frontier_GetStatic(ov80_0223DD48->save);index=sub_0205C1A0(ov80_0223DD48->type);ov80_0223DD48->wins=FrontierSave_GetStat(frontier,index,sub_0205C268(sub_0205C1A0(ov80_0223DD48->type)));}else{d=ov80_0223DD48;d->wins=0;frontier=Save_Frontier_GetStatic(d->save);index=sub_0205C1F0(ov80_0223DD48->type);FrontierSave_SetStat(frontier,index,sub_0205C268(sub_0205C1F0(d->type)),0);frontier=Save_Frontier_GetStatic(ov80_0223DD48->save);index=sub_0205C218(type);FrontierSave_SetStat(frontier,index,sub_0205C268(sub_0205C218(type)),0);for(i=0;i<3;i++){frontier=Save_Frontier_GetStatic(ov80_0223DD48->save);index=sub_0205C174(ov80_0223DD48->type,(u8)i);FrontierSave_SetStat(frontier,index,sub_0205C268(sub_0205C174(ov80_0223DD48->type,(u8)i)),1);}}
d=ov80_0223DD48;d->sets=d->wins/7;d->state=0;d->selection.slots[0]=first;d->selection.slots[1]=second;d->selection.slots[2]=third;
frontier=Save_Frontier_GetStatic((*(D *volatile *)&ov80_0223DD48)->save);index=sub_0205C1F0(ov80_0223DD48->type);d->score=FrontierSave_GetStat(frontier,index,sub_0205C268(sub_0205C1F0(d->type)));
frontier=Save_Frontier_GetStatic(ov80_0223DD48->save);index=sub_0205C1F0(ov80_0223DD48->type);FrontierSave_AddStat(frontier,index,sub_0205C268(sub_0205C1F0(d->type)),10);
}else{D*saved;u32 zero=0;d=ov80_0223DD48;d->type=CastleChallenge_GetField(record,zero,zero,zero,zero);d->round=CastleChallenge_GetField(record,1,zero,zero,zero);saved=ov80_0223DD48;frontier=Save_Frontier_GetStatic(saved->save);index=sub_0205C1A0(ov80_0223DD48->type);saved->wins=FrontierSave_GetStat(frontier,index,sub_0205C268(sub_0205C1A0(d->type)));saved->sets=saved->wins/7;for(i=0;i<3;i++){u8 slot=CastleChallenge_GetField(record,7,(u8)i,zero,zero);ov80_0223DD48->selection.slots[i]=slot;}}
for(i=0;i<3;i++){d=ov80_0223DD48;mon=Party_GetMonByIndex(SaveArray_Party_Get(d->save),d->selection.slots[i]);d=ov80_0223DD48;d->heldItems[i]=GetMonData(mon,6,0);}
party=SaveArray_Party_Get(d->save);count=ov80_02237B24(ov80_0223DD48->type,0);
for(i=0;i<count;i++){mon=Party_GetMonByIndex(party,ov80_0223DD48->selection.slots[i]);Party_AddMon(ov80_0223DD48->party,mon);mon=Party_GetMonByIndex(ov80_0223DD48->party,i);item=0;SetMonData(mon,6,&item);if(GetMonData(mon,161,0)>50){exp=GetMonExpBySpeciesAndLevel(GetMonData(mon,5,0),50);SetMonData(mon,8,&exp);CalcMonLevelAndStats(mon);}}
if(ov80_02237D8C(ov80_0223DD48->type)==1)ov80_0222A840(ov80_0223DD48->save);return ov80_0223DD48;
}
