#include "global.h"

#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_ring_drop.h"

#include "assert.h"
#include "filesystem.h"
#include "math_util.h"
#include "sprite_system.h"

// Course mon objects (OVY_96; not declared by pokeathlon_course_internal.h).
void PokeathlonCourseObject_SetCollision(PokeathlonCourseObject *object, s32 offsetY, s32 radius);
void PokeathlonCourseObject_SetGroundPosition(PokeathlonCourseObject *object, s32 x, s32 y);
void PokeathlonCourseObject_SetTouchRadius(PokeathlonCourseObject *object, int radius);
void PokeathlonCourseObject_GroundToCenter(PokeathlonCourseObject *object, s32 x, s32 y, s32 *outX, s32 *outY);
void PokeathlonCourseObject_CenterToGround(PokeathlonCourseObject *object, int x, int y, int *outX, int *outY);
void PokeathlonCourseObject_GetCollisionCircle(PokeathlonCourseObject *object, fx32 x, fx32 y, fx32 *outX, fx32 *outY, int *radius);

// The code calls these two with u8 arguments.
#define PokeathlonRingDrop_GetMonObject(objects, index)    (((void *(*)(void *, u8))PokeathlonCourseMonObjects_Get)((objects), (index)))
#define PokeathlonRingDrop_SetMonObjectDrawn(object, draw) (((void (*)(void *, u8))PokeathlonCourseMonObject_SetDrawFlag)((object), (draw)))

// An entry of NARC 170 #19: a mon object's collision circle by its size.
typedef struct PokeathlonRingDropCollision {
    s32 offsetY;
    s32 radius;
} PokeathlonRingDropCollision;

// The Pokemon start positions (ground x, y) and the switch spots: x, y, radius.
extern const s16 gPokeathlonRingDropMonStartPositions[RING_DROP_PARTICIPANTS][2];

typedef struct PokeathlonRingDropSwitchSpot {
    s16 x;
    s16 y;
    s16 radius;
} PokeathlonRingDropSwitchSpot;

// gPokeathlonRingDropSwitchSpots[participant] (the code reads y and radius through the labels ov96_0221D70A and ov96_0221D70C).
extern const PokeathlonRingDropSwitchSpot gPokeathlonRingDropSwitchSpots[], ov96_0221D70A[], ov96_0221D70C[];
// The direction 4 (PokeathlonRingDrop_GetDirection measures the angle from it).
extern VecFx32 gPokeathlonRingDropDirectionReference;
// The ring's edge: two lines (x, y pairs) the Pokemon must stay right of between y 64 and 160.
extern const s16 gPokeathlonRingDropRingEdge[4][2];

