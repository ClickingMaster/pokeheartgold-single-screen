#include "global.h"

// Own TU: include/sound_system.h declares a u32 volume (its matched callers pass unnarrowed values), while this
// definition only matches with a u8 volume, which it passes on to NNS_SndPlayerSetPlayerVolume unchanged.

void NNS_SndPlayerSetPlayerVolume(int playerNo, u8 volume);
void GF_SndHandleSetPlayerVolume(u32 playerNo, u8 volume);

void GF_SndHandleSetPlayerVolume(u32 playerNo, u8 volume) {
    NNS_SndPlayerSetPlayerVolume(playerNo, volume);
}
