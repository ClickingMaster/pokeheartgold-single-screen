#ifndef POKEHEARTGOLD_WIRELESS_MANAGER_H
#define POKEHEARTGOLD_WIRELESS_MANAGER_H

#include "global.h"

u16 Wireless_GetConnectedBitmap(void);
u32 Wireless_GetError(void);
BOOL Wireless_StartMeasureChannels(void);
u16 Wireless_SelectChannel(void);
BOOL Wireless_Init(void *, int);
u32 Wireless_GetWorkSize(void);
BOOL Wireless_StartParent(int, u16, u16, u16, u16, u32);
BOOL Wireless_StartChild(int, const void *);
void Wireless_SetReceiveCallback(void *, int);
void Wireless_Stop(void);
BOOL Wireless_End(void);
BOOL Wireless_IsIdle(void);
BOOL Wireless_IsBusy(void);
BOOL Wireless_HasError(void);
BOOL Wireless_HasFatalError(void);
BOOL Wireless_IsScanning(void);
void Wireless_UpdateGameInfo(const void *, int, u32, int);
BOOL Wireless_HasSentBeacons(void);
void Wireless_ClearBeaconCount(void);
void Wireless_SetBeaconCallback(void *);
u8 Wireless_IsEntryLocked(void);
BOOL Wireless_ScanAndConnect(int, const u16 *, u16);
BOOL Wireless_StartScan(void *, const u16 *, u16);
BOOL Wireless_StopScan(void);
void Wireless_SetGgid(u32);
void Wireless_SetParentGameInfo(void *, u16);

#endif // POKEHEARTGOLD_WIRELESS_MANAGER_H
