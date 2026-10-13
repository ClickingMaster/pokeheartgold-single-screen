#ifndef TO55_T15_PRIVATE_H
#define TO55_T15_PRIVATE_H

#include "nitro/os/alarm.h"
#include "nitro/os/message.h"

#include "global.h"

typedef void *(*Ov13AllocCallback)(u32 size);
typedef void (*Ov13FreeCallback)(void *ptr);

typedef struct Ov13Runtime {
    Ov13AllocCallback alloc;
    void *unk04;
    void *allocation;
    Ov13FreeCallback free;
} Ov13Runtime;

extern Ov13Runtime gWifiSetupRuntime;
extern OSMessageQueue gWifiSetupMessageQueue;
extern OSMessage gWifiSetupMessages[4];

BOOL WifiSetup_OnWcmNotify(void *message);
BOOL WifiSetup_OnTimeoutAlarm(void *message);
BOOL WifiSetupWcm_Init(BOOL (*callback)(void *), void *allocation, u32 size);
BOOL WifiSetupWcm_StartCleanup(void);
BOOL WifiSetupWcm_StartDisconnect(void);
BOOL WifiSetupWcm_StartSearch(u32, u32, u32, u32);
int WifiSetupWcm_CopyApList(void *, u32);
BOOL WifiSetupWcm_StopSearch(void);
void WifiSetup_ConvertBssDesc(const void *, void *);

int WifiSetup_Startup(Ov13AllocCallback alloc, Ov13FreeCallback free);
int WifiSetup_Cleanup(void);
int WifiSetup_DisconnectAccessPoint(void);
int WifiSetup_SearchAccessPoints(void **output);

#endif
