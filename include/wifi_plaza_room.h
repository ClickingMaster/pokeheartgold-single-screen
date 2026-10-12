#ifndef POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
#define POKEHEARTGOLD_WIFI_PLAZA_ROOM_H

#include "global.h"

#include "pm_string.h"

// OVY_49: the walkable 3D room of the Wi-Fi Plaza, launched by the lobby overlay (OVY_45).

typedef struct WiFiPlazaWork WiFiPlazaWork;         // OVY_45 lobby state (include/wifi_plaza_internal.h)
typedef struct WiFiPlazaRoomWork WiFiPlazaRoomWork; // the room app's work (0x3F8)
typedef struct WiFiPlazaPlayer WiFiPlazaPlayer;     // OVY_45 lobby player

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
BOOL ov45_0222A208(WiFiPlazaWork *lobby);
BOOL ov45_0222A230(WiFiPlazaWork *lobby, int index);
BOOL ov45_0222A25C(WiFiPlazaWork *lobby, int index);
BOOL ov45_0222A2A0(WiFiPlazaWork *lobby, int index);
u32 ov45_0222A2F8(WiFiPlazaWork *lobby);
void ov45_0222A310(WiFiPlazaWork *lobby);
BOOL ov45_0222A424(WiFiPlazaWork *lobby);
int ov45_0222A4B8(WiFiPlazaWork *lobby);
void ov45_0222A4C8(WiFiPlazaWork *lobby, u32 value);
void ov45_0222A4D0(WiFiPlazaWork *lobby);
void ov45_0222A520(WiFiPlazaWork *lobby, int value);
BOOL ov45_0222A550(WiFiPlazaWork *lobby, int index);
void ov45_0222A5E8(WiFiPlazaWork *lobby, int value);
void ov45_0222A704(WiFiPlazaWork *lobby, int index, int value);
void ov45_0222A72C(WiFiPlazaWork *lobby, u32 value);
void ov45_0222A770(WiFiPlazaWork *lobby, int first, int second);
int ov45_0222A920(WiFiPlazaPlayer *player);
BOOL ov45_0222AADC(void);
int ov45_0222AB28(WiFiPlazaWork *lobby, int index);
int ov45_0222AB68(WiFiPlazaWork *lobby);
int ov45_0222AB78(WiFiPlazaWork *lobby, int index);
void ov45_0222AC14(WiFiPlazaWork *lobby, int mode, int value, int player0, int player1, int player2, int player3, int flag);
int ov45_0222AD58(WiFiPlazaWork *lobby, int index);
int ov45_0222ADB8(WiFiPlazaWork *lobby, int index, int choice);
void ov45_0222ADC8(WiFiPlazaWork *lobby, int index);
void ov45_0222AE44(WiFiPlazaWork *lobby);
BOOL ov45_0222AFF8(WiFiPlazaWork *lobby);
BOOL ov45_0222B00C(WiFiPlazaWork *lobby);
u16 ov45_0222B020(WiFiPlazaWork *lobby);
void ov45_0222B0A4(WiFiPlazaWork *lobby);
u32 ov45_0222B0B0(WiFiPlazaWork *lobby);
void ov45_0222B118(WiFiPlazaWork *lobby, int value);
int ov45_0222B1B4(WiFiPlazaWork *lobby);
BOOL ov45_0222D844(void);
BOOL ov45_0222EB38(int kind);
int ov45_0222EB74(void);
BOOL ov45_0222EB94(void);
BOOL ov45_0222EBC4(void);
BOOL ov45_0222EBF0(int kind);
WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *lobby, u32 index);
WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *lobby);
u16 WiFiPlazaPlayer_GetAvatar(WiFiPlazaPlayer *player);
void IncrementGameStat119(WiFiPlazaWork *lobby);