BOOL PokeathlonRingDrop_IsAttackState(int state) {
    if (state == RING_DROP_MON_DASH || state == RING_DROP_MON_JUMP_FALL) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// source pushes target (they collide; direction points from source to target). A dash pushes three times its
// power and takes stamina, a press pushes with the press power; the attacker scores. Other pushes are weak bumps.
BOOL PokeathlonRingDropMon_Push(PokeathlonRingDropMon *target, PokeathlonRingDropMon *source, VecFx32 *direction) {
    int power = source->member->params->dashPower;
    int damage = source->member->params->dashDamage;
    if (PokeathlonRingDropMon_CanBePushed(target->state, source->state)) {
        if (source->state == RING_DROP_MON_DASH) {
            int score;
            power = (int)(3.0f * (float)power);
            damage = (int)(2.0f * (float)damage);
            score = (int)source->flags.bits.score + 1;
            if (score > 200) {
                score = 200;
            }
            source->flags.bits.score = score;
            PokeathlonCourse_AddMonCounter(source->course, (u8)source->flags.bits.participant, (u8)source->flags.bits.member, 3, 1);
            PokeathlonCourse_AddMonCounter(source->course, (u8)source->flags.bits.participant, (u8)source->flags.bits.member, 4, 1);
        } else if (source->state == RING_DROP_MON_JUMP_FALL) {
            int score;
            power = source->member->params->pressPower;
            damage = source->member->params->pressDamage;
            score = (int)source->flags.bits.score + 2;
            if (score > 200) {
                score = 200;
            }
            source->flags.bits.score = score;
            PokeathlonCourse_AddMonCounter(source->course, (u8)source->flags.bits.participant, (u8)source->flags.bits.member, 3, 1);
            PokeathlonCourse_AddMonCounter(source->course, (u8)source->flags.bits.participant, (u8)source->flags.bits.member, 4, 1);
        } else {
            power = FX32_CONST(3);
        }
        VEC_MultAdd(power, direction, &target->velocity, &target->velocity);
        PokeathlonRingDrop_ClampVelocity(&target->velocity, FX32_CONST(12));
        PokeathlonRingDropMon_FaceDirection(source, direction);
        if (PokeathlonRingDrop_IsAttackState(source->state)) {
            target->hitFrames = 4;
            source->hitFrames = 4;
            PokeathlonRingDropMon_DrainStamina(target, damage);
        }
        if (!target->attacker || PokeathlonRingDrop_IsAttackState(source->state)) {
            target->attacker = source;
            if (source->attacker && source->attacker != target) {
                target->attacker = source->attacker;
            }
        }
        target->flags.bits.pushed = 1;
        target->attackerState = source->state;
        if (PokeathlonRingDrop_IsAttackState(source->state)) {
            u32 attacker;
            if (target->flags.bits.hit != 2) {
                switch (source->state) {
                case RING_DROP_MON_DASH:
                    target->flags.bits.hit = 1;
                    break;
                case RING_DROP_MON_JUMP_FALL:
                    target->flags.bits.hit = 2;
                    break;
                }
            }
            attacker = source->flags.bits.participant;
            if (attacker < target->flags.bits.participant) {
                attacker++;
            }
            target->flags.bits.attacker = attacker;
            target->hitMarkFrames = 4;
        } else {
            target->flags.bits.bumped = 1;
        }
        GF_ASSERT(source->state != RING_DROP_MON_FALL_OUT && source->state != RING_DROP_MON_RETURN);
        GF_ASSERT(target->state != RING_DROP_MON_FALL_OUT && target->state != RING_DROP_MON_RETURN && target->state != RING_DROP_MON_JUMP_FALL);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// The twelve team members' mon objects.
void PokeathlonRingDrop_CreateMonObjects(PokeathlonRingDrop *ringDrop, PokeathlonCourseData *course) {
    struct {
        PokeathlonRingDrop *ringDrop;
    } saved;

    SpriteList *spriteList;

    saved.ringDrop = ringDrop;
    spriteList = SpriteManager_GetSpriteList(saved.ringDrop->spriteManager);
    saved.ringDrop->monObjects = PokeathlonCourseMonObjects_Create(saved.ringDrop->heapId, 12, 7, saved.ringDrop->resources, spriteList);
    {
        PokeathlonCourseMonInfo infos[12];
        PokeathlonCourseMonGfxTemplate template = { 0 };
        int i;
        for (i = 0; i < 12; i++) {
            int member = i % 3;
            int participant = i / 3;
            PokeathlonCourse_GetMonInfo(course, participant, member, &infos[i]);
            template.sizeTypes[i] = PokeathlonCourse_GetSpriteSizeType((void *)PokeathlonCourse_GetPerformance(course, participant, member));
        }
        template.priority = 3;
        template.vram = 1;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        PokeathlonCourseMonObjects_Load(saved.ringDrop->monObjects, 12, infos, &template, 0, 0);
    }
}

// Places the team members (the first of each team in the ring), sets their numbers and the start countdown sprites.
#pragma force_active on

void PokeathlonRingDrop_InitMons(PokeathlonRingDrop *ringDrop, PokeathlonCourseData *course) {
    PokeathlonRingDropCollision collisions[3] = { 0 };

    struct {
        s32 startY;
        s32 startX;
        s32 centerY;
        s32 centerX;
        PokeathlonCoursePoint own;
    } t;

    int i;
    u8 participant, member;
    void *object;
    PokeathlonRingDropMember *teamMember;
    s16 x, y;
    PokeathlonRingDropMon *mon;

    ReadWholeNarcMemberByIdPair(collisions, (NarcId)170, 19);
    GF_ASSERT(ringDrop != NULL);
    PokeathlonRingDrop_InitMemberParams(course);
    for (i = 0; i < 12; i++) {
        int size;
        participant = i / 3;
        member = i % 3;
        teamMember = &ringDrop->players[participant].mon.members[member];
        object = PokeathlonRingDrop_GetMonObject(ringDrop->monObjects, i);
        PokeathlonRingDrop_SetMonObjectDrawn(object, member == 0);
        size = PokeathlonCourse_GetCollisionSizeClass(((void *(*)(PokeathlonCourseData *, u8, u8))PokeathlonCourse_GetPerformance)(course, participant, member));
        PokeathlonCourseObject_SetCollision(object, collisions[size - 1].offsetY, collisions[size - 1].radius);
        object = PokeathlonRingDrop_GetMonObject(ringDrop->monObjects, i);
        PokeathlonCourseMonObject_GetGfx(object);
        PokeathlonCourseMonObject_SetAffineOverwriteMode(object, 2);
        PokeathlonRingDrop_SetMonAffineCenter(object, TRUE);
        x = gPokeathlonRingDropMonStartPositions[participant][0];
        y = gPokeathlonRingDropMonStartPositions[participant][1];
        if (participant == PokeathlonCourse_GetNetId(course)) {
            t.own.x = x;
            t.own.y = y;
        }
        PokeathlonCourseMonObject_SetAnimState(object, 2);
        PokeathlonCourseMonObject_SetDrawPriority(object, 7);
        PokeathlonCourseObject_SetGroundPosition(object, x, y);
        PokeathlonCourseObject_SetTouchRadius(object, PokeathlonCourse_GetTouchRadius());
        PokeathlonCourseObject_GroundToCenter(object, x, y, &t.centerX, &t.centerY);
        teamMember->index = i;
        teamMember->object = object;
        teamMember->params = ((PokeathlonRingDropMemberParams * (*)(PokeathlonCourseData *, u8, u8)) PokeathlonRingDrop_GetMemberParams)(course, participant, member);
        teamMember->stamina = teamMember->params->stamina;
    }
    PokeathlonCourse_CreateStartSprites(course, 0, ringDrop->resources, SpriteManager_GetSpriteList(ringDrop->spriteManager), NNS_G2D_VRAM_TYPE_2DMAIN, 1, &t.own);
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        mon = &ringDrop->players[i].mon;
        mon->player = &ringDrop->players[i];
        mon->member = ringDrop->players[i].mon.members;
        mon->ui = ringDrop->ui;
        mon->course = course;
        PokeathlonCourseObject_GroundToCenter(mon->member->object, gPokeathlonRingDropMonStartPositions[i][0], gPokeathlonRingDropMonStartPositions[i][1], &t.startX, &t.startY);
        PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_STAND);
        mon->flags.bits.direction = 2;
        mon->position.x = t.startX << FX32_SHIFT;
        mon->position.y = t.startY << FX32_SHIFT;
        mon->destination.x = t.startX << FX32_SHIFT;
        mon->destination.y = t.startY << FX32_SHIFT;
        mon->flags.bits.participant = i;
        mon->fallSpeed = 0;
        mon->status = 3;
        ringDrop->players[i].mon.index = i;
    }
}

#pragma force_active reset

void PokeathlonRingDrop_FreeMonObjects(PokeathlonRingDrop *ringDrop) {
    PokeathlonCourseMonObjects_Free(ringDrop->monObjects);
}

void PokeathlonRingDropMon_ApplyPushReaction(PokeathlonRingDropMon *mon, int reaction) {
    switch (reaction) {
    case RING_DROP_REACTION_NONE:
        break;
    case RING_DROP_REACTION_STAND:
        PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_STAND);
        break;
    case RING_DROP_REACTION_STOP:
        PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_DASH_END);
        mon->dashFrames = 1;
        break;
    case RING_DROP_REACTION_STAGGER:
        PokeathlonRingDropMon_Stagger(mon);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

int PokeathlonRingDropMon_GetPushReaction(PokeathlonRingDropMon *mon) {
    int state = mon->state;
    int attackerState = mon->attackerState;
    u32 pushed = mon->flags.bits.pushed;
    int reaction = RING_DROP_REACTION_NONE;

    if (pushed == 0) {
        return reaction;
    }
    switch (state) {
    case RING_DROP_MON_JUMP_FALL:
        reaction = RING_DROP_REACTION_NONE;
        break;
    case RING_DROP_MON_DASH_START:
    case RING_DROP_MON_DASH:
    case RING_DROP_MON_DASH_END:
    case RING_DROP_MON_JUMP_START:
    case RING_DROP_MON_LANDING:
        if (attackerState == RING_DROP_MON_JUMP_FALL || attackerState == RING_DROP_MON_DASH) {
            reaction = RING_DROP_REACTION_STAGGER;
        } else if (state == RING_DROP_MON_DASH) {
            reaction = RING_DROP_REACTION_STOP;
        } else {
            reaction = RING_DROP_REACTION_STAND;
        }
        break;
    case RING_DROP_MON_STAND:
    case RING_DROP_MON_STAGGER:
    case RING_DROP_MON_JUMP_RISE:
        if (attackerState == RING_DROP_MON_JUMP_FALL || attackerState == RING_DROP_MON_DASH) {
            reaction = RING_DROP_REACTION_STAGGER;
        }
        break;
    case 0:
    case RING_DROP_MON_FALL_OUT:
    case RING_DROP_MON_RETURN:
    case RING_DROP_MON_JUMP_TOP:
        GF_AssertFail();
        break;
    default:
        GF_AssertFail();
        break;
    }
    return reaction;
}

// Host: every pair of Pokemon that touch pushes each other once (until one of them stops sliding).
#pragma force_active on

void PokeathlonRingDrop_UpdateCollisions(PokeathlonCourseData *course) {
    int i;
    PokeathlonRingDrop *ringDrop;
    u8(*contacts)[RING_DROP_PARTICIPANTS];
    int j;

    ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    contacts = ringDrop->contacts;
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        PokeathlonRingDropMon *a = &ringDrop->players[i].mon;
        if (!PokeathlonRingDropMon_CanCollide(a) || a->hitFrames > 0) {
            continue;
        }
        for (j = i + 1; j < RING_DROP_PARTICIPANTS; j++) {
            PokeathlonRingDropMon *b = &ringDrop->players[j].mon;
            if (PokeathlonRingDropMon_CanCollide(b) && PokeathlonRingDropMon_Collide(a, b)) {
                VecFx32 direction = { 0, 0, 0 };
                BOOL pushedA, pushedB;
                int reactionA, reactionB;
                if (contacts[i][j] && !a->flags.bits.pushed) {
                    contacts[i][j] = contacts[j][i] = 0;
                }
                if (contacts[i][j] == 0) {
                    VEC_Subtract(&a->position, &b->position, &direction);
                    if (direction.x == 0 && direction.y == 0) {
                        direction.x = (MTRandom() & 0x3F) - 32;
                        direction.y = (MTRandom() & 0x3F) - 32;
                    }
                    VEC_Normalize(&direction, &direction);
                    pushedA = PokeathlonRingDropMon_Push(a, b, &direction);
                    direction.x *= -1;
                    direction.y *= -1;
                    pushedB = PokeathlonRingDropMon_Push(b, a, &direction);
                    reactionA = PokeathlonRingDropMon_GetPushReaction(a);
                    reactionB = PokeathlonRingDropMon_GetPushReaction(b);
                    PokeathlonRingDropMon_ApplyPushReaction(a, reactionA);
                    PokeathlonRingDropMon_ApplyPushReaction(b, reactionB);
                    if (pushedA || pushedB) {
                        contacts[i][j] = contacts[j][i] = 1;
                    }
                }
            }
        }
    }
}

