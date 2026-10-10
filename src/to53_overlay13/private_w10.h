#ifndef OVERLAY13_NETWORK_HELPERS_PRIVATE_H
#define OVERLAY13_NETWORK_HELPERS_PRIVATE_H

#include "global.h"

typedef struct Overlay13CipherState {
    u32 x;
    u32 y;
    u8 *table;
    u32 size;
} Overlay13CipherState;

extern void *gWifiSetupRuntime[];
extern void *gWifiSetupFreeHook[];
extern u8 gWifiSetupMessageQueue[];

void MI_CpuCopy8(const void *, void *, u32);
void MI_CpuFill8(void *, u8, u32);
u8 WifiSetup_CipherNextByte(Overlay13CipherState *);
void *WifiSetup_SocketAlloc(s32, s32);
void *WifiSetup_SocketFree(s32, s32);
void WifiSetup_MemCopy(void *, const void *, u32);
void WifiSetup_MemFill(void *, int, u32);
void ov13_022229FC(void);
s32 WifiSetup_StrLen(const char *);
void WifiSetup_ClearPollSet(void *);
void WifiSetup_AddToPollSet(u32, void *);
void ov13_02222B34(const void *, void *);
void ov13_02222BE4(void *, const void *, u32);
BOOL ov13_02222BF4(void *);
BOOL ov13_02222C08(void *);
void WifiSetup_Sleep(s32);
s32 ov13_02223100(s32);

#endif
