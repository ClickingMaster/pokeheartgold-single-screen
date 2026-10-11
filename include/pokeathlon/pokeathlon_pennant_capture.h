#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "geometry_math.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"

// Pennant Capture (Pokeathlon event 1): the four participants' Pokemon run around a walled field
// with obstacle blocks, pick up the pennants that appear at fixed spots (at most 9 carried) and
// score by carrying them into the goal area at the bottom of the field. Colliding with another
// Pokemon that runs in the same direction knocks it down and takes up to two of its pennants.
// Field coordinates are whole units (512 x 512); the touch screen shows a 256 x 192 window
// centred on the own Pokemon. The participants without a player are steered by a simple CPU.

// The movement parameters of one of the three Pokemon (one per team member).
typedef struct PokeathlonPennantCaptureMoveParams {
    f32 unk00;                        // 0x00
    f32 unk04;                        // 0x04
    f32 knockbackScale;               // 0x08: share of the own speed passed on to a Pokemon it knocks down
    s32 knockdownTime;                // 0x0C: base of the frames a knocked-down Pokemon stays down
    u8 unk10;                         // 0x10
    u8 unk11[3];                      // 0x11
} PokeathlonPennantCaptureMoveParams; // size: 0x14

// One participant's team on the field (the Pokemon currently running is `member`).
typedef struct PokeathlonPennantCapturePlayer {
    void *monObjects[3];                              // 0x00: one course object per team member
    PokeathlonPennantCaptureMoveParams moveParams[3]; // 0x0C
    void *carrySprite;                                // 0x48: shown above the Pokemon while active
    void *pointSprites[2];                            // 0x4C: "+points" effects, used alternately
    void *pennantSprite;                              // 0x54: pennant count change effect
    VecFx32 position;                                 // 0x58: field position (fx32)
    VecFx32 velocity;                                 // 0x64
    s16 drawOffsetY;                                  // 0x70
    u8 flashTimer;                                    // 0x72
    u8 shownPennants;                                 // 0x73
    s32 wallSide;                                     // 0x74: side of the obstacle hit last (0 = none)
    u16 nextPointSprite;                              // 0x78
    u8 unk7A[2];                                      // 0x7A
    VecFx32 cpuFrom;                                  // 0x7C: CPU: where the current path starts
    VecFx32 cpuTarget;                                // 0x88: CPU: where it leads
    u8 contacts[4];                                   // 0x94: touching the other participants' Pokemon
    u32 participant;                                  // 0x98
    u8 unk9C;                                         // 0x9C: set once a deposit finished
    u8 unk9D;                                         // 0x9D
    u8 swapState;                                     // 0x9E: team member swap shown on the top screen
    u8 swapStep;                                      // 0x9F
    u8 swapTimer;                                     // 0xA0
    u8 unkA1;                                         // 0xA1
    u8 knockdownTimer;                                // 0xA2
    u8 active;                                        // 0xA3
    u8 freezeTimer;                                   // 0xA4: frames without movement after a collision
    u8 cpuHasTarget;                                  // 0xA5
    u8 knockedDown;                                   // 0xA6
    u8 pointsToAdd;                                   // 0xA7: deposited pennants still to be counted
    u8 depositDelay;                                  // 0xA8
    u8 depositSteps;                                  // 0xA9
    u8 pennants;                                      // 0xAA: carried pennants (at most 9)
    u8 depositing;                                    // 0xAB
    u16 score;                                        // 0xAC
    u16 shownScore;                                   // 0xAE: score as last received from the host
    u8 direction;                                     // 0xB0: 1-4, see PokeathlonPennantCapture_GetDirection
    u8 member;                                        // 0xB1: team member on the field
    u8 unkB2;                                         // 0xB2
    u8 touchingPennant[5];                            // 0xB3
} PokeathlonPennantCapturePlayer;                     // size: 0xB8

// An obstacle block (32 x 32 units).
typedef struct PokeathlonPennantCaptureObstacle {
    void *object; // 0x00: NULL ends the list
    void *sprite; // 0x04
    u16 x;        // 0x08
    u16 y;        // 0x0A
} PokeathlonPennantCaptureObstacle;

