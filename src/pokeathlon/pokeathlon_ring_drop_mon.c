#include "global.h"

#include "pokeathlon/pokeathlon_ring_drop.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "sound_playback.h"
#include "string_util.h"
#include "text.h"

// Course helpers and mon objects (OVY_96).
void PokeathlonCourseObject_GroundToCenter(void *object, int x, int y, int *outX, int *outY);
void PokeathlonCourse_AddMonCounter(void *course, u8 participant, u8 member, int counter, int amount);
u8 *PokeathlonCourse_GetMonStats(void *course, int participant, int member);
BOOL PlaySEOnPlayer(u16 seqNo, int player);

extern const s16 ov96_0221D6E4[];
extern const s16 ov96_0221D6E6[];
// Jump heights by frame (jump type, frame) and fall speeds by jump type.
extern const s16 ov96_0221D7D0[5][10];
extern const u8 ov96_0221D69C[8];
extern const WindowTemplate ov96_0221D6A4;

void PokeathlonRingDropMon_Respawn(PokeathlonRingDropMon *mon) {
    PokeathlonRingDropMember *member = mon->member;
    member->stamina = member->params->stamina;
    PokeathlonRingDropMon_NextMember(mon);
    PokeathlonRingDropMon_ResetToStart(mon);
}

#pragma force_active on

void PokeathlonRingDropMon_ResetToStart(PokeathlonRingDropMon *mon) {
    int x, y;
    PokeathlonCourseObject_GroundToCenter(mon->member->object, ov96_0221D6E4[mon->flags.bits.participant * 2], ov96_0221D6E6[mon->flags.bits.participant * 2], &x, &y);
    mon->position.x = x << FX32_SHIFT;
    mon->position.y = y << FX32_SHIFT;
    mon->destination.x = x << FX32_SHIFT;
    mon->destination.y = y << FX32_SHIFT;
    {
        VecFx32 zero = { 0, 0, 0 };
        mon->velocity = zero;
    }
    mon->flags.bits.direction = 2;
    mon->flags.bits.pushed = 0;
    mon->flags.bits.jumpRequested = 0;
    mon->flags.bits.hasTarget = 0;
}

#pragma force_active reset

// The height of a jump falls back (faster every frame) except while jumping.
void PokeathlonRingDropMon_ApplyGravity(PokeathlonRingDropMon *mon) {
    if ((u32)(mon->state - RING_DROP_MON_JUMP_RISE) <= RING_DROP_MON_JUMP_FALL - RING_DROP_MON_JUMP_RISE) {
        return;
    }
    if (mon->height <= 0) {
        mon->fallSpeed = 0;
        return;
    }
    mon->height -= mon->fallSpeed;
    if (mon->height < 0) {
        mon->height = 0;
    }
    mon->fallSpeed += FX32_CONST(2);
}

// Host: a Pokemon's frame. Pokemon that leave the ring fall out; a jump or dash asked for starts when the
// Pokemon stands.
#pragma force_active on

