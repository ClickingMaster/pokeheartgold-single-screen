#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_internal.h"

#include "bg_window.h"
#include "sprite.h"

// Relay Run (Pokeathlon event 6): each team runs laps around an oval track for 2700 frames (45 seconds). The
// player flicks the running Pokemon with the stylus to speed it up and swaps to the next member of the team
// before it runs out of energy. Obstacles on the track bounce the runners back or (holes) make them fall.
// The host steps every team and sends the result to everyone in a 0x28 packet; every player draws from it.

#define RELAY_RUN_TEAMS        4
#define RELAY_RUN_MEMBERS      3
#define RELAY_RUN_OBSTACLES    30
#define RELAY_RUN_DURATION     2700 // frames
#define RELAY_RUN_TRACK_LENGTH 1024 // pixels per lap
#define RELAY_RUN_MAX_LAPS     60
#define RELAY_RUN_RUNNER_X     80 // screen x of the player's own runner
#define RELAY_RUN_TIRED_ENERGY 40

// PokeathlonRelayRunMember.condition
#define RELAY_RUN_FINE      0
#define RELAY_RUN_TIRED     1 // energy at RELAY_RUN_TIRED_ENERGY or below
#define RELAY_RUN_EXHAUSTED 2 // out of energy: can't be flicked until it recovers

// PokeathlonRelayRunObstacle.kind (0: none)
#define RELAY_RUN_OBSTACLE_HOLE 3

// Collision circle of a Pokemon by size (NARC a/1/7/0 member 12).
typedef struct PokeathlonRelayRunCollision {
    s32 offsetY;
    s32 radius;
} PokeathlonRelayRunCollision;

// One member of a team (0x1C).
typedef struct PokeathlonRelayRunMember {
    f32 acceleration; // 0x00: scales the speed a flick gives
    f32 weight;       // 0x04: scales it as well
    u8 unk08[4];      // 0x08
    f32 energy;       // 0x0C: goes down while it runs, up while it rests
    f32 maxEnergy;    // 0x10
    u8 topSpeed;      // 0x14: speed it is slowed to after bumping into something (pixels per frame)
    u8 unk15;         // 0x15
    u8 speedLimit;    // 0x16: the speed grades are measured against it
    u8 recoveryTime;  // 0x17: frames an exhausted member needs to recover
    u8 condition;     // 0x18: RELAY_RUN_FINE / _TIRED / _EXHAUSTED
    u8 unk19[3];      // 0x19
} PokeathlonRelayRunMember;

// A team (0xD4): the three Pokemon of one participant, one running at a time.
typedef struct PokeathlonRelayRunTeam {
    void *monObjects[RELAY_RUN_MEMBERS];                 // 0x00: PokeathlonCourseMonObject
    fx32 distance[RELAY_RUN_MEMBERS];                    // 0x0C: distance each member ran
    PokeathlonRelayRunMember members[RELAY_RUN_MEMBERS]; // 0x18
    void *effectSprite;                                  // 0x6C: bump / crash marks
    void *holeSprite;                                    // 0x70: the hole it fell into
    void *fallSprite;                                    // 0x74: the falling Pokemon
    void *speedSprite;                                   // 0x78: speed lines
    VecFx32 position;                                    // 0x7C: x along the lap (wraps at RELAY_RUN_TRACK_LENGTH), y on the track
    u8 animState;                                        // 0x88
    u8 unk89[2];                                         // 0x89
    u8 current;                                          // 0x8B: member running
    VecFx32 velocity;                                    // 0x8C
    u8 unk98[4];                                         // 0x98
    u8 laps;                                             // 0x9C
    u8 swapping;                                         // 0x9D: handing over to the next member
    u8 swapPhase;                                        // 0x9E: 1 swap, 2 wait for the display, 0 done
    u8 unk9F;                                            // 0x9F
    u8 fallPhase;                                        // 0xA0
    u8 fallTimer;                                        // 0xA1
    u8 speedGrade;                                       // 0xA2: 0 standing, 1 slow, 2 fast, 3 at its limit
    u8 tired;                                            // 0xA3: the running member ran out of energy; it is capped
    u8 recoveryTimer;                                    // 0xA4
    u8 bumped;                                           // 0xA5: hit an obstacle or the wall this frame
    u8 lastEffect;                                       // 0xA6: drawing: the condition shown last frame
    u8 lastSpeedGrade;                                   // 0xA7: drawing: the speed grade shown last frame
    u8 bestLaps;                                         // 0xA8
    u8 stunTimer;                                        // 0xA9: can't be moved or flicked while it runs
    u8 flicked;                                          // 0xAA: the stylus was lifted after a short drag
    u8 unkAB;                                            // 0xAB
    int heldFrames;                                      // 0xAC: frames the stylus has been held
    VecFx32 dragStart;                                   // 0xB0
    VecFx32 dragEnd;                                     // 0xBC
    BOOL hidden;                                         // 0xC8: fell into a hole
    int wallState;                                       // 0xCC: which wall / obstacle side it is touching
    u8 owner;                                            // 0xD0: participant
    u8 behindStart;                                      // 0xD1: ran backwards over the start line with no laps
    u8 speedSoundGrade;                                  // 0xD2: drawing: speed grade the dash sound was played for
    u8 speedSoundPlayed;                                 // 0xD3
} PokeathlonRelayRunTeam;                                // size: 0xD4

