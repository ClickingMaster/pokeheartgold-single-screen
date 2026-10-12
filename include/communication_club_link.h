#ifndef POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H
#define POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H

#include "field_system.h"
#include "script.h"

u8 CommPlayerManager_GetMoveTimer(BOOL arg0);
void sub_020580E4(FieldSystem *fieldSystem, s32 arg1, s32 arg2);
void sub_02058124(FieldSystem *fieldSystem, u8, u32);
void sub_02058164(u16 arg0);
void sub_02058180();
void CommClubLink_StartCardExchange(FieldSystem *fieldSystem);
void sub_02058284(void);
void sub_020582A8(void);
void sub_02058720(void);
BOOL sub_02058740(void);
void CommClubLink_SetCardReceived(void);
void CommClubLink_GetCard(void);
int sub_020881C0(s16 *quantity, u16 a1);

#endif // POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H
