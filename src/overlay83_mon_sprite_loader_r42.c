#include "overlay83_mon_sprite_loader_r42_private.h"

void ov83_02247630(void *arg0, void *arg1, u32 arg2) {
    ov83_022475D4(arg0, 0xB);
    ov83_02247568(arg0, arg1, arg2);
    ov83_0224755C(arg0, 1);
    PlaySE(0x5E3);
    PlaySE(0x655);
}

void ov83_02247668(Ov83SpriteSlot *slot, BoxPokemon *boxmon, u16 species, u32 personality) {
    PokepicTemplate pic;
    UnkStruct_02014E30 rect = ov83_0224818C;
    NNSG2dCharacterData *charData;
    void *handle;
    void *buffer;
    u32 imageLocation;
    u32 paletteLocation;
    int narcId;
    int paletteId;

    buffer = Heap_AllocAtEnd(HEAP_ID_107, 0xC80);
    if (species != 0) {
        GetBoxmonSpriteCharAndPlttNarcIds(&pic, boxmon, 2, FALSE);
        sub_02014510((NarcId)pic.narcID, pic.charDataID, HEAP_ID_107, &rect, buffer, personality, FALSE, 2, species);
        narcId = pic.narcID;
        paletteId = pic.palDataID;
    } else {
        handle = GfGfxLoader_GetCharData((NarcId)0xB8, 0x27, TRUE, &charData, HEAP_ID_107);
        MI_CpuCopy32(charData->pRawData, buffer, 0xC80);
        Heap_Free(handle);
        narcId = 0xB8;
        paletteId = 0x3D;
    }
    imageLocation = NNS_G2dGetImageLocation(Sprite_GetImageProxy(slot->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    DC_FlushRange(buffer, 0xC80);
    GXS_LoadOBJ(buffer, imageLocation, 0xC80);
    paletteLocation = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(slot->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    GfGfxLoader_GXLoadPal((NarcId)narcId, paletteId, GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)paletteLocation, 0x20, HEAP_ID_107);
    Heap_Free(buffer);
}
