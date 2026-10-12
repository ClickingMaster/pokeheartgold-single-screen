#include "global.h"

#include "pokeathlon/pokeathlon_disc_catch_play.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "math_util.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "touchscreen.h"

// What the course reports about a Pokemon for its sprite (the layout of PokeathlonCourseObjectInfo).
typedef struct PokeathlonDiscCatchMonInfo {
    u16 species;     // 0x00
    u16 form;        // 0x02
    u16 isLarge;     // 0x04
    u8 shiny;        // 0x06
    u8 gender;       // 0x07
    int participant; // 0x08
    u32 personality; // 0x0C
} PokeathlonDiscCatchMonInfo;

// Course and course object helpers (OVY_96), declared as the calls were built.
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int member);
void *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int member);
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, PokeathlonDiscCatchMonInfo *info);
int ov96_021E6108(void *performance);
int ov96_021E6138(void *performance);
u32 ov96_021E6104(void);
void PokeathlonCourse_StopMarkerAnims(PokeathlonCourseData *course);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, int member, int counter, int amount);
Sprite *PokeathlonCourse_AddMarker(PokeathlonCourseData *course, void *monPic, void *resources, SpriteList *spriteList, int a4);
void PokeathlonCourse_CreateStartSprites(PokeathlonCourseData *course, int a1, void *resources, SpriteList *spriteList, int a4, int a5, s16 (*positions)[2]);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, int timer);
void *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int count, int a2, void *resources, SpriteList *spriteList);
void PokeathlonCourseObjects_ShiftLargeUp(void *monPics);
void PokeathlonCourseMonObjects_Load(void *monPics, int count, PokeathlonDiscCatchMonInfo *infos, u32 *params, int a4, int a5);
void *PokeathlonCourseMonObjects_Get(void *monPics, u8 index);
void *PokeathlonCourseMonObject_GetGfx(void *monPic);
PokeathlonDiscCatchMonInfo *PokeathlonCourseMonGfx_GetInfo(void *gfx);
void PokeathlonCourseMonObject_SetDrawFlag(void *monPic, int draw);
void PokeathlonCourseMonObject_SetDrawPriority(void *monPic, int priority);
void PokeathlonCourseMonObject_SetAnimState(void *monPic);
void PokeathlonCourseObject_SetCollision(void *monPic, int offsetY, int radius);
void PokeathlonCourseObject_SetScale(void *monPic, f32 scaleX, f32 scaleY);
void PokeathlonCourseObject_SetGroundPosition(void *monPic, int x, int y);
void PokeathlonCourseObject_SetTouchRadius(void *monPic, int radius);
void PokeathlonCourseObject_GroundToCenter(void *monPic, int x, int y, int *outX, int *outY);
void PokeathlonCourseObject_CenterToGround(void *monPic, int x, int y, int *outX, int *outY);
int PokeathlonCourseObject_GetCollisionRadius(void *monPic);

extern const u32 ov96_0221CF1C[4];
extern const s16 ov96_0221CFBC[DISC_CATCH_PARTICIPANTS][DISC_CATCH_TEAM_SIZE][2];
extern const WindowTemplate ov96_0221CEEC;

// Each frame (every participant): sends the own touch and shows the host's frame packet; returns TRUE
// once the host reports the end.
BOOL PokeathlonDiscCatch_UpdateFrame(PokeathlonCourseData *course) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *area = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonDiscCatchFrame *received = (PokeathlonDiscCatchFrame *)ov96_021E8A20(area + 0xF0);
    PokeathlonDiscCatchFrame *own = (PokeathlonDiscCatchFrame *)ov96_021E8A20(area);
    if (received->finished == TRUE) {
        PokeathlonDiscCatch_StopAnims(work);
        PokeathlonCourse_StopMarkerAnims(course);
        PokeathlonDiscCatch_EndGame(course);
        return TRUE;
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)ov96_021E5F24(course), 3, 0, 1);
    }
    if (System_GetTouchHeld()) {
        own->touchX = gSystem.touchX;
        own->touchY = gSystem.touchY;
        own->touching = TRUE;
    } else {
        own->touching = FALSE;
    }
    PokeathlonDiscCatchSprites_Update(&work->sprites, course);
    return FALSE;
}

