#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_INTERNAL_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_INTERNAL_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_save.h"

#include "apricorn_internal.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sys_task.h"

// Course participants

// One Pokemon of a participant's team.
typedef struct PokeathlonCourseMon {
    u16 species;
    u16 form;
    u32 personality;
    u8 stats[5]; // current Pokeathlon performance (power, stamina, jump, skill, speed)
    u8 padding_0D;
    u16 starColors; // 3 bits per stat, in the order of stats
    u8 isShiny;
    u8 gender;
    u16 nickname[11];
} PokeathlonCourseMon;

// A course participant. PokeathlonCourseData stores four of them as PokeathlonParticipantData.
typedef struct PokeathlonCourseParticipant {
    u32 npcTeam; // NARC a/2/5/8 entry + 1 of an NPC team, 0 for a player
    PokeathlonCourseMon mons[3];
} PokeathlonCourseParticipant;

// Score list (PokeathlonCourseData.graphicsSystem): the per-event result rows and the course point totals

#define POKEATHLON_COURSE_SCORE_ENTRY_MAX  12
#define POKEATHLON_COURSE_EVENT_POINTS_MAX 10

struct PokeathlonCourseScore {
    s32 participant;
    u8 padding_04[5];
    u8 rank;
    u16 points;
    s32 score;
    String *text;
    String *name;
};

struct PokeathlonCourseScoreList {
    s32 count;
    u32 unk04;
    s32 next;
    u32 sortOrder;
    BOOL totalsAdded;
    u32 totals[4];
    PokeathlonCourseScore entries[POKEATHLON_COURSE_SCORE_ENTRY_MAX];
    PokeathlonCourseScore *ranking[POKEATHLON_COURSE_SCORE_ENTRY_MAX];
    u32 eventPoints[POKEATHLON_COURSE_EVENT_POINTS_MAX][4];
};

// Doubly linked node of the insertion sort in PokeathlonCourseScoreList_Rank.
typedef struct PokeathlonCourseScoreNode {
    struct PokeathlonCourseScoreNode *prev;
    struct PokeathlonCourseScoreNode *next;
    PokeathlonCourseScore *entry;
} PokeathlonCourseScoreNode;

// Pokemon sprites built from the overworld models (NARC data/mmodel/mmodel)

// One loaded Pokemon sprite.
struct PokeathlonCourseMonGfx {
    SpriteResource *charRes;
    SpriteResource *plttRes;
    SpriteResource *cellRes;
    SpriteResource *animRes;
    Sprite *sprite;
    PokeathlonCourseMonInfo info;
    u32 topRow;     // first non-transparent pixel row of the first frame
    BOOL shiftedUp; // large frame drawn one tile row higher
};

struct PokeathlonCourseMonGfxManager {
    enum HeapID heapId;
    u16 count;
    u16 loadedCount;
    BOOL shiftLargeUp;
    BOOL loaded;
    BOOL ownsSpriteList;
    SpriteList *ownSpriteList;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4]; // char, palette, cell, animation
    PokeathlonCourseMonGfx *mons;
    u8 buffer[0x800]; // one frame of character data
};

struct PokeathlonCourseMonGfxTemplate {
    BOOL loadAtOnce; // load all Pokemon in one frame
    int priority;
    BOOL vramTransfer;
    int vram; // 1 main, 2 sub, 3 both
    NNS_G2D_VRAM_TYPE whichScreen;
    int sizeTypes[12]; // 1..3, selects the shadow / marker offsets of each Pokemon
};

// The task that loads the requested Pokemon sprites, one per frame.
typedef struct PokeathlonCourseMonGfxLoad {
    u16 count;
    u16 current;
    int baseIndex;
    int state;
    PokeathlonCourseMonGfxManager *manager;
    PokeathlonCourseMonGfxTemplate template;
    PokeathlonCourseMonInfo infos[16];
} PokeathlonCourseMonGfxLoad;

// Course sprite resources (NARC a/1/5/3)