#pragma force_active reset

// Host: the Pokemon in the ring loses stamina; when it runs out, the Pokemon rests for 60 frames and comes back
// with 20. The members on the bench get theirs back (0.2 a frame).
void PokeathlonRingDrop_UpdateStamina(PokeathlonRingDropPlayer *players, PokeathlonCourseData *course) {
    int i;
    int member;

    for (i = 0; i < RING_DROP_PARTICIPANTS; players++, i++) {
        PokeathlonRingDropMon *mon = &players->mon;
        PokeathlonRingDropMember *teamMember;
        member = 0;
        teamMember = players->mon.members;
        do {
            int max;
            float refill = 0.0f;
            max = teamMember->params->stamina;
            if (mon->flags.bits.member == member) {
                if (mon->flags.bits.exhausted) {
                    mon->restFrames--;
                    if (mon->restFrames <= 0) {
                        teamMember->stamina = FX32_CONST(20);
                        mon->destination = mon->position;
                        mon->flags.bits.exhausted = 0;
                    }
                } else if (teamMember->stamina <= 0) {
                    mon->restFrames = 60;
                    mon->flags.bits.exhausted = 1;
                    mon->flags.bits.jumpRequested = 0;
                    mon->flags.bits.hasTarget = 0;
                    PokeathlonCourse_AddMonCounter(mon->course, (u8)mon->flags.bits.participant, (u8)mon->flags.bits.member, 1, 1);
                }
            } else {
                refill = 819.0f;
            }
            teamMember->stamina = (int)((float)teamMember->stamina + refill);
            if (teamMember->stamina > max) {
                teamMember->stamina = max;
            }
            member++;
            teamMember++;
        } while (member < RING_DROP_TEAM_SIZE);
    }
}