// One of the ten two-sprite field objects; the CPU players head for them.
typedef struct PokeathlonPennantCaptureFieldItem {
    void *object;       // 0x00: NULL ends the list
    void *sprite;       // 0x04
    void *shadowSprite; // 0x08
    u16 x;              // 0x0C
    u16 y;              // 0x0E
} PokeathlonPennantCaptureFieldItem;

typedef struct PokeathlonPennantCapturePennant {
    u32 active;       // 0x00
    void *sprite;     // 0x04
    void *poleSprite; // 0x08
    u32 spot;         // 0x0C: index into PokeathlonPennantCapturePennants.spots
    u16 x;            // 0x10
    u16 y;            // 0x12
} PokeathlonPennantCapturePennant;

typedef struct PokeathlonPennantCapturePoint {
    u16 x;
    u16 y;
} PokeathlonPennantCapturePoint;

// The five pennants on the field and the 20 spots they appear at, in turn.
typedef struct PokeathlonPennantCapturePennants {
    PokeathlonPennantCapturePennant pennants[5]; // 0x00
    PokeathlonPennantCapturePoint spots[20];     // 0x64
    u16 nextSpot;                                // 0xB4
    u16 count;                                   // 0xB6
} PokeathlonPennantCapturePennants;

// The event's work (PokeathlonCourse_GetHeapAllocPtr4).
typedef struct PokeathlonPennantCapture {
    BgConfig *bgConfig;                                  // 0x000
    Window scoreWindow;                                  // 0x004
    enum HeapID heapId;                                  // 0x014
    u32 unk018;                                          // 0x018
    NNSG2dCharacterData *bgChars;                        // 0x01C: the animated BG0 tiles (6 frames)
    u32 unk020;                                          // 0x020
    PokeathlonPennantCapturePlayer players[4];           // 0x024
    u8 unk304[0x48];                                     // 0x304
    void *pickupEffects[4];                              // 0x34C: SEQ_SE_GS_THLON06, used in turn
    void *dropEffects[4];                                // 0x35C: SEQ_SE_GS_SAA, used in turn
    struct PokeathlonPennantCaptureCpu *cpu;             // 0x36C
    struct PokeathlonPennantCaptureDisplay *display;     // 0x370
    u8 unk374[8];                                        // 0x374
    PokeathlonPennantCaptureObstacle obstacles[20];      // 0x37C
    PokeathlonPennantCaptureFieldItem fieldItems[10];    // 0x46C
    u8 unk50C[4];                                        // 0x50C
    u8 unk510;                                           // 0x510: stronger friction while set
    u8 bgAnimFrame;                                      // 0x511
    u8 bgAnimTimer;                                      // 0x512
    u8 shownPennantSpots[5];                             // 0x513
    u8 nextPickupEffect;                                 // 0x518
    u8 hitReported;                                      // 0x519
    u8 nextDropEffect;                                   // 0x51A
    u8 unk51B;                                           // 0x51B
    u16 hitY;                                            // 0x51C
    u16 hitX;                                            // 0x51E
    u8 unk520[4];                                        // 0x520
    u32 pickupEffectX[4];                                // 0x524
    u32 pickupEffectY[4];                                // 0x534
    u32 dropEffectX[4];                                  // 0x544
    u32 dropEffectY[4];                                  // 0x554
    PokeathlonPennantCapturePennants pennants;           // 0x564
    PokeathlonPennantCapturePoint obstaclePositions[20]; // 0x61C
    u8 unk66C[0x50];                                     // 0x66C
    u8 pennantsFull[4];                                  // 0x6BC: touched a pennant while carrying 9
} PokeathlonPennantCapture;

