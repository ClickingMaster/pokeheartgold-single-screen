#pragma force_active on
#include "global.h"
#include "heap.h"
extern const u8 SDK_OVERLAY_OVY_5_ID;
extern void *OverlayManager_CreateAndGetData(void *,u32,int),*OverlayManager_GetArgs(void *);
extern void ov12_02238A68(void *,void *),WirelessStatusIcon_Destroy(void),HandleLoadOverlay(int,int),ov05_0221BA00(void *);
extern u32 Comm_GetLocalNetId(void),sub_020378AC(u8),BattleSystem_GetBattlerIdPartner(void *,u8);
int ov12_0223A0D4(void *);
#define W(o) (*(void **)(state+(o)))
#define I(o) (*(u32 *)(state+(o)))
#define FIELD(o) (*(void **)((u8 *)W(0x1c4)+(o)))
#pragma force_active on
int ov12_0223A0D4(void *manager){
 u8 *state=OverlayManager_CreateAndGetData(manager,0x2490,5);void *args=OverlayManager_GetArgs(manager);u8 battler;
 MIi_CpuClearFast(0,(u32 *)state,0x2490);ov12_02238A68(state,args);
 if(!(I(0x2c)&4)||(I(0x240c)&16)||(I(0x2c)&128)){WirelessStatusIcon_Destroy();return 0;}
 HandleLoadOverlay((int)&SDK_OVERLAY_OVY_5_ID,2);if(!(I(0x2c)&8))return 0;
 W(0x1c4)=Heap_Alloc((enum HeapID)5,0x30);MIi_CpuClearFast(0,(u32 *)W(0x1c4),0x30);battler=(u8)Comm_GetLocalNetId();
 switch(sub_020378AC(battler)){
 case 0:case 3:FIELD(4)=((void **)(state+0x68))[battler];FIELD(12)=W(0x68+BattleSystem_GetBattlerIdPartner(state,battler)*4);break;
 case 1:case 2:FIELD(4)=W(0x68+BattleSystem_GetBattlerIdPartner(state,battler)*4);FIELD(12)=((void **)(state+0x68))[battler];break;
 }
 *(u32 *)((u8 *)W(0x1c4)+0x24)=5;*((u8 *)W(0x1c4)+0x28)=0;
 switch(sub_020378AC(battler)){
 case 0:case 3:*((u8 *)W(0x1c4)+0x29)=0;break;
 case 1:case 2:*((u8 *)W(0x1c4)+0x29)=1;break;
 }
 ov05_0221BA00(W(0x1c4));return 1;
}