void PokeathlonRingDropMon_Update(PokeathlonRingDropMon *mon, PokeathlonCourseData *course) {
    if (mon->hitFrames > 0) {
        int frames = mon->hitFrames - 1;
        mon->hitFrames = frames;
        if (mon->hitFrames > 0) {
            goto done;
        }
    }
    switch (mon->state) {
    case RING_DROP_MON_DASH:
        PokeathlonRingDropMon_UpdateDash(mon);
        break;
    case RING_DROP_MON_DASH_START:
    case RING_DROP_MON_DASH_END:
    case RING_DROP_MON_JUMP_START:
    case RING_DROP_MON_JUMP_TOP:
    case RING_DROP_MON_JUMP_FALL:
    case RING_DROP_MON_LANDING:
        break;
    default:
        PokeathlonRingDropMon_Move(mon, course);
        break;
    }
    switch (mon->state) {
    case RING_DROP_MON_RETURN:
        if (mon->flags.bits.pushed) {
            GF_AssertFail();
        }
        if (PokeathlonRingDropMon_IsOutOfRing(mon)) {
            PokeathlonRingDropMon_FallOut(mon);
        } else {
            if (mon->flags.bits.jumpRequested) {
                PokeathlonRingDropMon_StartJump(mon);
            } else if (mon->flags.bits.hasTarget) {
                PokeathlonRingDropMon_StartDash(mon, &mon->targetX);
            } else {
                mon->returnFrames--;
                if (mon->returnFrames <= 0) {
                    PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_STAND);
                }
            }
        }
        break;
    case RING_DROP_MON_FALL_OUT:
        mon->fallOutFrames--;
        if (mon->fallOutFrames <= 0) {
            PokeathlonRingDropMon_Respawn(mon);
            PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_RETURN);
        }
        break;
    case RING_DROP_MON_STAGGER:
        if (PokeathlonRingDropMon_IsOutOfRing(mon)) {
            PokeathlonRingDropMon_FallOut(mon);
        } else {
            mon->staggerFrames--;
            if (mon->staggerFrames <= 0) {
                PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_STAND);
            }
        }
        break;
    case RING_DROP_MON_STAND:
        if (PokeathlonRingDropMon_IsOutOfRing(mon)) {
            PokeathlonRingDropMon_FallOut(mon);
        } else if (!mon->flags.bits.pushed) {
            if (mon->flags.bits.jumpRequested) {
                PokeathlonRingDropMon_StartJump(mon);
            } else if (mon->flags.bits.hasTarget) {
                PokeathlonRingDropMon_StartDash(mon, &mon->targetX);
            }
        }
        break;
    case RING_DROP_MON_JUMP_START:
        mon->jumpFrames--;
        if (mon->jumpFrames <= 0) {
            mon->jumpFrames = 0;
            PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_JUMP_RISE);
        }
        break;
    case RING_DROP_MON_JUMP_RISE:
        mon->height = (s32)((u32)(s32)ov96_0221D7D0[mon->member->params->jumpType][mon->jumpFrames] << FX32_SHIFT);
        mon->jumpFrames++;
        if (mon->jumpFrames >= 10) {
            mon->jumpFrames = 10;
            PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_JUMP_TOP);
        }
        break;
    case RING_DROP_MON_JUMP_TOP:
        mon->jumpFrames--;
        if (mon->jumpFrames <= 0) {
            mon->jumpFrames = 0;
            PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_JUMP_FALL);
        }
        break;
    case RING_DROP_MON_JUMP_FALL:
        if (mon->height == 0) {
            PokeathlonRingDropMon_Land(mon);
            if (PokeathlonRingDropMon_IsOutOfRing(mon)) {
                PokeathlonRingDropMon_FallOut(mon);
            }
        } else {
            mon->height = (s32)((u32)mon->height - ((u32)ov96_0221D69C[mon->member->params->jumpType] << FX32_SHIFT));
            if (mon->height <= 0) {
                mon->height = 0;
            }
        }
        break;
    case RING_DROP_MON_LANDING:
        mon->jumpFrames--;
        if (mon->jumpFrames <= 0) {
            PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_STAND);
        }
        break;
    case RING_DROP_MON_DASH_START:
        mon->dashFrames--;
        if (mon->dashFrames <= 0) {
            mon->dashFrames = 6;
            PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_DASH);
        }
        break;
    case RING_DROP_MON_DASH:
        mon->dashFrames--;
        if (mon->dashFrames <= 0) {
            VecFx32 zero;
            mon->destination = mon->position;
            zero = (VecFx32) { 0, 0, 0 };
            mon->velocity = zero;
            PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_DASH_END);
        }
        break;
    case RING_DROP_MON_DASH_END:
        mon->dashFrames--;
        if (mon->dashFrames <= 0) {
            PokeathlonRingDropMon_EndDash(mon);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    PokeathlonRingDropMon_ApplyGravity(mon);
done:;
}

#pragma force_active reset

