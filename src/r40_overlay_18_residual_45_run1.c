#include "r40_overlay_18_residual_45_private.h"

void PokedexApp_PrintString(void *window, void *str, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, str, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, str, 0) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, str, x, y, 0xff, color, NULL);
}

void PokedexApp_PrintMessage(void *window, void *msgData, u32 msgId, u32 x, u32 y, u32 fontId, u32 color, u32 align) {
    void *str = NewString_ReadMsgData(msgData, msgId);
    PokedexApp_PrintString(window, str, x, y, fontId, color, align);
    String_Delete(str);
}

u32 PokedexApp_GetTypeIconCharFile(u32 index) {
    return sTypeIconCharFiles[index];
}

u8 PokedexApp_GetTypeIconPalette(u32 index) {
    return sTypeIconPaletteNums[index];
}

void *PokedexApp_LoadFootprintGraphics(u32 species, u32 heapId) {
    Ov18CharDataR45 *charData;
    void *loaded = GfGfxLoader_GetCharData(ov18_021E5900(species), ov18_021E5904(species), TRUE, &charData, heapId);
    u8 *raw = charData->raw;
    u8 *out = Heap_AllocAtEnd(heapId, 0x80);
    memset(out, 0, 0x80);
    memcpy(out, raw + 0x80, 0x40);
    memcpy(out + 0x40, raw, 0x40);
    Heap_Free(loaded);
    return out;
}
