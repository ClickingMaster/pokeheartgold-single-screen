#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 state;u8 p[3];void *owner,*pic;u32 params[3];u32 a,b;int type,mode;s16 x,y;} State;
extern int BattleAnimSystem_GetHeapID(void *),BattleAnimSystem_GetDefender(void *),BattleAnimSystem_GetAttacker(void *),ov07_02222004(void *,int);
extern u32 BattleAnimSystem_GetArg(void *p, int i);extern void *Heap_Alloc(int,u32),*BattleAnimSystem_GetBattlerPokepic(void *,int);extern void ov07_022223F0(void *,int,int,int),Pokepic_SetAttr(void *,int,int),BattleAnimSystem_CreateTaskDefaultPriority(void *,void (*)(void),void *),ov07_02223A1C(void);
void ov07_02223A94(void *);
void ov07_02223A94(void *owner){State *s=Heap_Alloc(BattleAnimSystem_GetHeapID(owner),44);int mode,index,sign,value;s->state=0;s->owner=owner;s->mode=((s32)BattleAnimSystem_GetArg(owner, 2));s->type=((s32)BattleAnimSystem_GetArg(owner, 0));mode=((s32)BattleAnimSystem_GetArg(owner, 3));if(mode==0||mode==2)index=BattleAnimSystem_GetDefender(s->owner);else index=BattleAnimSystem_GetAttacker(s->owner);ov07_022223F0(s->params,s->type,((s32)BattleAnimSystem_GetArg(owner, 1)),s->mode);sign=ov07_02222004(owner,index);s->params[2]*=sign;if(mode==2)sign*=-1;s->pic=BattleAnimSystem_GetBattlerPokepic(s->owner,index);if(((s32)BattleAnimSystem_GetArg(owner, 3))==1){s->x=((s32)BattleAnimSystem_GetArg(owner, 4));value=((s32)BattleAnimSystem_GetArg(owner, 5));}else{value=40;s->x=value*sign;}s->y=value;Pokepic_SetAttr(s->pic,10,s->x);Pokepic_SetAttr(s->pic,11,s->y);BattleAnimSystem_CreateTaskDefaultPriority(s->owner,ov07_02223A1C,s);}