static inline float PokeathlonRingDrop_ToFx32Rounded(s16 x) {
    if (x > 0) {
        return 0.5f + (float)(x << FX32_SHIFT);
    }
    return (float)(x << FX32_SHIFT) - 0.5f;
}

static inline float PokeathlonRingDrop_TouchRadius(void) {
    if (PokeathlonCourse_GetTouchRadius() > 0) {
        return 0.5f + (float)(PokeathlonCourse_GetTouchRadius() << FX32_SHIFT);
    }
    return (float)(PokeathlonCourse_GetTouchRadius() << FX32_SHIFT) - 0.5f;
}

static inline int PokeathlonRingDrop_Abs(int x) {
    return x < 0 ? -x : x;
}

// Host: applies the participants' touches (touching the switch button switches; touching the own Pokemon and
// dragging makes it walk; a short flick from it dashes, a short tap on it jumps), then moves every Pokemon.
#pragma force_active on

void PokeathlonRingDrop_UpdateMons(PokeathlonRingDropPlayer *players, PokeathlonCourseData *course) {
    int i;
    for (i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        PokeathlonRingDropPlayer *player = &players[i];
        VecFx32 touch = { 0, 0, 0 }, touchStart = { 0, 0, 0 };
        PokeathlonRingDropTouch *record = &player->touch;
        PokeathlonRingDropMon *mon = &player->mon;
        touch.x = (int)PokeathlonRingDrop_ToFx32Rounded(player->touchX);
        touch.y = (int)PokeathlonRingDrop_ToFx32Rounded(player->touchY);
        touchStart.x = (int)PokeathlonRingDrop_ToFx32Rounded(player->touchStartX);
        touchStart.y = (int)PokeathlonRingDrop_ToFx32Rounded(player->touchStartY);
        if (record->started) {
            if (PokeathlonRingDrop_IsOnSwitchButton(player->touchX, player->touchY)) {
                if (PokeathlonRingDropMon_CanSwitch(mon)) {
                    PokeathlonRingDropMon_StartSwitch(mon);
                }
            } else {
                int radius = (int)PokeathlonRingDrop_TouchRadius();
                if (!mon->flags.bits.switching && mon->state != RING_DROP_MON_FALL_OUT && PokeathlonRingDrop_IsWithinDistance(&mon->position, &touch, radius)) {
                    record->onMon = 1;
                }
            }
        } else if (record->touching) {
            if (record->onMon == 1) {
                s16 x = mon->position.x >> FX32_SHIFT, y = mon->position.y >> FX32_SHIFT;
                if (mon->state == RING_DROP_MON_FALL_OUT) {
                    record->onMon = 0;
                } else if (PokeathlonRingDrop_Abs(player->touchX - x) > 1 || PokeathlonRingDrop_Abs(player->touchY - y) > 1) {
                    PokeathlonRingDropMon_SetDestination(mon, &player->touchX);
                }
            }
        } else if (record->lastFrames) {
            if (record->onMon == 1 && (mon->state == RING_DROP_MON_STAND || mon->state == RING_DROP_MON_RETURN) && !mon->flags.bits.exhausted && !mon->flags.bits.switching) {
                if (PokeathlonRingDrop_IsWithinDistance(&touchStart, &touch, FX32_CONST(4))) {
                    if (record->lastFrames <= 7) {
                        mon->flags.raw = (mon->flags.raw & ~0x40000000u) | 0x20000000u;
                    }
                } else if (!PokeathlonRingDrop_IsWithinDistance(&touchStart, &touch, FX32_CONST(12)) && record->lastFrames <= 7) {
                    mon->flags.raw = (mon->flags.raw | 0x40000000u) & ~0x20000000u;
                    mon->targetX = *(u16 *)&player->touchX;
                    mon->targetY = *(u16 *)&player->touchY;
                }
            }
            record->onMon = 0;
        }
        PokeathlonRingDropMon_Update(&player->mon, course);
    }
    PokeathlonRingDrop_UpdateCollisions(course);
    PokeathlonRingDrop_UpdateStamina(players, course);
}

