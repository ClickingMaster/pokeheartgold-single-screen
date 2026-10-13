#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "geometry_math.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

// NitroSDK fixed-point square root (not declared in lib/include yet)
fx32 FX_Sqrt(fx32 x);

// sound_playback.c (no header declares them)
BOOL PlaySEOnPlayer(u16 seqNo, int playerNo);
u32 GF_SndCountPlayingSeqOnPlayer(u32 playerNo);

// Course helpers and the rest of the event (OVY_96, not yet tidied).
u8 *PokeathlonCourse_ToBytes(void *block);
u8 *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, u16 member);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, int participant, u8 member);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int kind, int amount);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, int score);
void PokeathlonCourseMonObject_SetDrawFlag(void *object, int visible);
void PokeathlonCourseMonObject_SetDrawPriority(void *object, int priority);
void PokeathlonCourseMonObject_SetAnimStateRaw(void *object, u8 size);
void PokeathlonCourseMonObject_SetAnimState(void *object, u8 size);
void PokeathlonCourseMonObject_SetAnim(void *object, int anim);
void PokeathlonCourseObject_GetCollisionCircle(void *object, int x, int y, int *outX, int *outY, int *outRadius);
int PokeathlonCourseObject_GetCollisionRadius(void *object);
void PokeathlonCourseObject_SetGroundPosition(void *object, int x, int y);
void PokeathlonCourseObject_GroundToCenter(void *object, int x, int y, int *outX, int *outY);
void PokeathlonCourseSpritePoolSprite_Show(void *sprite, int shown, int show);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *sprite, int anim);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *sprite, VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(void *sprite);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(void *sprite, int priority);
// in the top screen code
void ov96_02208374(PokeathlonPennantCaptureDisplay *display, u16 score);
BOOL ov96_02208448(PokeathlonPennantCaptureDisplay *display, int member);
BOOL ov96_02208608(PokeathlonPennantCaptureDisplay *display, int participant);
void ov96_02208658(PokeathlonPennantCaptureDisplay *display, u8 participant, u16 member, u8 hidden, u8 active);
void ov96_02208840(PokeathlonPennantCaptureDisplay *display, int participant, u8 pennants);
void ov96_02208864(PokeathlonPennantCaptureDisplay *display, int participant);

extern const WindowTemplate ov96_0221CAC0;
extern const VecFx32 ov96_0221CAC8;

// The sides of an obstacle a path enters through (edge and corner hit codes; 9-12 = from inside).
typedef struct PokeathlonPennantCaptureHitCodes {
    int sides[4];
} PokeathlonPennantCaptureHitCodes;

extern const PokeathlonPennantCaptureHitCodes ov96_0221CB00; // edge hits
extern const PokeathlonPennantCaptureHitCodes ov96_0221CAE0; // corner hits

typedef struct PokeathlonPennantCaptureCollisionFrame {
    int bY;
    int bX;
    int aY;
    int aX;
    int radiusB;
    int radiusA;
    VecFx32 resolved;
    VecFx32 zeroB;
    VecFx32 zeroA;
    VecFx32 diff;
    VecFx32 pointB;
    VecFx32 pointA;
} PokeathlonPennantCaptureCollisionFrame;

#pragma force_active on

void PokeathlonPennantCapture_UpdatePlayers(PokeathlonCourseData *course) {
    PokeathlonPennantCapture *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 handled[4];
    int i;
    u8 *flag;
    PokeathlonPennantCapturePlayer *player;
    VecFx32 hit;
    VecFx32 previous;
    VecFx32 friction;

    work->hitReported = 0;
    i = 0;
    flag = handled;
    for (; i < 4; flag++, i++) {
        *flag = 0;
        if (work->players[i].freezeTimer) {
            PokeathlonPennantCapture_TickFreezeTimer(course, work, &work->players[i]);
            *flag = 1;
        }
    }
    player = work->players;
    for (i = 0; i < 4; i++, player++) {
        if (handled[i]) {
            continue;
        }
        if (PokeathlonPennantCapture_CanDeposit(player)) {
            player->pointsToAdd = PokeathlonPennantCapture_GetDepositPoints(player->pennants);
            player->pennants = 0;
            player->depositing = 1;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->velocity.z = 0;
            player->direction = 2;
            player->depositSteps = 10;
            PokeathlonCourse_AddMonCounter(course, player->participant, player->member, 3, player->pointsToAdd);
        }
        if (player->depositSteps && player->depositing) {
            if (player->depositDelay == 0) {
                if (player->pointsToAdd) {
                    player->score++;
                    if (player->score > 999) {
                        player->score = 999;
                    }
                    player->pointsToAdd--;
                }
                player->depositSteps--;
                if (player->depositSteps == 0) {
                    player->depositSteps = 0;
                    player->swapPending = 1;
                    player->swapPhase = 1;
                    PokeathlonCourse_AddMonCounter(course, player->participant, player->member, 7, 1);
                } else {
                    player->depositDelay = 4;
                }
            } else {
                player->depositDelay--;
            }
        }
        if (player->depositing) {
            continue;
        }
        previous = player->position;
        VEC_Add(&player->position, &player->velocity, &player->position);
        // the goal opening at the bottom is narrower than the field
        if (player->position.x < 0xD0000 || player->position.x > 0x130000) {
            if (player->position.y < 0x68000) {
                player->position.y = 0x68000;
            } else if (player->position.y > 0x188000) {
                player->position.y = 0x188000;
            }
        } else {
            if (player->position.y < 0x68000) {
                player->position.y = 0x68000;
            } else if (player->position.y > 0x1A0000) {
                player->position.y = 0x1A0000;
            }
        }
        if (player->position.y > 0x188000) {
            if (player->position.x < 0xD0000) {
                player->position.x = 0xD0000;
            } else if (player->position.x > 0x130000) {
                player->position.x = 0x130000;
            }
        } else {
            if (player->position.x < 0x90000) {
                player->position.x = 0x90000;
            } else if (player->position.x > 0x170000) {
                player->position.x = 0x170000;
            }
        }
        player->wallSide = (u8)PokeathlonPennantCapture_CollideWithObstacles(work, player->wallSide, PokeathlonCourseObject_GetCollisionRadius(player->monObjects[player->member]), &previous, &player->position, &player->position);
        switch (player->wallSide) {
        case 1:
            if (player->velocity.y > 0) {
                player->velocity.y *= -0.3;
            }
            break;
        case 2:
            if (player->velocity.x < 0) {
                player->velocity.x *= -0.3;
            }
            break;
        case 3:
            if (player->velocity.y < 0) {
                player->velocity.y *= -0.3;
            }
            break;
        case 4:
            if (player->velocity.x > 0) {
                player->velocity.x *= -0.3;
            }
            break;
        case 5:
            if (player->velocity.x > 0) {
                player->velocity.x *= -0.3;
            }
            if (player->velocity.y > 0) {
                player->velocity.y *= -0.3;
            }
            break;
        case 6:
            if (player->velocity.x < 0) {
                player->velocity.x *= -0.3;
            }
            if (player->velocity.y > 0) {
                player->velocity.y *= -0.3;
            }
            break;
        case 7:
            if (player->velocity.x < 0) {
                player->velocity.x *= -0.3;
            }
            if (player->velocity.y < 0) {
                player->velocity.y *= -0.3;
            }
            break;
        case 8:
            if (player->velocity.x > 0) {
                player->velocity.x *= -0.3;
            }
            if (player->velocity.y < 0) {
                player->velocity.y *= -0.3;
            }
            break;
        }
        int collide = PokeathlonPennantCapture_CollidePlayers(course, work->players, &hit);
        if (!work->hitReported && collide) {
            work->hitReported = 1;
            work->hitX = hit.x / FX32_ONE;
            work->hitY = hit.y / FX32_ONE;
        }
        friction = player->velocity;
        if (VEC_Mag(&friction) > 0) {
            float force = 6.0f;
            int amount = (int)(0.1 * force * 4096.0);
            if (work->heldTooLong) {
                amount += 0x3000;
            }
            VEC_Normalize(&friction, &friction);
            friction.x = (fx32)(((s64)friction.x * amount + 2048) >> 12);
            friction.y = (fx32)(((s64)friction.y * amount + 2048) >> 12);
            int magnitude = VEC_Mag(&friction);
            if (VEC_Mag(&player->velocity) >= magnitude) {
                VEC_Subtract(&player->velocity, &friction, &player->velocity);
            } else {
                player->velocity.x = 0;
                player->velocity.y = 0;
            }
        }
        if (!player->swapPending && player->knockedDown == 1) {
            player->knockdownTimer--;
            if (player->knockdownTimer == 0) {
                player->knockedDown = 0;
            }
        }
        PokeathlonPennantCapture_PickUpPennants(&work->pennants, player, &work->pennantsFull[i]);
    }
}

