#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_INTERNAL_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_INTERNAL_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

#include "apricorn_internal.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// Shared by the Pokeathlon course translation units of OVY_96 (course flow, link commands, Pokemon sprites,
// scores, sprite resources).

// ---------------------------------------------------------------------------------------------------------
// Course participants

// One Pokemon of a participant's team (0x28).
typedef struct PokeathlonCourseMon {
    u16 species;      // 0x00
    u16 form;         // 0x02
    u32 personality;  // 0x04
    u8 stats[5];      // 0x08: current Pokeathlon performance (speed, power, skill, stamina, jump)
    u8 filler_0D[3];  // 0x0D
    u8 isShiny;       // 0x10
    u8 gender;        // 0x11
    u16 nickname[11]; // 0x12
} PokeathlonCourseMon;

// A course participant (0x7C). PokeathlonCourseData stores four of them as the opaque PokeathlonParticipantData.
typedef struct PokeathlonCourseParticipant {
    u32 npcTeam;                 // 0x00: NARC a/2/5/8 entry + 1 of an NPC team, 0 for a player
    PokeathlonCourseMon mons[3]; // 0x04
} PokeathlonCourseParticipant;

// What the Pokemon sprite loader needs to know about one Pokemon (0x10).
typedef struct PokeathlonCourseMonInfo {
    u16 species;     // 0x00
    u16 form;        // 0x02
    u16 isLarge;     // 0x04: overworld sprite uses the 64x64 frames (follower parameter byte 1)
    u8 isShiny;      // 0x06
    u8 gender;       // 0x07
    int participant; // 0x08
    u32 personality; // 0x0C
} PokeathlonCourseMonInfo;

// ---------------------------------------------------------------------------------------------------------
// Score list (PokeathlonCourseData.graphicsSystem): the per-event result rows and the course point totals

#define POKEATHLON_COURSE_SCORE_ENTRY_MAX  12
#define POKEATHLON_COURSE_EVENT_POINTS_MAX 10

typedef struct PokeathlonCourseScore {
    s32 participant; // 0x00
    u8 unk04[5];     // 0x04
    u8 rank;         // 0x09
    u16 points;      // 0x0A
    s32 score;       // 0x0C
    String *text;    // 0x10
    String *name;    // 0x14
} PokeathlonCourseScore;

typedef struct PokeathlonCourseScoreList {
    s32 count;                                                         // 0x000
    u32 unk04;                                                         // 0x004
    s32 next;                                                          // 0x008
    u32 sortOrder;                                                     // 0x00C
    BOOL totalsAdded;                                                  // 0x010
    u32 totals[4];                                                     // 0x014
    PokeathlonCourseScore entries[POKEATHLON_COURSE_SCORE_ENTRY_MAX];  // 0x024
    PokeathlonCourseScore *ranking[POKEATHLON_COURSE_SCORE_ENTRY_MAX]; // 0x144
    u32 eventPoints[POKEATHLON_COURSE_EVENT_POINTS_MAX][4];            // 0x174
} PokeathlonCourseScoreList;

// Doubly linked node of the insertion sort in PokeathlonCourseScoreList_Rank.
typedef struct PokeathlonCourseScoreNode {
    struct PokeathlonCourseScoreNode *prev;
    struct PokeathlonCourseScoreNode *next;
    PokeathlonCourseScore *entry;
} PokeathlonCourseScoreNode;

// ---------------------------------------------------------------------------------------------------------
// Pokemon sprites built from the overworld models (NARC data/mmodel/mmodel)

// One loaded Pokemon sprite (0x2C).
typedef struct PokeathlonCourseMonGfx {
    SpriteResource *charRes;      // 0x00
    SpriteResource *plttRes;      // 0x04
    SpriteResource *cellRes;      // 0x08
    SpriteResource *animRes;      // 0x0C
    Sprite *sprite;               // 0x10
    PokeathlonCourseMonInfo info; // 0x14
    u32 topRow;                   // 0x24: first non-transparent pixel row of the first frame
    BOOL shiftedUp;               // 0x28: large frame drawn one tile row higher
} PokeathlonCourseMonGfx;

typedef struct PokeathlonCourseMonGfxManager {
    enum HeapID heapId;           // 0x000
    u16 count;                    // 0x004
    u16 loadedCount;              // 0x006
    BOOL shiftLargeUp;            // 0x008
    BOOL loaded;                  // 0x00C
    BOOL ownsSpriteList;          // 0x010
    SpriteList *ownSpriteList;    // 0x014
    SpriteList *spriteList;       // 0x018
    G2dRenderer renderer;         // 0x01C
    GF_2DGfxResMan *resMans[4];   // 0x144: char, palette, cell, animation
    PokeathlonCourseMonGfx *mons; // 0x154
    u8 buffer[0x800];             // 0x158: one frame of character data
} PokeathlonCourseMonGfxManager;

typedef struct PokeathlonCourseMonGfxTemplate {
    BOOL loadAtOnce;               // 0x00: load all Pokemon in one frame
    int priority;                  // 0x04
    BOOL vramTransfer;             // 0x08
    int vram;                      // 0x0C: 1 main, 2 sub, 3 both
    NNS_G2D_VRAM_TYPE whichScreen; // 0x10
    int sizeTypes[12];             // 0x14: 1..3, selects the shadow / marker offsets of each Pokemon
} PokeathlonCourseMonGfxTemplate;

