#ifndef TO53_H05_PRIVATE_H
#define TO53_H05_PRIVATE_H
#include "global.h"
typedef struct Ov45State { u8 data[0x600]; } Ov45State;
typedef struct Ov45Obj { u8 pad[0x20]; u8 payload[1]; } Ov45Obj;
typedef union Ov45R15Flags { u8 raw; struct { u8 bit0:1; u8 pad:6; u8 bit7:1; } bits; } Ov45R15Flags;
extern void ov45_0222EC10(void *);
extern int WiFiPlazaPlayer_GetCountry(void *);
extern int WiFiPlazaPlayer_GetRegion(void *);
extern void ov45_0222D638(void *, u16, u8, int);
extern void *ov45_0222EA2C(int);
extern int ov45_0222EDC4(u8);
extern u8 ov45_0222EDF0(u8);
extern int ov45_0222E96C(void);
extern u32 ov45_0222AD4C(Ov45State *);
extern int WiFiPlazaProfile_IsChecksumValid(u8 *, void *);
extern int ov45_0222CC50(u8 *, int);
extern int ov45_0222CC7C(u8 *, int);
extern void ov45_0222E9F8(u8 *);
extern void WiFiPlazaProfile_UpdateChecksum(u8 *, void *);
extern void MI_CpuCopy8(const void *, void *, u32);
extern void *Save_PlayerData_GetProfile(void *);
extern void *SaveArray_Party_Get(void *);
extern void *Save_Pokedex_Get(void *);
extern void *Save_WiFiHistory_Get(void *);
extern void *Save_SysInfo_RTC_Get(void *);
extern void *PlayerProfile_GetPlayerName_NewString(void *, int);
extern void CopyStringToU16Array(void *, u16 *, int);
extern void String_Delete(void *);
extern u32 PlayerProfile_GetTrainerID(void *);
extern int Party_GetCount(void *);
extern void *Party_GetMonByIndex(void *, int);
extern u32 GetMonData(void *, int, void *);
extern u8 PlayerProfile_GetTrainerGender(void *);
extern u8 PlayerProfile_GetLanguage(void *);
extern u16 PlayerProfile_GetAvatar(void *);
extern u16 ov45_0222CCE4(u16);
extern u16 WifiHistory_GetPlayerCountry(void *);
extern u8 WiFiHistory_GetPlayerRegion(void *);
extern u8 Pokedex_GetNatDexFlag(void *);
extern u8 PlayerProfile_GetGameClearFlag(void *);
void ov45_0222B79C(Ov45Obj *, u8 *);
void ov45_0222B840(Ov45State *);
void WiFiPlazaProfile_InitFromSave(u8 *, void *, int);
void ov45_0222BA3C(Ov45State *);
#endif
