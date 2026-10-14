#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_DISPLAY_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_DISPLAY_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"

// Circle Push (Pokeathlon event 2): the top screen. It shows the player names with their team
// colors, the round counter and, in a loop while nothing else is shown, two idle animations
// (two Pokemon of different teams sliding past each other, and a zooming picture). Events of the
// match interrupt it: the scoreboard with the own team's points, and the picture of a Pokemon
// that scored.

// What PokeathlonCourse_GetMonInfo reports about one Pokemon of a team.
typedef struct PokeathlonCirclePushMonView {
    u16 species;     // 0x00
    u16 form;        // 0x02
    u16 unk04;       // 0x04
    u8 shiny;        // 0x06
    u8 gender;       // 0x07
    u32 participant; // 0x08
    u32 personality; // 0x0C
} PokeathlonCirclePushMonView;

// Front picture of one of the 12 Pokemon, uncompressed, for upload into a picture sprite.
typedef struct PokeathlonCirclePushMonPic {
    void *charData;    // 0x00
    void *paletteData; // 0x04
} PokeathlonCirclePushMonPic;

// Positions of the nameplate sprites (pixels).
typedef struct PokeathlonCirclePushNameplateCoords {
    u8 y[4];
    u8 x[4];
} PokeathlonCirclePushNameplateCoords;

// Vertical slide of a picture: in to hold, out to end (fx32 y).
typedef struct PokeathlonCirclePushPicKeys {
    fx32 start;
    fx32 hold;
    fx32 end;
} PokeathlonCirclePushPicKeys;

typedef struct PokeathlonCirclePushDisplay PokeathlonCirclePushDisplay;

typedef void (*PokeathlonCirclePushDisplayCallback)(PokeathlonCirclePushDisplay *display);

enum PokeathlonCirclePushDisplayPhase {
    CIRCLE_PUSH_DISPLAY_NONE,
    CIRCLE_PUSH_DISPLAY_SCOREBOARD,
    CIRCLE_PUSH_DISPLAY_MON_PIC,
    CIRCLE_PUSH_DISPLAY_MATCHUP,
    CIRCLE_PUSH_DISPLAY_ZOOM,
};

// The animated part of the top screen (sub BG 2 and 3 and the sprites).
typedef struct PokeathlonCirclePushDisplayAnim {
    BOOL active;                                // 0x00
    BOOL scoreboardShown;                       // 0x04
    u8 monShown[12];                            // 0x08: the picture of this Pokemon was shown
    PokeathlonCirclePushDisplayCallback update; // 0x14
    u16 idleCount;                              // 0x18: picks the next idle animation
    u16 frame;                                  // 0x1A
    u32 phase;                                  // 0x1C: enum PokeathlonCirclePushDisplayPhase
    u32 picImageLocations[2];                   // 0x20: VRAM of the two picture sprites
    u32 picPaletteLocations[2];                 // 0x28
    PokeathlonCirclePushMonPic monPics[12];     // 0x30: participant * 3 + team member
    void *picSprites[2];                        // 0x90
    NNSG2dScreenData *zoomBg3Screen;            // 0x98
    NNSG2dScreenData *matchupBg3Screen;         // 0x9C
    NNSG2dScreenData *zoomBg2Screen;            // 0xA0
    NNSG2dScreenData *scoreboardScreen;         // 0xA4
    NNSG2dScreenData *matchupBg2Screen;         // 0xA8
    NNSG2dScreenData *monPicScreen;             // 0xAC
    NNSG2dScreenData *digitsScreen;             // 0xB0: the digits 0-9 in two rows of five
    PokeathlonCourseSpritePoolSprite *unkB4;                                // 0xB4
    PokeathlonCourseSpritePoolSprite *roundSprite;                          // 0xB8
    void *scoreboardSprites[2];                 // 0xBC
    PokeathlonCourseSpritePoolSprite *zoomSprite;                           // 0xC4
    void *nameplateSprites[4];                  // 0xC8: per name window, in the team color
    void *playerEventSprites[4];                // 0xD8: per name window
    u8 score;                                   // 0xE8: points of the own team
    u8 shownScore;                              // 0xE9
    u8 scroll;                                  // 0xEA: sub BG 2 vertical offset while a picture is shown
    void *screenFiles[7];                       // 0xEC: the files of the screens above
} PokeathlonCirclePushDisplayAnim;

