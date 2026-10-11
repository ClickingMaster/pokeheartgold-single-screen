#pragma force_active on
#include "global.h"
#pragma force_active on
struct Data{u8 pad[22];u16 id;};struct S{int heap;u8 pad[20];int*pc;u8 gap[0xc0-28];struct Data*data;};struct R{u8 pad[16];int choice;};struct R*ov07_0221DF1C(struct S*);int ov07_0221BFC0(struct S*);int BattleAnimUtil_GetBattlerSide(struct S*,int);void ov07_0221ED14(void*,void*);void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);void ov07_0221F024(struct S*);
void ov07_0221F024(struct S*s){struct R*r=ov07_0221DF1C(s);int a,b,c;s->pc++;a=*s->pc;s->pc++;b=*s->pc;s->pc++;c=*s->pc;s->pc++;if(ov07_0221BFC0(s)==1)r->choice=c;else if(BattleAnimUtil_GetBattlerSide(s,s->data->id)==3)r->choice=b;else r->choice=a;SysTask_CreateOnMainQueue(ov07_0221ED14,r,1100);}
