#ifndef POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
#define POKEHEARTGOLD_WIFI_PLAZA_ROOM_H

#include "global.h"

#include "pm_string.h"

// OVY_49: the walkable 3D room of the Wi-Fi Plaza, launched by the lobby overlay (OVY_45).

typedef struct WiFiPlazaWork WiFiPlazaWork;         // OVY_45 lobby state (include/wifi_plaza_internal.h)
typedef struct WiFiPlazaRoomWork WiFiPlazaRoomWork; // the room app's work (0x3F8)

WiFiPlazaWork *WiFiPlazaRoom_GetLobby(WiFiPlazaRoomWork *room);
void *WiFiPlazaRoom_GetObjManager(WiFiPlazaRoomWork *room);
void *WiFiPlazaRoom_GetModels(WiFiPlazaRoomWork *room);
void *WiFiPlazaRoom_GetMap(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_PrintMessage(WiFiPlazaRoomWork *room, String *message);
BOOL WiFiPlazaRoom_IsMessageFinished(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_CloseMessage(WiFiPlazaRoomWork *room);
void WiFiPlazaRoom_OpenYesNoMenu(WiFiPlazaRoomWork *room);
u32 WiFiPlazaRoom_GetYesNoResult(WiFiPlazaRoomWork *room); // 0 yes, 1 no / cancelled, 2 still open
void WiFiPlazaRoom_CloseYesNoMenu(WiFiPlazaRoomWork *room);
String *WiFiPlazaRoom_GetMsgString(WiFiPlazaRoomWork *room, int bank, int msgId);

// Lobby (OVY_45) calls, with the types this overlay uses them with (several return values that
// wifi_plaza_internal.h declares as void).
BOOL ov45_0222A288(WiFiPlazaWork *lobby, int bit);
u32 ov45_0222A2E0(WiFiPlazaWork *lobby);
u32 ov45_0222A324(WiFiPlazaWork *lobby);
u32 ov45_0222A330(WiFiPlazaWork *lobby);
u32 ov45_0222A35C(WiFiPlazaWork *lobby);
BOOL ov45_0222A374(WiFiPlazaWork *lobby);
u32 ov45_0222A394(WiFiPlazaWork *lobby);
BOOL ov45_0222A3A0(WiFiPlazaWork *lobby);
u32 ov45_0222A3BC(WiFiPlazaWork *lobby);
u32 ov45_0222A3D4(WiFiPlazaWork *lobby);
u32 ov45_0222A3EC(WiFiPlazaWork *lobby);
int ov45_0222A53C(WiFiPlazaWork *lobby);
int ov45_0222AD2C(WiFiPlazaWork *lobby);
int ov45_0222AD3C(WiFiPlazaWork *lobby);
int ov45_0222AD80(WiFiPlazaWork *lobby, int index);
int ov45_0222AD90(WiFiPlazaWork *lobby, int index);
int ov45_0222ADA0(WiFiPlazaWork *lobby);
int ov45_0222ADA8(WiFiPlazaWork *lobby, int index);
int ov45_0222ADE8(WiFiPlazaWork *lobby, int index);
void ov45_0222ADF8(WiFiPlazaWork *lobby, int index);
void ov45_0222AE08(u32 value, u32 *quotient, u32 *remainder);
u32 ov45_0222B1DC(WiFiPlazaWork *lobby);
u32 ov45_0222B1EC(WiFiPlazaWork *lobby);

// Other OVY_49 modules, until their own headers exist.
// object manager (avatars)
void WiFiPlazaObj_SetLightSet(void *objManager, int lightSet);
void WiFiPlazaObj_FadeToLightSet(void *objManager, int lightSet);
void *WiFiPlazaObj_GetOwnPlayer(void *objManager);
void *WiFiPlazaObj_GetSpecial(void *objManager);
u32 WiFiPlazaObj_GetParam(void *obj, int param);
void ov49_02258EAC(void *objManager, void *obj, u16 value, u8 kind);
void WiFiPlazaObj_SetHandler(void *objManager, void *obj, int enable);
void ov49_02259130(void *obj, int enable);
void WiFiPlazaObj_SetDirection(void *obj, u32 direction);
// scenery models
void WiFiPlazaModels_SetMoverX(void *models, int index, fx32 x);
void WiFiPlazaModels_SetMoverOffset(void *models, int index, const VecFx32 *offset);
int WiFiPlazaModels_StartMoverAnim1(void *models, int index);
int WiFiPlazaModels_StartMoverAnim3(void *models, int index);
void WiFiPlazaModels_SetMoverVisible(void *models, int index, int visible);
void WiFiPlazaModels_SetMoverAngle(void *models, int index, int angle);
u32 WiFiPlazaModels_IsMoverAnim1Playing(void *models, int index);
u32 WiFiPlazaModels_IsMoverAnim3Playing(void *models, int index);
void WiFiPlazaModels_StartBaseAnim4(void *models);
void WiFiPlazaModels_StopBaseAnim4(void *models);
void WiFiPlazaModels_SetEffectActive(void *models, int enable);
void WiFiPlazaModels_TriggerStageBursts(void *models, u32 id);
void WiFiPlazaModels_EnableKind1To4Lights(void *models);
void WiFiPlazaModels_SetPairAnim(void *models, u32 mode);
u32 WiFiPlazaModels_GetPairAnimState(void *models);
void WiFiPlazaModels_ShowEffectProps(void *models, int show, int scale);
int WiFiPlazaModels_ShowEffectProp(void *models, u8 id, int show, int scale);
void WiFiPlazaModels_TriggerStageShows(void *models, u32 id);
// player state handlers
void *ov49_0225EF40(void *state, u32 size);
void ov49_0225EF68(void *state);
void *ov49_0225EF84(void *state);
u32 ov49_0225EF88(void *state);
void ov49_0225EF8C(void *state, u32 step);

#endif // POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