// The frame state the host sends to all participants (link data block at +0xF0, 0x28 bytes).
// words[0-3]: per participant x (bits 0-8), y (9-17), score (18-27), pennants (28-31).
// words[4]: drop effect x, y, bit 18 new drop effect, bits 19-22 hidden participants,
//           bits 23-30 swap state per participant (2 bits each).
// words[5]: pickup effect x, y, bit 18 new pickup effect, bits 19-22 "pennants full" per participant.
// spots[0-4]: bits 0-4 spot of pennant i; for i < 4 also member (bits 5-6), size - 1 (7-8),
//             active (9) and bonus points (11-15) of participant i.
typedef struct PokeathlonPennantCaptureFrame {
    u32 words[6];
    u16 spots[5];
} PokeathlonPennantCaptureFrame;

// Link data as seen by the frame update: own block, the other participants' blocks, the frame.
typedef struct PokeathlonPennantCaptureLinkData {
    u8 own[0x50];
    u8 others[4][0x28];
    u8 frame[0x28];
} PokeathlonPennantCaptureLinkData;

// A participant's team member on the top screen.
typedef struct PokeathlonPennantCaptureDisplayMon {
    void *icon;      // 0x00
    void *carryMark; // 0x04
    void *frame;     // 0x08
    u8 active;       // 0x0C
    u8 unk0D[0xF];   // 0x0D
} PokeathlonPennantCaptureDisplayMon;

// One of the three team members' battle pictures (two are kept in VRAM, slid in and out in turn).
typedef struct PokeathlonPennantCaptureMonPic {
    void *character;
    void *palette;
} PokeathlonPennantCaptureMonPic;

typedef struct PokeathlonPennantCaptureCounter {
    u16 x;
    u16 y;
} PokeathlonPennantCaptureCounter;

// The top screen (score, the participants' Pokemon, the timer).
typedef struct PokeathlonPennantCaptureDisplay {
    enum HeapID heapId;            // 0x000
    PokeathlonCourseData *course;  // 0x004
    BgConfig *bgConfig;            // 0x008
    Window window;                 // 0x00C
    MsgData *msgData;              // 0x01C
    MessageFormat *msgFormat;      // 0x020
    void *scoreScreenFile;         // 0x024
    NNSG2dScreenData *scoreScreen; // 0x028
    u8 unk02C[0xC];                // 0x02C

    union {
        PokeathlonPennantCaptureDisplayMon mons[4]; // stride 0x1C; the last entry's tail is iconFiles[0-2]

        struct {
            u8 filler[0x64];
            void *iconFiles[12]; // 0x09C
        } raw;
    } u; // 0x038

    NNSG2dCharacterData *iconChars[12];          // 0x0CC
    u8 iconPalettes[12];                         // 0x0FC
    PokeathlonPennantCaptureMonPic monPics[3];   // 0x108
    u32 picCharOffsets[2];                       // 0x120
    u32 picPlttOffsets[2];                       // 0x128
    void *picSprites[2];                         // 0x130
    void *goalSprite;                            // 0x138
    void *statusSprites[4][4];                   // 0x13C
    void *pointSprites[4][2];                    // 0x17C
    int picState;                                // 0x19C
    u16 picSlot;                                 // 0x1A0
    u16 nextPointSprite;                         // 0x1A2
    u8 shownMember[4];                           // 0x1A4
    String *names[3];                            // 0x1A8
    Sprite *timerDigits[2];                      // 0x1B4
    PokeathlonPennantCaptureCounter counters[3]; // 0x1BC
} PokeathlonPennantCaptureDisplay;               // size: 0x1C8

// The per-level move parameter tables (tenths), indexed by the Pokemon's performance levels.
typedef struct PokeathlonPennantCaptureMoveTables {
    int knockbackScale[5];
    int unk00[5];
    int unk04[5];
    int knockdownTime[5];
    int unk10[5];
} PokeathlonPennantCaptureMoveTables;

// An obstacle block grown by a radius: its corners, then its four edges moved outward.
typedef struct PokeathlonPennantCaptureBounds {
    Point2D corners[4];
    Point2D edges[4][2];
} PokeathlonPennantCaptureBounds;

// A participant's Pokemon as listed for the top screen.
typedef struct PokeathlonPennantCaptureIconMon {
    u16 species;
    u16 form;
} PokeathlonPennantCaptureIconMon;

