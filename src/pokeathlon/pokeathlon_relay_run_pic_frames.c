#pragma force_active on
#include "global.h"

#include "pokeathlon/pokeathlon_relay_run.h"
#include "pokeathlon/pokeathlon_relay_run_cpu.h"

#include "sprite.h"

// Two resource ids, one per picture frame.
typedef struct PokeathlonRelayRunPicIds {
    u32 ids[2];
} PokeathlonRelayRunPicIds;

// The definition order fixes the .rodata layout: the HUD and the CPU read the ids through
// gPokeathlonRelayRunPicIds as one array.
static const PokeathlonRelayRunPicIds sPicResIds2 = { { 0x66, 0x67 } };
static const PokeathlonRelayRunPicIds sPicKinds = { { 4, 5 } };
const PokeathlonRelayRunPicIds gPokeathlonRelayRunPicIds = { { 0x66, 0x67 } };
static const PokeathlonRelayRunPicIds sPicResIds3 = { { 0x66, 0x67 } };

// Sprite pool helpers (pokeathlon_course_objects.c), declared with untyped entries and the argument
// types this caller passes.
void PokeathlonCourseSpritePool_CreateSprite(void *pool, int priority, int whichScreen, u8 resId, u8 kind);
void *PokeathlonCourseSpritePool_Take(void *pool, u8 resId, u8 kind);
void *PokeathlonCourseSpritePoolSprite_GetSprite(void *entry);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *entry, int seq);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *entry, VecFx32 *position);

void PokeathlonRelayRunHud_CreatePicFrames(void *spritePool, PokeathlonRelayRunHud *hud);

// The two picture frames of the top screen and the tired mark shown on each.
void PokeathlonRelayRunHud_CreatePicFrames(void *spritePool, PokeathlonRelayRunHud *hud) {
    PokeathlonRelayRunPicIds resIds = gPokeathlonRelayRunPicIds;
    PokeathlonRelayRunPicIds kinds = sPicKinds;
    VecFx32 position;
    int i;
    void *sprite;

    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePool_CreateSprite(spritePool, 3, NNS_G2D_VRAM_TYPE_2DSUB, resIds.ids[i], kinds.ids[i]);
    }
    for (i = 0; i < 2; i++) {
        hud->picFrames[i] = PokeathlonCourseSpritePool_Take(spritePool, resIds.ids[i], kinds.ids[i]);
        sprite = PokeathlonCourseSpritePoolSprite_GetSprite(hud->picFrames[i]);
        position.z = 0;
        position.x = 40 << FX32_SHIFT;
        position.y = ((88 * (1 - i) + 48) << FX32_SHIFT) + (512 << FX32_SHIFT);
        Sprite_SetMatrix(sprite, &position);
        PokeathlonCourseSpritePoolSprite_Show(hud->picFrames[i], TRUE, TRUE);
        hud->tiredMarks[i] = PokeathlonCourseSpritePool_Take(spritePool, 101, 16);
        PokeathlonCourseSpritePoolSprite_SetAnim(hud->tiredMarks[i], 8);
        PokeathlonCourseSpritePoolSprite_SetPosition(hud->tiredMarks[i], &position);
        PokeathlonCourseSpritePoolSprite_Show(hud->tiredMarks[i], TRUE, FALSE);
    }
    PokeathlonRelayRunHud_GetPicVramOffsets(spritePool, hud);
}
