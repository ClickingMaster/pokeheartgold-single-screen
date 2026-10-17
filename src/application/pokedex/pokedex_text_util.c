#include "application/pokedex/pokedex_text_util.h"

#include "global.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

static const u8 sTypeIconPaletteNums[18] = { 0, 2, 0, 3, 1, 1, 3, 2, 0, 0, 2, 2, 1, 1, 1, 0, 2, 3 };

static const u32 sTypeIconCharFiles[18] = { 36, 42, 50, 46, 44, 41, 47, 43, 45, 36, 37, 39, 38, 40, 51, 49, 52, 48 };

void PokedexApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFF, color, NULL); // TEXT_SPEED_NOTRANSFER
}

void PokedexApp_PrintMessage(Window *window, MsgData *msgData, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    PokedexApp_PrintString(window, string, x, y, fontId, color, align);
    String_Delete(string);
}

u32 PokedexApp_GetTypeIconCharFile(u32 type) {
    return sTypeIconCharFiles[type];
}

u8 PokedexApp_GetTypeIconPalette(u32 type) {
    return sTypeIconPaletteNums[type];
}

// The footprint as one 16x16 OBJ: the two 8x16 halves of the stored graphic swapped.
void *PokedexApp_LoadFootprintGraphics(u32 species, enum HeapID heapId) {
    NNSG2dCharacterData *charData;
    void *loaded = GfGfxLoader_GetCharData(Pokefoot_GetNarcId(), Pokefoot_GetCharFileId(species), TRUE, &charData, heapId);
    u8 *raw = charData->pRawData;
    u8 *out = Heap_AllocAtEnd(heapId, 0x80);

    memset(out, 0, 0x80);
    memcpy(out, raw + 0x80, 0x40);
    memcpy(out + 0x40, raw, 0x40);
    Heap_Free(loaded);
    return out;
}