#pragma force_active reset

// Host: writes the Pokemon into the frame packet to send.
// (The work is reached from its contacts field, which the players follow.)
#define RING_DROP_CONTACTS_TO(field) (offsetof(PokeathlonRingDrop, field) - offsetof(PokeathlonRingDrop, contacts))

void PokeathlonRingDrop_BuildFramePacket(u8 (*contacts)[RING_DROP_PARTICIPANTS], PokeathlonCourseData *course) {
    PokeathlonRingDropMonPacket *packets;
    int bumped;
    PokeathlonRingDropMon *mon;
    PokeathlonRingDropMonPacket *packet;
    int i;
    PokeathlonRingDropFrameMarks *marks;

    bumped = 0;
    packets = (void *)PokeathlonCourse_ToBytes((u8 *)PokeathlonCourse_GetDataCopyArea(course) + RING_DROP_DATA_FRAME_OUT);
    for (i = 0, mon = (PokeathlonRingDropMon *)((u8 *)contacts + RING_DROP_CONTACTS_TO(players[0].mon)), packet = packets; i < RING_DROP_PARTICIPANTS; i++, mon = (PokeathlonRingDropMon *)((u8 *)mon + sizeof(PokeathlonRingDropPlayer)), packet++) {
        s32 x = (mon->position.x << 4) >> 16;
        s32 y = (mon->position.y << 4) >> 16;
        if (mon->hitMarkFrames != 0) {
            mon->hitMarkFrames--;
        } else if (mon->hitMarkFrames == 0) {
            mon->flags.bits.attacker = 0;
            mon->flags.bits.hit = 0;
        }
        packet->attacker = mon->flags.bits.attacker;
        packet->hit = mon->flags.bits.hit;
        packet->member = mon->flags.bits.member;
        packet->direction = mon->flags.bits.direction - 1;
        if (x > 255) {
            x = 255;
        } else if (x < 0) {
            x = 0;
        }
        packet->x = x;
        if (y > 255) {
            y = 255;
        } else if (y < 0) {
            y = 0;
        }
        packet->y = y;
        packet->height = mon->height >> FX32_SHIFT;
        packet->state = mon->state;
        packet->score = mon->flags.bits.score;
        packet->switchState = mon->flags.bits.switchState;
        packet->status = mon->status;
        if (mon->flags.bits.bumped) {
            bumped = 1;
            mon->flags.bits.bumped = 0;
        }
    }
    marks = &((PokeathlonRingDropFramePacket *)packets)->marks;
    marks->raw = (marks->raw & ~0x80) | (((u32)bumped << 31) >> 24);
    marks->raw = (u8)marks->raw;
    for (i = 0; i < 12; i++) {
        int participant = i / 3;
        int member = i % 3;
        s32 stamina = *(fx32 *)((u8 *)contacts + participant * sizeof(PokeathlonRingDropPlayer) + member * sizeof(PokeathlonRingDropMember) + RING_DROP_CONTACTS_TO(players[0].mon.members[0].stamina)) >> FX32_SHIFT;
        int mark;
        if (stamina == 0) {
            mark = 2;
        } else if (stamina < 30) {
            mark = 1;
        } else {
            mark = 0;
        }
        marks->bits.members += mark << (i * 2);
    }
}