struct PokeathlonCourseSpriteResources {
    enum HeapID heapId;
    u32 flags; // which of the 12 sprite groups are loaded
    int charVram;
    GF_2DGfxResMan *resMans[4]; // char, palette, cell, animation
    SpriteResource *chars[12];
    SpriteResource *cells[12];
    SpriteResource *anims[12];
    SpriteResource *palette;
    SpriteResource *extraPalette;
};

// Course states, in the order of sPokeathlonStateInfoFuncTable (PokeathlonCourse_SetNextState)

enum PokeathlonCourseStateId {
    POKEATHLON_COURSE_STATE_SEND_EVENTS,
    POKEATHLON_COURSE_STATE_RUN_EVENT_OVERLAY,
    POKEATHLON_COURSE_STATE_RUN_WAIT,
    POKEATHLON_COURSE_STATE_SEND_PARTICIPANT,
    POKEATHLON_COURSE_STATE_SEND_PARTICIPANTS,
    POKEATHLON_COURSE_STATE_SEND_PROFILES,
    POKEATHLON_COURSE_STATE_SEND_RECORD,
    POKEATHLON_COURSE_STATE_START_OPENING,
    POKEATHLON_COURSE_STATE_WAIT_OPENING_START,
    POKEATHLON_COURSE_STATE_RUN_OPENING,
    POKEATHLON_COURSE_STATE_END_OPENING,
    POKEATHLON_COURSE_STATE_START_EVENT_INTRO,
    POKEATHLON_COURSE_STATE_WAIT_EVENT_INTRO_START,
    POKEATHLON_COURSE_STATE_RUN_EVENT_INTRO,
    POKEATHLON_COURSE_STATE_END_EVENT_INTRO,
    POKEATHLON_COURSE_STATE_START_ROUND,
    POKEATHLON_COURSE_STATE_WAIT_ROUND_START,
    POKEATHLON_COURSE_STATE_SEND_EVENT_DATA,
    POKEATHLON_COURSE_STATE_RUN_EVENT,
    POKEATHLON_COURSE_STATE_RESET_DATA_COPY_AREA,
    POKEATHLON_COURSE_STATE_SEND_EVENT_RESULT,
    POKEATHLON_COURSE_STATE_SEND_EVENT_RESULTS,
    POKEATHLON_COURSE_STATE_END_ROUND,
    POKEATHLON_COURSE_STATE_EXIT_EVENT,
    POKEATHLON_COURSE_STATE_NEXT_ROUND,
    POKEATHLON_COURSE_STATE_START_RESULTS,
    POKEATHLON_COURSE_STATE_WAIT_RESULTS_START,
    POKEATHLON_COURSE_STATE_RUN_RESULTS,
    POKEATHLON_COURSE_STATE_END_RESULTS,
    POKEATHLON_COURSE_STATE_SEND_MON_COUNTERS,
    POKEATHLON_COURSE_STATE_SEND_RESULTS,
    POKEATHLON_COURSE_STATE_START_CLOSING,
    POKEATHLON_COURSE_STATE_WAIT_CLOSING_START,
    POKEATHLON_COURSE_STATE_RUN_CLOSING,
    POKEATHLON_COURSE_STATE_CREATE_JUICE_RECORDS,
    POKEATHLON_COURSE_STATE_SEND_JUICE_RECORD,
    POKEATHLON_COURSE_STATE_STORE_JUICE_RECORDS,
    POKEATHLON_COURSE_STATE_EXIT,
    POKEATHLON_COURSE_STATE_WAIT_FOR_COMMAND,
};

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
    u32 active;
    u32 cmd;
    u8 data[0x28];
    s32 size;
    void *buffer;
} PokeathlonCourseCmdSlot;

typedef struct PokeathlonCourseCmdQueue {
    PokeathlonCourseCmdSlot slots[8];
    const PokeathlonCourseCmd *cmds;
    int cmdCount;
    void *context;
    SysTask *task;
} PokeathlonCourseCmdQueue;

