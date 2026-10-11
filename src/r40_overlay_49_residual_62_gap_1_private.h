#ifndef SOL40_FOLLOWUP_OV49_R62_GAP1_PRIVATE_H
#define SOL40_FOLLOWUP_OV49_R62_GAP1_PRIVATE_H

#include "global.h"

#pragma require_prototypes off

void *WiFiPlazaRoom_GetLobby(void *);
void *WiFiPlazaRoom_GetMsgString(void *, u32, u32);
void *WiFiPlazaRoom_GetObjManager(void *);
void *WiFiPlazaObj_GetOwnPlayer(void *);
u32 ov45_0222F274(u32);
u32 ov45_0222F294(u32);
u32 ov45_0222F2D4(u32);
u32 FontID_String_GetWidth(u32, void *, u32);

#endif