// What the host keeps of one participant's stylus (0xC).
typedef struct PokeathlonRelayRunPress {
    BOOL pressed; // 0x0: on the frame the stylus touches the screen
    BOOL held;    // 0x4
    u32 unk8;     // 0x8
} PokeathlonRelayRunPress;

// An obstacle on the track (0xC). The host loads the obstacle layout (NARC a/2/2/7, one of ten).
typedef struct PokeathlonRelayRunObstacle {
    BOOL active;  // 0x0
    void *sprite; // 0x4: PokeathlonCourseSpritePoolSprite
    u16 x;        // 0x8: along the lap
    u8 y;         // 0xA
    u8 kind;      // 0xB
} PokeathlonRelayRunObstacle;

// The stylus of a player (0x24).
typedef struct PokeathlonRelayRunTouch {
    BOOL held;       // 0x00
    u8 x;            // 0x04
    u8 y;            // 0x05
    u8 unk06[2];     // 0x06
    u8 displayPhase; // 0x08: the swap / fall animation reached this phase on the player's screen
    u8 unk09[0x1B];  // 0x09
} PokeathlonRelayRunTouch;

// A block of the link data holding a stylus (0x28).
typedef struct PokeathlonRelayRunTouchBlock {
    PokeathlonRelayRunTouch touch; // 0x00
    u8 unk24[4];                   // 0x24
} PokeathlonRelayRunTouchBlock;

// Everything a player draws, sent by the host every frame (0x28).
typedef struct PokeathlonRelayRunPacket {
    u16 x[RELAY_RUN_TEAMS];        // 0x00
    u8 y[RELAY_RUN_TEAMS];         // 0x08
    u8 runner[RELAY_RUN_TEAMS];    // 0x0C: current member | speed grade << 2
    u8 laps[RELAY_RUN_TEAMS];      // 0x10: laps | swapping << 7
    s32 flags;                     // 0x14: condition of each member (2 bits, team * 3 + member), finished << 24,
                                   //       bump << 25, behindStart of each team << 26
    u16 timer;                     // 0x18
    u16 bumpX;                     // 0x1A
    u8 bumpY;                      // 0x1C
    u8 swapPhases;                 // 0x1D: 2 bits per team
    u8 swapSigns[RELAY_RUN_TEAMS]; // 0x1E: "change" sign shown at the top of each player's screen
    u8 unk22[6];                   // 0x22
} PokeathlonRelayRunPacket;

// The link data of the event (PokeathlonCourse_GetDataCopyArea).
typedef struct PokeathlonRelayRunLink {
    PokeathlonRelayRunTouchBlock touch;                    // 0x000: this player's stylus
    PokeathlonRelayRunPacket packetToSend;                 // 0x028: host
    PokeathlonRelayRunTouchBlock touches[RELAY_RUN_TEAMS]; // 0x050: everyone's stylus, collected by the host
    PokeathlonRelayRunPacket packet;                       // 0x0F0: received from the host
} PokeathlonRelayRunLink;

typedef struct PokeathlonRelayRun {
    BgConfig *bgConfig;                                        // 0x000
    Window window;                                             // 0x004
    enum HeapID heapId;                                        // 0x014
    void *spritePool;                                          // 0x018: PokeathlonCourseSpritePool
    void *signSprite;                                          // 0x01C
    Sprite *lapDigits[2];                                      // 0x020
    void *finishSprite;                                        // 0x028
    u8 unk02C[4];                                              // 0x02C
    PokeathlonRelayRunTeam teams[RELAY_RUN_TEAMS];             // 0x030
    PokeathlonRelayRunPress presses[RELAY_RUN_TEAMS];          // 0x380
    BOOL dragging[RELAY_RUN_TEAMS];                            // 0x3B0
    u8 finished;                                               // 0x3C0
    u8 unk3C1;                                                 // 0x3C1
    u8 heldTooLong;                                            // 0x3C2: someone held the stylus down for 255 frames: more friction
    u8 nextBumpMark;                                           // 0x3C3
    void *monResources;                                        // 0x3C4: PokeathlonCourseSpriteResources
    void *monObjects;                                          // 0x3C8: PokeathlonCourseMonObjects
    void *bumpMarks[4];                                        // 0x3CC
    void *cpu;                                                 // 0x3DC: the teams without a player
    void *hud;                                                 // 0x3E0: the touch screen
    s32 layoutToSend[RELAY_RUN_OBSTACLES];                     // 0x3E4: host: kind | y << 8 | x << 16
    s32 layout[RELAY_RUN_OBSTACLES];                           // 0x45C: received
    PokeathlonRelayRunObstacle obstacles[RELAY_RUN_OBSTACLES]; // 0x4D4
    u8 bumped;                                                 // 0x63C: two runners bumped into each other this frame
    u8 bumpY;                                                  // 0x63D
    u16 bumpX;                                                 // 0x63E
    u16 timer;                                                 // 0x640: frames left
    u16 signSoundPlayed;                                       // 0x642
    u8 swapSigns[RELAY_RUN_TEAMS];                             // 0x644: when to change members (set by the CPU helper)
} PokeathlonRelayRun;                                          // size: 0x648

