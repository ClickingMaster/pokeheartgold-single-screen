#ifndef POKEHEARTGOLD_OVERLAY_00_THUMB_H
#define POKEHEARTGOLD_OVERLAY_00_THUMB_H

#include "overlay_44_02232E9C.h"

s32 WifiConnection_ClassifyErrorCode(u32 arg0, u32 arg1);
int WifiConnection_GetTargetFriendIndex(void);
void WifiConnection_SetDisconnectWhenAlone(s32 arg0);
s32 WifiConnection_StopServerMatching(void);
void WifiConnection_ResetStopServerMatching(void);
void WifiConnection_SetDisconnectCallback(void *arg0, void *arg1);
void WifiConnection_StopVoiceChat(void);
void WifiConnection_StartVoiceChat(s32 arg0);
void WifiConnection_SetOwnStatusData(void *arg0, s32 arg1);
void WifiConnection_SetVoiceChatEnabled(u8 arg0);
void DWC_SetAuthServer(s32 arg0);
void WifiConnection_SetExtraHeapEnabled(s32 arg0, s32 arg1);
s32 WifiConnection_GetFriendStatus(s32 arg0);
s32 WifiConnection_GetNewClientIndex(void);
BOOL WifiConnection_GetVoiceActivity(void);
BOOL WifiConnection_IsVoiceChatEnabled(void);
void WifiConnection_SetConnectFilterCallback(BOOL (*arg0)(s32));
void WifiConnection_ClearFriendDataChanged(void);
BOOL WifiConnection_IsFriendDataChanged(void);
void WifiConnection_SetFriendStatusBuffer(UnkStruct_ov44_02231958 *arg0, s32 arg1);

#endif // POKEHEARTGOLD_OVERLAY_00_THUMB_H
