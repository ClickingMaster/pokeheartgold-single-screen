#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_goal_roll.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "math_util.h"
#include "player_data.h"
#include "sound_playback.h"
#include "text.h"

// Course helpers, sprite scene and object helpers (OVY_96, not yet tidied).
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int member);
int PokeathlonCourse_GetTouchRadius(void);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int event, int value);
BOOL PokeathlonCourse_IsInRange(int x, int y, int touchX, int touchY, int radius, int *distance);
void PokeathlonCourseObject_GetCollisionCircle(void *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY, int *outRadius);
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonGoalRollSprite *sprite, BOOL shown, BOOL show);
void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonGoalRollSprite *sprite, int anim);

extern const VecFx32 gPokeathlonGoalRollNormalTopLeft;
extern const VecFx32 gPokeathlonGoalRollNormalBottomRight;
extern const VecFx32 gPokeathlonGoalRollNormalBottomLeft;
extern const VecFx32 gPokeathlonGoalRollUp;
extern const VecFx32 gPokeathlonGoalRollFrameLeft;
extern const VecFx32 gPokeathlonGoalRollNormalTopRight;
extern const VecFx32 gPokeathlonGoalRollFrameRight;
extern const WindowTemplate gPokeathlonGoalRollNameWindowTemplates[];
extern const s16 gPokeathlonGoalRollBallSpawnPoints[];
extern const s16 ov96_0221D43A[];
extern const Point2D gPokeathlonGoalRollGoalLines[4][2]; // the goal line of each corner
extern const VecFx32 gPokeathlonGoalRollGoalPosts[4][2];
extern const u8 gPokeathlonGoalRollBallSpeedsA[][30];
extern const u8 gPokeathlonGoalRollBallSpeedsB[][30];
extern const u8 gPokeathlonGoalRollKickSpeeds[];
extern const u8 gPokeathlonGoalRollBounceOffsets[];

extern const PokeathlonGoalRollWindowData gPokeathlonGoalRollWindowData;

#pragma force_active on