// The task that loads the requested Pokemon sprites, one per frame.
typedef struct PokeathlonCourseMonGfxLoad {
    u16 count;                               // 0x000
    u16 current;                             // 0x002
    int baseIndex;                           // 0x004
    int state;                               // 0x008
    PokeathlonCourseMonGfxManager *manager;  // 0x00C
    PokeathlonCourseMonGfxTemplate template; // 0x010
    PokeathlonCourseMonInfo infos[16];       // 0x054
} PokeathlonCourseMonGfxLoad;

// ---------------------------------------------------------------------------------------------------------
// Course sprite resources (NARC a/1/5/3)

typedef struct PokeathlonCourseSpriteResources {
    enum HeapID heapId;           // 0x00
    u32 flags;                    // 0x04: which of the 12 sprite groups are loaded
    int charVram;                 // 0x08
    GF_2DGfxResMan *resMans[4];   // 0x0C: char, palette, cell, animation
    SpriteResource *chars[12];    // 0x1C
    SpriteResource *cells[12];    // 0x4C
    SpriteResource *anims[12];    // 0x7C
    SpriteResource *palette;      // 0xAC
    SpriteResource *extraPalette; // 0xB0
} PokeathlonCourseSpriteResources;

// A Pokemon on the course screen: its sprite plus a shadow and a player marker (0x44).
typedef struct PokeathlonCourseMonObject {
    PokeathlonCourseMonGfx *gfx; // 0x00
    Sprite *shadowSprite;        // 0x04
    Sprite *markerSprite;        // 0x08
    s32 markerOffsetY;           // 0x0C
    s32 shadowOffsetY;           // 0x10
    s32 halfWidth;               // 0x14
    s32 halfHeight;              // 0x18
    u8 unk1C[4];                 // 0x1C
    s32 x;                       // 0x20: centre
    s32 y;                       // 0x24
    s32 topRow;                   // 0x28
    s32 unk2C;                   // 0x2C
    u32 unk30;                   // 0x30
    u32 animState;               // 0x34
    u32 anim;                    // 0x38
    s32 drawPriorityStep;        // 0x3C
    BOOL shadowHidden;           // 0x40
} PokeathlonCourseMonObject;

typedef struct PokeathlonCourseMonObjects {
    enum HeapID heapId;                         // 0x000
    int count;                                  // 0x004
    int baseDrawPriority;                       // 0x008
    BOOL loaded;                                // 0x00C
    SpriteList *spriteList;                     // 0x010
    PokeathlonCourseMonGfxManager *gfxManager;  // 0x014
    PokeathlonCourseSpriteResources *resources; // 0x018
    PokeathlonCourseMonObject objects[12];      // 0x01C
} PokeathlonCourseMonObjects;

// ---------------------------------------------------------------------------------------------------------
// Link commands (0x16..0x21). In a single-player course the commands go through a local queue that runs them
// in the next VBlank task, so the course code is the same for both.

#define POKEATHLON_COURSE_CMD_FIRST            0x16
#define POKEATHLON_COURSE_CMD_EVENTS           0x16
#define POKEATHLON_COURSE_CMD_PARTICIPANT      0x17
#define POKEATHLON_COURSE_CMD_ALL_PARTICIPANTS 0x18
#define POKEATHLON_COURSE_CMD_PROFILES         0x19
#define POKEATHLON_COURSE_CMD_RECORD           0x1A
#define POKEATHLON_COURSE_CMD_PACKET           0x1B
#define POKEATHLON_COURSE_CMD_EVENT_DATA       0x1C
#define POKEATHLON_COURSE_CMD_EVENT_RESULT     0x1D
#define POKEATHLON_COURSE_CMD_EVENT_RESULTS    0x1E
#define POKEATHLON_COURSE_CMD_MON_COUNTERS     0x1F
#define POKEATHLON_COURSE_CMD_RESULTS          0x20
#define POKEATHLON_COURSE_CMD_JUICE_RECORD     0x21
#define POKEATHLON_COURSE_CMD_INLINE_SIZE      0x26

typedef void (*PokeathlonCourseCmdFunc)(int netId, int size, void *data, void *context);
typedef int (*PokeathlonCourseCmdSizeFunc)(void);
typedef void *(*PokeathlonCourseCmdBufferFunc)(int netId, void *context, int size);

typedef struct PokeathlonCourseCmd {
    PokeathlonCourseCmdFunc func;
    PokeathlonCourseCmdSizeFunc getSize;
    PokeathlonCourseCmdBufferFunc getBuffer;
} PokeathlonCourseCmd;

typedef struct PokeathlonCourseCmdSlot {
    u32 active;    // 0x00
    u32 cmd;       // 0x04
    u8 data[0x28]; // 0x08
    s32 size;      // 0x30
    void *buffer;  // 0x34
} PokeathlonCourseCmdSlot;