struct PokeathlonCourseLink {
    u16 isLink; // 1 = wireless link, 0 = local queue
    u16 checkSize;
    PokeathlonCourseCmdQueue *queue;
};

struct PokeathlonCoursePoint {
    u16 x;
    u16 y;
};

// One entry of NARC poketool/personal_performance.
struct PokeathlonCoursePerformance {
    u8 unk00[6];
    u8 collisionSizeClass;
    u8 unk07;
    u8 spriteSizeType;
    u8 unk09[11];
};

// The performance data of every participant's Pokemon.
typedef struct PokeathlonCoursePerformanceTable {
    PokeathlonCoursePerformance mons[4][3];
} PokeathlonCoursePerformanceTable;

// A marker sprite following a Pokemon.
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

typedef struct PokeathlonCourseArgsLayout {
    SaveData *saveData;
    u32 mode;        // 1 = link
    u16 points;      // athlete points won (out)
    u16 totalPoints; // course total (out)
    u8 course;       // 0..4 solo courses, 5..10 link courses (10 = random events)
    u8 rank;         // (out)
    u8 shouldFreeHeap;
    u8 isSupreme; // harder NPC teams
} PokeathlonCourseArgsLayout;

// A player's result of the event that is running (exchanged with link commands 0x1D / 0x1E).
typedef struct PokeathlonCourseEventResult {
    u16 score;
    u16 unk2;
} PokeathlonCourseEventResult;

typedef void (*PokeathlonCourseFinishFunc)(PokeathlonCourseData *course);

// A link packet of the event that is running; the data copy area holds 7 of them.
typedef struct PokeathlonCoursePacket {
    u8 data[0x24];
    u8 isHostPacket;
    u8 unk25[3];
} PokeathlonCoursePacket;

// Callbacks of the event that is running (PokeathlonCourseData.state points to the event's table).
typedef struct PokeathlonCourseEvent {
    void *init;
    void (*draw)(PokeathlonCourseData *course, u32 param);
    void (*onResults)(PokeathlonCourseData *course, u32 param);
    void (*exit)(PokeathlonCourseData *course, u32 param);
    BOOL sendsOwnResult;
} PokeathlonCourseEvent;

// The course results (its first 4 PokeathlonFieldData are the counters).
typedef struct PokeathlonCourseResults {
    u32 counters[4][3][8]; // per participant and Pokemon, capped at 9999
    u32 bonuses;
    u32 instructions;
    u16 eventPoints[4][4]; // [participant][round]
    u16 eventScores[4][4]; // [participant][round]
    u8 wonRound[4];
    u8 lastInRound[4];
    u16 wonCourse : 1;
    u16 lastInCourse : 1;
    u16 rank : 2;
    u16 unkBits4 : 12;
    u16 totalPoints;
    PokeathlonStatistics record; // the player's saved record
} PokeathlonCourseResults;