void PokeathlonGoalRoll_ShowGoal(PokeathlonGoalRoll *goalRoll, u8 ballIndex, int bigBall, u8 x, u8 code, u8 own) {
    u8 goal, owner, scorer;
    int anim;
    PokeathlonGoalRollBall *ball;

    if (code < 0xD8) {
        return;
    }
    ball = &goalRoll->balls[ballIndex];
    if (ball->scored != 0) {
        return;
    }
    ball->scored = 1;
    goal = code - 0xD8;
    GF_ASSERT(goal < 16);
    owner = goal / 4;
    PokeathlonCourseSpritePoolSprite_SetAnim(goalRoll->goalSprites[owner], 15);
    scorer = goal % 4;
    if (scorer != owner) {
        int isBig = 0;
        if (bigBall) {
            isBig = 1;
        }
        PokeathlonGoalRoll_StartPlateFlash(goalRoll, scorer, 1, isBig);
        PokeathlonGoalRoll_StartPlateFlash(goalRoll, owner, 0, 0);
    } else {
        PokeathlonGoalRoll_StartPlateFlash(goalRoll, owner, 0, 0);
    }
    if (goal % 5 == 0 && own == goal / 5) {
        PlaySE(SEQ_SE_GS_WH01);
        return;
    }
    if (own == scorer) {
        switch (owner) {
        case 0:
        case 2:
            anim = 17;
            break;
        case 1:
        case 3:
            anim = 18;
            break;
        default:
            GF_AssertFail();
            return;
        }
        PokeathlonCourseSpritePoolSprite_Show(goalRoll->goalFlashSprites[owner], TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_SetAnim(goalRoll->goalFlashSprites[owner], anim);
        PokeathlonCourseSpritePoolSprite_Show(goalRoll->goalLabelSprites[owner], TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_SetAnim(goalRoll->goalLabelSprites[owner], owner + 19);
        PlaySE(SEQ_SE_GS_WH02);
    } else {
        PlaySE(SEQ_SE_GS_WH01);
    }
}

void PokeathlonGoalRoll_MoveMons(PokeathlonCourseData *course) {
    VecFx32 previous[12];
    PokeathlonGoalRoll *goalRoll;
    int i;
    PokeathlonGoalRollMon *mon;
    fx32 distance;
    fx32 limit;
    u16 angle;

    goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    VecFx32 zero = { 0, 0, 0 };
    VecFx32 delta;
    VecFx32 direction;
    VecFx32 position;

    for (i = 0; i < 12; i++) {
        mon = &goalRoll->mons[i / 3][i % 3];
        mon->direction = zero;
        previous[i] = mon->position;
        position = mon->position;
        if (mon->motion != GOAL_ROLL_MOTION_NONE) {
            if ((mon->motion == GOAL_ROLL_MOTION_2 || mon->motion == GOAL_ROLL_MOTION_KNOCKBACK) && mon->motionWait > 0) {
                mon->motionWait--;
            }
            if (mon->motionFrames == 0 && mon->motionWait <= 0) {
                mon->motion = GOAL_ROLL_MOTION_NONE;
                mon->motionWait = 0;
            } else {
                VEC_MultAdd(mon->motionFrames << 12, &mon->motionStep, &position, &position);
                mon->position = position;
                mon->target = mon->position;
                if (mon->motionFrames > 0) {
                    mon->motionFrames--;
                }
                if (mon->mode != GOAL_ROLL_MODE_STUNNED && mon->motion == GOAL_ROLL_MOTION_KNOCKBACK) {
                    PokeathlonGoalRoll_NextFacing(mon);
                }
            }
        }
        if (mon->mode == GOAL_ROLL_MODE_STUNNED || mon->stunTimer != 0) {
            mon->stunTimer--;
            if (mon->stunTimer == 0) {
                mon->mode = GOAL_ROLL_MODE_WALK;
                mon->power = mon->maxPower;
            }
            continue;
        }
        if (mon->motion != GOAL_ROLL_MOTION_NONE) {
            continue;
        }
        VEC_Subtract(&mon->target, &position, &delta);
        distance = VEC_Mag(&delta);
        if (distance <= 0) {
            PokeathlonGoalRoll_RecoverPower(mon);
            continue;
        }
        VEC_Normalize(&delta, &direction);
        mon->direction = direction;
        if (mon->mode == GOAL_ROLL_MODE_DASH) {
            limit = mon->dashSpeed;
        } else if (mon->mode == GOAL_ROLL_MODE_TIRED) {
            limit = 2 * FX32_ONE;
        } else {
            limit = mon->walkSpeed;
        }
        if (distance <= limit) {
            position = mon->target;
        } else {
            VecFx32 origin = { 0, 0, 0 };

            VEC_MultAdd(limit, &direction, &origin, &delta);
            VEC_Add(&delta, &position, &position);
        }
        if (distance != 0) {
            VecFx32 up = gPokeathlonGoalRollUp;
            VecFx32 heading;

            heading.x = direction.y;
            heading.z = direction.x;
            heading.y = 0;
            angle = CalcAngleBetweenVecs(&up, &heading);
            if (angle <= 0x2000 || angle >= 0xE000) {
                mon->facing = 4;
            } else if (angle > 0x2000 && angle < 0x6000) {
                mon->facing = 2;
            } else if (angle >= 0x6000 && angle <= 0xA000) {
                mon->facing = 3;
            } else {
                mon->facing = 1;
            }
        }
        mon->position = position;
    }
    {
        PokeathlonGoalRollMon *moved;
        VecFx32 *from;
        for (i = 0, from = previous; i < 12; i++, from++) {
            moved = &goalRoll->mons[i / 3][i % 3];
            if (moved->position.x != from->x || moved->position.y != from->y) {
                VecFx32 out;

                if (PokeathlonGoalRoll_CollideWithWalls(&moved->position, &previous[i], moved->radius, TRUE, &out)) {
                    moved->position = out;
                    moved->target = moved->position;
                }
            }
            if (moved->position.x == moved->target.x && moved->position.y == moved->target.y && moved->mode != GOAL_ROLL_MODE_TIRED && moved->mode != GOAL_ROLL_MODE_STUNNED) {
                moved->mode = GOAL_ROLL_MODE_WALK;
            }
        }
    }
}

// PokeathlonGoalRoll_IsWithinAngle as this call was built: with a u16 angle.
typedef BOOL (*PokeathlonGoalRollIsWithinAngleFunc)(const VecFx32 *a, const VecFx32 *b, u16 angle);

// The balls against the Pokemon (pushed, kicked, knocking a Pokemon back) and the goal posts.
void PokeathlonGoalRoll_CollideBalls(PokeathlonCourseData *course) {
    int member;
    VecFx32 *ballPosition;
    PokeathlonGoalRollMon(*teams)[GOAL_ROLL_TEAM_SIZE];
    int k;
    PokeathlonGoalRoll *goalRoll;
    int radius;
    u8 speed;
    int participant;
    fx32 ballRadius;
    PokeathlonGoalRollBall *ball;
    PokeathlonGoalRollMon *mon;
    int i;
    fx32 dot;

    goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    for (k = 0, ball = goalRoll->balls; k < 2; k++, ball++) {
        if (ball->kind == GOAL_ROLL_BALL_NONE || (ball->state != GOAL_ROLL_BALL_ROLLING && ball->state != GOAL_ROLL_BALL_KICKED)) {
            continue;
        }
        if (ball->kind == GOAL_ROLL_BALL_SMALL) {
            radius = 8;
        } else if (ball->kind == GOAL_ROLL_BALL_BIG) {
            radius = 12;
        }
        ballRadius = radius * FX32_ONE;
        for (i = 0, ballPosition = &ball->position, teams = goalRoll->mons; i < 12; i++) {
            VecFx32 center = { 0, 0, 0 };
            VecFx32 normal;
            int monRadius;
            member = i % 3;
            participant = i / 3;
            mon = &teams[participant][member];
            PokeathlonCourseObject_GetCollisionCircle(mon->object, mon->position.x, mon->position.y, &center.x, &center.y, &monRadius);
            if (PokeathlonGoalRoll_CirclesOverlap(&center, monRadius * FX32_ONE, ballPosition, ballRadius)) {
                if ((ball->touching[i] != 0 && ball->bounced != 0) || mon->motion == GOAL_ROLL_MOTION_KNOCKBACK || mon->motion == GOAL_ROLL_MOTION_2) {
                    continue;
                }
                VEC_Subtract(ballPosition, &center, &normal);
                VEC_Normalize(&normal, &normal);
                if (ball->state == GOAL_ROLL_BALL_KICKED) {
                    int power;
                    if (ball->lastKicker == i) {
                        continue;
                    }
                    power = mon->power - ball->strength;
                    if (power <= 0) {
                        mon->power = 0;
                        mon->mode = GOAL_ROLL_MODE_STUNNED;
                        mon->stunTimer = 90;
                        mon->motion = GOAL_ROLL_MOTION_KNOCKBACK;
                        mon->motionWait = 30;
                        mon->motionStep = normal;
                        mon->motionStep.x *= -1;
                        mon->motionStep.y *= -1;
                        mon->motionFrames = 7;
                        PokeathlonCourse_AddMonCounter(course, participant, member, 1, 1);
                    } else {
                        mon->power = power;
                        mon->motion = GOAL_ROLL_MOTION_KNOCKBACK;
                        mon->motionWait = 30;
                        mon->motionFrames = 7;
                        mon->motionStep = normal;
                        mon->motionStep.x *= -1;
                        mon->motionStep.y *= -1;
                    }
                } else {
                    if (VEC_Mag(&mon->direction)) {
                        BOOL facingBall = ((PokeathlonGoalRollIsWithinAngleFunc)PokeathlonGoalRoll_IsWithinAngle)(&mon->direction, &normal, (mon->kickAngle << 16) / 360);
                        if (mon->mode == GOAL_ROLL_MODE_DASH && facingBall) {
                            ball->state = GOAL_ROLL_BALL_KICKED;
                            ball->speedTable = 2;
                            ball->speedRow = mon->kickRow;
                            ball->strength = mon->kickPower;
                            ball->kickLevel = mon->kickLevel;
                            speed = gPokeathlonGoalRollBallSpeedsA[ball->speedRow][0];
                            mon->motion = GOAL_ROLL_MOTION_RECOIL;
                            mon->motionStep = normal;
                            mon->motionStep.x *= -1;
                            mon->motionStep.y *= -1;
                            mon->motionFrames = 3;
                            mon->mode = GOAL_ROLL_MODE_WALK;
                            ball->step = 1;
                            ball->steps = 30;
                            PokeathlonCourse_AddMonCounter(course, participant, member, 4, 1);
                        } else {
                            ball->speedTable = 1;
                            ball->speedRow = mon->kickRow;
                            speed = gPokeathlonGoalRollBallSpeedsB[ball->speedRow][0];
                            ball->step = 1;
                            ball->steps = 30;
                        }
                        ball->speed = speed;
                    }
                    PokeathlonGoalRoll_SetBallDirection(goalRoll, ball, &normal);
                    ball->lastKicker = i;
                }
                ball->touching[i] = TRUE;
            } else {
                ball->touching[i] = FALSE;
            }
        }
        for (i = 0; i < 8; i++) {
            VecFx32 normal;
            const VecFx32 *post = &gPokeathlonGoalRollGoalPosts[i % 4][i / 4];
            if (PokeathlonGoalRoll_CirclesOverlap(post, 2 * FX32_ONE * 4, &ball->position, ballRadius)) {
                VEC_Subtract(post, &ball->position, &normal);
                VEC_Normalize(&normal, &normal);
                dot = VEC_DotProduct(&ball->velocity, &normal);
                if (dot > 0) {
                    VEC_MultAdd(-(dot * 2), &normal, &ball->velocity, &ball->velocity);
                    VEC_Normalize(&ball->velocity, &ball->direction);
                }
                break;
            }
        }
    }
}

void PokeathlonGoalRoll_MoveBalls(PokeathlonCourseData *course) {
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    VecFx32 previous, zero, out;
    int i;

    if (goalRoll->balls[1].kind == GOAL_ROLL_BALL_NONE && goalRoll->timer <= 900) {
        goalRoll->balls[1].state = GOAL_ROLL_BALL_WAITING;
        goalRoll->balls[1].kind = GOAL_ROLL_BALL_SMALL;
    }
    for (i = 0; i < 2; i++) {
        PokeathlonGoalRollBall *ball = &goalRoll->balls[i];
        int speed;
        u8 chance;
        int wall;

        if (!ball->kind) {
            continue;
        }
        if (ball->state == GOAL_ROLL_BALL_WAITING) {
            ball->wait++;
            if (ball->wait < 8) {
                continue;
            }
            ball->wait = 0;
            ball->state = GOAL_ROLL_BALL_ROLLING;
            continue;
        }
        if (ball->state == GOAL_ROLL_BALL_SCORED) {
            ball->timer--;
            if (ball->timer != 0) {
                continue;
            }
            PokeathlonGoalRoll_ResetBall(ball);
            PokeathlonGoalRoll_PickBallSpawn(goalRoll, ball);
            // the first ball comes back as the big one, more likely late in the match and after misses
            if (i == 0) {
                if (goalRoll->timer >= 1200) {
                    chance = 0;
                } else if (goalRoll->timer >= 600) {
                    chance = 5;
                } else if (goalRoll->timer >= 300) {
                    chance = 10;
                } else {
                    chance = 25;
                }
                chance += goalRoll->bigBallMisses;
                if (chance > 100) {
                    chance = 100;
                }
                if ((int)LCRandom() % 100 < chance) {
                    ball->kind = GOAL_ROLL_BALL_BIG;
                    goalRoll->bigBallMisses = 0;
                } else {
                    ball->kind = GOAL_ROLL_BALL_SMALL;
                    if (goalRoll->bigBallMisses < 100) {
                        goalRoll->bigBallMisses++;
                    }
                }
            } else {
                ball->kind = GOAL_ROLL_BALL_SMALL;
            }
            continue;
        }
        if (ball->state != GOAL_ROLL_BALL_ROLLING && ball->state != GOAL_ROLL_BALL_KICKED) {
            continue;
        }
        speed = ball->speed << 12;
        if (speed) {
            previous = ball->position;
            ball->position.x += ball->velocity.x;
            ball->position.y += ball->velocity.y;
            if (speed >= gPokeathlonGoalRollKickSpeeds[ball->kickLevel] << 12) {
                if (ball->speedTable == 2) {
                    ball->state = GOAL_ROLL_BALL_KICKED;
                } else {
                    ball->state = GOAL_ROLL_BALL_ROLLING;
                }
            } else {
                ball->state = GOAL_ROLL_BALL_ROLLING;
                ball->strength = 0;
            }
            {
                VecFx32 *zeroPtr = &zero;
                memset(zeroPtr, 0, sizeof(*zeroPtr));
            }
            {
                int next;
                int step = ball->step;
                if (ball->speedTable == 2) {
                    next = gPokeathlonGoalRollBallSpeedsA[ball->speedRow][step];
                } else if (ball->speedTable == 1) {
                    next = gPokeathlonGoalRollBallSpeedsB[ball->speedRow][step];
                } else {
                    GF_AssertFail();
                    next = 0;
                }
                ball->speed = next;
            }
            PokeathlonGoalRoll_SetBallDirection(goalRoll, ball, &ball->direction);
            ball->step++;
            if (ball->step >= ball->steps) {
                ball->velocity.x = 0;
                ball->velocity.y = 0;
                ball->direction = zero;
                ball->speedTable = 0;
                ball->speed = 0;
            } else if (ball->speed == 0) {
                ball->direction = zero;
                ball->step = 0;
                ball->steps = 0;
                ball->speedTable = 0;
            }
            ball->bounced = FALSE;
            wall = PokeathlonGoalRoll_CollideWithWalls(&ball->position, &previous, 8, FALSE, &out);
            if (wall) {
                ball->position = out;
                ball->bounced = TRUE;
            }
            switch (wall) {
            case 3:
            case 4:
                ball->direction.y *= -1;
                ball->velocity.y *= -1;
                break;
            case 1:
            case 2:
                ball->direction.x *= -1;
                ball->velocity.x *= -1;
                break;
            }
        } else {
            ball->state = GOAL_ROLL_BALL_ROLLING;
            ball->strength = 0;
            ball->kickLevel = 0;
            ball->speedTable = 0;
        }
    }
    if (goalRoll->balls[0].kind && goalRoll->balls[1].kind) {
        PokeathlonGoalRollBall *first = &goalRoll->balls[0];
        PokeathlonGoalRollBall *second = &goalRoll->balls[1];
        int radius;
        if (first->state == GOAL_ROLL_BALL_SCORED || first->state == GOAL_ROLL_BALL_WAITING || second->state == GOAL_ROLL_BALL_SCORED || second->state == GOAL_ROLL_BALL_WAITING) {
            return;
        }
        if (first->kind == GOAL_ROLL_BALL_SMALL) {
            radius = 8;
        } else if (first->kind == GOAL_ROLL_BALL_BIG) {
            radius = 12;
        } else {
            GF_AssertFail();
            return;
        }
        if (PokeathlonGoalRoll_CirclesOverlap(&first->position, radius << 12, &second->position, 2 << 14)) {
            PokeathlonGoalRoll_CollideBallPair(first, second);
        }
    }
}

#pragma force_active reset

void PokeathlonGoalRoll_CollideBallPair(PokeathlonGoalRollBall *first, PokeathlonGoalRollBall *second) {
    PokeathlonGoalRollBall saved;

    saved.velocity = first->velocity;
    saved.direction = first->direction;
    saved.state = first->state;
    saved.lastKicker = first->lastKicker;
    saved.step = first->step;
    saved.steps = first->steps;
    saved.speedTable = first->speedTable;
    saved.speed = first->speed;
    saved.speedRow = first->speedRow;
    saved.position = first->position;
    PokeathlonGoalRoll_BounceBallOff(first, second, 0);
    PokeathlonGoalRoll_BounceBallOff(second, &saved, 1);
}

#pragma force_active on

// Sends `ball` away from `other`, taking over the other ball's kick if it was faster.
void PokeathlonGoalRoll_BounceBallOff(PokeathlonGoalRollBall *ball, const PokeathlonGoalRollBall *other, int side) {
    struct {
        volatile int side;
        VecFx32 right;
        VecFx32 left;
        VecFx32 zero;
    } frame;

    frame.side = side;
    memset(&frame.zero, 0, sizeof frame.zero);
    if (ball->speed <= 3) {
        if (other->speed > 3) {
            ball->state = other->state;
            ball->step = other->step;
            ball->steps = other->steps;
            ball->speedTable = other->speedTable;
            ball->speed = other->speed;
            ball->speedRow = other->speedRow;
        } else {
            ball->state = GOAL_ROLL_BALL_ROLLING;
            ball->step = 0;
            ball->steps = 30;
            ball->speedTable = 1;
            ball->speed = 3;
            ball->speedRow = 0;
        }
    } else {
        ball->state = GOAL_ROLL_BALL_ROLLING;
        ball->step = 0;
        ball->steps = 30;
        ball->speedTable = 1;
        ball->speed = 3;
        ball->speedRow = 0;
    }
    VEC_Subtract(&ball->position, &other->position, &ball->direction);
    if (VEC_Mag(&ball->direction) == 0) {
        frame.left = gPokeathlonGoalRollFrameLeft;
        frame.right = gPokeathlonGoalRollFrameRight;
        if (frame.side == 0) {
            ball->direction = frame.left;
        } else {
            ball->direction = frame.right;
        }
    }
    VEC_Normalize(&ball->direction, &ball->direction);
    VEC_MultAdd(ball->speed << 12, &ball->direction, (const VecFx32 *)&frame.zero, &ball->velocity);
    if (other->lastKicker < GOAL_ROLL_NO_MON) {
        ball->lastKicker = other->lastKicker;
    }
}

// A ball that crossed the goal line of a corner scores for the team that touched it last.
void PokeathlonGoalRoll_CheckGoals(PokeathlonCourseData *course) {
    PokeathlonGoalRollMon(*teams)[GOAL_ROLL_TEAM_SIZE];
    int k;
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    k = 0;
    PokeathlonGoalRollBall *ball = goalRoll->balls;
    teams = goalRoll->mons;
    for (; k < 2; k++, ball++) {
        if (ball->kind && (ball->state == GOAL_ROLL_BALL_ROLLING || ball->state == GOAL_ROLL_BALL_KICKED)) {
            Point2D point;
            int goal, index, participant, member, points;
            PokeathlonGoalRollMon *kicker;
            point.x = (s32)ball->position.x / 4096;
            point.y = (s32)ball->position.y / 4096;
            goal = PokeathlonGoalRoll_GetBallQuadrant(ball);
            if (!(u8)Point2D_IsLeftOfLine(&gPokeathlonGoalRollGoalLines[goal][0], &gPokeathlonGoalRollGoalLines[goal][1], &point)) {
                continue;
            }
            ball->state = GOAL_ROLL_BALL_SCORED;
            ball->timer = 10;
            index = ball->lastKicker;
            if (index == GOAL_ROLL_NO_MON) {
                continue;
            }
            participant = index / 3;
            member = index % 3;
            kicker = &teams[participant][member];
            if (goal == participant) {
                if (kicker->goals) {
                    kicker->goals--;
                }
                if (goalRoll->scores[ball->lastKicker / 3]) {
                    goalRoll->scores[ball->lastKicker / 3]--;
                }
                index = ball->lastKicker;
                PokeathlonCourse_AddMonCounter(course, (u8)(index / 3), (u8)(index % 3), 8, 1);
                index = ball->lastKicker;
                PokeathlonCourse_AddMonCounter(course, (u8)(index / 3), (u8)(index % 3), 1, 1);
            } else {
                points = 0;
                if (ball->kind == GOAL_ROLL_BALL_SMALL) {
                    points = 1;
                } else if (ball->kind == GOAL_ROLL_BALL_BIG) {
                    points = 2;
                } else {
                    GF_AssertFail();
                }
                if (kicker->goals + points <= 99) {
                    kicker->goals += points;
                }
                if (points + goalRoll->scores[ball->lastKicker / 3] <= 99) {
                    goalRoll->scores[ball->lastKicker / 3] += points;
                }
                if (goalRoll->scores[goal]) {
                    goalRoll->scores[goal]--;
                }
                index = ball->lastKicker;
                PokeathlonCourse_AddMonCounter(course, (u8)(index / 3), (u8)(index % 3), 3, 1);
            }
        }
    }
}

#pragma force_active reset

// Pokemon low on power walk slowly.
void PokeathlonGoalRoll_UpdateTiredMons(PokeathlonCourseData *course) {
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 i;
    int tired = GOAL_ROLL_MODE_TIRED;

    for (i = 0; i < 12; i++) {
        int participant = i / 3;
        int member = i % 3;
        PokeathlonGoalRollMon *mon = &goalRoll->mons[participant][member];
        u32 mode = mon->mode;
        if (mode != GOAL_ROLL_MODE_STUNNED) {
            if (mon->power < 30) {
                mon->mode = tired;
            } else if (mode != GOAL_ROLL_MODE_DASH) {
                mon->mode = GOAL_ROLL_MODE_WALK;
            }
        }
    }
}

void PokeathlonGoalRoll_SetTouchTarget(int x, int y, VecFx32 *out) {
    out->x = x << 12;
    out->y = y << 12;
    out->z = 0;
}

#pragma force_active on

// The member of `participant`'s team nearest to the touch point within reach, GOAL_ROLL_NO_MON if none.
int PokeathlonGoalRoll_GetTouchedMon(PokeathlonGoalRoll *goalRoll, u8 participant, u8 touchX, u8 touchY) {
    int distances[3];
    u8 inReach[3];
    int distance;
    int reach;
    int nearest;
    u8 i;
    u8 count;
    u8 best;

    reach = PokeathlonCourse_GetTouchRadius();
    count = 0;
    nearest = reach << 12;
    for (i = 0; i < 3; i++) {
        PokeathlonGoalRollMon *mon = &goalRoll->mons[participant][i];
        if (PokeathlonCourse_IsInRange(mon->position.x / FX32_ONE, mon->position.y / FX32_ONE, touchX, touchY, reach, &distance)) {
            distances[i] = distance;
            inReach[i] = TRUE;
            count++;
        } else {
            distances[i] = 0;
            inReach[i] = FALSE;
        }
    }
    if (count == 0) {
        return GOAL_ROLL_NO_MON;
    }
    best = 3;
    for (i = 0; i < 3; i++) {
        if (inReach[i] && distances[i] < nearest) {
            best = i;
            nearest = distances[i];
        }
    }
    if (best == 3) {
        GF_AssertFail();
        best = GOAL_ROLL_NO_MON;
    }
    return best;
}

#pragma force_active reset

BOOL PokeathlonGoalRoll_CirclesOverlap(const VecFx32 *a, fx32 radiusA, const VecFx32 *b, fx32 radiusB) {
    VecFx32 delta;
    VEC_Subtract(a, b, &delta);
    return VEC_Mag(&delta) <= radiusA + radiusB;
}

#pragma force_active on

void PokeathlonGoalRoll_ResetBall(PokeathlonGoalRollBall *ball) {
    VecFx32 zero = { 0, 0, 0 };
    u8 i;
    ball->velocity = zero;
    ball->direction = zero;
    ball->kind = GOAL_ROLL_BALL_NONE;
    ball->state = GOAL_ROLL_BALL_WAITING;
    ball->timer = 0;
    ball->lastKicker = GOAL_ROLL_NO_MON;
    ball->step = 0;
    ball->steps = 0;
    ball->speedTable = 0;
    ball->speed = 0;
    ball->speedRow = 0;
    ball->wait = 0;
    ball->strength = 0;
    ball->kickLevel = 0;
    for (i = 0; i < 12; i++) {
        ball->touching[i] = FALSE;
    }
}

// velocity = direction * speed
void PokeathlonGoalRoll_SetBallDirection(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollBall *ball, const VecFx32 *direction) {
    VecFx32 zero = { 0, 0, 0 };
    VEC_Normalize(direction, &ball->direction);
    ball->velocity = ball->direction;
    VEC_MultAdd(ball->speed << 12, &ball->velocity, &zero, &ball->velocity);
}

#pragma force_active reset

fx32 PokeathlonGoalRoll_CalcNormalizedDot(const VecFx32 *a, const VecFx32 *b) {
    VecFx32 normA, normB;
    VEC_Normalize(a, &normA);
    VEC_Normalize(b, &normB);
    return VEC_DotProduct(&normA, &normB);
}

BOOL PokeathlonGoalRoll_IsWithinAngle(const VecFx32 *a, const VecFx32 *b, u32 angle) {
    fx32 dot = PokeathlonGoalRoll_CalcNormalizedDot(a, b);
    return dot >= FX_CosIdx(angle);
}

void PokeathlonGoalRoll_RecoverPower(PokeathlonGoalRollMon *mon) {
    mon->recoverTimer++;
    if (mon->recoverTimer >= 10) {
        mon->recoverTimer = 0;
        mon->power += 2;
        if (mon->power > mon->maxPower) {
            mon->power = mon->maxPower;
        }
    }
}

void PokeathlonGoalRoll_InitMonStats(PokeathlonCourseData *course, PokeathlonGoalRollMonParams *params, u8 participant, u8 member, PokeathlonGoalRollMon *mon) {
    u8 *levels = PokeathlonCourse_GetMonStats(course, participant, member);
    mon->kickAngle = params->kickAngle[levels[4]];
    mon->unk6E = params->unk6E[levels[4]];
    mon->maxPower = params->power[levels[3]];
    mon->power = params->power[levels[3]];
    mon->walkSpeed = (params->walkSpeed[levels[1]] << 12) / 10;
    mon->dashSpeed = (params->dashSpeed[levels[1]] << 12) / 10;
    mon->kickPower = params->kickPower[levels[0]];
    mon->kickRow = levels[4];
    mon->kickLevel = levels[0];
    mon->unk74 = levels[3];
}

void PokeathlonGoalRoll_InitWindows(PokeathlonGoalRoll *goalRoll) {
    u32 i;
    AddWindow(goalRoll->bgConfig, &goalRoll->scoreWindow, &gPokeathlonGoalRollWindowData.window);
    for (i = 0; (s32)i < 4; i++) {
        AddWindow(goalRoll->bgConfig, &goalRoll->nameWindows[i], &gPokeathlonGoalRollNameWindowTemplates[i]);
    }
    BG_FillCharDataRange(goalRoll->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    BG_FillCharDataRange(goalRoll->bgConfig, GF_BG_LYR_MAIN_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, goalRoll->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_4, 19, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, 0x20, goalRoll->heapId);
}

void PokeathlonGoalRoll_PrintTotalPoints(PokeathlonGoalRoll *goalRoll, u32 score) {
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    FillWindowPixelBuffer(&goalRoll->scoreWindow, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, goalRoll->heapId);
    msgFormat = MessageFormat_New(goalRoll->heapId);
    BufferIntegerAsString(msgFormat, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0309_00165, goalRoll->heapId);
    AddTextPrinterParameterizedWithColor(&goalRoll->scoreWindow, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&goalRoll->scoreWindow);
}

// whole pixels to fx32
#define TO_FX32(x) ((s32)((u32)(x) << 12))

#pragma force_active on

// Keeps a circle of `radius` that moved from `previous` to `position` inside the field: the four diagonal corner
// walls (only with `checkCorners`) and the straight edges. Returns the wall hit (1 left, 2 right,
// 3 top, 4 bottom, 5..8 the corners top left, top right, bottom left, bottom right; 0 none) and
// the corrected position in `out`. The field is 256 x 184 with its centre at (128, 112).
int PokeathlonGoalRoll_CollideWithWalls(VecFx32 *position, VecFx32 *previous, s32 radius, int checkCorners, VecFx32 *out) {
    VecFx32 edgeStart = { 0 }, edgeEnd = { 0 }, hit, normal, cornerStart = { 0 }, cornerEnd = { 0 };
    VecFx32 normalTopLeft = gPokeathlonGoalRollNormalTopLeft, normalTopRight = gPokeathlonGoalRollNormalTopRight, normalBottomLeft = gPokeathlonGoalRollNormalBottomLeft, normalBottomRight = gPokeathlonGoalRollNormalBottomRight;
    Point2D cornerA, cornerB, previousPoint, positionPoint, crossing;
    out->x = 0;
    out->y = 0;
    out->z = 0;
    previousPoint.x = previous->x / 4096;
    previousPoint.y = previous->y / 4096;
    positionPoint.x = position->x / 4096;
    positionPoint.y = position->y / 4096;
    u32 quadrant = 0;
    if (position->y < 0x70000) {
        if (position->x >= 0x80000) {
            quadrant = 1;
        }
    } else if (position->x < 0x80000) {
        quadrant = 2;
    } else {
        quadrant = 3;
    }
    int result = 0;
    u8 beyondCorner = 0, crossesCorner = 0;
    if (!checkCorners) {
        goto edges;
    }
    {
        switch (quadrant) {
        case 0:
            cornerStart.x = -0x20000;
            cornerStart.y = 0x70000;
            cornerEnd.x = 0x60000;
            cornerEnd.y = -0x10000;
            VEC_Normalize(&normalTopLeft, &normal);
            result = 5;
            break;
        case 1:
            cornerStart.x = 0xA0000;
            cornerStart.y = -0x10000;
            cornerEnd.x = 0x120000;
            cornerEnd.y = 0x70000;
            VEC_Normalize(&normalTopRight, &normal);
            result = 6;
            break;
        case 2:
            cornerStart.x = 0x60000;
            cornerStart.y = 0xE0000;
            cornerEnd.x = -0x20000;
            cornerEnd.y = 0x60000;
            VEC_Normalize(&normalBottomLeft, &normal);
            result = 7;
            break;
        case 3:
            cornerStart.x = 0x120000;
            cornerStart.y = 0x60000;
            cornerEnd.x = 0xA0000;
            cornerEnd.y = 0xE0000;
            VEC_Normalize(&normalBottomRight, &normal);
            result = 8;
            break;
        default:
            GF_AssertFail();
            break;
        }
        if (result) {
            VEC_MultAdd(TO_FX32(radius), &normal, &cornerStart, &cornerStart);
            VEC_MultAdd(TO_FX32(radius), &normal, &cornerEnd, &cornerEnd);
            cornerA.x = cornerStart.x / 4096;
            cornerA.y = cornerStart.y / 4096;
            cornerB.x = cornerEnd.x / 4096;
            cornerB.y = cornerEnd.y / 4096;
            if (Point2D_CalcSegmentIntersection(&cornerA, &cornerB, &previousPoint, &positionPoint, &crossing)) {
                if (!Point2D_IsLeftOfLine(&cornerA, &cornerB, &crossing)) {
                    *out = *previous;
                    beyondCorner = 1;
                } else {
                    out->x = TO_FX32(crossing.x);
                    out->y = TO_FX32(crossing.y);
                }
                crossesCorner = 1;
            } else {
                result = 0;
            }
        }
    }
edges:
    if (position->y < 0x70000) {
        s32 edge = (s32)((u32)radius + 16);
        if (position->y < TO_FX32(edge) && previous->y == TO_FX32((s32)(16U - (u32)radius))) {
            *out = *previous;
            return 3;
        }
        edgeStart.x = 0x8000;
        edgeEnd.x = 0xF7000;
        edgeEnd.y = edgeStart.y = TO_FX32(edge);
        if (PokeathlonGoalRoll_IntersectWall(&edgeStart, &edgeEnd, previous, position, &hit)) {
            if (beyondCorner) {
                return result;
            }
            if (crossesCorner && out->y >= hit.y) {
                return result;
            }
            if (checkCorners && PokeathlonGoalRoll_SegmentsIntersect(&cornerA, &cornerB, &previousPoint, &hit)) {
                *out = *previous;
                return 3;
            }
            *out = hit;
            return 3;
        }
        if (position->x < 0x80000) {
            radius = (s32)((u32)radius + 8);
            if (position->x < TO_FX32(radius) && previous->x == TO_FX32(radius)) {
                *out = *previous;
                return 1;
            }
            edgeEnd.x = edgeStart.x = TO_FX32(radius);
            edgeStart.y = 0xB7000;
            edgeEnd.y = 0x10000;
            if (PokeathlonGoalRoll_IntersectWall(&edgeStart, &edgeEnd, previous, position, &hit)) {
                if (beyondCorner) {
                    return result;
                }
                if (crossesCorner && out->x >= hit.x) {
                    return result;
                }
                if (checkCorners && PokeathlonGoalRoll_SegmentsIntersect(&cornerA, &cornerB, &previousPoint, &hit)) {
                    *out = *previous;
                    return 1;
                }
                *out = hit;
                return 1;
            }
        } else {
            s32 edge = (s32)(247U - (u32)radius);
            if (position->x > TO_FX32(edge) && previous->x == TO_FX32(edge)) {
                *out = *previous;
                return 2;
            }
            edgeEnd.x = edgeStart.x = TO_FX32(edge);
            edgeStart.y = 0x10000;
            edgeEnd.y = 0xB7000;
            if (PokeathlonGoalRoll_IntersectWall(&edgeStart, &edgeEnd, previous, position, &hit)) {
                if (beyondCorner) {
                    return result;
                }
                if (crossesCorner && out->x <= hit.x) {
                    return result;
                }
                if (checkCorners && PokeathlonGoalRoll_SegmentsIntersect(&cornerA, &cornerB, &previousPoint, &hit)) {
                    *out = *previous;
                    return 2;
                }
                *out = hit;
                return 2;
            }
        }
    } else {
        s32 edge = (s32)(183U - (u32)radius);
        if (position->y > TO_FX32(edge) && previous->y == TO_FX32(edge)) {
            *out = *previous;
            return 4;
        }
        edgeStart.x = 0xF7000;
        edgeEnd.x = 0x8000;
        edgeEnd.y = edgeStart.y = TO_FX32(edge);
        if (PokeathlonGoalRoll_IntersectWall(&edgeStart, &edgeEnd, previous, position, &hit)) {
            if (beyondCorner) {
                return result;
            }
            if (crossesCorner && out->y <= hit.y) {
                return result;
            }
            if (checkCorners && PokeathlonGoalRoll_SegmentsIntersect(&cornerA, &cornerB, &previousPoint, &hit)) {
                *out = *previous;
                return 4;
            }
            *out = hit;
            return 4;
        }
        if (position->x < 0x80000) {
            radius = (s32)((u32)radius + 8);
            if (position->x < TO_FX32(radius) && previous->x == TO_FX32(radius)) {
                *out = *previous;
                return 1;
            }
            edgeEnd.x = edgeStart.x = TO_FX32(radius);
            edgeStart.y = 0xB7000;
            edgeEnd.y = 0x10000;
            if (PokeathlonGoalRoll_IntersectWall(&edgeStart, &edgeEnd, previous, position, &hit)) {
                if (beyondCorner) {
                    return result;
                }
                if (crossesCorner && out->x >= hit.x) {
                    return result;
                }
                if (checkCorners && PokeathlonGoalRoll_SegmentsIntersect(&cornerA, &cornerB, &previousPoint, &hit)) {
                    *out = *previous;
                    return 1;
                }
                *out = hit;
                return 1;
            }
        } else {
            s32 edge = (s32)(247U - (u32)radius);
            if (position->x > TO_FX32(edge) && previous->x == TO_FX32(edge)) {
                *out = *previous;
                return 2;
            }
            edgeEnd.x = edgeStart.x = TO_FX32(edge);
            edgeStart.y = 0x10000;
            edgeEnd.y = 0xB7000;
            if (PokeathlonGoalRoll_IntersectWall(&edgeStart, &edgeEnd, previous, position, &hit)) {
                if (beyondCorner) {
                    return result;
                }
                if (crossesCorner && out->x <= hit.x) {
                    return result;
                }
                if (checkCorners && PokeathlonGoalRoll_SegmentsIntersect(&cornerA, &cornerB, &previousPoint, &hit)) {
                    *out = *previous;
                    return 2;
                }
                *out = hit;
                return 2;
            }
        }
    }
    return result;
}

#pragma force_active reset

BOOL PokeathlonGoalRoll_SegmentsIntersect(Point2D *a, Point2D *b, Point2D *c, const VecFx32 *point) {
    Point2D d;
    Point2D out;
    d.x = point->x / FX32_ONE;
    d.y = point->y / FX32_ONE;
    return Point2D_CalcSegmentIntersection(a, b, c, &d, &out);
}

#pragma force_active on

// Where the segment c-d crosses the axis-aligned wall a-b (into `out`); FALSE if it doesn't.
BOOL PokeathlonGoalRoll_IntersectWall(const VecFx32 *a, const VecFx32 *b, const VecFx32 *c, const VecFx32 *d, VecFx32 *out) {
    VecFx32 point = { 0, 0, 0 };
    VecFx32 direction;
    Point2D pointA, pointB, pointC, pointD;
    s32 distC, distD;
    pointA.x = a->x / 4096;
    pointA.y = a->y / 4096;
    pointB.x = b->x / 4096;
    pointB.y = b->y / 4096;
    pointC.x = c->x / 4096;
    pointC.y = c->y / 4096;
    pointD.x = d->x / 4096;
    pointD.y = d->y / 4096;
    if (!Point2D_SegmentsIntersect(&pointA, &pointB, &pointC, &pointD)) {
        return FALSE;
    }
    VEC_Subtract(b, a, &direction);
    VEC_Normalize(&direction, &direction);
    // the absolute values are taken by multiplying with -1 (as built)
    if (a->y == b->y) {
        distC = (s32)((u32)a->y - (u32)c->y);
        if (distC < 0) {
            u32 minusOne = 0;
            minusOne = ~minusOne;
            distC = (s32)((u32)distC * minusOne);
        };
        distD = (s32)((u32)a->y - (u32)d->y);
        if (distD < 0) {
            u32 minusOne = 0;
            minusOne = ~minusOne;
            distD = (s32)((u32)distD * minusOne);
        };
        if (!distD) {
            return FALSE;
        }
        fx32 x = c->x;
        point.y = a->y;
        point.x = x;
        VEC_MultAdd(FX_Div(distC, (s32)((u32)distC + (u32)distD)), &direction, &point, out);
        return TRUE;
    }
    if (a->x == b->x) {
        distC = (s32)((u32)a->x - (u32)c->x);
        if (distC < 0) {
            u32 minusOne = 0;
            minusOne = ~minusOne;
            distC = (s32)((u32)distC * minusOne);
        };
        distD = (s32)((u32)a->x - (u32)d->x);
        if (distD < 0) {
            u32 minusOne = 0;
            minusOne = ~minusOne;
            distD = (s32)((u32)distD * minusOne);
        };
        if (!distD) {
            return FALSE;
        }
        point.x = a->x;
        point.y = c->y;
        VEC_MultAdd(FX_Div(distC, (s32)((u32)distC + (u32)distD)), &direction, &point, out);
        return TRUE;
    }
    GF_ASSERT(FALSE);
    return FALSE;
}

#pragma force_active reset

// Places a scored ball at a random one of the 15 spawn points not used by either ball.
void PokeathlonGoalRoll_PickBallSpawn(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollBall *ball) {
    u8 candidates[16];
    u8 count = 0;
    s32 i;
    for (i = 0; i < 15; i++) {
        if (goalRoll->balls[0].spawnPoint != i && goalRoll->balls[1].spawnPoint != i) {
            candidates[count++] = i;
        }
    }
    {
        u8 selected = candidates[(u8)(LCRandom() % count)];
        ball->position.x = gPokeathlonGoalRollBallSpawnPoints[selected * 2] << 12;
        ball->position.y = ov96_0221D43A[selected * 2] << 12;
        ball->spawnPoint = selected;
    }
}

// The y of a ball in the frame packet: whole pixels; 0xC0.. a ball waiting to appear at one of
// the three spawn rows (+ its wait timer); 0xD8 + 4 * goal + team a ball in a goal.
u8 PokeathlonGoalRoll_EncodeBallY(PokeathlonGoalRollBall *ball) {
    s32 y;
    s32 result;
    if (ball->state == GOAL_ROLL_BALL_WAITING) {
        if (ball->position.y == 0x58000) {
            result = ball->wait + 0xC0;
        } else if (ball->position.y == 0x68000) {
            result = ball->wait + 0xC8;
        } else if (ball->position.y == 0x78000) {
            result = ball->wait + 0xD0;
        } else {
            GF_AssertFail();
            y = ball->position.y;
            result = y / FX32_ONE;
        }
    } else if (ball->state == GOAL_ROLL_BALL_SCORED) {
        if (ball->lastKicker == GOAL_ROLL_NO_MON) {
            y = ball->position.y;
            result = y / FX32_ONE;
        } else {
            u32 goal = PokeathlonGoalRoll_GetBallQuadrant(ball);
            result = PokeathlonGoalRoll_GetGoalCode(goal, (u8)(ball->lastKicker / 3));
        }
    } else {
        y = ball->position.y;
        result = y / FX32_ONE;
    }
    return result;
}

s32 PokeathlonGoalRoll_DecodeBallY(s32 code) {
    u8 frame;
    s32 result;
    if (code < 0xC0) {
        return code << 12;
    }
    if (code < 0xD8) {
        code -= 0xC0;
        frame = code % 8;
        if (code < 8) {
            result = gPokeathlonGoalRollBounceOffsets[frame] + 0x58;
        } else if (code < 0x10) {
            result = gPokeathlonGoalRollBounceOffsets[frame] + 0x68;
        } else {
            result = gPokeathlonGoalRollBounceOffsets[frame] + 0x78;
        }
        return result << 12;
    }
    if (code < 0xE8) {
        return code << 12;
    }
    GF_AssertFail();
    return code << 12;
}

void PokeathlonGoalRoll_NextFacing(PokeathlonGoalRollMon *mon) {
    switch (mon->facing) {
    case 1:
        mon->facing = 4;
        break;
    case 4:
        mon->facing = 2;
        break;
    case 2:
        mon->facing = 3;
        break;
    case 3:
        mon->facing = 1;
        break;
    }
}

void PokeathlonGoalRoll_PlayGoalAreaSE(PokeathlonGoalRoll *goalRoll, u32 ballIndex, BOOL bigBall, s32 code) {
    if (code < 0xC0) {
        goalRoll->ballInGoalArea[ballIndex] = 0;
        return;
    }
    if (code < 0xD8) {
        if (goalRoll->ballInGoalArea[ballIndex] == 0) {
            goalRoll->ballInGoalArea[ballIndex] = 1;
            if (ballIndex == 0 && bigBall != 0) {
                PlaySE(SEQ_SE_GS_APPEAR);
                return;
            }
            PlaySE(SEQ_SE_GS_THLON09);
        }
        return;
    }
    goalRoll->ballInGoalArea[ballIndex] = 0;
}

#pragma force_active on

void PokeathlonGoalRoll_PrintPlate(PokeathlonGoalRoll *goalRoll, u8 participant, int mode) {
    String *string;
    u32 color;
    FillWindowPixelBuffer(&goalRoll->nameWindows[participant], 0);
    if (mode != 0) {
        if (mode == 2) {
            string = goalRoll->plateStrings[0];
        } else if (mode == 1) {
            string = goalRoll->plateStrings[1];
        } else if (mode == 3) {
            string = goalRoll->plateStrings[2];
        } else {
            GF_AssertFail();
            string = goalRoll->plateStrings[0];
        }
        AddTextPrinterParameterizedWithColor(&goalRoll->nameWindows[participant], 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    } else {
        if (goalRoll->leading[participant]) {
            color = MAKE_TEXT_COLOR(3, 4, 0);
        } else {
            color = MAKE_TEXT_COLOR(1, 2, 0);
        }
        AddTextPrinterParameterizedWithColor(&goalRoll->nameWindows[participant], 4, goalRoll->nameStrings[participant], 0, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
    }
    CopyWindowToVram(&goalRoll->nameWindows[participant]);
}

#pragma force_active reset

void PokeathlonGoalRoll_LoadStrings(PokeathlonCourseData *course) {
    u8 i;
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, goalRoll->heapId);
    MessageFormat *msgFormat = MessageFormat_New(goalRoll->heapId);
    goalRoll->plateStrings[0] = NewString_ReadMsgData(msgData, msg_0309_00307);
    goalRoll->plateStrings[1] = NewString_ReadMsgData(msgData, msg_0309_00308);
    goalRoll->plateStrings[2] = NewString_ReadMsgData(msgData, msg_0309_00309);
    for (i = 0; i < 4; i++) {
        String *name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, i), goalRoll->heapId);
        BufferString(msgFormat, 0, name, 2, 1, 2);
        goalRoll->nameStrings[i] = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0309_00306, goalRoll->heapId);
        String_Delete(name);
    }
    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);
}

