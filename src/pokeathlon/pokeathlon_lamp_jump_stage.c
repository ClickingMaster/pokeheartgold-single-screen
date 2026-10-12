#include "pokeathlon/pokeathlon_lamp_jump_stage.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_results_announcement.h"

#include "assert.h"
#include "bg_window.h"
#include "camera.h"
#include "font.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "swap_3d_buffers.h"

// pkg13's fields of PokeathlonLampJump that this part names (pokeathlon_lamp_jump.h): same layout as before.
typedef char PokeathlonLampJumpSizeCheck[sizeof(PokeathlonLampJump) == 0x1004 ? 1 : -1];
typedef char PokeathlonLampJumpLayoutCheck[(offsetof(PokeathlonLampJump, unk64) == 0x64 && offsetof(PokeathlonLampJump, timerDigits) == 0x84 && offsetof(PokeathlonLampJump, nextEffect) == 0x143 && offsetof(PokeathlonLampJump, cameraTarget) == 0x194 && offsetof(PokeathlonLampJump, trampoline) == 0x1A0 && offsetof(PokeathlonLampJump, monColumns) == 0xFA0 && offsetof(PokeathlonLampJump, jumps) == 0xFAC && offsetof(PokeathlonLampJumpMon, combo) == 0x24 && offsetof(PokeathlonLampJumpMon, stunTimer) == 0x27 && offsetof(PokeathlonLampJumpMon, colliding) == 0x29 && offsetof(PokeathlonLampJumpMon, bounce) == 0x2C && offsetof(PokeathlonLampJumpMon, stunFrames) == 0x30 && offsetof(PokeathlonLampJumpJump, airborne) == 0x10) ? 1 : -1];

// sound_playback.c / sound_system.c (no header declares them)
BOOL PlaySEOnPlayer(u16 seqNo, int playerNo);
void GF_SndSetTrackPitchByPlayerNo(int playerNo, u32 tracks, int pitch);

// NitroSystem G3D
void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 num);
void NNS_G3dGlbFlushP(void);

// Course helpers and the sprite scene (OVY_96, not yet tidied).
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, int member);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int counter, int value);
void *ov96_021EA634(void *a0, SpriteList *spriteList, int a2, enum HeapID heapId);
void PokeathlonCourseObject_GetCollisionCircle(void *sprite, int x, int y, fx32 *outX, fx32 *outY, int *outRadius);
void PokeathlonCourseObject_GroundToCenter(void *sprite, int x, int y, int *outX, int *outY);
void PokeathlonCourseSpritePool_LoadChar(void *scene, NarcId narcId, int a2, int a3, int a4);
void PokeathlonCourseSpritePool_LoadPltt(void *scene, NarcId narcId, int a2, int a3, int a4, int a5);
void PokeathlonCourseSpritePool_LoadCell(void *scene, NarcId narcId, int a2, int a3);
void PokeathlonCourseSpritePool_LoadAnim(void *scene, NarcId narcId, int a2, int a3);
void *PokeathlonCourseSpritePool_CreateAndTake(void *scene, int a1, int a2, int a3, int a4);
void PokeathlonCourseSpritePoolSprite_Show(void *sprite, int a1, int a2);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *sprite, int anim);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *sprite, const VecFx32 *pos);
SpriteList *PokeathlonCourseSpritePool_GetSpriteList(void *scene);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(void *sprite, int priority);

extern int ov96_0221E5A0;         // red of the next trampoline vertex
extern const u8 ov96_0221C0B0[2]; // x of the timer digits
extern const u8 ov96_0221C0B4[2]; // animation of the timer digits
extern const VecFx32 ov96_0221C0C4;
extern const VecFx32 ov96_0221C0D0;
extern const VecFx32 ov96_0221C0DC;
extern const VecFx32 ov96_0221C0E8;
extern const VecFx32 ov96_0221C0F4;
extern const u32 ov96_0221C110[]; // pitch of the lamp sound per points
extern const WindowTemplate ov96_0221C1CC[5];
extern const u16 ov96_0221C21C[29][2]; // lamp positions, in half pixels
extern const WindowTemplate ov96_0221C2A0;
extern u8 ov96_0221DC18[]; // sound player of each Pokemon (not const: reloaded after every call)

#pragma force_active on

static int sTrampolineBlue = 20;
static int sTrampolineGreen = 10;

static inline void PokeathlonLampJump_GeVtx16(u32 xy, u32 z) {
    u32 args[2];

    args[0] = xy;
    args[1] = z;
    NNS_G3dGeBufferOP_N(G3OP_VTX_16, args, 2);
}

static inline void PokeathlonLampJump_GeColor(u32 rgb) {
    NNS_G3dGeBufferOP_N(G3OP_COLOR, &rgb, 1);
}

// A quad strip from the trampoline's surface down to y -1.5, each column in the next color.
void PokeathlonLampJump_DrawTrampoline(PokeathlonLampJump *lampJump) {
    VecFx32 scale = ov96_0221C0E8;
    u32 polygonAttr;
    u32 color;
    u32 primitive;
    int i;

    NNS_G3dGeBufferOP_N(G3OP_MTX_SCALE, (const u32 *)&scale, 3);
    polygonAttr = 0x310081;
    NNS_G3dGeBufferOP_N(G3OP_POLYGON_ATTR, &polygonAttr, 1);
    color = GX_RGB(31, 5, 0);
    NNS_G3dGeBufferOP_N(G3OP_COLOR, &color, 1);
    NNS_G3dGlbFlushP();
    primitive = GX_BEGIN_QUAD_STRIP;
    NNS_G3dGeBufferOP_N(G3OP_BEGIN, &primitive, 1);
    ov96_0221E5A0 = 0;
    sTrampolineGreen = 10;
    sTrampolineBlue = 20;
    PokeathlonLampJump_GeVtx16(0xE000, 0);
    PokeathlonLampJump_GeVtx16(0xE800E000, 0);
    for (i = 1; i <= 128; i++) {
        float x = ((float)i - 64.0) * 4.0 / 128.0;

        PokeathlonLampJump_GeVtx16((u16)(int)(x * 4096.0f) | ((u32)(u16)(s16)lampJump->trampoline.surface[i - 1] << 16), 0);
        PokeathlonLampJump_GeVtx16((u16)(int)(x * 4096.0f) | 0xE8000000, 0);
        PokeathlonLampJump_GeColor((u16)(ov96_0221E5A0 | (sTrampolineGreen << 5) | (sTrampolineBlue << 10)));
        ov96_0221E5A0 = (ov96_0221E5A0 + 1) % 31;
        sTrampolineGreen = (sTrampolineGreen + 1) % 31;
        sTrampolineBlue = (sTrampolineBlue + 1) % 31;
    }
    PokeathlonLampJump_GeVtx16(0xE000, 0);
    PokeathlonLampJump_GeVtx16(0xE800E000, 0);
    NNS_G3dGeBufferOP_N(G3OP_END, NULL, 0);
}

