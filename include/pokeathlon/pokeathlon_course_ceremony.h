#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_CEREMONY_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_CEREMONY_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_ui.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// The opening and closing ceremonies of a Pokeathlon course (msg 309 #0-31). The opening
// introduces the teams (course name, then every team with its three Pokemon and a line about
// its record); the closing gives the bonus points (Challenge Bonus, No-Miss Bonus, points
// leader, prize for effort), adds up the event points and names the winner, its medals and
// records. The host decides the introductions and the awards and sends them with the course
// data; every player shows them in step with the others.

// One team's introduction line, chosen by the host (8 bytes).
typedef struct PokeathlonCourseCeremonyIntro {
    u32 msgId;      // msg 311 # (NPC team) or msg 309 # (player)
    u32 value : 24; // the number in the line
    u32 cheer : 8;  // 0..12: pitch of the crowd's cheer (ov96_0221B088)
} PokeathlonCourseCeremonyIntro;

// What the host sends for the opening (at course data copy area + 0x28).
typedef struct PokeathlonCourseCeremonyOpeningData {
    PokeathlonCourseCeremonyIntro intros[4]; // 0x00
    u8 messageCount;                         // 0x20: how far the host's text is
    u8 waiting : 1;                          // 0x21: a player is behind the host
    u8 allFinished : 1;
    u8 unk21_2 : 6;
} PokeathlonCourseCeremonyOpeningData;

// What the host sends for the closing (at course data copy area + 0x28).
typedef struct PokeathlonCourseCeremonyClosingData {
    u8 challengeBonus[4];      // 0x00: per team
    u32 effortPrizeKind : 3;   // 0x04: statistic of the prize for effort (4: none)
    u32 effortPrizeMons : 12;  // bit per Pokemon (team * 3 + member)
    u32 noMissMons : 12;       // Pokemon that did not miss
    u32 unk04_27 : 5;          //
    u32 maxEffort : 16;        // 0x08
    u32 pointsLeaderMons : 12; // Pokemon that collected the most points
    u32 waiting : 1;           // a player is behind the host
    u32 allFinished : 1;
    u32 unk08_30 : 2;
    u8 messageCount;   // 0x0C: how far the host's text is
    u8 drawingLots[4]; // 0x0D: tie-break order of the teams
} PokeathlonCourseCeremonyClosingData;

// What every player sends back (at course data copy area + 0x00, the host keeps them at
// + 0x50 + 0x28 * participant).
typedef struct PokeathlonCourseCeremonySync {
    u8 messageCount;
    u8 printing;
    u8 finished;
} PokeathlonCourseCeremonySync;

// A Pokemon of a participant's team (0x28, PokeathlonCourse_GetParticipantUnk04).
typedef struct PokeathlonCourseCeremonyMon {
    u16 species;      // 0x00
    u16 form;         // 0x02
    u8 unk04[0xE];    // 0x04
    u16 nickname[11]; // 0x12
} PokeathlonCourseCeremonyMon;

// Statistics of a Pokemon in the events of the course (PokeathlonCourseData field data, 0x20).
typedef struct PokeathlonCourseMonEventStats {
    u32 failed; // 0x00
    u32 jumped; // 0x04
    u32 points; // 0x08
    u32 hit;    // 0x0C
    u32 fellDown;  // 0x10
    u32 dashed; // 0x14
    u32 switched;  // 0x18
    u32 selfImpeded;  // 0x1C
} PokeathlonCourseMonEventStats;

// Result of the ceremony kept in the field data for the course statistics.
typedef struct PokeathlonCourseCeremonyResult {
    u8 unk000[0x180];
    u32 awards; // 0x180: bonus awards of the player's team
    u8 unk184[0x4C];
    u16 won : 1;          // 0x1D0
    u16 fewestPoints : 1; //
    u16 place : 2;        //
    u16 unk1D0_4 : 12;    //
    u16 points;           // 0x1D2
} PokeathlonCourseCeremonyResult;

// Team points of the closing ceremony: counts up to the points, three digit sprites.
typedef struct PokeathlonCourseCeremonyScore {
    ManagedSprite *sprites[4]; // 0x00: plate, hundreds, tens, ones
    u32 points;                // 0x10
    u32 shownPoints;           // 0x14
    BOOL counting;             // 0x18
} PokeathlonCourseCeremonyScore;