// Room core accessors (OVY_49), until the room header types them.
void *ov49_02259FEC(WiFiPlazaRoomWork *room);
void *ov49_0225A008(WiFiPlazaRoomWork *room);
void *ov49_0225A010(WiFiPlazaRoomWork *room);
void ov49_0225A018(WiFiPlazaRoomWork *room, u32 value);
u8 ov49_0225A02C(WiFiPlazaRoomWork *room);
u8 ov49_0225A030(WiFiPlazaRoomWork *room);
void ov49_0225A034(WiFiPlazaRoomWork *room, u8 value);
void ov49_0225A038(WiFiPlazaRoomWork *room, u8 value);
void ov49_0225A03C(WiFiPlazaRoomWork *room, u8 value);
u8 ov49_0225A040(WiFiPlazaRoomWork *room);
void ov49_0225A044(WiFiPlazaRoomWork *room);
void ov49_0225A04C(WiFiPlazaRoomWork *room, u8 index, u8 value);
u8 ov49_0225A064(WiFiPlazaRoomWork *room, u32 index);
void ov49_0225A06C(WiFiPlazaRoomWork *room, u8 index, u8 value);
u8 ov49_0225A084(WiFiPlazaRoomWork *room, u32 index);
void ov49_0225A09C(WiFiPlazaRoomWork *room, String *message);
void ov49_0225A0BC(WiFiPlazaRoomWork *room);
void ov49_0225A0CC(WiFiPlazaRoomWork *room);
void ov49_0225A10C(WiFiPlazaRoomWork *room, u32 count);
void ov49_0225A134(WiFiPlazaRoomWork *room);
void ov49_0225A144(WiFiPlazaRoomWork *room, String *string, u32 value);
void *ov49_0225A154(WiFiPlazaRoomWork *room);
void ov49_0225A174(WiFiPlazaRoomWork *room, void *listTemplate, u32 a, u32 b);
u32 ov49_0225A1D4(WiFiPlazaRoomWork *room);
void ov49_0225A1E4(WiFiPlazaRoomWork *room, u32 a, u32 b);
void ov49_0225A1F4(WiFiPlazaRoomWork *room, u32 a);
void ov49_0225A294(WiFiPlazaRoomWork *room);
void ov49_0225A334(WiFiPlazaRoomWork *room, u32 index, u32 a);
void ov49_0225A37C(WiFiPlazaRoomWork *room, u32 a, u32 b);
void ov49_0225A39C(WiFiPlazaRoomWork *room, u32 a, u32 b);
void ov49_0225A40C(WiFiPlazaRoomWork *room, u32 a, u32 b);
void ov49_0225A428(WiFiPlazaRoomWork *room, u32 index, u32 a);
void ov49_0225A490(WiFiPlazaRoomWork *room);
void ov49_0225A4D0(WiFiPlazaRoomWork *room);
u32 ov49_0225A4E0(WiFiPlazaRoomWork *room);
BOOL ov49_0225A4F0(WiFiPlazaRoomWork *room);
BOOL ov49_0225A500(WiFiPlazaRoomWork *room);
void ov49_0225A510(WiFiPlazaRoomWork *room);
BOOL ov49_0225A520(WiFiPlazaRoomWork *room, u32 mover);
void ov49_0225A530(WiFiPlazaRoomWork *room);
void ov49_0225A53C(WiFiPlazaRoomWork *room, u16 value);
void ov49_0225A54C(WiFiPlazaRoomWork *room);
BOOL ov49_0225A55C(WiFiPlazaRoomWork *room);
void ov49_0225A56C(WiFiPlazaRoomWork *room, u32 index);
BOOL ov49_0225A5AC(WiFiPlazaRoomWork *room, u32 index);

// Other OVY_49 modules, until their own headers exist.
// object manager (avatars)
#ifndef POKEHEARTGOLD_WIFI_PLAZA_OBJ_H // typed in wifi_plaza_obj.h
void WiFiPlazaObj_SetLightSet(void *objManager, int lightSet);
void WiFiPlazaObj_FadeToLightSet(void *objManager, int lightSet);
void *WiFiPlazaObj_GetOwnPlayer(void *objManager);
void *WiFiPlazaObj_GetSpecial(void *objManager);
u32 WiFiPlazaObj_GetParam(void *obj, int param);
void ov49_02258EAC(void *objManager, void *obj, u16 value, u8 kind);
void WiFiPlazaObj_SetHandler(void *objManager, void *obj, int enable);
void ov49_02259130(void *obj, int enable);
void WiFiPlazaObj_SetDirection(void *obj, u32 direction);
#endif
// scenery models
void WiFiPlazaModels_GetMoverFrontPos(void *models, int index, int side, VecFx32 *pos);
void WiFiPlazaModels_TouchPropsAt(void *models, u8 x, u8 y);
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
#ifndef POKEHEARTGOLD_WIFI_PLAZA_PLAYER_STATE_H // typed in wifi_plaza_player_state.h
void *WiFiPlazaPlayerState_AllocWork(void *state, u32 size);
void WiFiPlazaPlayerState_FreeWork(void *state);
void *WiFiPlazaPlayerState_GetWork(void *state);
u32 WiFiPlazaPlayerState_GetStep(void *state);
void WiFiPlazaPlayerState_SetStep(void *state, u32 step);
#endif

#endif // POKEHEARTGOLD_WIFI_PLAZA_ROOM_H