// Centres the affine transforms of a mon object on its body (or on its feet).
void PokeathlonRingDrop_SetMonAffineCenter(PokeathlonCourseMonObject *object, BOOL centered) {
    PokeathlonCourseMonGfx *gfx = PokeathlonCourseMonObject_GetGfx(object);
    int top = PokeathlonCourseMonGfx_GetTopRow(gfx);
    VecFx32 center = { 0 };
    int size = PokeathlonCourseMonGfx_GetInfo(gfx)->isLarge != 0 ? 0x40 : 0x20;
    if (centered) {
        center.x = (size / 2) << FX32_SHIFT;
        center.y = (top + (size - top) / 2) << FX32_SHIFT;
    } else {
        center.x = (size / 2) << FX32_SHIFT;
        center.y = size << FX32_SHIFT;
    }
    PokeathlonCourseMonObject_SetAffineMatrix(object, &center);
}

void PokeathlonRingDropMon_SetState(PokeathlonRingDropMon *mon, int state) {
    mon->returnFrames = state == RING_DROP_MON_RETURN ? 60 : 0;
    if (state == RING_DROP_MON_DASH_END) {
        mon->dashFrames = 16;
    }
    mon->state = state;
}

// Whether two Pokemon touch: their heights differ by at most 5 and their collision circles overlap (a dash makes
// the circle 8 bigger).
#pragma force_active on