BOOL PokeathlonRelayRun_Init(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_Draw(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_Exit(PokeathlonCourseData *course);
void PokeathlonRelayRun_SetGfxBanks(void);
void PokeathlonRelayRun_InitBgs(BgConfig *bgConfig);
void PokeathlonRelayRun_LoadBgGraphics(PokeathlonRelayRun *relayRun);
void PokeathlonRelayRun_LoadSpriteGraphics(void *spritePool);
void PokeathlonRelayRun_CreateSprites(PokeathlonRelayRun *relayRun, void *spritePool);
BOOL PokeathlonRelayRun_Start(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRelayRun_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRelayRun_SaveScores(PokeathlonCourseData *course, u8 *state);
void PokeathlonRelayRun_UpdateHost(PokeathlonCourseData *course);
void PokeathlonRelayRun_PackPacket(PokeathlonRelayRun *relayRun, PokeathlonRelayRunPacket *packet);
BOOL PokeathlonRelayRun_Update(PokeathlonCourseData *course);
void PokeathlonRelayRun_DrawPacket(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_IsTouchingRunner(PokeathlonRelayRun *relayRun, int teamId, int x, int y);
BOOL PokeathlonRelayRun_IsTouchingSwapButton(PokeathlonRelayRun *relayRun, u32 x, u32 y);
void PokeathlonRelayRun_MoveTeams(PokeathlonCourseData *course);
void PokeathlonRelayRunTeam_UpdateStun(PokeathlonCourseData *course, PokeathlonRelayRun *relayRun, PokeathlonRelayRunTeam *team);

// Defined further on in the event (still under their address names).
void ov96_021FEAEC(PokeathlonRelayRun *relayRun, int x);
void ov96_021FEFE8(PokeathlonCourseData *course, void *setup, u8 team, u8 member, PokeathlonRelayRunTeam *teamState);
int ov96_021FF5A8(PokeathlonRelayRun *relayRun, int wallState, int radius, VecFx32 *from, VecFx32 *to, VecFx32 *out);
f32 ov96_021FF6DC(PokeathlonRelayRunTeam *team, int member);
void ov96_021FF72C(const VecFx32 *a, const VecFx32 *b, VecFx32 *out);
BOOL ov96_021FF764(PokeathlonCourseData *course, PokeathlonRelayRunTeam *teams, VecFx32 *where);
void ov96_021FFB44(PokeathlonRelayRunObstacle *obstacle);
void ov96_021FFB7C(PokeathlonRelayRunTeam *team, u8 y, int isOwn);
void ov96_021FFBD8(BgConfig *bgConfig, int x);
void ov96_021FFC34(Sprite **lapDigits, u8 laps);
void ov96_021FFD4C(PokeathlonRelayRun *relayRun);
void ov96_021FFE5C(PokeathlonRelayRun *relayRun, u8 team, int touchingSwapButton);
void ov96_021FFEE8(PokeathlonCourseData *course);
void *ov96_021FFF3C(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void ov96_021FFFE8(void *hud);
void ov96_02200068(void *hud);
void ov96_022000E4(void *hud, void *spritePool);
void ov96_02200180(void *hud, void *spritePool, void *monResources);
void ov96_022002F8(void *hud, PokeathlonCoursePoint *mons);
void ov96_022003E8(void *hud, PokeathlonCourseMonInfo *mons);
void ov96_02200454(void *hud, int a, int b);
void ov96_0220050C(void *hud, u8 team, u16 x);
void ov96_022005B4(void *hud, u8 laps, int x);
void ov96_02200A64(void *hud, int timer);
void ov96_02200B04(void *hud, int team);
void *ov96_02200E3C(enum HeapID heapId, int count, int mode);
void ov96_02200E78(void *cpu, u8 team, PokeathlonRelayRunTeam *teamState);
void ov96_02200E80(void *cpu, s32 *layout);
void ov96_02200EEC(void *cpu);
void ov96_02200EF4(PokeathlonCourseData *course, void *cpu, u16 timer);
void ov96_021FE550(PokeathlonCourseData *course, int team, PokeathlonRelayRunTeam *teams, PokeathlonRelayRunLink *link);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H