// Place of a team, after sorting by points.
typedef struct PokeathlonCourseCeremonyRank {
    u32 points : 16;
    u32 place : 8; // 0: first
    u32 team : 4;
    u32 drawingLot : 4;
} PokeathlonCourseCeremonyRank;

// Confetti of the winner (closing ceremony).
typedef union PokeathlonCourseConfettiFlags {
    u32 raw;

    struct {
        u32 angle : 16;
        u32 speed : 4;
        u32 active : 1;
        u32 landed : 1;
        u32 unused : 10;
    };
} PokeathlonCourseConfettiFlags;

typedef struct PokeathlonCourseConfettiPiece {
    ManagedSprite *sprite;
    fx32 xVelocity;
    fx32 yVelocity;
    fx32 x;
    fx32 y;
    PokeathlonCourseConfettiFlags state;
} PokeathlonCourseConfettiPiece;

typedef struct PokeathlonCourseConfetti {
    enum HeapID heapId;                       // 0x000
    PokeathlonCourseConfettiPiece pieces[15]; // 0x004
    SpriteSystem *spriteSystem;               // 0x16C
    SpriteManager *spriteManager;             // 0x170
    u32 timer;                                // 0x174
    u32 delay;                                // 0x178
    BOOL active;                              // 0x17C
} PokeathlonCourseConfetti;

typedef struct PokeathlonCourseCeremonyCryTask {
    u16 state;
    s32 timer;
    void *object;
    BOOL *active;
} PokeathlonCourseCeremonyCryTask;

typedef struct PokeathlonCourseCeremony {
    enum HeapID heapId;                     // 0x00
    BgConfig *bgConfig;                     // 0x04
    SysTask *cryTask;                       // 0x08
    PokeathlonCourseMessageBox *messageBox; // 0x0C
    void *spriteResources;                  // 0x10
    void *objects;                          // 0x14: the 12 Pokemon
    SpriteSystem *spriteSystem;             // 0x18
    SpriteManager *spriteManager;           // 0x1C

    union {
        ManagedSprite *sprites[26]; // 0x20

        struct {
            ManagedSprite *trainerSprites[5];  // 0x20: four teams and the host of the ceremony
            ManagedSprite *scoreSprites[4][4]; // 0x34
            ManagedSprite *monPicSprites[3];   // 0x74: the winning team
            ManagedSprite *medalSprites[2];    // 0x80
        };
    };

    PokeathlonCourseCeremonyCryTask *cryData; // 0x88
    PokeathlonCourseCeremonyScore *scores;    // 0x8C: four teams
    PokeathlonCourseConfetti *confetti;       // 0x90
    int timer;                                // 0x94
    BOOL cryActive;                           // 0x98
    PokeathlonCourseCeremonyRank ranks[4];    // 0x9C
    u8 awardCounts[4];                        // 0xAC
    u8 cheering;                              // 0xB0
    u8 course;                                // 0xB1
    u8 team;                                  // 0xB2
    u8 member;                                // 0xB3
    u8 counter;                               // 0xB4
    u8 state;                                 // 0xB5
    u8 unkB6;                                 // 0xB6
    u8 messageCount;                          // 0xB7
    BOOL finished;                            // 0xB8
} PokeathlonCourseCeremony;                   // size: 0xBC

typedef u32 (*PokeathlonCourseCeremonyAwardTest)(PokeathlonCourseData *course, int team, int member);