#pragma force_active reset

void PokeathlonPennantCapture_TickFreezeTimer(PokeathlonCourseData *course, PokeathlonPennantCapture *work, PokeathlonPennantCapturePlayer *player) {
    if (player->freezeTimer != 0) {
        player->freezeTimer--;
    }
}

// The frame update was built against a declaration that took the member as u16 (it only shows in the
// register allocation of the caller); the definition takes a u8.
typedef void (*PokeathlonPennantCaptureMonSwapFunc)(PokeathlonPennantCaptureDisplay *display, PokeathlonPennantCapturePlayer *player, PokeathlonPennantCaptureFrame *frame, u8 participant, u16 member, u8 *block, int isHost, int isSelf, int remote);
#define UpdateMonSwapAsU16 ((PokeathlonPennantCaptureMonSwapFunc)PokeathlonPennantCapture_UpdateMonSwap)

#pragma force_active on

void PokeathlonPennantCapture_ApplyFrame(PokeathlonCourseData *course, int viewer, PokeathlonPennantCapturePlayer *players, PokeathlonPennantCaptureLinkData *link) {
    u32 baseX;
    u32 baseY;
    PokeathlonPennantCapture *work;
    PokeathlonPennantCaptureFrame *frame;
    void *object;
    u8 size;
    u8 hidden;
    u8 active;
    int curY;
    int x;
    long px;
    u8 member;
    u8 self;
    long py;
    int isHost;
    int y;
    int curX;
    int i;
    int j;
    u8 *block;
    int remote;
    u8 pennants;
    u16 score;

    self = PokeathlonCourse_GetNetId(course);
    frame = (PokeathlonPennantCaptureFrame *)PokeathlonCourse_ToBytes(link->frame);
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    baseX = frame->words[viewer] & 0x1FF;
    baseY = (frame->words[viewer] >> 9) & 0x1FF;

    if ((u8)((frame->words[5] >> 18) & 1)) {
        x = frame->words[5] & 0x1FF;
        y = (frame->words[5] >> 9) & 0x1FF;
        work->pickupEffectX[work->nextPickupEffect] = x;
        work->pickupEffectY[work->nextPickupEffect] = y;
        PokeathlonCourseSpritePoolSprite_Show(work->pickupEffects[work->nextPickupEffect], 1, 1);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->pickupEffects[work->nextPickupEffect], 7);
        work->nextPickupEffect = (work->nextPickupEffect + 1) % 4;
        PokeathlonPennantCapture_PlaySEIfOnScreen(x + 128 - baseX, y + 96 - baseY, SEQ_SE_GS_THLON06, 1);
    }
    if ((u8)(((u8)((frame->words[5] >> 19) & 0xF) >> self) & 1)) {
        PlaySEOnPlayer(SEQ_SE_GS_HATAMOTENAI, 3);
    }
    if ((u8)((frame->words[4] >> 18) & 1)) {
        x = frame->words[4] & 0x1FF;
        y = (frame->words[4] >> 9) & 0x1FF;
        work->dropEffectX[work->nextDropEffect] = x;
        work->dropEffectY[work->nextDropEffect] = y;
        PokeathlonCourseSpritePoolSprite_Show(work->dropEffects[work->nextDropEffect], 1, 1);
        PokeathlonCourseSpritePoolSprite_SetAnim(work->dropEffects[work->nextDropEffect], 4);
        work->nextDropEffect = (work->nextDropEffect + 1) % 4;
        PokeathlonPennantCapture_PlaySEIfOnScreen(x + 128 - baseX, y + 96 - baseY, SEQ_SE_GS_SAA, 1);
    }

    for (i = 0; i < 4; i++) {
        VecFx32 pickupPos = { 0, 0, 0 };
        y = (work->pickupEffectY[i] + 96 - baseY) << 12;
        x = (work->pickupEffectX[i] + 128 - baseX) << 12;
        pickupPos.x = x;
        pickupPos.y = y;
        PokeathlonCourseSpritePoolSprite_SetPosition(work->pickupEffects[i], &pickupPos);
    }
    for (i = 0; i < 4; i++) {
        VecFx32 dropPos = { 0, 0, 0 };
        y = (work->dropEffectY[i] + 96 - baseY) << 12;
        x = (work->dropEffectX[i] + 128 - baseX) << 12;
        dropPos.x = x;
        dropPos.y = y;
        PokeathlonCourseSpritePoolSprite_SetPosition(work->dropEffects[i], &dropPos);
    }
    PokeathlonPennantCapture_SetDropEffectDrawPriority(work);

    isHost = viewer == 0 ? TRUE : FALSE;

    for (i = 0; i < 4; i++, players++) {
        PokeathlonPennantCapturePlayer *player = players;
        VecFx32 position;
        curX = frame->words[i] & 0x1FF;
        curY = (frame->words[i] >> 9) & 0x1FF;
        member = (frame->spots[i] >> 5) & 3;
        size = (u8)((frame->spots[i] >> 7) & 3) + 1;
        active = (frame->spots[i] >> 9) & 1;
        hidden = ((u8)((frame->words[4] >> 19) & 0xF) >> i) & 1;
        object = player->monObjects[member];
        if (PokeathlonCourse_GetParticipantCount(course) <= i) {
            remote = TRUE;
            block = PokeathlonCourse_ToBytes(link->others[i]);
        } else {
            remote = FALSE;
            block = PokeathlonCourse_ToBytes(link->own);
        }
        if (viewer == i) {
            UpdateMonSwapAsU16(work->display, player, frame, i, member, block, isHost, TRUE, FALSE);
        } else {
            UpdateMonSwapAsU16(work->display, player, frame, i, member, block, isHost, FALSE, remote);
        }
        px = curX - baseX + 128;
        py = curY - baseY + 96;
        py += player->drawOffsetY;
        position.x = px << 12;
        position.y = py << 12;
        position.z = 0;
        PokeathlonPennantCaptureDisplay_UpdateMon(work->display, i, member, hidden, active);
        if (!hidden) {
            if ((px >= -32 && px <= 288) || (py >= -32 && py <= 224)) {
                PokeathlonCourseMonObject_SetDrawFlag(object, TRUE);
            } else {
                PokeathlonCourseMonObject_SetDrawFlag(object, FALSE);
            }
        }
        for (j = 0; j < 3; j++) {
            PokeathlonCourseObject_SetGroundPosition(player->monObjects[j], px, py);
        }
        {
            VecFx32 carryPos = { 0, 0, 0 };
            carryPos.x = px << 12;
            carryPos.y = py << 12;
            if (PokeathlonCourse_GetPerformance(course, i, member)[5]) {
                carryPos.y -= 0x18000;
            } else {
                carryPos.y -= 0x10000;
            }
            PokeathlonCourseSpritePoolSprite_SetPosition(player->carrySprite, &carryPos);
        }
        PokeathlonPennantCapture_SetPlayerDrawPriority(player, py);
        if (!hidden) {
            remote = FALSE;
            if (active == TRUE) {
                if (active != player->active) {
                    PokeathlonCourseSpritePoolSprite_Show(player->carrySprite, 1, 1);
                }
                PokeathlonCourseMonObject_SetAnim(object, 21);
                player->flashTimer = 0;
                remote = TRUE;
                if (self == i) {
                    if (GF_SndCountPlayingSeqOnPlayer(3) == 0) {
                        PokeathlonPennantCapture_PlaySEIfOnScreen(position.x / FX32_ONE, position.y / FX32_ONE, SEQ_SE_GS_W146, 1);
                    }
                } else if (GF_SndCountPlayingSeqOnPlayer(4) == 0) {
                    PokeathlonPennantCapture_PlaySEIfOnScreen(position.x / FX32_ONE, position.y / FX32_ONE, SEQ_SE_GS_W146, 0);
                }
            } else {
                PokeathlonCourseSpritePoolSprite_Show(player->carrySprite, 1, 0);
            }
            if (!remote) {
                PokeathlonCourseMonObject_SetAnimStateRaw(object, size);
                if (player->flashTimer) {
                    PokeathlonCourseMonObject_SetAnim(object, 26);
                    player->flashTimer--;
                } else {
                    PokeathlonCourseMonObject_SetAnim(object, 0);
                }
            }
        } else {
            PokeathlonCourseSpritePoolSprite_Show(player->carrySprite, 1, 0);
            PokeathlonCourseMonObject_SetAnimState(object, size);
        }
        player->active = active;
        pennants = (frame->words[i] >> 28) & 0xF;
        PokeathlonPennantCaptureDisplay_SetPennants(work->display, i, pennants);
        if (player->shownPennants < pennants) {
            player->flashTimer = 10;
            PokeathlonCourseSpritePoolSprite_SetAnim(player->pennantSprite, 8);
            PokeathlonCourseSpritePoolSprite_Show(player->pennantSprite, 1, 1);
            if (self == i) {
                PokeathlonPennantCapture_PlaySEIfOnScreen(position.x / FX32_ONE, position.y / FX32_ONE, SEQ_SE_DP_PIRORIRO, 1);
            } else {
                PokeathlonPennantCapture_PlaySEIfOnScreen(position.x / FX32_ONE, position.y / FX32_ONE, SEQ_SE_DP_BOX02, 0);
            }
        } else if (!hidden && player->shownPennants > pennants) {
            PokeathlonCourseSpritePoolSprite_SetAnim(player->pennantSprite, 9);
            PokeathlonCourseSpritePoolSprite_Show(player->pennantSprite, 1, 1);
        }
        player->shownPennants = pennants;
        PokeathlonCourseSpritePoolSprite_SetPosition(player->pennantSprite, &position);
        score = (frame->words[i] >> 18) & 0x3FF;
        if (viewer == i) {
            PokeathlonPennantCaptureDisplay_SetScore(work->display, score);
        }
        if (player->shownScore < score) {
            PokeathlonCourseSpritePoolSprite_SetAnim(player->pointSprites[player->nextPointSprite], 3);
            PokeathlonCourseSpritePoolSprite_Show(player->pointSprites[player->nextPointSprite], 1, 1);
            player->nextPointSprite = (player->nextPointSprite + 1) % 2;
            PokeathlonPennantCaptureDisplay_ShowPoint(work->display, i);
            if (self == i) {
                PokeathlonPennantCapture_PlaySEIfOnScreen(position.x / FX32_ONE, position.y / FX32_ONE, SEQ_SE_GS_POINT3, 1);
            } else {
                PokeathlonPennantCapture_PlaySEIfOnScreen(position.x / FX32_ONE, position.y / FX32_ONE, SEQ_SE_GS_POINT3, 0);
            }
        }
        for (pennants = 0; pennants < 2; pennants++) {
            PokeathlonCourseSpritePoolSprite_SetPosition(player->pointSprites[pennants], &position);
        }
        player->shownScore = score;
    }
}

