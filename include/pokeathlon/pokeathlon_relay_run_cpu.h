#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_CPU_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_CPU_H

#include "global.h"

#include "pokeathlon/pokeathlon_relay_run.h"

// The CPU of the Relay Run teams without a player. Every `interval` frames it looks at each CPU
// team in turn: it rates the 4 x 4 cells (40 x 40 pixels) ahead of the team's runner (other runners
// and obstacles in a lane count against it, the far cells count for it) and either flicks the
// runner towards a random point of one of the best cells or, when nothing is good enough, hands
// over to the next member of the team.

#define RELAY_RUN_CPU_CELLS     16
#define RELAY_RUN_CPU_CELL_SIZE 40
#define RELAY_RUN_CPU_LOOKAHEAD (4 * RELAY_RUN_CPU_CELL_SIZE)
#define RELAY_RUN_CPU_NONE      RELAY_RUN_TEAMS // PokeathlonRelayRunCpu_Update: no team was looked at

// What the CPU sees of a team: the team and the collision centre of its runner (whole pixels).
typedef struct PokeathlonRelayRunCpuTeam {
    PokeathlonRelayRunTeam *team; // 0x00
    u32 unk04;                    // 0x04
    int x;                        // 0x08
    int y;                        // 0x0C
} PokeathlonRelayRunCpuTeam;

// An obstacle of the track layout, as a square of `size` pixels.
typedef struct PokeathlonRelayRunCpuObstacle {
    BOOL active; // 0x00
    int x;       // 0x04: left
    int y;       // 0x08: top
    int kind;    // 0x0C: RELAY_RUN_OBSTACLE_*
    int size;    // 0x10
} PokeathlonRelayRunCpuObstacle;

// One of the cells ahead of the runner, rated each time the CPU looks at a team.
typedef struct PokeathlonRelayRunCpuCell {
    int x;       // 0x00: cell centre
    int y;       // 0x04
    int targetX; // 0x08: a random point of the cell
    int targetY; // 0x0C
    s16 score;   // 0x10
    s16 index;   // 0x12
} PokeathlonRelayRunCpuCell;

// A node of the list PokeathlonRelayRunCpu_SortCells sorts with (node 0 is the head).
typedef struct PokeathlonRelayRunCpuCellNode {
    struct PokeathlonRelayRunCpuCellNode *prev;
    struct PokeathlonRelayRunCpuCellNode *next;
    PokeathlonRelayRunCpuCell *cell;
} PokeathlonRelayRunCpuCellNode;

typedef struct PokeathlonRelayRunCpu {
    PokeathlonRelayRunCpuTeam teams[RELAY_RUN_TEAMS];             // 0x000
    PokeathlonRelayRunCpuObstacle obstacles[RELAY_RUN_OBSTACLES]; // 0x040
    u8 lanes[RELAY_RUN_TEAMS];                                    // 0x298: lane (0-3) of each team's runner
    PokeathlonRelayRunCpuCell cells[RELAY_RUN_CPU_CELLS];         // 0x29C
    u32 thinking;                                                 // 0x3DC: looking at the CPU teams one per frame
    u8 cpuCount;                                                  // 0x3E0
    u8 cursor;                                                    // 0x3E1: next CPU team to look at
    u8 timer;                                                     // 0x3E2
    u8 interval;                                                  // 0x3E3
    u8 firstCpu;                                                  // 0x3E4: the CPU teams come after the players
    u8 humanCount;                                                // 0x3E5
} PokeathlonRelayRunCpu;                                          // size: 0x3E8

void PokeathlonRelayRunHud_GetPicVramOffsets(void *pool, PokeathlonRelayRunHud *hud);
void PokeathlonRelayRunHud_InitWindows(PokeathlonRelayRunHud *hud);
PokeathlonRelayRunCpu *PokeathlonRelayRunCpu_New(enum HeapID heapId, int cpuCount, int mode);
void PokeathlonRelayRunCpu_SetTeam(PokeathlonRelayRunCpu *cpu, u8 index, PokeathlonRelayRunTeam *team);
void PokeathlonRelayRunCpu_SetObstacles(PokeathlonRelayRunCpu *cpu, s32 *layout);
void PokeathlonRelayRunCpu_Delete(PokeathlonRelayRunCpu *cpu);
u8 PokeathlonRelayRunCpu_Update(PokeathlonCourseData *course, PokeathlonRelayRunCpu *cpu, s32 timer);
void PokeathlonRelayRunCpu_Think(PokeathlonCourseData *course, PokeathlonRelayRunCpu *cpu, u8 index, s32 timer);
void PokeathlonRelayRunCpu_WrapX(s32 *x, s32 delta);
PokeathlonRelayRunCpuCell *PokeathlonRelayRunCpu_PickBestCell(PokeathlonRelayRunCpu *cpu);
u8 PokeathlonRelayRunCpu_SortCells(PokeathlonRelayRunCpuCell **cells, PokeathlonRelayRunCpuCell **out);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_CPU_H
