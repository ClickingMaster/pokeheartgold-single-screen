#include "global.h"

int TranslateAgbSpecies(int species);

extern u16 ov74_0223CC5C[];
extern u16 ov74_0223CC5E[];
int TranslateAgbSpecies(int species) {
    u32 i;
    if (species >= 0x115) {
        for (i = 0; i < 0x90; i++) {
            if (species == ov74_0223CC5C[i * 2]) {
                return ov74_0223CC5E[i * 2];
            }
        }
    }
    return species;
}
