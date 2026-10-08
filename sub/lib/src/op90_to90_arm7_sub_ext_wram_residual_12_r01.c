#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8 wmspW[];
#define STATUS (*(u8 **)(wmspW+0x1550))
#define H(off) (*(u16*)(status+(off)))
#define W(off) (*(u32*)(status+(off)))
u32 __VENEER_OS_DisableInterrupts(void);void __VENEER_OS_RestoreInterrupts(u32);
u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);void __VENEER_WMSP_ReturnResult2Wm9(void*);
void WMSP_CancelVAlarm(void);void WMSP_SetThreadPriorityLow(void);void WMSP_CleanSendQueue(u32);void WMSP_IndicateDisconnectionFromMyself(int,u16,void*);void __VENEER_MI_CpuFill8(void*,u32,u32);void __VENEER_MI_CpuCopy8(const void*,void*,u32);u16 *WMSP_WL_DevGetStationState(void*);u16 *WMSP_WL_ParamGetMode(void*);u16 *WMSP_WL_MlmeDeAuthenticate(void*,void*,u32);u16 *WMSP_WL_MlmeReset(void*,u32);u16 *WMSP_WL_DevIdle(void*);u16 *WMSP_WL_ParamSetPreambleType(void*,u32);void WMSP_ResetSizeVars(void);u16 *WMSP_WL_DevTestSignal(void*,u32,u32,u32,u32);void WmspError_06002FC0(u32,u32);
void WMSP_Reset(void);
// @fn WMSP_Reset
void WMSP_Reset(void){
struct {u8 broadcast[6],mac[6],buffer[512];} local;
#define mac local.mac
#define broadcast local.broadcast
int index;u8 *buffer=local.buffer;u8 *status=STATUS;u32 cancel=0,old,connections;int parent;int state,retry;u32 mode;u16 *result,*callback;
old=__VENEER_OS_DisableInterrupts();if(W(12)==1){W(12)=0;cancel=1;WMSP_CancelVAlarm();WMSP_SetThreadPriorityLow();{u16 s=H(0);if(s==10)H(0)=8;else if(s==9)H(0)=7;}}
{u16 s=H(0);if((u16)(s+0xfff9)<=1){connections=H(0x182);parent=(s==7);}else {connections=0;}}
H(0x182)=0;H(0x86)=0;W(0x14)=0;W(0x10)=0;W(0x1c)=0;H(0xc2)=0;__VENEER_OS_RestoreInterrupts(old);if(cancel)WMSP_CleanSendQueue(65535);if(parent)H(0xf6)=0;if(connections){for(index=0;index<16;index++)if(connections&(1U<<index))WMSP_IndicateDisconnectionFromMyself(parent,(u16)index,index==0?status+0x18a:status+0x128+(index-1)*6);}
__VENEER_MI_CpuFill8(status+0x128,0,90);result=WMSP_WL_DevGetStationState(buffer);if(result[2]){WmspError_06002FC0(0x308,result[2]);return;}state=result[3];result=WMSP_WL_ParamGetMode(buffer);if(result[2]){WmspError_06002FC0(0x284,result[2]);return;}mode=result[3];
switch(state){case 48:case 64:
if((u16)(mode+0xfffe)<=1){__VENEER_MI_CpuCopy8(status+0x18a,mac,6);for(retry=0;retry<2;retry++){result=WMSP_WL_MlmeDeAuthenticate(buffer,mac,3);switch(result[2]){case 0:H(0)=3;goto reset;case 7:case 12:break;default:goto reset;}}}
else if(mode==1){__VENEER_MI_CpuFill8(broadcast,255,6);result=WMSP_WL_MlmeDeAuthenticate(buffer,broadcast,3);if(result[2]==0)H(0)=3;}
case 32:reset:result=WMSP_WL_MlmeReset(buffer,1);if(result[2]){WmspError_06002FC0(0,result[2]);return;}
case 0:result=WMSP_WL_DevIdle(buffer);if(result[2]){WmspError_06002FC0(0x302,result[2]);return;}
case 16:if(H(0x1ee)==0){result=WMSP_WL_ParamSetPreambleType(buffer,1);if(result[2]){WmspError_06002FC0(0x20e,result[2]);return;}H(0x1ee)=1;}H(0)=2;W(0x198)=0;WMSP_ResetSizeVars();break;
case 17:case 18:if(mode==0){result=WMSP_WL_DevTestSignal(buffer,0,0,20,1);if(result[2]){WmspError_06002FC0(0x309,result[2]);return;}}result=WMSP_WL_DevIdle(buffer);if(result[2]){WmspError_06002FC0(0x302,result[2]);return;}H(0)=2;break;
default:WmspError_06002FC0(0x308,0);return;}
callback=__VENEER_WMSP_GetBuffer4Callback2Wm9();callback[0]=1;callback[1]=0;__VENEER_WMSP_ReturnResult2Wm9(callback);
}