#pragma force_active reset

void PokeathlonLampJump_Draw3D(PokeathlonLampJump *lampJump) {
    Thunk_G3X_Reset();
    Camera_PushLookAtToNNSGlb();
    PokeathlonLampJump_DrawTrampoline(lampJump);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

#pragma force_active on

void PokeathlonLampJump_InitCamera(PokeathlonLampJump *lampJump) {
    VecFx32 up;
    CameraAngle angle;
    VecFx32 offset = ov96_0221C0F4;

    lampJump->cameraTarget.x = 0;
    lampJump->cameraTarget.y = 0;
    lampJump->cameraTarget.z = 0;
    angle.x = 0;
    angle.y = 0;
    angle.z = 0;
    Camera_Init_FromTargetDistanceAndAngle(&lampJump->cameraTarget, FX32_CONST(100), &angle, 164, 1, TRUE, lampJump->camera);
    Camera_OffsetLookAtPosAndTarget(&offset, lampJump->camera);
    Camera_SetPerspectiveClippingPlane(FX32_CONST(4), FX32_CONST(256), lampJump->camera);
    up.x = 0;
    up.y = FX32_ONE;
    up.z = 0;
    Camera_SetLookAtCamUp(&up, lampJump->camera);
    Camera_SetStaticPtr(lampJump->camera);
}

// The surface is the lowest of the three dents and the three lines between the Pokemon.
void PokeathlonLampJump_CalcTrampolineSurface(PokeathlonLampJumpTrampoline *trampoline) {
    int i;

    PokeathlonLampJump_CalcLine(trampoline, 0, 0, 1);
    PokeathlonLampJump_CalcLine(trampoline, 1, 1, 2);
    PokeathlonLampJump_CalcLine(trampoline, 2, 2, 0);
    for (i = 0; i < 128; i++) {
        trampoline->surface[i] = trampoline->dents[0][i];
        if (trampoline->surface[i] > trampoline->dents[1][i]) {
            trampoline->surface[i] = trampoline->dents[1][i];
        }
        if (trampoline->surface[i] > trampoline->dents[2][i]) {
            trampoline->surface[i] = trampoline->dents[2][i];
        }
        if (trampoline->surface[i] > trampoline->lines[0][i]) {
            trampoline->surface[i] = trampoline->lines[0][i];
        }
        if (trampoline->surface[i] > trampoline->lines[1][i]) {
            trampoline->surface[i] = trampoline->lines[1][i];
        }
        if (trampoline->surface[i] > trampoline->lines[2][i]) {
            trampoline->surface[i] = trampoline->lines[2][i];
        }
    }
}

#pragma force_active reset

// The surface from some of the parts only: bit 0 = dent 0 and line 0-1, bit 1 = dent 1 and line 1-2,
// bit 2 = dent 2 and line 2-0.
void PokeathlonLampJump_CalcPartialSurface(PokeathlonLampJumpTrampoline *trampoline, int parts, s32 *heights) {
    int i = 0;
    int part0 = 1, part1 = 2, part2 = 4;
    part0 &= parts;
    part1 &= parts;
    part2 &= parts;
    for (; i < 128; i++) {
        heights[i] = 0;
        if (part0) {
            if (heights[i] > trampoline->dents[0][i]) {
                heights[i] = trampoline->dents[0][i];
            }
            if (heights[i] > trampoline->lines[0][i]) {
                heights[i] = trampoline->lines[0][i];
            }
        }
        if (part1) {
            if (heights[i] > trampoline->dents[1][i]) {
                heights[i] = trampoline->dents[1][i];
            }
            if (heights[i] > trampoline->lines[1][i]) {
                heights[i] = trampoline->lines[1][i];
            }
        }
        if (part2) {
            if (heights[i] > trampoline->dents[2][i]) {
                heights[i] = trampoline->dents[2][i];
            }
            if (heights[i] > trampoline->lines[2][i]) {
                heights[i] = trampoline->lines[2][i];
            }
        }
    }
}

#pragma force_active on

// The cloth between two Pokemon: a straight line between the bottoms of their dents.
void PokeathlonLampJump_CalcLine(PokeathlonLampJumpTrampoline *trampoline, int line, u8 monA, u8 monB) {
    s32 *left;
    s32 *right;
    u8 *leftMon;
    u8 *rightMon;
    int i;
    s32 start;
    s32 delta;
    s32 span;

    if (trampoline->monColumns[monA] <= trampoline->monColumns[monB]) {
        left = &trampoline->monColumns[monA];
        right = &trampoline->monColumns[monB];
        leftMon = &monA;
        rightMon = &monB;
    } else {
        left = &trampoline->monColumns[monB];
        right = &trampoline->monColumns[monA];
        leftMon = &monB;
        rightMon = &monA;
    }
    for (i = 0; i < 128; i++) {
        trampoline->lines[line][i] = 0;
    }
    start = trampoline->dents[*leftMon][*left];
    delta = trampoline->dents[*rightMon][*right] - start;
    span = *right - *left;
    for (i = 0; i < span - 1; i++) {
        trampoline->lines[line][*left + i + 1] = start + delta * (i + 1) / span;
    }
}

#pragma force_active reset

// A lamp hit: one more point than the previous lamp of the same jump (up to 5). Returns the points.
u8 PokeathlonLampJump_ScoreLamp(PokeathlonLampJumpMon *mon) {
    if (mon->combo < 5) {
        mon->combo++;
    }
    mon->score = mon->score + mon->combo;
    if (mon->score > 999) {
        mon->score = 999;
    }
    return (u8)mon->combo;
}

void PokeathlonLampJump_UpdateJumps(PokeathlonCourseData *course, PokeathlonLampJump *lampJump) {
    // three cursors, each moved on by one Pokemon (matching): monRow->mons[0] and jumpRow->jumps[0] are
    // the current Pokemon's
    PokeathlonLampJump *monRow;
    PokeathlonLampJump *jumpRow;
    PokeathlonLampJumpMon *mon;
    u8 participant;
    int i;
    int lamp;
    const u8 *soundPlayer;

    struct {
        s32 screenY;
        s32 screenX;
        s32 radius;
        VecFx32 pos;
    } hit;

    participant = (u8)ov96_021E5F24(course);
    for (i = 0; i < 3; i++) {
        PokeathlonLampJump_UpdateJump(course, lampJump, (u8)i);
    }
    i = 0;
    monRow = lampJump;
    jumpRow = lampJump;
    mon = lampJump->mons;
    soundPlayer = ov96_0221DC18;
    while (i < 3) {
        void *sprite = monRow->mons[0].sprite;
        PokeathlonCourseObject_GroundToCenter(sprite, monRow->mons[0].x / FX32_ONE, (FX32_CONST(288) - (jumpRow->jumps[0].height << 6)) / FX32_ONE, &hit.screenX, &hit.screenY);
        PokeathlonCourseObject_GetCollisionCircle(sprite, hit.screenX << FX32_SHIFT, hit.screenY << FX32_SHIFT, &hit.pos.x, &hit.pos.y, &hit.radius);
        hit.pos.z = 0;
        for (lamp = 0; lamp < 29; lamp++) {
            if (PokeathlonLampJump_IsLampLit(lampJump->unk8C, lamp)) {
                VecFx32 *lampPos = PokeathlonLampJump_GetLampPos(lampJump->unk8C, lamp);
                if (PokeathlonLampJump_CheckCollision(&hit.pos, hit.radius << FX32_SHIFT, lampPos, FX32_CONST(8))) {
                    u8 points;
                    PokeathlonLampJump_PutOutLamp(lampJump->unk8C, lamp);
                    points = PokeathlonLampJump_ScoreLamp(mon);
                    PokeathlonLampJump_ShowLampPoints(lampJump->unk8C, lamp, points);
                    lampJump->unk142 = TRUE;
                    PlaySEOnPlayer(SEQ_SE_GS_POINT1, *soundPlayer);
                    GF_SndSetTrackPitchByPlayerNo(*soundPlayer, 0xFFFF, ov96_0221C110[points - 1]);
                    PokeathlonCourse_AddMonCounter(course, participant, (u8)i, 3, 1);
                }
            }
        }
        monRow = (PokeathlonLampJump *)((u8 *)monRow + sizeof(PokeathlonLampJumpMon));
        soundPlayer++;
        jumpRow = (PokeathlonLampJump *)((u8 *)jumpRow + sizeof(PokeathlonLampJumpJump));
        mon++;
        i++;
    }
    for (i = 0; i < 3; i++) {
        if (lampJump->mons[i].unk26) {
            lampJump->mons[i].stunTimer--;
            if (lampJump->mons[i].stunTimer == 0) {
                lampJump->mons[i].stunTimer = 0;
                lampJump->mons[i].unk26 = FALSE;
            }
        }
    }
}

#pragma force_active on

// One Pokemon on the trampoline: pressing it down, being thrown up, flying, landing.
void PokeathlonLampJump_UpdateJump(PokeathlonCourseData *course, PokeathlonLampJump *lampJump, u8 mon) {
    int heights[128];
    PokeathlonLampJumpJump *jump = &lampJump->jumps[mon];
    int column;
    u8 participant;
    int parts;

    if (!jump->jumping) {
        return;
    }
    participant = ov96_021E5F24(course);
    column = lampJump->monColumns[mon];
    switch (mon) {
    case 0:
        parts = 6;
        break;
    case 1:
        parts = 5;
        break;
    case 2:
        parts = 3;
        break;
    default:
        GF_AssertFail();
        return;
    }
    // the trampoline without this Pokemon's own dent and the line that starts at it
    PokeathlonLampJump_CalcPartialSurface(&lampJump->trampoline, parts, heights);
    if (jump->airborne) {
        if (jump->launchVelocity <= 0) {
            GF_AssertFail();
            jump->airborne = FALSE;
            jump->height = heights[column];
            jump->launchVelocity = 0;
            jump->jumping = FALSE;
            jump->velocity = 0;
            lampJump->mons[mon].unk28 = TRUE;
            PokeathlonLampJump_SetDent(lampJump, mon, column, 0);
            return;
        }
        jump->height += jump->velocity;
        if (lampJump->mons[mon].velocityX) {
            PokeathlonLampJump_MoveSideways(&lampJump->mons[mon], jump);
        }
        if (jump->height > heights[column]) {
            jump->velocity = (int)(jump->velocity - 128.0);
            if (-jump->velocity > jump->launchVelocity) {
                jump->velocity = -jump->launchVelocity;
            }
        } else if (jump->height <= heights[column]) {
            if (jump->velocity == 0) {
                jump->airborne = FALSE;
                jump->height = heights[column];
                jump->launchVelocity = 0;
                jump->jumping = FALSE;
                PokeathlonLampJump_SetDent(lampJump, mon, column, 0);
            } else if (jump->velocity < 0) {
                jump->airborne = FALSE;
                PokeathlonLampJump_SlowDownSideways(&lampJump->mons[mon]);
                if ((double)jump->velocity > -128.0) {
                    jump->velocity = -128;
                }
                jump->launchVelocity = jump->velocity;
            } else {
                jump->airborne = FALSE;
                jump->height = heights[column];
                jump->launchVelocity = 0;
                jump->jumping = FALSE;
                jump->velocity = 0;
                PokeathlonLampJump_SetDent(lampJump, mon, column, 0);
            }
        }
    } else {
        if (jump->launchVelocity > 0) {
            GF_AssertFail();
        }
        jump->height += jump->velocity;
        if ((double)jump->height < -6144.0) {
            jump->height = -6144;
            jump->velocity = 0;
        }
        if (jump->height >= heights[column]) {
            // back at the surface: bounce off
            PokeathlonLampJump_SetDent(lampJump, mon, column, 0);
            if (lampJump->mons[mon].unk28) {
                jump->velocity = lampJump->mons[mon].bounce * jump->velocity / 12;
            }
            if (jump->velocity > 2200) {
                jump->velocity = 2200;
            }
            jump->velocity = jump->velocity * 9 / 10;
            if (jump->velocity == 0) {
                if (lampJump->mons[mon].unk28) {
                    PlaySEOnPlayer(SEQ_SE_DP_BOX03, ov96_0221DC18[3]);
                }
                jump->airborne = FALSE;
                jump->height = heights[column];
                jump->launchVelocity = 0;
                jump->jumping = FALSE;
                PokeathlonLampJump_SetDent(lampJump, mon, column, 0);
            } else if (jump->velocity > 0) {
                if ((double)jump->velocity <= 1408.0 && !lampJump->mons[mon].unk28) {
                    jump->airborne = FALSE;
                    jump->height = heights[column];
                    jump->launchVelocity = 0;
                    jump->velocity = 0;
                    jump->jumping = FALSE;
                    PokeathlonLampJump_SetDent(lampJump, mon, column, 0);
                } else {
                    jump->airborne = TRUE;
                    jump->launchVelocity = jump->velocity;
                    lampJump->mons[mon].unk28 = FALSE;
                    PlaySEOnPlayer(SEQ_SE_DP_DANSA5, ov96_0221DC18[3]);
                    PokeathlonCourse_AddMonCounter(course, participant, mon, 2, 1);
                }
            }
        } else if (jump->height < heights[column]) {
            if (jump->velocity < 0) {
                jump->velocity += 819.2;
                if (jump->velocity >= 0) {
                    jump->velocity = 0;
                }
            } else if (jump->velocity >= 0) {
                jump->velocity += 341.3333333333333;
                if (jump->launchVelocity >= 0) {
                    GF_AssertFail();
                    jump->airborne = FALSE;
                    jump->height = heights[column];
                    jump->launchVelocity = 0;
                    jump->jumping = FALSE;
                    jump->velocity = 0;
                    PokeathlonLampJump_SetDent(lampJump, mon, column, 0);
                    return;
                }
                if (jump->velocity > -jump->launchVelocity) {
                    jump->velocity = -jump->launchVelocity;
                }
            }
        }
    }
    if (!jump->airborne || !jump->jumping || !jump->velocity) {
        lampJump->mons[mon].combo = 0;
    }
    if (!jump->airborne && jump->jumping) {
        PokeathlonLampJump_SetDent(lampJump, mon, column, jump->height);
    }
    if (!jump->jumping) {
        lampJump->mons[mon].unk28 = TRUE;
    }
}

#pragma force_active reset

void PokeathlonLampJump_MoveSideways(PokeathlonLampJumpMon *mon, PokeathlonLampJumpJump *jump) {
    fx32 x;
    if (jump->height > 0) {
        x = mon->drawX + mon->velocityX;
        mon->x = x;
        if (x < FX32_CONST(32)) {
            mon->x = FX32_CONST(32);
        } else if (x > FX32_CONST(223)) {
            mon->x = FX32_CONST(223);
        }
    }
}

#pragma force_active on

// The dent a Pokemon makes at a column: straight lines from both edges down to `depth`.
void PokeathlonLampJump_CalcDent(int column, int depth, s32 *dent) {
    int negDepth = -depth, right = 127 - column, i;
    s32 *p;
    i = 0;
    if (column > 0) {
        p = dent;
        do {
            int x = (u8)(i + 1);
            *p++ = -(negDepth * x / (column + 1));
            i++;
        } while (i < column);
    }
    for (i = 0; i < right; i++) {
        dent[(u8)(column + 1 + i)] = -((u8)(right - i) * negDepth / (right + 1));
    }
}

#pragma force_active reset

// A Pokemon that is not jumping rests on the surface.
void PokeathlonLampJump_SetRestingHeights(PokeathlonLampJump *lampJump) {
    int i;
    PokeathlonLampJumpTrampoline *trampoline;
    PokeathlonLampJumpJump *jumps;
    trampoline = &lampJump->trampoline;
    jumps = lampJump->jumps;
    for (i = 0; i < 3; i++) {
        if (jumps[i].jumping == FALSE) {
            jumps[i].height = trampoline->surface[trampoline->monColumns[i]];
        }
    }
}

void PokeathlonLampJump_SetDent(PokeathlonLampJump *lampJump, int mon, u8 column, fx32 depth) {
    int i;
    for (i = 0; i < 128; i++) {
        lampJump->trampoline.dents[mon][i] = 0;
    }
    lampJump->trampoline.dents[mon][column] = depth;
    PokeathlonLampJump_CalcDent(column, lampJump->trampoline.dents[mon][column], lampJump->trampoline.dents[mon]);
}

void PokeathlonLampJump_LoadSpriteResources(void *scene) {
    PokeathlonCourseSpritePool_LoadChar(scene, NARC_a_1_6_8, 3, 101, 3);
    PokeathlonCourseSpritePool_LoadPltt(scene, NARC_a_1_6_8, 0, 101, 3, 4);
    PokeathlonCourseSpritePool_LoadCell(scene, NARC_a_1_6_8, 2, 101);
    PokeathlonCourseSpritePool_LoadAnim(scene, NARC_a_1_6_8, 1, 101);
}

#pragma force_active on

void PokeathlonLampJump_CreateSprites(PokeathlonLampJump *lampJump, void *scene) {
    int i;
    int j;
    VecFx32 pos;

    PokeathlonLampJump_CreateLampSprites(lampJump->unk8C, scene, 2);
    lampJump->unk64 = PokeathlonCourseSpritePool_CreateAndTake(scene, 1, 3, 101, 3);
    PokeathlonCourseSpritePoolSprite_SetAnim(lampJump->unk64, 12);
    PokeathlonCourseSpritePoolSprite_Show(lampJump->unk64, 1, 1);
    for (i = 0; i < 4; i++) {
        lampJump->playerSprites[i] = PokeathlonCourseSpritePool_CreateAndTake(scene, 1, 3, 101, 4);
        PokeathlonCourseSpritePoolSprite_SetAnim(lampJump->playerSprites[i], 13 + i);
        PokeathlonCourseSpritePoolSprite_Show(lampJump->playerSprites[i], 1, 1);
    }
    // (sic) 4 each: the fourth effect sprite of a Pokemon lands in timer, the fourth effect sprite in
    // timerDigits[0]; both are set again later.
    for (i = 0; i < 4; i++) {
        lampJump->mons[i].effectSprite = PokeathlonCourseSpritePool_CreateAndTake(scene, 1, 3, 101, 7);
        PokeathlonCourseSpritePoolSprite_SetAnim(lampJump->mons[i].effectSprite, 11);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(lampJump->mons[i].effectSprite, 9);
    }
    for (i = 0; i < 4; i++) {
        lampJump->effectSprites[i] = PokeathlonCourseSpritePool_CreateAndTake(scene, 1, 3, 101, 8);
        PokeathlonCourseSpritePoolSprite_SetAnim(lampJump->effectSprites[i], 9);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(lampJump->effectSprites[i], 6);
    }
    for (j = 0; j < 2; j++) {
        SpriteList *spriteList = PokeathlonCourseSpritePool_GetSpriteList(scene);
        lampJump->timerDigits[j] = ov96_021EA634(lampJump->unk188, spriteList, 1, lampJump->heapId);
        Sprite_SetDrawFlag(lampJump->timerDigits[j], TRUE);
        pos.x = ov96_0221C0B0[j] * FX32_ONE;
        pos.y = FX32_CONST(8);
        pos.z = 0;
        Sprite_SetMatrix(lampJump->timerDigits[j], &pos);
        Sprite_SetAnimCtrlSeq(lampJump->timerDigits[j], ov96_0221C0B4[j]);
    }
}

// The lamps, and the player markers: this player's first, the others below each other.
void PokeathlonLampJump_PlaceSprites(PokeathlonLampJump *lampJump, u8 self) {
    VecFx32 pos;
    VecFx32 ownPos;
    VecFx32 otherPos;
    VecFx32 firstOtherPos;
    int i;
    u8 n;

    PokeathlonLampJump_PlaceLamps(lampJump->unk8C);
    pos = ov96_0221C0C4;
    PokeathlonCourseSpritePoolSprite_SetPosition(lampJump->unk64, &pos);
    ownPos = ov96_0221C0DC;
    PokeathlonCourseSpritePoolSprite_SetPosition(lampJump->playerSprites[self], &ownPos);
    firstOtherPos = ov96_0221C0D0;
    n = 0;
    for (i = 0; i < 4; i++) {
        otherPos = firstOtherPos;
        if (i != self) {
            otherPos.y += (u32)n << 16;
            PokeathlonCourseSpritePoolSprite_SetPosition(lampJump->playerSprites[i], &otherPos);
            n++;
        }
    }
}

#pragma force_active reset

void PokeathlonLampJump_UpdateScoreDisplay(PokeathlonLampJump *lampJump) {
    u16 total = 0;
    int i;
    for (i = 0; i < 3; i++) {
        total = total + lampJump->mons[i].score;
    }
    if (total > 999) {
        total = 999;
    }
    PokeathlonLampJump_PrintScore(lampJump, total);
}

#pragma force_active on

// Three digits of 4x8 tiles from the digit sheet in the BG2 screen (5 digits per row).
void PokeathlonLampJump_PrintScore(PokeathlonLampJump *lampJump, int score) {
    u8 x[3];
    u8 y[3];
    u8 digit;
    u8 i;

    digit = score / 100;
    x[0] = (digit % 5) * 4;
    y[0] = (digit / 5) * 8;
    digit = (score - digit * 100) / 10;
    x[1] = (digit % 5) * 4;
    y[1] = (digit / 5) * 8;
    digit = score % 10;
    x[2] = (digit % 5) * 4;
    y[2] = (digit / 5) * 8;
    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(lampJump->bgConfig, GF_BG_LYR_SUB_0, (u8)(1 + i * 4), 0, 4, 8, lampJump->bg2Screen->rawData, x[i], y[i], 20, 16);
    }
    ScheduleBgTilemapBufferTransfer(lampJump->bgConfig, GF_BG_LYR_SUB_0);
}

