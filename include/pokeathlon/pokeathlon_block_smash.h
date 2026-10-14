#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Block Smash (Pokeathlon event 3): every participant breaks a stack of ten blocks dropped onto the
// touch screen by tapping it; each hit takes the active Pokemon's power off the top block's health
// (100), more when the weak spot marked on the block is tapped. A broken stack is rebuilt. The team's
// three Pokemon are switched by swiping the active one up; the top screen shows the four
// participants' scores. The event runs for 900 frames (30 seconds).

typedef struct PokeathlonBlockSmashMon PokeathlonBlockSmashMon;       // the own team (0x48)
typedef struct PokeathlonBlockSmashRivals PokeathlonBlockSmashRivals; // the other teams' Pokemon (0xC8)
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
    ManagedSprite *sprite; // 0x00
    ManagedSprite *spot;   // 0x04: the weak spot marked on the block
    s16 health;            // 0x08: 100 when dropped
    s16 spotX;             // 0x0A: offset of the weak spot from the block
    s16 spotY;             // 0x0C
    u16 unkE;
    u32 spotSlot : 4; // 0x10: one of the four weak spot positions
    u32 fallSpeed : 8;
    u32 state : 8; // 1 = falling, 2 = landed
    u32 index : 7; // place in the stack (0 = bottom)
    u32 active : 1;
    u32 unk10_28 : 4;
} PokeathlonBlockSmashBlock; // size: 0x14

// A fragment thrown off a broken block.
typedef struct PokeathlonBlockSmashFragment {
    ManagedSprite *sprite;      // 0x00
    int active;                 // 0x04
    fx32 velocityX;             // 0x08
    fx32 velocityY;             // 0x0C
    s16 x;                      // 0x10
    s16 y;                      // 0x12
} PokeathlonBlockSmashFragment; // size: 0x14

// The stack of blocks on the touch screen.
typedef struct PokeathlonBlockSmashBlocks {
    PokeathlonCourseData *course;               // 0x000
    SpriteSystem *spriteSystem;                 // 0x004
    SpriteManager *spriteManager;               // 0x008
    ManagedSprite *hitSprite;                   // 0x00C
    ManagedSprite *missSprite;                  // 0x010
    PokeathlonBlockSmashFragment fragments[16]; // 0x014
    PokeathlonBlockSmashBlock *blocks[10];      // 0x154
    u32 dropTimer : 16;                         // 0x17C
    u32 score : 16;                             // blocks broken in total (at most 200)
    u32 state : 4;                              // 0x180: 1 = dropping a new stack, 2 = ready to be hit
    u32 count : 4;                              // blocks dropped / left on the stack
    u32 unk180_8 : 24;
} PokeathlonBlockSmashBlocks; // size: 0x184

// A participant's row on the top screen: the active Pokemon's icon and the score bars.
typedef struct PokeathlonBlockSmashTeamRow {
    u8 member;                 // 0x00: the Pokemon the icon shows
    ManagedSprite *icon;       // 0x04
    ManagedSprite *switching;  // 0x08: shown instead of the icon while the team switches Pokemon
    ManagedSprite *bars[20];   // 0x0C: one per 5 blocks
    ManagedSprite *stars[5];   // 0x5C: shown once all 20 bars are full
} PokeathlonBlockSmashTeamRow; // size: 0x70

// The top screen's rows and what their Pokemon icons are loaded from.
typedef struct PokeathlonBlockSmashTeamIcons {
    NARC *narc;                          // 0x000: the Pokemon icons
    PokeathlonBlockSmashTeamRow rows[4]; // 0x004
    u16 plttId;                          // 0x1C4
} PokeathlonBlockSmashTeamIcons;

// The swipe that switches the active Pokemon.
typedef struct PokeathlonBlockSmashSwipe {
    u32 frames : 8;
    u32 startY : 8;
    u32 y : 8;
    u32 active : 1;
    u32 unused : 7;
} PokeathlonBlockSmashSwipe;

// The event's work (PokeathlonCourse_GetHeapAllocPtr4).
typedef struct PokeathlonBlockSmash {
    enum HeapID heapId;                  // 0x000
    BgConfig *bgConfig;                  // 0x004
    SpriteSystem *spriteSystem;          // 0x008
    SpriteManager *spriteManager;        // 0x00C
    void *monPics;                       // 0x010: the other teams' Pokemon
    void *iconResources;                 // 0x014
    String *names[3];                    // 0x018: the own team's nicknames
    ManagedSprite *timerFrame;           // 0x024
    ManagedSprite *switchButton;         // 0x028
    Sprite *scoreDigits[3];              // 0x02C
    Sprite *timerDigits[2];              // 0x038
    PokeathlonBlockSmashBlocks *blocks;  // 0x040
    PokeathlonBlockSmashRivals *rivals;  // 0x044
    PokeathlonBlockSmashCpu *cpu;        // 0x048
    PokeathlonBlockSmashMon *mon;        // 0x04C
    PokeathlonBlockSmashTeamIcons icons; // 0x050
    Window windows[4];                   // 0x218
    MsgData *msgData;                    // 0x258
    MessageFormat *msgFormat;            // 0x25C
    int timer;                           // 0x260: frames left
    PokeathlonBlockSmashSwipe swipe;     // 0x264
    SysTask *shakeTask;                  // 0x268
    s8 shakeOffset;                      // 0x26C
    u8 shakeDecay;                       // 0x26D
    u8 shakeSide;                        // 0x26E
} PokeathlonBlockSmash;                  // size: 0x270

// A participant's entry in the link data. Each participant sends its own as the first word of its
// 0x28 block (PokeathlonCourse_GetDataCopyArea); the host collects them into the four words of its
// block at 0x28, which every participant receives at 0xF0.
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

