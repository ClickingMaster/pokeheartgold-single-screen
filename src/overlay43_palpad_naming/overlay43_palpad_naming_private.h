#ifndef TO53_OVERLAY43_PALPAD_NAMING_PRIVATE_H
#define TO53_OVERLAY43_PALPAD_NAMING_PRIVATE_H

#include "global.h"

#include "ascii_util.h"
#include "naming_screen.h"
#include "player_data.h"
#include "sound_playback.h"

typedef struct PalPadSharedH17 {
    u8 padding_00[4];
    SaveData *saveData;
    u8 padding_08[0x58];
    u16 *name;
    u32 useName;
} PalPadSharedH17;

typedef struct PalPadNamingStateH17 {
    u8 padding_00[8];
    u8 state;
} PalPadNamingStateH17;

typedef struct PalPadWorkH17 {
    u8 padding_00[0x38];
    int timer;
} PalPadWorkH17;

void ov43_0222CE48(void *sub, void *shared, void *base, u32 heapId);
void ov43_0222D028(PalPadWorkH17 *work, PalPadNamingStateH17 *state, void *base);
BOOL ov43_0222CE14(PalPadWorkH17 *work, PalPadNamingStateH17 *state);
void ov43_0222D0F8(PalPadWorkH17 *work, void *base, int enabled);
void ov43_0222A318(PalPadNamingStateH17 *state, int value, int count);
void ov43_0222D090(PalPadWorkH17 *work, void *base);

NamingScreenArgs *ov43_0222CD44(void *unused, PalPadSharedH17 *shared, enum HeapID heapId);
void ov43_0222CD90(void *sub, void *shared, void *base, u32 heapId);
BOOL ov43_0222CD98(PalPadWorkH17 *work, PalPadNamingStateH17 *state, void *base);

#endif
