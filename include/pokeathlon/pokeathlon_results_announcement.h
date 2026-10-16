#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RESULTS_ANNOUNCEMENT_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RESULTS_ANNOUNCEMENT_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "font_sprite.h"
#include "obj_char_transfer.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "sprite_system.h"
#include "string_util.h"

// The "Results Announcement" screen the course shows after an event (event template gPokeathlonCourseEvent_ResultsAnnouncement,
// states gPokeathlonResultsAnnouncementSteps): the announcer comments on the event (msg 309 #246..), the teams' plates slide in
// from last to first place, the winner is congratulated and the course points are counted up on the
// scoreboard (sprite digits) on the other screen.

// A text drawn as sprites, with its slot in the OBJ character VRAM.
typedef struct PokeathlonResultsAnnouncementText {
    TextOBJ *textObj;                 // 0x00
    ObjCharSpaceReservation charSlot; // 0x04
} PokeathlonResultsAnnouncementText;  // size: 0x10

// A window the sprite texts are rendered from.
typedef struct PokeathlonResultsAnnouncementTextWindow {
    Window window;                         // 0x00
    TextOBJLayout *layout;                 // 0x10
    u32 charSize;                          // 0x14
} PokeathlonResultsAnnouncementTextWindow; // size: 0x18

// The plate of one team in the ranking.
typedef struct PokeathlonResultsAnnouncementPlate {
    Sprite *sprites[4];                     // 0x00: plate (team color), rank, first-place mark, team picture
    PokeathlonResultsAnnouncementText name; // 0x10: player name
    PokeathlonResultsAnnouncementText text; // 0x20: the score text of the ranking
    BOOL active;                            // 0x30
    u32 index;                              // 0x34
    int x;                                  // 0x38
    int y;                                  // 0x3C
    u32 participant;                        // 0x40
    u16 rank;                               // 0x44: 0 = first place
    u8 unk46[2];                            // 0x46
} PokeathlonResultsAnnouncementPlate;       // size: 0x48

typedef struct PokeathlonResultsAnnouncementPlates {
    enum HeapID heapId;                                 // 0x000
    PokeathlonCourseData *course;                       // 0x004
    BgConfig *bgConfig;                                 // 0x008
    u32 unk00C;                                         // 0x00C
    FontSystem *fontSystem;                             // 0x010
    PokeathlonResultsAnnouncementPlate plates[5];       // 0x014
    SpriteList *spriteList;                             // 0x17C
    G2dRenderer renderer;                               // 0x180
    GF_2DGfxResMan *resourceManagers[6];                // 0x2A8
    SpriteResource *resources[4];                       // 0x2C0
    u8 unk2D0[8];                                       // 0x2D0
    SpriteResourcesHeader resourcesHeader;              // 0x2D8
    PokeathlonResultsAnnouncementTextWindow nameWindow; // 0x2FC
    PokeathlonResultsAnnouncementTextWindow textWindow; // 0x314
    BOOL firstPlace;                                    // 0x32C: the last plate shown is the winner's
    int nextPlate;                                      // 0x330
} PokeathlonResultsAnnouncementPlates;                  // size: 0x334

// A plate sliding in (or out).
typedef struct PokeathlonResultsAnnouncementSlide {
    BOOL active;                               // 0x00
    PokeathlonResultsAnnouncementPlate *plate; // 0x04
    int delay;                                 // 0x08: frames before it starts moving
    BOOL started;                              // 0x0C
} PokeathlonResultsAnnouncementSlide;

