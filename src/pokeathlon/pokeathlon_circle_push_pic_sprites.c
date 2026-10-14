#pragma force_active on
#include "global.h"

#include "pokeathlon/pokeathlon_circle_push_display.h"

#include "sprite.h"

// Two resource ids, one per picture sprite.
typedef struct PokeathlonCirclePushPicIds {
    u32 ids[2];
} PokeathlonCirclePushPicIds;

// The definition order fixes the .rodata layout: the display reads past the nameplate positions
// (gPokeathlonCirclePushNameplateCoords) into the ids that follow them.
static const PokeathlonCirclePushPicIds sPicKinds = { { 5, 6 } };
static const PokeathlonCirclePushPicIds sPicResIds = { { 0x66, 0x67 } };
static const PokeathlonCirclePushPicIds sPicResIds2 = { { 0x66, 0x67 } };
const PokeathlonCirclePushNameplateCoords gPokeathlonCirclePushNameplateCoords = {
    { 0x88, 0x48, 0x68, 0x88 },
    { 0x20, 0x98, 0x98, 0x98 },
};
static const PokeathlonCirclePushPicIds sPicResIds3 = { { 0x66, 0x67 } };

// Sprite pool helpers (pokeathlon_course_objects.c), declared with untyped entries and the argument
// types this caller passes.
void PokeathlonCourseSpritePool_CreateSprite(void *pool, u8 priority, u8 whichScreen, u8 resId, u8 kind);
void *PokeathlonCourseSpritePool_Take(void *pool, u8 resId, u8 kind);
Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(void *entry);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);

void PokeathlonCirclePushDisplay_CreatePicSprites(void *pool, PokeathlonCirclePushDisplay *display);

// The two picture sprites of the top screen, at x 64 and 192.
void PokeathlonCirclePushDisplay_CreatePicSprites(void *pool, PokeathlonCirclePushDisplay *display) {
    PokeathlonCirclePushPicIds resIds = sPicResIds;
    PokeathlonCirclePushPicIds kinds = sPicKinds;
    int i;
    Sprite *sprite;
    VecFx32 position;
    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePool_CreateSprite(pool, 3, NNS_G2D_VRAM_TYPE_2DSUB, resIds.ids[i], kinds.ids[i]);
    }
    for (i = 0; i < 2; i++) {
        display->anim.picSprites[i] = PokeathlonCourseSpritePool_Take(pool, resIds.ids[i], kinds.ids[i]);
        sprite = PokeathlonCourseSpritePoolSprite_GetSprite(display->anim.picSprites[i]);
        Sprite_SetDrawPriority(sprite, 2);
        position.z = 0;
        position.x = (i * 128 + 64) << FX32_SHIFT;
        position.y = 608 << FX32_SHIFT;
        Sprite_SetMatrix(sprite, &position);
        PokeathlonCourseSpritePoolSprite_Show(display->anim.picSprites[i], TRUE, FALSE);
    }
    PokeathlonCirclePushDisplay_GetPicVramLocations(pool, display);
}
