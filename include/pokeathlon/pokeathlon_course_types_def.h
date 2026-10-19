#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_TYPES_DEF_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_TYPES_DEF_H

#include "global.h"

#include "sprite.h"

// Structs shared by the Pokeathlon course and its events (the Pokemon objects are defined below, the rest in
// pokeathlon_course_objects.h, pokeathlon_course_internal.h and pokeathlon_course_ui.h).
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
typedef struct PokeathlonCourseMonGfxManager PokeathlonCourseMonGfxManager;
typedef struct PokeathlonCourseLink PokeathlonCourseLink;
typedef struct PokeathlonCoursePoint PokeathlonCoursePoint;
typedef struct PokeathlonCoursePerformance PokeathlonCoursePerformance;
typedef struct PokeathlonEventBanner PokeathlonEventBanner;
typedef struct PokeathlonCourseMessageBox PokeathlonCourseMessageBox;

struct PokeathlonCourseData;

// What the Pokemon sprite loader needs to know about one Pokemon (0x10).
struct PokeathlonCourseMonInfo {
    u16 species;     // 0x00
    u16 form;        // 0x02
    u16 isLarge;     // 0x04: overworld sprite uses the 64x64 frames (follower parameter byte 1)
    u8 isShiny;      // 0x06
    u8 gender;       // 0x07
    int participant; // 0x08
    u32 personality; // 0x0C
};

// A Pokemon on a course screen: its sprite plus a shadow and a player marker (0x44). Created by the course
// sprite factory (PokeathlonCourseMonObjects_Load).
struct PokeathlonCourseMonObject {
    PokeathlonCourseMonGfx *gfx; // the loaded Pokemon sprite
    Sprite *shadowSprite;
    Sprite *markerSprite;
    s32 markerOffsetY;
    s32 shadowOffsetY; // rows between the frame bottom and the ground point
    s32 halfWidth;
    s32 halfHeight;
    s32 touchRadius;
    s32 x; // centre
    s32 y;
    s32 topRow;           // first non-transparent pixel row of the frame
    s32 collisionOffsetY; // centre of the collision circle, rows above the frame bottom
    s32 collisionRadius;
    u32 animState;
    u32 anim;
    s32 drawPriorityStep;
    BOOL shadowHidden;
};

struct PokeathlonCourseMonObjects {
    enum HeapID heapId;                         // 0x000
    int count;                                  // 0x004
    int baseDrawPriority;                       // 0x008
    BOOL loaded;                                // 0x00C
    SpriteList *spriteList;                     // 0x010
    PokeathlonCourseMonGfxManager *gfxManager;  // 0x014
    PokeathlonCourseSpriteResources *resources; // 0x018
    PokeathlonCourseMonObject objects[12];      // 0x01C
};

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