#pragma force_active reset

// Seconds left, two digit sprites.
void PokeathlonLampJump_UpdateTimerDisplay(PokeathlonLampJump *lampJump, int frames) {
    u8 seconds = (u8)(frames / 30);
    Sprite_SetAnimCtrlSeq(lampJump->timerDigits[0], (u8)(seconds / 10) + 1);
    Sprite_SetAnimCtrlSeq(lampJump->timerDigits[1], (u8)(seconds % 10) + 1);
}

// Window 0: this player's name; windows 1-3: the others.
void PokeathlonLampJump_PrintPlayerNames(PokeathlonCourseData *course, PokeathlonLampJump *lampJump) {
    u8 i;
    Window *window;
    u8 self;
    String *name;
    u8 line;
    u8 participant;

    for (i = 0; i < 5; i++) {
        FillWindowPixelBuffer(&lampJump->windows[i], 0);
    }
    self = ov96_021E5F24(course);
    name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, self), lampJump->heapId);
    AddTextPrinterParameterizedWithColor(&lampJump->windows[0], 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(name);
    line = 1;
    participant = 0;
    window = &lampJump->windows[0];
    for (; participant < 4; participant++) {
        if (participant != self) {
            name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(course, participant), lampJump->heapId);
            AddTextPrinterParameterizedWithColor(window + line, 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
            String_Delete(name);
            line++;
        }
    }
    for (i = 0; i < 5; i++) {
        CopyWindowToVram(&lampJump->windows[i]);
    }
}

