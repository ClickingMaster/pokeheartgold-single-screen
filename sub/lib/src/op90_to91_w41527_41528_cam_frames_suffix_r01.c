#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct { u16 state; u8 pad[0x1a]; } CamSta;
typedef struct {
    u8 pad0[0x31c];
    CamSta *sta;    /* 0x31c */
    u8 pad320[0x350 - 0x320];
    u16 mode;       /* 0x350 */
    u8 pad352[0x52e - 0x352];
    u16 dozeMap;    /* 0x52e */
    u16 map530;     /* 0x530 */
    u16 map532;     /* 0x532 */
} WlWork;
#define ROOT (*(WlWork **)0x0380fff4)
extern u32 __VENEER_OS_DisableIrqMask(u32);
extern u32 __VENEER_OS_EnableIrqMask(u32);
extern u32 CAM_GetAID(u16);
extern void CAM_ReleaseAID(u16);
extern void CAM_SetDoze(u16);
void CAM_SetStaState(u16 aid, u16 state);
#pragma opt_common_subs off
// @fn CAM_SetStaState
void CAM_SetStaState(u16 aid, u16 state){u32 e;e=__VENEER_OS_DisableIrqMask(0x1000000);if(state<0x40){ROOT->map530|=1<<aid;ROOT->map532|=1<<aid;if(ROOT->mode==1&&CAM_GetAID(aid)!=0)CAM_ReleaseAID(aid);}else{ROOT->map532&=~(1<<aid);if((ROOT->dozeMap>>aid)&1)CAM_SetDoze(aid);}ROOT->sta[aid].state=state;__VENEER_OS_EnableIrqMask(e);}
#pragma opt_common_subs reset
