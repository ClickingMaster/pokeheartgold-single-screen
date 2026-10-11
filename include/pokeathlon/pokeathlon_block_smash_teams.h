#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_TEAMS_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_TEAMS_H

#include "pokeathlon/pokeathlon_block_smash.h"

// Block Smash: the own team's Pokemon (the active one hits, the other two wait at the side), the
// other teams' rows on the touch screen and the CPU participants.
//
// Every hit costs the active Pokemon stamina; at 0 it is exhausted until it recovered 8. Hitting the
// weak spot fills the gauge by 30; a full gauge (100) gives every hit +150 power for 90 frames.
// Swiping up switches to the next Pokemon (the gauge drops by 10).

enum PokeathlonBlockSmashMonMode {
    BLOCK_SMASH_MON_READY,
    BLOCK_SMASH_MON_HIT,       // the hit animation (10 frames)
    BLOCK_SMASH_MON_EXHAUSTED, // until the stamina is back at 8
    BLOCK_SMASH_MON_SWITCHING,
};

// The own team (0x48).
struct PokeathlonBlockSmashMon {
    enum HeapID heapId;           // 0x00
    SpriteSystem *spriteSystem;   // 0x04
    SpriteManager *spriteManager; // 0x08
    PokeathlonCourseData *course; // 0x0C
    // 0: the active Pokemon, 1: its hit effect, 2: the mark above it, 3: the switch smoke, 4: the ball,
    // 5, 6: the waiting Pokemon, 7, 8: their marks
    ManagedSprite *sprites[9];             // 0x10
    PokeathlonBlockSmashMember members[3]; // 0x34
    u32 member : 2;                        // 0x40: the active Pokemon
    u32 mode : 8;
    u32 frames : 11; // counts up every frame (stamina recovery)
    u32 gaugeTimer : 11;
    u32 gauge : 8; // 0x44: 0-100
    u32 gaugeFull : 1;
    u32 tired : 1; // the active Pokemon's stamina is low
    u32 switchState : 6;
    u32 modeFrames : 8;
    u32 switchFrames : 8;
};

// One of the other teams' rows (the touch screen's upper part).
typedef struct PokeathlonBlockSmashRival {
    PokeathlonCourseData *course;
    void *monPics[3];          // 0x04
    u32 maxStamina[3];         // 0x10
    ManagedSprite *sprites[6]; // 0x1C: 0: score drop, 1: falling block, 2: stamina mark, 3: gauge effect, 4, 5: switch effects
    SysTask *switchTask;       // 0x34
    u32 newMember : 2;         // 0x38
    u32 switchState : 4;
    u32 switchFrames : 8;
    u32 switching : 1;
    u32 score : 8; // last shown
    u32 member : 2;
    u32 participant : 2;
    u32 column : 2;
    u32 unk38_29 : 3;
} PokeathlonBlockSmashRival; // size: 0x3C

struct PokeathlonBlockSmashRivals {
    enum HeapID heapId;                // 0x00
    SpriteSystem *spriteSystem;        // 0x04
    SpriteManager *spriteManager;      // 0x08
    PokeathlonCourseData *course;      // 0x0C
    void *monPics;                     // 0x10
    PokeathlonBlockSmashRival rows[3]; // 0x14
}; // size: 0xC8

enum PokeathlonBlockSmashCpuMode {
    BLOCK_SMASH_CPU_READY,
    BLOCK_SMASH_CPU_SWITCHING, // 26 frames
    BLOCK_SMASH_CPU_EXHAUSTED, // until the stamina is back at 8
    BLOCK_SMASH_CPU_NEW_STACK, // 42 frames after every 10th block
};

// A participant without a player (the host plays them; same rules as the own team).
typedef struct PokeathlonBlockSmashCpuPlayer {
    PokeathlonCourseData *course;          // 0x00
    PokeathlonBlockSmashMember members[3]; // 0x04
    int blockHealth;                       // 0x10
    u32 score : 8;                         // 0x14
    u32 modeFrames : 8;
    u32 gauge : 8;
    u32 mode : 6;
    u32 member : 2;
    u32 frames : 11; // 0x18
    u32 gaugeTimer : 11;
    u32 wait : 8; // frames until the next decision
    u32 participant : 2;
    u32 sinceHit : 8; // 0x1C
    u32 recentlyHit : 1;
    u32 gaugeFull : 1;
    u32 unk1C_10 : 22;
    u32 unk20;
} PokeathlonBlockSmashCpuPlayer; // size: 0x24

struct PokeathlonBlockSmashCpu {
    enum HeapID heapId;                       // 0x00
    PokeathlonCourseData *course;             // 0x04
    PokeathlonBlockSmashCpuPlayer players[3]; // 0x08
    u32 count;                                // 0x74
};

