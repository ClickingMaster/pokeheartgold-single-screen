#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Block Smash: every participant breaks stacks of ten blocks by tapping them; each hit takes the active Pokemon's
// power off the top block's health, more when the weak spot marked on the block is tapped.

// PokeathlonBlockSmashBlocks.state
enum {
    BLOCKS_STATE_DROPPING = 1,
    BLOCKS_STATE_READY,
};

typedef struct PokeathlonBlockSmashMon PokeathlonBlockSmashMon;       // the own team
typedef struct PokeathlonBlockSmashRivals PokeathlonBlockSmashRivals; // the other teams' Pokemon
typedef struct PokeathlonBlockSmashCpu PokeathlonBlockSmashCpu;       // the participants without a player (host only)

// What the active Pokemon brings to a hit (one word per team member in PokeathlonBlockSmashMon).
typedef struct PokeathlonBlockSmashMember {
    u32 power : 9;
    u32 maxStamina : 9;
    u32 stamina : 8;
    u32 kind : 6; // which of the three weak spot shapes it hits
} PokeathlonBlockSmashMember;

// One block of the stack (heap object).
typedef struct PokeathlonBlockSmashBlock {
    ManagedSprite *sprite;
    ManagedSprite *spot; // the weak spot marked on the block
    s16 health;          // 100 when dropped
    s16 spotX;           // offset of the weak spot from the block
    s16 spotY;
    u16 unkE;
    u32 spotSlot : 4; // one of the four weak spot positions
    u32 fallSpeed : 8;
    u32 state : 8; // 1 = falling, 2 = landed
    u32 index : 7; // place in the stack (0 = bottom)
    u32 active : 1;
    u32 unk10_28 : 4;
} PokeathlonBlockSmashBlock;

// A fragment thrown off a broken block.
typedef struct PokeathlonBlockSmashFragment {
    ManagedSprite *sprite;
    int active;
    fx32 velocityX;
    fx32 velocityY;
    s16 x;
    s16 y;
} PokeathlonBlockSmashFragment;

// The stack of blocks on the touch screen.
typedef struct PokeathlonBlockSmashBlocks {
    PokeathlonCourseData *course;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *hitSprite;
    ManagedSprite *missSprite;
    PokeathlonBlockSmashFragment fragments[16];
    PokeathlonBlockSmashBlock *blocks[10];
    u32 dropTimer : 16;
    u32 score : 16;                             // blocks broken in total (at most 200)
    u32 state : 4;                              // 1 = dropping a new stack, 2 = ready to be hit
    u32 count : 4;                              // blocks dropped / left on the stack
    u32 unk180_8 : 24;
} PokeathlonBlockSmashBlocks;

// A participant's row on the top screen: the active Pokemon's icon and the score bars.
typedef struct PokeathlonBlockSmashTeamRow {
    u8 member; // the Pokemon the icon shows
    ManagedSprite *icon;
    ManagedSprite *switching; // shown instead of the icon while the team switches Pokemon
    ManagedSprite *bars[20];  // one per 5 blocks
    ManagedSprite *stars[5];  // shown once all 20 bars are full
} PokeathlonBlockSmashTeamRow;

// The top screen's rows and what their Pokemon icons are loaded from.
typedef struct PokeathlonBlockSmashTeamIcons {
    NARC *narc; // the Pokemon icons
    PokeathlonBlockSmashTeamRow rows[4];
    u16 plttId;
} PokeathlonBlockSmashTeamIcons;

// The swipe that switches the active Pokemon.
typedef struct PokeathlonBlockSmashSwipe {
    u32 frames : 8;
    u32 startY : 8;
    u32 y : 8;
    u32 active : 1;
    u32 unused : 7;
} PokeathlonBlockSmashSwipe;

// The event's work (PokeathlonCourse_GetWork).
typedef struct PokeathlonBlockSmash {
    enum HeapID heapId;
    BgConfig *bgConfig;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    void *monPics; // the other teams' Pokemon
    PokeathlonCourseSpriteResources *iconResources;
    String *names[3]; // the own team's nicknames
    ManagedSprite *timerFrame;
    ManagedSprite *switchButton;
    Sprite *scoreDigits[3];
    Sprite *timerDigits[2];
    PokeathlonBlockSmashBlocks *blocks;
    PokeathlonBlockSmashRivals *rivals;
    PokeathlonBlockSmashCpu *cpu;
    PokeathlonBlockSmashMon *mon;
    PokeathlonBlockSmashTeamIcons icons;
    Window windows[4];
    MsgData *msgData;
    MessageFormat *msgFormat;
    int timer; // frames left
    PokeathlonBlockSmashSwipe swipe;
    SysTask *shakeTask;
    s8 shakeOffset;
    u8 shakeDecay;
    u8 shakeSide;
} PokeathlonBlockSmash;

// A participant's entry in the link data. Each participant sends its own as the first word of its block; the host
// collects them into the four words of its own block, which every participant receives.
typedef struct PokeathlonBlockSmashReport {
    u32 score : 8;
    u32 stamina : 8; // the active Pokemon's
    u32 gauge : 8;
    u32 member : 2; // the active Pokemon
    u32 gaugeFull : 1;
    u32 switchingFrame : 1; // set by the host from `switching`
    u32 switching : 1;
    u32 unk1D : 3;
} PokeathlonBlockSmashReport;

// The host's block of the link data as every participant receives it.
typedef struct PokeathlonBlockSmashFrame {
    PokeathlonBlockSmashReport reports[4];
    u32 unk10 : 16;
    u32 finished : 1;                      // the host's timer ran out
    u32 unk10_17 : 15;
} PokeathlonBlockSmashFrame;

// A participant's block of the link data (the part that is copied).
typedef struct PokeathlonBlockSmashLinkBlock {
    u32 words[9];
} PokeathlonBlockSmashLinkBlock;

BOOL PokeathlonBlockSmash_Intro(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonBlockSmash_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonBlockSmash_Finish(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_Init(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_Draw(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_Exit(PokeathlonCourseData *course);
u16 PokeathlonBlockSmashBlocks_Hit(PokeathlonBlockSmashBlocks *blocks, int member, s16 power, int hitSpot, int strongHit, BOOL *cleared);
PokeathlonBlockSmashBlocks *PokeathlonBlockSmashBlocks_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course);
void PokeathlonBlockSmashBlocks_Delete(PokeathlonBlockSmashBlocks *blocks);
void PokeathlonBlockSmashBlocks_Update(PokeathlonBlockSmashBlocks *blocks);
void PokeathlonBlockSmashBlocks_Pause(PokeathlonBlockSmashBlocks *blocks);
BOOL PokeathlonBlockSmashBlocks_IsSpotTouched(PokeathlonBlockSmashBlocks *blocks, int kind);
BOOL PokeathlonBlockSmashBlocks_IsTopBlockTouched(PokeathlonBlockSmashBlocks *blocks);
u32 PokeathlonBlockSmashBlocks_GetState(PokeathlonBlockSmashBlocks *blocks);
u16 PokeathlonBlockSmashBlocks_GetScore(PokeathlonBlockSmashBlocks *blocks);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_H
