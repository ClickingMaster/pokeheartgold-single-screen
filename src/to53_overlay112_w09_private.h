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

extern const void *sTouchButtons[];
extern const u8 sWindowBankScreens[];

void PokewalkerApp_PrintString(void *, int, String *, int, u32);
void PokewalkerApp_PrintSubString(void *, int, String *, int, u32);
void ov112_021E9C94(void);
u32 PokewalkerApp_GetBoxMonIconFile(void *);
u32 PokewalkerApp_GetSpeciesIconFile(void *);
void PokewalkerApp_ShowSubBg1(void *, int);
BOOL ov112_021E9338(void *);
void PokewalkerApp_PrintPlayerName(Window *, void *);
int PokewalkerApp_GetTouchedButton(int);
void PokewalkerApp_LoadSubBg1Tilemap(void *);
void PokewalkerApp_PrintExit(void *, int);
void PokewalkerApp_PrintMessage(void *, int, int, u32);
void PokewalkerApp_PrintStringDefaultColor(void *, int, String *, int);
void PokewalkerApp_PrintSubMessage(void *, int, int, u32);
u8 PokewalkerApp_GetPixel4bpp(const u8 *, int, int, int);
u8 PokewalkerApp_GetPixel2bpp(const u8 *, int, int);
void PokewalkerApp_SetPixel2bpp(u8 *, int, int, int);
u32 GetBoxMonData(void *, int, void *);
int _s32_div_f(int, int);
void PokewalkerApp_ApplyAdvantageTypes(const u8 *, void *, u16 *);
void PokewalkerApp_LoadTilemap(void *, int, int);
void PokewalkerApp_SetCursorPosition(void *, int);
void PokewalkerApp_ShowYesNoMenu(void *, int);
void PokewalkerApp_ShowSaveEraseStepsMenu(void *, int);

#endif