#pragma force_active reset

void PokeathlonPennantCapture_UpdateObstacleSprites(PokeathlonPennantCapture *work, int x, int y) {
    u8 i;

    for (i = 0; i < 20; i++) {
        int dx;
        int dy;
        VecFx32 position;
        if (work->obstacles[i].object == NULL) {
            break;
        }
        dx = work->obstacles[i].x - x + 0x80;
        dy = work->obstacles[i].y - y + 0x60;
        position.z = 0;
        position.x = dx * FX32_ONE;
        position.y = dy * FX32_ONE;
        PokeathlonCourseSpritePoolSprite_SetPosition(work->obstacles[i].sprite, &position);
        if (dx >= -32 && dx <= 288 && dy >= -32 && dy <= 224) {
            PokeathlonCourseSpritePoolSprite_Show(work->obstacles[i].sprite, 1, 1);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->obstacles[i].sprite, 1, 0);
        }
    }
}

void PokeathlonPennantCapture_UpdateFieldItemSprites(PokeathlonPennantCapture *work, int x, int y) {
    u8 i;

    for (i = 0; i < 10; i++) {
        int dx;
        int dy;
        VecFx32 position;
        if (work->fieldItems[i].object == NULL) {
            break;
        }
        dx = work->fieldItems[i].x - x + 0x80;
        dy = work->fieldItems[i].y - y + 0x60;
        position.z = 0;
        position.x = dx * FX32_ONE;
        position.y = dy * FX32_ONE;
        PokeathlonCourseSpritePoolSprite_SetPosition(work->fieldItems[i].sprite, &position);
        PokeathlonCourseSpritePoolSprite_SetPosition(work->fieldItems[i].shadowSprite, &position);
        if (dx >= -32 && dx <= 288 && dy >= -32 && dy <= 224) {
            PokeathlonCourseSpritePoolSprite_Show(work->fieldItems[i].sprite, 1, 1);
            PokeathlonCourseSpritePoolSprite_Show(work->fieldItems[i].shadowSprite, 1, 1);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->fieldItems[i].sprite, 1, 0);
            PokeathlonCourseSpritePoolSprite_Show(work->fieldItems[i].shadowSprite, 1, 0);
        }
    }
}