void PokeathlonLampJump_InitWindows(PokeathlonLampJump *lampJump) {
    u8 i;
    for (i = 0; i < 5; i++) {
        AddWindow(lampJump->bgConfig, &lampJump->windows[i], &ov96_0221C1CC[i]);
    }
    BG_FillCharDataRange(lampJump->bgConfig, GF_BG_LYR_SUB_2, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, lampJump->heapId);
    BG_FillCharDataRange(lampJump->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, lampJump->heapId);
}

// Window 4: "Pokemon Total: {X} Points".
void PokeathlonLampJump_PrintTotal(PokeathlonLampJump *lampJump, int score) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;
    FillWindowPixelBuffer(&lampJump->windows[4], 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, lampJump->heapId);
    messageFormat = MessageFormat_New(lampJump->heapId);
    BufferIntegerAsString(messageFormat, 0, score, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msg_0309_00161, lampJump->heapId);
    AddTextPrinterParameterizedWithColor(&lampJump->windows[4], 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&lampJump->windows[4]);
}

void PokeathlonLampJump_SlowDownSideways(PokeathlonLampJumpMon *mon) {
    fx32 velocityX = mon->velocityX;
    if (velocityX == 0) {
        return;
    }
    if (velocityX > 0) {
        velocityX -= FX32_CONST(2);
        mon->velocityX = velocityX;
        if (velocityX < 0) {
            mon->velocityX = 0;
        }
    } else if (velocityX < 0) {
        velocityX += FX32_CONST(2);
        mon->velocityX = velocityX;
        if (velocityX > 0) {
            mon->velocityX = 0;
        }
    }
}

