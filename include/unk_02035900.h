#ifndef POKEHEARTGOLD_UNK_02035900_H
#define POKEHEARTGOLD_UNK_02035900_H

#include "global.h"

#include "safari_zone.h"

BOOL Comm_Update(void);
BOOL Comm_IsNetIdConnected(u16 arg0);
BOOL Comm_SendCommand(int arg0, void *arg1, int arg2);
int Comm_CountConnected(void);
BOOL sub_02037474(void);
void sub_020374E4(void);
void sub_02037504(void);
BOOL Comm_GetLocalNetId(void);
void sub_020376D4(s32 arg0, SafariZoneAreaSet *areaSet);
void sub_020376EC(int arg0);
BOOL sub_02037700();
int sub_0203774C(u32 arg0);
int sub_0203775C(u32 arg0);
int sub_020378AC(int arg0);
void sub_020380F4(void);
void CommState_SetUnionRoomLinkMode(void);
int sub_02037824(int arg0);
BOOL sub_02037880(void);
int sub_020378AC(int);
BOOL sub_02037958(void);
void sub_020380F4(void);
void CommState_SetUnionRoomLinkMode(void);

#endif // POKEHEARTGOLD_UNK_02035900_H
