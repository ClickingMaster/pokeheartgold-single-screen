#include "field/encounter_effect.h"

#include "global.h"

#include "scanline_effect.h"

// The trainer and Team Rocket effects declare this locally (with an int period).
void EncounterEffectHBlankWave_AlternateDirection(EncounterEffectHBlankWave *wave, u32 period);

#pragma force_active on

// Negates the offsets of every other block of period lines, so neighbouring blocks wave in opposite directions.
void EncounterEffectHBlankWave_AlternateDirection(EncounterEffectHBlankWave *wave, u32 period) {
    int i;
    s16 offset;
    u32 *buffer = ScanlineEffect_GetBuffer(wave->effect);
    for (i = 0; i < 192; i++) {
        offset = *buffer;
        if (((i / period) & 1) == 0) {
            *buffer = (u16)offset;
        } else {
            *buffer = (u16)-offset;
        }
        buffer++;
    }
}
