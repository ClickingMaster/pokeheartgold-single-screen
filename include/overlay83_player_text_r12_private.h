#ifndef POKEHEARTGOLD_OVERLAY83_PLAYER_TEXT_R12_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY83_PLAYER_TEXT_R12_PRIVATE_H
#include "global.h"
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
void *Save_PlayerData_GetProfile(void *);
void *String_New(u32, u32);
const u16 *PlayerProfile_GetNamePtr(void *);
void CopyU16ArrayToString(void *, const u16 *);
u32 PlayerProfile_GetTrainerGender(void *);
u8 AddTextPrinterParameterizedWithColor(void *, u32, void *, u32, u32, u32, u32, void *);
void ScheduleWindowCopyToVram(void *);
void String_Delete(void *);
u32 Comm_GetLocalNetId(void *);
void *sub_02034818(u32);
void BufferPlayersName(void *, u32, void *);
void ov83_0223FCB4(void *, void *, u32, u32, u32, u32, u32, u32, u32, u32);
void ov83_02240C8C(void *, void *, u32, u32, u8);
void ov83_02240CFC(void *, void *, u32, u32);
void ov83_02240D64(void *, void *, u32, u32, u8, u8);
#endif