typedef struct PokeathlonCourseLayout {
    u32 field_0[4];
    u8 padding_10[0x14C];
    u8 field_15C;
    u8 padding_15D[0x83];
    const PokeathlonCourseEvent *event;
    void *work;
    u8 eventIds[4]; // event of each round, as sent to the other players
    u8 unk1EC;
    u8 eventInitState;
    u8 participantCount;
    u8 receivedCount;
    u32 round;
    u32 eventDrawEnabled;
    PokeathlonCourseArgsLayout *args;
    u32 boxCancelled;
    PokeathlonCourseParticipant localParticipant;
    u32 boxCourseId;
    OverlayManager *subOverlay;
    enum HeapID heapId;
    PokeathlonCourseLink *link;
    PokeathlonCoursePacket packets[7]; // the data copy area
    u32 eventDataTransfer[4];
    PokeathlonCourseState courseState;
    PokeathlonStateInfo stateInfo;
    PokeathlonStateInfo eventStateInfo;
    u32 unk3D4;
    u32 roundEvents[4];
    u32 courseId;
    PokeathlonCourseParticipant participants[4];
    PlayerProfile *playerProfiles;
    PokeathlonCourseEventResult eventResults[4]; // the player's own, then (host) everybody's
    u16 receivedResults[18];                     // the results the host sent
    PokeathlonCourseScoreList *scores;
    PokeathlonCoursePerformanceTable performance;
    Sprite *countSprite; // event HUD: "3, 2, 1" and "Finish"
    u8 hudTimer;
    u8 finishState;
    u16 hudBaseY;
    Sprite *newRecordSprite;
    Sprite *timeUpSprite;
    u32 hudPlanes; // BG planes shown again after "Finish"
    Sprite *startSprites[3];
    u8 startSpriteCount;
    u8 startState;
    u8 rounds;
    u8 timeUpSeconds;
    PokeathlonCourseResults results;
    PokeathlonStatistics records[4]; // every player's saved record
    u32 localCounters[3][8];
    u8 monCounters[0x180];
    s32 frameCounter;
    u16 frameTimer;
    u16 recordScore; // the event result compared with the saved record
    u32 counterEnabled;
    PokeathlonCourseMarkers markers;
    void *juiceRecord;
    void *playerJuiceRecords;
    PokeathlonCourseFinishFunc finishCallback;
} PokeathlonCourseLayout;

// Saved records (parts of PokeathlonSave)

// A Pokemon of a course record.
typedef struct PokeathlonCourseSavedMon {
    u32 species : 9;
    u32 form : 5;
    u32 gender : 2;
    u32 isShiny : 1;
    u32 unused : 15;
    u32 personality;
    u32 unk08;
} PokeathlonCourseSavedMon;

// Best total of a course with its round points and Pokemon (one per course at the start of PokeathlonSave).
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

extern const PokeathlonCourseEvent gPokeathlonCourseEvent_Opening;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_Closing;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_ResultsAnnouncement;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_Wait;
extern const PokeathlonCourseEvent gPokeathlonCourseEvent_EventIntro;
extern const PokeathlonCourseEvent *const gPokeathlonCourseEvents[];        // events by event id
extern const PokeathlonCourseStepFunc *const gPokeathlonCourseEventSteps[]; // their state functions
extern PokeathlonCourseStepFunc gPokeathlonCourseOpeningSteps[];
extern PokeathlonCourseStepFunc gPokeathlonCourseClosingSteps[];
extern PokeathlonCourseStepFunc gPokeathlonCourseWaitSteps[];
extern PokeathlonCourseStepFunc gPokeathlonEventIntroSteps[];
extern const PokeathlonCourseStepFunc gPokeathlonResultsAnnouncementSteps[];
extern const u8 gPokeathlonEventScoreOrders[];     // score order of each event (0: higher is better, 1: lower)
extern const u16 gPokeathlonEventTargetScores[];   // target score of each event
extern const u8 gPokeathlonCourseEventIds[][4];    // events of the courses 0..9
extern const u8 gPokeathlonCourse10EventSets[][4]; // the 10 event sets of course 10
extern const u16 gPokeathlonNpcTeamSpriteIds[];    // per NPC team
extern const u16 gPokeathlonPerformanceFileIds[];  // performance file of each species' first form
extern const PokeathlonCourseCmd gPokeathlonCourseCmds[];

SpriteList *PokeathlonCourseSpritePool_GetSpriteList(void *eventGfx);
BOOL PokeathlonCourseObject_IsDrawn(PokeathlonCourseMonObject *object);

u32 ApricornJuiceRecord_sizeof(void);
ApricornRecord *ApricornRecord_New(SaveApricornBox *box, PlayerProfile *profile, u32 value, enum HeapID heapId);
void ApricornBox_StoreJuiceRecords(SaveApricornBox *box, ApricornRecord *records, int count, int skip);
BOOL Comm_SendCommandNoCopy(int command, void *data, int size);

