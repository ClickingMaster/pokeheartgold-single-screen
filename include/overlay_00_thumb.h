#ifndef POKEHEARTGOLD_OVERLAY_00_THUMB_H
#define POKEHEARTGOLD_OVERLAY_00_THUMB_H

#include "overlay_44_02232E9C.h"

s32 WifiConnection_ClassifyErrorCode(u32 arg0, u32 arg1);
int WifiConnection_GetTargetFriendIndex(void);
void ov00_021E7220(s32 arg0);
s32 ov00_021E70F4(void);
void ov00_021E7130(void);
void WifiConnection_SetDisconnectCallback(void *arg0, void *arg1);
void WifiConnection_StopVoiceChat(void);
void WifiConnection_StartVoiceChat(s32 arg0);
void ov00_021E6D60(void *arg0, s32 arg1);
void ov00_021E70B8(u8 arg0);
void ov00_021EC454(s32 arg0);
void WifiConnection_SetExtraHeapEnabled(s32 arg0, s32 arg1);
s32 WifiConnection_GetFriendStatus(s32 arg0);
s32 ov00_021E709C(void);
BOOL WifiConnection_GetVoiceActivity(void);
BOOL ov00_021E7080(void);
void WifiConnection_SetConnectFilterCallback(BOOL (*arg0)(s32));
void ov00_021E7328(void);
BOOL ov00_021E7314(void);
void WifiConnection_SetFriendStatusBuffer(UnkStruct_ov44_02231958 *arg0, s32 arg1);

#endif // POKEHEARTGOLD_OVERLAY_00_THUMB_H
