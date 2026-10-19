#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_INTRO_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_INTRO_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_ui.h"
#include "pokeathlon/pokeathlon_util.h"

#include "bg_window.h"
#include "filesystem.h"
#include "sprite_system.h"

// Two course screens: the link screen "Waiting for everyone's selection..." and the introduction of the next
// event, with its banner, rules and best record and the "Rules" and "Controls" pages.

typedef struct PokeathlonCourseWait {
    enum HeapID heapId;
    BgConfig *bgConfig;
    PokeathlonCourseMessageBox *messageBox;
    int timer;
} PokeathlonCourseWait;

enum PokeathlonEventIntroPage {
    EVENT_INTRO_PAGE_DETAILS = 1,
    EVENT_INTRO_PAGE_RULES,
    EVENT_INTRO_PAGE_CONTROLS,
};

typedef struct PokeathlonEventIntro {
    PokeathlonCourseData *course;
    BgConfig *bgConfig;
    NARC *narc; // a/2/1/9
    enum HeapID heapId;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u32 page; // enum PokeathlonEventIntroPage
    int timer;
    u8 event;
    u8 animStep; // animation of the event's mascot
    u8 round;
    u8 unk23;
    BOOL linkMode;
    BOOL showRecord;
    BOOL zooming;
    PokeathlonTextContext *text;
    PokeathlonEventBanner *banner;
    ManagedSprite *sprites[4]; // buttons, mascot, Pokemon
} PokeathlonEventIntro;

BOOL PokeathlonCourseWait_Init(PokeathlonCourseData *course);
BOOL PokeathlonCourseWait_Main(void);
BOOL PokeathlonCourseWait_Exit(PokeathlonCourseData *course);
BOOL PokeathlonCourseWait_FadeOut(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_Init(PokeathlonCourseData *course);
BOOL PokeathlonEventIntro_Main(PokeathlonCourseData *course);
BOOL PokeathlonEventIntro_Exit(PokeathlonCourseData *course);
BOOL PokeathlonEventIntro_Appear(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_DetailsPage(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_RulesPage(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_ControlsPage(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_FadeOut(PokeathlonCourseData *course, u8 *state);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_INTRO_H
