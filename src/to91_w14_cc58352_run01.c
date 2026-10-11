#pragma force_active on
#include "global.h"
#include "battle/battle_system.h"
#include "battle_controller_opponent_r2_exact_private.h"
#include "pokepic.h"
#include "heap.h"
#include "sys_task_api.h"
#pragma force_active on
typedef struct CCWork {
 BattleSystem *system; OpponentData *opponent; Pokepic *pic;
 u8 pad0C[4]; BattleSourceR2 source; u8 pad4C[0x1C];
 u8 command,battler,type,state,form,pad6D; u16 species;
 u32 personality,comparison;
} CCWork;
typedef char CCSize[sizeof(CCWork)==0x78?1:-1];
extern void BattleController_InitMoveAnimData(BattleSystem*,BattleContext*,void*,int,int,int,int,int);
extern void ov07_0223494C(void*,int);
extern int BattleAnimSystem_Execute(u8*);
extern BOOL BattleAnimSystem_IsActive(void*),BattleAnimSystem_EndMove(void*);
extern void BattleController_EmitCommandComplete(BattleSystem*,int,int);
void ov12_0225CC58(SysTask*,void*);
void ov12_0225CC58(SysTask *task,void *data) {
 CCWork *w=data;
 BattleSourceR1 a; BattleParamsR2 info; BattleSourceR1 b,c;
 u32 *p=ov12_0223A8DC(w->system);
 switch(w->state) {
 case 0:if(w->personality){BattleController_InitMoveAnimData(w->system,0,&a,1,15,w->battler,w->battler,0);ov12_02261B80(w->system,(u8*)w->opponent,p,&a);w->state++;}else w->state=4;break;
 case 2:ov12_02261CA8(w->system,&w->source,&info,w->battler);ov07_0223494C(&info,5);BattleController_InitMoveAnimData(w->system,0,&b,1,16,w->battler,w->battler,0);ov12_02261B80(w->system,(u8*)w->opponent,p,&b);w->opponent->unk1A0=0;w->state++;break;
 case 1:case 3:case 5:BattleAnimSystem_Execute((u8*)p);if(!BattleAnimSystem_IsActive(p)){BattleAnimSystem_EndMove(p);w->state++;}break;
 case 4:BattleController_InitMoveAnimData(w->system,0,&c,1,42,w->battler,w->battler,0);ov12_02261B80(w->system,(u8*)w->opponent,p,&c);w->state++;break;
 case 6:Pokepic_Delete(w->pic);
 case 7:BattleController_EmitCommandComplete(w->system,w->battler,w->command);Heap_Free(w);SysTask_Destroy(task);break;
 }
}