// A dash towards target (ground pixels): 4 frames of wind-up, then 6 frames of speeding up.
void PokeathlonRingDropMon_StartDash(PokeathlonRingDropMon *mon, const s16 *target) {
    VecFx32 vec = { 0, 0, 0 };
    BOOL valid = TRUE;
    if (mon->state != RING_DROP_MON_STAND && mon->state != RING_DROP_MON_RETURN) {
        valid = FALSE;
    }
    if (!valid) {
        GF_AssertFail();
    }
    if (mon->flags.bits.exhausted) {
        GF_AssertFail();
    }
    if (mon->flags.bits.switching) {
        GF_AssertFail();
    }
    mon->flags.raw &= 0xBFFFFFFF;
    vec.x = target[0] << FX32_SHIFT;
    vec.y = target[1] << FX32_SHIFT;
    VEC_Subtract(&vec, &mon->position, &mon->dashDirection);
    VEC_Normalize(&mon->dashDirection, &mon->dashDirection);
    VEC_MultAdd(FX32_CONST(4), &mon->dashDirection, &mon->dashDirection, &mon->velocity);
    if (mon->velocity.x != 0 || mon->velocity.y != 0) {
        mon->flags.bits.direction = PokeathlonRingDrop_VecToDirection(&mon->dashDirection);
        mon->dashFrames = 4;
        PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_DASH_START);
    }
}

void PokeathlonRingDropMon_EndDash(PokeathlonRingDropMon *mon) {
    BOOL valid = TRUE;
    if (mon->state != RING_DROP_MON_DASH_END && mon->state != RING_DROP_MON_DASH) {
        valid = FALSE;
    }
    if (!valid) {
        GF_AssertFail();
    }
    VecFx32 zero = { 0, 0, 0 };
    mon->velocity = zero;
    mon->destination = mon->position;
    PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_STAND);
    if (PokeathlonRingDropMon_IsOutOfRing(mon)) {
        PokeathlonRingDropMon_FallOut(mon);
    }
}

void PokeathlonRingDropMon_UpdateDash(PokeathlonRingDropMon *mon) {
    VecFx32 step;
    VecFx32 zero = { 0, 0, 0 };
    VEC_MultAdd(0xB33, &mon->dashDirection, &zero, &step);
    VEC_Add(&mon->velocity, &step, &mon->velocity);
    VEC_Add(&mon->position, &mon->velocity, &mon->position);
}

// A jump: 5 frames of wind-up, 10 rising, 10 at the top, then it falls and presses.
#pragma force_active on

void PokeathlonRingDropMon_StartJump(PokeathlonRingDropMon *mon) {
    GF_ASSERT(mon->state == RING_DROP_MON_STAND || mon->state == RING_DROP_MON_RETURN);
    GF_ASSERT(mon->flags.bits.exhausted == 0);
    GF_ASSERT(mon->flags.bits.switching == 0);
    mon->flags.raw &= 0xDFFFFFFF;
    mon->jumpFrames = 5;
    if (mon->member->params->jumpType) {
        PokeathlonCourse_AddMonCounter(mon->course, (u8)mon->flags.bits.participant, (u8)mon->flags.bits.member, 2, 1);
    }
    PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_JUMP_START);
}

#pragma force_active reset

void PokeathlonRingDropMon_Land(PokeathlonRingDropMon *mon) {
    PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_LANDING);
    mon->jumpFrames = 10;
    mon->height = 0;
    mon->destination = mon->position;
}

// The less stamina is left, the longer the Pokemon staggers.
void PokeathlonRingDropMon_Stagger(PokeathlonRingDropMon *mon) {
    s32 frames = FX_Div(mon->member->stamina, mon->member->params->stamina);
    frames = (10 * (FX32_ONE - frames)) - 3;
    if (frames < 0) {
        frames = 0;
    }
    mon->staggerFrames = frames >> FX32_SHIFT;
    PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_STAGGER);
}

