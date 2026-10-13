#include "poketool/pokefoot/pokefoot.naix"

#include "filesystem.h"

NarcId Pokefoot_GetNarcId(void);
int Pokefoot_GetCharFileId(int species);
int Pokefoot_GetPlttFileId(void);

NarcId Pokefoot_GetNarcId(void) {
    return NARC_poketool_pokefoot_pokefoot;
}

int Pokefoot_GetCharFileId(int species) {
    return species + NARC_pokefoot_pokefoot_00000003_NCGR_lz;
}

int Pokefoot_GetPlttFileId(void) {
    return NARC_pokefoot_pokefoot_00000000_NCLR;
}