// The Pokemon's abilities from their ratings: jump (frames, height) and the power and speed tables
// (NARC a/1/7/0 file 7, in tenths of a pixel).
void PokeathlonDiscCatch_InitStats(PokeathlonCourseData *course) {
    u32 participantArg;
    int member;
    int participant;
    s32 table[20];
    ReadWholeNarcMemberByIdPair(table, NARC_a_1_7_0, 7);
    for (participant = 0; participant < DISC_CATCH_PARTICIPANTS; participant++) {
        participantArg = (u8)participant;
        for (member = 0; member < DISC_CATCH_TEAM_SIZE; member++) {
            u8 *ratings = PokeathlonCourse_GetMonStats(course, participant, member);
            PokeathlonDiscCatchStats *stats = PokeathlonDiscCatch_GetStats(course, participantArg, (u8)member);
            s32 value;
            float rounded;
            GF_ASSERT(ratings[2] < 5);
            GF_ASSERT(ratings[0] < 5);
            GF_ASSERT(ratings[1] < 5);
            stats->jump = ratings[2];
            stats->jumpFrames = table[ratings[2]];
            stats->jumpHeight = table[5 + ratings[2]];
            value = table[10 + ratings[0]];
            if (value > 0) {
                rounded = 0.5f + (float)(value << FX32_SHIFT);
            } else {
                rounded = (float)(value << FX32_SHIFT) - 0.5f;
            }
            stats->power = (int)rounded / 10;
            value = table[15 + ratings[1]];
            if (value > 0) {
                rounded = 0.5f + (float)(value << FX32_SHIFT);
            } else {
                rounded = (float)(value << FX32_SHIFT) - 0.5f;
            }
            stats->speed = (int)rounded / 10;
        }
    }
}

PokeathlonDiscCatchStats *PokeathlonDiscCatch_GetStats(PokeathlonCourseData *course, u32 participant, u32 member) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonDiscCatchStats *stats;
    GF_ASSERT(work != NULL);
    GF_ASSERT(participant < DISC_CATCH_PARTICIPANTS);
    GF_ASSERT(member < DISC_CATCH_TEAM_SIZE);
    stats = work->stats[0] + (participant * DISC_CATCH_TEAM_SIZE + member);
    GF_ASSERT(stats != NULL);
    return stats;
}

u16 PokeathlonDiscCatchStats_GetJumpDuration(PokeathlonDiscCatchStats *stats) {
    return stats->jumpFrames * 2;
}

u32 PokeathlonDiscCatchDisc_GetPoints(const PokeathlonDiscCatchDisc *disc) {
    switch (disc->state) {
    case DISC_CATCH_DISC_CAUGHT_1:
        return 1;
    case DISC_CATCH_DISC_CAUGHT_2:
        return 2;
    case DISC_CATCH_DISC_CAUGHT_3:
        return 3;
    case DISC_CATCH_DISC_CAUGHT_5:
        return 5;
    default:
        GF_ASSERT(FALSE);
        return 0;
    }
}

// The twelve Pokemon sprites of the touch screen.
void PokeathlonDiscCatch_CreateMonPics(PokeathlonDiscCatch *work) {
    SpriteList *spriteList;
    PokeathlonCourseData *course;
    spriteList = SpriteManager_GetSpriteList(work->spriteManager);
    work->monPics = PokeathlonCourseMonObjects_Create(work->heapId, DISC_CATCH_MONS, 7, work->monPicResources, spriteList);
    PokeathlonCourseObjects_ShiftLargeUp(work->monPics);
    {
        PokeathlonDiscCatchMonInfo infos[DISC_CATCH_MONS];
        u32 params[17] = { 0 };
        int i;
        course = work->course;
        for (i = 0; i < DISC_CATCH_MONS; i++) {
            int member = i % DISC_CATCH_TEAM_SIZE;
            int participant = i / DISC_CATCH_TEAM_SIZE;
            PokeathlonCourse_GetMonInfo(course, participant, member, &infos[i]);
            params[i + 5] = ov96_021E6108(PokeathlonCourse_GetPerformance(course, participant, member));
        }
        params[1] = 1;
        params[3] = 1;
        params[4] = 1;
        PokeathlonCourseMonObjects_Load(work->monPics, DISC_CATCH_MONS, infos, params, 0, 0);
    }
}

