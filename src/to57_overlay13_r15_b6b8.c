#include "global.h"
#pragma require_prototypes off

extern u8 ov13_0224DEE0[];
extern int WCM_Init(void *, u32);
extern int WCM_CleanupAsync(void);
extern void WCM_Finish(void);
extern int WCM_SearchAsync(void *, void *, int);
extern int WCM_ConnectAsync(void *, void *, int);
extern int WCM_StartupAsync(void *, void (*)(void *));
void ov13_02223114(void *arg);

typedef struct WcmEvent {
    s16 type;
    s16 result;
} WcmEvent;

typedef struct WcmConfig {
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
} WcmConfig;

typedef struct WcmWork {
    void *work;
    int searchArg;
    WcmConfig *startupArg;
    int connectArg;
    void *bssid;
    void *ssid;
    int state;
    void (*callback)();
    void *connectBssid;
    void *connectSsid;
} WcmWork;

#define WCM (*(WcmWork *)ov13_0224DEE0)

BOOL ov13_022236B8(void (*callback)(), void *allocation, u32 size) {
    OSIntrMode irq;
    u32 tmp;
    u32 tmp2;

    irq = OS_DisableInterrupts();
    tmp = (u32)allocation;
    tmp += 0x53;
    tmp &= ~3;
    tmp2 = tmp;
    WCM.connectSsid = allocation;
    WCM.startupArg = (WcmConfig *)tmp;
    tmp2 += 0x2f;
    tmp2 &= ~0x1f;
    WCM.work = (void *)tmp2;
    tmp2 += 0x231f;
    tmp2 &= ~0x1f;
    WCM.connectBssid = (void *)tmp2;
    tmp2 += 0xdf;
    tmp2 &= ~0x1f;
    WCM.startupArg->unk04 = tmp2;
    WCM.startupArg->unk08 = (u32)allocation + size - WCM.startupArg->unk04;
    WCM.startupArg->unk0C = 0;
    WCM.startupArg->unk00 = 3;
    WCM.callback = callback;
    if (WCM.state == 0) {
        if (WCM_Init(WCM.work, 0x2300) != 0) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        WCM.state = 1;
    }
    if (WCM.state == 1) {
        if (WCM_StartupAsync(WCM.startupArg, ov13_02223114) != 3) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        WCM.state = 4;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}
