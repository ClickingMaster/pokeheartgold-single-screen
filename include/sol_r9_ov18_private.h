#ifndef SOL_R9_OVERLAY18_PRIVATE_H
#define SOL_R9_OVERLAY18_PRIVATE_H
#include "global.h"

typedef struct Window18 {
    u8 raw[0x10];
} Window18;

typedef struct Work18 {
    void *root;
    void *bgConfig;
    void *unk8;
    Window18 windows[101];
} Work18;

void AddWindow(void *, void *, const void *);
void RemoveWindow(void *);
void ScheduleWindowCopyToVram(void *);
void FillWindowPixelBuffer(void *, u32);
void PokedexApp_PrintMessage(void *, void *, u32, u32, u32, u32, u32, u32);
void ov18_021EE44C(void *, u32, u32);
void PokedexApp_DrawCountNumber(void *, u32, u32);
void PokedexApp_AddWindows(Work18 *, const void *, u32);
void PokedexApp_DrawCountNumberAndCopy(void *, u32, u32);
void PokedexApp_DrawSearchMessage(void *, u32);
void PokedexApp_DrawSearchMenuPage(void *);
void PokedexApp_DrawSearchCriteria(void *);
void ov18_021EEE84(void *);
void BgCellManager_PutWindowTiles(void *, u32, void *);

void PokedexApp_RemoveAllWindows(Work18 *);
void ov18_021EE508(void *, u32, u32);
void PokedexApp_DrawCountNumberAndCopy(void *, u32, u32);
void PokedexApp_DrawSeenAndCaughtCounts(void *);
void PokedexApp_SetupSearchWindows(void *);
void PokedexApp_DrawSearchOrderLabel(void *, u32);
void PokedexApp_DrawSearchNameLabel(void *, u32);
void PokedexApp_DrawSearchTypeLabel(void *, u32);
void PokedexApp_DrawSearchHeightLabel(void *, u32);
void PokedexApp_DrawSearchWeightLabel(void *, u32);
void PokedexApp_DrawSearchAreaLabel(void *, u32);
void PokedexApp_DrawSearchFormLabel(void *, u32);
void PokedexApp_DrawHeightLabel(void *, u32);
void PokedexApp_DrawWeightLabel(void *, u32);
u32 LanguageToDexFlag(u32);
void PokedexApp_DrawForeignDexLabel(void *, u32);
void ov18_021E613C(void *, u32);
void PokedexApp_DrawSearchButton(void *, u32, u32);
void PokedexApp_DrawSearchFormPage(void *);
u32 PokedexApp_GetListCursorIndex(void *);
u32 PokedexApp_GetCursorSpecies(void *);
void PokedexApp_DrawListEntryHeader(void *, u32, u32);
void PokedexApp_DrawListEntryDetails(void *, u32, u32);
void PokedexApp_SetupListWindows(void *);
void PokedexApp_DrawInfoLabel(void *, u32);
void PokedexApp_DrawDexNumber(void *, u32, u32);
void PokedexApp_DrawSpeciesName(void *, u32, u32);
void PokedexApp_DrawCategory(void *, u32, u32, u32);
void PokedexApp_DrawSearchEntryHeader(void *, u32, u32);

extern const u8 sSearchWindowTemplates[];
extern const u8 sListWindowTemplates[];

#endif
