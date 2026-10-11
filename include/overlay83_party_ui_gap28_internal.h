#ifndef OVERLAY83_PARTY_UI_GAP28_INTERNAL_H
#define OVERLAY83_PARTY_UI_GAP28_INTERNAL_H
#include "global.h"
void *Save_PlayerData_GetProfile(void *);
void *PlayerProfile_GetNamePtr(void *);
int PlayerProfile_GetTrainerGender(void *);
void *String_New(int, int);
void CopyU16ArrayToString(void *, void *);
void String_Delete(void *);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
int Comm_GetLocalNetId(void *);
void *sub_02034818(int);
void BufferPlayersName(void *, int, void *);
void ov83_0224484C(void *, void *, int, int, int, int, int, int, int, int);
u32 ov83_022447E0(void *, void *, int, int, int, int, int, int, int, int);
int Options_GetFrame(void *);
void ov83_02247944(void *, int);
void ov83_02244AD8(u8 *, void *, int, int, u8);
void ov83_02244B40(u8 *, void *, int, int);
void ov83_02244BA8(void *, void *, int, int, u8, u8);
void ov83_02244BEC(u8 *);
#endif
