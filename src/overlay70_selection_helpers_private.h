#ifndef SOL40_R29_PRIVATE_H
#define SOL40_R29_PRIVATE_H
#include "global.h"
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define S8(p,o) (*(s8 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define S16(p,o) (*(s16 *)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
void GtsSelector_RemoveWindows(void *,int); void ListMenuItems_Delete(void *); void GF_AssertFail(void);
void Sprite_SetDrawFlag(void *,int); void Sprite_SetAnimCtrlSeq(void *,int); void GtsSelector_ShowGenders(void *); void GtsSelector_ShowLevels(void *); void Gts_SetSpritePosition(void *,int,int);
int GtsSelector_HandleCountryListInput(void *); int GtsSelector_HandleGenderInput(void *); int GtsSelector_HandleLevelInput(void *); void BgCellManager_StartCellMove(void *,int,int,int,int);
void *NewString_ReadMsgData(void *,int); void String_Delete(void *); void GtsSelector_PrintString(void *,void *,void *,int,int); void CopyWindowPixelsToVram_TextMode(void *); void BgCellManager_PutWindowTiles(void *,int,void *);
void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,int,int); void PrintUIntOnWindow(void *,int,int,int,void *,int,int); void sub_0200CDAC(void *,int,void *,int,int); void *Gts_LoadSpeciesByLetter(int,int,u32 *); int GtsSelector_CountSeenSpecies(void *,void *,u32,void *); void Heap_Free(void *);
int Gts_GetCountryGroupRange(u32,u32 *);
extern const u8 sGtsSelectorGenderCursorX[],sGtsSelectorGenderCursorY[],sGtsSelectorLevelCursorX[],sGtsSelectorLevelCursorY[];
typedef struct Ov70Pair29 { s32 a,b; } Ov70Pair29; extern const s32 ov70_02245E84[]; extern const Ov70Pair29 ov70_02245F5C[]; extern void (*ov70_02246780[])(void *); extern const Ov70Pair29 ov70_02245F58[]; typedef struct Far29 { u8 pad[0x12d0]; void *task; u32 value; } Far29;

int GtsSelector_HandleCountryList(void *); int GtsSelector_CancelCountryList(void *); int GtsSelector_FinishCountry(void *); int GtsSelector_OpenGenders(void *); int GtsSelector_HandleGenders(void *); int GtsSelector_FinishGender(void *); int GtsSelector_OpenLevels(void *); int GtsSelector_HandleLevels(void *); int GtsSelector_FinishLevel(void *); void GtsSelector_PrintMessage(void *,void *,void *,int,int,int); void GtsSelector_PrintButtonLabel(void *,void *,void *,int); void GtsSelector_PrintPageNumber(void *,void *,void *,int,int); int GtsSelector_CountSeenSpeciesOfLetter(void *,int); int GtsSelector_GroupHasSeenSpecies(void *,int); int GtsSelector_CountCountriesOfLetter(void *,u32); void Gts_GetWifiIconLevel(void *); void sub_0203A930(void); void ov70_02244124(void *); void SysTask_Destroy(void *); void ov70_022446A8(void *); void ov70_02244644(void *); void ov70_02244290(void *); void Gts_SwitchScreen(void *); int ov70_02244154(void *); int ov70_02243FE0(void *,int);
#endif
