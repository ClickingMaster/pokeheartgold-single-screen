#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <stdarg.h>
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define P(p,o) (*(u8**)((u8*)(p)+(o)))
extern u8 _021E4088[],_021E4220[];
extern u16 _02110FBC;
extern u32 _02110FC0;
extern int RTC_GetTime(RTCTime*);
extern int WMi_SendCommand(int,int,...);
extern int WMi_SendCommandDirect(void*,int);
extern int WmInitCore(void*,int,int);
extern int WM_SetPortCallback(int,void*,void*);
extern int WM_StartDataSharing(void*,int,int,int,int);
void CARD_SetSpiReadWaitCycles(u32 x);
void CARD_SetSpiWriteWaitCycles(u32 x);
void CARDi_SpiDummyWait(void);
void CARD_SpiWaitInit(void);
void WMi_SetCallbackTable(int x,void*cb);
u8* WMi_GetSystemWork(void);
int WMi_CheckInitialized(void);
int WMi_CheckIdle(void);
int WMi_CheckStateEx(int count,...);
u8* WMi_GetStatusAddress(void);
u16 WM_GetAID(void);
u16 WM_GetConnectedAIDs(void);
int WM_SetIndCallback(void*cb);
int WM_GetMPSendBufferSize(void);
int WM_GetMPReceiveBufferSize(void);
u16 WM_GetAllowedChannel(void);
u16 WM_GetLinkLevel(void);
u16 WM_GetDispersionBeaconPeriod(void);
u16 WM_GetDispersionScanPeriod(void);
u16 WM_GetNextTgid(void);
int WM_Enable(void*cb);
int WMi_EnableEx(void*cb,int x);
int WM_Disable(void*cb);
int WM_PowerOn(void*cb);
int WM_PowerOff(void*cb);
int WM_End(void*cb);
int WM_EndParent(void*cb);
int WM_EndScan(void*cb);
int WM_Reset(void*cb);
int WM_Initialize(void*p,void*cb,int x);
int WM_InitializeForListening(void*p,void*cb,int x,int y);
int WMi_InitializeEx(void*p,void*cb,int x,int flags);
int WmCheckParentParameter(u8*p);
int WMi_StartParentEx(void*cb,int x);
int WM_StartParent(void*cb);
int WM_EndMP(void*cb);
int WM_EndDCF(void*cb);
int WM_StartDCF(void*cb,void*data,u32 size);
int WM_StartKeySharing(void*p,int x);
int WM_EndKeySharing(void*p);
int WM_SetWEPKey(void*cb,u32 mode,void*key);
int WM_SetWEPKeyEx(void*cb,u32 mode,int x,void*key);
int WM_SetBeaconIndication(void*cb,int x);
int WM_SetLifeTime(void*cb,u16 a,u16 b,u16 c,u16 d);
int WM_MeasureChannel(void*cb,u16 a,u16 b,u16 c,u16 d);
int WM_SetEntry(void*cb,int x);
int WM_Init(void*p,int x);
void WmClearFifoRecvFlag(void);
int WM_ReadStatus(void*out);
int WM_EndDataSharing(u8*p);
int WM_SetParentParameter(void*cb,u8*p);
void WmClearFifoRecvFlag(void) {u16 n=*(vu16*)0x027fff96;if(n&1)*(vu16*)0x027fff96=n&~1;}

u8* WMi_GetStatusAddress(void) {if(WMi_CheckInitialized())return 0;return P(P(_021E4220,4),4);}

u16 WM_GetAID(void) {u16 v;OSIntrMode mode=OS_DisableInterrupts();u8*p=P(_021E4220,4);if(p)v=H(p,0x150);else v=0;OS_RestoreInterrupts(mode);return v;}

u16 WM_GetConnectedAIDs(void) {int v;OSIntrMode mode=OS_DisableInterrupts();u8*p=P(_021E4220,4);if(p)v=W(p,0x14c);else v=0;OS_RestoreInterrupts(mode);return v;}

int WM_SetIndCallback(void*cb) {OSIntrMode mode=OS_DisableInterrupts();int r=WMi_CheckInitialized();if(r){OS_RestoreInterrupts(mode);return r;}P(WMi_GetSystemWork(),0xc8)=cb;OS_RestoreInterrupts(mode);return 0;}