typedef struct PokeathlonCourseCmdQueue {
    PokeathlonCourseCmdSlot slots[8]; // 0x000
    const PokeathlonCourseCmd *cmds;  // 0x1C0
    int cmdCount;                     // 0x1C4
    void *context;                    // 0x1C8
    SysTask *task;                    // 0x1CC
} PokeathlonCourseCmdQueue;

typedef struct PokeathlonCourseLink {
    u16 isLink;                      // 0x0: 1 = wireless link, 0 = local queue
    u16 checkSize;                   // 0x2
    PokeathlonCourseCmdQueue *queue; // 0x4
} PokeathlonCourseLink;

// ---------------------------------------------------------------------------------------------------------
// Parts of PokeathlonCourseData (pokeathlon.h) that its declaration still leaves as filler

typedef struct PokeathlonCoursePoint {
    u16 x;
    u16 y;
} PokeathlonCoursePoint;

// One entry of NARC poketool/personal_performance (0x14).
typedef struct PokeathlonCoursePerformance {
    u8 raw[0x14];
} PokeathlonCoursePerformance;

// The performance data of every participant's Pokemon (PokeathlonCourseData 0x618).
typedef struct PokeathlonCoursePerformanceTable {
    PokeathlonCoursePerformance mons[4][3];
} PokeathlonCoursePerformanceTable;

// A marker sprite following a Pokemon (PokeathlonCourseData 0xD30).
typedef struct PokeathlonCourseMarker {
    BOOL active;
    PokeathlonCourseMonObject *object;
    Sprite *sprite;
    SysTask *task;
} PokeathlonCourseMarker;

typedef struct PokeathlonCourseMarkers {
    u32 count;
    PokeathlonCourseMarker markers[3];
} PokeathlonCourseMarkers;

// PokeathlonCourseArgs (pokeathlon.h) as the course module uses it.
typedef struct PokeathlonCourseArgsLayout {
    SaveData *saveData; // 0x0
    u32 mode;           // 0x4: 1 = link
    u16 points;         // 0x8: athlete points won (out)
    u16 totalPoints;    // 0xA: course total (out)
    u8 course;          // 0xC: 0..4 solo courses, 5..10 link courses (10 = random events)
    u8 rank;            // 0xD (out)
    u8 shouldFreeHeap;  // 0xE
    u8 isSupreme;         // 0xF: harder NPC teams
} PokeathlonCourseArgsLayout;

// A player's result of the event that is running (exchanged with link commands 0x1D / 0x1E).
typedef struct PokeathlonCourseEventResult {
    u16 unk0;
    u16 unk2;
} PokeathlonCourseEventResult;

typedef void (*PokeathlonCourseFinishFunc)(PokeathlonCourseData *course);

// A 0x28-byte link packet of the event that is running; the data copy area holds 7 of them.
typedef struct PokeathlonCoursePacket {
    u8 data[0x24]; // 0x00
    u8 isHostPacket;      // 0x24
    u8 unk25[3];   // 0x25
} PokeathlonCoursePacket;

// Callbacks of the event that is running (PokeathlonCourseData.state points to the event's table).
typedef struct PokeathlonCourseEvent {
    void *init;
    void (*draw)(PokeathlonCourseData *course, u32 param);
    void (*onResults)(PokeathlonCourseData *course, u32 param);
    void (*exit)(PokeathlonCourseData *course, u32 param);
    BOOL unk10;
} PokeathlonCourseEvent;

typedef BOOL (*PokeathlonCourseStepFunc)(PokeathlonCourseData *course, u8 *state);

// The course results (PokeathlonCourseData 0x72C; its first 4 PokeathlonFieldData are the counters).
typedef struct PokeathlonCourseResults {
    u32 counters[4][3][8]; // 0x000: per participant and Pokemon, capped at 9999
    u32 bonuses;            // 0x180
    u32 instructions;            // 0x184
    u16 eventPoints[4][4]; // 0x188: [participant][round]
    u16 eventScores[4][4]; // 0x1A8: [participant][round]
    u8 wonRound[4];        // 0x1C8
    u8 lastInRound[4];     // 0x1CC
    u16 wonCourse : 1;     // 0x1D0
    u16 lastInCourse : 1;
    u16 rank : 2;
    u16 unkBits4 : 12;
    u16 totalPoints;                    // 0x1D2
    PokeathlonStatistics record; // 0x1D4: the player's saved record
} PokeathlonCourseResults;