struct PokeathlonCirclePushDisplay {
    enum HeapID heapId;                   // 0x000
    PokeathlonCourseData *course;         // 0x004
    BgConfig *bgConfig;                   // 0x008
    Window windows[5];                    // 0x00C: player names (own name first), round counter
    MsgData *msgData;                     // 0x05C
    MessageFormat *msgFormat;             // 0x060
    u32 round;                            // 0x064
    PokeathlonCirclePushDisplayAnim anim; // 0x068
    u8 windowOfParticipant[4];            // 0x170
}; // size: 0x174

void PokeathlonCirclePushDisplay_CreateSprites(PokeathlonCourseSpritePool *scene, PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_PrintPlayerNames(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_SetRound(PokeathlonCirclePushDisplay *display, u32 round);
void PokeathlonCirclePushDisplay_ShowPlayerEvent(PokeathlonCirclePushDisplay *display, u32 participant, u32 kind);
void PokeathlonCirclePushDisplay_ResetAnim(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_GetPicVramLocations(PokeathlonCourseSpritePool *scene, PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadMonPics(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplayAnim_LoadMonPic(PokeathlonCirclePushDisplayAnim *anim, u32 slot, u32 mon);
void PokeathlonCirclePushDisplay_CreateWindows(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_PrintRound(PokeathlonCirclePushDisplay *display, u32 round);
void PokeathlonCirclePushDisplay_Update(PokeathlonCirclePushDisplay *display, BOOL showScoreboard, const u8 *monScored);
void PokeathlonCirclePushDisplay_SetScore(PokeathlonCirclePushDisplay *display, u32 score, u32 participant);
u8 PokeathlonCirclePushDisplay_GetScore(PokeathlonCirclePushDisplay *display);
int PokeathlonCirclePushDisplayAnim_TakeNewMon(PokeathlonCirclePushDisplayAnim *anim, const u8 *monScored);
BOOL PokeathlonCirclePushDisplayAnim_CanStart(PokeathlonCirclePushDisplayAnim *anim, int phase);
void PokeathlonCirclePushDisplayAnim_Stop(PokeathlonCirclePushDisplayAnim *anim);
void PokeathlonCirclePushDisplayAnim_HideSprites(PokeathlonCirclePushDisplayAnim *anim);
BOOL PokeathlonCirclePushDisplayAnim_TryStart(PokeathlonCirclePushDisplayAnim *anim, int phase);
void PokeathlonCirclePushDisplayAnim_StartScoreboard(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig);
void PokeathlonCirclePushDisplayAnim_StartMonPic(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig, int mon);
void PokeathlonCirclePushDisplayAnim_StartMatchup(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig);
void PokeathlonCirclePushDisplayAnim_StartZoom(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig);
void PokeathlonCirclePushDisplayAnim_StartIdle(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig);
void PokeathlonCirclePushDisplay_UpdateScoreboard(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_UpdateMonPic(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_UpdateMatchup(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_UpdateZoom(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplayAnim_SetupMatchup(PokeathlonCirclePushDisplayAnim *anim);
void PokeathlonCirclePushDisplayAnim_MovePicAlongPath(u8 frame, const VecFx32 *path, PokeathlonCourseSpritePoolSprite *picSprite);
void PokeathlonCirclePushDisplayAnim_SetupMonPic(PokeathlonCirclePushDisplayAnim *anim, int mon);
void PokeathlonCirclePushDisplayAnim_MovePicVertically(u8 frame, const PokeathlonCirclePushPicKeys *keys, PokeathlonCourseSpritePoolSprite *picSprite);
void PokeathlonCirclePushDisplayAnim_PrintScore(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig);
void PokeathlonCirclePushDisplay_OffsetScreenTiles(int offset, NNSG2dScreenData *screen);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_DISPLAY_H