// Bounce, stun time and flick scale from the Pokemon's Skill, Stamina and Jump.
void PokeathlonLampJump_SetMonParams(PokeathlonCourseData *course, PokeathlonLampJumpMonParams *params, u8 member, PokeathlonLampJumpMon *mon) {
    u8 self = ov96_021E5F24(course);
    u8 *stats = PokeathlonCourse_GetMonStats(course, self, member);
    mon->bounce = params->acceleration[stats[2]];
    mon->stunFrames = params->velocity[stats[3]];
    mon->flickScale = params->scale[stats[4]];
}

#pragma force_active on

// Two Pokemon that touch: both are stunned; in the air they swap their speeds, else they are pushed apart.
BOOL PokeathlonLampJump_CollideMons(PokeathlonCourseData *course, PokeathlonLampJump *lampJump) {
    VecFx32 posA;
    VecFx32 posB;
    int radiusA, radiusB, x, y;
    int i, j;
    void *spriteA, *spriteB;
    u8 participant;
    PokeathlonLampJumpJump *jumpA;
    PokeathlonLampJumpMon *monA = &lampJump->mons[0];

    for (i = 0; i < 3; i++, monA++) {
        jumpA = &lampJump->jumps[i];
        for (j = 0; j < 3; j++) {
            PokeathlonLampJumpMon *monB = &lampJump->mons[j];
            PokeathlonLampJumpJump *jumpB = &lampJump->jumps[j];
            if (i == j) {
                continue;
            }
            participant = ov96_021E5F24(course);
            spriteA = monA->sprite;
            spriteB = monB->sprite;
            PokeathlonCourseObject_GroundToCenter(spriteA, monA->x / FX32_ONE, (FX32_CONST(288) - (jumpA->height << 6)) / FX32_ONE, &x, &y);
            PokeathlonCourseObject_GetCollisionCircle(spriteA, x << FX32_SHIFT, y << FX32_SHIFT, &posA.x, &posA.y, &radiusA);
            posA.z = 0;
            PokeathlonCourseObject_GroundToCenter(spriteB, monB->x / FX32_ONE, (FX32_CONST(288) - (jumpB->height << 6)) / FX32_ONE, &x, &y);
            PokeathlonCourseObject_GetCollisionCircle(spriteB, x << FX32_SHIFT, y << FX32_SHIFT, &posB.x, &posB.y, &radiusB);
            posB.z = 0;
            if (PokeathlonLampJump_CheckCollision(&posA, radiusA << FX32_SHIFT, &posB, radiusB << FX32_SHIFT) == FALSE) {
                monA->colliding[j] = FALSE;
                monB->colliding[i] = FALSE;
                continue;
            }
            if (monB->colliding[i]) {
                continue;
            }
            monA->unk26 = TRUE;
            monB->unk26 = TRUE;
            monA->stunTimer = (30 * monA->stunFrames) / 30;
            monB->stunTimer = (30 * monB->stunFrames) / 30;
            {
                VecFx32 effectPos = { 0, 0, 0 };
                VEC_Subtract(&posB, &posA, &effectPos);
                VEC_Normalize(&effectPos, &effectPos);
                VEC_MultAdd(radiusA << FX32_SHIFT, &effectPos, &posA, &effectPos);
                lampJump->effectPositions[lampJump->nextEffect] = effectPos;
            }
            lampJump->effectPending[lampJump->nextEffect] = TRUE;
            lampJump->nextEffect = (lampJump->nextEffect + 1) % 3;
            PlaySEOnPlayer(SEQ_SE_GS_THLON06, ov96_0221DC18[3]);
            PokeathlonCourse_AddMonCounter(course, participant, (u8)i, 4, 1);
            PokeathlonCourse_AddMonCounter(course, participant, (u8)j, 4, 1);
            PokeathlonCourse_AddMonCounter(course, participant, (u8)i, 1, 1);
            PokeathlonCourse_AddMonCounter(course, participant, (u8)j, 1, 1);
            if (jumpA->airborne && jumpB->airborne && jumpA->height > 0 && jumpB->height > 0) {
                int tmp;
                if (monA->colliding[j] || monB->colliding[i]) {
                    continue;
                }
                monA->colliding[j] = TRUE;
                monB->colliding[i] = TRUE;
                tmp = jumpB->velocity;
                jumpB->velocity = jumpA->velocity;
                jumpA->velocity = tmp;
                tmp = monB->velocityX;
                monB->velocityX = monA->velocityX;
                monA->velocityX = tmp;
                if (jumpA->velocity < 0) {
                    jumpA->velocity = 0;
                }
                if (jumpB->velocity < 0) {
                    jumpB->velocity = 0;
                }
            } else {
                if (monA->x <= monB->x) {
                    monA->x -= FX32_CONST(16);
                    monB->x += FX32_CONST(16);
                } else {
                    monA->x += FX32_CONST(16);
                    monB->x -= FX32_CONST(16);
                }
                if (monA->x < FX32_CONST(32)) {
                    monA->x = FX32_CONST(32);
                } else if (monA->x > FX32_CONST(223)) {
                    monA->x = FX32_CONST(223);
                }
                if (monB->x < FX32_CONST(32)) {
                    monB->x = FX32_CONST(32);
                } else if (monB->x > FX32_CONST(223)) {
                    monB->x = FX32_CONST(223);
                }
            }
        }
    }
    return FALSE;
}