void PokeathlonPennantCapture_UpdatePennantSprites(PokeathlonPennantCapture *work, PokeathlonPennantCaptureFrame *frame, int x, int y) {
    u8 i;
    PokeathlonPennantCapturePennant *base;

    for (i = 0, base = work->pennants.pennants; i < 5; i++) {
        int spot;
        int dx;
        int dy;
        VecFx32 position;
        spot = frame->spots[i] & 0x1F;
        dx = work->pennants.spots[spot].x - x + 0x80;
        dy = work->pennants.spots[spot].y - y + 0x60;
        position.z = 0;
        position.x = dx * FX32_ONE;
        position.y = dy * FX32_ONE;
        PokeathlonCourseSpritePoolSprite_SetPosition(work->pennants.pennants[i].sprite, &position);
        PokeathlonCourseSpritePoolSprite_SetPosition(work->pennants.pennants[i].poleSprite, &position);
        if (dx >= -32 && dx <= 288 && dy >= -32 && dy <= 224) {
            PokeathlonCourseSpritePoolSprite_Show(work->pennants.pennants[i].sprite, 1, 1);
            PokeathlonCourseSpritePoolSprite_Show(work->pennants.pennants[i].poleSprite, 1, 1);
        } else {
            PokeathlonCourseSpritePoolSprite_Show(work->pennants.pennants[i].sprite, 1, 0);
            PokeathlonCourseSpritePoolSprite_Show(work->pennants.pennants[i].poleSprite, 1, 0);
        }
        if (spot != work->shownPennantSpots[i]) {
            PokeathlonCourseSpritePoolSprite_SetAnim(work->pennants.pennants[i].poleSprite, 4);
            work->shownPennantSpots[i] = spot;
        }
        PokeathlonPennantCapture_SetPennantDrawPriority(&base[i], dy);
    }
}

// The Pokemon jumps off the field (32 frames), then the top screen swaps the team member's picture.
void PokeathlonPennantCapture_RunMonExit(PokeathlonPennantCaptureDisplay *display, PokeathlonPennantCapturePlayer *player, int participant, int member, u8 *block, int waitOther, int isSelf) {
    switch (player->swapStep) {
    case 0:
        player->swapTimer = 0;
        player->swapStep++;
        break;
    case 1:
        player->drawOffsetY = player->drawOffsetY + 8;
        player->swapTimer++;
        if (player->swapTimer >= 0x20) {
            PokeathlonCourseMonObject_SetDrawFlag(player->monObjects[member], 0);
            player->swapStep++;
        }
        break;
    case 2:
        if (isSelf) {
            if (PokeathlonPennantCaptureDisplay_SlideMonPic(display, member)) {
                block[8] = 1;
                player->swapStep++;
            }
        } else if (waitOther) {
            if (PokeathlonPennantCaptureDisplay_StepSwapWait(display, participant)) {
                block[8] = 1;
                player->swapStep++;
            }
        } else {
            player->swapStep++;
        }
        break;
    case 3:
        break;
    }
}

// The new team member drops back onto the field (32 frames).
void PokeathlonPennantCapture_RunMonEnter(PokeathlonPennantCapturePlayer *player, int participant, int member, u8 *block, int report) {
    switch (player->swapStep) {
    case 0:
        player->swapTimer = 0;
        PokeathlonCourseMonObject_SetDrawFlag(player->monObjects[member], 1);
        player->swapStep++;
        break;
    case 1:
        player->drawOffsetY = player->drawOffsetY - 8;
        player->swapTimer++;
        if (player->swapTimer >= 0x20) {
            player->drawOffsetY = 0;
            player->swapStep++;
        }
        break;
    case 2:
        if (report) {
            block[8] = 2;
        }
        player->swapStep++;
        break;
    case 3:
        break;
    }
}