BOOL PokeathlonCourseOpening_Init(PokeathlonCourseData *course);
BOOL PokeathlonCourseOpening_Main(PokeathlonCourseData *course);
BOOL PokeathlonCourseOpening_Exit(PokeathlonCourseData *course);
BOOL PokeathlonCourseClosing_Init(PokeathlonCourseData *course);
BOOL PokeathlonCourseClosing_Main(PokeathlonCourseData *course);
BOOL PokeathlonCourseClosing_Exit(PokeathlonCourseData *course);
void PokeathlonCourseCeremony_SetBanks(void);
BOOL PokeathlonCourseOpening_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseOpening_Introduce(PokeathlonCourseData *course);
BOOL PokeathlonCourseOpening_FadeOut(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseClosing_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseClosing_Awards(PokeathlonCourseData *course);
BOOL PokeathlonCourseClosing_FadeOut(PokeathlonCourseData *course, u8 *state);
void PokeathlonCourseCeremony_CryTask(SysTask *task, void *data);
void PokeathlonCourseCeremony_StartCry(PokeathlonCourseCeremony *ceremony, u8 index);
BOOL PokeathlonCourseCeremony_IsCryActive(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_InitBgs(BgConfig *bgConfig);
void PokeathlonCourseCeremony_LoadBgGfx(BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonCourseCeremony_LoadWinnerBg(BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonCourseCeremony_InitSpriteSystem(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_FreeSprites(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_CreateTrainerSprites(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_CreateScoreSprites(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_CreateMonPicSprites(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_CreateMedalSprites(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_LoadWinnerMonPics(PokeathlonCourseData *course, int team);
void PokeathlonCourseCeremony_LoadSpriteResources(PokeathlonCourseData *course);
void PokeathlonCourseCeremony_LoadMonPicResources(PokeathlonCourseCeremony *ceremony);
void PokeathlonCourseCeremony_ShowWinnerTeam(PokeathlonCourseData *course, int team);
void PokeathlonCourseCeremony_LoadTrainerSprites(PokeathlonCourseData *course);
void PokeathlonCourseCeremony_SetTrainerPose(ManagedSprite **sprites, PokeathlonCourseData *course, int team, int pose, enum HeapID heapId);
PokeathlonCourseCeremonyScore *PokeathlonCourseCeremony_CreateScores(enum HeapID heapId, ManagedSprite **sprites);
void PokeathlonCourseCeremony_FreeScores(PokeathlonCourseCeremonyScore *scores);
void PokeathlonCourseCeremony_UpdateScores(PokeathlonCourseCeremonyScore *scores);
BOOL PokeathlonCourseCeremony_AreScoresCounted(PokeathlonCourseCeremonyScore *scores);
void PokeathlonCourseCeremony_AddPoints(PokeathlonCourseCeremonyScore *scores, int team, u32 points);
u32 PokeathlonCourseCeremony_GetPoints(PokeathlonCourseCeremonyScore *scores, u8 team);
int PokeathlonCourseCeremony_CompareRanks(const void *a, const void *b);
void PokeathlonCourseCeremony_RankTeams(PokeathlonCourseCeremonyScore *scores, PokeathlonCourseCeremonyRank *ranks, const u8 *drawingLots);
void PokeathlonCourseCeremonyScore_Update(PokeathlonCourseCeremonyScore *score);
BOOL PokeathlonCourseCeremonyScore_IsCounted(PokeathlonCourseCeremonyScore *score);
void PokeathlonCourseCeremonyScore_Add(PokeathlonCourseCeremonyScore *score, u32 points);
void PokeathlonCourseConfetti_Spawn(PokeathlonCourseConfettiPiece *piece, SpriteSystem *spriteSystem, SpriteManager *spriteManager);
BOOL PokeathlonCourseConfetti_Fall(PokeathlonCourseConfettiPiece *piece);
void PokeathlonCourseConfetti_MoveToTopScreen(PokeathlonCourseConfettiPiece *piece, SpriteSystem *spriteSystem, SpriteManager *spriteManager);
PokeathlonCourseConfetti *PokeathlonCourseConfetti_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager);
void PokeathlonCourseConfetti_Free(PokeathlonCourseConfetti *confetti);
void PokeathlonCourseConfetti_Update(PokeathlonCourseConfetti *confetti);
void PokeathlonCourseConfetti_Start(PokeathlonCourseConfetti *confetti, BOOL active);
void PokeathlonCourseConfetti_SpawnPieces(PokeathlonCourseConfetti *confetti, u8 count);
BOOL PokeathlonCourseCeremony_Init(PokeathlonCourseData *course);
BOOL PokeathlonCourseCeremony_Main(PokeathlonCourseData *course);
BOOL PokeathlonCourseCeremony_Exit(PokeathlonCourseData *course);
void PokeathlonCourseCeremony_BufferMonName(PokeathlonCourseData *course, int team, int member, int field);
void PokeathlonCourseCeremony_BufferTeamPoints(PokeathlonCourseData *course, int team, int points);
void PokeathlonCourseCeremony_BufferTeamMon(PokeathlonCourseData *course, int team, int member);
u32 PokeathlonCourseCeremony_HasNoMissBonus(PokeathlonCourseData *course);
u32 PokeathlonCourseCeremony_IsNoMissMon(PokeathlonCourseData *course, int team, int member);
u32 PokeathlonCourseCeremony_IsPointsLeader(PokeathlonCourseData *course, int team, int member);
u32 PokeathlonCourseCeremony_IsEffortPrizeMon(PokeathlonCourseData *course, int team, int member);
int PokeathlonCourseConfetti_GetRandomDelay(void);
u8 PokeathlonCourseCeremony_GetChallengeBonus(PokeathlonCourseData *course, u8 team);
u32 PokeathlonCourseCeremony_GetEventPoints(PokeathlonCourseData *course, u8 team, u8 round);
int PokeathlonCourseCeremony_GetNoMissPoints(PokeathlonCourseCeremony *ceremony);
int PokeathlonCourseCeremony_GetPointsLeaderPoints(PokeathlonCourseCeremony *ceremony);
int PokeathlonCourseCeremony_GetEffortPrizePoints(PokeathlonCourseCeremony *ceremony);
u32 PokeathlonCourseCeremony_CountNoMissMons(PokeathlonCourseData *course);
u32 PokeathlonCourseCeremony_CountPointsLeaders(PokeathlonCourseData *course);
u32 PokeathlonCourseCeremony_CountEffortPrizes(PokeathlonCourseData *course);
void PokeathlonCourseCeremony_GetDrawingLots(PokeathlonCourseData *course, u8 *drawingLots);
u32 PokeathlonCourseCeremony_GetIntroMsgId(PokeathlonCourseData *course, u32 team);
u32 PokeathlonCourseCeremony_GetIntroValue(PokeathlonCourseData *course, u32 team);
u32 PokeathlonCourseCeremony_GetIntroCheer(PokeathlonCourseData *course, u32 team);
void PokeathlonCourseCeremony_ChooseIntros(PokeathlonCourseData *course);
u32 PokeathlonCourseCeremony_CalcChallengeBonus(PokeathlonCourseData *course, u8 team);
u32 PokeathlonCourseCeremony_ChooseEffortPrizeKind(PokeathlonCourseData *course);
void PokeathlonCourseCeremony_ChooseAwards(PokeathlonCourseData *course);
u32 PokeathlonCourseCeremony_GetCourseMedal(u32 course, u32 which);
u8 PokeathlonCourseCeremony_GetEffortPrizeKind(PokeathlonCourseData *course);
u32 PokeathlonCourseCeremony_GetMonStat(PokeathlonCourseData *course, u8 kind, u8 team, u8 member);
BOOL PokeathlonCourseCeremony_NextMon(PokeathlonCourseCeremony *ceremony);
BOOL PokeathlonCourseCeremony_AwardNextMon(PokeathlonCourseData *course, PokeathlonCourseCeremonyAwardTest test, int points);
void PokeathlonCourseCeremony_SaveResult(PokeathlonCourseData *course, PokeathlonCourseCeremonyScore *scores);
void PokeathlonCourseCeremony_BufferNumber(MessageFormat *msgFormat, s32 value, u32 digits, u32 field);
u32 PokeathlonCourseCeremony_GetCheerLevel(s32 value);
int PokeathlonCourseCeremony_GetIntroMsgBase(int statistic);
void PokeathlonCourseCeremony_GetIntroStats(PokeathlonStatistics *dest, const PokeathlonStatistics *src);
void PokeathlonCourseCeremony_WeighIntroStats(u8 *weights, const u32 *stats);
int PokeathlonCourseCeremony_PickIntroStat(const u8 *weights, u8 *level);
void PokeathlonCourseCeremony_ChooseOwnIntro(PokeathlonCourseCeremonyIntro *intro, PokeathlonCourseData *course);
void PokeathlonCourseCeremony_ChooseLinkIntro(PokeathlonCourseCeremonyIntro *intro, PokeathlonCourseData *course, int team);
void PokeathlonCourseCeremony_ChooseNpcIntro(PokeathlonCourseCeremonyIntro *intro, PokeathlonCourseData *course, int team);
int PokeathlonCourseCeremony_FindPlace(const PokeathlonCourseCeremonyRank *ranks, u8 team);
void PokeathlonCourseCeremony_BreakTie(PokeathlonCourseCeremonyRank *ranks);
BOOL PokeathlonCourseCeremony_SyncOpening(PokeathlonCourseData *course);
BOOL PokeathlonCourseCeremony_SyncClosing(PokeathlonCourseData *course);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_CEREMONY_H
