#pragma force_active on
#include "overlay07_slot_motion_work_private.h"
void *BattleAnimUtil_AllocWork(void *,u32);
void BattleAnimUtil_InitCommonWork(void *,Ov07T10Work *);
void BattleAnimUtil_CollectPicsByFlags(void *,u32,Ov07T10Slot *,int *);
u32 BattleAnimSystem_GetArg(void *,int);
void ov07_02222A44(R304SinMotion *,int,int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *,void *,void *);
void ov07_02225444(SysTask *,void *);
void ov07_0222548C(void *);
void ov07_0222548C(void*m){
 int count; int multiplier;
 Ov07SlotMotionWork*w=BattleAnimUtil_AllocWork(m,sizeof(*w));
 BattleAnimUtil_InitCommonWork(m,&w->prefix);
 BattleAnimUtil_CollectPicsByFlags(m,BattleAnimSystem_GetArg(m,0),&w->slot,&count);
 w->slot.xy[1]+=8;
 multiplier=(s32)BattleAnimSystem_GetArg(m,1);
 ov07_02222A44(&w->motion.creator,multiplier,(s32)BattleAnimSystem_GetArg(m,2));
 w->motion.creator.ampX/=2;
 w->motion.creator.ampY/=2;
 BattleAnimSystem_CreateTaskDefaultPriority(w->prefix.owner,(void*)ov07_02225444,w);
}