// Full layout of PokeathlonCourseData (0xD70). pokeathlon.h names only part of it; the course functions view
// the course through this type where they use the parts it leaves as filler.
typedef struct PokeathlonCourseLayout {
    u32 field_0[4];                               // 0x000
    u8 filler_10[0x14C];                          // 0x010
    u8 field_15C;                                 // 0x15C
    u8 filler_15D[0x83];                          // 0x15D
    const PokeathlonCourseEvent *event;           // 0x1E0
    void *heapAllocPtr4;                          // 0x1E4
    u8 eventIds[4];                               // 0x1E8: event of each round, as sent to the other players
    u8 unk1EC;                                    // 0x1EC
    u8 eventInitState;                                 // 0x1ED
    u8 participantCount;                          // 0x1EE
    u8 receivedCount;                                 // 0x1EF
    u32 round;                                    // 0x1F0
    u32 eventDrawEnabled;                                // 0x1F4
    PokeathlonCourseArgsLayout *args;             // 0x1F8
    u32 unk1FC;                                   // 0x1FC
    PokeathlonCourseParticipant localParticipant; // 0x200
    u32 field_27C;                                // 0x27C
    OverlayManager *subOverlay;                   // 0x280
    enum HeapID heapId;                           // 0x284
    PokeathlonCourseLink *link;                   // 0x288
    PokeathlonCoursePacket packets[7];            // 0x28C: the data copy area
    u32 eventDataTransfer[4];                             // 0x3A4
    PokeathlonCourseState courseState;            // 0x3B4
    PokeathlonStateInfo stateInfo;                // 0x3C4
    PokeathlonStateInfo eventStateInfo;           // 0x3CC
    u32 unk3D4;                                   // 0x3D4
    u32 roundEvents[4];                           // 0x3D8
    u32 courseId;                                // 0x3E8
    PokeathlonCourseParticipant participants[4];  // 0x3EC
    PlayerProfile *playerProfiles;                // 0x5DC
    PokeathlonCourseEventResult eventResults[4];  // 0x5E0: the player's own, then (host) everybody's
    u16 receivedResults[18];                      // 0x5F0: the results the host sent
    PokeathlonCourseScoreList *scores;            // 0x614
    PokeathlonCoursePerformanceTable performance; // 0x618
    Sprite *countSprite;                          // 0x708: event HUD: "3, 2, 1" and "Finish"
    u8 hudTimer;                                  // 0x70C
    u8 finishState;                               // 0x70D
    u16 hudBaseY;                                 // 0x70E
    Sprite *newRecordSprite;                      // 0x710
    Sprite *timeUpSprite;                         // 0x714
    u32 hudPlanes;                                // 0x718: BG planes shown again after "Finish"
    Sprite *startSprites[3];                      // 0x71C
    u8 startSpriteCount;                          // 0x728
    u8 startState;                                // 0x729
    u8 rounds;                                    // 0x72A
    u8 timeUpSeconds;                             // 0x72B
    PokeathlonCourseResults results;              // 0x72C
    PokeathlonStatistics records[4];       // 0x974: every player's saved record
    u32 localCounters[3][8];                      // 0xB44
    u8 monCounters[0x180];                          // 0xBA4
    s32 frameCounter;                             // 0xD24
    u16 frameTimer;                               // 0xD28
    u16 recordScore;                              // 0xD2A: the event result compared with the saved record
    u32 counterEnabled;                           // 0xD2C
    PokeathlonCourseMarkers markers;              // 0xD30
    void *heapAllocPtr2;                          // 0xD64
    void *heapAllocPtr3;                          // 0xD68
    PokeathlonCourseFinishFunc finishCallback;    // 0xD6C
} PokeathlonCourseLayout;

typedef struct PokeathlonCourseUnkTypes {
    u8 unk00[6];
    u8 collisionSizeClass;
    u8 unk07;
    u8 spriteSizeType;
} PokeathlonCourseUnkTypes;

// ---------------------------------------------------------------------------------------------------------
// Saved records (parts of PokeathlonSave)

// A Pokemon of a course record (0xC).
typedef struct PokeathlonCourseSavedMon {
    u32 species : 9;
    u32 form : 5;
    u32 gender : 2;
    u32 isShiny : 1;
    u32 unused : 15;
    u32 personality;
    u32 unk08;
} PokeathlonCourseSavedMon;

// Best total of a course with its round points and Pokemon (0x2C; one per course at the start of PokeathlonSave).
typedef struct PokeathlonCourseBestRecord {
    u16 roundPoints[3];
    u16 totalPoints;
    PokeathlonCourseSavedMon mons[3];
} PokeathlonCourseBestRecord;

// A result in an event ranking: score, the Pokemon (form << 10 | species) and, in link rankings, the trainer.
typedef struct PokeathlonCourseRankEntry {
    u16 score;
    u16 mons[3];
    u32 trainerId;
    u16 name[8];
    u8 language;
    u8 padding[3];
} PokeathlonCourseRankEntry;

typedef struct PokeathlonCourseEventRecordRow {
    u16 score;
    u16 mons[3];
} PokeathlonCourseEventRecordRow;

// Top 5 of a solo event (Pokeathlon_RecordsSolo).
typedef struct PokeathlonCourseEventRecord {
    PokeathlonCourseEventRecordRow top[5];
    u32 timesPlayed;
} PokeathlonCourseEventRecord;

typedef struct PokeathlonCourseTrainerRecord {
    u32 trainerId;
    u16 name[8];
    u8 language;
    u8 padding[3];
} PokeathlonCourseTrainerRecord;

// Top 5 of a link event (Pokeathlon_RecordsLink).
typedef struct PokeathlonCourseLinkEventRecord {
    PokeathlonCourseEventRecordRow top[5];
    u32 timesPlayed;
    PokeathlonCourseTrainerRecord trainers[5];
} PokeathlonCourseLinkEventRecord;

// An NPC team (NARC a/2/5/8).
typedef struct PokeathlonCourseNpcTeam {
    u16 species[3];
    u8 nameId; // msg 310
    u8 msgId;
} PokeathlonCourseNpcTeam;