#pragma force_active reset

PokeathlonLampJumpLamps *PokeathlonLampJump_NewLamps(enum HeapID heapId) {
    PokeathlonLampJumpLamps *lamps = Heap_Alloc(heapId, sizeof(PokeathlonLampJumpLamps));
    MI_CpuFill8(lamps, 0, sizeof(PokeathlonLampJumpLamps));
    lamps->relightTimer = 0;
    return lamps;
}

void PokeathlonLampJump_FreeLamps(PokeathlonLampJumpLamps *lamps) {
    Heap_Free(lamps);
}

void PokeathlonLampJump_CreateLampSprites(PokeathlonLampJumpLamps *lamps, void *scene, int a2) {
    int i;
    PokeathlonLampJumpLamp *lamp = lamps->lamps;
    for (i = 0; i < 29; i++, lamp++) {
        lamp->sprite = PokeathlonCourseSpritePool_CreateAndTake(scene, a2, 3, 101, 2);
        PokeathlonCourseSpritePoolSprite_Show(lamp->sprite, 1, 1);
        PokeathlonCourseSpritePoolSprite_SetAnim(lamp->sprite, 8);
        lamp->pointsSprite = PokeathlonCourseSpritePool_CreateAndTake(scene, a2, 3, 101, 5);
        lamp->hitSprite = PokeathlonCourseSpritePool_CreateAndTake(scene, a2, 3, 101, 6);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(lamp->sprite, 20);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(lamp->pointsSprite, 7);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(lamp->hitSprite, 8);
    }
}

