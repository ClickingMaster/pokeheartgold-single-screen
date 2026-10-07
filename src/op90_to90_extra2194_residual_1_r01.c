#pragma force_active on
#include "global.h"
#include "player_data.h"
typedef struct DexArgs { void *pokedex; PlayerProfile *playerProfile; } DexArgs;
typedef struct Dex { DexArgs *args; u8 pad[0x184C - 4]; u32 *weights; u8 pad2[0x18A2 - 0x1850]; u16 curSpecies; } Dex;
typedef struct Out { Dex *dex; u32 unk4; u16 unk8; u8 unkA; u8 unkB; } Out;
typedef struct Range { u16 min, max; } Range;
extern Range ov18_021FA5CC[];
void ov18_021F504C(Dex *dex, Out *o);
// @fn ov18_021F504C
void ov18_021F504C(Dex *dex, Out *o) {
    u32 ref, weight, lo, diff;
    u8 lighter;
    u32 i;
    o->dex = dex;
    o->unk4 = 0;
    o->unkB = 0;
    if (PlayerProfile_GetTrainerGender(dex->args->playerProfile) == 0) {
        ref = 400;
    } else {
        ref = 410;
    }
    weight = dex->weights[dex->curSpecies];
    if (weight >= ref) {
        lo = ref;
        ref = weight;
        lighter = 0;
    } else {
        lo = weight;
        lighter = 1;
    }
    o->unkA = lighter;
    diff = (ref - lo) / 10;
    for (i = 0; i < 17; i++) {
        if (diff >= ov18_021FA5CC[i].min && diff <= ov18_021FA5CC[i].max) {
            o->unk8 = i;
            return;
        }
    }
}
