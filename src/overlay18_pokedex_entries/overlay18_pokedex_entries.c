#include "private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void PokedexApp_DrawForeignDexNumber(void *work, u32 species, u32 index, u32 language)
{
    u8 *windowBase = (u8 *)work + 0xC;
    u32 offset = index << 4;
    int mode;

    FillWindowPixelBuffer(windowBase + offset, 0);
    switch (language) {
    case 1:
        mode = 0;
        break;
    default:
        mode = 1;
        break;
    }
    BufferIntegerAsString(PTR(work, 0x660), 0,
        Pokedex_ConvertToCurrentDexNo(*((u8 *)work + 0x1858), species), 3, 2, mode);
    PokedexApp_PrintFormattedMessage(work, PTR(work, 0x65C), index, 9, 0, 0, 0, 0x20100, 0);
    ScheduleWindowCopyToVram(windowBase + offset);
}

void PokedexApp_DrawForeignSpeciesName(void *work, u32 species, u32 index)
{
    u8 *windowBase = (u8 *)work + 0xC;
    u32 offset = index << 4;
    void *string;

    FillWindowPixelBuffer(windowBase + offset, 0);
    string = ov18_021E590C(species, *((u8 *)work + 0x185C), 0x25);
    PokedexApp_PrintString(windowBase + offset, string, 0, 0, 0, 0x20100, 0);
    String_Delete(string);
    ScheduleWindowCopyToVram(windowBase + offset);
}

void PokedexApp_DrawForeignCategory(void *work, u32 species, u32 slot, u32 index)
{
    u8 *windowBase = work;
    u32 offset = index << 4;
    void *string;

    windowBase += 0xC;
    FillWindowPixelBuffer(windowBase + offset, 0);
    if (*(u16 *)((u8 *)work + slot * 4 + 0x1032) == 2) {
        string = ov18_021E595C(species, *((u8 *)work + 0x185C), 0x25);
    } else {
        string = ov18_021E595C(0, *((u8 *)work + 0x185C), 0x25);
    }
    PokedexApp_PrintString(windowBase + offset, string, 0x7C, 0, 0, 0x20100, 1);
    String_Delete(string);
    ScheduleWindowCopyToVram(windowBase + offset);
}

void PokedexApp_DrawSearchEntryDetails(To47Work18 *work, u32 species, u32 slot)
{
    ClearWindowTilemapAndScheduleTransfer(&work->windows[86]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[88]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[85]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[87]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[89]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[90]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[91]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[92]);
    ClearWindowTilemapAndScheduleTransfer(&work->windows[93]);

    if (species != 0) {
        PokedexApp_DrawFlavorText(work, species, slot, 0x55);
        if (work->language == 2) {
            PokedexApp_DrawHeightLabel(work, 0x56);
            PokedexApp_DrawWeightLabel(work, 0x58);
            PokedexApp_DrawHeight(work, species, slot, 0x57);
            PokedexApp_DrawWeight(work, species, slot, 0x59);
        } else {
            PokedexApp_DrawForeignDexLabel(work, 0x5A);
            PokedexApp_DrawForeignDexNumber(work, species, 0x5B, work->language);
            PokedexApp_DrawForeignSpeciesName(work, species, 0x5C);
            PokedexApp_DrawForeignCategory(work, species, slot, 0x5D);
        }
    }
}
