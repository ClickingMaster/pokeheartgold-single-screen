#pragma force_active on
#include "global.h"

#include "pokeathlon/pokeathlon_hurdle_dash.h"
#include "pokeathlon/pokeathlon_hurdle_dash_race.h"

#include "assert.h"
#include "sprite.h"

// Course helpers (pokeathlon_course*.c), declared with untyped objects and the argument types this
// caller passes (u8 member indices; a mon object where the object helpers expect a course object).
void *PokeathlonCourseMonObjects_Get(void *objects, u8 index);
void *PokeathlonCourseMonObject_GetGfx(void *object);
PokeathlonHurdleDashMonView *PokeathlonCourseMonGfx_GetInfo(void *gfx);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int mon);
void *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, u8 mon);
int PokeathlonCourse_GetCollisionSizeClass(void *performance);
void PokeathlonCourseObject_SetCollision(void *object, u32 offsetY, u32 radius);
int PokeathlonCourse_GetTouchRadius(void);
void PokeathlonCourseObject_SetTouchRadius(void *object, u32 radius);
void PokeathlonCourseObject_SetGroundPosition(void *object, int x, int y);
void PokeathlonCourseMonObject_SetAnimState(void *object, u32 animState);
void PokeathlonCourseMonObject_SetDrawPriority(void *object, u32 drawPriority);
void PokeathlonCourseMonObject_SetAffineOverwriteMode(void *object, int mode);
void PokeathlonCourseMonObject_SetAffineMatrix(void *object, const VecFx32 *matrix);
void *PokeathlonCourseSpritePool_Take(void *pool, u8 resId, u8 kind);
Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(void *entry);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);

void PokeathlonHurdleDashRunner_Init(PokeathlonCourseData *course, const PokeathlonHurdleDashTable *table, const PokeathlonHurdleDashParams *params, PokeathlonHurdleDashTrack *track, void *scene, void *monObjects, u8 lane, PokeathlonHurdleDashRunner *runner);

// Sets up one of the player's Pokemon in its lane: its stats from the event tables, its object and the
// jump marker, rank and boost sprites.
void PokeathlonHurdleDashRunner_Init(PokeathlonCourseData *course, const PokeathlonHurdleDashTable *table, const PokeathlonHurdleDashParams *params, PokeathlonHurdleDashTrack *track, void *scene, void *monObjects, u8 lane, PokeathlonHurdleDashRunner *runner) {
    VecFx32 scale;
    VecFx32 markerPosition;
    VecFx32 rankPosition;
    VecFx32 boostPosition;
    void *monObject;
    int x;
    int member;
    void *gfx;
    PokeathlonHurdleDashMonView *info;
    u8 *stats;
    int performance;
    Sprite *sprite;

    runner->distance = 0;
    runner->time = 0;
    runner->subScrollY = 48;
    runner->mainScrollY = 320;
    runner->lane = lane;
    runner->rank = 1;
    runner->rankScale = 1.0f;
    runner->finished = FALSE;
    runner->hurdles = PokeathlonHurdleDashTrack_GetLaneHurdles(track, lane);
    switch (lane) {
    case 0:
        runner->mainBg = 0;
        runner->subBg = 4;
        break;
    case 1:
        runner->mainBg = 1;
        runner->subBg = 5;
        break;
    case 2:
        runner->mainBg = 2;
        runner->subBg = 6;
        break;
    }
    switch (lane) {
    case 0:
        member = 0;
        x = 48;
        break;
    case 1:
        member = 1;
        x = 128;
        break;
    case 2:
        member = 2;
        x = 208;
        break;
    default:
        GF_AssertFail();
        return;
    }
    monObject = PokeathlonCourseMonObjects_Get(monObjects, member);
    gfx = PokeathlonCourseMonObject_GetGfx(monObject);
    runner->mon.monObject = monObject;
    info = PokeathlonCourseMonGfx_GetInfo(gfx);
    stats = PokeathlonCourse_GetMonStats(course, PokeathlonCourse_GetNetId(course), lane);
    runner->mon.riseFrames = table->rows[0][stats[2]];
    runner->mon.hangFrames = table->rows[1][stats[2]];
    runner->mon.jumpStat = stats[2];
    performance = PokeathlonCourse_GetCollisionSizeClass(PokeathlonCourse_GetPerformance(course, PokeathlonCourse_GetNetId(course), lane)) - 1;
    PokeathlonCourseObject_SetCollision(monObject, params->values[performance][0], params->values[performance][1]);
    PokeathlonCourseObject_SetTouchRadius(monObject, PokeathlonCourse_GetTouchRadius());
    runner->mon.topSpeed = table->rows[3][stats[1]] / 10.0f;
    runner->mon.acceleration = table->rows[2][stats[4]] / 100.0f;
    if (info->isLarge) {
        scale.y = scale.x = 0x20000;
    } else {
        scale.y = scale.x = 0x10000;
    }
    runner->mon.groundY = 120;
    PokeathlonCourseObject_SetGroundPosition(monObject, x, runner->mon.groundY + 272);
    PokeathlonCourseMonObject_SetAnimState(monObject, 1);
    PokeathlonCourseMonObject_SetDrawPriority(monObject, 4);
    PokeathlonCourseMonObject_SetAffineOverwriteMode(monObject, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    PokeathlonCourseMonObject_SetAffineMatrix(monObject, &scale);

    runner->mon.jumpMarker = PokeathlonCourseSpritePool_Take(scene, 102, 5);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(runner->mon.jumpMarker);
    runner->mon.markerY = runner->mon.groundY + 288;
    markerPosition.z = 0;
    markerPosition.x = x << FX32_SHIFT;
    markerPosition.y = runner->mon.markerY << FX32_SHIFT;
    Sprite_SetMatrix(sprite, &markerPosition);
    PokeathlonCourseSpritePoolSprite_Show(runner->mon.jumpMarker, TRUE, FALSE);
    Sprite_SetDrawPriority(sprite, 3);

    runner->mon.rankSprite = PokeathlonCourseSpritePool_Take(scene, 104, 7);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(runner->mon.rankSprite);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    rankPosition.z = 0;
    rankPosition.y = 428 << FX32_SHIFT;
    rankPosition.x = x << FX32_SHIFT;
    Sprite_SetMatrix(sprite, &rankPosition);
    Sprite_SetDrawPriority(sprite, 4);
    Sprite_SetAffineOverwriteMode(sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);

    runner->mon.boostEffect = PokeathlonCourseSpritePool_Take(scene, 105, 8);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(runner->mon.boostEffect);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    boostPosition.z = 0;
    boostPosition.y = 392 << FX32_SHIFT;
    boostPosition.x = x << FX32_SHIFT;
    Sprite_SetMatrix(sprite, &boostPosition);
    PokeathlonCourseSpritePoolSprite_Show(runner->mon.boostEffect, TRUE, TRUE);
    Sprite_SetDrawPriority(sprite, 2);
    runner->speed = 1.0f;
}
