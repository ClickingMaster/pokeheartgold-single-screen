#pragma force_active on
#include "global.h"
#include "pokepic.h"
#include "filesystem.h"
void GetBoxmonSpriteCharAndPlttNarcIds(PokepicTemplate *t, BoxPokemon *boxMon, u8 whichFacing, BOOL a3);
void NARC_ReadPokepicAnimScript(NARC *narc, PokepicAnimScript *dest, u16 species, u16 a3);
u16 sub_02070854(BoxPokemon *boxMon, u8 whichFacing, BOOL a2);
typedef struct Work {
    void *context;
    u8 unk04[8];
    PokepicManager *manager;
    u32 unk10;
    PokepicAnimScript animScript[10];
    u8 unk3C[0x44];
    NARC *narc;
} Work;
BoxPokemon *ov71_02247390(void *context);
u16 ov71_022473A8(void *context);
Pokepic *ov71_02247F9C(Work *work);

// @fn ov71_02247F9C
Pokepic *ov71_02247F9C(Work *work) {
    PokepicTemplate template;
    BoxPokemon *boxMon = ov71_02247390(work->context);
    GetBoxmonSpriteCharAndPlttNarcIds(&template, boxMon, 2, FALSE);
    NARC_ReadPokepicAnimScript(work->narc, work->animScript, ov71_022473A8(work->context), 1);
    {
        int y = sub_02070854(boxMon, 2, FALSE) + 80;
        return PokepicManager_CreatePokepic(work->manager, &template, 128, y, 0, 0, work->animScript, NULL);
    }
}
