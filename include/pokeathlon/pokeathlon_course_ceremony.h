#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_CEREMONY_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_CEREMONY_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_course_ui.h"

#include "bg_window.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// The opening and closing ceremonies of a Pokeathlon course. The host decides the introductions and the awards and
// sends them with the course data; every player shows them in step with the others.

// One team's introduction line, chosen by the host.
typedef struct PokeathlonCourseCeremonyIntro {
    u32 msgId;      // msg 311 # (NPC team) or msg 309 # (player)
    u32 value : 24; // the number in the line
    u32 cheer : 8;  // 0..12: pitch of the crowd's cheer (sPokeathlonCourseCeremonyCheerPitches)
} PokeathlonCourseCeremonyIntro;

// What the host sends for the opening.
typedef struct PokeathlonCourseCeremonyOpeningData {
    PokeathlonCourseCeremonyIntro intros[4];
    u8 messageCount; // how far the host's text is
    u8 waiting : 1;  // a player is behind the host
    u8 allFinished : 1;
    u8 unk21_2 : 6;
} PokeathlonCourseCeremonyOpeningData;

// What the host sends for the closing.
typedef struct PokeathlonCourseCeremonyClosingData {
    u8 challengeBonus[4];      // per team
    u32 effortPrizeKind : 3;   // statistic of the prize for effort (4: none)
    u32 effortPrizeMons : 12;  // bit per Pokemon (team * 3 + member)
    u32 noMissMons : 12;       // Pokemon that did not miss
    u32 unk04_27 : 5;
    u32 maxEffort : 16;
    u32 pointsLeaderMons : 12; // Pokemon that collected the most points
    u32 waiting : 1;           // a player is behind the host
    u32 allFinished : 1;
    u32 unk08_30 : 2;
    u8 messageCount;   // how far the host's text is
    u8 drawingLots[4]; // tie-break order of the teams
} PokeathlonCourseCeremonyClosingData;

// What every player sends back.
typedef struct PokeathlonCourseCeremonySync {
    u8 messageCount;
    u8 printing;
    u8 finished;
} PokeathlonCourseCeremonySync;

// A Pokemon of a participant's team (PokeathlonCourse_GetParticipantMons).
typedef struct PokeathlonCourseCeremonyMon {
    u16 species;
    u16 form;
    u8 unk04[0xE];
    u16 nickname[11];
} PokeathlonCourseCeremonyMon;

// Statistics of a Pokemon in the events of the course (PokeathlonCourseData field data).
typedef struct PokeathlonCourseMonEventStats {
    u32 failed;
    u32 jumped;
    u32 points;
    u32 hit;
    u32 fellDown;
    u32 dashed;
    u32 switched;
    u32 selfImpeded;
} PokeathlonCourseMonEventStats;

// Result of the ceremony kept in the field data for the course statistics.
typedef struct PokeathlonCourseCeremonyResult {
    u8 unk000[0x180];
    u32 awards; // bonus awards of the player's team
    u8 unk184[0x4C];
    u16 won : 1;
    u16 fewestPoints : 1;
    u16 place : 2;
    u16 unk1D0_4 : 12;
    u16 points;
} PokeathlonCourseCeremonyResult;

// Team points of the closing ceremony: counts up to the points, three digit sprites.
typedef struct PokeathlonCourseCeremonyScore {
    ManagedSprite *sprites[4]; // plate, hundreds, tens, ones
    u32 points;
    u32 shownPoints;
    BOOL counting;
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
    enum HeapID heapId;
    PokeathlonCourseConfettiPiece pieces[15];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u32 timer;
    u32 delay;
    BOOL active;
} PokeathlonCourseConfetti;

typedef struct PokeathlonCourseCeremonyCryTask {
    u16 state;
    s32 timer;
    void *object;
    BOOL *active;
} PokeathlonCourseCeremonyCryTask;

typedef struct PokeathlonCourseCeremony {
    enum HeapID heapId;
    BgConfig *bgConfig;
    SysTask *cryTask;
    PokeathlonCourseMessageBox *messageBox;
    PokeathlonCourseSpriteResources *spriteResources;
    PokeathlonCourseMonObjects *objects; // the 12 Pokemon
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;

    union {
        ManagedSprite *sprites[26];

        struct {
            ManagedSprite *trainerSprites[5]; // four teams and the host of the ceremony
            ManagedSprite *scoreSprites[4][4];
            ManagedSprite *monPicSprites[3]; // the winning team
            ManagedSprite *medalSprites[2];
        };
    };

    PokeathlonCourseCeremonyCryTask *cryData;
    PokeathlonCourseCeremonyScore *scores; // four teams
    PokeathlonCourseConfetti *confetti;
    int timer;
    BOOL cryActive;
    PokeathlonCourseCeremonyRank ranks[4];
    u8 awardCounts[4];
    u8 cheering;
    u8 course;
    u8 team;
    u8 member;
    u8 counter;
    u8 state;
    u8 unkB6;
    u8 messageCount;
    BOOL finished;
} PokeathlonCourseCeremony;

typedef u32 (*PokeathlonCourseCeremonyAwardTest)(PokeathlonCourseData *course, int team, int member);

BOOL PokeathlonCourseOpening_Init(PokeathlonCourseData *course);
BOOL PokeathlonCourseOpening_Main(PokeathlonCourseData *course);
BOOL PokeathlonCourseOpening_Exit(PokeathlonCourseData *course);
BOOL PokeathlonCourseClosing_Init(PokeathlonCourseData *course);
BOOL PokeathlonCourseClosing_Main(PokeathlonCourseData *course);
BOOL PokeathlonCourseClosing_Exit(PokeathlonCourseData *course);
BOOL PokeathlonCourseOpening_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseOpening_Introduce(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseOpening_FadeOut(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseClosing_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseClosing_Awards(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCourseClosing_FadeOut(PokeathlonCourseData *course, u8 *state);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_CEREMONY_H