// A team member as passed to PokeathlonPennantCaptureDisplay_LoadMonPics.
typedef struct PokeathlonPennantCapturePicMon {
    u16 species;
    u16 form;
    u16 unused4;
    u8 shiny;
    u8 gender;
    u32 unused8;
    u32 personality;
} PokeathlonPennantCapturePicMon;

// What the CPU sees of a participant: the Pokemon and its position on the touch screen of the host.
typedef struct PokeathlonPennantCaptureCpuPlayer {
    PokeathlonPennantCapturePlayer *player;
    u16 x;
    u16 y;
} PokeathlonPennantCaptureCpuPlayer;

typedef struct PokeathlonPennantCaptureCpuObject {
    int active;
    int x;
    int y;
} PokeathlonPennantCaptureCpuObject;

// One of the 6 x 8 cells (32 x 32) around the CPU's Pokemon, rated each time it picks a new target.
typedef struct PokeathlonPennantCaptureCpuCell {
    int x;       // 0x00: cell centre
    int y;       // 0x04
    int targetX; // 0x08: a random point of the cell
    int targetY; // 0x0C
    s16 score;   // 0x10
    s16 index;   // 0x12
} PokeathlonPennantCaptureCpuCell;

// The CPU of the participants without a player (one decision per CPU every `interval` frames).
typedef struct PokeathlonPennantCaptureCpu {
    PokeathlonPennantCaptureCpuPlayer players[4];     // 0x000
    PokeathlonPennantCaptureCpuObject obstacles[20];  // 0x020
    PokeathlonPennantCaptureCpuObject fieldItems[10]; // 0x110
    PokeathlonPennantCapturePennant *pennants;        // 0x188
    u32 unk18C;                                       // 0x18C
    PokeathlonPennantCaptureCpuCell cells[48];        // 0x190
    u32 thinking;                                     // 0x550
    u8 cpuCount;                                      // 0x554
    u8 cursor;                                        // 0x555
    u8 timer;                                         // 0x556
    u8 interval;                                      // 0x557
    u8 firstCpu;                                      // 0x558
    u8 humanCount;                                    // 0x559
} PokeathlonPennantCaptureCpu;                        // size: 0x55C

// Resource ids of sprite sets (ov96_0221CBCC, defined with the override object between the display parts).
typedef struct PokeathlonPennantCaptureSpriteIds {
    u32 ids[2];
} PokeathlonPennantCaptureSpriteIds;

