#pragma force_active on
#include "global.h"
#pragma force_active on

typedef struct{void*system;u8 command,slot,printer,done;u32 pad;} W31796Task;
extern void*BattleSystem_GetMessageLoader(void*);extern void*Heap_Alloc(u32,u32);extern u8 BattleSystem_GetTextFrameDelay(void*);extern u32 BattleSystem_PrintBattleMessage(void*,void*,void*,u8),BattleSystem_GetBattleType(void*);
extern void ov12_02260614(void*,void*),BattleController_ReplySelectionMessageDone(void*,u32),BattleController_EmitCommandComplete(void*,u32,u32);extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,u32);
void ov12_0225AED8(void*,u8*,u8*);
void ov12_0225AED8(void*system,u8*state,u8*cmd){u8 mode=state[0x196];if(mode==0){void*messages=BattleSystem_GetMessageLoader(system);W31796Task*task=Heap_Alloc(5,12);task->system=system;task->command=cmd[0];task->slot=state[0x194];task->done=0;task->printer=BattleSystem_PrintBattleMessage(system,messages,cmd+4,BattleSystem_GetTextFrameDelay(system));SysTask_CreateOnMainQueue(ov12_02260614,task,0);}else if(mode==1){BattleController_ReplySelectionMessageDone(system,state[0x194]);BattleController_EmitCommandComplete(system,state[0x194],cmd[0]);}else{if(!(BattleSystem_GetBattleType(system)&4))BattleController_ReplySelectionMessageDone(system,state[0x194]);BattleController_EmitCommandComplete(system,state[0x194],cmd[0]);}}
