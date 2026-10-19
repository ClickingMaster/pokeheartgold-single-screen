#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_PLAY_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_PLAY_H

#include "global.h"

#include "pokeathlon/pokeathlon_disc_catch.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite_system.h"

// Disc Catch play: the host's field and discs, the frame packet both ways, the sprites shown from it,
// the top screen scoreboard and the CPU of the participants without a player.

#define DISC_CATCH_ABS(x) ((x) < 0 ? -(x) : (x))

// Where a disc was caught, and by whom.
typedef struct PokeathlonDiscCatchCatcher {
    s16 x; // touch screen pixels
    s16 y;
    u8 participant;
    u8 member;
} PokeathlonDiscCatchCatcher;

enum PokeathlonDiscCatchScoreboardSprite {
    DISC_CATCH_SCOREBOARD_CLOCK,
    DISC_CATCH_SCOREBOARD_SECONDS,                                    // two digits
    DISC_CATCH_SCOREBOARD_BADGES = DISC_CATCH_SCOREBOARD_SECONDS + 2, // one per participant
    DISC_CATCH_SCOREBOARD_MARKS = DISC_CATCH_SCOREBOARD_BADGES + 4,   // 12 rows of two down the sides
    DISC_CATCH_SCOREBOARD_SPRITES = DISC_CATCH_SCOREBOARD_MARKS + 24,
};

// The top screen: the players' names, the own score and the seconds left.
struct PokeathlonDiscCatchScoreboard {
    PokeathlonCourseData *course;
    enum HeapID heapId;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    MsgData *msgData;
    MessageFormat *messageFormat;
    u8 participant; // the own one
    void *digitsFile;
    NNSG2dScreenData *digits;                // the score digits, copied to BG 6
    Window windows[DISC_CATCH_PARTICIPANTS]; // names (the own one first)
    ManagedSprite *sprites[DISC_CATCH_SCOREBOARD_SPRITES];
    u8 secondsColor; // 0, 1 at 20 seconds, 2 at 3
    u8 wraps;        // the packet's score is 7 bits
    u16 shownScore;  // in the packet's 7 bits
};

// The CPU's steering of one Pokemon: it waits at a home spot, chases a disc near it and jumps at it.
typedef struct PokeathlonDiscCatchCpuMon {
    PokeathlonCourseData *course;
    s16 targetX; // where it is sent (with targetY)
    s16 targetY; // its home row
    s16 homeX;
    u32 chasing : 1;
    u32 disc : 2;
    u32 participant : 2;
    u32 member : 2;
    u32 state : 4;
    u32 thinkTimer : 8;
    u32 unkC_19 : 13;
    u8 homeTimer;
} PokeathlonDiscCatchCpuMon;

struct PokeathlonDiscCatchCpu {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    PokeathlonDiscCatchCpuMon teams[DISC_CATCH_TEAM_SIZE][DISC_CATCH_TEAM_SIZE]; // at most 3 CPU participants
    u8 count;
};

BOOL PokeathlonDiscCatch_UpdateFrame(PokeathlonCourseData *course);
PokeathlonDiscCatchStats *PokeathlonDiscCatch_GetStats(PokeathlonCourseData *course, u32 participant, u32 member);
u16 PokeathlonDiscCatchStats_GetJumpDuration(PokeathlonDiscCatchStats *stats);
void PokeathlonDiscCatch_CreateMonPics(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_InitMons(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_UpdateField(PokeathlonDiscCatchTeam *teams, PokeathlonCourseData *course);
void PokeathlonDiscCatch_WriteFramePacket(PokeathlonDiscCatchField *field, PokeathlonCourseData *course);
void PokeathlonDiscCatchDiscs_Init(PokeathlonDiscCatchDiscs *discs, PokeathlonDiscCatchTeam *teams);
void PokeathlonDiscCatchDiscs_Update(PokeathlonDiscCatchDiscs *discs, u8 *area, u32 seconds);
void PokeathlonDiscCatchSprites_Init(PokeathlonDiscCatchSprites *sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonDiscCatchScoreboard *scoreboard);
void PokeathlonDiscCatchSprites_Update(PokeathlonDiscCatchSprites *sprites, PokeathlonCourseData *course);
void PokeathlonDiscCatchSprites_Delete(PokeathlonDiscCatchSprites *sprites);
void PokeathlonDiscCatch_AddScoreWindow(PokeathlonDiscCatch *work);

PokeathlonDiscCatchScoreboard *PokeathlonDiscCatchScoreboard_New(SpriteSystem *spriteSystem, SpriteManager *spriteManager, BgConfig *bgConfig, MsgData *msgData, MessageFormat *messageFormat, u8 participant, enum HeapID heapId, PokeathlonCourseData *course);
void PokeathlonDiscCatchScoreboard_Delete(PokeathlonDiscCatchScoreboard *scoreboard);
void PokeathlonDiscCatchScoreboard_Pause(PokeathlonDiscCatchScoreboard *scoreboard);
u8 PokeathlonDiscCatchScoreboard_SetOwnScore(PokeathlonDiscCatchScoreboard *scoreboard, u32 participant, u16 score);
BOOL PokeathlonDiscCatchScoreboard_OnScore(PokeathlonDiscCatchScoreboard *scoreboard, u32 participant, u16 score);
void PokeathlonDiscCatchScoreboard_SetSeconds(PokeathlonDiscCatchScoreboard *scoreboard, int seconds);
u16 PokeathlonDiscCatchScoreboard_GetOwnScore(PokeathlonDiscCatchScoreboard *scoreboard);

PokeathlonDiscCatchCpu *PokeathlonDiscCatchCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonDiscCatchCpu_Delete(PokeathlonDiscCatchCpu *cpu);
void PokeathlonDiscCatchCpu_Update(PokeathlonDiscCatchCpu *cpu);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_PLAY_H
