#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RESULTS_ANNOUNCEMENT_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RESULTS_ANNOUNCEMENT_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "font_sprite.h"
#include "obj_char_transfer.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "unk_02009D48.h"

// The "Results Announcement" screen the course shows after an event (event template ov96_0221A86C,
// states ov96_0221DC24): the announcer comments on the event (msg 309 #246..), the teams' plates slide in
// from last to first place, the winner is congratulated and the course points are counted up on the
// scoreboard (sprite digits) on the other screen.

// A text drawn as sprites, with its slot in the OBJ character VRAM.
typedef struct PokeathlonResultsAnnouncementText {
    TextOBJ *textObj;                // 0x00
    UnkStruct_02021AC8 charSlot;     // 0x04
} PokeathlonResultsAnnouncementText; // size: 0x10

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
    void *plate;                               // 0x00
    void *digits[3];                           // 0x04
    PokeathlonResultsAnnouncementText name;    // 0x10
    u32 points;                                // 0x20
    int y;                                     // 0x24
} PokeathlonResultsAnnouncementScoreboardTeam; // size: 0x28

// The course points of the four teams (sub screen).
typedef struct PokeathlonResultsAnnouncementScoreboard {
    enum HeapID heapId;                                   // 0x00
    void *scene;                                          // 0x04
    FontSystem *fontSystem;                               // 0x08
    PokeathlonResultsAnnouncementTextWindow window;       // 0x0C
    PokeathlonResultsAnnouncementScoreboardTeam teams[4]; // 0x24
} PokeathlonResultsAnnouncementScoreboard;                // size: 0xC4

