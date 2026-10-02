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
u16 WM_GetNextTgid(void) {RTCTime t;if(_02110FC0==65536){RTC_Init();if(RTC_GetTime(&t)==0)_02110FC0=(u16)(t.second+(t.minute<<8));}_02110FC0=(u16)(_02110FC0+1);return (u16)_02110FC0;}

int WM_Enable(void*cb) {return WMi_EnableEx(cb,0);}

int WMi_EnableEx(void*cb,int x) {int r=WMi_CheckStateEx(1,0);u8*p;if(r)return r;WMi_SetCallbackTable(3,cb);p=WMi_GetSystemWork();r=WMi_SendCommand(3,4,W(p,0),W(p,4),W(p,16),x);if(!r)r=2;return r;}

int WM_Disable(void*cb) {int r=WMi_CheckStateEx(1,1);if(r)return r;WMi_SetCallbackTable(4,cb);r=WMi_SendCommand(4,0);if(!r)r=2;return r;}

int WM_PowerOn(void*cb) {int r=WMi_CheckStateEx(1,1);if(r)return r;WMi_SetCallbackTable(5,cb);r=WMi_SendCommand(5,0);if(!r)r=2;return r;}

int WM_PowerOff(void*cb) {int r=WMi_CheckStateEx(1,2);if(r)return r;WMi_SetCallbackTable(6,cb);r=WMi_SendCommand(6,0);if(!r)r=2;return r;}

int WM_Initialize(void*p,void*cb,int x) {return WMi_InitializeEx(p,cb,x,0);}

int WM_InitializeForListening(void*p,void*cb,int x,int y) {int flags=1;if(!y)flags|=2;return WMi_InitializeEx(p,cb,x,flags);}

int WMi_InitializeEx(void*p,void*cb,int x,int flags) {int r=WM_Init(p,x);u8*s;if(r)return r;WMi_SetCallbackTable(0,cb);s=WMi_GetSystemWork();r=WMi_SendCommand(0,4,W(s,0),W(s,4),W(s,16),flags);if(!r)r=2;return r;}

int WM_Reset(void*cb) {int r=WMi_CheckIdle();if(r)return r;WMi_SetCallbackTable(1,cb);r=WMi_SendCommand(1,0);if(!r)r=2;return r;}

int WM_End(void*cb) {int r=WMi_CheckStateEx(1,2);if(r)return r;WMi_SetCallbackTable(2,cb);r=WMi_SendCommand(2,0);if(!r)r=2;return r;}

int WM_SetParentParameter(void*cb,u8*p) {int r=WMi_CheckStateEx(1,2);u16 mode;if(r)return r;if(!p)return 6;if(H(p,4)&&!P(p,0))return 6;mode=H(p,20);if(H(p,52)+(mode?42:0)>512||H(p,54)+(mode?6:0)>512)return 6;WmCheckParentParameter(p);WMi_SetCallbackTable(7,cb);DC_StoreRange(p,64);if(H(p,4))DC_StoreRange(P(p,0),H(p,4));r=WMi_SendCommand(7,1,p);if(!r)r=2;return r;}

int WmCheckParentParameter(u8*p) {if(H(p,4)>112)return 0;if(H(p,24)<10||H(p,24)>1000)return 0;if(H(p,50)<1||H(p,50)>14)return 0;return 1;}

int WMi_StartParentEx(void*cb,int x) {int r=WMi_CheckStateEx(1,2);u8*p;if(r)return r;p=WMi_GetSystemWork();H(p,0x150)=0;W(p,0x14c)=0;WMi_SetCallbackTable(8,cb);r=WMi_SendCommand(8,1,x);if(!r)r=2;return r;}

int WM_StartParent(void*cb) {return WMi_StartParentEx(cb,1);}

int WM_EndParent(void*cb) {int r=WMi_CheckStateEx(1,7);if(r)return r;WMi_SetCallbackTable(9,cb);r=WMi_SendCommand(9,0);if(!r)r=2;return r;}
