#ifndef OVERLAY49_BATTLE_HUD_STATE_R59_PRIVATE_H
#define OVERLAY49_BATTLE_HUD_STATE_R59_PRIVATE_H

#include "global.h"

u32 ov49_0225EF88(void *);
void *WiFiPlazaRoom_GetObjManager(void *);
void *WiFiPlazaObj_GetOwnPlayer(void *);
void WiFiPlazaObj_SetHandler(void *, void *, int);
void *WiFiPlazaRoom_GetLobby(void *);
void ov45_0222A5E8(void *, u32);
void PlaySE(u32);
u32 WiFiPlazaRoom_GetMsgString(void *, u32, u32);
void WiFiPlazaRoom_PrintMessage(void *, u32);
void ov49_0225EF90(void *);
u32 WiFiPlazaRoom_IsMessageFinished(void *);
void ov49_0225A294(void *);
u32 WiFiPlazaRoom_GetYesNoResult(void *);
void ov49_0225EF8C(void *, u32);
void WiFiPlazaRoom_CloseYesNoMenu(void *);
void WiFiPlazaRoom_CloseMessage(void *);
void ov49_0225A018(void *, u32);
void *ov49_0225A008(void *);
void WiFiPlazaCamera_ClearFollowObject(void *);
u32 WiFiPlazaObj_IsFinished(void *);
void ov49_0225A034(void *, u32);
void ov49_0225A038(void *, u32);
void ov49_02258EAC(void *, void *, u32, u32);
void *ov49_0225A010(void *);
u32 WiFiPlazaObj_GetParam(void *, u32);
void ov49_0225EF98(void *, u32, const void *, u32);
extern const u8 ov49_02269B38[];
BOOL ov49_02260254(void *, void *, u32);

#endif