typedef struct PokeathlonResultsAnnouncement {
    enum HeapID heapId;                                  // 0x00
    u32 unk04[2];                                        // 0x04
    BgConfig *bgConfig;                                  // 0x0C
    void *scene;                                         // 0x10
    PokeathlonResultsAnnouncementScoreboard *scoreboard; // 0x14
    void *messageBox;                                    // 0x18: the announcer's text box
    PokeathlonResultsAnnouncementRanking ranking;        // 0x1C
    void *unk80;                                         // 0x80
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
    u32 unk00;
    u32 unk04;
    u32 unk08;
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

// in pokeathlon_lamp_jump_stage.c
void PokeathlonResultsAnnouncement_InitWindow(Window *window, BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonResultsAnnouncement_PrintTitle(Window *window, enum HeapID heapId);

BOOL PokeathlonResultsAnnouncement_Init(PokeathlonCourseData *course);
BOOL PokeathlonResultsAnnouncement_Draw(PokeathlonCourseData *course);
BOOL PokeathlonResultsAnnouncement_Exit(PokeathlonCourseData *course);
void PokeathlonResultsAnnouncement_SetGfxBanks(void);
void PokeathlonResultsAnnouncement_InitBgs(BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonResultsAnnouncement_LoadBgGfx(PokeathlonResultsAnnouncement *announcement);
BOOL PokeathlonResultsAnnouncement_AllPlayersFinished(PokeathlonCourseData *course);
BOOL PokeathlonResultsAnnouncement_Run(PokeathlonCourseData *course, u8 *state);
void PokeathlonResultsAnnouncement_CreatePlates(PokeathlonResultsAnnouncementPlates **plates, enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonResultsAnnouncement_StartRanking(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonCourseData *course, u8 total, int unused);
void PokeathlonResultsAnnouncement_MoveSlide(PokeathlonResultsAnnouncementSlide *slide, int dx, int dy);
void PokeathlonResultsAnnouncement_MoveSlides(PokeathlonResultsAnnouncementRanking *ranking, int dx, int dy);
BOOL PokeathlonResultsAnnouncement_IsSlideDone(PokeathlonResultsAnnouncementSlide *slide, int direction);
void PokeathlonResultsAnnouncement_Wait(PokeathlonResultsAnnouncementRanking *ranking, int frames);
BOOL PokeathlonResultsAnnouncement_UpdateRanking(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonCourseData *course);
void PokeathlonResultsAnnouncement_QueueSlide(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonResultsAnnouncementPlate *plate, int delay);
void PokeathlonResultsAnnouncement_SetIncomingSlide(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonResultsAnnouncementPlate *plate, int delay);
void PokeathlonResultsAnnouncement_ClearSlide(PokeathlonResultsAnnouncementSlide *slide);
BOOL PokeathlonResultsAnnouncement_HasMorePlates(PokeathlonResultsAnnouncementRanking *ranking);
PokeathlonResultsAnnouncementPlate *PokeathlonResultsAnnouncement_AddPlate(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonCourseData *course, int x, int y);
void PokeathlonResultsAnnouncement_RemoveSlide(PokeathlonResultsAnnouncementRanking *ranking, PokeathlonResultsAnnouncementSlide *slide);
PokeathlonResultsAnnouncementSlide *PokeathlonResultsAnnouncement_GetLeavingSlide(PokeathlonResultsAnnouncementRanking *ranking);
u32 PokeathlonResultsAnnouncement_GetIntroMessage(PokeathlonCourseData *course);
u32 PokeathlonResultsAnnouncement_GetCommentMessage(PokeathlonCourseData *course);
void PokeathlonResultsAnnouncement_InitSprites(PokeathlonResultsAnnouncement *announcement);
void PokeathlonResultsAnnouncement_FreeSprites(PokeathlonResultsAnnouncement *announcement);
void PokeathlonResultsAnnouncement_LoadSpriteResources(PokeathlonResultsAnnouncement *announcement);
void PokeathlonResultsAnnouncement_CreateSprite(PokeathlonResultsAnnouncement *announcement);
BOOL PokeathlonResultsAnnouncement_WaitForPlayers(PokeathlonCourseData *course);
PokeathlonResultsAnnouncementPlates *PokeathlonResultsAnnouncement_NewPlates(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonResultsAnnouncement_FreePlates(PokeathlonResultsAnnouncementPlates *plates);
void PokeathlonResultsAnnouncement_DrawPlates(PokeathlonResultsAnnouncementPlates *plates);
void PokeathlonResultsAnnouncement_InitPlates(PokeathlonResultsAnnouncementPlates *plates, int unused);
PokeathlonResultsAnnouncementPlate *PokeathlonResultsAnnouncement_ShowPlate(PokeathlonResultsAnnouncementPlates *plates, int x, int y, PokeathlonResultsAnnouncementScore *score);
void PokeathlonResultsAnnouncement_HidePlate(PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResultsAnnouncement_GetPlatePos(PokeathlonResultsAnnouncementPlate *plate, int *x, int *y);
void PokeathlonResultsAnnouncement_SetPlatePos(PokeathlonResultsAnnouncementPlate *plate, int x, int y);
void PokeathlonResultsAnnouncement_MovePlate(PokeathlonResultsAnnouncementPlate *plate, int dx, int dy);
void PokeathlonResultsAnnouncement_LoadPlateResources(PokeathlonResultsAnnouncementPlates *plates);
void PokeathlonResultsAnnouncement_InitPlateResourcesHeader(PokeathlonResultsAnnouncementPlates *plates);
void PokeathlonResultsAnnouncement_InitPlateSpriteTemplate(SpriteTemplate *template, PokeathlonResultsAnnouncementPlates *plates, SpriteResourcesHeader *unused, NNS_G2D_VRAM_TYPE whichScreen);
Sprite *PokeathlonResultsAnnouncement_CreatePlateSprite(SpriteTemplate *template, int anim, int priority);
void PokeathlonResultsAnnouncement_CreatePlateSprites(PokeathlonResultsAnnouncementPlates *plates, int index, Sprite **sprites);
void PokeathlonResultsAnnouncement_PrintPlateName(PokeathlonResultsAnnouncementPlates *plates, int participant, PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResultsAnnouncement_PrintPlateText(PokeathlonResultsAnnouncementPlates *plates, String *string, PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResultsAnnouncement_RefreshPlate(PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResultsAnnouncement_SetPlateVisible(BOOL firstPlace, BOOL draw, PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResultsAnnouncement_SetPlateSpritePositions(int x, int y, PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResultsAnnouncement_MovePlateSprites(int dx, int dy, PokeathlonResultsAnnouncementPlate *plate);
void PokeathlonResultsAnnouncement_InitTextWindow(PokeathlonResultsAnnouncementPlates *plates, PokeathlonResultsAnnouncementTextWindow *window, int width);
void PokeathlonResultsAnnouncement_InitTextWindows(PokeathlonResultsAnnouncementPlates *plates);
void PokeathlonResultsAnnouncement_CreateText(PokeathlonResultsAnnouncementPlates *plates, PokeathlonResultsAnnouncementTextWindow *window, String *string, int x, int dx, int dy, Sprite *parent, PokeathlonResultsAnnouncementText *out);
void PokeathlonResultsAnnouncement_DestroyText(PokeathlonResultsAnnouncementText *text);
PokeathlonResultsAnnouncementScoreboard *PokeathlonResultsAnnouncement_NewScoreboard(enum HeapID heapId, BgConfig *bgConfig, void *scene);
void PokeathlonResultsAnnouncement_FreeScoreboard(PokeathlonResultsAnnouncementScoreboard *scoreboard);
void PokeathlonResultsAnnouncement_LoadScoreboardResources(PokeathlonResultsAnnouncementScoreboard *scoreboard, s32 a1);
void PokeathlonResultsAnnouncement_InitScoreboard(PokeathlonResultsAnnouncementScoreboard *scoreboard, PokeathlonCourseData *course, int y);
void PokeathlonResultsAnnouncement_SetScore(PokeathlonResultsAnnouncementScoreboard *scoreboard, u32 points, u32 team);
void PokeathlonResultsAnnouncement_CreateScoreboardText(PokeathlonResultsAnnouncementScoreboard *scoreboard, PokeathlonResultsAnnouncementTextWindow *window, String *string, int x, int dx, int dy, Sprite *parent, PokeathlonResultsAnnouncementText *out);
void PokeathlonResultsAnnouncement_DestroyScoreboardText(PokeathlonResultsAnnouncementText *text);
void PokeathlonResultsAnnouncement_SetScoreDigits(PokeathlonResultsAnnouncementScoreboardTeam *team, s32 points);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RESULTS_ANNOUNCEMENT_H
