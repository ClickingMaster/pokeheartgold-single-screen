#pragma force_active on
#include "global.h"
typedef struct{u8 slot,ability;u16 move;} Data;typedef struct{u8 p[0x94];Data data;u8 q[0x194-0x98];u8 battler;} Work;
extern int BattleSystem_GetPartySize(void *,int);extern void *BattleSystem_GetPartyMon(void *,int,int);extern int GetMonData(void *,int,void *);extern void SetMonData(void *,int,void *),ov12_0226430C(void *,int,int),ov12_02259928(Work *);void ov12_02259358(void *,Work *);
void ov12_02259358(void *battle,Work *w){int zero=0;Data *data=&w->data;int count=BattleSystem_GetPartySize(battle,w->battler),i;for(i=0;i<count;i++){void *mon=BattleSystem_GetPartyMon(battle,w->battler,i);int item;if(data->ability==104)item=0;else item=GetMonData(mon,10,0);if(data->move!=215||(data->move==215&&item!=43))SetMonData(mon,160,&zero);}ov12_0226430C(battle,w->battler,data->slot);ov12_02259928(w);}
