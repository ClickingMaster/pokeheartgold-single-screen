#ifndef TO50_OV49_R59_U1_PRIVATE_H
#define TO50_OV49_R59_U1_PRIVATE_H
#include "global.h"

#include "sound.h"
#include "system.h"

#define U16_AT(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define U32_AT(p, n) (*(u32 *)((u8 *)(p) + (n)))

typedef struct Ov49F518Work {
    u16 nextState;
    u16 selected;
    u32 timer;
    u8 menu[0x20];
    u16 first;
    u16 second;
} Ov49F518Work;

#pragma require_prototypes off
void *WiFiPlazaRoom_GetLobby(void *);
void *WiFiPlazaRoom_GetObjManager(void *);
void *ov49_02258DAC(void *);
void *ov49_0225EF84(void *);
u8 *ov49_0225EF40(void *, u32);
u32 ov49_0225EF88(void *);
u32 WiFiPlazaRoom_GetMsgString(void *, u32, u32);
void *ov49_0225A008(void *);
void *ov49_0225A010(void *);
u32 ov49_0225A02C(void *);
u32 ov49_0225A1D4(void *);
u32 WiFiPlazaRoom_GetYesNoResult(void *);
u32 WiFiPlazaRoom_IsMessageFinished(void *);
u32 ov49_02258F38(void *);
extern const u8 ov49_02269B38[];
BOOL ov49_0225F518(void *, void *, u32);

#endif
