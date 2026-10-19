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

// The results screen the course shows after an event: the announcer comments on the event, the teams' plates slide
// in from last to first place and the course points are counted up on the other screen.

// A text drawn as sprites, with its slot in the OBJ character VRAM.
typedef struct PokeathlonResultsAnnouncementText {
    TextOBJ *textObj;
    ObjCharSpaceReservation charSlot;
} PokeathlonResultsAnnouncementText;

// A window the sprite texts are rendered from.
typedef struct PokeathlonResultsAnnouncementTextWindow {
    Window window;
    TextOBJLayout *layout;
    u32 charSize;
} PokeathlonResultsAnnouncementTextWindow;

// The plate of one team in the ranking.
typedef struct PokeathlonResultsAnnouncementPlate {
    Sprite *sprites[4];                     // plate (team color), rank, first-place mark, team picture
    PokeathlonResultsAnnouncementText name; // player name
    PokeathlonResultsAnnouncementText text; // the score text of the ranking
    BOOL active;
    u32 index;
    int x;
    int y;
    u32 participant;
    u16 rank; // 0 = first place
    u8 unk46[2];
} PokeathlonResultsAnnouncementPlate;

typedef struct PokeathlonResultsAnnouncementPlates {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    BgConfig *bgConfig;
    u32 unk00C;
    FontSystem *fontSystem;
    PokeathlonResultsAnnouncementPlate plates[5];
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resourceManagers[6];
    SpriteResource *resources[4];
    u8 unk2D0[8];
    SpriteResourcesHeader resourcesHeader;
    PokeathlonResultsAnnouncementTextWindow nameWindow;
    PokeathlonResultsAnnouncementTextWindow textWindow;
    BOOL firstPlace; // the last plate shown is the winner's
    int nextPlate;
} PokeathlonResultsAnnouncementPlates;

// A plate sliding in (or out).
typedef struct PokeathlonResultsAnnouncementSlide {
    BOOL active;
    PokeathlonResultsAnnouncementPlate *plate;
    int delay; // frames before it starts moving
    BOOL started;
} PokeathlonResultsAnnouncementSlide;

// Shows the ranking: the plates slide in from the right, from the last place up.
typedef struct PokeathlonResultsAnnouncementRanking {
    PokeathlonResultsAnnouncementPlates *plates;
    PokeathlonResultsAnnouncementSlide slides[4];
    PokeathlonResultsAnnouncementSlide incoming;
    int timer;
    u8 state;
    u8 total; // plates to show
    u8 count; // plates shown
    u8 slide; // next slide to leave
    u8 next;  // plates created
    u8 unk5D[3];
    BOOL fanfarePlayed;
} PokeathlonResultsAnnouncementRanking;

// One team on the scoreboard: name plate, three digits and the name.
typedef struct PokeathlonResultsAnnouncementScoreboardTeam {
    PokeathlonCourseSpritePoolSprite *plate;
    void *digits[3];
    PokeathlonResultsAnnouncementText name;
    u32 points;
    int y;
} PokeathlonResultsAnnouncementScoreboardTeam;

// The course points of the four teams (sub screen).
typedef struct PokeathlonResultsAnnouncementScoreboard {
    enum HeapID heapId;
    PokeathlonCourseSpritePool *scene;
    FontSystem *fontSystem;
    PokeathlonResultsAnnouncementTextWindow window;
    PokeathlonResultsAnnouncementScoreboardTeam teams[4];
} PokeathlonResultsAnnouncementScoreboard;

typedef struct PokeathlonResultsAnnouncement {
    enum HeapID heapId;
    u32 unk04[2];
    BgConfig *bgConfig;
    PokeathlonCourseSpritePool *scene;
    PokeathlonResultsAnnouncementScoreboard *scoreboard;
    PokeathlonCourseMessageBox *messageBox; // the announcer's text box
    PokeathlonResultsAnnouncementRanking ranking;
    PokeathlonEventBanner *banner;
    Window window; // "Results Announcement"
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[2];
    u32 messageCount;
    BOOL finished;
} PokeathlonResultsAnnouncement;

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

u16 PokeathlonResults_GetPlateRank();
PokeathlonResultsAnnouncementPlates *PokeathlonResults_NewPlates(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonResults_FreePlates(PokeathlonResultsAnnouncementPlates *plates);
void PokeathlonResults_DrawPlates(PokeathlonResultsAnnouncementPlates *plates);
void PokeathlonResults_InitPlates(PokeathlonResultsAnnouncementPlates *plates, int unused);
PokeathlonResultsAnnouncementPlate *PokeathlonResults_ShowPlate(PokeathlonResultsAnnouncementPlates *plates, int x, int y, PokeathlonCourseScore *score);
void PokeathlonResults_HidePlate(PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResults_GetPlatePos(PokeathlonResultsAnnouncementPlate *plate, int *x, int *y);
void PokeathlonResults_SetPlatePos(PokeathlonResultsAnnouncementPlate *plate, int x, int y);
void PokeathlonResults_MovePlate(PokeathlonResultsAnnouncementPlate *plate, int dx, int dy);
PokeathlonResultsAnnouncementScoreboard *PokeathlonResults_NewScoreboard(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseSpritePool *scene);
void PokeathlonResults_FreeScoreboard(PokeathlonResultsAnnouncementScoreboard *scoreboard);
void PokeathlonResults_LoadScoreboardResources(PokeathlonResultsAnnouncementScoreboard *scoreboard, s32 index);
void PokeathlonResults_InitScoreboard(PokeathlonResultsAnnouncementScoreboard *scoreboard, PokeathlonCourseData *course, int y);
void PokeathlonResults_SetScore(PokeathlonResultsAnnouncementScoreboard *scoreboard, u32 points, u32 team);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RESULTS_ANNOUNCEMENT_H