BOOL PokeathlonRingDropMon_Collide(PokeathlonRingDropMon *a, PokeathlonRingDropMon *b) {
    VecFx32 delta, centerA, centerB;
    int radiusA, radiusB;
    fx32 height = a->height - b->height;
    if (height < 0) {
        height = -height;
    }
    if (height > 5 * FX32_ONE) {
        return FALSE;
    }
    PokeathlonCourseObject_GetCollisionCircle(a->member->object, a->position.x, a->position.y, &centerA.x, &centerA.y, &radiusA);
    PokeathlonCourseObject_GetCollisionCircle(b->member->object, b->position.x, b->position.y, &centerB.x, &centerB.y, &radiusB);
    if (a->state == RING_DROP_MON_DASH) {
        radiusA += 8;
    }
    if (b->state == RING_DROP_MON_DASH) {
        radiusB += 8;
    }
    fx32 radius = (radiusA + radiusB) * FX32_ONE;
    centerA.z = 0;
    centerB.z = 0;
    VEC_Subtract(&centerA, &centerB, &delta);
    if (VEC_Mag(&delta) <= radius) {
        return TRUE;
    }
    return FALSE;
}

// The switch button works while the Pokemon stands on its participant's switch spot.
BOOL PokeathlonRingDropMon_CanSwitch(PokeathlonRingDropMon *mon) {
    if (PokeathlonRingDropMon_CanAct(mon)) {
        VecFx32 spot = { 0, 0, 0 };
        u8 participant = mon->flags.bits.participant;
        spot.x = gPokeathlonRingDropSwitchSpots[participant].x * FX32_ONE;
        spot.y = ov96_0221D70A[participant].x * FX32_ONE;
        return PokeathlonRingDrop_IsWithinDistance(&mon->position, &spot, ov96_0221D70C[participant].x * FX32_ONE);
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokeathlonRingDropMon_CanAct(PokeathlonRingDropMon *mon) {
    if (mon->flags.bits.pushed) {
        return FALSE;
    }
    if (mon->flags.bits.exhausted) {
        return FALSE;
    }
    if (mon->flags.bits.switching) {
        return FALSE;
    }
    return mon->state == RING_DROP_MON_STAND;
}

void PokeathlonRingDropMon_SetDestination(PokeathlonRingDropMon *mon, const s16 *position) {
    mon->destination.x = position[0] << FX32_SHIFT;
    mon->destination.y = position[1] << FX32_SHIFT;
}

// 1..4 by the angle of vec (x, y): 4 within 45 degrees of gPokeathlonRingDropDirectionReference, 2 and 3 the next quarters, 1 the last.
#pragma force_active on

int PokeathlonRingDrop_VecToDirection(const VecFx32 *vec) {
    VecFx32 reference = gPokeathlonRingDropDirectionReference;
    VecFx32 v = { 0, 0, 0 };
    u16 angle;
    v.x = vec->y;
    v.z = vec->x;
    v.y = 0;
    angle = CalcAngleBetweenVecs(&reference, &v);
    if (angle <= 0x2000 || angle >= 0xE000) {
        return 4;
    }
    if (angle > 0x2000 && angle < 0x6000) {
        return 2;
    }
    if (angle >= 0x6000 && angle <= 0xA000) {
        return 3;
    }
    return 1;
}

BOOL PokeathlonRingDropMon_CanCollide(PokeathlonRingDropMon *mon) {
    GF_ASSERT(mon);
    GF_ASSERT(mon->state);
    return mon->state != RING_DROP_MON_JUMP_TOP && !mon->flags.bits.switching && mon->state != RING_DROP_MON_RETURN && mon->state != RING_DROP_MON_FALL_OUT;
}

#pragma force_active reset

void PokeathlonRingDrop_ClearContacts(PokeathlonCourseData *course, s32 participant) {
    u8 *contact = ((PokeathlonRingDrop *)PokeathlonCourse_GetHeapAllocPtr4(course))->contacts[participant];
    s32 i = 0;
    do {
        i++;
        *contact = 0;
        contact++;
    } while (i < RING_DROP_PARTICIPANTS);
}

// A pushed Pokemon slides (its velocity decays) until it stops; otherwise it walks to its destination at its
// speed (half of it below 30 stamina).
#pragma force_active on

void PokeathlonRingDropMon_Move(PokeathlonRingDropMon *mon, PokeathlonCourseData *course) {
    VecFx32 delta, direction, move, decay;
    fx32 distance = 0, speed;
    int moving;

    VEC_Subtract(&mon->destination, &mon->position, &delta);
    distance = VEC_Mag(&delta);
    moving = distance > 0 ? 1 : 0;
    if (mon->flags.bits.pushed) {
        if (mon->attackerState == 0) {
            GF_AssertFail();
        }
        if (mon->attackerState == RING_DROP_MON_DASH || mon->attackerState == RING_DROP_MON_JUMP_RISE) {
            speed = FX32_ONE;
        } else {
            speed = 0xB33;
        }
        if (mon->staggerFrames <= 0) {
            VecFx32 normal = { 0, 0, 0 }, zero = { 0, 0, 0 };
            VEC_Normalize(&mon->velocity, &normal);
            VEC_MultAdd(speed, &normal, &zero, &decay);
            decay.x = decay.x < 0 ? -decay.x : decay.x;
            decay.y = decay.y < 0 ? -decay.y : decay.y;
            PokeathlonRingDrop_DecayVelocity(&mon->velocity, decay.x, decay.y);
        }
        VEC_Add(&mon->position, &mon->velocity, &mon->position);
        mon->destination = mon->position;
        if (mon->velocity.x != 0) {
            return;
        }
        if (mon->velocity.y != 0) {
            return;
        }
        PokeathlonRingDrop_ClearContacts(course, (u8)mon->flags.bits.participant);
        mon->attacker = NULL;
        mon->flags.raw &= 0xF7FFFFFF;
    }
    if (!mon->flags.bits.exhausted && mon->staggerFrames <= 0 && moving) {
        VEC_Normalize(&delta, &direction);
        if (!mon->flags.bits.pushed) {
            mon->flags.bits.direction = PokeathlonRingDrop_VecToDirection(&direction);
        }
        speed = mon->member->params->speed;
        if (mon->member->stamina < 30 * FX32_ONE) {
            speed = FX_Mul(speed, FX32_HALF);
        }
        if (distance <= speed) {
            move = delta;
        } else {
            VecFx32 zero = { 0, 0, 0 };
            VEC_MultAdd(speed, &direction, &zero, &move);
        }
        VEC_Add(&mon->position, &move, &mon->position);
        mon->dashDirection = direction;
    }
}

static inline void PokeathlonRingDrop_LoadRingEdge(VecFx32 *points) {
    VecFx32 *cursor;
    const s16(*source)[2] = gPokeathlonRingDropRingEdge;
    int i = 0;
    cursor = points;
    for (; i < 4; i++) {
        cursor->x = (s32)((u32)(s32)(*source)[0] << FX32_SHIFT);
        cursor->y = (s32)((u32)(s32)(*source)[1] << FX32_SHIFT);
        source++;
        cursor++;
    }
}

BOOL PokeathlonRingDropMon_IsOutOfRing(PokeathlonRingDropMon *mon) {
    VecFx32 points[4];
    int x, y;
    if (mon->position.x <= 0 || mon->position.y <= 0) {
        return TRUE;
    }
    PokeathlonCourseObject_CenterToGround(mon->member->object, mon->position.x >> FX32_SHIFT, mon->position.y >> FX32_SHIFT, &x, &y);
    if (y >= 64 && y <= 160) {
        int groundX = FX32_CONST(x);
        int groundY = FX32_CONST(y);
        PokeathlonRingDrop_LoadRingEdge(points);
        int left = FX_Div(FX_Mul(points[1].y - points[0].y, groundX - points[0].x), points[1].x - points[0].x);
        left += points[0].y;
        int right = FX_Div(FX_Mul(points[3].y - points[2].y, groundX - points[2].x), points[3].x - points[2].x);
        right += points[2].y;
        if (groundY >= left && groundY >= right) {
            return FALSE;
        }
    }
    return TRUE;
}

#pragma force_active reset
