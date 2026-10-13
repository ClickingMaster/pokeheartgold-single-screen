#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_STAGE_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_STAGE_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_lamp_jump.h"

// Lamp Jump (event 5), the second part: the trampoline the Pokemon bounce on (drawn in 3D as a band of
// changing colors), the lamps (29, up to 25 lit at a time, relit every 240 frames), the bouncing and the
// collisions of the three Pokemon, the score and timer displays and the participants without a player.

// One lamp. It is lit while `lit`; touching a lit lamp puts it out and scores.
typedef struct PokeathlonLampJumpLamp {
    BOOL lit;             // 0x00
    VecFx32 pos;          // 0x04
    void *sprite;         // 0x10
    void *pointsSprite;   // 0x14: the points of the hit
    void *hitSprite;      // 0x18
} PokeathlonLampJumpLamp; // size: 0x1C

typedef struct PokeathlonLampJumpLamps {
    PokeathlonLampJumpLamp lamps[29]; // 0x000
    int relightTimer;                 // 0x32C
} PokeathlonLampJumpLamps;            // size: 0x330

// The participants without a player: every 3 frames one of them may hit a lamp, more likely with more Jump
// and Skill.
typedef struct PokeathlonLampJumpCpuPlayers {
    BOOL active;                // 0x00
    u8 *dataCopy;               // 0x04: PokeathlonCourse_GetDataCopyArea
    u8 skill[4];                // 0x08: per participant, the sum of the Skill of its Pokemon
    u8 jump[4];                 // 0x0C: per participant, the sum of the Jump of its Pokemon
    u8 skillUpTo[4][3];         // 0x10: per participant, the Skill sums up to each Pokemon (picks the one that hits)
    u8 count;                   // 0x1C
    u8 current;                 // 0x1D
    u8 tick;                    // 0x1E
    u8 interval;                // 0x1F
    u8 first;                   // 0x20: first participant without a player
    u8 unk21;                   // 0x21: the same (4: none)
    u8 unk22[2];                // 0x22
} PokeathlonLampJumpCpuPlayers; // size: 0x24

void PokeathlonLampJump_DrawTrampoline(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_Draw3D(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_InitCamera(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_CalcTrampolineSurface(PokeathlonLampJumpTrampoline *trampoline);
void PokeathlonLampJump_CalcPartialSurface(PokeathlonLampJumpTrampoline *trampoline, int parts, s32 *heights);
void PokeathlonLampJump_CalcLine(PokeathlonLampJumpTrampoline *trampoline, int line, u8 monA, u8 monB);
u8 PokeathlonLampJump_ScoreLamp(PokeathlonLampJumpMon *mon);
void PokeathlonLampJump_UpdateJumps(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
void PokeathlonLampJump_UpdateJump(PokeathlonCourseData *course, PokeathlonLampJump *lampJump, u8 mon);
void PokeathlonLampJump_MoveSideways(PokeathlonLampJumpMon *mon, PokeathlonLampJumpJump *jump);
void PokeathlonLampJump_CalcDent(int column, int depth, s32 *dent);
void PokeathlonLampJump_SetRestingHeights(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_SetDent(PokeathlonLampJump *lampJump, int mon, u8 column, fx32 depth);
void PokeathlonLampJump_LoadSpriteResources(void *scene);
void PokeathlonLampJump_CreateSprites(PokeathlonLampJump *lampJump, void *scene);
void PokeathlonLampJump_PlaceSprites(PokeathlonLampJump *lampJump, u8 self);
void PokeathlonLampJump_UpdateScoreDisplay(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_PrintScore(PokeathlonLampJump *lampJump, int score);
void PokeathlonLampJump_UpdateTimerDisplay(PokeathlonLampJump *lampJump, int frames);
void PokeathlonLampJump_PrintPlayerNames(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
void PokeathlonLampJump_InitWindows(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_PrintTotal(PokeathlonLampJump *lampJump, int score);
void PokeathlonLampJump_SlowDownSideways(PokeathlonLampJumpMon *mon);
void PokeathlonLampJump_SetMonParams(PokeathlonCourseData *course, PokeathlonLampJumpMonParams *params, u8 member, PokeathlonLampJumpMon *mon);
BOOL PokeathlonLampJump_CollideMons(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
PokeathlonLampJumpLamps *PokeathlonLampJump_NewLamps(enum HeapID heapId);
void PokeathlonLampJump_FreeLamps(PokeathlonLampJumpLamps *lamps);
void PokeathlonLampJump_CreateLampSprites(PokeathlonLampJumpLamps *lamps, void *scene, int priority);
void PokeathlonLampJump_PlaceLamps(PokeathlonLampJumpLamps *lamps);
void PokeathlonLampJump_UpdateLamps(PokeathlonLampJumpLamps *lamps);
void PokeathlonLampJump_PutOutLamp(PokeathlonLampJumpLamps *lamps, u8 index);
VecFx32 *PokeathlonLampJump_GetLampPos(PokeathlonLampJumpLamps *lamps, u8 index);
BOOL PokeathlonLampJump_IsLampLit(PokeathlonLampJumpLamps *lamps, u8 index);
void PokeathlonLampJump_ShowLampPoints(PokeathlonLampJumpLamps *lamps, u8 index, u8 points);
void PokeathlonLampJump_LightLamps(PokeathlonLampJumpLamps *lamps);
PokeathlonLampJumpCpuPlayers *PokeathlonLampJump_NewCpuPlayers(enum HeapID heapId, u8 count, int unused, u8 *dataCopy, PokeathlonCourseData *course);
void PokeathlonLampJump_FreeCpuPlayers(PokeathlonLampJumpCpuPlayers *cpu);
int PokeathlonLampJump_UpdateCpuPlayers(PokeathlonCourseData *course, PokeathlonLampJumpCpuPlayers *cpu);
void PokeathlonLampJump_UpdateCpuPlayer(PokeathlonCourseData *course, PokeathlonLampJumpCpuPlayers *cpu, u8 participant);
void PokeathlonResultsAnnouncement_InitWindow(Window *window, BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonResultsAnnouncement_PrintTitle(Window *window, enum HeapID heapId);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_STAGE_H