static inline int GetSizeClass(PokeathlonCourseData *course, int index) {
    return ov96_021E6138(PokeathlonCourse_GetPerformance(course, index / DISC_CATCH_TEAM_SIZE, index % DISC_CATCH_TEAM_SIZE));
}

// Puts the Pokemon on their start places (the host also writes them into the first frame packet).
#pragma force_active on

void PokeathlonDiscCatch_InitMons(PokeathlonDiscCatch *work) {
    int collisions[3][2] = { 0 };
    s16 ownPositions[DISC_CATCH_TEAM_SIZE][2];
    int x, y;
    PokeathlonCourseData *course;
    s32 i;
    int participant, member;
    void *monPic;
    void *pic;
    PokeathlonDiscCatchMonInfo *info;
    PokeathlonDiscCatchMon *mon;
    s16 groundX, groundY;
    PokeathlonDiscCatchFrame *frame;

    ReadWholeNarcMemberByIdPair(collisions, NARC_a_1_7_0, 17);
    GF_ASSERT(work != NULL);
    GF_ASSERT(work->course != NULL);
    course = work->course;
    PokeathlonDiscCatch_InitStats(course);
    for (i = 0; i < DISC_CATCH_PARTICIPANTS; i++) {
        work->field.teams[i].result.participant = i;
    }
    for (i = 0; i < DISC_CATCH_MONS; i++) {
        member = i % DISC_CATCH_TEAM_SIZE;
        participant = i / DISC_CATCH_TEAM_SIZE;
        mon = &work->field.teams[participant].mons[member];
        monPic = PokeathlonCourseMonObjects_Get(work->monPics, i);
        PokeathlonCourseMonObject_SetDrawFlag(monPic, TRUE);
        {
            int sizeClass = GetSizeClass(course, i);
            PokeathlonCourseObject_SetCollision(monPic, collisions[sizeClass - 1][0], collisions[sizeClass - 1][1]);
        }
        pic = PokeathlonCourseMonObjects_Get(work->monPics, i);
        info = PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(pic));
        PokeathlonDiscCatch_SetMonScaleCenter(pic, FALSE);
        PokeathlonCourseObject_SetScale(pic, 1.0f, 1.0f);
        groundX = ov96_0221CFBC[participant][member][0];
        groundY = ov96_0221CFBC[participant][member][1];
        PokeathlonCourseMonObject_SetAnimState(pic);
        PokeathlonCourseObject_SetGroundPosition(pic, groundX, groundY);
        PokeathlonCourseObject_SetTouchRadius(pic, ov96_021E6104());
        PokeathlonCourseObject_GroundToCenter(pic, groundX, groundY, &x, &y);
        mon->course = course;
        mon->index = i;
        mon->participant = participant;
        mon->member = member;
        mon->monPic = pic;
        mon->large = info->isLarge;
        mon->stats = PokeathlonDiscCatch_GetStats(course, (u8)participant, (u8)member);
        mon->state = DISC_CATCH_MON_ON_FIELD;
        mon->facing = 2;
        mon->position.x = x * FX32_ONE;
        mon->position.y = y * FX32_ONE;
        mon->target.x = x * FX32_ONE;
        mon->target.y = y * FX32_ONE;
        if (participant == ov96_021E5F24(course)) {
            ownPositions[member][0] = groundX;
            ownPositions[member][1] = groundY;
            Sprite_SetDrawPriority(PokeathlonCourse_AddMarker(course, pic, work->monPicResources, SpriteManager_GetSpriteList(work->spriteManager), 1), 4);
            PokeathlonCourseMonObject_SetDrawPriority(pic, 6);
        } else {
            PokeathlonCourseMonObject_SetDrawPriority(pic, 7);
        }
        if (ov96_021E5F24(course) == 0) {
            frame = (PokeathlonDiscCatchFrame *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
            frame->monX[i] = x;
            frame->monY[i] = y;
        }
    }
    PokeathlonCourse_CreateStartSprites(course, 0, work->monPicResources, SpriteManager_GetSpriteList(work->spriteManager), 1, 3, ownPositions);
}

