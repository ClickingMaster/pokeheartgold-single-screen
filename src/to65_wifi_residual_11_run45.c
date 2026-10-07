#pragma force_active on
// clang-format off
#include "global.h"
#include "dgt.h"
#include "WIFI_stubsmd5_dummy.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct A {u8 used,pad;u16 v;u32 index;struct A*prev,*next;u8 data[192];} A;
typedef struct {u32 n;A*first,*last;} L;
typedef struct {u16 a,b;void*c,*d;int e;} Msg;
typedef struct {u8 pad[0x2260];int phase;u32 option;u16 x268;u8 lock,reset;u32 x26c;L*list;u32 size;u32 mode;void(*callback)(Msg*);s16 notify;u16 aid;} W;
typedef void(*Recv)(void*,void*,void*,u32);
typedef struct {u8 init,pad[3];u32 x4,x8;u8 padc[24];u32 result;Recv recv;} I;
extern W*_021D5FEC;
extern I _021D5FF0;
extern u8 _021D5FFC[],_021D5FF4[],_021D6020[],_021D601C;

extern int WCM_BeginSearchAsync();
extern int WM_Disconnect();
extern int WM_EndDCF();
extern int WM_PowerOff();
extern int WM_Reset();
extern int WM_SetDCFData();
extern int WcmWmcbCommon();
extern int WcmWmcbReset();
void CPSi_big_from_char(u16*p,u8*q,int n,int cap);
void CPSi_char_from_big(u8*p,u16*q,int n);
int WCM_Finish(void);
int WCM_SearchAsync(void*a,void*b,int c);
int WCM_EndSearchAsync(void);
int WCM_GetPhase(void);
u32 WCM_UpdateOption(u32 x);
W* WCMi_GetSystemWork(void);
void WcmInitOption(void);
u16 WcmGetNextScanChannel(int x);
void WcmNotify(int a,void*b,void*c,int d);
void WcmNotifyEx(int a,int b,void*c,void*d,int e);
void WcmSetPhase(int x);
void WCMi_ResetKeepAliveAlarm(void);
void WcmKeepAliveAlarm(void*p);
void WcmWmReset(void);
void WcmWmcbIndication(u16*p);
void WcmWmcbEndScan(u16*p);
void WcmWmcbDisconnect(u16*p);
void WCM_ClearApList(void);
int WCM_CountApList(void);
int WCM_LockApList(int x);
void* WCM_PointApList(int i);
void WCMi_EntryApList(u16*p,int n);
A* WcmGetOldestApList(void);
A* WcmSearchApList(u8*p);
A* WcmSearchIndexedApList(u32 i);
void WcmAppendApList(A*a);
void WCMi_InitCpsif(void);
void WCMi_CpsifRecvCallback(u8*p);
void WCMi_CpsifSendNullPacket(void);
u8* WCM_GetApMacAddress(void);
u8* WCM_GetApEssid(u16*out);
void WCM_SetRecvDCFCallback(Recv p);
void WcmCpsifWmCallback(u16*p);
void WcmCpsifKACallback(void*p);
int WcmCpsifTryLockMutexInIRQ(OSMutex*p);
void WcmCpsifUnlockMutexInIRQ(OSMutex*p);
int WCM_CompareBssID(u8*p,u8*q);
int WCM_GetLinkLevel(void);
u8 WCMi_GetRssiAverage(void);
int WcmGetLinkLevel(void);
void WCMi_ShelterRssi(int x);
int WCM_Init(W*p,u32 n);
int WCM_CleanupAsync(void);
void WcmConfigure(u32*p,void(*cb)(Msg*));
A* WcmAllocApList(void);
int WCM_DisconnectAsync(void);
void WcmWmcbStartDCF(u16*p);
void WcmWmcbEndDCF(u16*p);
void CPSi_big_from_char(u16*p,u8*q,int n,int cap) {MI_CpuFill8(p,0,cap*2);q+=n-1;while(n>1){*p++=*q+q[-1]*256;n-=2;q-=2;}if(n>0)*p=*q;}

void CPSi_char_from_big(u8*p,u16*q,int n) {p+=n-1;while(n>1){u8*z=p-1;*p=*q;p-=2;*z=*q++>>8;n-=2;}if(n>0)*p=*q;}

int WCM_Init(W*p,u32 n) {OSIntrMode m=OS_DisableInterrupts();if(_021D5FEC){OS_RestoreInterrupts(m);return 1;}if(!p){OS_RestoreInterrupts(m);return 1;}if((u32)p&31){OS_RestoreInterrupts(m);return 1;}if(n<0x2300){OS_RestoreInterrupts(m);return 6;}_021D5FEC=p;p->phase=1;_021D5FEC->notify=0;_021D5FEC->x268=0;_021D5FEC->lock=0;_021D5FEC->reset=0;_021D5FEC->aid=0;*(u16*)((u8*)_021D5FEC+0x22f8)=0;WcmInitOption();WCMi_InitCpsif();if(!OS_IsTickAvailable())OS_InitTick();if(!OS_IsAlarmAvailable())OS_InitAlarm();OS_CreateAlarm((OSAlarm*)((u8*)_021D5FEC+0x22cc));OS_RestoreInterrupts(m);return 0;}

int WCM_Finish(void) {OSIntrMode m=OS_DisableInterrupts();if(!_021D5FEC){OS_RestoreInterrupts(m);return 1;}if(_021D5FEC->phase!=1){OS_RestoreInterrupts(m);return 1;}_021D5FEC=0;OS_RestoreInterrupts(m);return 0;}
