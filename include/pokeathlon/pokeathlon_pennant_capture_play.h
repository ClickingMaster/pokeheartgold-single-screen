#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_PLAY_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_PLAY_H

#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_pennant_capture.h"

// Pennant Capture: screen set-up, the event states and the host's frame step.

// What every participant sends each frame.
typedef struct PokeathlonPennantCaptureInput {
    BOOL touching;
    u8 x;
    u8 y;
    u8 unk06[2];
    u8 swapPhase; // team member swap: 1 = requested, 2 = done
    u8 unk09[0x1B];
} PokeathlonPennantCaptureInput;

void PokeathlonPennantCapture_InitBgs(BgConfig *bgConfig);
void PokeathlonPennantCapture_LoadBgGfx(PokeathlonPennantCapture *work);
void PokeathlonPennantCapture_LoadSpriteResources(PokeathlonCourseSpritePool *pool);
void PokeathlonPennantCapture_CreateSprites(PokeathlonPennantCapture *work, PokeathlonCourseSpritePool *pool);
BOOL PokeathlonPennantCapture_State_Setup(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonPennantCapture_State_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonPennantCapture_State_End(PokeathlonCourseData *course, u8 *state);
void PokeathlonPennantCapture_PackFrame(PokeathlonPennantCapture *work, PokeathlonPennantCaptureFrame *frame);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_PLAY_H