// pokeathlon_pennant_capture_field.c
void PokeathlonPennantCapture_UpdatePlayers(PokeathlonCourseData *course);
void PokeathlonPennantCapture_TickFreezeTimer(PokeathlonCourseData *course, PokeathlonPennantCapture *work, PokeathlonPennantCapturePlayer *player);
void PokeathlonPennantCapture_ApplyFrame(PokeathlonCourseData *course, int viewer, PokeathlonPennantCapturePlayer *players, PokeathlonPennantCaptureLinkData *link);
void PokeathlonPennantCapture_UpdateObstacleSprites(PokeathlonPennantCapture *work, int x, int y);
void PokeathlonPennantCapture_UpdateFieldItemSprites(PokeathlonPennantCapture *work, int x, int y);
void PokeathlonPennantCapture_UpdatePennantSprites(PokeathlonPennantCapture *work, PokeathlonPennantCaptureFrame *frame, int x, int y);
void PokeathlonPennantCapture_RunMonExit(PokeathlonPennantCaptureDisplay *display, PokeathlonPennantCapturePlayer *player, int participant, int member, u8 *block, int waitOther, int isSelf);
void PokeathlonPennantCapture_RunMonEnter(PokeathlonPennantCapturePlayer *player, int participant, int member, u8 *block, int report);
void PokeathlonPennantCapture_UpdateMonSwap(PokeathlonPennantCaptureDisplay *display, PokeathlonPennantCapturePlayer *player, PokeathlonPennantCaptureFrame *frame, u8 participant, u8 member, u8 *block, int isHost, int isSelf, int remote);
void PokeathlonPennantCapture_SetMoveParams(PokeathlonCourseData *course, const PokeathlonPennantCaptureMoveTables *tables, int participant, u8 member, PokeathlonPennantCapturePlayer *player);
void PokeathlonPennantCapture_GetObstaclesNear(PokeathlonPennantCapture *work, fx32 fieldX, fx32 fieldY, PokeathlonPennantCapturePoint **out, u8 *count);
void PokeathlonPennantCapture_CalcObstacleBounds(int radius, const PokeathlonPennantCapturePoint *center, PokeathlonPennantCaptureBounds *bounds);
int PokeathlonPennantCapture_CollideWithObstacle(VecFx32 *from, VecFx32 *to, int radius, PokeathlonPennantCaptureBounds *bounds, VecFx32 *out);
int PokeathlonPennantCapture_CollideWithObstacles(PokeathlonPennantCapture *work, int excluded, int radius, VecFx32 *position, VecFx32 *end, VecFx32 *out);
fx32 PokeathlonPennantCapture_CalcCircleEntry(VecFx32 *a, VecFx32 *b, VecFx32 *point, fx32 radius);
float ov96_022073F0(PokeathlonPennantCapturePlayer *player, int member);
void PokeathlonPennantCapture_AddPoint2D(const Point2D *a, const Point2D *b, Point2D *out);
int PokeathlonPennantCapture_CollidePlayers(PokeathlonCourseData *course, PokeathlonPennantCapturePlayer *players, VecFx32 *out);
void PokeathlonPennantCapture_UpdateFieldDrawPriorities(PokeathlonPennantCapture *work);
void PokeathlonPennantCapture_SetPennantDrawPriority(PokeathlonPennantCapturePennant *pennant, int y);
void PokeathlonPennantCapture_SetDropEffectDrawPriority(PokeathlonPennantCapture *work);
void PokeathlonPennantCapture_SetPlayerDrawPriority(PokeathlonPennantCapturePlayer *player, int y);
void PokeathlonPennantCapture_InitScoreWindow(PokeathlonPennantCapture *work);
void PokeathlonPennantCapture_PrintScore(PokeathlonPennantCapture *work, u16 score);
void PokeathlonPennantCapture_ResetPennantSpots(PokeathlonPennantCapture *work);
void PokeathlonPennantCapture_AdvancePennantSpot(PokeathlonPennantCapturePennants *pennants);
void PokeathlonPennantCapture_PlacePennant(PokeathlonPennantCapturePennants *pennants);
void PokeathlonPennantCapture_RefillPennants(PokeathlonPennantCapture *work);
BOOL PokeathlonPennantCapture_PickUpPennants(PokeathlonPennantCapturePennants *pennants, PokeathlonPennantCapturePlayer *player, u8 *full);
BOOL PokeathlonPennantCapture_CirclesOverlap(const VecFx32 *a, const VecFx32 *b, fx32 radiusA, fx32 radiusB);
int PokeathlonPennantCapture_GetDirection(const VecFx32 *vector);
BOOL PokeathlonPennantCapture_TryKnockDown(PokeathlonCourseData *course, PokeathlonPennantCapturePlayer *a, PokeathlonPennantCapturePlayer *b);
BOOL PokeathlonPennantCapture_CanDeposit(PokeathlonPennantCapturePlayer *player);
u8 ov96_02207BD4(u8 pennants);
int PokeathlonPennantCapture_GetDrawPriority(int y);
void PokeathlonPennantCapture_AnimateBg(PokeathlonPennantCapture *work);
void PokeathlonPennantCapture_PlaySEIfOnScreen(int x, int y, int seq, BOOL self);
void PokeathlonPennantCapture_UpdateOwnScore(PokeathlonCourseData *course);

