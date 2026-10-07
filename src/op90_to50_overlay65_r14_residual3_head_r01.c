#pragma force_active on
#include "to50_overlay65_r14_residual3_private.h"

// @fn ov65_0221C46C
void ov65_0221C46C(void *party, int start, void *work) {
    Ov65WorkR3 *w = work;
    int i;
    for (i = 0; i < Party_GetCount(party); i++) {
        int index = i + start;
        w->allocations[index] = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(w->records[index].species, w->records[index].form, w->records[index].gender), FALSE, &w->charData[index], HEAP_ID_26);
        DC_FlushRange(w->charData[index]->pRawData, 0x200);
        ov65_0221BFBC(w->charData[index], w->records[index].species, w->records[index].gender, w->records[index].form, index, w->mainSprites[index]);
        Sprite_SetDrawFlag(w->mainSprites[index], TRUE);
        if (w->records[index].heldItem == 0) {
            Sprite_SetDrawFlag(w->itemSprites[index], FALSE);
        } else {
            Sprite_SetDrawFlag(w->itemSprites[index], TRUE);
            Sprite_SetAnimCtrlSeq(w->itemSprites[index], w->records[index].heldItem + 2);
        }
        if (w->records[index].extra == NULL) {
            Sprite_SetDrawFlag(w->extraSprites[index], FALSE);
        } else {
            Sprite_SetDrawFlag(w->extraSprites[index], TRUE);
            Sprite_SetAnimCtrlSeq(w->extraSprites[index], 21);
        }
    }
    for (; i < 6; i++) {
        Sprite_SetDrawFlag(w->mainSprites[start + i], FALSE);
        Sprite_SetDrawFlag(w->itemSprites[start + i], FALSE);
        Sprite_SetDrawFlag(w->extraSprites[start + i], FALSE);
    }
}
