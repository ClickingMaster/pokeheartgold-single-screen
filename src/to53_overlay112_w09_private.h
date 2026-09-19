#ifndef TO53_R3_W09_PRIVATE_H
#define TO53_R3_W09_PRIVATE_H
#include "global.h"

typedef struct String String;
typedef struct Window Window;

u32 Boxmon_GetIconNaix(void *);
u32 GetMonIconNaixEx(void *, u32, u32);
void ToggleBgLayer(u32, u8);
void GF_AssertFail(void);
void *TouchscreenHitbox_FindRectAtTouchNew(const void *);
void *Save_PlayerData_GetProfile(void *);
String *PlayerProfile_GetPlayerName_NewString(void *, u32);
void FontID_Alloc(u32, u32);
void FontID_Release(u32);
void String_Delete(String *);
void AddTextPrinterParameterizedWithColor(Window *, u32, String *, u32, u32, u32, u32, u32);
void *NARC_New(u32, u32);
void NARC_Delete(void *);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, u32, void *, u32, u32, u32, u32, u32);
void FillWindowPixelBuffer(Window *, u8);
void StringExpandPlaceholders(void *, String *, String *);
String *NewString_ReadMsgData(void *, u32);
void RemoveWindow(Window *);

extern const void *ov112_021FF514[];
extern const u8 ov112_021FEC80[];

void ov112_021E9FA4(void *, int, String *, int, u32);
void ov112_021EA010(void *, int, String *, int, u32);
void ov112_021E9C94(void);
u32 ov112_021E9464(void *);
u32 ov112_021E9470(void *);
void ov112_021E98E8(void *, int);
BOOL ov112_021E9338(void *);
void ov112_021E90FC(Window *, void *);
int ov112_021E9888(int);
void ov112_021E98A4(void *);
void ov112_021E9F40(void *, int);
void ov112_021E9F5C(void *, int, int, u32);
void ov112_021E9FD8(void *, int, String *, int);
void ov112_021EA044(void *, int, int, u32);
u8 ov112_021E935C(const u8 *, int, int, int);
u8 ov112_021E98F8(const u8 *, int, int);
void ov112_021E9934(u8 *, int, int, int);
u32 GetBoxMonData(void *, int, void *);
int _s32_div_f(int, int);
void ov112_021E8C5C(const u8 *, void *, u16 *);
void ov112_021E7CA4(void *, int, int);
void ov112_021EA51C(void *, int);
void ov112_021E966C(void *, int);
void ov112_021E96D0(void *, int);

#endif