void PokeathlonGoalRoll_FreeStrings(PokeathlonCourseData *course) {
    u8 i;
    PokeathlonGoalRoll *goalRoll = PokeathlonCourse_GetHeapAllocPtr4(course);
    String_Delete(goalRoll->plateStrings[0]);
    String_Delete(goalRoll->plateStrings[1]);
    String_Delete(goalRoll->plateStrings[2]);
    for (i = 0; i < 4; i++) {
        String_Delete(goalRoll->nameStrings[i]);
    }
}

void PokeathlonGoalRoll_CreatePlateTasks(PokeathlonGoalRoll *goalRoll) {
    u8 i = 0;
    do {
        PokeathlonGoalRollPlate *plate = &goalRoll->plates[i];
        plate->state = 0;
        plate->id = i;
        plate->timer = 0;
        plate->unk03 = -1;
        plate->owner = goalRoll;
        plate->task = SysTask_CreateOnMainQueue((SysTaskFunc)PokeathlonGoalRoll_PlateTask, plate, 1);
        i++;
    } while (i < 4);
}

void PokeathlonGoalRoll_DestroyPlateTasks(PokeathlonGoalRoll *goalRoll) {
    u8 i = 0;
    do {
        SysTask_Destroy(goalRoll->plates[i].task);
        i++;
    } while (i < 4);
}
