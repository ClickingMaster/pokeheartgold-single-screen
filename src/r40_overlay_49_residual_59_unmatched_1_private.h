#ifndef SOL_R40_OVERLAY_49_RESIDUAL_59_UNMATCHED_1_PRIVATE_H
#define SOL_R40_OVERLAY_49_RESIDUAL_59_UNMATCHED_1_PRIVATE_H

#include "global.h"

void *WiFiPlazaRoom_GetObjManager(void *p);
void *ov49_02258D70(void *p, u32 index);
void *WiFiPlazaRoom_GetLobby(void *p);
u32 ov49_0225EF88(void *p);
void ov49_0225A56C(void *p, u32 index);
void ov49_0225EF8C(void *p, u32 state);
int ov45_0222A53C(void *p);
void ov49_0225A53C(void *p, u32 value);
u32 ov49_0225A5AC(void *p, u32 index);
void ov49_02258EEC(void *, void *, int);
u32 ov49_0225A4F0(void *p);
u32 ov49_0225A4E0(void *p);
void ov49_0225A04C(void *p, u32 index, u8 value);
void ov49_0225A54C(void *p);
void *ov49_02259FEC(void *p);
void *ov49_02258DAC(void *p);
u32 ov49_02258E60(void *p, u32 index);
void ov49_0225F334(void *p, u32 a, u16 b, u16 c, u32 d);
void ov45_0222A520(void *p, u32 value);
void ov49_0225EF90(void *p);
u8 ov49_0225A030(void *p);
void ov49_02258EAC(void *p, void *entry, u32 value, u16 arg);
void ov49_0225A018(void *p, u32 value);
u32 ov45_0222A4B8(void *p);
u32 WiFiPlazaRoom_GetMsgString(void *p, u32 a, u32 b);
void WiFiPlazaRoom_PrintMessage(void *p, u32 value);
u32 WiFiPlazaRoom_IsMessageFinished(void *p);
void WiFiPlazaRoom_CloseMessage(void *p);
void *ov49_0225A010(void *p);
u8 ov49_0225A02C(void *p);
void ov49_0225EF98(void *p, u32 value, const void *config, u32 unused);
extern const u8 ov49_02269B38[];
extern const u8 ov49_02269B68[];
u32 ov45_0222D844(void);
void ov45_0222EB94(void);
u32 ov45_0222EBC4(void);
void ov49_0225A37C(void *p, u16 value, u32 unused);
void ov49_0225EFC4(void *p, u32 arg, const void *config, u32 unused);
void ov45_0222A4C8(void *p, u32 value);
void ov45_0222A4D0(void *p);
u32 ov45_0222A424(void *p);

BOOL ov49_0225F448(void *controller, void *work, u32 index);
BOOL ov49_0225FA14(void *controller, void *work, u32 arg);
BOOL ov49_0225FB5C(void *controller, void *work, u32 arg);
BOOL ov49_0225FCA8(void *controller, void *work, u32 arg);

#endif
