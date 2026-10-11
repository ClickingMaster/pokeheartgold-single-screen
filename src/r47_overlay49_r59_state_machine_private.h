#ifndef TO47_OV49_R59_UNMATCHED2_PRIVATE_H
#define TO47_OV49_R59_UNMATCHED2_PRIVATE_H
#include "global.h"

#include "overlay_42.h"

typedef struct Ov49Pair16 {
    s16 x;
    s16 y;
} Ov49Pair16;
typedef struct Ov49ResultR59 {
    s16 x;
    s16 y;
    u16 value;
    u16 kind;
    u16 state;
} Ov49ResultR59;

void *ov49_0225EF84(void *);
void *ov49_0225A010(void *);
void *WiFiPlazaRoom_GetObjManager(void *);
void *WiFiPlazaObj_GetOwnPlayer(void *);
void *WiFiPlazaRoom_GetLobby(void *);
u32 ov49_0225EF88(void *);
u8 *ov49_0225EF40(void *, u32);
void WiFiPlazaObj_SetHandler(void *, void *, int);
void ov49_0225EF90(void *);
u32 ov49_0225A040(void *);
void ov49_0225EFC4(void *, u32, const void *, void *);
void ov49_0225EF8C(void *, u32);
void ov49_0225A034(void *, u32);
void ov49_0225A038(void *, u8);
void IncrementGameStat119(void *);
void ov45_0222B118(void *, u32);
u32 WiFiPlazaObj_GetParam(void *, u32);
void *ov49_02259FEC(void *);
u32 ov49_02258E34(void *);
void ov45_0222A4C8(void *, u32);
void ov49_02258EAC(void *, void *, u32, u32);
void ov49_0225A37C(void *, u32, u32);
u32 WiFiPlazaRoom_GetMsgString(void *, u32, u32);
void WiFiPlazaRoom_PrintMessage(void *, u32);
u32 WiFiPlazaRoom_IsMessageFinished(void *);
void WiFiPlazaRoom_CloseMessage(void *);
void ov49_0225EF68(void *);
void ov45_0222A5E8(void *, u32);
void ov49_0225EF98(void *, u32, const void *, u32);
extern const u8 ov49_02269B38[];
extern const u8 ov49_02269B88[];
BOOL ov49_022607C4(void *, void *, u32);
#endif