// ---------------------------------------------------------------------------------------------------------
// Defined elsewhere in OVY_96 (other event / course modules, data in asm)

extern const PokeathlonCourseEvent gPokeathlonCourseEvent_Opening;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_Closing;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_ResultsAnnouncement;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_Wait;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_EventIntro;
extern const PokeathlonCourseEvent *const gPokeathlonCourseEvents[]; // events by event id
extern const PokeathlonCourseStepFunc *const gPokeathlonCourseEventSteps[];  // their state functions
extern const PokeathlonCourseStepFunc gPokeathlonCourseOpeningSteps[];
extern const PokeathlonCourseStepFunc gPokeathlonCourseClosingSteps[];
extern const PokeathlonCourseStepFunc gPokeathlonCourseWaitSteps[];
extern const PokeathlonCourseStepFunc gPokeathlonEventIntroSteps[];
extern const PokeathlonCourseStepFunc gPokeathlonResultsAnnouncementSteps[];
extern const u8 gPokeathlonEventScoreOrders[];        // score order of each event (0: higher is better, 1: lower)
extern const u16 gPokeathlonEventTargetScores[];   // target score of each event
extern const u8 gPokeathlonCourseEventIds[][4]; // events of the courses 0..9
extern const u8 gPokeathlonCourse10EventSets[][4]; // the 10 event sets of course 10
extern const u16 gPokeathlonNpcTeamSpriteIds[];   // per NPC team
extern const u16 gPokeathlonPerformanceFileIds[];   // performance file of each species' first form
extern const PokeathlonCourseCmd gPokeathlonCourseCmds[];

SpriteList *PokeathlonCourseSpritePool_GetSpriteList(void *eventGfx);
BOOL PokeathlonCourseObject_IsDrawn(PokeathlonCourseMonObject *object);

// Declared by no header (defined in src/apricorn_box.c and the communication code).
u32 ApricornJuiceRecord_sizeof(void);
ApricornRecord *ApricornRecord_New(SaveApricornBox *box, PlayerProfile *profile, u32 value, enum HeapID heapId);
void ApricornBox_StoreJuiceRecords(SaveApricornBox *box, ApricornRecord *records, int count, int skip);
BOOL Comm_SendCommandNoCopy(int command, void *data, int size);

// pokeathlon.h declares PokeathlonCourseLink_SendCommand as void; it returns whether the command was sent.
#define PokeathlonCourse_TrySendCommand(cmd, data, size, link) (((BOOL(*)(int, void *, void *, int))PokeathlonCourseLink_SendCommand)(cmd, data, size, link))

