#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_TYPES_DEF_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_TYPES_DEF_H

#include "global.h"

// Structs shared by the Pokeathlon course and its events (defined in pokeathlon_course_objects.h,
// pokeathlon_course_internal.h and pokeathlon_course_ui.h).
typedef struct PokeathlonCourseObject PokeathlonCourseObject;
typedef struct PokeathlonCourseObjectInfo PokeathlonCourseObjectInfo;
typedef struct PokeathlonCourseObjects PokeathlonCourseObjects;
typedef struct PokeathlonCourseSpritePoolTemplate PokeathlonCourseSpritePoolTemplate;
typedef struct PokeathlonCourseSpritePoolSprite PokeathlonCourseSpritePoolSprite;
typedef struct PokeathlonCourseSpritePool PokeathlonCourseSpritePool;
typedef struct PokeathlonCourseMonInfo PokeathlonCourseMonInfo;
typedef struct PokeathlonCourseScore PokeathlonCourseScore;
typedef struct PokeathlonCourseScoreList PokeathlonCourseScoreList;
typedef struct PokeathlonCourseMonGfx PokeathlonCourseMonGfx;
typedef struct PokeathlonCourseMonGfxTemplate PokeathlonCourseMonGfxTemplate;
typedef struct PokeathlonCourseSpriteResources PokeathlonCourseSpriteResources;
typedef struct PokeathlonCourseMonObject PokeathlonCourseMonObject;
typedef struct PokeathlonCourseMonObjects PokeathlonCourseMonObjects;
typedef struct PokeathlonCourseLink PokeathlonCourseLink;
typedef struct PokeathlonCoursePoint PokeathlonCoursePoint;
typedef struct PokeathlonCoursePerformance PokeathlonCoursePerformance;
typedef struct PokeathlonEventBanner PokeathlonEventBanner;
typedef struct PokeathlonCourseMessageBox PokeathlonCourseMessageBox;

struct PokeathlonCourseData;

// What PokeathlonCourseSpritePool_New sets up: sprite and resource capacities.
struct PokeathlonCourseSpritePoolTemplate {
    int capacity;      // 0x00: sprites
    int resourceCount; // 0x04: resource ids
    u8 resCounts[4];   // 0x08: char, palette, cell, animation
};

// PokeathlonCourse_GetMode, PokeathlonCourseArgsLayout.mode
enum PokeathlonCourseMode {
    POKEATHLON_COURSE_MODE_SOLO,
    POKEATHLON_COURSE_MODE_LINK,
};

// One step of a course scene; returns TRUE when the step is done.
typedef BOOL (*PokeathlonCourseStepFunc)(struct PokeathlonCourseData *course, u8 *state);

// Steps of an event's play state: the start countdown, the event itself, then the finish
enum PokeathlonEventPlayState {
    EVENT_PLAY_STATE_COUNTDOWN,
    EVENT_PLAY_STATE_RUN,
    EVENT_PLAY_STATE_FINISH,
};

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_TYPES_DEF_H
