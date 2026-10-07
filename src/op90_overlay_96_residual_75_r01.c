#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {
    Sprite *icon;
    Sprite *frame;
    Sprite *label;
    u8 padc[4];
} Ent;
typedef struct {
    int heapId;
    u8 pad4[0x15c];
    u8 x160[0x24];
    Ent ents[12];
} Work;
typedef struct {
    u16 species;
    u16 form;
} MonId;
void ov96_021FC5E0(SpriteTemplate *tmpl, Work *work, void *a, int b);
void ov96_021FC248(Work *work, int i, int y);
u32 GetMonIconNaixEx(u32 species, u32 isEgg, u32 form);
const u8 GetMonIconPaletteEx(u32 species, u32 form, u32 isEgg);
BOOL NNS_G2dGetUnpackedBGCharacterData(void *src, NNSG2dCharacterData **out);
void ov96_021FC450(Work *work, MonId *mons);
// @fn ov96_021FC450
void ov96_021FC450(Work *work, MonId *mons) {
    SpriteTemplate tmpl;
    NNSG2dCharacterData *charData;
    u32 loadSize;
    u32 flushSize;
    NARC *narc;
    void *buf;
    u8 form;
    u32 offset;
    int y;
    int i;
    u16 species;
    u8 pal;
    narc = NARC_New((NarcId)0x14, (enum HeapID)work->heapId);
    buf = Heap_AllocAtEnd((enum HeapID)work->heapId, 0x1000);
    ov96_021FC5E0(&tmpl, work, work->x160, 2);
    offset = 0x200;
    y = 0x10;
    flushSize = offset;
    loadSize = offset;
    for (i = 0; i < 12; i++, y += 0x20) {
        work->ents[i].icon = Sprite_CreateAffine(&tmpl);
        Sprite_SetAnimActiveFlag(work->ents[i].icon, TRUE);
        Sprite_SetAnimCtrlSeq(work->ents[i].icon, i);
        work->ents[i].frame = Sprite_CreateAffine(&tmpl);
        Sprite_SetAnimCtrlSeq(work->ents[i].frame, i / 3 + 12);
        work->ents[i].label = Sprite_CreateAffine(&tmpl);
        if (i < 8) {
            Sprite_SetAnimCtrlSeq(work->ents[i].label, i + 16);
        }
        species = mons[i].species;
        form = mons[i].form;
        if (species == 0) {
            Sprite_SetDrawFlag(work->ents[i].icon, FALSE);
            Sprite_SetDrawFlag(work->ents[i].frame, FALSE);
            Sprite_SetDrawFlag(work->ents[i].label, FALSE);
        }
        Sprite_SetDrawPriority(work->ents[i].icon, 2);
        Sprite_SetDrawPriority(work->ents[i].frame, 3);
        Sprite_SetDrawPriority(work->ents[i].label, 1);
        ov96_021FC248(work, i, y);
        Sprite_SetDrawFlag(work->ents[i].label, TRUE);
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(species, FALSE, form), buf);
        NNS_G2dGetUnpackedBGCharacterData(buf, &charData);
        pal = GetMonIconPaletteEx(species, form, FALSE) + 3;
        DC_FlushRange(charData->pRawData, flushSize);
        GXS_LoadOBJ(charData->pRawData, offset, loadSize);
        Sprite_SetPaletteOverride(work->ents[i].icon, pal);
        offset += 0x200;
    }
    Heap_Free(buf);
    NARC_Delete(narc);
}