// pokeathlon_pennant_capture_display.c
PokeathlonPennantCaptureDisplay *PokeathlonPennantCaptureDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonPennantCaptureDisplay_Delete(PokeathlonPennantCaptureDisplay *display);
void PokeathlonPennantCaptureDisplay_LoadBgGfx(PokeathlonPennantCaptureDisplay *display);
void PokeathlonPennantCaptureDisplay_LoadSpriteResources(PokeathlonPennantCaptureDisplay *display, void *pool);
void PokeathlonPennantCaptureDisplay_CreateSprites(PokeathlonPennantCaptureDisplay *display, void *pool, void *resources);
void PokeathlonPennantCaptureDisplay_LoadIcons(PokeathlonPennantCaptureDisplay *display, const PokeathlonPennantCaptureIconMon *mons);
void PokeathlonPennantCaptureDisplay_LoadMonPics(PokeathlonPennantCaptureDisplay *display, const PokeathlonPennantCapturePicMon *mons);
void PokeathlonPennantCaptureDisplay_LoadMonPicToVram(PokeathlonPennantCaptureDisplay *display, u32 slot, u32 member);
void PokeathlonPennantCaptureDisplay_SetIcon(PokeathlonPennantCaptureDisplay *display, int participant, int member);
void PokeathlonPennantCaptureDisplay_SetScore(PokeathlonPennantCaptureDisplay *display, int score);
BOOL PokeathlonPennantCaptureDisplay_SlideMonPic(PokeathlonPennantCaptureDisplay *display, int member);
BOOL PokeathlonPennantCaptureDisplay_StepSwapWait(PokeathlonPennantCaptureDisplay *display, u32 participant);
void PokeathlonPennantCaptureDisplay_UpdateMon(PokeathlonPennantCaptureDisplay *display, u8 participant, u16 member, BOOL hidden, int active);
void PokeathlonPennantCaptureDisplay_SetTimer(PokeathlonPennantCaptureDisplay *display, int frames);
void PokeathlonPennantCaptureDisplay_BringToFront(PokeathlonPennantCaptureDisplay *display, int chosen);
void PokeathlonPennantCaptureDisplay_SetPennants(PokeathlonPennantCaptureDisplay *display, int participant, u32 pennants);
void PokeathlonPennantCaptureDisplay_ShowPoint(PokeathlonPennantCaptureDisplay *display, int participant);
void PokeathlonPennantCaptureDisplay_PrintName(PokeathlonPennantCaptureDisplay *display, u32 member);

// pokeathlon_pennant_capture_cpu.c
void PokeathlonPennantCaptureDisplay_GetPicVramOffsets(void *pool, PokeathlonPennantCaptureDisplay *display);
void PokeathlonPennantCaptureDisplay_InitWindow(PokeathlonPennantCaptureDisplay *display);
void PokeathlonPennantCaptureDisplay_ShowStatus(void **sprites, int show);
PokeathlonPennantCaptureCpu *PokeathlonPennantCaptureCpu_New(enum HeapID heapId, int cpuCount);
void PokeathlonPennantCaptureCpu_SetPlayer(PokeathlonPennantCaptureCpu *cpu, int participant, PokeathlonPennantCapturePlayer *player);
void PokeathlonPennantCaptureCpu_SetPennants(PokeathlonPennantCaptureCpu *cpu, PokeathlonPennantCapturePennant *pennants);
void PokeathlonPennantCaptureCpu_SetObstacles(PokeathlonPennantCaptureCpu *cpu, const u8 *spots);
void PokeathlonPennantCaptureCpu_Delete(PokeathlonPennantCaptureCpu *cpu);
int PokeathlonPennantCaptureCpu_Update(PokeathlonPennantCaptureCpu *cpu);
void PokeathlonPennantCaptureCpu_ChooseTarget(PokeathlonPennantCaptureCpu *cpu, u8 who);
PokeathlonPennantCaptureCpuCell *PokeathlonPennantCaptureCpu_PickBestCell(PokeathlonPennantCaptureCpu *cpu);
u8 PokeathlonPennantCaptureCpu_SortCells(PokeathlonPennantCaptureCpuCell **cells, PokeathlonPennantCaptureCpuCell **out);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_PENNANT_CAPTURE_H
