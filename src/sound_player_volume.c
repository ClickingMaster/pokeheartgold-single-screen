#include "global.h"

void GF_SndHandleSetPlayerVolume(u32 playerNo, u8 volume);

void GF_SndHandleSetPlayerVolume(u32 playerNo, u8 volume) {
    NNS_SndPlayerSetPlayerVolume(playerNo, volume);
}