// The Pokemon falls out of the ring: the last attacker gets 10 points, or the Pokemon loses 10 if nobody pushed it.
#pragma force_active on

void PokeathlonRingDropMon_FallOut(PokeathlonRingDropMon *mon) {
    PokeathlonRingDropMon *attacker;
    int score;
    mon->fallOutFrames = 60;
    mon->staggerFrames = 0;
    attacker = mon->attacker;
    if (attacker == NULL) {
        score = mon->flags.bits.score - 10;
        if (score < 0) {
            score = 0;
        }
        mon->flags.bits.score = score;
        PokeathlonCourse_AddMonCounter(mon->course, mon->flags.bits.participant, mon->flags.bits.member, 8, 1);
    } else {
        score = attacker->flags.bits.score + 10;
        if (score > 200) {
            score = 200;
        }
        attacker->flags.bits.score = score;
        PokeathlonCourse_AddMonCounter(attacker->course, attacker->flags.bits.participant, attacker->flags.bits.member, 3, 1);
        mon->attacker = NULL;
    }
    PokeathlonCourse_AddMonCounter(mon->course, mon->flags.bits.participant, mon->flags.bits.member, 5, 1);
    PokeathlonCourse_AddMonCounter(mon->course, mon->flags.bits.participant, mon->flags.bits.member, 1, 1);
    PokeathlonRingDropMon_SetState(mon, RING_DROP_MON_FALL_OUT);
}

void PokeathlonRingDropMon_StartSwitch(PokeathlonRingDropMon *mon) {
    GF_ASSERT(!mon->flags.bits.switching);
    GF_ASSERT(!mon->flags.bits.exhausted);
    mon->flags.bits.switching = 1;
    mon->flags.bits.switchState = 1;
    mon->destination = mon->position;
    PokeathlonCourse_AddMonCounter(mon->course, mon->flags.bits.participant, mon->flags.bits.member, 7, TRUE);
}

#pragma force_active reset

void PokeathlonRingDropMon_DrainStamina(PokeathlonRingDropMon *mon, int amount) {
    mon->member->stamina -= amount;
    if (mon->member->stamina < 0) {
        mon->member->stamina = 0;
    }
}

// value as fx32, rounded away from zero
static inline float PokeathlonRingDrop_ToFx32(int value, float half) {
    float f;
    if (value > 0) {
        f = half + (float)(value << FX32_SHIFT);
    } else {
        f = (float)(value << FX32_SHIFT) - half;
    }
    return f;
}

// Every team member's numbers from its performance stats 0-3 (levels 0-4) and the table NARC 170 #9.
#pragma force_active on

void PokeathlonRingDrop_InitMemberParams(PokeathlonCourseData *course) {
    int table[6][5] = { 0 };
    ReadWholeNarcMemberByIdPair(table, (NarcId)170, 9);
    u8 participant;
    for (int i = 0; i < RING_DROP_PARTICIPANTS; i++) {
        int member = 0;
        float half = 0.5f;
        participant = (u8)i;
        for (; member < RING_DROP_TEAM_SIZE; member++) {
            u8 *stats = PokeathlonCourse_GetMonStats(course, i, member);
            PokeathlonRingDropMemberParams *params = ((PokeathlonRingDropMemberParams * (*)(PokeathlonCourseData *, u8, u8)) PokeathlonRingDrop_GetMemberParams)(course, participant, (u8)member);
            if (stats[0] >= 5) {
                GF_AssertFail();
            }
            if (stats[3] >= 5) {
                GF_AssertFail();
            }
            if (stats[1] >= 5) {
                GF_AssertFail();
            }
            if (stats[2] >= 5) {
                GF_AssertFail();
            }
            params->dashPower = (int)PokeathlonRingDrop_ToFx32(table[0][stats[0]], half) / 10;
            params->dashDamage = (int)PokeathlonRingDrop_ToFx32(table[1][stats[0]], half) / 10;
            params->stamina = (int)PokeathlonRingDrop_ToFx32(table[2][stats[3]], half);
            params->speed = (int)PokeathlonRingDrop_ToFx32(table[3][stats[1]], half) / 10;
            params->pressPower = (int)PokeathlonRingDrop_ToFx32(table[5][stats[2]], half) / 10;
            params->pressDamage = (int)PokeathlonRingDrop_ToFx32(table[5][stats[2]], half) / 10;
            params->jumpType = stats[2];
        }
    }
}