// Course flow, results, link commands
void PokeathlonCourse_LoadPerformanceData(PokeathlonCourseData *course);
PokeathlonCoursePerformance *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int mon);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int mon);
int PokeathlonCourse_GetTouchRadius(void);
int PokeathlonCourse_GetSpriteSizeType(const PokeathlonCoursePerformance *performance);
int PokeathlonCourse_GetCollisionSizeClass(const PokeathlonCoursePerformance *performance);
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
u32 PokeathlonCourse_State_StartResults(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_WaitResultsStart(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_RunResults(PokeathlonCourseData *course);
u32 PokeathlonCourse_State_EndResults(PokeathlonCourseData *course);
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
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u32 participant, u32 mon, int counter, int amount);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u16 score);
void PokeathlonCourse_SetFinishCallback(PokeathlonCourseData *course, PokeathlonCourseFinishFunc callback);
u16 PokeathlonCourse_GetNpcTeamSpriteId(u32 npcTeam);
u8 PokeathlonCourse_GetNpcTeamMsgId(PokeathlonCourseData *course, u32 npcTeam);
void PokeathlonCourse_SetupNpcParticipants(PokeathlonCourseData *course, int count);
void *PokeathlonCourse_GetJuiceRecord(PokeathlonCourseData *course, u32 index);
BOOL PokeathlonCourse_IsInRange(int x, int y, int targetX, int targetY, int range, int *distance);
void PokeathlonCourseLink_SetCheckSize(PokeathlonCourseLink *link, u16 checkSize);
PokeathlonCourseMonGfxManager *PokeathlonCourseMonGfx_AllocManager(enum HeapID heapId);

// Pokemon sprites
void PokeathlonCourseMonGfx_InitManager(PokeathlonCourseMonGfxManager *manager, u32 count, SpriteList *spriteList);
void PokeathlonCourseMonGfx_FreeManager(PokeathlonCourseMonGfxManager *manager);
void PokeathlonCourseMonGfx_StartLoad(PokeathlonCourseMonGfxManager *manager, s32 count, PokeathlonCourseMonInfo *infos, PokeathlonCourseMonGfxTemplate *template, u32 priority, int baseIndex);
BOOL PokeathlonCourseMonGfx_IsLoaded(PokeathlonCourseMonGfxManager *manager);
PokeathlonCourseMonGfx *PokeathlonCourseMonGfx_Get(PokeathlonCourseMonGfxManager *manager, int index);
Sprite *PokeathlonCourseMonGfx_GetSprite(PokeathlonCourseMonGfx *mon);
PokeathlonCourseMonInfo *PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonGfx *mon);
void PokeathlonCourseMonGfx_LoadPalette(PokeathlonCourseMonInfo *info, enum HeapID heapId, void *dest);
void PokeathlonCourseMonGfx_SetPalette(PokeathlonCourseMonGfxManager *manager, int index, int vram, void *palette);
u8 PokeathlonCourseMonGfx_GetTopRow(PokeathlonCourseMonGfx *mon);
void PokeathlonCourseMonGfx_SetScale(PokeathlonCourseMonGfx *mon, f32 x, f32 y);
void PokeathlonCourseMonGfx_ShiftLargeUp(PokeathlonCourseMonGfxManager *manager);
BOOL PokeathlonCourseMonGfx_IsShiftedUp(PokeathlonCourseMonGfx *mon);
void PokeathlonCourse_InitObjTransfer(ObjCharTransferTemplate *charTemplate, int plttCount, enum HeapID heapId, GXOBJVRamModeChar modeMain, GXOBJVRamModeChar modeSub);
void PokeathlonCourse_FreeObjTransfer(void);

// Score list and link command handlers
PokeathlonCourseScore *PokeathlonCourseScoreList_GetEntry(PokeathlonCourseScoreList *list, u32 index);
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

// Sprite resources and course Pokemon objects
PokeathlonCourseSpriteResources *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, u32 flags, int charVram);
void PokeathlonCourseSprites_FreeResources(PokeathlonCourseSpriteResources *resources);
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