// Shows the ranking: the plates slide in from the right, from the last place up.
typedef struct PokeathlonResultsAnnouncementRanking {
    PokeathlonResultsAnnouncementPlates *plates;  // 0x00
    PokeathlonResultsAnnouncementSlide slides[4]; // 0x04
    PokeathlonResultsAnnouncementSlide incoming;  // 0x44
    int timer;                                    // 0x54
    u8 state;                                     // 0x58
    u8 total;                                     // 0x59: plates to show
    u8 count;                                     // 0x5A: plates shown
    u8 slide;                                     // 0x5B: next slide to leave
    u8 next;                                      // 0x5C: plates created
    u8 unk5D[3];                                  // 0x5D
    BOOL fanfarePlayed;                           // 0x60
} PokeathlonResultsAnnouncementRanking;           // size: 0x64

// One team on the scoreboard: name plate, three digits and the name.
typedef struct PokeathlonResultsAnnouncementScoreboardTeam {
    PokeathlonCourseSpritePoolSprite *plate;   // 0x00
    void *digits[3];                           // 0x04
    PokeathlonResultsAnnouncementText name;    // 0x10
    u32 points;                                // 0x20
    int y;                                     // 0x24
} PokeathlonResultsAnnouncementScoreboardTeam; // size: 0x28

// The course points of the four teams (sub screen).
typedef struct PokeathlonResultsAnnouncementScoreboard {
    enum HeapID heapId;                                   // 0x00
    PokeathlonCourseSpritePool *scene;                    // 0x04
    FontSystem *fontSystem;                               // 0x08
    PokeathlonResultsAnnouncementTextWindow window;       // 0x0C
    PokeathlonResultsAnnouncementScoreboardTeam teams[4]; // 0x24
} PokeathlonResultsAnnouncementScoreboard;                // size: 0xC4

typedef struct PokeathlonResultsAnnouncement {
    enum HeapID heapId;                                  // 0x00
    u32 unk04[2];                                        // 0x04
    BgConfig *bgConfig;                                  // 0x0C
    PokeathlonCourseSpritePool *scene;                   // 0x10
    PokeathlonResultsAnnouncementScoreboard *scoreboard; // 0x14
    PokeathlonCourseMessageBox *messageBox;              // 0x18: the announcer's text box
    PokeathlonResultsAnnouncementRanking ranking;        // 0x1C
    PokeathlonEventBanner *banner;                       // 0x80
    Window window;                                       // 0x84: "Results Announcement"
    SpriteSystem *spriteSystem;                          // 0x94
    SpriteManager *spriteManager;                        // 0x98
    ManagedSprite *sprites[2];                           // 0x9C
    u32 messageCount;                                    // 0xA4
    BOOL finished;                                       // 0xA8
} PokeathlonResultsAnnouncement;                         // size: 0xAC

// An entry of the course's ranking (PokeathlonCourseScoreList_GetRanked).
typedef struct PokeathlonResultsAnnouncementScore {
    u32 participant; // 0x00
    u8 unk04[5];     // 0x04
    u8 rank;         // 0x09
    u16 points;      // 0x0A
    s32 score;       // 0x0C
    String *text;    // 0x10
    String *name;    // 0x14
} PokeathlonResultsAnnouncementScore;

typedef struct PokeathlonResultsAnnouncementSceneConfig {
    u32 capacity;
    u32 resourceCount;
    u32 resCounts;
} PokeathlonResultsAnnouncementSceneConfig;

typedef struct PokeathlonResultsAnnouncementBgTemplates {
    BgTemplate templates[6];
} PokeathlonResultsAnnouncementBgTemplates;

typedef struct PokeathlonResultsAnnouncementAffine {
    int a;
    int b;
    int c;
    int d;
} PokeathlonResultsAnnouncementAffine;

void PokeathlonResults_InitWindow(Window *window, BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonResults_PrintTitle(Window *window, enum HeapID heapId);

BOOL PokeathlonResults_Init(PokeathlonCourseData *course);
BOOL PokeathlonResults_Draw(PokeathlonCourseData *course);
BOOL PokeathlonResults_Exit(PokeathlonCourseData *course);
BOOL PokeathlonResults_Run(PokeathlonCourseData *course, u8 *state);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RESULTS_ANNOUNCEMENT_H
