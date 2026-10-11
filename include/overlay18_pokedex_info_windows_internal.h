#ifndef POKEHEARTGOLD_OVERLAY18_POKEDEX_INFO_WINDOWS_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY18_POKEDEX_INFO_WINDOWS_INTERNAL_H
#include "global.h"

#include "bg_cell_manager.h"

#define PTR(p, off) (*(void **)((u8 *)(p) + (off)))

void PokedexApp_RemoveAllWindows(void *);
void PokedexApp_AddWindows(void *, const void *, u32);
void PokedexApp_PrintMessage(void *, void *, u32, u32, u32, u32, u32, u32);
void PokedexApp_DrawFormsWindows(void *);
u32 ov18_021E8AE0(void *, int);
u32 MapHeader_GetMapSec(u32);
void BufferLandmarkName(void *, int, u32);

extern const u8 ov18_021F9DB0[];
extern const u8 sAreaWindowTemplates[];
extern const u8 sFormsWindowTemplates[];

void PokedexApp_ClearSearchEntryWindows(void *);
void ov18_021F0168(void *);
void ov18_021F018C(void *);
void ov18_021F01D4(void *);
void PokedexApp_FreeAreaWindows(void *);
void PokedexApp_DrawTimeOfDayLabel(void *);
u32 PokedexApp_GetAreaListEntryMsg(void *, int);
void PokedexApp_FreeSizeCheckWindows(void *);
void ov18_021F0858(void *);
void ov18_021F08E0(void *);
void PokedexApp_SetupFormsWindows(void *);
#endif