void PokeathlonPennantCapture_UpdateMonSwap(PokeathlonPennantCaptureDisplay *display, PokeathlonPennantCapturePlayer *player, PokeathlonPennantCaptureFrame *frame, u8 participant, u8 member, u8 *block, int isHost, int isSelf, int remote) {
    u8 states = frame->words[4] >> 23;
    u8 state = (states >> (participant * 2)) & 3;

    if (player->swapState != state) {
        player->swapState = state;
        player->swapStep = 0;
    }
    switch (player->swapState) {
    case 0:
        break;
    case 1:
        PokeathlonPennantCapture_RunMonExit(display, player, participant, member, block, (isHost && remote) ? 1 : 0, isSelf);
        break;
    case 2:
        PokeathlonPennantCapture_RunMonEnter(player, participant, member, block, isSelf | ((isHost && remote) ? 1 : 0));
        break;
    }
}

#pragma force_active on

void PokeathlonPennantCapture_SetMoveParams(PokeathlonCourseData *course, const PokeathlonPennantCaptureMoveTables *tables, int participant, u8 member, PokeathlonPennantCapturePlayer *player) {
    u8 *levels = PokeathlonCourse_GetMonStats(course, participant, member);

    player->moveParams[member].knockbackScale = (f32)tables->knockbackScale[levels[0]] / 10.0;
    player->moveParams[member].maxSpeed = tables->maxSpeed[levels[1]];
    player->moveParams[member].unk00 = (f32)tables->unk00[levels[4]] / 10.0;
    player->moveParams[member].unk04 = (f32)tables->unk04[levels[1]] / 10.0;
    player->moveParams[member].knockdownTime = tables->knockdownTime[levels[3]];
}

#pragma force_active reset

void PokeathlonPennantCapture_GetObstaclesNear(PokeathlonPennantCapture *work, fx32 fieldX, fx32 fieldY, PokeathlonPennantCapturePoint **out, u8 *count) {
    int i;
    int x;
    int y;
    PokeathlonPennantCapturePoint *position;

    for (i = 0; i < 20; i++) {
        out[i] = NULL;
    }
    x = fieldX / FX32_ONE;
    y = fieldY / FX32_ONE;
    *count = 0;
    position = work->obstaclePositions;
    for (i = 0; i < 20; i++, position++) {
        int dx;
        int dy;
        if (work->obstacles[i].object == NULL) {
            break;
        }
        dx = work->obstacles[i].x - x + 0x80;
        dy = work->obstacles[i].y - y + 0x60;
        if (dx >= -32 && dx <= 288 && dy >= -32 && dy <= 224) {
            out[(*count)++] = position;
        }
    }
}

void PokeathlonPennantCapture_CalcObstacleBounds(int radius, const PokeathlonPennantCapturePoint *center, PokeathlonPennantCaptureBounds *bounds) {
    Point2D *out = bounds->corners;
    int x = center->x;
    int y = center->y;

    out[0].x = x - 16;
    out[0].y = y - 16;
    out[1].x = x + 16;
    out[1].y = y - 16;
    out[2].x = x + 16;
    out[2].y = y + 16;
    out[3].x = x - 16;
    out[3].y = y + 16;
    out[4] = out[0];
    out[5] = out[1];
    out[6] = out[1];
    out[7] = out[2];
    out[8] = out[2];
    out[9] = out[3];
    out[10] = out[3];
    out[11] = out[0];
    out[4].y -= radius;
    out[5].y -= radius;
    out[6].x += radius;
    out[7].x += radius;
    out[8].y += radius;
    out[9].y += radius;
    out[10].x -= radius;
    out[11].x -= radius;
}

#pragma force_active on

int PokeathlonPennantCapture_CollideWithObstacle(VecFx32 *from, VecFx32 *to, int radius, PokeathlonPennantCaptureBounds *bounds, VecFx32 *out) {
    VecFx32 *end = to;
    VecFx32 *result = out;
    int i;
    Point2D hit;
    Point2D start;
    Point2D stop;
    fx32 t;
    BOOL onEdge;
    int sideX, sideY;
    int lo;
    BOOL useX, useY;
    int hi;
    Point2D *corner;
    int x0, x1, y0, y1;
    s32 ret;
    start.x = from->x / FX32_ONE;
    int dx0, dx1, dy0, dy1;
    start.y = from->y / FX32_ONE;
    stop.x = end->x / FX32_ONE;
    stop.y = end->y / FX32_ONE;
    for (i = 0; i < 4; ++i) {
        PokeathlonPennantCaptureHitCodes codes = ov96_0221CB00;
        if (Point2D_CalcSegmentIntersection(&bounds->edges[i][0], &bounds->edges[i][1], &start, &stop, &hit)) {
            result->x = hit.x << 12;
            result->y = hit.y << 12;
            result->z = 0;
            return codes.sides[i];
        }
        onEdge = FALSE;
        if (i == 0 && i == 2) {
            if (start.y == bounds->edges[i][0].y) {
                if (bounds->edges[i][0].x < bounds->edges[i][1].x) {
                    lo = bounds->edges[i][0].x;
                    hi = bounds->edges[i][1].x;
                } else {
                    lo = bounds->edges[i][1].x;
                    hi = bounds->edges[i][0].x;
                }
                if (lo <= start.x && start.x <= hi) {
                    onEdge = TRUE;
                }
            }
        } else {
            if (start.x == bounds->edges[i][0].x) {
                if (bounds->edges[i][0].y < bounds->edges[i][1].y) {
                    lo = bounds->edges[i][0].y;
                    hi = bounds->edges[i][1].y;
                } else {
                    lo = bounds->edges[i][1].y;
                    hi = bounds->edges[i][0].y;
                }
                if (lo <= start.y && start.y <= hi) {
                    onEdge = TRUE;
                }
            }
        }
        if (onEdge) {
            *result = *from;
            return codes.sides[i];
        }
    }
    i = 0, corner = bounds->corners;
    while (i < 4) {
        PokeathlonPennantCaptureHitCodes codes = ov96_0221CAE0;
        VecFx32 point;
        VecFx32 delta;
        point.x = corner->x << 12;
        point.y = corner->y << 12;
        point.z = 0;
        VEC_Subtract(end, &point, &delta);
        if (VEC_Mag(&delta) <= radius << 12) {
            VecFx32 path;
            VecFx32 touch;
            t = PokeathlonPennantCapture_CalcCircleEntry(from, end, &point, radius * FX32_ONE);
            VEC_Subtract(end, from, &path);
            VEC_MultAdd(t, &path, from, &touch);
            result->x = touch.x;
            result->y = touch.y;
            result->z = 0;
            return codes.sides[i];
        }
        i++, corner++;
    }
    // ended inside the block: push out through the nearest side
    if (bounds->corners[0].x << 12 <= end->x && end->x <= bounds->corners[2].x << 12 && bounds->corners[0].y << 12 <= end->y && end->y <= bounds->corners[2].y << 12) {
        *result = *end;
        x0 = bounds->corners[0].x;
        useX = useY = FALSE;
        dx0 = (x0 << 12) - end->x;
        x1 = bounds->corners[2].x;
        dx1 = (x1 << 12) - end->x;
        if (dx0 < 0) {
            dx0 *= -1;
        }
        if (dx1 < 0) {
            dx1 *= -1;
        }
        if (dx0 < dx1) {
            sideX = 12;
        } else {
            dx0 = dx1;
            sideX = 10;
        }
        y0 = bounds->corners[0].y;
        dy0 = (y0 << 12) - end->y;
        y1 = bounds->corners[2].y;
        dy1 = (y1 << 12) - end->y;
        if (dy0 < 0) {
            dy0 *= -1;
        }
        if (dy1 < 0) {
            dy1 *= -1;
        }
        if (dy0 < dy1) {
            sideY = 9;
        } else {
            dy0 = dy1;
            sideY = 11;
        }
        if (dx0 <= dy0) {
            useX = TRUE;
            ret = sideX;
        } else {
            useY = TRUE;
            ret = sideY;
        }
        if (useX) {
            if (sideX == 12) {
                result->x = (x0 - radius) << 12;
            } else {
                result->x = (x1 + radius) << 12;
            }
            return ret;
        }
        if (useY) {
            if (sideY == 9) {
                result->y = (y0 - radius) << 12;
            } else {
                result->y = (y1 + radius) << 12;
            }
            return ret;
        }
        return ret;
    }
    return 0;
}

