#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_INTRO_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_INTRO_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_ui.h"
#include "pokeathlon/pokeathlon_util.h"

#include "bg_window.h"
#include "filesystem.h"
#include "sprite_system.h"

// Two course screens: the link screen "Waiting for everyone's selection..." (msg 309 #310), and
// the introduction of the next event (msg 309 #197-245): its banner, number, rules and best
// record on the bottom screen, and the pages "Rules" and "Controls" behind the touch buttons.

typedef struct PokeathlonCourseWait {
    enum HeapID heapId;                     // 0x00
    BgConfig *bgConfig;                     // 0x04
    PokeathlonCourseMessageBox *messageBox; // 0x08
    int timer;                              // 0x0C
} PokeathlonCourseWait;

enum PokeathlonEventIntroPage {
    EVENT_INTRO_PAGE_DETAILS = 1,
    EVENT_INTRO_PAGE_RULES,
    EVENT_INTRO_PAGE_CONTROLS,
};

typedef struct PokeathlonEventIntro {
    PokeathlonCourseData *course;  // 0x00
    BgConfig *bgConfig;            // 0x04
    NARC *narc;                    // 0x08: a/2/1/9
    enum HeapID heapId;            // 0x0C
    SpriteSystem *spriteSystem;    // 0x10
    SpriteManager *spriteManager;  // 0x14
    u32 page;                      // 0x18: enum PokeathlonEventIntroPage
    int timer;                     // 0x1C
    u8 event;                      // 0x20
    u8 animStep;                   // 0x21: animation of the event's mascot
    u8 round;                      // 0x22
    u8 unk23;                      // 0x23
    BOOL linkMode;                 // 0x24
    BOOL showRecord;               // 0x28
    BOOL zooming;                  // 0x2C
    PokeathlonTextContext *text;   // 0x30
    PokeathlonEventBanner *banner; // 0x34
    ManagedSprite *sprites[4];     // 0x38: buttons, mascot, Pokemon
} PokeathlonEventIntro;            // size: 0x48

void PokeathlonCourse_LoadSpritePalette(ManagedSprite *sprite, NarcId narcId, int fileId, BOOL mainScreen, BOOL grayScale, enum HeapID heapId);
int PokeathlonCourse_GetNpcTrainerPicFileId(int trainerClass);
BOOL PokeathlonCourseWait_Init(PokeathlonCourseData *course);
BOOL PokeathlonCourseWait_Main(void);
BOOL PokeathlonCourseWait_Exit(PokeathlonCourseData *course);
void PokeathlonCourseWait_SetBanks(void);
void PokeathlonCourseWait_VBlankCallback(void *data);
void PokeathlonCourseWait_InitBgs(BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonCourseWait_FreeBgs(PokeathlonCourseWait *wait);
void PokeathlonCourseWait_Create(PokeathlonCourseData *course);
void PokeathlonCourseWait_Free(PokeathlonCourseData *course);
BOOL PokeathlonCourseWait_FadeOut(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_Init(PokeathlonCourseData *course);
BOOL PokeathlonEventIntro_Main(PokeathlonCourseData *course);
BOOL PokeathlonEventIntro_Exit(PokeathlonCourseData *course);
void PokeathlonEventIntro_VBlankCallback(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_SetBanks(void);
BOOL PokeathlonEventIntro_IsBgmAt(u32 variable, s16 value);
u8 PokeathlonEventIntro_GetEvent(PokeathlonCourseData *course);
BOOL PokeathlonEventIntro_HasFlag0EF(PokeathlonCourseData *course);
void PokeathlonEventIntro_Create(PokeathlonCourseData *course);
void PokeathlonEventIntro_LoadDetailsBg(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_LoadPagePicture(PokeathlonEventIntro *intro, int event);
void PokeathlonEventIntro_InitBgs(BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonEventIntro_LoadGraphics(PokeathlonEventIntro *intro);
u16 PokeathlonEventIntro_GetRecord(PokeathlonEventIntro *intro, int event);
void PokeathlonEventIntro_PrintDetails(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_PrintRules(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_PrintControls(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_InitSprites(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_LoadSpriteResources(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_UpdateMascot(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_PlaceSprites(PokeathlonEventIntro *intro, int layout);
void PokeathlonEventIntro_ResetMascot(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_CreateSprites(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_SetMascotAnim(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_HideZoomBgs(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_StartZoom(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_UpdateZoom(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_FadeToPage(PokeathlonCourseData *course, int page);
BOOL PokeathlonEventIntro_IsFadeFinished(PokeathlonCourseData *course);
void PokeathlonEventIntro_ShowDetails(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_ShowRules(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_ShowControls(PokeathlonEventIntro *intro);
void PokeathlonEventIntro_ShowPage(PokeathlonCourseData *course);
BOOL PokeathlonEventIntro_Appear(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_DetailsPage(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_RulesPage(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_ControlsPage(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonEventIntro_FadeOut(PokeathlonCourseData *course, u8 *state);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_EVENT_INTRO_H
