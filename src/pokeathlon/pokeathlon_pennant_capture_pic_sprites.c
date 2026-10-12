#pragma force_active on
#include "global.h"

#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "sprite.h"

// The definition order fixes the .rodata layout: the display parts and the CPU read the ids through
// ov96_0221CBCC as an array.
static const PokeathlonPennantCaptureSpriteIds sPicKinds = { { 4, 5 } };
static const PokeathlonPennantCaptureSpriteIds sPicResIds2 = { { 0x66, 0x67 } };
const PokeathlonPennantCaptureSpriteIds ov96_0221CBCC = { { 0x66, 0x67 } };
static const PokeathlonPennantCaptureSpriteIds sPicResIds3 = { { 0x66, 0x67 } };

// Sprite pool helpers (pokeathlon_course_objects.c), declared with untyped entries and the argument
// types this caller passes.
void PokeathlonCourseSpritePool_CreateSprite(void *pool, int priority, int whichScreen, u8 resId, u8 kind);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);
void *PokeathlonCourseSpritePool_Take(void *pool, u8 resId, u8 kind);
void *PokeathlonCourseSpritePoolSprite_GetSprite(void *entry);

void PokeathlonPennantCaptureDisplay_CreatePicSprites(void *pool, PokeathlonPennantCaptureDisplay *display);

// The two picture sprites of the top screen.
void PokeathlonPennantCaptureDisplay_CreatePicSprites(void *pool, PokeathlonPennantCaptureDisplay *display) {
    PokeathlonPennantCaptureSpriteIds resIds = ov96_0221CBCC;
    PokeathlonPennantCaptureSpriteIds kinds = sPicKinds;
    VecFx32 position;
    void *sprite;
    int i;
    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePool_CreateSprite(pool, 3, NNS_G2D_VRAM_TYPE_2DSUB, resIds.ids[i], kinds.ids[i]);
    }
    for (i = 0; i < 2; i++) {
        display->picSprites[i] = PokeathlonCourseSpritePool_Take(pool, resIds.ids[i], kinds.ids[i]);
        sprite = PokeathlonCourseSpritePoolSprite_GetSprite(display->picSprites[i]);
        position.z = 0;
        position.x = 40 << FX32_SHIFT;
        position.y = ((88 * (1 - i) + 48) << FX32_SHIFT) + (700 << FX32_SHIFT);
        Sprite_SetMatrix(sprite, &position);
        PokeathlonCourseSpritePoolSprite_Show(display->picSprites[i], TRUE, TRUE);
    }
    PokeathlonPennantCaptureDisplay_GetPicVramOffsets(pool, display);
}
