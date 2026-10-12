#include "application/pokedex/pokedex_text_util.h"

#include "global.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"

extern const u32 sTypeIconCharFiles[];
extern const u8 sTypeIconPaletteNums[];

// Declared with a u32 font id, as these calls were compiled: text.h's FontID (u8) would narrow the argument.
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

void PokedexApp_PrintString(Window *window, String *string, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, 0xFF, color, NULL); // TEXT_SPEED_NOTRANSFER (text.h is not included, see above)
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