#pragma force_active reset

int PokeathlonPennantCapture_CollideWithObstacles(PokeathlonPennantCapture *work, int excluded, int radius, VecFx32 *position, VecFx32 *end, VecFx32 *out) {
    u8 count;
    u8 result;
    int i;
    VecFx32 localOut;
    PokeathlonPennantCapturePoint *candidates[20];
    PokeathlonPennantCaptureBounds bounds;

    PokeathlonPennantCapture_GetObstaclesNear(work, position->x, position->y, candidates, &count);
    i = 0;
    result = 0;
    for (; i < count; i++) {
        int code;
        PokeathlonPennantCapture_CalcObstacleBounds(radius, candidates[i], &bounds);
        code = PokeathlonPennantCapture_CollideWithObstacle(position, end, radius, &bounds, &localOut);
        if (code != 0) {
            if (code >= 9) {
                out->x = localOut.x;
                out->y = localOut.y;
                out->z = 0;
            } else if (code != excluded) {
                out->x = localOut.x;
                out->y = localOut.y;
                out->z = 0;
            }
            result = (u8)code;
            break;
        }
    }
    return result;
}

// The smaller root t of |a + t (b - a) - point| = radius.
fx32 PokeathlonPennantCapture_CalcCircleEntry(VecFx32 *a, VecFx32 *b, VecFx32 *point, fx32 radius) {
    VecFx32 delta;
    fx32 magnitude;
    fx32 dot;
    fx32 length;
    fx32 root;
    fx32 first;
    fx32 second;
    VEC_Subtract(b, a, &delta);
    magnitude = VEC_Mag(&delta);
    magnitude *= magnitude;
    dot = VEC_DotProduct(&delta, point);
    length = VEC_Mag(point);
    length *= length;
    root = FX_Sqrt(dot - (length - radius * radius) * magnitude);
    first = FX_Div(root - dot, magnitude);
    second = FX_Div(-(dot + root), magnitude);
    if (first <= second) {
        second = first;
    }
    return second;
}

float PokeathlonPennantCapture_GetMaxSpeed(PokeathlonPennantCapturePlayer *player, int member) {
    return (float)player->moveParams[member].maxSpeed;
}

void PokeathlonPennantCapture_AddPoint2D(const Point2D *a, const Point2D *b, Point2D *out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}

#pragma force_active on
#pragma opt_loop_invariants off
#pragma opt_strength_reduction off

static inline void PokeathlonPennantCapture_ZeroVec(VecFx32 *v) {
    memset(v, 0, sizeof(*v));
}

// Written as byte address arithmetic: indexing contacts[] directly does not match.
#define CONTACT(player, i) (*((u8 *)((u32)(player) + (u32)(i)) + 0x94))

int PokeathlonPennantCapture_CollidePlayers(PokeathlonCourseData *course, PokeathlonPennantCapturePlayer *players, VecFx32 *out) {
    void *objectA;
    void *objectB;
    int applied;
    int i, j;
    PokeathlonPennantCapturePlayer *a, *b;
    int aX;
    PokeathlonPennantCaptureCollisionFrame frame;
    applied = 0;
    frame.pointA.z = 0;
    frame.pointB.z = 0;
    for (i = 0, a = players; i < 4; i++, a++) {
        int member = a->member;
        if (a->depositing) {
            continue;
        }
        objectA = a->monObjects[member];
        PokeathlonCourseObject_GroundToCenter(objectA, a->position.x / 4096, a->position.y / 4096, &frame.aX, &frame.aY);
        for (j = 0, b = players; j < 4; j++, b++) {
            if (i == j || b->depositing) {
                continue;
            }
            aX = frame.aX;
            objectB = b->monObjects[b->member];
            PokeathlonCourseObject_GroundToCenter(objectB, b->position.x / 4096, b->position.y / 4096, &frame.bX, &frame.bY);
            PokeathlonCourseObject_GetCollisionCircle(objectB, (s32)((u32)frame.bX << 12), (s32)((u32)frame.bY << 12), &frame.pointB.x, &frame.pointB.y, &frame.radiusB);
            PokeathlonCourseObject_GetCollisionCircle(objectA, (s32)((u32)aX << 12), (s32)((u32)frame.aY << 12), &frame.pointA.x, &frame.pointA.y, &frame.radiusA);
            VEC_Subtract(&frame.pointA, &frame.pointB, &frame.diff);
            if (VEC_Mag(&frame.diff) < (s32)(((u32)frame.radiusB + (u32)frame.radiusA) << 12)) {
                if (CONTACT(a, j)) {
                    continue;
                }
                if (!PokeathlonPennantCapture_TryKnockDown(course, a, b)) {
                    a->freezeTimer = 6;
                    b->freezeTimer = 6;
                }
                if (VEC_Mag(&a->velocity) > 11 * 4096) {
                    PokeathlonPennantCapture_ZeroVec(&frame.zeroA);
                    VEC_Normalize(&a->velocity, &a->velocity);
                    VEC_MultAdd(11 * 4096, &a->velocity, &frame.zeroA, &a->velocity);
                }
                if (VEC_Mag(&b->velocity) > 11 * 4096) {
                    PokeathlonPennantCapture_ZeroVec(&frame.zeroB);
                    VEC_Normalize(&b->velocity, &b->velocity);
                    VEC_MultAdd(11 * 4096, &b->velocity, &frame.zeroB, &b->velocity);
                }
                GF_ASSERT(CONTACT(a, j) == 0);
                GF_ASSERT(CONTACT(b, i) == 0);
                CONTACT(a, j) = 1;
                CONTACT(b, i) = 1;
                if (!applied) {
                    PokeathlonPennantCapture_ZeroVec(&frame.resolved);
                    applied = 1;
                    VEC_Subtract(&frame.pointB, &frame.pointA, &frame.resolved);
                    VEC_Normalize(&frame.resolved, &frame.resolved);
                    VEC_MultAdd((s32)((u32)frame.radiusA << 12), &frame.resolved, &frame.pointA, &frame.resolved);
                    *out = frame.resolved;
                }
                PokeathlonCourse_AddMonCounter(course, (u8)a->participant, a->member, 4, 1);
                PokeathlonCourse_AddMonCounter(course, (u8)b->participant, b->member, 4, 1);
                break;
            } else {
                CONTACT(a, j) = 0;
                *((u8 *)((u32)i + (u32)b) + 0x94) = 0;
            }
        }
    }
    return applied;
}

