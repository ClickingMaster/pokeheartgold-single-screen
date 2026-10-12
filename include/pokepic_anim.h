#ifndef POKEHEARTGOLD_POKEPIC_ANIM_H
#define POKEHEARTGOLD_POKEPIC_ANIM_H

#include "global.h"

#include "pokepic.h"

struct PokepicFacingAnimData {
    u8 cryDelay;
    u8 animScriptId;
    u8 animDelay;
    PokepicAnimScript animScripts[10];
};

struct PokepicSpeciesAnimData {
    struct PokepicFacingAnimData facings[2];
    s8 shadowHeight;
    s8 shadowXOffset;
    u8 shadowSize;
};

struct PokepicAnimParams {
    u16 scriptId;
    u16 delay;
    u8 reverseX;
};

void PokepicAnimManager_StartAnim(void *, void *, struct PokepicAnimParams *, u8);

#endif // POKEHEARTGOLD_POKEPIC_ANIM_H