// pokeathlon_block_smash_teams.c
PokeathlonBlockSmashMon *PokeathlonBlockSmashMon_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course);
void PokeathlonBlockSmashMon_Delete(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_Update(PokeathlonBlockSmashMon *mon, BOOL stackDropping);
void PokeathlonBlockSmashMon_OnHit(PokeathlonBlockSmashMon *mon, BOOL hitSpot, u8 broken);
void PokeathlonBlockSmashMon_StartSwitch(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_SetAnimate(PokeathlonBlockSmashMon *mon, BOOL animate);
u32 PokeathlonBlockSmashMon_GetMode(PokeathlonBlockSmashMon *mon);
BOOL PokeathlonBlockSmashMon_CanSwitch(PokeathlonBlockSmashMon *mon);
PokeathlonBlockSmashMember *PokeathlonBlockSmashMon_GetActiveMember(PokeathlonBlockSmashMon *mon);
u32 PokeathlonBlockSmashMon_IsGaugeFull(PokeathlonBlockSmashMon *mon);
u32 PokeathlonBlockSmashMon_IsTired(PokeathlonBlockSmashMon *mon);
u16 PokeathlonBlockSmashMon_GetGauge(PokeathlonBlockSmashMon *mon);
u8 PokeathlonBlockSmashMon_GetActiveMemberIndex(PokeathlonBlockSmashMon *mon);
u32 PokeathlonBlockSmashMon_CanHit(PokeathlonBlockSmashMon *mon);
PokeathlonBlockSmashRivals *PokeathlonBlockSmashRivals_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course, void *monPics);
void PokeathlonBlockSmashRivals_Delete(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Update(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Pause(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashMon_UpdateReady(PokeathlonBlockSmashMon *mon, BOOL stackDropping);
void PokeathlonBlockSmashMon_UpdateHit(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_UpdateExhausted(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_UpdateSwitch(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_UpdateTired(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashRival_Init(PokeathlonBlockSmashRival *row, SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *monPics, u8 participant, PokeathlonCourseData *course, u32 *maxStamina);
void PokeathlonBlockSmashRival_ShowStaminaMark(PokeathlonBlockSmashRival *row, int anim);
void PokeathlonBlockSmashRival_UpdateStatus(PokeathlonBlockSmashRival *row, PokeathlonBlockSmashReport *report);
void PokeathlonBlockSmashRival_UpdateScore(PokeathlonBlockSmashRival *row, PokeathlonBlockSmashReport *report);
void PokeathlonBlockSmashRival_SwitchTask(SysTask *task, void *data);
void PokeathlonBlockSmashRival_StartSwitch(PokeathlonBlockSmashRival *row);
void PokeathlonBlockSmashRival_Update(PokeathlonBlockSmashRival *row);
void PokeathlonBlockSmashMon_CreateSprites(ManagedSprite **sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager);
void PokeathlonBlockSmashMon_SetMonSprite(PokeathlonBlockSmashMon *mon, int sprite, u8 participant, u8 member, BOOL a4);
void PokeathlonBlockSmashMon_SetMode(PokeathlonBlockSmashMon *mon, int mode);
void PokeathlonBlockSmashMon_UpdateGauge(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_AddGauge(PokeathlonBlockSmashMon *mon, BOOL hitSpot, u8 broken);
BOOL PokeathlonBlockSmash_SetTiredAnimSpeed(ManagedSprite *sprite, fx32 stamina, fx32 maxStamina);
void PokeathlonBlockSmashMon_StartFullGauge(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_EndFullGauge(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashRivals_InitMonPics(void *monPics, u32 self);
void PokeathlonBlockSmash_ShowGaugeEffect(ManagedSprite *sprite, u16 gauge, BOOL playSE);
void PokeathlonBlockSmashMon_SetWaitingPosition(PokeathlonBlockSmashMon *mon, int sprite, s16 x, s16 y);
PokeathlonBlockSmashCpu *PokeathlonBlockSmashCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonBlockSmashCpu_Delete(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_Update(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_WriteReports(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpuPlayer_Init(PokeathlonBlockSmashCpuPlayer *player, u8 participant, PokeathlonCourseData *course);
u32 PokeathlonBlockSmashCpuPlayer_Update(PokeathlonBlockSmashCpuPlayer *player);
u32 PokeathlonBlockSmashCpuPlayer_UpdateReady(PokeathlonBlockSmashCpuPlayer *player);
u32 PokeathlonBlockSmashCpuPlayer_UpdateSwitch(PokeathlonBlockSmashCpuPlayer *player);
u32 PokeathlonBlockSmashCpuPlayer_UpdateNewStack(PokeathlonBlockSmashCpuPlayer *player);
u32 PokeathlonBlockSmashCpuPlayer_UpdateExhausted(PokeathlonBlockSmashCpuPlayer *player);
BOOL PokeathlonBlockSmashCpuPlayer_DecideHitAgain(PokeathlonBlockSmashCpuPlayer *player);
u32 PokeathlonBlockSmashCpuPlayer_DecideSpotHit(PokeathlonBlockSmashCpuPlayer *player);
BOOL PokeathlonBlockSmashCpuPlayer_DecideSwitch(PokeathlonBlockSmashCpuPlayer *player);
void PokeathlonBlockSmashCpuPlayer_SetMode(PokeathlonBlockSmashCpuPlayer *player, int mode);
void PokeathlonBlockSmashCpuPlayer_Hit(PokeathlonBlockSmashCpuPlayer *player, int hitSpot);
void PokeathlonBlockSmashCpuPlayer_WriteReport(PokeathlonBlockSmashCpuPlayer *player, u32 result, PokeathlonCourseData *course);
void PokeathlonBlockSmashCpuPlayer_AddGauge(PokeathlonBlockSmashCpuPlayer *player, int hitSpot, u32 broken);
void PokeathlonBlockSmashCpuPlayer_UpdateGauge(PokeathlonBlockSmashCpuPlayer *player);
void PokeathlonBlockSmashCpuPlayer_EndFullGauge(PokeathlonBlockSmashCpuPlayer *player);
ManagedSprite *PokeathlonBlockSmash_NewMainSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);
ManagedSprite *PokeathlonBlockSmash_NewSubSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);
void PokeathlonBlockSmash_InitMembers(PokeathlonBlockSmashMember *members, u32 participant, PokeathlonCourseData *course);
void PokeathlonBlockSmash_RecoverStamina(PokeathlonBlockSmashMember *members, u32 frames, int active);
s32 PokeathlonBlockSmash_TakeStamina(s32 stamina, u32 frames);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_TEAMS_H