#pragma force_active reset

// Pokemon that run into each other bounce apart: each is pushed by the other's power (a fifth more
// for a small Pokemon), once per contact.
#pragma force_active on

void PokeathlonDiscCatch_UpdateCollisions(PokeathlonCourseData *course) {
    int i, j;
    PokeathlonDiscCatchMon *a, *b;
    fx32 power;
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8(*touching)[DISC_CATCH_MONS] = work->field.touching;
    for (i = 0; i < DISC_CATCH_MONS; i++) {
        a = &work->field.teams[i / DISC_CATCH_TEAM_SIZE].mons[i % DISC_CATCH_TEAM_SIZE];
        for (j = i + 1; j < DISC_CATCH_MONS; j++) {
            b = &work->field.teams[j / DISC_CATCH_TEAM_SIZE].mons[j % DISC_CATCH_TEAM_SIZE];
            if (PokeathlonDiscCatchMon_IsInPlay(a) && PokeathlonDiscCatchMon_IsInPlay(b) && PokeathlonDiscCatchMon_IsTouching(a, b) && !touching[i][j]) {
                VecFx32 push = { 0, 0, 0 };
                touching[i][j] = touching[j][i] = TRUE;
                VEC_Subtract(&a->position, &b->position, &push);
                VEC_Normalize(&push, &push);
                power = b->stats->power;
                if (a->large == FALSE) {
                    power = power * 6 / 5;
                }
                VEC_MultAdd(power, &push, &a->direction, &a->velocity);
                power = a->stats->power;
                if (b->large == FALSE) {
                    power = power * 6 / 5;
                }
                VEC_MultAdd(power, &push, &b->direction, &b->velocity);
                b->velocity.x *= -1;
                b->velocity.y *= -1;
                PokeathlonDiscCatch_ClampVelocity(&a->velocity, FX32_CONST(8));
                PokeathlonDiscCatch_ClampVelocity(&b->velocity, FX32_CONST(8));
                b->facing = PokeathlonDiscCatch_GetFacing(&push);
                b->knockedBack = TRUE;
                a->knockedBack = TRUE;
            } else {
                touching[i][j] = touching[j][i] = FALSE;
            }
        }
    }
}

#pragma force_active reset

