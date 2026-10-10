#pragma force_active on
#include "global.h"

#include "WIFI_stubsmd5_dummy.h"
#include "dgt.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct A {
    u8 used, pad;
    u16 v;
    u32 index;
    struct A *prev, *next;
    u8 data[192];
} A;

typedef struct {
    u32 n;
    A *first, *last;
} L;

typedef struct {
    u16 a, b;
    void *c, *d;
    int e;
} Msg;

typedef struct {
    u8 pad[0x2260];
    int phase;
    u32 option;
    u16 x268;
    u8 lock, reset;
    u32 x26c;
    L *list;
    u32 size;
    u32 mode;
    void (*callback)(Msg *);
    s16 notify;
    u16 aid;
} W;

typedef void (*Recv)(void *, void *, void *, u32);

typedef struct {
    u8 init, pad[3];
    u32 x4, x8;
    u8 padc[24];
    u32 result;
    Recv recv;
} I;

extern W *_021D5FEC;
extern I _021D5FF0;
extern u8 _021D5FFC[];
extern u8 _021D5FF4[];
extern u8 _021D6020[];
extern u8 _021D601C;

extern int WCM_BeginSearchAsync();
extern int WM_Disconnect();
extern int WM_EndDCF();
extern int WM_PowerOff();
extern int WM_Reset();
extern int WM_SetDCFData();
extern int WcmWmcbCommon();
extern int WcmWmcbReset();
void CPSi_big_from_char(u16 *p, u8 *q, int n, int cap);
void CPSi_char_from_big(u8 *p, u16 *q, int n);
int WCM_Finish(void);
int WCM_SearchAsync(void *a, void *b, int c);
int WCM_EndSearchAsync(void);
int WCM_GetPhase(void);
u32 WCM_UpdateOption(u32 x);
W *WCMi_GetSystemWork(void);
void WcmInitOption(void);
u16 WcmGetNextScanChannel(int x);
void WcmNotify(int a, void *b, void *c, int d);
void WcmNotifyEx(int a, int b, void *c, void *d, int e);
void WcmSetPhase(int x);
void WCMi_ResetKeepAliveAlarm(void);
void WcmKeepAliveAlarm(void *p);
void WcmWmReset(void);
void WcmWmcbIndication(u16 *p);
void WcmWmcbEndScan(u16 *p);
void WcmWmcbDisconnect(u16 *p);
void WCM_ClearApList(void);
int WCM_CountApList(void);
int WCM_LockApList(int x);
void *WCM_PointApList(int i);
void WCMi_EntryApList(u16 *p, int n);
A *WcmGetOldestApList(void);
A *WcmSearchApList(u8 *p);
A *WcmSearchIndexedApList(u32 i);
void WcmAppendApList(A *a);
void WCMi_InitCpsif(void);
void WCMi_CpsifRecvCallback(u8 *p);
void WCMi_CpsifSendNullPacket(void);
u8 *WCM_GetApMacAddress(void);
u8 *WCM_GetApEssid(u16 *out);
void WCM_SetRecvDCFCallback(Recv p);
void WcmCpsifWmCallback(u16 *p);
void WcmCpsifKACallback(void *p);
int WcmCpsifTryLockMutexInIRQ(OSMutex *p);
void WcmCpsifUnlockMutexInIRQ(OSMutex *p);
int WCM_CompareBssID(u8 *p, u8 *q);
int WCM_GetLinkLevel(void);
u8 WCMi_GetRssiAverage(void);
int WcmGetLinkLevel(void);
void WCMi_ShelterRssi(int x);
int WCM_Init(W *p, u32 n);
int WCM_CleanupAsync(void);
void WcmConfigure(u32 *p, void (*cb)(Msg *));
A *WcmAllocApList(void);
int WCM_DisconnectAsync(void);
void WcmWmcbStartDCF(u16 *p);
void WcmWmcbEndDCF(u16 *p);

void WCM_ClearApList(void) {
    OSIntrMode m = OS_DisableInterrupts();
    W *w = WCMi_GetSystemWork();
    if (!w) {
        OS_RestoreInterrupts(m);
        return;
    }
    if (w->list && (int)w->size > 0) {
        MI_CpuFill8(w->list, 0, w->size);
    }
    OS_RestoreInterrupts(m);
}

int WCM_CountApList(void) {
    OSIntrMode m = OS_DisableInterrupts();
    W *w = WCMi_GetSystemWork();
    int n = 0;
    if (!w) {
        OS_RestoreInterrupts(m);
        return n;
    }
    if (w->list && w->size > 12) {
        n = w->list->n;
    }
    OS_RestoreInterrupts(m);
    return n;
}

int WCM_LockApList(int x) {
    OSIntrMode m = OS_DisableInterrupts();
    W *w = WCMi_GetSystemWork();
    if (!w) {
        OS_RestoreInterrupts(m);
        return 0;
    }
    if (x) {
        x = w->lock != 0;
        w->lock = 1;
    } else {
        x = w->lock != 0;
        w->lock = 0;
    }
    OS_RestoreInterrupts(m);
    return x;
}

void *WCM_PointApList(int i) {
    OSIntrMode m = OS_DisableInterrupts();
    W *w = WCMi_GetSystemWork();
    A *a;
    if (!w) {
        OS_RestoreInterrupts(m);
        return 0;
    }
    a = WcmSearchIndexedApList(i);
    if (!a) {
        OS_RestoreInterrupts(m);
        return 0;
    }
    OS_RestoreInterrupts(m);
    return (u8 *)a + 16;
}

void WCMi_EntryApList(u16 *p, int n) {
    W *w = WCMi_GetSystemWork();
    A *a;
    if (!w) {
        return;
    }
    if (w->lock || p[30]) {
        return;
    }
    a = WcmSearchApList((u8 *)p + 4);
    if (!a) {
        a = WcmAllocApList();
    }
    if (!a && w->mode == 1) {
        a = WcmGetOldestApList();
    }
    if (!a) {
        return;
    }
    a->v = n;
    MIi_CpuCopyFast((u32 *)p, (u32 *)((u8 *)a + 16), 192);
    WcmAppendApList(a);
}
