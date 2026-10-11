#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 heap;u8 facility,variant,status,pad;u16 streak;u8 flag,pad2;u16 total;u8 pad3[0x18-0xe];u16 stats[14];u8 pad4[0x3d2-0x34];u16 secondIds[6];u8 pad5[0x4d4-0x3de];void *firstParty,*secondParty;u8 pad6[0x4e8-0x4dc];u16 firstIds[6];void *record,*save;} Context;
typedef struct {u16 half;u8 pad[6];u8 byte;u8 pad2[3];u32 word;u8 tail[12];} Temp;
extern void *Save_Frontier_GetFactoryFlags(void*),*Save_Frontier_GetStatic(void*),*Party_GetMonByIndex(void*,int);
extern void ov80_02236DD4(u8),ov80_02236DF8(u8,int),FactoryChallenge_SetField(void*,int,u8,void*),FactoryChallenge_SetSaved(void*,int),FactoryFlags_SetField(void*,int,u8,void*);
extern u32 sub_0205C048(u8,u8),sub_0205BFF0(u8,u8),sub_0205C01C(u8,u8),sub_0205C074(u8,u8),sub_0205C268(u32),FrontierSave_GetStat(void*,u32,u32),GetMonData(void*,int,void*),Party_GetCount(void*);
extern void FrontierSave_SetStat(void*,u32,u32,u16),FrontierSave_RaiseStat(void*,u32,u32,u16);
void ov80_0223049C(Context*,int);
void ov80_0223049C(Context *c,int mode){Temp temp;void *cache=Save_Frontier_GetFactoryFlags(c->save);void *frontier=Save_Frontier_GetStatic(c->save);u16 i;u32 count;
ov80_02236DD4(c->facility);ov80_02236DF8(c->facility,1);
temp.byte=c->variant;FactoryChallenge_SetField(c->record,0,0,&temp.byte);temp.byte=c->facility;FactoryChallenge_SetField(c->record,1,0,&temp.byte);FactoryChallenge_SetSaved(c->record,1);temp.byte=c->status;FactoryChallenge_SetField(c->record,2,0,&temp.byte);
FrontierSave_SetStat(frontier,sub_0205C048(c->variant,c->facility),sub_0205C268(sub_0205C048(c->variant,c->facility)),c->streak);
FrontierSave_SetStat(frontier,sub_0205BFF0(c->variant,c->facility),sub_0205C268(sub_0205BFF0(c->variant,c->facility)),c->total);
if(mode!=2){u32 old=FrontierSave_GetStat(frontier,sub_0205C01C(c->variant,c->facility),sub_0205C268(sub_0205C01C(c->variant,c->facility)));u32 newValue;
FrontierSave_RaiseStat(frontier,sub_0205C01C(c->variant,c->facility),sub_0205C268(sub_0205C01C(c->variant,c->facility)),c->total);
newValue=FrontierSave_GetStat(frontier,sub_0205C01C(c->variant,c->facility),sub_0205C268(sub_0205C01C(c->variant,c->facility)));
if(c->total==old)FrontierSave_RaiseStat(frontier,sub_0205C074(c->variant,c->facility),sub_0205C268(sub_0205C074(c->variant,c->facility)),c->streak);else if(old<newValue)FrontierSave_SetStat(frontier,sub_0205C074(c->variant,c->facility),sub_0205C268(sub_0205C074(c->variant,c->facility)),c->streak);
temp.byte=c->flag;FactoryFlags_SetField(cache,10,(u8)(c->facility+c->variant*4),&temp.byte);if(c->facility==3){int index;if(c->variant==0)index=102;else index=104;FrontierSave_SetStat(frontier,index,sub_0205C268(index),c->flag);}}
for(i=0;i<14;i++){temp.half=c->stats[i];FactoryChallenge_SetField(c->record,3,(u8)i,&temp.half);}
count=Party_GetCount(c->firstParty);for(i=0;i<count;i++){void *mon=Party_GetMonByIndex(c->firstParty,i);temp.half=c->firstIds[i];FactoryChallenge_SetField(c->record,4,(u8)i,&temp.half);temp.byte=GetMonData(mon,71,0);FactoryChallenge_SetField(c->record,5,(u8)i,&temp.byte);temp.word=GetMonData(mon,0,0);FactoryChallenge_SetField(c->record,6,(u8)i,&temp.word);}
count=Party_GetCount(c->secondParty);for(i=0;i<count;i++){void *mon=Party_GetMonByIndex(c->secondParty,i);temp.half=c->secondIds[i];FactoryChallenge_SetField(c->record,7,(u8)i,&temp.half);temp.byte=GetMonData(mon,71,0);FactoryChallenge_SetField(c->record,8,(u8)i,&temp.byte);temp.word=GetMonData(mon,0,0);FactoryChallenge_SetField(c->record,9,(u8)i,&temp.word);}
}
