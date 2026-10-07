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
int WCM_GetPhase(void) {OSIntrMode m=OS_DisableInterrupts();int r=0;if(_021D5FEC)r=_021D5FEC->phase;OS_RestoreInterrupts(m);return r;}

u32 WCM_UpdateOption(u32 x) {OSIntrMode m=OS_DisableInterrupts();u32 mask=0;W*w=_021D5FEC;u32 old=w->option;if(!w){OS_RestoreInterrupts(m);return 0;}if(x&0x8000){mask|=0x3ffe;if(!(x&0x3ffe))x|=0xa082;}if(x&0x20000)mask|=0x10000;if(x&0x80000)mask|=0x40000;if(x&0x200000)mask|=0x100000;if(x&0x800000)mask|=0x400000;w->option=x|(old&~mask);OS_RestoreInterrupts(m);return old;}

W* WCMi_GetSystemWork(void) {return _021D5FEC;}

void WcmConfigure(u32*p,void(*cb)(Msg*)) {if(!p){_021D5FEC->x26c=3;_021D5FEC->list=0;_021D5FEC->size=0;_021D5FEC->mode=0;}else{_021D5FEC->x26c=p[0]&3;if((((4-(p[1]&3))&3)+12)>p[2]){_021D5FEC->list=0;_021D5FEC->size=0;}else{_021D5FEC->list=(L*)((p[1]+3)&~3);_021D5FEC->size=p[2]-((4-(p[1]&3))&3);MI_CpuFill8(_021D5FEC->list,0,_021D5FEC->size);}_021D5FEC->mode=p[3];}_021D5FEC->callback=cb;}