// The host's field step: each team's touch picks up, drags or (tapped) makes jump one of its
// Pokemon; then the Pokemon move and bounce off each other.
void PokeathlonDiscCatch_UpdateField(PokeathlonDiscCatchTeam *teams, PokeathlonCourseData *course) {
    int participant;
    ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
    for (participant = 0; participant < DISC_CATCH_PARTICIPANTS; participant++, teams++) {
        PokeathlonDiscCatchMon *mon;
        int i;
        teams->mons[teams->heldMon].dragged = FALSE;
        if (teams->touchNew) {
            u8 touched = PokeathlonDiscCatch_FindTouchedMon(teams->mons, teams->touchX, teams->touchY);
            if (touched != DISC_CATCH_NO_MON) {
                teams->holding = TRUE;
                teams->heldMon = touched;
            }
        } else if (teams->touching) {
            if (teams->holding == TRUE) {
                mon = &teams->mons[teams->heldMon];
                {
                    s16 monX = mon->position.x >> FX32_SHIFT;
                    s16 monY = mon->position.y >> FX32_SHIFT;
                    if (mon->state == DISC_CATCH_MON_FALLEN) {
                        teams->holding = FALSE;
                    } else if (DISC_CATCH_ABS(teams->touchX - monX) > 8 || DISC_CATCH_ABS(teams->touchY - monY) > 8) {
                        PokeathlonDiscCatchMon_SetTarget(mon, &teams->touchX);
                        mon->draggedOff = PokeathlonDiscCatchMon_IsOffField(mon, mon->target.x, mon->target.y);
                    }
                }
            }
        } else {
            if (teams->holding == TRUE) {
                u8 touched;
                mon = &teams->mons[teams->heldMon];
                touched = PokeathlonDiscCatch_FindTouchedMon(teams->mons, teams->touchX, teams->touchY);
                if (mon->state == DISC_CATCH_MON_ON_FIELD && touched == teams->heldMon && teams->lastTouchFrames <= 5) {
                    PokeathlonDiscCatchMon_Jump(mon);
                }
            }
            teams->holding = FALSE;
        }
        for (i = 0, mon = teams->mons; i < DISC_CATCH_TEAM_SIZE; i++, mon++) {
            PokeathlonDiscCatchMon_Update(mon, course);
        }
    }
    PokeathlonDiscCatch_UpdateCollisions(course);
}

// The host's frame packet: the Pokemon's places, facings, jumps and falls, and one team's score.
#pragma force_active on

