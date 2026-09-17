#include "to50_overlay99_pokedex_accessors_private.h"

int ov99_021E71E4(void **work, u16 species) {
    if (species == 0 || species > 493) {
        return 0;
    }
    return Pokedex_GetSeenFormByIdx(ov99_021E71DC(work), species, 0);
}

int ov99_021E7208(void **work, u16 species) {
    int gender;

    if (species == 0 || species > 493) {
        return 0;
    }
    gender = Pokedex_SpeciesGetLastSeenGender(ov99_021E71DC(work), species, 0);
    if (gender == -1) {
        gender = GetGenderBySpeciesAndPersonality(species, 0);
    }
    return gender;
}