// Course flow, results, link commands (pokeathlon_course_flow.c)
void PokeathlonCourse_LoadPerformanceData(PokeathlonCourseData *course);
PokeathlonCoursePerformance *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int mon);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int mon);
int PokeathlonCourse_GetTouchRadius(void);
int PokeathlonCourse_GetSpriteSizeType(const PokeathlonCourseUnkTypes *types);
int PokeathlonCourse_GetCollisionSizeClass(const PokeathlonCourseUnkTypes *types);
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int which, PokeathlonCourseMonInfo *info);
Sprite **PokeathlonCourse_CreateHudSprites(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList);
Sprite **PokeathlonCourse_CreateHudSpritesInPool(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, void *eventGfx);
void PokeathlonCourse_CreateStartSprites(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, NNS_G2D_VRAM_TYPE whichScreen, u8 count, PokeathlonCoursePoint *positions);
void PokeathlonCourse_CreateStartSpritesInPool(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, void *eventGfx, NNS_G2D_VRAM_TYPE whichScreen, u8 count, PokeathlonCoursePoint *positions);
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, int framesLeft);
void PokeathlonCourse_InitMarkers(PokeathlonCourseData *course);
Sprite *PokeathlonCourse_AddMarker(PokeathlonCourseData *course, PokeathlonCourseMonObject *object, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority);
void PokeathlonCourse_FreeMarkers(PokeathlonCourseData *course);
void PokeathlonCourse_SetMarkerAnim(PokeathlonCourseData *course, int index, int seq);
void PokeathlonCourse_StopMarkerAnims(PokeathlonCourseData *course);
void PokeathlonCourse_ShowStartSprites(PokeathlonCourseData *course);
BOOL PokeathlonCourse_IsStartAnimDone(PokeathlonCourseData *course);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, u32 planes);
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
int PokeathlonCourse_GetPerformanceFileId(int species, int form);
u32 PokeathlonCourse_State_SendEvents(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunEventOverlay(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunWait(PokeathlonCourseData *course, u8 *state);
u32 PokeathlonCourse_State_SendParticipant(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendParticipants(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendProfiles(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendRecord(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_StartOpening(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_WaitOpeningStart(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunOpening(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_EndOpening(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_StartEventIntro(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_WaitEventIntroStart(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunEventIntro(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_EndEventIntro(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_StartRound(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_WaitRoundStart(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendEventData(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunEvent(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_ResetDataCopyArea(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendEventResult(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendEventResults(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_EndRound(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_ExitEvent(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_NextRound(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_StartResultsAnnouncement(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_WaitResultsAnnouncementStart(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunResultsAnnouncement(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_EndResultsAnnouncement(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_StartClosing(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_WaitClosingStart(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunClosing(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_CreateJuiceRecords(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendMonCounters(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendResults(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_SendJuiceRecord(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_StoreJuiceRecords(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_Exit(PokeathlonCourseData *course);
int PokeathlonCourse_State_WaitForCommand(void);
BOOL PokeathlonCourse_SendEventResults(PokeathlonCourseData *course);
void PokeathlonCourse_StoreRoundResults(PokeathlonCourseData *course, int round);
void PokeathlonCourse_SaveResults(PokeathlonCourseData *course);
void PokeathlonCourse_SaveCourseRecord(PokeathlonCourseData *course, PokeathlonSave *pokeathlon);
void PokeathlonCourse_SaveSoloEventRecords(PokeathlonCourseData *course, PokeathlonCourseEventRecord *records);
void PokeathlonCourse_SaveLinkEventRecords(PokeathlonCourseData *course, PokeathlonCourseLinkEventRecord *records);
void PokeathlonCourse_SaveSoloEventBests(PokeathlonCourseData *course, Pokeathlon_RecordsSolo *records, u16 *bests);
void PokeathlonCourse_InsertRankEntry(u32 order, PokeathlonCourseRankEntry *entry, PokeathlonCourseRankEntry *top);
void PokeathlonCourse_CopySoloRanking(PokeathlonCourseData *course, PokeathlonCourseEventRecord *record, PokeathlonCourseRankEntry *top);
void PokeathlonCourse_CopyLinkRanking(PokeathlonCourseData *course, PokeathlonCourseLinkEventRecord *record, PokeathlonCourseRankEntry *top);
BOOL PokeathlonCourse_BeatsRecord(int record, int score, u32 order);
BOOL PokeathlonCourse_BeatsRecordInOrder(int record, int score, u32 order);
void PokeathlonCourse_SaveStatistics(PokeathlonCourseData *course, PokeathlonStatistics *statistics);
void PokeathlonCourse_SaveFriendshipStatues(PokeathlonCourseData *course);
void PokeathlonCourse_SetCourseMedals(PokeathlonCourseData *course);
void PokeathlonCourse_SetMonMedals(PokeathlonCourseData *course, u32 medals);
BOOL PokeathlonCourse_HasAllCourseRecords(PokeathlonSave *pokeathlon);
BOOL PokeathlonCourse_HasAllMedalsOfAMon(PokeathlonCourseData *course);
BOOL PokeathlonCourse_BeatsAllEventTargets(PokeathlonCourseData *course);
void PokeathlonCourse_ChooseEvents(PokeathlonCourseArgsLayout *args, u32 rounds, u8 *eventIds);
void PokeathlonCourse_MarkerTask(SysTask *task, void *data);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u32 participant, u32 mon, int counter, int amount);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u16 score);
void PokeathlonCourse_SetFinishCallback(PokeathlonCourseData *course, PokeathlonCourseFinishFunc callback);
void PokeathlonCourse_AddToCounter(u32 counter, u32 amount, u32 *counters);
BOOL PokeathlonCourse_IsNewRecord(PokeathlonCourseData *course);
u16 PokeathlonCourse_GetNpcTeamSpriteId(u32 npcTeam);
u8 PokeathlonCourse_GetNpcTeamMsgId(PokeathlonCourseData *course, u32 npcTeam);
void PokeathlonCourse_SetupNpcParticipants(PokeathlonCourseData *course, int count);
void PokeathlonCourse_ChooseNpcTeams(u32 count, u8 courseId, u32 hard, u8 *npcTeams);
void *PokeathlonCourse_GetJuiceRecord(PokeathlonCourseData *course, u32 index);
BOOL PokeathlonCourse_IsInRange(int x, int y, int targetX, int targetY, int range, int *distance);
void PokeathlonCourseLink_SetCheckSize(PokeathlonCourseLink *link, u16 checkSize);
BOOL PokeathlonCourseLink_SendLargeCommand(int cmd, void *data, int size, PokeathlonCourseLink *link);
PokeathlonCourseCmdQueue *PokeathlonCourseCmdQueue_New(const PokeathlonCourseCmd *cmds, int cmdCount, void *context, enum HeapID heapId);
void PokeathlonCourseCmdQueue_Task(SysTask *task, void *data);
void PokeathlonCourseCmdQueue_Free(PokeathlonCourseCmdQueue *queue);
void PokeathlonCourseCmdQueue_Add(PokeathlonCourseCmdQueue *queue, u32 cmd, void *data, s32 size);
void PokeathlonCourseCmdQueue_AddLarge(PokeathlonCourseCmdQueue *queue, u32 cmd, void *data, s32 size);
PokeathlonCourseMonGfxManager *PokeathlonCourseMonGfx_AllocManager(enum HeapID heapId);

// Pokemon sprites (pokeathlon_course_mon_gfx.c)
void PokeathlonCourseMonGfx_InitManager(PokeathlonCourseMonGfxManager *manager, u32 count, SpriteList *spriteList);
void PokeathlonCourseMonGfx_FreeManager(PokeathlonCourseMonGfxManager *manager);
void PokeathlonCourseMonGfx_StartLoad(PokeathlonCourseMonGfxManager *manager, s32 count, PokeathlonCourseMonInfo *infos, PokeathlonCourseMonGfxTemplate *template, u32 priority, int baseIndex);
BOOL PokeathlonCourseMonGfx_IsLoaded(PokeathlonCourseMonGfxManager *manager);
PokeathlonCourseMonGfx *PokeathlonCourseMonGfx_Get(PokeathlonCourseMonGfxManager *manager, int index);
Sprite *PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonGfx *mon);
PokeathlonCourseMonInfo *PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonGfx *mon);
void PokeathlonCourseMonGfx_LoadPalette(PokeathlonCourseMonInfo *info, enum HeapID heapId, void *dest);
void PokeathlonCourseMonGfx_SetPalette(PokeathlonCourseMonGfxManager *manager, int index, int vram, void *palette);
void PokeathlonCourseMonGfx_Load(PokeathlonCourseMonGfxManager *manager, PokeathlonCourseMonGfx *mon, PokeathlonCourseMonInfo *info, int vram, u8 id, BOOL vramTransfer);
void PokeathlonCourseMonGfx_CreateSprite(PokeathlonCourseMonGfxManager *manager, int priority, int vramTransfer, NNS_G2D_VRAM_TYPE whichScreen, u8 resourceId, u8 index);
void PokeathlonCourseMonGfx_LoadTask(SysTask *task, void *data);
BOOL PokeathlonCourseMonGfx_LoadStep(PokeathlonCourseMonGfxLoad *load);
u8 PokeathlonCourseMonGfx_GetTopRow(PokeathlonCourseMonGfx *mon);
u8 PokeathlonCourseMonGfx_FindTopRow(PokeathlonCourseMonGfx *mon);
u8 PokeathlonCourseMonGfx_FindTopRowInTile(const u8 *data, u32 offset);
int PokeathlonCourse_GetMonModelFileId(int species, int form, int gender);
void PokeathlonCourseMonGfx_SetScale(PokeathlonCourseMonGfx *mon, f32 x, f32 y);
void PokeathlonCourseMonGfx_ShiftLargeUp(PokeathlonCourseMonGfxManager *manager);
BOOL PokeathlonCourseMonGfx_IsShiftedUp(PokeathlonCourseMonGfx *mon);
void PokeathlonCourse_InitObjTransfer(ObjCharTransferTemplate *charTemplate, int plttCount, enum HeapID heapId, GXOBJVRamModeChar modeMain, GXOBJVRamModeChar modeSub);
void PokeathlonCourse_FreeObjTransfer(void);

// Score list and link command handlers (pokeathlon_course_scores.c)
PokeathlonCourseScore *PokeathlonCourseScoreList_AddEntry(PokeathlonCourseScoreList *list);
PokeathlonCourseScore *PokeathlonCourseScoreList_GetEntry(PokeathlonCourseScoreList *list, u32 index);
void PokeathlonCourseScoreList_Reset(PokeathlonCourseScoreList *list);
void PokeathlonCourseScoreList_Rank(PokeathlonCourseScoreList *list, int mode);
PokeathlonCourseScore *PokeathlonCourseScoreList_GetRanked(PokeathlonCourseScoreList *list, u32 index);
void PokeathlonCourseScoreList_Init(PokeathlonCourseScoreList *list);
s32 PokeathlonCourseScoreList_GetCount(PokeathlonCourseScoreList *list);
u32 PokeathlonCourseScoreList_GetField04(PokeathlonCourseScoreList *list);
void PokeathlonCourseScoreList_AddPointsToTotals(PokeathlonCourseScoreList *list);
void PokeathlonCourseScoreList_SaveEventPoints(PokeathlonCourseScoreList *list, int event);
u32 PokeathlonCourseScoreList_GetEventPoints(PokeathlonCourseScoreList *list, int event, int participant);
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, PokeathlonCourseScoreList *list, u32 participant, u32 score, u8 idx, u8 digits, u32 msgno);
void PokeathlonCourse_AddScoreEntry2(PokeathlonCourseData *course, PokeathlonCourseScoreList *list, u32 participant, u32 score, u32 value1, u32 value2, u8 idx1, u8 idx2, u8 digits1, u8 digits2, u32 msgno);
void PokeathlonCourseCmd_RecvEvents(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvParticipant(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvAllParticipants(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvProfiles(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvRecord(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvPacket(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvEventData(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvEventResult(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvEventResults(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvMonCounters(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvResults(int netId, int size, void *data, void *context);
void PokeathlonCourseCmd_RecvJuiceRecord(int netId, int size, void *data, void *context);
int PokeathlonCourseCmd_EventsSize(void);
int PokeathlonCourseCmd_ParticipantSize(void);
int PokeathlonCourseCmd_AllParticipantsSize(void);
u32 PokeathlonCourseCmd_ProfilesSize(void);
int PokeathlonCourseCmd_RecordSize(void);
int PokeathlonCourseCmd_MonCountersSize(void);
int PokeathlonCourseCmd_ResultsSize(void);
u32 PokeathlonCourseCmd_JuiceRecordSize(void);
void *PokeathlonCourseCmd_GetEventDataBuffer(int netId, void *context);
void *PokeathlonCourseCmd_GetRecordBuffer(int netId, void *context);
void *PokeathlonCourseCmd_GetResultsBuffer(int netId, void *context);
void *PokeathlonCourseCmd_GetMonCountersBuffer(int netId, void *context, int size);
void *PokeathlonCourseCmd_GetJuiceRecordBuffer(int netId, void *context);

// Sprite resources and course Pokemon objects (pokeathlon_course_sprites.c)
PokeathlonCourseSpriteResources *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, u32 flags, int charVram);
void PokeathlonCourseSprites_FreeResources(PokeathlonCourseSpriteResources *resources);
u8 PokeathlonCourseSprites_HasGroup(PokeathlonCourseSpriteResources *resources, u32 mask);
GF_2DGfxResMan *PokeathlonCourseSprites_GetResMan(PokeathlonCourseSpriteResources *resources, int type);
void PokeathlonCourseSprites_LoadResources(PokeathlonCourseSpriteResources *resources);
Sprite *PokeathlonCourseSprites_CreateCountdown(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, NNS_G2D_VRAM_TYPE whichScreen, enum HeapID heapId);
Sprite *PokeathlonCourseSprites_CreateTimerDigit(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
Sprite *PokeathlonCourse_CreateTimerDigitSprite(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
Sprite *PokeathlonCourseSprites_CreateLapCounterDigit(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
Sprite *PokeathlonCourseSprites_CreateNewRecord(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
Sprite *PokeathlonCourseSprites_CreateTimeUpCountdown(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
Sprite *PokeathlonCourse_CreateTimerDigitSpriteAlt(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
Sprite *PokeathlonCourseSprites_CreateStart(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, NNS_G2D_VRAM_TYPE whichScreen, int priority, enum HeapID heapId);
Sprite *PokeathlonCourseSprites_CreateMarker(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
PokeathlonCourseMonObjects *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int count, int baseDrawPriority, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList);
void PokeathlonCourseMonObjects_Free(PokeathlonCourseMonObjects *objects);
void PokeathlonCourseMonObjects_Load(PokeathlonCourseMonObjects *objects, int count, PokeathlonCourseMonInfo *infos, PokeathlonCourseMonGfxTemplate *template, u32 priority, int baseIndex);
BOOL PokeathlonCourseMonObjects_IsLoaded(PokeathlonCourseMonObjects *objects);
PokeathlonCourseMonObject *PokeathlonCourseMonObjects_Get(PokeathlonCourseMonObjects *objects, int index);
PokeathlonCourseMonGfx *PokeathlonCourseMonObject_GetGfx(PokeathlonCourseMonObject *object);
void PokeathlonCourseMonObjects_FinishLoad(SysTask *task, void *data);
Sprite *PokeathlonCourseMonObjects_CreateSprite(PokeathlonCourseMonObjects *objects, int priority, NNS_G2D_VRAM_TYPE whichScreen, int resourceId);
void PokeathlonCourseMonObject_SetDrawFlag(PokeathlonCourseMonObject *object, BOOL draw);
void PokeathlonCourseMonObject_SetShadowDrawFlag(PokeathlonCourseMonObject *object, BOOL draw);
void PokeathlonCourseMonObject_SetMonDrawFlag(PokeathlonCourseMonObject *object, BOOL draw);
void PokeathlonCourseMonObject_SetDrawPriority(PokeathlonCourseMonObject *object, int drawPriority);
void PokeathlonCourseMonObject_SetDrawPriorityStep(PokeathlonCourseMonObject *object, s32 step);
void PokeathlonCourseMonObject_SetAffineOverwriteMode(PokeathlonCourseMonObject *object, u8 mode);
void PokeathlonCourseMonObject_SetAffineMatrix(PokeathlonCourseMonObject *object, VecFx32 *matrix);
void PokeathlonCourseMonObject_SetAnimStateRaw(PokeathlonCourseMonObject *object, u32 animState);
void PokeathlonCourseMonObject_SetAnimState(PokeathlonCourseMonObject *object, u32 animState);
void PokeathlonCourseMonObject_SetAnim(PokeathlonCourseMonObject *object, int anim);
void PokeathlonCourseMonObject_SetAnimCode(PokeathlonCourseMonObject *object, int anim);
BOOL PokeathlonCourseMonObject_IsAnimated(PokeathlonCourseMonObject *object);
void PokeathlonCourseMonObject_SetPosition(PokeathlonCourseMonObject *object, s32 x, s32 y, BOOL moveShadow);
void PokeathlonCourseMonObject_SetMonPosition(PokeathlonCourseMonObject *object, s32 x, s32 y);
void PokeathlonCourseMonObject_GetSpritePosition(PokeathlonCourseMonObject *object, s32 *x, s32 *y);
void PokeathlonCourseMonObject_GetPosition(PokeathlonCourseMonObject *object, s32 *x, s32 *y);
void PokeathlonCourseMonObject_Move(PokeathlonCourseMonObject *object, s32 dx, s32 dy, u32 animState, BOOL moveShadow);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_INTERNAL_H
