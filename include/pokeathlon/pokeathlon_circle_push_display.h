#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_DISPLAY_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_DISPLAY_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"

// Circle Push: the top screen, with the player names, the round counter, the scoreboard and the idle animations
// shown while nothing else is.

// Front picture of one of the 12 Pokemon, uncompressed, for upload into a picture sprite.
typedef struct PokeathlonCirclePushMonPic {
    void *charData;
    void *paletteData;
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
    BOOL active;
    BOOL scoreboardShown;
    u8 monShown[12]; // the picture of this Pokemon was shown
    PokeathlonCirclePushDisplayCallback update;
    u16 idleCount; // picks the next idle animation
    u16 frame;
    u32 phase;                // enum PokeathlonCirclePushDisplayPhase
    u32 picImageLocations[2]; // VRAM of the two picture sprites
    u32 picPaletteLocations[2];
    PokeathlonCirclePushMonPic monPics[12]; // participant * 3 + team member
    void *picSprites[2];
    NNSG2dScreenData *zoomBg3Screen;
    NNSG2dScreenData *matchupBg3Screen;
    NNSG2dScreenData *zoomBg2Screen;
    NNSG2dScreenData *scoreboardScreen;
    NNSG2dScreenData *matchupBg2Screen;
    NNSG2dScreenData *monPicScreen;
    NNSG2dScreenData *digitsScreen; // the digits 0-9 in two rows of five
    PokeathlonCourseSpritePoolSprite *unkB4;
    PokeathlonCourseSpritePoolSprite *roundSprite;
    void *scoreboardSprites[2];
    PokeathlonCourseSpritePoolSprite *zoomSprite;
    void *nameplateSprites[4];   // per name window, in the team color
    void *playerEventSprites[4]; // per name window
    u8 score;                    // points of the own team
    u8 shownScore;
    u8 scroll;            // sub BG 2 vertical offset while a picture is shown
    void *screenFiles[7]; // the files of the screens above
} PokeathlonCirclePushDisplayAnim;

struct PokeathlonCirclePushDisplay {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    BgConfig *bgConfig;
    Window windows[5]; // player names (own name first), round counter
    MsgData *msgData;
    MessageFormat *msgFormat;
    u32 round;
    PokeathlonCirclePushDisplayAnim anim;
    u8 windowOfParticipant[4];
};

void PokeathlonCirclePushDisplay_CreateSprites(PokeathlonCourseSpritePool *scene, PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_PrintPlayerNames(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_SetRound(PokeathlonCirclePushDisplay *display, u32 round);
void PokeathlonCirclePushDisplay_ShowPlayerEvent(PokeathlonCirclePushDisplay *display, u32 participant, u32 kind);
void PokeathlonCirclePushDisplay_GetPicVramLocations(PokeathlonCourseSpritePool *scene, PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadMonPics(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_CreateWindows(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_Update(PokeathlonCirclePushDisplay *display, BOOL showScoreboard, const u8 *monScored);
void PokeathlonCirclePushDisplay_SetScore(PokeathlonCirclePushDisplay *display, u32 score, u32 participant);
u8 PokeathlonCirclePushDisplay_GetScore(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplayAnim_PrintScore(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig);
void PokeathlonCirclePushDisplay_OffsetScreenTiles(int offset, NNSG2dScreenData *screen);

// The Pokemon picture sprites of the display.
void PokeathlonCirclePushDisplay_CreatePicSprites(PokeathlonCourseSpritePool *pool, PokeathlonCirclePushDisplay *display);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_DISPLAY_H