#pragma opt_loop_invariants reset
#pragma opt_strength_reduction reset
#pragma force_active reset

void PokeathlonPennantCapture_UpdateFieldDrawPriorities(PokeathlonPennantCapture *work) {
    int i;

    for (i = 0; i < 10; i++) {
        int priority;
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->fieldItems[i].shadowSprite, 3);
        priority = PokeathlonPennantCapture_GetDrawPriority(PokeathlonCourseSpritePoolSprite_GetPosition(work->fieldItems[i].sprite)->y / FX32_ONE);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->fieldItems[i].sprite, priority + 7);
    }

    for (i = 0; i < 20; i++) {
        int priority = PokeathlonPennantCapture_GetDrawPriority(PokeathlonCourseSpritePoolSprite_GetPosition(work->obstacles[i].sprite)->y / FX32_ONE);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->obstacles[i].sprite, priority + 1);
    }
}

void PokeathlonPennantCapture_SetPennantDrawPriority(PokeathlonPennantCapturePennant *pennant, int y) {
    int priority = PokeathlonPennantCapture_GetDrawPriority(y);

    PokeathlonCourseSpritePoolSprite_SetDrawPriority(pennant->sprite, priority + 9);
    PokeathlonCourseSpritePoolSprite_SetDrawPriority(pennant->poleSprite, priority + 8);
}

void PokeathlonPennantCapture_SetDropEffectDrawPriority(PokeathlonPennantCapture *work) {
    int i;

    for (i = 0; i < 4; i++) {
        int priority = PokeathlonPennantCapture_GetDrawPriority(PokeathlonCourseSpritePoolSprite_GetPosition(work->dropEffects[i])->y / FX32_ONE);
        PokeathlonCourseSpritePoolSprite_SetDrawPriority(work->dropEffects[i], priority + 2);
    }
}

void PokeathlonPennantCapture_SetPlayerDrawPriority(PokeathlonPennantCapturePlayer *player, int y) {
    int i;
    int priority = PokeathlonPennantCapture_GetDrawPriority(y);

    PokeathlonCourseSpritePoolSprite_SetDrawPriority(player->carrySprite, priority + 3);
    for (i = 0; i < 3; i++) {
        PokeathlonCourseMonObject_SetDrawPriority(player->monObjects[i], priority + 4);
    }
}

void PokeathlonPennantCapture_InitScoreWindow(PokeathlonPennantCapture *work) {
    AddWindow(work->bgConfig, &work->scoreWindow, &ov96_0221CAC0);
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
}

