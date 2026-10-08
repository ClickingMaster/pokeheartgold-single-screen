#pragma force_active on
#include "global.h"

typedef struct Opponent {
    void *sprite;
    u8 unk4[0x2c];
    VecFx32 pos;
    u8 unk3C[0xc];
    int busy;
    u8 unk4C[0x2c];
    int active;
} Opponent;

typedef struct Target {
    u8 unk0[8];
    VecFx32 pos;
    u8 unk14[0x24];
    u8 kind;
    u8 disabled;
    u8 unk3A[0x12];
} Target;

typedef struct PlayerInfo {
    u8 unk0[0x71];
    u8 level;
} PlayerInfo;

typedef struct Participant {
    u32 unk0;
    PlayerInfo *info;
    u8 unk8[0x1c];
} Participant;

int ov96_02215614(int x, int y, int otherX, int otherY);
BOOL ov96_02215650(int x1, int y1, int x2, int y2, int quadrant, int index);
BOOL ov96_022156E8(VecFx32 *pos, fx32 radius, VecFx32 *otherPos, fx32 otherRadius);
int ov96_021EAF8C(void *sprite);
int ov96_02214DBC(Opponent *opp, Target *target, u8 state, int quadrant);
int ov96_02215478(Opponent *opp, Participant *participants, u8 index);
int ov96_02215460(Opponent *opp, Target *target, u8 state, u8 index, Participant *participants);
void ov96_021E8228(void *course, u8 index, u8 slot, int event, int value);
int ov96_02215184(void *course, u8 index, u8 slot, Opponent *opp, Target *targets, Participant *participants);

// @fn ov96_02215184
int ov96_02215184(void *course, u8 index, u8 slot, Opponent *opp, Target *targets, Participant *participants) {
    u8 i;
    int current;
    int quadrant;
    int radius;
    int result;
    fx32 dist;
    Target *target;
    u8 states[2];
    u8 quadrants[2];
    VecFx32 delta;
    u8 state0;
    u8 state1;
    BOOL check0;
    BOOL check1;

    if (opp->active) {
        return 0;
    }
    if (opp->busy) {
        return 0;
    }
    quadrant = ov96_02215614(128, 96, opp->pos.x / FX32_ONE, opp->pos.y / FX32_ONE);
    for (i = 0; i < 2; i++) {
        target = &targets[i];
        if (target->disabled == 0 && target->kind != 0) {
            quadrants[i] = ov96_02215614(128, 96, target->pos.x / FX32_ONE, target->pos.y / FX32_ONE);
        } else {
            quadrants[i] = 4;
        }
    }
    for (i = 0; i < 2; i++) {
        target = &targets[i];
        states[i] = 8;
        current = quadrants[i];
        if (current == 4) {
            continue;
        }
        radius = 0;
        if (target->kind == 1) {
            radius = 8;
        } else if (target->kind == 2) {
            radius = 12;
        } else {
            GF_AssertFail();
        }
        if (ov96_022156E8(&opp->pos, ov96_021EAF8C(opp->sprite) << 12, &target->pos, radius << 12)) {
            states[i] = 8;
        } else if (quadrant == current) {
            if (ov96_02215650(target->pos.x / FX32_ONE, target->pos.y / FX32_ONE, opp->pos.x / FX32_ONE, opp->pos.y / FX32_ONE, quadrant, index)) {
                states[i] = 13;
            }
        }
    }
    if (states[0] == 13) {
        return ov96_02214DBC(opp, targets, states[0], quadrant);
    }
    if (states[1] == 13) {
        targets++;
        return ov96_02214DBC(opp, targets, states[1], quadrant);
    }
    if (participants[index].info->level >= 50) {
        return ov96_02215478(opp, participants, index);
    }
    for (i = 0; i < 2; i++) {
        target = &targets[i];
        if (quadrants[i] != 4) {
            if (index == quadrants[i]) {
                if (ov96_02215650(target->pos.x / FX32_ONE, target->pos.y / FX32_ONE, opp->pos.x / FX32_ONE, opp->pos.y / FX32_ONE, quadrant, index)) {
                    states[i] = 12;
                } else {
                    states[i] = 11;
                }
            } else {
                states[i] = 11;
            }
        } else {
            states[i] = 10;
        }
    }
    state0 = states[0];
    state1 = states[1];
    check0 = state0 == 7 ? TRUE : FALSE;
    check1 = state1 == 7 ? TRUE : FALSE;
    if (check0 == check1) {
        VEC_Subtract(&opp->pos, &targets[0].pos, &delta);
        dist = VEC_Mag(&delta);
        VEC_Subtract(&opp->pos, &targets[1].pos, &delta);
        if (dist >= VEC_Mag(&delta)) {
            result = ov96_02215460(opp, targets, states[0], index, participants);
        } else {
            targets++;
            result = ov96_02215460(opp, targets, state1, index, participants);
        }
    } else if (state0 >= state1) {
        result = ov96_02215460(opp, targets, state0, index, participants);
    } else {
        targets++;
        result = ov96_02215460(opp, targets, state1, index, participants);
    }
    if (opp->active == 1) {
        ov96_021E8228(course, index, slot, 6, 1);
    }
    return result;
}
