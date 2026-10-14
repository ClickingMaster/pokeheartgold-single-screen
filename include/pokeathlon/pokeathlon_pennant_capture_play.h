#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_PLAY_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_PLAY_H

#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "pokeathlon/pokeathlon_course_types_def.h"

// Pennant Capture (event 1): screen set-up, the event states and the host's frame step.

// What every participant sends each frame.
typedef struct PokeathlonPennantCaptureInput {
    BOOL touching; // 0x00
    u8 x;          // 0x04
    u8 y;          // 0x05
    u8 unk06[2];   // 0x06
    u8 swapPhase;  // 0x08: team member swap: 1 = requested, 2 = done
    u8 unk09[0x1B];
} PokeathlonPennantCaptureInput; // size: 0x24 (in a 0x28 link entry)

void PokeathlonPennantCapture_InitBgs(BgConfig *bgConfig);
void PokeathlonPennantCapture_LoadBgGfx(PokeathlonPennantCapture *work);
void PokeathlonPennantCapture_LoadSpriteResources(PokeathlonCourseSpritePool *pool);
void PokeathlonPennantCapture_CreateSprites(PokeathlonPennantCapture *work, PokeathlonCourseSpritePool *pool);
BOOL PokeathlonPennantCapture_State_Setup(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonPennantCapture_State_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonPennantCapture_State_End(PokeathlonCourseData *course, u8 *state);
void PokeathlonPennantCapture_HostStep(PokeathlonCourseData *course);
void PokeathlonPennantCapture_PackFrame(PokeathlonPennantCapture *work, PokeathlonPennantCaptureFrame *frame);
BOOL PokeathlonPennantCapture_SendInput(PokeathlonCourseData *course);
void PokeathlonPennantCapture_ShowFrame(PokeathlonCourseData *course);
BOOL PokeathlonPennantCapture_IsTouchOnOwnMon(PokeathlonPennantCapture *work, int participant, int x, int y);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_PLAY_H