void PokeathlonLampJump_PlaceLamps(PokeathlonLampJumpLamps *lamps) {
    int i;
    VecFx32 pos;
    const u16 *xy = ov96_0221C21C[0];
    PokeathlonLampJumpLamp *lamp = lamps->lamps;
    for (i = 0; i < 29; i++, xy += 2, lamp++) {
        pos.x = xy[0] << 15;
        pos.y = xy[1] << 15;
        pos.z = 0;
        PokeathlonCourseSpritePoolSprite_SetPosition(lamp->sprite, &pos);
        PokeathlonCourseSpritePoolSprite_SetPosition(lamp->pointsSprite, &pos);
        PokeathlonCourseSpritePoolSprite_SetPosition(lamp->hitSprite, &pos);
        lamp->pos = pos;
    }
}

// Every 240 frames the lamps that are out are lit again.
void PokeathlonLampJump_UpdateLamps(PokeathlonLampJumpLamps *lamps) {
    int timer = lamps->relightTimer;
    if (timer == 0) {
        PokeathlonLampJump_LightLamps(lamps);
        lamps->relightTimer = 240;
    } else {
        lamps->relightTimer = timer - 1;
    }
}

void PokeathlonLampJump_PutOutLamp(PokeathlonLampJumpLamps *lamps, u8 index) {
    lamps->lamps[index].lit = FALSE;
    PokeathlonCourseSpritePoolSprite_SetAnim(lamps->lamps[index].sprite, 8);
}

VecFx32 *PokeathlonLampJump_GetLampPos(PokeathlonLampJumpLamps *lamps, u8 index) {
    return &lamps->lamps[index].pos;
}

BOOL PokeathlonLampJump_IsLampLit(PokeathlonLampJumpLamps *lamps, u8 index) {
    return lamps->lamps[index].lit;
}