#pragma force_active reset

PokeathlonRingDropMemberParams *PokeathlonRingDrop_GetMemberParams(PokeathlonCourseData *course, u32 participant, u32 member) {
    u8 *params;
    u32 p = participant;
    u32 m = member;
    params = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (params == NULL) {
        GF_AssertFail();
    }
    if (p >= RING_DROP_PARTICIPANTS) {
        GF_AssertFail();
    }
    if (m >= RING_DROP_TEAM_SIZE) {
        GF_AssertFail();
    }
    params += offsetof(PokeathlonRingDrop, memberParams);
    params += (p * RING_DROP_TEAM_SIZE + m) * sizeof(PokeathlonRingDropMemberParams);
    if (params == NULL) {
        GF_AssertFail();
    }
    return (PokeathlonRingDropMemberParams *)params;
}

void PokeathlonRingDropMon_NextMember(PokeathlonRingDropMon *mon) {
    u32 member = (*(volatile PokeathlonRingDropMonFlags *)&mon->flags).bits.member;
    member = (member + 1) % RING_DROP_TEAM_SIZE;
    mon->flags.bits.member = member;
    mon->member = &mon->player->mon.members[mon->flags.bits.member];
}

// The own participant's sounds play on player 3, the others' on player 4.
BOOL PokeathlonRingDrop_PlaySE(u32 seqNo, BOOL own) {
    return PlaySEOnPlayer((u16)seqNo, own ? 3 : 4);
}

void PokeathlonRingDrop_AddScoreWindow(PokeathlonRingDrop *ringDrop) {
    AddWindow(ringDrop->bgConfig, &ringDrop->window, &ov96_0221D6A4);
}

void PokeathlonRingDrop_PrintScore(PokeathlonCourseData *course, u32 score) {
    PokeathlonRingDrop *ringDrop = PokeathlonCourse_GetHeapAllocPtr4(course);
    MessageFormat *msgFormat = ringDrop->msgFormat;
    MsgData *msgData = ringDrop->msgData;
    String *string;
    FillWindowPixelBuffer(&ringDrop->window, 0);
    BufferIntegerAsString(msgFormat, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, 0xA3, ringDrop->heapId);
    AddTextPrinterParameterizedWithColor(&ringDrop->window, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(&ringDrop->window);
}

// The look of the own switch button: 0 the Pokemon can switch, 1 it can but is tired (stamina below 30), 3 it
// cannot (it is not on its switch spot or busy); 2 and 4 while the button is touched.
void PokeathlonRingDropMon_UpdateStatus(PokeathlonRingDropMon *mon, PokeathlonRingDropTouch *touch, BOOL onSwitchButton) {
    PokeathlonRingDropMember *member = mon->member;
    PokeathlonRingDropMon *self = mon;
    s32 stamina = member->stamina >> FX32_SHIFT;
    u8 *status = &self->status;
    int tired;
    u32 newStatus;
    if (stamina < 30) {
        tired = 1;
    } else {
        tired = 0;
    }
    if (PokeathlonRingDropMon_CanSwitch(self)) {
        if (tired) {
            newStatus = 1;
        } else {
            newStatus = 0;
        }
    } else {
        newStatus = 3;
    }
    if (onSwitchButton == FALSE) {
        *status = newStatus;
        return;
    }
    if (touch->started) {
        if (newStatus <= 1) {
            *status = 2;
            return;
        }
        if (newStatus == 3) {
            *status = 4;
            return;
        }
        GF_AssertFail();
        return;
    }
    if (!touch->touching) {
        *status = newStatus;
    }
}
