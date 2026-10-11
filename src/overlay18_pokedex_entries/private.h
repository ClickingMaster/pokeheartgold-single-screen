#ifndef TO47_SOL_R4_OV18_GAP05_PRIVATE_H
#define TO47_SOL_R4_OV18_GAP05_PRIVATE_H

#include "global.h"

void FillWindowPixelBuffer(void *, u32);
void ClearWindowTilemapAndScheduleTransfer(void *);
void ScheduleWindowCopyToVram(void *);
u8 GetWindowWidth(void *);
u32 FontID_String_GetWidthMultiline(u32, void *, u32);
void String_Delete(void *);
u32 Pokedex_ConvertToCurrentDexNo(BOOL, u32);
void BufferIntegerAsString(void *, int, int, int, int, int);
void *ov18_021E590C(u32, int, int);
void *ov18_021E595C(u32, int, int);
void *ov18_021E59A8(u32, int, int, int);
void PokedexApp_PrintFormattedMessage(void *, void *, u32, u32, s32, u32, u32, u32, u32);
void PokedexApp_PrintString(void *, void *, u32, u32, u32, u32, u32);
void PokedexApp_DrawHeightLabel(void *, u32);
void PokedexApp_DrawWeightLabel(void *, u32);
void PokedexApp_DrawHeight(void *, u32, u32, u32);
void PokedexApp_DrawWeight(void *, u32, u32, u32);
void PokedexApp_DrawForeignDexLabel(void *, u32);
void PokedexApp_DrawForeignDexNumber(void *, u32, u32, u32);
void PokedexApp_DrawForeignSpeciesName(void *, u32, u32);
void PokedexApp_DrawForeignCategory(void *, u32, u32, u32);
void PokedexApp_DrawDexNumber(void *, u32, u32);
void PokedexApp_DrawSpeciesName(void *, u32, u32);
void PokedexApp_DrawCategory(void *, u32, u32, u32);
void PokedexApp_DrawFlavorText(void *, u32, u32, u32);

typedef struct To47Window18 {
    u8 raw[0x10];
} To47Window18;

typedef struct To47Work18 {
    u8 header[0xc];
    To47Window18 windows[101];
    u8 filler_065c[0x1200];
    u8 language;
} To47Work18;

void PokedexApp_DrawListEntryDetails(To47Work18 *, u32, u32);

void PokedexApp_DrawSearchEntryDetails(To47Work18 *, u32, u32);

#endif