// The host's block of the link data as every participant receives it (0xF0).
typedef struct PokeathlonBlockSmashFrame {
    PokeathlonBlockSmashReport reports[4]; // 0x00
    u32 unk10 : 16;                        // 0x10
    u32 finished : 1;                      // the host's timer ran out
    u32 unk10_17 : 15;
} PokeathlonBlockSmashFrame;

// A participant's 0x28 block of the link data (the part that is copied).
typedef struct PokeathlonBlockSmashLinkBlock {
    u32 words[9];
} PokeathlonBlockSmashLinkBlock;

// pokeathlon_block_smash.c
BOOL PokeathlonBlockSmash_Intro(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonBlockSmash_UpdateGame(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonBlockSmash_Finish(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_Init(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_Draw(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonBlockSmash_Exit(PokeathlonCourseData *course);
void PokeathlonBlockSmash_SetVramBanks(void);
void PokeathlonBlockSmash_InitSpriteSystem(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_FreeSpriteSystem(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_CreateHudSprites(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_DeleteHudSprites(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_LoadSpriteResources(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_LoadMainAndSubSpriteRes(SpriteSystem *spriteSystem, SpriteManager *spriteManager);
void PokeathlonBlockSmash_UpdateScoreDigits(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_StopHudAnims(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_InitBgs(BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonBlockSmash_FreeBgsAndText(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_LoadBgGraphics(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_InitText(PokeathlonCourseData *course);
void PokeathlonBlockSmash_UpdateTimer(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_PrintInitialText(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_PrintMessage(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId);
void PokeathlonBlockSmash_CreateTeamRows(PokeathlonCourseData *course);
void PokeathlonBlockSmash_DeleteTeamRows(PokeathlonBlockSmash *work);
void PokeathlonBlockSmash_UpdateTeamRows(PokeathlonCourseData *course);
void PokeathlonBlockSmash_SetTeamRowIcon(PokeathlonCourseData *course, u32 participant, u32 member);
void PokeathlonBlockSmash_HandleSwipe(PokeathlonBlockSmash *work, BOOL anywhere);
void PokeathlonBlockSmash_WriteReport(PokeathlonCourseData *course);
void PokeathlonBlockSmash_UpdateLinkData(PokeathlonCourseData *course);
void PokeathlonBlockSmash_PrintScore(PokeathlonCourseData *course, u32 score);
void PokeathlonBlockSmash_ShakeTask(SysTask *task, void *data);
void PokeathlonBlockSmash_StartShake(PokeathlonBlockSmash *work, int offset, int decay);
PokeathlonBlockSmashBlocks *PokeathlonBlockSmashBlocks_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course);
void PokeathlonBlockSmashBlocks_Delete(PokeathlonBlockSmashBlocks *blocks);
void PokeathlonBlockSmashBlocks_Update(PokeathlonBlockSmashBlocks *blocks);
void PokeathlonBlockSmashBlocks_Pause(PokeathlonBlockSmashBlocks *blocks);
BOOL PokeathlonBlockSmashBlocks_IsSpotTouched(PokeathlonBlockSmashBlocks *blocks, int kind);
BOOL PokeathlonBlockSmashBlocks_IsTopBlockTouched(PokeathlonBlockSmashBlocks *blocks);
u16 PokeathlonBlockSmashBlocks_Hit(PokeathlonBlockSmashBlocks *blocks, int member, s16 power, int hitSpot, int strongHit, BOOL *cleared);
u32 PokeathlonBlockSmashBlocks_GetState(PokeathlonBlockSmashBlocks *blocks);
u16 PokeathlonBlockSmashBlocks_GetScore(PokeathlonBlockSmashBlocks *blocks);
void PokeathlonBlockSmashBlocks_UpdateDrop(PokeathlonBlockSmashBlocks *blocks);
void PokeathlonBlockSmashBlocks_CheckEmpty(PokeathlonBlockSmashBlocks *blocks);
PokeathlonBlockSmashBlock *PokeathlonBlockSmashBlock_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, u32 index);
void PokeathlonBlockSmashBlock_Drop(PokeathlonBlockSmashBlock *block);
void PokeathlonBlockSmashBlock_Delete(PokeathlonBlockSmashBlock *block);
void PokeathlonBlockSmashBlock_Update(PokeathlonBlockSmashBlock *block);
void PokeathlonBlockSmashBlock_MoveSpot(PokeathlonBlockSmashBlock *block);
void PokeathlonBlockSmashBlock_UpdateSpot(PokeathlonBlockSmashBlock *block);
BOOL PokeathlonBlockSmashBlock_IsSpotTouched(PokeathlonBlockSmashBlock *block, int kind);
void PokeathlonBlockSmashBlock_GetPosition(PokeathlonBlockSmashBlock *block, s16 *x, s16 *y);
u32 PokeathlonBlockSmashBlock_GetState(PokeathlonBlockSmashBlock *block);
void PokeathlonBlockSmashFragments_Init(PokeathlonBlockSmashFragment *fragments, SpriteSystem *spriteSystem, SpriteManager *spriteManager);
void PokeathlonBlockSmashFragments_Delete(PokeathlonBlockSmashFragment *fragments);
void PokeathlonBlockSmashFragments_Spawn(PokeathlonBlockSmashFragment *fragments, u8 count, s16 x, s16 y);
BOOL PokeathlonBlockSmashFragment_Update(PokeathlonBlockSmashFragment *fragment);
void PokeathlonBlockSmashFragments_Update(PokeathlonBlockSmashFragment *fragments);
void PokeathlonBlockSmashFragments_Pause(PokeathlonBlockSmashFragment *fragments);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_H