void PokeathlonLampJump_ShowLampPoints(PokeathlonLampJumpLamps *lamps, u8 index, u8 points) {
    PokeathlonCourseSpritePoolSprite_SetAnim(lamps->lamps[index].hitSprite, 10);
    PokeathlonCourseSpritePoolSprite_Show(lamps->lamps[index].hitSprite, 1, 1);
    PokeathlonCourseSpritePoolSprite_SetAnim(lamps->lamps[index].pointsSprite, points);
    PokeathlonCourseSpritePoolSprite_Show(lamps->lamps[index].pointsSprite, 1, 1);
}

#pragma force_active on
#pragma opt_scalarizeliveranges off

// Lights up to 25 of the lamps that are out, picked at random.
void PokeathlonLampJump_LightLamps(PokeathlonLampJumpLamps *lamps) {
    struct {
        PokeathlonLampJumpLamp *lamps[29];
    } unlit;

    struct {
        int count;
        PokeathlonLampJumpLamp **end;
    } list;

    int i;
    PokeathlonLampJumpLamp *lamp = lamps->lamps;

    list.count = 0;
    for (i = 0; i < 29; i++, lamp++) {
        if (lamp->lit == FALSE) {
            unlit.lamps[list.count++] = lamp;
        }
    }
    i = 0;
    list.end = unlit.lamps + list.count;
    for (; i < 25; i++) {
        u8 index;
        if (list.count <= 0) {
            break;
        }
        index = LCRandom() % list.count;
        unlit.lamps[index]->lit = TRUE;
        PokeathlonCourseSpritePoolSprite_SetAnim(unlit.lamps[index]->sprite, 7);
        unlit.lamps[index] = list.end[-1];
        list.end--;
        list.count--;
    }
}

#pragma opt_scalarizeliveranges reset
#pragma force_active reset

#pragma force_active on

PokeathlonLampJumpCpuPlayers *PokeathlonLampJump_NewCpuPlayers(enum HeapID heapId, u8 count, int unused, u8 *dataCopy, PokeathlonCourseData *course) {
    int i;
    int j;
    PokeathlonLampJumpCpuPlayers *cpu = Heap_Alloc(heapId, sizeof(PokeathlonLampJumpCpuPlayers));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonLampJumpCpuPlayers));
    cpu->count = count;
    cpu->unk21 = 4 - count;
    cpu->first = cpu->unk21;
    cpu->interval = 3;
    cpu->dataCopy = dataCopy;
    for (i = 0; i < cpu->count; i++) {
        u8 participant = cpu->first + i;
        for (j = 0; j < 3; j++) {
            u8 *stats = PokeathlonCourse_GetMonStats(course, participant, j);
            cpu->skill[participant] += stats[2];
            cpu->jump[participant] += stats[4];
            cpu->skillUpTo[participant][j] = cpu->skill[participant];
        }
    }
    return cpu;
}

#pragma force_active reset

void PokeathlonLampJump_FreeCpuPlayers(PokeathlonLampJumpCpuPlayers *cpu) {
    Heap_Free(cpu);
}

// Every `interval` frames the participants without a player get a turn, one per frame.
int PokeathlonLampJump_UpdateCpuPlayers(PokeathlonCourseData *course, PokeathlonLampJumpCpuPlayers *cpu) {
    int participant;
    int result = 4;

    if (cpu->unk21 == 4) {
        return 0;
    }
    cpu->tick++;
    for (participant = cpu->first; participant < 4; participant++) {
        *(u32 *)ov96_021E8A20(cpu->dataCopy + 0x50 + participant * 0x28) = FALSE;
    }
    if (cpu->tick >= cpu->interval) {
        cpu->active = TRUE;
        cpu->tick = 0;
    }
    if (cpu->active) {
        participant = cpu->first + cpu->current;
        if (participant < cpu->first) {
            GF_AssertFail();
        }
        if (participant < cpu->first) {
            return 4;
        }
        PokeathlonLampJump_UpdateCpuPlayer(course, cpu, (u8)participant);
        result = (u8)participant;
        cpu->current++;
        if (cpu->current >= cpu->count) {
            cpu->current = 0;
            cpu->active = FALSE;
        }
    }
    return result;
}

// Hits a lamp if Jump + 2 * Skill + a random 0-100 reaches 100; the Pokemon is picked by Skill.
void PokeathlonLampJump_UpdateCpuPlayer(PokeathlonCourseData *course, PokeathlonLampJumpCpuPlayers *cpu, u8 participant) {
    int value;
    u8 roll;
    u8 i;
    u32 *lampHit = (u32 *)ov96_021E8A20(cpu->dataCopy + 0x50 + participant * 0x28);

    if (cpu->skill[participant] == 0) {
        value = 0;
    } else {
        value = cpu->jump[participant] + cpu->skill[participant] * 2 + LCRandom() % 101;
    }
    if (value >= 100) {
        *lampHit = TRUE;
        roll = LCRandom() % cpu->skill[participant];
        for (i = 0; i < 3; i++) {
            if (roll < cpu->skillUpTo[participant][i]) {
                PokeathlonCourse_AddMonCounter(course, participant, i, 3, 1);
                break;
            }
        }
        if (i >= 3) {
            GF_AssertFail();
        }
    }
}

// Results Announcement (pokeathlon_results_announcement.c): its title window.
void PokeathlonResultsAnnouncement_InitWindow(Window *window, BgConfig *bgConfig, enum HeapID heapId) {
    if (bgConfig == NULL) {
        GF_AssertFail();
    }
    AddWindow(bgConfig, window, &ov96_0221C2A0);
    BG_FillCharDataRange(bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, heapId);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BGEXT, (enum GFPalSlotOffset)0x4000, heapId);
}

// "Results Announcement"
void PokeathlonResultsAnnouncement_PrintTitle(Window *window, enum HeapID heapId) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;

    FillWindowPixelBuffer(window, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, heapId);
    messageFormat = MessageFormat_New(heapId);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msg_0309_00303, heapId);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}
