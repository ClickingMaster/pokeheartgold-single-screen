#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_propagation off
typedef struct A{u8 pad[17];u8 index;}A;typedef struct Entry{u8 pad[40];u8 value[5];u8 gap[35];}Entry;typedef struct W{A*args;Entry entries[];}W;
extern void BattlePartyMenu_SetSpriteAnim(void*,int),BattlePartyMenu_ShowSpriteAt(void*,int,int);extern int BattlePartyMenu_CalcConditionMarkerCoord(u8,int,int);void BattlePartyMenu_ShowConditionMarkers(W*);
void BattlePartyMenu_ShowConditionMarkers(W*w){register int offset=80*w->args->index;register Entry*entries=w->entries;int x,y;
BattlePartyMenu_SetSpriteAnim(*(void**)((u8*)w+0x2024),0);
BattlePartyMenu_SetSpriteAnim(*(void**)((u8*)w+0x2028),1);
BattlePartyMenu_SetSpriteAnim(*(void**)((u8*)w+0x202c),3);
BattlePartyMenu_SetSpriteAnim(*(void**)((u8*)w+0x2030),4);
BattlePartyMenu_SetSpriteAnim(*(void**)((u8*)w+0x2034),2);
x=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[0],144,144);y=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[0],2,24);BattlePartyMenu_ShowSpriteAt(*(void**)((u8*)w+0x2024),x,y);
x=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[1],164,144);y=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[1],16,24);BattlePartyMenu_ShowSpriteAt(*(void**)((u8*)w+0x2028),x,y);
x=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[2],156,144);y=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[2],41,24);BattlePartyMenu_ShowSpriteAt(*(void**)((u8*)w+0x202c),x,y);
x=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[3],131,143);y=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[3],41,24);BattlePartyMenu_ShowSpriteAt(*(void**)((u8*)w+0x2030),x,y);
x=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[4],123,143);y=BattlePartyMenu_CalcConditionMarkerCoord(((Entry*)((u8*)entries+offset))->value[4],16,24);BattlePartyMenu_ShowSpriteAt(*(void**)((u8*)w+0x2034),x,y);
}