void PokeathlonPennantCapture_PrintScore(PokeathlonPennantCapture *work, u16 score) {
    MsgData *msgData;
    MessageFormat *format;
    String *string;

    FillWindowPixelBuffer(&work->scoreWindow, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, work->heapId);
    format = MessageFormat_New(work->heapId);
    BufferIntegerAsString(format, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(format, msgData, msg_0309_00157, work->heapId);
    AddTextPrinterParameterizedWithColor(&work->scoreWindow, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    MessageFormat_Delete(format);
    DestroyMsgData(msgData);
    CopyWindowToVram(&work->scoreWindow);
}

void PokeathlonPennantCapture_ResetPennantSpots(PokeathlonPennantCapture *work) {
    work->pennants.nextSpot = 0;
}

void PokeathlonPennantCapture_AdvancePennantSpot(PokeathlonPennantCapturePennants *pennants) {
    pennants->nextSpot = (pennants->nextSpot + 1) % 20;
}

void PokeathlonPennantCapture_PlacePennant(PokeathlonPennantCapturePennants *pennants) {
    int i;

    for (i = 0; i < 5; i++) {
        if (pennants->pennants[i].active == 0) {
            pennants->pennants[i].spot = pennants->nextSpot;
            pennants->pennants[i].x = pennants->spots[pennants->nextSpot].x;
            pennants->pennants[i].y = pennants->spots[pennants->nextSpot].y;
            PokeathlonPennantCapture_AdvancePennantSpot(pennants);
            pennants->pennants[i].active = 1;
            return;
        }
    }
    GF_AssertFail();
}

void PokeathlonPennantCapture_RefillPennants(PokeathlonPennantCapture *work) {
    int i;
    int missing = 5 - work->pennants.count;

    for (i = 0; i < missing; i++) {
        PokeathlonPennantCapture_PlacePennant(&work->pennants);
        work->pennants.count++;
        if (work->pennants.count > 5) {
            GF_AssertFail();
        }
    }
}

BOOL PokeathlonPennantCapture_PickUpPennants(PokeathlonPennantCapturePennants *pennants, PokeathlonPennantCapturePlayer *player, u8 *full) {
    int i;
    VecFx32 position;

    *full = FALSE;
    for (i = 0; i < 5; i++) {
        if (pennants->pennants[i].active != 0) {
            position.x = pennants->spots[pennants->pennants[i].spot].x << FX32_SHIFT;
            position.y = pennants->spots[pennants->pennants[i].spot].y << FX32_SHIFT;
            position.z = 0;
            if (PokeathlonPennantCapture_CirclesOverlap(&player->position, &position, PokeathlonCourseObject_GetCollisionRadius(player->monObjects[player->member]) << FX32_SHIFT, 0x8000)) {
                if (player->pennants >= 9) {
                    if (player->touchingPennant[i] == 0) {
                        *full = TRUE;
                        player->touchingPennant[i] = TRUE;
                    }
                    return FALSE;
                }
                player->touchingPennant[i] = TRUE;
                pennants->pennants[i].active = FALSE;
                player->pennants++;
                if (pennants->count == 0) {
                    GF_AssertFail();
                }
                pennants->count--;
                return TRUE;
            }
            player->touchingPennant[i] = FALSE;
        } else {
            player->touchingPennant[i] = FALSE;
        }
    }
    return FALSE;
}

BOOL PokeathlonPennantCapture_CirclesOverlap(const VecFx32 *a, const VecFx32 *b, fx32 radiusA, fx32 radiusB) {
    VecFx32 delta;

    VEC_Subtract(a, b, &delta);
    if (VEC_Mag(&delta) < radiusA + radiusB) {
        return TRUE;
    }
    return FALSE;
}

// 1 = up, 2 = right, 3 = down, 4 = left (from the angle to ov96_0221CAC8)
int PokeathlonPennantCapture_GetDirection(const VecFx32 *vector) {
    VecFx32 normalized;
    VecFx32 reference;
    VecFx32 rotated;
    u16 angle;
    int result = 1;

    VEC_Normalize(vector, &normalized);
    if (VEC_Mag(&normalized) != 0) {
        reference = ov96_0221CAC8;
        rotated.x = normalized.y;
        rotated.z = normalized.x;
        rotated.y = 0;
        angle = CalcAngleBetweenVecs(&reference, &rotated);
        if (angle <= 0x2000 || angle >= 0xE000) {
            result = 4;
        } else if (angle > 0x2000 && angle < 0x6000) {
            result = 2;
        } else if (angle >= 0x6000 && angle <= 0xA000) {
            result = 3;
        } else {
            result = 1;
        }
    }
    return result;
}

// A faster Pokemon running into another one that faces the same way knocks it down and takes up to two pennants.
BOOL PokeathlonPennantCapture_TryKnockDown(PokeathlonCourseData *course, PokeathlonPennantCapturePlayer *a, PokeathlonPennantCapturePlayer *b) {
    fx32 magA;
    fx32 magB;
    PokeathlonPennantCapturePlayer *fast;
    fx32 scale;
    u8 member;

    magA = VEC_Mag(&a->velocity);
    magB = VEC_Mag(&b->velocity);
    if (magA > magB) {
        fast = a;
    } else if (magA < magB) {
        fast = b;
        b = a;
    } else {
        return FALSE;
    }
    if (fast->direction != b->direction) {
        return FALSE;
    }
    if (PokeathlonPennantCapture_GetDirection(&fast->velocity) != fast->direction) {
        return FALSE;
    }
    if (fast->knockedDown == 1) {
        return FALSE;
    }
    if (b->pennants >= 2) {
        b->pennants -= 2;
        fast->pennants += 2;
    } else {
        fast->pennants += b->pennants;
        b->pennants = 0;
    }
    if (fast->pennants > 9) {
        fast->pennants = 9;
    }
    b->knockedDown = 1;
    PokeathlonCourse_AddMonCounter(course, (u8)b->participant, b->member, 1, 1);
    member = b->member;
    b->knockdownTimer = b->moveParams[member].knockdownTime + VEC_Mag(&fast->velocity) / FX32_ONE;
    {
        VecFx32 zero = { 0, 0, 0 };
        scale = (fx32)(4096.0f * fast->moveParams[fast->member].knockbackScale);
        VEC_MultAdd(scale, &fast->velocity, &zero, &b->velocity);
        fast->velocity = zero;
    }
    fast->freezeTimer = 0x12;
    b->freezeTimer = 6;
    return TRUE;
}

BOOL PokeathlonPennantCapture_CanDeposit(PokeathlonPennantCapturePlayer *player) {
    BOOL inGoal = FALSE;

    if (player->position.y > 0x198000 && player->position.x >= 0xD0000 && player->position.x <= 0x130000) {
        inGoal = TRUE;
    }
    if (!inGoal) {
        return FALSE;
    }
    if (player->pennants == 0) {
        return FALSE;
    }
    if (player->knockedDown == 0) {
        return TRUE;
    }
    return FALSE;
}

// Called as if it returned its argument (it does: r0 is left unchanged).
u8 PokeathlonPennantCapture_GetDepositPoints(u8 pennants) {
}

int PokeathlonPennantCapture_GetDrawPriority(int y) {
    return (0x100 - y) * 8 + 5;
}

void PokeathlonPennantCapture_AnimateBg(PokeathlonPennantCapture *work) {
    if (work->bgAnimTimer >= 4) {
        void *chars = work->bgChars->pRawData;
        u32 offset = work->bgAnimFrame << 11;
        DC_FlushRange((u8 *)chars + offset, 0x800);
        GX_LoadBG0Char((u8 *)chars + offset, 0x1800, 0x800);
        work->bgAnimTimer = 0;
        work->bgAnimFrame = (work->bgAnimFrame + 1) % 6;
    } else {
        work->bgAnimTimer++;
    }
}

void PokeathlonPennantCapture_PlaySEIfOnScreen(int x, int y, int seq, BOOL self) {
    if (x >= 0 && x < 256 && y >= 0 && y < 192) {
        PlaySEOnPlayer((u16)seq, self ? 3 : 4);
    }
}

#pragma force_active on

void PokeathlonPennantCapture_UpdateOwnScore(PokeathlonCourseData *course) {
    PokeathlonPennantCaptureFrame *frame = (PokeathlonPennantCaptureFrame *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 240);
    PokeathlonPennantCapture *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 self = (u8)PokeathlonCourse_GetNetId(course);
    u16 score = (u16)((frame->words[self] >> 18) & 1023);
    u8 bonus = (u8)((frame->spots[self] >> 11) & 31);
    score = (u16)(score + bonus);
    if (score > 999) {
        score = 999;
    }
    PokeathlonCourse_SetRecordScore(course, score);
    PokeathlonPennantCapture_PrintScore(work, score);
}

#pragma force_active reset
