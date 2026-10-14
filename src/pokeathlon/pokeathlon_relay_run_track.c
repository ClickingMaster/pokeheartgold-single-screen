#include "global.h"

#include "constants/sndseq.h"

#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_relay_run.h"

#include "assert.h"
#include "bg_window.h"
#include "geometry_math.h"
#include "math_util.h"
#include "sprite.h"

// Sprite pool helpers (pokeathlon_course_objects.c), declared with untyped entries as the course objects use them.
void PokeathlonCourseSpritePoolSprite_Show(PokeathlonCourseSpritePoolSprite *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
BOOL PokeathlonCourseSpritePoolSprite_TryChangeAnim(PokeathlonCourseSpritePoolSprite *entry, int seq);
void PokeathlonCourseSpritePoolSprite_SetPosition(PokeathlonCourseSpritePoolSprite *entry, VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(PokeathlonCourseSpritePoolSprite *entry);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(PokeathlonCourseSpritePoolSprite *entry, int priority);
void PokeathlonCourseObject_SetGroundPosition(PokeathlonCourseObject *object, s32 x, s32 y);
void PokeathlonCourseObject_GroundToCenter(void *object, s32 x, s32 y, s32 *outX, s32 *outY);
void PokeathlonCourseObject_GetCollisionCircle(void *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY, s32 *radius);

// NitroSDK fixed-point square root, declared by no general header.
fx32 FX_Sqrt(fx32 x);

BOOL PlaySEOnPlayer(u16 seqNo, int playerNo);
u32 GF_SndCountPlayingSeqOnPlayer(u32 playerNo);

extern const PokeathlonRelayRunHitTable sRelayRunEdgeHits; // edges: RELAY_RUN_HIT_TOP, _RIGHT, _BOTTOM, _LEFT
extern const PokeathlonRelayRunHitTable sRelayRunCornerHits; // corners: RELAY_RUN_HIT_TOP_LEFT ..

// Draws every team from the packet the host sent, scrolled so the player's own runner (team own) stands at
// RELAY_RUN_RUNNER_X: positions, member swaps, dash lines, tired / exhausted marks and the bump mark.
#pragma force_active on

void PokeathlonRelayRun_DrawTeams(PokeathlonCourseData *course, int own, PokeathlonRelayRunTeam *team, PokeathlonRelayRunLink *link) {
    int x;
    int dx;
    int screenX;
    PokeathlonRelayRun *relayRun;
    PokeathlonRelayRunPacket *packet;
    PokeathlonCourseMonObject *monObject;
    u8 swapping;
    u8 speedGrade;
    int condition;
    u8 runner;
    u8 member;
    int i;
    int isHost;
    int isOwn;
    int bumpX;
    u32 teamX;
    PokeathlonRelayRunTouch *touch;
    int isCpu;
    BOOL running;
    BOOL exhausted;
    u8 bench0;
    u8 bench1;
    s32 flags;

    packet = (PokeathlonRelayRunPacket *)PokeathlonCourse_ToBytes(&link->packet);
    relayRun = PokeathlonCourse_GetHeapAllocPtr4(course);
    x = packet->x[own];
    {
        VecFx32 position = { 0, 0, 0 };
        if ((u8)((packet->flags >> 25) & 1)) {
            bumpX = x - packet->bumpX;
            if (bumpX >= 768.0) {
                bumpX = 80.0 + (1024.0 - bumpX);
            } else {
                bumpX = RELAY_RUN_RUNNER_X - bumpX;
            }
            position.x = bumpX << FX32_SHIFT;
            position.y = packet->bumpY << FX32_SHIFT;
            PokeathlonCourseSpritePoolSprite_SetPosition(relayRun->bumpMarks[relayRun->nextBumpMark], &position);
            PokeathlonCourseSpritePoolSprite_Show(relayRun->bumpMarks[relayRun->nextBumpMark], TRUE, TRUE);
            PokeathlonCourseSpritePoolSprite_SetAnim(relayRun->bumpMarks[relayRun->nextBumpMark], 9);
            relayRun->nextBumpMark = (relayRun->nextBumpMark + 1) % 4;
            if (bumpX >= 0 && bumpX < 256) {
                PlaySEOnPlayer(SEQ_SE_GS_THLON06, PLAYER_SE_3);
            }
        }
    }
    isHost = own == 0;
    for (i = 0; i < RELAY_RUN_TEAMS; i++) {
        VecFx32 position;
        VecFx32 above;

        isOwn = own == i;
        runner = packet->runner[i];
        member = runner & 3;
        teamX = packet->x[i];
        dx = (int)teamX - x;
        // the track wraps: a runner just past the start line is just ahead of one about to cross it
        if (x > 724.0) {
            if (teamX < 300) {
                dx = (1024.0 - x) + teamX;
            }
        } else if (x < 100 && teamX > 924.0) {
            dx = 0.0 - (x + (1024.0 - teamX));
        }
        screenX = dx + RELAY_RUN_RUNNER_X;
        swapping = (packet->laps[i] >> 7) & 1;
        condition = (packet->flags >> ((u8)((u32)member + i * RELAY_RUN_MEMBERS) * 2)) & 3;
        speedGrade = (runner >> 2) & 3;
        monObject = team->monObjects[member];
        position.x = screenX << FX32_SHIFT;
        position.y = packet->y[i] << FX32_SHIFT;
        position.z = 0;
        if (PokeathlonCourse_GetParticipantCount(course) <= i) {
            isCpu = TRUE;
            touch = (PokeathlonRelayRunTouch *)PokeathlonCourse_ToBytes(&link->touches[i]);
        } else {
            isCpu = FALSE;
            touch = (PokeathlonRelayRunTouch *)PokeathlonCourse_ToBytes(link);
        }
        if (own == i) {
            PokeathlonRelayRunTeam_DrawSwap(relayRun->hud, team, &position, packet, i, (u8)member, touch, isHost, TRUE, FALSE);
        } else {
            PokeathlonRelayRunTeam_DrawSwap(relayRun->hud, team, &position, packet, i, (u8)member, touch, isHost, FALSE, isCpu);
        }
        PokeathlonRelayRunHud_UpdateTeam(relayRun->hud, i, (u8)member, swapping, condition);
        if (team->runnerHidden == FALSE && screenX >= -32 && screenX <= 288) {
            PokeathlonCourseMonObject_SetDrawFlag(monObject, TRUE);
        } else {
            PokeathlonCourseMonObject_SetDrawFlag(monObject, FALSE);
        }
        for (int j = 0; j < RELAY_RUN_MEMBERS; j++) {
            PokeathlonCourseObject_SetGroundPosition(team->monObjects[j], screenX, packet->y[i]);
        }
        PokeathlonRelayRunTeam_SetDrawPriority(team, packet->y[i], i == PokeathlonCourse_GetNetId(course));
        PokeathlonRelayRun_PlaceAboveMon(PokeathlonCourse_GetPerformance(course, i, member), &position, team->conditionSprite);
        above = position;
        above.y -= FX32_CONST(24);
        PokeathlonCourseSpritePoolSprite_SetPosition(team->speedSprite, &above);
        if (!swapping) {
            running = FALSE;
            exhausted = FALSE;
            switch (speedGrade) {
            case 2:
                if (speedGrade != team->lastSpeedGrade) {
                    PokeathlonCourseSpritePoolSprite_Show(team->speedSprite, TRUE, TRUE);
                    PokeathlonCourseSpritePoolSprite_SetAnim(team->speedSprite, 4);
                }
                PokeathlonCourseMonObject_SetAnim(monObject, 8);
                running = TRUE;
                if (team->speedSoundGrade <= 1 && team->speedSoundPlayed == FALSE) {
                    PokeathlonRelayRun_PlaySEIfOnScreen(screenX, SEQ_SE_GS_FW015_2, isOwn);
                    team->speedSoundGrade = 2;
                    team->speedSoundPlayed = TRUE;
                }
                break;
            case 3:
                if (speedGrade != team->lastSpeedGrade) {
                    PokeathlonCourseSpritePoolSprite_Show(team->speedSprite, TRUE, TRUE);
                    PokeathlonCourseSpritePoolSprite_SetAnim(team->speedSprite, 5);
                }
                PokeathlonCourseMonObject_SetAnim(monObject, 8);
                running = TRUE;
                if (team->speedSoundGrade <= 1 && team->speedSoundPlayed == FALSE) {
                    PokeathlonRelayRun_PlaySEIfOnScreen(screenX, SEQ_SE_GS_FW015_2, isOwn);
                    team->speedSoundGrade = 3;
                    team->speedSoundPlayed = TRUE;
                }
                break;
            default:
                PokeathlonCourseSpritePoolSprite_Show(team->speedSprite, TRUE, FALSE);
                team->speedSoundGrade = speedGrade;
                team->speedSoundPlayed = FALSE;
                break;
            }
            switch (condition) {
            case RELAY_RUN_TIRED:
                PokeathlonCourseSpritePoolSprite_Show(team->conditionSprite, TRUE, TRUE);
                PokeathlonCourseSpritePoolSprite_TryChangeAnim(team->conditionSprite, 6);
                break;
            case RELAY_RUN_EXHAUSTED:
                if (condition != team->lastCondition) {
                    PokeathlonCourseSpritePoolSprite_Show(team->conditionSprite, TRUE, TRUE);
                    PokeathlonCourseSpritePoolSprite_SetAnim(team->conditionSprite, 7);
                }
                PokeathlonCourseMonObject_SetAnim(monObject, 21);
                exhausted = TRUE;
                if (isOwn) {
                    if (!GF_SndCountPlayingSeqOnPlayer(PLAYER_SE_1)) {
                        PokeathlonRelayRun_PlaySEIfOnScreen(screenX, SEQ_SE_GS_W146, isOwn);
                    }
                } else {
                    if (!GF_SndCountPlayingSeqOnPlayer(PLAYER_SE_2)) {
                        PokeathlonRelayRun_PlaySEIfOnScreen(screenX, SEQ_SE_GS_W146, isOwn);
                    }
                }
                break;
            default:
                PokeathlonCourseSpritePoolSprite_Show(team->conditionSprite, TRUE, FALSE);
                break;
            }
            if (!running && !exhausted) {
                PokeathlonCourseMonObject_SetAnim(monObject, 0);
            }
        } else {
            PokeathlonCourseSpritePoolSprite_Show(team->speedSprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_Show(team->conditionSprite, TRUE, FALSE);
        }
        team->lastSpeedGrade = speedGrade;
        team->lastCondition = condition;
        team++;
    }
    bench0 = PokeathlonRelayRunHud_GetBenchMember(relayRun->hud, 0);
    bench1 = PokeathlonRelayRunHud_GetBenchMember(relayRun->hud, 1);
    flags = packet->flags;
    PokeathlonRelayRunHud_ShowTiredMarks(relayRun->hud, (flags >> ((u8)(bench0 + own * RELAY_RUN_MEMBERS) * 2)) & 3, (flags >> ((u8)(bench1 + own * RELAY_RUN_MEMBERS) * 2)) & 3);
}

#pragma force_active reset

// Places the obstacle sprites for the track scrolled to x (the player's own runner).
void PokeathlonRelayRun_ScrollObstacles(PokeathlonRelayRun *relayRun, int x) {
    u8 i;

    for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
        // obstacle i through relayRun moved by i obstacles (shifted->obstacles[0]), the way the original addresses it
        PokeathlonRelayRun *shifted = (PokeathlonRelayRun *)((u8 *)relayRun + i * sizeof(PokeathlonRelayRunObstacle));
        VecFx32 position;
        int screenX;
        u8 y;

        if (shifted->obstacles[0].active == FALSE) {
            break;
        }
        y = shifted->obstacles[0].y;
        screenX = x - shifted->obstacles[0].x;
        if ((double)screenX >= 768.0) {
            screenX = (int)(80.0 + (1024.0 - (double)screenX));
        } else if ((double)screenX <= -768.0) {
            screenX = (int)(80.0 - (1024.0 + (double)screenX));
        } else {
            screenX = RELAY_RUN_RUNNER_X - screenX;
        }
        position.z = 0;
        position.x = screenX << FX32_SHIFT;
        position.y = (y + 32) << FX32_SHIFT;
        PokeathlonCourseSpritePoolSprite_SetPosition(shifted->obstacles[0].sprite, &position);
        if (screenX >= -32 && screenX <= 288) {
            PokeathlonCourseSpritePoolSprite_Show(shifted->obstacles[0].sprite, TRUE, TRUE);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(shifted->obstacles[0].sprite, TRUE, FALSE);
        }
    }
}

// Member swap, first half: the runner leaves (the swap sprite flies up 20 px a frame), then the next member is waited for:
// the player's own team waits for the touch screen to bring up its picture, the host waits a while for the
// teams without a player. Then the swap reaches phase 1 on this screen.
void PokeathlonRelayRunTeam_AnimateLeave(PokeathlonRelayRunHud *hud, PokeathlonRelayRunTeam *team, VecFx32 *position, u8 teamId, u8 member, PokeathlonRelayRunTouch *touch, BOOL isHostForCpu, BOOL isOwn) {
    VecFx32 swapPosition;

    switch (team->swapStep) {
    case 0:
        PokeathlonCourseSpritePoolSprite_SetPosition(team->swapSprite, position);
        PokeathlonCourseSpritePoolSprite_SetPosition(team->shadowSprite, position);
        PokeathlonCourseSpritePoolSprite_Show(team->shadowSprite, TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_SetAnim(team->shadowSprite, 8);
        PokeathlonCourseSpritePoolSprite_Show(team->swapSprite, TRUE, TRUE);
        PokeathlonCourseMonObject_SetDrawFlag(team->monObjects[member], FALSE);
        team->runnerHidden = TRUE;
        team->swapTimer = 0;
        PokeathlonRelayRun_PlaySEIfOnScreen(position->x / FX32_ONE, SEQ_SE_GS_W100, isOwn);
        team->swapStep++;
        break;
    case 1:
        swapPosition = *PokeathlonCourseSpritePoolSprite_GetPosition(team->swapSprite);
        swapPosition.y -= FX32_CONST(20);
        swapPosition.x = position->x;
        PokeathlonCourseSpritePoolSprite_SetPosition(team->swapSprite, &swapPosition);
        PokeathlonCourseSpritePoolSprite_SetPosition(team->shadowSprite, position);
        team->swapTimer++;
        if (team->swapTimer >= 20) {
            team->swapStep++;
        }
        break;
    case 2:
        if (isOwn != FALSE) {
            if (PokeathlonRelayRunHud_RotateMonPics(hud, member)) {
                touch->displayPhase = 1;
                team->swapStep++;
            }
        } else if (isHostForCpu != FALSE) {
            if (PokeathlonRelayRunHud_RotateCpuMonPics(hud, teamId)) {
                touch->displayPhase = 1;
                team->swapStep++;
            }
        } else {
            team->swapStep++;
        }
        break;
    case 3:
        break;
    }
}

// Member swap, second half: the swap sprite drops 400 px onto the track and the next member appears; the swap
// reaches phase 2 on this screen.
void PokeathlonRelayRunTeam_AnimateArrive(PokeathlonRelayRunTeam *team, VecFx32 *position, u8 unused, u8 member, PokeathlonRelayRunTouch *touch, BOOL reportPhase, BOOL isOwn) {
    switch (team->swapStep) {
    case 0: {
        VecFx32 swapPosition;
        swapPosition = *position;
        swapPosition.y -= FX32_CONST(400);
        PokeathlonCourseSpritePoolSprite_SetPosition(team->swapSprite, &swapPosition);
        PokeathlonCourseSpritePoolSprite_Show(team->swapSprite, TRUE, TRUE);
        PokeathlonCourseMonObject_SetDrawFlag(team->monObjects[member], FALSE);
        team->swapTimer = 0;
        PokeathlonRelayRun_PlaySEIfOnScreen(position->x / FX32_ONE, SEQ_SE_GS_NAGERU, isOwn);
        team->swapStep++;
        break;
    }
    case 1: {
        VecFx32 swapPosition;
        swapPosition = *PokeathlonCourseSpritePoolSprite_GetPosition(team->swapSprite);
        swapPosition.y += FX32_CONST(20);
        swapPosition.x = position->x;
        PokeathlonCourseSpritePoolSprite_SetPosition(team->swapSprite, &swapPosition);
        team->swapTimer++;
        if (team->swapTimer >= 20) {
            PokeathlonCourseSpritePoolSprite_SetPosition(team->shadowSprite, position);
            team->swapStep++;
        }
        break;
    }
    case 2:
        if (reportPhase != FALSE) {
            touch->displayPhase = 2;
        }
        PokeathlonCourseSpritePoolSprite_SetPosition(team->swapSprite, position);
        PokeathlonCourseSpritePoolSprite_SetPosition(team->shadowSprite, position);
        team->swapStep++;
        // fallthrough
    case 3:
        PokeathlonCourseSpritePoolSprite_SetPosition(team->swapSprite, position);
        PokeathlonCourseSpritePoolSprite_SetPosition(team->shadowSprite, position);
        break;
    }
}

// The swap is over: the new member runs.
void PokeathlonRelayRunTeam_EndSwap(PokeathlonRelayRunTeam *team, u8 unused, u8 member, PokeathlonRelayRunTouch *touch, BOOL reportPhase, int isOwn) {
    VecFx32 position;
    PokeathlonCourseMonObject_SetDrawFlag(team->monObjects[member], TRUE);
    team->runnerHidden = FALSE;
    PokeathlonCourseSpritePoolSprite_Show(team->shadowSprite, TRUE, TRUE);
    PokeathlonCourseSpritePoolSprite_SetAnim(team->shadowSprite, 8);
    PokeathlonCourseSpritePoolSprite_Show(team->swapSprite, TRUE, FALSE);
    if (reportPhase) {
        touch->displayPhase = 0;
    }
    position = *PokeathlonCourseSpritePoolSprite_GetPosition(team->shadowSprite);
    PokeathlonRelayRun_PlaySEIfOnScreen(position.x / FX32_ONE, SEQ_SE_GS_BOWA2, isOwn);
}

// Plays the member swap of a team as far as the host's packet says it has got (2 bits of swapPhases each).
#pragma force_active on

void PokeathlonRelayRunTeam_DrawSwap(PokeathlonRelayRunHud *hud, PokeathlonRelayRunTeam *team, VecFx32 *ground, PokeathlonRelayRunPacket *packet, u8 teamId, u8 member, PokeathlonRelayRunTouch *touch, int isHost, int isOwn, int isCpu) {
    u16 changed = FALSE;
    u8 phase = (packet->swapPhases >> (teamId * 2)) & 3;
    s32 x, y;
    VecFx32 position;
    BOOL reportPhase;

    if (team->shownSwapPhase != phase) {
        team->shownSwapPhase = phase;
        team->swapStep = 0;
        changed = TRUE;
    }
    PokeathlonCourseObject_GroundToCenter(team->monObjects[member], ground->x / FX32_ONE, ground->y / FX32_ONE, &x, &y);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    position.z = 0;
    switch (team->shownSwapPhase) {
    case 1:
        PokeathlonRelayRunTeam_AnimateLeave(hud, team, &position, teamId, member, touch, isHost && isCpu, isOwn);
        break;
    case 2:
        reportPhase = isHost && isCpu;
        PokeathlonRelayRunTeam_AnimateArrive(team, &position, teamId, member, touch, reportPhase | isOwn, isOwn);
        break;
    case 0:
        if (changed) {
            PokeathlonRelayRunTeam_EndSwap(team, teamId, member, touch, (isHost && isCpu) | isOwn, isOwn);
        }
        PokeathlonCourseSpritePoolSprite_SetPosition(team->shadowSprite, &position);
        break;
    }
}

#pragma force_active reset

// Sets up a member of a team from its Pokemon's stats and the event's stat tables (setup->tables[n][stat]).
#pragma force_active on

void PokeathlonRelayRun_InitMember(PokeathlonCourseData *course, void *setupFile, u8 participant, u8 member, PokeathlonRelayRunTeam *team) {
    PokeathlonRelayRunSetup *setup = setupFile;
    u8 *stats = PokeathlonCourse_GetMonStats(course, participant, member);
    team->members[member].pushPower = (f32)setup->tables[0][stats[0]] / 10.0;
    team->members[member].maxEnergy = setup->tables[1][stats[3]];
    team->members[member].energy = setup->tables[1][stats[3]];
    team->members[member].speedLimit = setup->tables[5][stats[1]];
    team->members[member].patchSpeed = setup->tables[7][stats[1]];
    team->members[member].tiredSpeed = setup->tables[6][stats[1]];
    team->members[member].acceleration = (f32)setup->tables[2][stats[4]] / 10.0;
    team->members[member].weight = (f32)setup->tables[3][stats[1]] / 10.0;
    team->members[member].recoveryTime = setup->tables[4][stats[3]];
}

#pragma force_active reset

// Collects the obstacles within a screen of x: walls (kinds 1 and 2) and slow patches separately.
#pragma force_active on

void PokeathlonRelayRun_FindNearbyObstacles(PokeathlonRelayRun *relayRun, fx32 x, s32 **walls, s32 **patches, u8 *wallCount, u8 *patchCount) {
    int i;
    int wholeX;
    for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
        walls[i] = NULL;
        patches[i] = NULL;
    }
    *wallCount = 0;
    *patchCount = 0;
    wholeX = x / FX32_ONE;
    for (i = 0; i < RELAY_RUN_OBSTACLES; i++) {
        u8 kind;
        int distance;
        int screenX;
        if (relayRun->obstacles[i].active == FALSE) {
            return;
        }
        kind = relayRun->layout[i];
        distance = wholeX - relayRun->obstacles[i].x;
        if (distance >= 768.0) {
            screenX = 80.0 + (1024.0 - distance);
        } else if (distance <= -768.0) {
            screenX = 80.0 - (1024.0 + distance);
        } else {
            screenX = RELAY_RUN_RUNNER_X - distance;
        }
        if (screenX >= -32 && screenX <= 288) {
            if (kind == RELAY_RUN_OBSTACLE_SLOW_PATCH) {
                patches[(*patchCount)++] = &relayRun->layout[i];
            } else {
                walls[(*wallCount)++] = &relayRun->layout[i];
            }
        }
    }
}

#pragma force_active reset

// The outline of an obstacle from its layout entry: a 48x48 (kind 1) or 32x32 box centred on it, with its edges
// moved out by inset (the runner's radius); a slow patch is the 64x64 square around it.
#pragma force_active on

void PokeathlonRelayRun_GetObstacleShape(int inset, const s32 *entry, volatile s32 *shape) {
    s32 word = *entry;
    u32 kind = (u8)word;
    int y = (u8)(word >> 8);
    int x = (u16)(word >> 16);
    u32 size;
    y += 32;
    shape[0] = kind;
    if (kind == RELAY_RUN_OBSTACLE_SLOW_PATCH) {
        shape[1] = x - 32;
        shape[2] = y - 32;
        return;
    }
    size = kind == 1 ? 48 : 32;
    x -= size / 2;
    y -= size / 2;
    // corners: top left, top right, bottom right, bottom left
    shape[1] = x;
    shape[2] = y;
    shape[3] = x + size;
    shape[4] = y;
    shape[5] = x + size;
    shape[6] = y + size;
    shape[7] = x;
    shape[8] = y + size;
    // edges: top, right, bottom, left
    {
        int ax = shape[1], ay = shape[2];
        shape[9] = ax;
        shape[10] = ay;
    }
    {
        int ax = shape[3], ay = shape[4];
        shape[11] = ax;
        shape[12] = ay;
    }
    {
        int ax = shape[3], ay = shape[4];
        shape[13] = ax;
        shape[14] = ay;
    }
    {
        int ax = shape[5], ay = shape[6];
        shape[15] = ax;
        shape[16] = ay;
    }
    {
        int ax = shape[5], ay = shape[6];
        shape[17] = ax;
        shape[18] = ay;
    }
    {
        int ax = shape[7], ay = shape[8];
        shape[19] = ax;
        shape[20] = ay;
    }
    {
        int ax = shape[7], ay = shape[8];
        shape[21] = ax;
        shape[22] = ay;
    }
    {
        int ax = shape[1], ay = shape[2];
        shape[23] = ax;
        shape[24] = ay;
    }
    shape[10] -= inset;
    shape[12] -= inset;
    shape[13] += inset;
    shape[15] += inset;
    shape[18] += inset;
    shape[20] += inset;
    shape[21] -= inset;
    shape[23] -= inset;
}

#pragma force_active reset

// Moves a runner from start to end past a box obstacle: returns which edge (RELAY_RUN_HIT_TOP ..) or corner
// (RELAY_RUN_HIT_TOP_LEFT ..) it ran into and where it stops, or the side it is pushed out of when it ended up
// inside (RELAY_RUN_HIT_INSIDE_TOP ..). 0 if it missed.
#pragma force_active on

int PokeathlonRelayRun_CollideWithObstacle(VecFx32 *start, VecFx32 *end, int radius, PokeathlonRelayRunObstacleShape *shape, VecFx32 *out) {
    PokeathlonRelayRunHitTable edgeHits;
    PokeathlonRelayRunHitTable cornerHits;
    VecFx32 corner;
    VecFx32 diff;
    VecFx32 delta;
    VecFx32 result;
    Point2D hit;
    Point2D a;
    Point2D b;
    int i;
    BOOL onEdge;
    int lo, hi;
    if (shape->kind == RELAY_RUN_OBSTACLE_SLOW_PATCH) {
        return RELAY_RUN_HIT_NONE;
    }
    a.x = start->x / FX32_ONE;
    a.y = start->y / FX32_ONE;
    b.x = end->x / FX32_ONE;
    b.y = end->y / FX32_ONE;
    for (i = 0; i < 4; i++) {
        edgeHits = sRelayRunEdgeHits;
        if (Point2D_CalcSegmentIntersection(&shape->edges[i][0], &shape->edges[i][1], &a, &b, &hit)) {
            out->x = hit.x << FX32_SHIFT;
            out->y = hit.y << FX32_SHIFT;
            out->z = 0;
            return edgeHits.hits[i];
        }
        onEdge = FALSE;
        if (i == 0 && i == 2) { // never true: the start point is tested against the vertical edges' line only
            if (a.y == shape->edges[i][0].y) {
                if (shape->edges[i][0].x < shape->edges[i][1].x) {
                    lo = shape->edges[i][0].x;
                    hi = shape->edges[i][1].x;
                } else {
                    lo = shape->edges[i][1].x;
                    hi = shape->edges[i][0].x;
                }
                if (lo <= a.x && a.x <= hi) {
                    onEdge = TRUE;
                }
            }
        } else {
            if (a.x == shape->edges[i][0].x) {
                if (shape->edges[i][0].y < shape->edges[i][1].y) {
                    lo = shape->edges[i][0].y;
                    hi = shape->edges[i][1].y;
                } else {
                    lo = shape->edges[i][1].y;
                    hi = shape->edges[i][0].y;
                }
                if (lo <= a.y && a.y <= hi) {
                    onEdge = TRUE;
                }
            }
        }
        if (onEdge) {
            *out = *start;
            return edgeHits.hits[i];
        }
    }
    for (i = 0; i < 4; i++) {
        cornerHits = sRelayRunCornerHits;
        corner.x = shape->corners[i].x << FX32_SHIFT;
        corner.y = shape->corners[i].y << FX32_SHIFT;
        corner.z = 0;
        VEC_Subtract(end, &corner, &diff);
        if (VEC_Mag(&diff) <= radius * FX32_ONE) {
            fx32 t = PokeathlonRelayRun_CalcCircleEntry(start, end, &corner, radius << FX32_SHIFT);
            VEC_Subtract(end, start, &delta);
            VEC_MultAdd(t, &delta, start, &result);
            out->x = result.x;
            out->y = result.y;
            out->z = 0;
            return cornerHits.hits[i];
        }
    }
    if (shape->corners[0].x << FX32_SHIFT <= end->x && end->x <= shape->corners[2].x << FX32_SHIFT
        && shape->corners[0].y << FX32_SHIFT <= end->y && end->y <= shape->corners[2].y << FX32_SHIFT) {
        int dxRight;
        BOOL pushX;
        int left;
        int dxLeft;
        BOOL pushY;
        int bottom;
        int sideX;
        int top;
        int dyTop;
        int dyBottom;
        int sideY;
        int side;
        *out = *end;
        pushY = FALSE;
        pushX = FALSE;
        left = shape->corners[0].x;
        dxLeft = (left << FX32_SHIFT) - end->x;
        dxRight = (shape->corners[2].x << FX32_SHIFT) - end->x;
        if (dxLeft < 0) {
            dxLeft *= -1;
        }
        if (dxRight < 0) {
            dxRight *= -1;
        }
        if (dxLeft < dxRight) {
            sideX = RELAY_RUN_HIT_INSIDE_LEFT;
        } else {
            dxLeft = dxRight;
            sideX = RELAY_RUN_HIT_INSIDE_RIGHT;
        }
        top = shape->corners[0].y;
        dyTop = (top << FX32_SHIFT) - end->y;
        bottom = shape->corners[2].y;
        dyBottom = (bottom << FX32_SHIFT) - end->y;
        if (dyTop < 0) {
            dyTop *= -1;
        }
        if (dyBottom < 0) {
            dyBottom *= -1;
        }
        if (dyTop < dyBottom) {
            sideY = RELAY_RUN_HIT_INSIDE_TOP;
        } else {
            dyTop = dyBottom;
            sideY = RELAY_RUN_HIT_INSIDE_BOTTOM;
        }
        if (dxLeft <= dyTop) {
            pushX = TRUE;
            side = sideX;
        } else {
            pushY = TRUE;
            side = sideY;
        }
        if (pushX) {
            if (sideX == RELAY_RUN_HIT_INSIDE_LEFT) {
                out->x = (left - radius) << FX32_SHIFT;
            } else {
                out->x = (shape->corners[2].x + radius) << FX32_SHIFT;
            }
        } else if (pushY) {
            if (sideY == RELAY_RUN_HIT_INSIDE_TOP) {
                out->y = (top - radius) << FX32_SHIFT;
            } else {
                out->y = (bottom + radius) << FX32_SHIFT;
            }
        }
        return side;
    }
    return RELAY_RUN_HIT_NONE;
}

#pragma force_active reset

// Whether position is on a slow patch (the 64x64 square at its shape's first corner).
BOOL PokeathlonRelayRun_IsOnSlowPatch(const VecFx32 *position, const PokeathlonRelayRunObstacleShape *shape) {
    s32 px = position->x;
    s32 py = position->y;
    s32 x = shape->corners[0].x << FX32_SHIFT;
    s32 y = shape->corners[0].y << FX32_SHIFT;
    if (x < px && px < x + FX32_CONST(64) && y < py && py < y + FX32_CONST(64)) {
        return TRUE;
    }
    return FALSE;
}

// Moves a runner (radius) from from to to among the obstacles near it. Returns whether it is on a slow patch
// << 8 | the wall it ran into (RELAY_RUN_HIT_*, 0: none); out gets where it stops, unless it is still touching
// the same wall as last frame (wallState).
#pragma force_active on

u16 PokeathlonRelayRun_CollideWithObstacles(PokeathlonRelayRun *relayRun, int wallState, int radius, VecFx32 *from, VecFx32 *to, VecFx32 *out) {
    s32 *walls[RELAY_RUN_OBSTACLES];
    s32 *patches[RELAY_RUN_OBSTACLES];
    PokeathlonRelayRunObstacleShape patchShape;
    PokeathlonRelayRunObstacleShape wallShape;
    VecFx32 stop;
    u8 counts[2];
    int i;
    u8 onPatch;
    u8 result;
    PokeathlonRelayRun_FindNearbyObstacles(relayRun, from->x, walls, patches, &counts[1], &counts[0]);
    onPatch = FALSE;
    for (i = 0; i < counts[0]; i++) {
        PokeathlonRelayRun_GetObstacleShape(radius, patches[i], (volatile s32 *)&patchShape);
        if (PokeathlonRelayRun_IsOnSlowPatch(from, &patchShape)) {
            onPatch = TRUE;
            break;
        }
    }
    result = RELAY_RUN_HIT_NONE;
    for (i = 0; i < counts[1]; i++) {
        int hit;
        PokeathlonRelayRun_GetObstacleShape(radius, walls[i], (volatile s32 *)&wallShape);
        hit = PokeathlonRelayRun_CollideWithObstacle(from, to, radius, &wallShape, &stop);
        if (hit != RELAY_RUN_HIT_NONE) {
            if (hit >= RELAY_RUN_HIT_INSIDE_TOP) {
                out->x = stop.x;
                out->y = stop.y;
                out->z = 0;
            } else if (hit != wallState) {
                out->x = stop.x;
                out->y = stop.y;
                out->z = 0;
            }
            result = hit;
            break;
        }
    }
    return (onPatch << 8) + result;
}

#pragma force_active reset

// Where along the segment a -> b (0: a, 1: b) it first comes within r of c.
fx32 PokeathlonRelayRun_CalcCircleEntry(const VecFx32 *a, const VecFx32 *b, const VecFx32 *c, fx32 r) {
    VecFx32 d;
    fx32 s, t, lenSq, dot, m, u;
    VEC_Subtract(b, a, &d);
    lenSq = VEC_Mag(&d);
    lenSq = lenSq * lenSq;
    dot = VEC_DotProduct(&d, c);
    m = VEC_Mag((VecFx32 *)c);
    s = FX_Sqrt(dot - (m * m - r * r) * lenSq);
    t = FX_Div(s - dot, lenSq);
    u = FX_Div(-(dot + s), lenSq);
    return t > u ? u : t;
}

// The fastest a flick can make the member run: its speed on a slow patch, when tired, or its speed limit.
f32 PokeathlonRelayRunTeam_GetMaxSpeed(PokeathlonRelayRunTeam *team, int member) {
    if (team->onPatch != FALSE) {
        return (f32)team->members[member].patchSpeed;
    }
    if (team->members[member].energy <= 40.0) {
        return (f32)team->members[member].tiredSpeed;
    }
    return (f32)team->members[member].speedLimit;
}

// Adds a flick to a velocity: x always adds up, y only when it goes the same way (else the flick's y wins).
void PokeathlonRelayRun_AddFlick(const VecFx32 *velocity, const VecFx32 *flick, VecFx32 *out) {
    out->x = velocity->x + flick->x;
    if (FX_Mul(velocity->y, flick->y) >= 0) {
        out->y = velocity->y + flick->y;
    } else {
        out->y = flick->y;
    }
}

// Runners bumping into each other: the one running into another stops and passes its speed (times its push
// power, at most 11 pixels per frame) on; a tired member that is hit is exhausted; both are stunned. Returns
// whether any two bumped, where the first two did in where.
#pragma force_active on

BOOL PokeathlonRelayRun_CollideRunners(PokeathlonCourseData *course, PokeathlonRelayRunTeam *teams, VecFx32 *where) {
    VecFx32 posP;
    VecFx32 posQ;
    VecFx32 d;
    int radP, radQ;
    int px, py, qx, qy;
    void *objectP, *objectQ;
    BOOL result;
    u8 pushMember;
    int pX;
    int wrappedPx;
    s32 i;
    u8 memberP;
    int j;
    PokeathlonRelayRunTeam *p, *q;
    result = FALSE;
    posP.z = 0;
    posQ.z = 0;
    for (i = 0, p = teams; i < RELAY_RUN_TEAMS; i++, p++) {
        memberP = p->current;
        if (p->swapping != FALSE || p->stunTimer != 0) {
            continue;
        }
        objectP = p->monObjects[memberP];
        PokeathlonCourseObject_GroundToCenter(objectP, p->position.x / FX32_ONE, p->position.y / FX32_ONE, &px, &py);
        for (j = 0, q = teams; j < RELAY_RUN_TEAMS; j++, q++) {
            if (i == j) {
                continue;
            }
            if (q->swapping != FALSE) {
                continue;
            }
            wrappedPx = px;
            objectQ = q->monObjects[q->current];
            PokeathlonCourseObject_GroundToCenter(objectQ, q->position.x / FX32_ONE, q->position.y / FX32_ONE, &qx, &qy);
            {
                // across the start line: compare them on the same lap
                pX = p->position.x;
                if (pX > 768.0 * FX32_ONE && q->position.x < 256 * FX32_ONE) {
                    {
                        double t = qx;
                        t += 1024.0;
                        qx = t;
                    }
                } else if (q->position.x > 768.0 * FX32_ONE && pX < 256 * FX32_ONE) {
                    {
                        double t = wrappedPx;
                        t += 1024.0;
                        wrappedPx = t;
                    }
                }
            }
            PokeathlonCourseObject_GetCollisionCircle(objectQ, qx << FX32_SHIFT, qy << FX32_SHIFT, &posQ.x, &posQ.y, &radQ);
            PokeathlonCourseObject_GetCollisionCircle(objectP, wrappedPx << FX32_SHIFT, py << FX32_SHIFT, &posP.x, &posP.y, &radP);
            VEC_Subtract(&posQ, &posP, &d);
            if (VEC_Mag(&d) < (radQ + radP) << FX32_SHIFT) {
                if (p->bumpedInto[j] != 0) {
                    continue;
                }
                VecFx32 zero = { 0, 0, 0 };
                if (VEC_DotProduct(&d, &p->velocity) <= 0) {
                    continue;
                }
                if (VEC_Mag(&p->velocity) != 0) {
                    pushMember = p->current;
                    VEC_MultAdd(4096.0f * p->members[pushMember].pushPower, &p->velocity, &zero, &q->velocity);
                    if (VEC_Mag(&q->velocity) > FX32_CONST(11)) {
                        VecFx32 zero2 = { 0, 0, 0 };
                        VEC_Normalize(&q->velocity, &q->velocity);
                        VEC_MultAdd(FX32_CONST(11), &q->velocity, &zero2, &q->velocity);
                    }
                }
                // (members of q indexed by p's runner, as the original does)
                if (q->members[memberP].condition == RELAY_RUN_TIRED) {
                    *(int *)&q->members[memberP].energy = 0;
                    q->members[memberP].condition = RELAY_RUN_EXHAUSTED;
                    q->recoveryTimer = q->members[memberP].recoveryTime;
                    PokeathlonCourse_AddMonCounter(course, q->owner, q->current, 1, 1);
                }
                {
                    VecFx32 stop = { 0, 0, 0 };
                    p->velocity = stop;
                }
                if (p->members[pushMember].condition == RELAY_RUN_TIRED) {
                    *(int *)&p->members[pushMember].energy = 0;
                    p->members[pushMember].condition = RELAY_RUN_EXHAUSTED;
                    p->recoveryTimer = p->members[pushMember].recoveryTime;
                    PokeathlonCourse_AddMonCounter(course, p->owner, p->current, 1, 1);
                }
                GF_ASSERT(p->bumpedInto[j] == 0);
                GF_ASSERT(q->bumpedInto[i] == 0);
                q->bumpedInto[j] = 1;
                p->bumpedInto[i] = 1;
                if (!result) {
                    VecFx32 contact = { 0, 0, 0 };
                    result = TRUE;
                    VEC_Subtract(&posQ, &posP, &contact);
                    VEC_Normalize(&contact, &contact);
                    VEC_MultAdd(radP << FX32_SHIFT, &contact, &posP, &contact);
                    if (contact.x >= 1024.0 * FX32_ONE) {
                        contact.x = contact.x - 1024.0 * FX32_ONE;
                    }
                    *where = contact;
                }
                q->stunTimer = 6;
                p->stunTimer = 18;
                PokeathlonCourse_AddMonCounter(course, q->owner, q->current, 4, 1);
                PokeathlonCourse_AddMonCounter(course, p->owner, p->current, 4, 1);
                break;
            } else {
                p->bumpedInto[j] = 0;
                q->bumpedInto[i] = 0;
            }
        }
    }
    return result;
}

#pragma force_active reset

// Places a mark (sprite) above a Pokemon standing at position, higher for bigger Pokemon (size class 1-3 in
// its performance data).
void PokeathlonRelayRun_PlaceAboveMon(PokeathlonCoursePerformance *performance, const VecFx32 *position, PokeathlonCourseSpritePoolSprite *sprite) {
    VecFx32 above = *position;
    switch (performance->raw[8]) {
    case 1:
        above.y -= FX32_CONST(20);
        break;
    case 2:
        above.y -= FX32_CONST(32);
        break;
    case 3:
        above.y -= FX32_CONST(40);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    PokeathlonCourseSpritePoolSprite_SetPosition(sprite, &above);
}

// Obstacles further down the track are drawn in front; slow patches lie under everything.
void PokeathlonRelayRunObstacle_SetDrawPriority(PokeathlonRelayRunObstacle *obstacle) {
    int row;
    if (obstacle->kind == RELAY_RUN_OBSTACLE_SLOW_PATCH) {
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(obstacle->sprite, 600);
        return;
    }
    row = 160 - (obstacle->y + 32);
    if (row < 0) {
        row = 0;
    }
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(obstacle->sprite, 30 * (row / 8 + 1) + 3);
}

// Draw priorities of a team at y: in front of the obstacles above it, the player's own team before the others.
void PokeathlonRelayRunTeam_SetDrawPriority(PokeathlonRelayRunTeam *team, u8 y, int isOwn) {
    int row = 168 - y;
    int priority;
    if (row < 0) {
        row = 0;
    }
    row = row / 8;
    if (isOwn != FALSE) {
        priority = 30 * row + 4;
    } else {
        priority = 30 * row + 7;
    }
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(team->shadowSprite, priority);
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(team->speedSprite, priority + 1);
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(team->conditionSprite, priority + 1);
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(team->swapSprite, priority + 2);
    {
        int i;
        void **objects = team->monObjects;
        for (i = 0; i < RELAY_RUN_MEMBERS; i++) {
            PokeathlonCourseMonObject_SetDrawPriority(*objects, priority + 2);
            objects++;
        }
    }
}

// The start line (BG 0, tile 32 in columns 10-11) while the track around it is on screen; tile 1 when the
// player's runner is between x 272 and 768.
void PokeathlonRelayRun_UpdateStartLine(BgConfig *bgConfig, int x) {
    if (x > 272 && x < 768) {
        FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_0, 1, 10, 3, 2, 21, 16);
    } else {
        FillBgTilemapRect(bgConfig, GF_BG_LYR_MAIN_0, 32, 10, 3, 2, 21, 16);
    }
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_MAIN_0);
}
