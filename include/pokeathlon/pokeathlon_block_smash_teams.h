#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_TEAMS_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_TEAMS_H

#include "pokeathlon/pokeathlon_block_smash.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

// Block Smash: the own team's Pokemon, the other teams' rows on the touch screen and the CPU participants. Every
// hit costs stamina; hitting the weak spot fills the gauge, and a full gauge gives every hit +150 power for 90 frames.

enum PokeathlonBlockSmashMonMode {
    BLOCK_SMASH_MON_READY,
    BLOCK_SMASH_MON_HIT,       // the hit animation (10 frames)
    BLOCK_SMASH_MON_EXHAUSTED, // until the stamina is back at 8
    BLOCK_SMASH_MON_SWITCHING,
};

// The own team.
struct PokeathlonBlockSmashMon {
    enum HeapID heapId;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PokeathlonCourseData *course;
    // 0: the active Pokemon, 1: its hit effect, 2: the mark above it, 3: the switch smoke, 4: the ball,
    // 5, 6: the waiting Pokemon, 7, 8: their marks
    ManagedSprite *sprites[9];
    PokeathlonBlockSmashMember members[3];
    u32 member : 2; // the active Pokemon
    u32 mode : 8;
    u32 frames : 11; // counts up every frame (stamina recovery)
    u32 gaugeTimer : 11;
    u32 gauge : 8; // 0-100
    u32 gaugeFull : 1;
    u32 tired : 1; // the active Pokemon's stamina is low
    u32 switchState : 6;
    u32 modeFrames : 8;
    u32 switchFrames : 8;
};

// One of the other teams' rows (the touch screen's upper part).
typedef struct PokeathlonBlockSmashRival {
    PokeathlonCourseData *course;
    void *monPics[3];
    u32 maxStamina[3];
    ManagedSprite *sprites[6]; // 0: score drop, 1: falling block, 2: stamina mark, 3: gauge effect, 4, 5: switch effects
    SysTask *switchTask;
    u32 newMember : 2;
    u32 switchState : 4;
    u32 switchFrames : 8;
    u32 switching : 1;
    u32 score : 8; // last shown
    u32 member : 2;
    u32 participant : 2;
    u32 column : 2;
    u32 unk38_29 : 3;
} PokeathlonBlockSmashRival;

struct PokeathlonBlockSmashRivals {
    enum HeapID heapId;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PokeathlonCourseData *course;
    PokeathlonCourseMonObjects *monPics;
    PokeathlonBlockSmashRival rows[3];
};

enum PokeathlonBlockSmashCpuMode {
    BLOCK_SMASH_CPU_READY,
    BLOCK_SMASH_CPU_SWITCHING, // 26 frames
    BLOCK_SMASH_CPU_EXHAUSTED, // until the stamina is back at 8
    BLOCK_SMASH_CPU_NEW_STACK, // 42 frames after every 10th block
};

// A participant without a player (the host plays them; same rules as the own team).
typedef struct PokeathlonBlockSmashCpuPlayer {
    PokeathlonCourseData *course;
    PokeathlonBlockSmashMember members[3];
    int blockHealth;
    u32 score : 8;
    u32 modeFrames : 8;
    u32 gauge : 8;
    u32 mode : 6;
    u32 member : 2;
    u32 frames : 11;
    u32 gaugeTimer : 11;
    u32 wait : 8; // frames until the next decision
    u32 participant : 2;
    u32 sinceHit : 8;
    u32 recentlyHit : 1;
    u32 gaugeFull : 1;
    u32 unk1C_10 : 22;
    u32 unk20;
} PokeathlonBlockSmashCpuPlayer;

struct PokeathlonBlockSmashCpu {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    PokeathlonBlockSmashCpuPlayer players[3];
    u32 count;
};

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
PokeathlonBlockSmashRivals *PokeathlonBlockSmashRivals_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course, PokeathlonCourseMonObjects *monPics);
void PokeathlonBlockSmashRivals_Delete(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Update(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Pause(PokeathlonBlockSmashRivals *rivals);
PokeathlonBlockSmashCpu *PokeathlonBlockSmashCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonBlockSmashCpu_Delete(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_Update(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_WriteReports(PokeathlonBlockSmashCpu *cpu);
ManagedSprite *PokeathlonBlockSmash_NewMainSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);
ManagedSprite *PokeathlonBlockSmash_NewSubSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_BLOCK_SMASH_TEAMS_H