void PokeathlonDiscCatch_WriteFramePacket(PokeathlonDiscCatchField *field, PokeathlonCourseData *course) {
    int x, y;
    PokeathlonDiscCatchFrame *frame;
    int i, shift;
    frame = (PokeathlonDiscCatchFrame *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
    frame->scoreParticipant++;
    frame->score = field->teams[frame->scoreParticipant].result.score & 0x7F;
    frame->jumped = 0;
    frame->facings = 0;
    frame->fallen = 0;
    for (i = 0, shift = 0; i < DISC_CATCH_MONS; i++, shift += 2) {
        int value;
        PokeathlonDiscCatchMon *mon = &field->teams[i / DISC_CATCH_TEAM_SIZE].mons[i % DISC_CATCH_TEAM_SIZE];
        PokeathlonCourseObject_CenterToGround(mon->monPic, (s16)(mon->position.x >> FX32_SHIFT), (s16)(mon->position.y >> FX32_SHIFT), &x, &y);
        value = x;
        if (value > 255) {
            value = 255;
        } else if (value < 0) {
            value = 0;
        }
        frame->monX[i] = value;
        value = y;
        if (value > 255) {
            value = 255;
        } else if (value < 0) {
            value = 0;
        }
        frame->monY[i] = value;
        if (mon->jumpSignal) {
            frame->jumped |= 1 << i;
            mon->jumpSignal--;
        }
        if (mon->state == DISC_CATCH_MON_FALLEN) {
            frame->fallen |= 1 << i;
        }
        frame->facings += (mon->facing - 1) << shift;
    }
}

#pragma force_active reset

void PokeathlonDiscCatchDisc_Launch(PokeathlonDiscCatchDisc *disc, u8 speed, u8 index) {
    disc->active = TRUE;
    disc->x = 128; // overwritten below
    disc->progress = 0;
    disc->x = 0;
    disc->state = DISC_CATCH_DISC_FLYING;
    disc->index = index;
    disc->path = MTRandom() % 15;
    disc->timer = 0;
    disc->speed = speed;
}

void PokeathlonDiscCatchDisc_Remove(PokeathlonDiscCatchDisc *disc) {
    disc->active = FALSE;
    disc->state = DISC_CATCH_DISC_NONE;
}

// Once the disc is over the touch screen: the first jumping Pokemon it is over catches it.
#pragma force_active on

BOOL PokeathlonDiscCatchDisc_FindCatcher(PokeathlonDiscCatchDisc *disc, PokeathlonDiscCatchTeam *teams, PokeathlonDiscCatchCatcher *catcher) {
    s16 y;
    int i;
    if (disc->progress < 288) {
        return FALSE;
    }
    y = disc->progress - 288;
    for (i = 0; i < DISC_CATCH_MONS; i++) {
        VecFx32 distance = { 0, 0, 0 };
        int groundX, groundY;
        int member = i % DISC_CATCH_TEAM_SIZE, participant = i / DISC_CATCH_TEAM_SIZE;
        PokeathlonDiscCatchMon *mon = &teams[participant].mons[member];
        if (mon->state == DISC_CATCH_MON_JUMPING) {
            PokeathlonCourseObject_CenterToGround(mon->monPic, mon->position.x >> FX32_SHIFT, mon->position.y >> FX32_SHIFT, &groundX, &groundY);
            distance.x = groundX - disc->x;
            distance.y = groundY - y;
            distance.x <<= FX32_SHIFT;
            distance.y <<= FX32_SHIFT;
            if (VEC_Mag(&distance) <= (PokeathlonCourseObject_GetCollisionRadius(mon->monPic) + 14) << FX32_SHIFT) {
                catcher->participant = participant;
                catcher->member = member;
                catcher->x = groundX;
                catcher->y = groundY;
                return TRUE;
            }
        }
    }
    return FALSE;
}

#pragma force_active reset

// The outer corners of a scoring band (the last one to three tile rows, outside a narrowing middle)
// are worth the next band's points.
BOOL PokeathlonDiscCatch_IsInBonusZone(const s16 *tile, int bandEnd) {
    BOOL result = TRUE;
    BOOL inside = TRUE;
    BOOL lastRow = FALSE;
    s16 y = tile[1];
    if (y == bandEnd - 1) {
        BOOL outside = TRUE;
        if (tile[0] >= 8 && tile[0] <= 23) {
            outside = FALSE;
        }
        if (outside) {
            lastRow = TRUE;
        }
    }
    if (!lastRow) {
        BOOL secondRow = FALSE;
        if (y == bandEnd - 2) {
            BOOL outside = TRUE;
            if (tile[0] >= 5 && tile[0] <= 26) {
                outside = FALSE;
            }
            if (outside) {
                secondRow = TRUE;
            }
        }
        if (!secondRow) {
            inside = FALSE;
        }
    }
    if (!inside) {
        BOOL thirdRow = FALSE;
        if (y == bandEnd - 3) {
            BOOL outside = TRUE;
            if (tile[0] >= 2 && tile[0] <= 29) {
                outside = FALSE;
            }
            if (outside) {
                thirdRow = TRUE;
            }
        }
        if (!thirdRow) {
            result = FALSE;
        }
    }
    return result;
}

// The disc's points by where it was caught (bands of tile rows down the touch screen).
void PokeathlonDiscCatchDisc_Catch(PokeathlonDiscCatchDisc *disc, PokeathlonDiscCatchTeam *teams, const PokeathlonDiscCatchCatcher *catcher) {
    s16 tile[2];
    PokeathlonDiscCatchTeam *team;
    int score;
    int points;
    PokeathlonCourseData *course;
    tile[0] = catcher->x / 8;
    tile[1] = catcher->y / 8;
    if (tile[1] < 9) {
        disc->state = DISC_CATCH_DISC_CAUGHT_1;
        if (PokeathlonDiscCatch_IsInBonusZone(tile, 9)) {
            disc->state = DISC_CATCH_DISC_CAUGHT_2;
        }
    } else if (tile[1] < 15) {
        disc->state = DISC_CATCH_DISC_CAUGHT_2;
        if (PokeathlonDiscCatch_IsInBonusZone(tile, 15)) {
            disc->state = DISC_CATCH_DISC_CAUGHT_3;
        }
    } else if (tile[1] < 20) {
        disc->state = DISC_CATCH_DISC_CAUGHT_3;
        if (PokeathlonDiscCatch_IsInBonusZone(tile, 20)) {
            disc->state = DISC_CATCH_DISC_CAUGHT_5;
        }
    } else {
        disc->state = DISC_CATCH_DISC_CAUGHT_5;
    }
    team = &teams[catcher->participant];
    course = team->mons[0].course;
    points = PokeathlonDiscCatchDisc_GetPoints(disc);
    score = team->result.score;
    score += points;
    if (score > 200) {
        score = 200;
    }
    team->result.score = score;
    disc->owner = catcher->participant;
    PokeathlonCourse_AddMonCounter(course, catcher->participant, catcher->member, 3, 1);
}

// A disc's frame: flying (until caught or landed), landed (sliding off) or caught (shown for 20 frames).
void PokeathlonDiscCatchDisc_Update(PokeathlonDiscCatchDisc *disc, PokeathlonDiscCatchDiscPacket *packet, PokeathlonDiscCatchTeam *teams) {
    PokeathlonDiscCatchCatcher catcher;
    if (!disc->active) {
        return;
    }
    switch (disc->state) {
    case DISC_CATCH_DISC_FLYING:
        if (PokeathlonDiscCatchDisc_FindCatcher(disc, teams, &catcher)) {
            PokeathlonDiscCatchDisc_Catch(disc, teams, &catcher);
        } else if (disc->progress >= 480) {
            disc->progress = 0;
            disc->state = DISC_CATCH_DISC_LANDED;
        } else {
            disc->progress += disc->speed;
            disc->x = PokeathlonDiscCatch_GetDiscX(disc->progress, disc->path);
        }
        break;
    case DISC_CATCH_DISC_LANDED:
        disc->progress += disc->speed;
        if (disc->progress >= 48) {
            PokeathlonDiscCatchDisc_Remove(disc);
        }
        break;
    case DISC_CATCH_DISC_CAUGHT_1:
    case DISC_CATCH_DISC_CAUGHT_2:
    case DISC_CATCH_DISC_CAUGHT_3:
    case DISC_CATCH_DISC_CAUGHT_5:
        disc->timer++;
        if (disc->timer >= 20) {
            PokeathlonDiscCatchDisc_Remove(disc);
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    packet->progress = (disc->progress * 2) / 8;
    packet->path = disc->path;
    packet->state = disc->state;
    packet->owner = disc->owner;
}

void PokeathlonDiscCatchDiscs_Init(PokeathlonDiscCatchDiscs *discs, PokeathlonDiscCatchTeam *teams) {
    discs->teams = teams;
}

// The phase of the event by the seconds left (more discs, faster, towards the end).
int PokeathlonDiscCatch_GetPhase(u32 seconds) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (ov96_0221CF1C[i] <= seconds) {
            return i;
        }
    }
    return -1;
}

#pragma force_active on

void PokeathlonDiscCatchDiscs_LaunchNext(PokeathlonDiscCatchDiscs *discs, u32 seconds) {
    int count = 1;
    discs->phase = PokeathlonDiscCatch_GetPhase(seconds);
    switch (discs->phase) {
    case 0:
    case 2:
        discs->delay = 30;
        break;
    case 1:
    case 3:
        discs->delay = 0;
        break;
    }
    u8 speed;
    if (discs->phase < 3) {
        speed = 4;
    } else {
        speed = ((MTRandom() & 1) + 1) * 4;
    }
    if (discs->phase != 0) {
        count = DISC_CATCH_DISCS;
    }
    for (int i = 0; i < count; i++) {
        if (!discs->discs[i].active) {
            PokeathlonDiscCatchDisc_Launch(&discs->discs[i], speed, (u8)i);
            return;
        }
    }
}

void PokeathlonDiscCatchDiscs_Update(PokeathlonDiscCatchDiscs *discs, u8 *area, u32 seconds) {
    int i;
    PokeathlonDiscCatchDiscPacket *packets = (PokeathlonDiscCatchDiscPacket *)ov96_021E8A20(area + 0x28);
    if (discs->delay == 0) {
        PokeathlonDiscCatchDiscs_LaunchNext(discs, seconds);
    } else {
        discs->delay--;
    }
    for (i = 0; i < DISC_CATCH_DISCS; i++) {
        PokeathlonDiscCatchDisc_Update(&discs->discs[i], &packets[i], discs->teams);
    }
}

void PokeathlonDiscCatchSprites_Init(PokeathlonDiscCatchSprites *sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonDiscCatchScoreboard *scoreboard) {
    int i, j;
    BOOL onTouchScreen;
    PokeathlonDiscCatchDiscSprites *disc;
    sprites->scoreboard = scoreboard;
    for (i = 0; i < DISC_CATCH_MONS; i++) {
        sprites->mons[i].splash = PokeathlonDiscCatch_NewEffectSprite(spriteSystem, spriteManager, 0, 0, 5, 5);
    }
    for (i = 0, disc = sprites->discs; i < DISC_CATCH_DISCS; disc++, i++) {
        for (j = 0; j < 2; j++) {
            if (j == 1) {
                onTouchScreen = TRUE;
            } else {
                onTouchScreen = FALSE;
            }
            disc->disc[j] = PokeathlonDiscCatch_NewDiscSprite(spriteSystem, spriteManager, 0, 0, 0, 3, onTouchScreen);
            disc->shadow[j] = PokeathlonDiscCatch_NewDiscSprite(spriteSystem, spriteManager, 0, 0, 2, 20, onTouchScreen);
        }
        disc->points = PokeathlonDiscCatch_NewEffectSprite(spriteSystem, spriteManager, 0, 0, 0, 1);
        disc->sparkle = PokeathlonDiscCatch_NewEffectSprite(spriteSystem, spriteManager, 0, 0, 4, 2);
    }
}

#pragma force_active reset

// Every participant's frame: shows the host's frame packet (discs, Pokemon, a team's score, the time).
#pragma force_active on

void PokeathlonDiscCatchSprites_Update(PokeathlonDiscCatchSprites *sprites, PokeathlonCourseData *course) {
    PokeathlonDiscCatchFrame *frame = (PokeathlonDiscCatchFrame *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonDiscCatch_UpdateDiscSprites(sprites, (PokeathlonDiscCatchDiscPacket *)frame, course);
    PokeathlonDiscCatch_UpdateMonSprites(sprites, frame, course);
    u8 participant = frame->scoreParticipant;
    if (frame->score != sprites->shownScores[participant]) {
        PokeathlonDiscCatchScoreboard_OnScore(sprites->scoreboard, participant, frame->score);
    }
    sprites->shownScores[participant] = frame->score;
    int seconds = frame->seconds - 1;
    if (seconds <= 0) {
        seconds = 0;
    }
    if (seconds <= 0) {
        seconds = 0;
    }
    PokeathlonDiscCatchScoreboard_SetSeconds(work->scoreboard, seconds);
    PokeathlonCourse_UpdateTimeUpCountdown(course, 30 * frame->seconds);
}

#pragma force_active reset

void PokeathlonDiscCatchSprites_Delete(PokeathlonDiscCatchSprites *sprites) {
    s32 i;
    s32 j;
    PokeathlonDiscCatchDiscSprites *disc;
    i = 0;
    for (; i < DISC_CATCH_MONS; i++) {
        GF_ASSERT(sprites->mons[i].splash != NULL);
        Sprite_DeleteAndFreeResources(sprites->mons[i].splash);
        if (sprites->mons[i].squash.task != NULL) {
            SysTask_Destroy(sprites->mons[i].squash.task);
        }
    }
    i = 0;
    disc = sprites->discs;
    for (; i < DISC_CATCH_DISCS; i++, disc++) {
        j = 0;
        for (; j < 2; j++) {
            Sprite_DeleteAndFreeResources(disc->disc[j]);
            Sprite_DeleteAndFreeResources(disc->shadow[j]);
        }
        Sprite_DeleteAndFreeResources(disc->points);
        Sprite_DeleteAndFreeResources(disc->sparkle);
    }
}

void PokeathlonDiscCatch_AddScoreWindow(PokeathlonDiscCatch *work) {
    AddWindow(work->bgConfig, &work->window, &ov96_0221CEEC);
}
