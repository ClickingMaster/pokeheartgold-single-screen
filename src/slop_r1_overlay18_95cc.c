#include "slop_r1_module_05_private.h"

void ov18_021F95CC(SlopR1Module05Work *work) {
    PokepicTemplate template;

    GetPokemonSpriteCharAndPlttNarcIds(&template, work->mon, 2);
    work->pokepic = PokepicManager_CreatePokepic(work->pokepicManager, &template, 0x30, 0x48, 0, 0, NULL, NULL);
}
