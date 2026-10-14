#include "pokeathlon/pokeathlon_hurdle_dash_race.h"

#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_course_internal.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

typedef char PokeathlonHurdleDashSizeCheck[(sizeof(PokeathlonHurdleDash) == 0x3C8 && sizeof(PokeathlonHurdleDashRunner) == 0x6C && sizeof(PokeathlonHurdleDashMon) == 0x40 && sizeof(PokeathlonHurdleDashCpuRunner) == 0x28 && sizeof(PokeathlonHurdleDashTrack) == 0xC24 && sizeof(PokeathlonHurdleDashStandings) == 0x244 && sizeof(PokeathlonHurdleDashPacket) == 0x24) ? 1 : -1];
typedef char PokeathlonHurdleDashLayoutCheck[(offsetof(PokeathlonHurdleDash, track) == 0xD8 && offsetof(PokeathlonHurdleDash, standings) == 0x228 && offsetof(PokeathlonHurdleDash, raceTime) == 0x22C && offsetof(PokeathlonHurdleDash, rankTimer) == 0x3C4 && offsetof(PokeathlonHurdleDashRunner, mon) == 0x24 && offsetof(PokeathlonHurdleDashRunner, rankScale) == 0x64 && offsetof(PokeathlonHurdleDashCpuRunner, topSpeed) == 0x10 && offsetof(PokeathlonHurdleDashCpuRunner, lastHurdle) == 0x24) ? 1 : -1];

#define LIST_INSERT_BEFORE(node, at) \
    (at)->prev->next = (node);       \
    (node)->prev = (at)->prev;       \
    (node)->next = (at);             \
    (at)->prev = (node)

// Sound helpers no header declares yet.
void PlaySEOnPlayer(u32 seqNo, u32 playerNo);
void SetPlayerPan(u32 playerNo, s32 pan);

// The compiler's division helper, called directly by the timer code.
s32 _s32_div_f(s32 num, s32 den);

// Course object and sprite pool helpers (pokeathlon_course_objects.c); declared here because SetScale is
// called with the two float scales its header types as pointers.
s32 PokeathlonCourseObject_GetCollisionRadius(void *object);
s32 PokeathlonCourseObject_GetCollisionOffsetY(void *object);
void PokeathlonCourseObject_SetScale(void *object, f32 x, f32 y);
void *PokeathlonCourseSpritePool_Take(void *pool, int resId, int kind);
void *PokeathlonCourseSpritePool_CreateAndTake(void *pool, int priority, int whichScreen, int resId, u8 kind);
void PokeathlonCourseSpritePoolSprite_Show(void *entry, BOOL shown, BOOL draw);
void PokeathlonCourseSpritePoolSprite_SetAnim(void *entry, int seq);
BOOL PokeathlonCourseSpritePoolSprite_IsAnimated(void *entry);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *entry, const VecFx32 *position);
VecFx32 *PokeathlonCourseSpritePoolSprite_GetPosition(void *entry);
Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(void *entry);
void PokeathlonCourseSpritePoolSprite_SetDrawPriority(void *entry, int priority);

extern const s8 sHurdleDashRisingJumpHeights[][9];                   // height of a rising jump per frame, by jump stat
extern const s8 sHurdleDashFallingJumpHeights[][9];                   // height of a falling jump per frame, by jump stat
extern u8 gPokeathlonHurdleDashLaneSoundPlayers[];                            // sound player of each lane
extern const s8 gPokeathlonHurdleDashLanePans[];                      // pan of each lane
extern const PokeathlonHurdleDashLaneX sHurdleDashTouchLaneX; // x of each lane (touch)
extern const PokeathlonHurdleDashLaneX sHurdleDashDustLaneX; // x of each lane (dust)
extern const WindowTemplate sHurdleDashTimeWindowTemplate;            // the time window
extern const f32 sHurdleDashBoostSpeeds[];                     // speed above the top speed by boost level
extern const VecFx32 sHurdleDashNewHurdlePos;                   // position of a new hurdle

void *PokeathlonHurdleDashRunner_GetHurdle(PokeathlonHurdleDashRunner *runner, int index) {
    return PokeathlonHurdleDash_GetHurdle(runner->hurdles, index);
}

void PokeathlonHurdleDash_UpdateRunner(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashRunner *runner, PokeathlonHurdleDashTouch *touch) {
    BOOL timed;
    PokeathlonHurdle *hurdle;
    Sprite *marker;
    s32 i;
    s32 distance;
    f32 pixels;
    BOOL checkHurdles;
    PokeathlonHurdleDashRect rect;
    PokeathlonHurdleDashRect rect2;

    if (runner->finished != TRUE && runner->mon.state != 2 && runner->boostLevel == 0 && runner->speed < runner->mon.topSpeed) {
        runner->speed = runner->speed + runner->mon.acceleration;
        if (runner->speed > runner->mon.topSpeed) {
            runner->speed = runner->mon.topSpeed;
        }
    }
    if (touch != NULL && runner->mon.state == 0) {
        PokeathlonHurdleDashMon_StartJump(course, touch->unk4, runner->lane, &runner->mon);
        timed = FALSE;
        if (runner->mon.jumpStat != 0) {
            for (i = 0; i < 4; i++) {
                hurdle = PokeathlonHurdleDashRunner_GetHurdle(runner, i);
                if (PokeathlonHurdle_IsStanding(hurdle)) {
                    PokeathlonHurdle_GetRect(hurdle, &rect);
                    if (PokeathlonHurdleDash_IsJumpTimed(runner->mon.groundY + 0x110 - PokeathlonCourseObject_GetCollisionRadius(runner->mon.monObject), &rect)) {
                        timed = TRUE;
                        PokeathlonHurdleDashRunner_Boost(course, runner);
                        break;
                    }
                }
            }
        }
        marker = PokeathlonCourseSpritePoolSprite_GetSprite(runner->mon.jumpMarker);
        Sprite_SetDrawFlag(marker, TRUE);
        if (timed) {
            Sprite_SetAnimCtrlSeq(marker, 0);
        } else {
            Sprite_SetAnimCtrlSeq(marker, 1);
        }
    } else if (touch != NULL && runner->mon.state == 1 && runner->mon.phase == 3 && runner->mon.jumpStat != 0) {
        runner->mon.jumpQueued = 1;
        runner->mon.queuedJumps = runner->mon.queuedJumps + 1;
    }
    pixels = runner->speed + runner->speedFraction;
    distance = (s32)pixels;
    runner->speedFraction = pixels - (f32)distance;
    runner->distance = runner->distance + distance;
    PokeathlonHurdleDash_RevealHurdles(hurdleDash, runner->distance, runner->lane);
    PokeathlonHurdleDashRunner_UpdateDust(hurdleDash, runner);
    runner->subScrollY = runner->subScrollY - distance;
    if ((s32)(runner->subScrollY + 0x200) <= 0) {
        runner->subScrollY = runner->subScrollY + 0x200;
    }
    runner->mainScrollY = runner->subScrollY + 0x110;
    if ((s32)(runner->mainScrollY + 0x200) <= 0) {
        runner->mainScrollY = runner->mainScrollY + 0x200;
    }
    for (i = 0; i < 4; i++) {
        PokeathlonHurdle_Scroll(PokeathlonHurdleDashRunner_GetHurdle(runner, i), distance);
    }
    PokeathlonHurdleDashDust_Scroll(hurdleDash->dust, runner->lane, distance);
    PokeathlonHurdleDashMon_Update(runner->lane, &runner->mon, &runner->speed);
    if (runner->finished == TRUE) {
        if (runner->mon.state == 0) {
            runner->mon.state = 3;
        } else if (runner->mon.state == 3 || runner->mon.state == 1) {
            s32 groundY = runner->mon.groundY;
            if (runner->distance >= 0x1080) {
                if (groundY == 120) {
                    runner->mon.state = 4;
                    runner->speed = 0.0f;
                    runner->speedFraction = 0.0f;
                }
            } else if (runner->distance >= 0x1000) {
                runner->speed = 1.0f;
                runner->speedFraction = 0.0f;
                PokeathlonCourseMonObject_SetAnimState(runner->mon.monObject, 1);
                Sprite_SetAnimCtrlSeq(PokeathlonCourseSpritePoolSprite_GetSprite(runner->mon.boostEffect), 0);
            }
        }
        PokeathlonHurdleDashRunner_DrawFinishLine(hurdleDash->bgConfig, runner);
        return;
    } else {
        checkHurdles = TRUE;
        if (runner->mon.state == 1) {
            checkHurdles = FALSE;
        }
        if (runner->mon.jumpQueued == 1 && runner->mon.queuedJumps == 1) {
            checkHurdles = TRUE;
        }
        if (checkHurdles) {
            for (i = 0; i < 4; i++) {
                hurdle = PokeathlonHurdleDashRunner_GetHurdle(runner, i);
                if (PokeathlonHurdle_IsStanding(hurdle)) {
                    void *monObject = runner->mon.monObject;
                    PokeathlonHurdle_GetRect(hurdle, &rect2);
                    if (PokeathlonHurdleDash_IsTouchingHurdle(runner->mon.groundY + 0x110 - PokeathlonCourseObject_GetCollisionOffsetY(monObject), (u8)PokeathlonCourseObject_GetCollisionRadius(monObject), &rect2)) {
                        PokeathlonHurdle_KnockDown(hurdle);
                        PlaySEOnPlayer(SEQ_SE_GS_W436, gPokeathlonHurdleDashLaneSoundPlayers[runner->lane]);
                        SetPlayerPan(gPokeathlonHurdleDashLaneSoundPlayers[runner->lane], gPokeathlonHurdleDashLanePans[runner->lane]);
                        if (runner->mon.jumpQueued == 1 && runner->mon.queuedJumps == 1) {
                            PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), runner->lane, 2, 1);
                            runner->mon.timer = 3;
                            runner->mon.phase = 1;
                            PokeathlonHurdleDashRunner_Boost(course, runner);
                        } else {
                            runner->speed = 0.5f;
                            runner->boostLevel = 0;
                            runner->mon.state = 2;
                            runner->mon.spinTimer = 0;
                            runner->mon.direction = 1;
                            runner->mon.phase = 4;
                            Sprite_SetAnimCtrlSeq(PokeathlonCourseSpritePoolSprite_GetSprite(runner->mon.boostEffect), 0);
                            PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), runner->lane, 1, 1);
                        }
                        runner->mon.jumpQueued = 0;
                        runner->mon.queuedJumps = 0;
                    }
                }
            }
        }
        if (runner->finished != TRUE && runner->distance >= 0x1000) {
            runner->finished = TRUE;
            runner->time = hurdleDash->frame;
            PlaySEOnPlayer(SEQ_SE_GS_MYGOAL, gPokeathlonHurdleDashLaneSoundPlayers[runner->lane]);
            hurdleDash->rankTimer = 30;
        }
    }
    PokeathlonHurdleDashRunner_DrawFinishLine(hurdleDash->bgConfig, runner);
}

BOOL PokeathlonHurdleDash_UpdateRunners(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash) {
    BOOL allFinished = FALSE;
    s32 touchedLane = -1;
    PokeathlonHurdleDashTouch touch;
    u8 finished;
    s32 i;
    PokeathlonHurdleDashRunner *runner;

    touch.lane = -1;
    touch.unk4 = allFinished;
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), 3, 0, 1);
        touchedLane = PokeathlonHurdleDash_GetTouchedLane(hurdleDash, gSystem.touchX, gSystem.touchY);
        if (touchedLane < 3) {
            touch.lane = touchedLane;
        }
    }
    finished = 0;
    i = 0;
    runner = hurdleDash->runners;
    for (; i < 3; i++) {
        PokeathlonHurdleDashTouch *runnerTouch = NULL;
        if (runner->finished == FALSE) {
            if (touchedLane == i) {
                runnerTouch = &touch;
            }
        } else {
            finished++;
        }
        PokeathlonHurdleDash_UpdateRunner(course, hurdleDash, runner, runnerTouch);
        runner++;
    }
    G2_SetBG0Offset(0, hurdleDash->runners[0].mainScrollY);
    G2_SetBG1Offset(0, hurdleDash->runners[1].mainScrollY);
    G2_SetBG2Offset(0, hurdleDash->runners[2].mainScrollY);
    G2S_SetBG0Offset(0, hurdleDash->runners[0].subScrollY);
    G2S_SetBG1Offset(0, hurdleDash->runners[1].subScrollY);
    G2S_SetBG2Offset(0, hurdleDash->runners[2].subScrollY);
    if (finished >= 3) {
        allFinished = TRUE;
    }
    return allFinished;
}

void PokeathlonHurdleDashMon_StartJump(PokeathlonCourseData *course, u32 unused, u32 lane, PokeathlonHurdleDashMon *mon) {
    if (mon->state == 0) {
        if (mon->jumpStat != 0) {
            PlaySEOnPlayer(SEQ_SE_DP_DANSA, gPokeathlonHurdleDashLaneSoundPlayers[lane]);
            PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), lane, 2, 1);
        } else {
            PlaySEOnPlayer(SEQ_SE_GS_MOGURU, gPokeathlonHurdleDashLaneSoundPlayers[lane]);
        }
        SetPlayerPan(gPokeathlonHurdleDashLaneSoundPlayers[lane], gPokeathlonHurdleDashLanePans[lane]);
        mon->state = 1;
        mon->timer = 0;
        mon->phase = 1;
        PokeathlonCourseMonObject_GetSpritePosition(mon->monObject, &mon->x, &mon->y);
    }
}

// The third argument (the runner's speed) is not used.
#pragma force_active on

void PokeathlonHurdleDashMon_Update(int lane, PokeathlonHurdleDashMon *mon, f32 *speed) {
    switch (mon->state) {
    case 1:
        mon->timer++;
        switch (mon->phase) {
        case 1: {
            int height = sHurdleDashRisingJumpHeights[mon->jumpStat][mon->timer - 1];
            int y = mon->y - height;
            if (mon->jumpStat == 0) {
                PokeathlonHurdleDashMon_ScaleForHop(mon, FALSE);
                PokeathlonCourseMonObject_SetMonPosition(mon->monObject, mon->x, y);
            } else {
                PokeathlonHurdleDashMon_ScaleForHeight(mon, height);
                PokeathlonCourseMonObject_SetPosition(mon->monObject, mon->x, y, FALSE);
            }
            if (mon->timer >= mon->riseFrames) {
                mon->phase = 2;
                mon->timer = 0;
                if (mon->jumpStat == 0) {
                    PokeathlonCourseMonObject_SetMonDrawFlag(mon->monObject, FALSE);
                }
            }
            break;
        }
        case 2:
            if (mon->timer >= mon->hangFrames) {
                mon->phase = 3;
                mon->timer = 0;
                if (mon->jumpStat == 0) {
                    PokeathlonCourseMonObject_SetMonDrawFlag(mon->monObject, TRUE);
                    PlaySEOnPlayer(SEQ_SE_GS_MOGURU, gPokeathlonHurdleDashLaneSoundPlayers[lane]);
                    SetPlayerPan(gPokeathlonHurdleDashLaneSoundPlayers[lane], gPokeathlonHurdleDashLanePans[lane]);
                }
            }
            break;
        case 3: {
            int height = sHurdleDashFallingJumpHeights[mon->jumpStat][mon->timer - 1];
            int y = mon->y - height;
            if (mon->jumpStat == 0) {
                PokeathlonHurdleDashMon_ScaleForHop(mon, TRUE);
                PokeathlonCourseMonObject_SetMonPosition(mon->monObject, mon->x, y);
            } else {
                PokeathlonHurdleDashMon_ScaleForHeight(mon, height);
                PokeathlonCourseMonObject_SetPosition(mon->monObject, mon->x, y, FALSE);
            }
            if (mon->timer >= mon->riseFrames) {
                mon->state = 0;
                mon->phase = 0;
                mon->timer = 0;
                PokeathlonCourseMonObject_SetPosition(mon->monObject, mon->x, mon->y, FALSE);
                if (mon->jumpStat != 0) {
                    PlaySEOnPlayer(SEQ_SE_GS_SUTYA, gPokeathlonHurdleDashLaneSoundPlayers[lane]);
                    SetPlayerPan(gPokeathlonHurdleDashLaneSoundPlayers[lane], gPokeathlonHurdleDashLanePans[lane]);
                }
                PokeathlonHurdleDashMon_ScaleForHeight(mon, 0);
                mon->jumpQueued = 0;
                mon->queuedJumps = 0;
            } else if (mon->timer < mon->riseFrames - 5) {
                mon->jumpQueued = 0;
            }
            break;
        }
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 2:
        mon->timer++;
        if (mon->spinTimer == 0) {
            switch (mon->direction) {
            case 1:
                mon->direction = 3;
                break;
            case 3:
                mon->direction = 2;
                break;
            case 2:
                mon->direction = 4;
                break;
            case 4:
                mon->direction = 1;
                break;
            default:
                break;
            }
            PokeathlonCourseMonObject_SetAnimState(mon->monObject, mon->direction);
        }
        mon->spinTimer = (mon->spinTimer + 1) % 4;
        if (mon->timer > 30) {
            mon->timer = 0;
            mon->state = 0;
            mon->phase = 0;
            PokeathlonCourseMonObject_SetAnimState(mon->monObject, 1);
        }
        break;
    case 3: {
        BOOL moved = FALSE;
        int delta;
        if (mon->groundY < 120) {
            delta = 1;
            if (mon->groundY + 1 > 120) {
                delta = mon->groundY - 120;
            }
            moved = TRUE;
        } else if (mon->groundY > 120) {
            delta = -1;
            if (mon->groundY - 1 < 120) {
                delta = 120 - mon->groundY;
            }
            moved = TRUE;
        }
        if (moved) {
            mon->groundY += delta;
            mon->markerY += delta;
            PokeathlonCourseMonObject_Move(mon->monObject, 0, delta, 1, TRUE);
        }
        break;
    }
    case 0:
        mon->jumpQueued = 0;
        mon->queuedJumps = 0;
        break;
    }
    {
        Sprite *sprite = PokeathlonCourseSpritePoolSprite_GetSprite(mon->jumpMarker);
        VecFx32 position = *Sprite_GetMatrixPtr(sprite);
        position.y = mon->markerY << FX32_SHIFT;
        Sprite_SetMatrix(sprite, &position);
    }
    {
        int x;
        int y;
        Sprite *sprite;
        VecFx32 position;
        PokeathlonCourseMonObject_GetSpritePosition(mon->monObject, &x, &y);
        sprite = PokeathlonCourseSpritePoolSprite_GetSprite(mon->boostEffect);
        position = *Sprite_GetMatrixPtr(sprite);
        position.y = y << FX32_SHIFT;
        Sprite_SetMatrix(sprite, &position);
    }
}

#pragma force_active reset

// Whether a Pokemon at y with the given collision radius touches the hurdle.
BOOL PokeathlonHurdleDash_IsTouchingHurdle(s32 y, s32 radius, const PokeathlonHurdleDashRect *rect) {
    s32 bottom = y + radius;
    s32 top = y - radius;
    BOOL touching = FALSE;
    if (top <= rect->y + rect->height && rect->y <= bottom) {
        touching = TRUE;
    }
    return touching;
}

// Whether a jump started with the Pokemon at y is timed for the hurdle: up to 24 pixels after it.
BOOL PokeathlonHurdleDash_IsJumpTimed(s32 y, const PokeathlonHurdleDashRect *rect) {
    BOOL timed = FALSE;
    if (y >= rect->y + rect->height && y - 24 < rect->y + rect->height) {
        timed = TRUE;
    }
    return timed;
}

#pragma opt_propagation off

// Draws the finish line on the runner's two BGs when it comes into view and scrolls it away after.
#pragma force_active on

void PokeathlonHurdleDashRunner_DrawFinishLine(BgConfig *bgConfig, PokeathlonHurdleDashRunner *runner) {
    switch (runner->finishLineState) {
    case 0: {
        u16 palette;
        u16 edge;
        if (runner->distance < 112) {
            return;
        }
        palette = 2;
        edge = (palette << 9) + palette;
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, 2, (u8)(10 * runner->lane + 2), 50, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, 3, (u8)(10 * runner->lane + 3), 50, 6, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, edge, (u8)(10 * runner->lane + 9), 50, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, 2, (u8)(10 * runner->lane + 2), 50, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, 3, (u8)(10 * runner->lane + 3), 50, 6, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, edge, (u8)(10 * runner->lane + 9), 50, 1, 2, 16);
        ScheduleBgTilemapBufferTransfer(bgConfig, (u8)runner->mainBg);
        ScheduleBgTilemapBufferTransfer(bgConfig, (u8)runner->subBg);
        runner->finishLineState = 1;
        runner->unk1A = 255;
        break;
    }
    case 1: {
        int scrolled = (int)runner->distance - 16;
        int y;
        if (scrolled < 3656) {
            return;
        }
        y = (64 - ((scrolled / 8) % 64)) % 64 - 2;
        if (y < 0) {
            y += 64;
        }
        runner->finishLineY = y;
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, 96, (u8)(10 * runner->lane + 2), runner->finishLineY, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, 97, (u8)(10 * runner->lane + 3), runner->finishLineY, 6, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, 98, (u8)(10 * runner->lane + 9), runner->finishLineY, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, 96, (u8)(10 * runner->lane + 2), runner->finishLineY, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, 97, (u8)(10 * runner->lane + 3), runner->finishLineY, 6, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, 98, (u8)(10 * runner->lane + 9), runner->finishLineY, 1, 2, 16);
        ScheduleBgTilemapBufferTransfer(bgConfig, (u8)runner->mainBg);
        ScheduleBgTilemapBufferTransfer(bgConfig, (u8)runner->subBg);
        runner->finishLineState = 2;
        break;
    }
    case 2: {
        int scrolled = (int)runner->distance - 16;
        int y;
        u16 edge;
        if (scrolled < 4168) {
            return;
        }
        y = (64 - ((scrolled / 8) % 64)) % 64 - 2;
        if (y < 0) {
            y += 64;
        }
        runner->finishLineY = y;
        {
            u16 palette = 2;
            edge = (palette << 9) + palette;
        }
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, 2, (u8)(10 * runner->lane + 2), runner->finishLineY, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, 3, (u8)(10 * runner->lane + 3), runner->finishLineY, 6, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->mainBg, edge, (u8)(10 * runner->lane + 9), runner->finishLineY, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, 2, (u8)(10 * runner->lane + 2), runner->finishLineY, 1, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, 3, (u8)(10 * runner->lane + 3), runner->finishLineY, 6, 2, 16);
        FillBgTilemapRect(bgConfig, (u8)runner->subBg, edge, (u8)(10 * runner->lane + 9), runner->finishLineY, 1, 2, 16);
        ScheduleBgTilemapBufferTransfer(bgConfig, (u8)runner->mainBg);
        ScheduleBgTilemapBufferTransfer(bgConfig, (u8)runner->subBg);
        runner->finishLineState = 3;
        break;
    }
    }
}

#pragma force_active reset

#pragma opt_propagation reset

// Sends the own Pokemon's progress; the host also gathers everyone's, ranks the 12 Pokemon and tells
// whether every participant is done.
#pragma force_active on

void PokeathlonHurdleDash_SyncRace(PokeathlonCourseData *course) {
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonCoursePacket *packets = (PokeathlonCoursePacket *)PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonHurdleDashPacket *packet = (PokeathlonHurdleDashPacket *)PokeathlonCourse_ToBytes(packets);
    int i;
    for (i = 0; i < 3; i++) {
        packet->distances[i] = hurdleDash->runners[i].distance;
        if (hurdleDash->runners[i].finished && packet->finished[i] == 0) {
            packet->finished[i] = 1;
            packet->finishTimes[i] = hurdleDash->frame;
        }
    }
    if (!PokeathlonCourse_GetNetId(course)) {
        u8 done = 0;
        PokeathlonHurdleDashStatus *status = (PokeathlonHurdleDashStatus *)PokeathlonCourse_ToBytes(&packets[1]);
        PokeathlonHurdleDashPacket *hostPacket = (PokeathlonHurdleDashPacket *)PokeathlonCourse_ToBytes(&packets[2]);
        int j;
        *hostPacket = *(PokeathlonHurdleDashPacket *)PokeathlonCourse_ToBytes(&packets[0]);
        for (i = 0, packets += 2; i < 4; packets++, i++) {
            packet = (PokeathlonHurdleDashPacket *)PokeathlonCourse_ToBytes(packets);
            for (j = 0; j < 3; j++) {
                status->distances[i][j] = packet->distances[j];
            }
            if (packet->done) {
                done++;
            }
        }
        if (done == PokeathlonCourse_GetParticipantCount(course)) {
            status->allDone = 1;
        }
        PokeathlonHurdleDash_CalcRanks(course);
    }
    PokeathlonHurdleDash_UpdateRanks(course);
}

#pragma force_active reset

// Orders the standings bar by rank (ties in participant order) and shows each Pokemon's rank.
#pragma force_active on

void PokeathlonHurdleDash_UpdateStandings(PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashStatus *status) {
    PokeathlonHurdleDashStandingsNode nodes[13];
    int i;
    int j;
    PokeathlonHurdleDashStandingsNode *node;
    PokeathlonHurdleDashStandingsNode *cur;
    PokeathlonHurdleDashStandingsNode *p;
    u8 rank;
    i = 0;
    nodes[0].next = &nodes[1];
    nodes[0].prev = NULL;
    nodes[0].rank = 0;
    nodes[0].index = 0;
    for (; i < 12; i++) {
        node = &nodes[i + 1];
        node->rank = (u8)((status->ranks[(u8)(i / 2)] >> ((u8)(i % 2) * 4)) & 0xF);
        node->index = i;
        node->next = NULL;
        cur = &nodes[0];
        for (j = 0; j < i; j++) {
            cur = cur->next;
            if (cur->rank > node->rank) {
                cur->prev->next = node;
                node->prev = cur->prev;
                node->next = cur;
                cur->prev = node;
                break;
            } else if (cur->rank == node->rank && cur->index > node->index) {
                cur->prev->next = node;
                node->prev = cur->prev;
                node->next = cur;
                cur->prev = node;
                break;
            }
        }
        if (j == i) {
            cur->next = node;
            node->prev = cur;
        }
    }
    PokeathlonHurdleDashStandings_SetDrawFlag(hurdleDash->standings, TRUE);
    p = &nodes[0];
    for (i = 0; i < 12; i++) {
        p = p->next;
        rank = p->rank;
        if (rank > 8) {
            rank = 8;
        }
        PokeathlonHurdleDashStandings_SetMonX(hurdleDash->standings, p->index, i * 32 + 16);
        PokeathlonHurdleDashStandings_SetRank(hurdleDash->standings, p->index, rank);
        PokeathlonHurdleDashStandings_SetMonDrawFlag(hurdleDash->standings, p->index, TRUE);
    }
}

#pragma force_active reset

// The rank badges over the own Pokemon (pop up at 1.5 times their size whenever the rank changes) and
// the standings bar, every 30 frames once the race has run for 150; the race timer.
#pragma force_active on

void PokeathlonHurdleDash_UpdateRanks(PokeathlonCourseData *course) {
    PokeathlonHurdleDashStatus *status;
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    status = (PokeathlonHurdleDashStatus *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 6 * sizeof(PokeathlonCoursePacket));
    if (hurdleDash->started) {
        hurdleDash->rankTimer++;
    }
    if (!hurdleDash->ranksShown) {
        if (hurdleDash->rankTimer >= 150) {
            u8 i;
            hurdleDash->rankTimer = 0;
            hurdleDash->ranksShown = TRUE;
            PokeathlonHurdleDash_UpdateStandings(hurdleDash, status);
            for (i = 0; i < 3; i++) {
                Sprite *sprite = PokeathlonCourseSpritePoolSprite_GetSprite(hurdleDash->runners[i].mon.rankSprite);
                u8 byte;
                u8 rank;
                VecFx32 scale;
                PokeathlonCourseSpritePoolSprite_Show(hurdleDash->runners[i].mon.rankSprite, TRUE, TRUE);
                byte = (PokeathlonCourse_GetNetId(course) * 3 + i) / 2;
                rank = (PokeathlonCourse_GetNetId(course) * 3 + i) % 2;
                rank = (u8)((status->ranks[byte] >> (rank * 4)) & 0xF);
                GF_ASSERT(rank > 0 && rank <= 12);
                hurdleDash->runners[i].rankScale = 1.5f;
                scale.x = (int)(4096.0f * hurdleDash->runners[i].rankScale);
                scale.y = (int)(4096.0f * hurdleDash->runners[i].rankScale);
                scale.z = (int)(4096.0f * hurdleDash->runners[i].rankScale);
                hurdleDash->runners[i].rank = rank;
                Sprite_SetAffineScale(sprite, &scale);
                Sprite_SetAnimCtrlSeq(sprite, rank - 1);
            }
        }
    } else if (hurdleDash->rankTimer >= 30) {
        u8 i;
        hurdleDash->rankTimer = 0;
        PokeathlonHurdleDash_UpdateStandings(hurdleDash, status);
        for (i = 0; i < 3; i++) {
            Sprite *sprite = PokeathlonCourseSpritePoolSprite_GetSprite(hurdleDash->runners[i].mon.rankSprite);
            u8 byte;
            u8 rank;
            byte = (PokeathlonCourse_GetNetId(course) * 3 + i) / 2;
            rank = (PokeathlonCourse_GetNetId(course) * 3 + i) % 2;
            rank = (u8)((status->ranks[byte] >> (rank * 4)) & 0xF);
            GF_ASSERT(rank > 0 && rank <= 12);
            if (hurdleDash->runners[i].finished) {
                VecFx32 position = *Sprite_GetMatrixPtr(sprite);
                position.y = 0x138000;
                Sprite_SetMatrix(sprite, &position);
                Sprite_SetAnimCtrlSeq(sprite, rank + 11);
            }
            if (rank != hurdleDash->runners[i].rank) {
                hurdleDash->runners[i].rank = rank;
                if (!hurdleDash->runners[i].finished) {
                    VecFx32 scale;
                    hurdleDash->runners[i].rankScale = 1.5f;
                    scale.x = (int)(4096.0f * hurdleDash->runners[i].rankScale);
                    scale.y = (int)(4096.0f * hurdleDash->runners[i].rankScale);
                    scale.z = (int)(4096.0f * hurdleDash->runners[i].rankScale);
                    Sprite_SetAffineScale(sprite, &scale);
                    Sprite_SetAnimCtrlSeq(sprite, rank - 1);
                }
            }
        }
    }
    {
        // The badges shrink back to their size. (shifted: hurdleDash moved by i runners, so that
        // shifted->runners[0] is runner i; this is how the original addresses them.)
        PokeathlonHurdleDash *shifted;
        u8 i;
        for (i = 0; i < 3; i++) {
            shifted = (PokeathlonHurdleDash *)((u8 *)hurdleDash + i * sizeof(PokeathlonHurdleDashRunner));
            Sprite *sprite = PokeathlonCourseSpritePoolSprite_GetSprite(shifted->runners[0].mon.rankSprite);
            VecFx32 scale;
            if (Sprite_GetScalePtr(sprite)->x > FX32_ONE) {
                shifted->runners[0].rankScale = (float)((double)shifted->runners[0].rankScale - 0.1);
            } else {
                shifted->runners[0].rankScale = 1.0f;
            }
            scale.x = (int)(4096.0f * shifted->runners[0].rankScale);
            scale.y = (int)(4096.0f * shifted->runners[0].rankScale);
            scale.z = (int)(4096.0f * shifted->runners[0].rankScale);
            Sprite_SetAffineScale(sprite, &scale);
        }
    }
    PokeathlonHurdleDash_UpdateTimerDigits(hurdleDash);
}

#pragma force_active reset

// Ranks the 12 Pokemon (host): finished ones by finish time, the others by distance; equal ones share a rank.
void PokeathlonHurdleDash_CalcRanks(PokeathlonCourseData *course) {
    PokeathlonHurdleDashStatus *status;
    s32 participant;
    s32 member;
    u8 *block;
    s32 base;
    PokeathlonHurdleDashPacket *packet;
    PokeathlonHurdleDashRankNode head;
    PokeathlonHurdleDashRankNode nodes[12];
    block = PokeathlonCourse_GetDataCopyArea(course);
    status = (PokeathlonHurdleDashStatus *)PokeathlonCourse_ToBytes(block + sizeof(PokeathlonCoursePacket));
    participant = 0;
    head.prev = NULL;
    head.next = &nodes[0];
    head.distance = 0;
    head.index = 0;
    head.finishTime = 0;
    head.finished = 0;
    head.tied = 0;
    block += 2 * sizeof(PokeathlonCoursePacket);
    base = participant;
    for (; participant < 4; participant++) {
        packet = (PokeathlonHurdleDashPacket *)PokeathlonCourse_ToBytes(block);
        for (member = 0; member < 3; member++) {
            PokeathlonHurdleDashRankNode *node;
            PokeathlonHurdleDashRankNode *at = &head;
            s32 index = member + base;
            node = (PokeathlonHurdleDashRankNode *)((u8 *)&nodes[-1] + (index + 1) * sizeof(PokeathlonHurdleDashRankNode));
            s32 j;
            node->distance = packet->distances[0];
            node->finishTime = packet->finishTimes[0];
            node->index = index;
            node->finished = packet->finished[0];
            node->tied = 0;
            j = 0;
            node->next = (void *)j;
            if (index > 0) {
                do {
                    BOOL inserted;
                    at = at->next;
                    if (node->finished != 0) {
                        inserted = PokeathlonHurdleDashRankNode_InsertFinished(node, at);
                    } else {
                        inserted = PokeathlonHurdleDashRankNode_InsertRunning(node, at);
                    }
                    if (inserted != 0) {
                        break;
                    }
                    j++;
                } while (j < index);
            }
            if (j == index) {
                at->next = node;
                node->prev = at;
            }
            packet = (PokeathlonHurdleDashPacket *)((u16 *)packet + 1);
        }
        block += sizeof(PokeathlonCoursePacket);
        base += 3;
    }
    {
        u8 rank = 0;
        u8 step;
        PokeathlonHurdleDashRankNode *node;
        s32 i;
        node = &head;
        step = 1;
        for (i = 0; i < 6; i++) {
            status->ranks[i] = 0;
        }
        for (i = 0; i < 12; i++) {
            node = node->next;
            if (node == NULL) {
                GF_AssertFail();
            }
            if (node->tied == 0) {
                rank += step;
                step = 1;
            } else {
                step++;
            }
            PokeathlonHurdleDash_SetRank(status, (u8)node->index, rank);
        }
    }
}

// Inserts a Pokemon that has finished before the node at, if it ranks before it.
BOOL PokeathlonHurdleDashRankNode_InsertFinished(PokeathlonHurdleDashRankNode *node, PokeathlonHurdleDashRankNode *at) {
    if (at->finished != 0) {
        if (at->finishTime > node->finishTime) {
            LIST_INSERT_BEFORE(node, at);
            return TRUE;
        } else if (at->finishTime == node->finishTime) {
            if (at->index > node->index) {
                LIST_INSERT_BEFORE(node, at);
                at->tied = 1;
                return TRUE;
            } else {
                node->tied = 1;
            }
        }
    } else {
        LIST_INSERT_BEFORE(node, at);
        return TRUE;
    }
    return FALSE;
}

// Inserts a Pokemon that is still running before the node at, if it ranks before it.
BOOL PokeathlonHurdleDashRankNode_InsertRunning(PokeathlonHurdleDashRankNode *node, PokeathlonHurdleDashRankNode *at) {
    if (at->distance < node->distance) {
        LIST_INSERT_BEFORE(node, at);
        return TRUE;
    } else if (at->distance == node->distance) {
        if (at->finishTime > node->finishTime) {
            LIST_INSERT_BEFORE(node, at);
            return TRUE;
        } else if (at->finishTime == node->finishTime) {
            if (at->index > node->index) {
                LIST_INSERT_BEFORE(node, at);
                at->tied = 1;
                return TRUE;
            } else {
                node->tied = 1;
            }
        }
    }
    return FALSE;
}

#pragma opt_lifetimes off

#pragma force_active on

void PokeathlonHurdleDash_SetRank(PokeathlonHurdleDashStatus *status, int index, u32 rank) {
    u8 shift;
    u8 byte;
    u8 bits;
    u8 *ranks;
    if ((u32)index >= 12) {
        GF_AssertFail();
    }
    if (rank == 0 || rank > 12) {
        GF_AssertFail();
    }
    shift = (u8)(index % 2);
    byte = (u8)(index >> 1);
    bits = (u8)(rank << (shift * 4));
    ranks = status->ranks;
    ranks[byte] |= bits;
}

#pragma force_active reset

#pragma opt_lifetimes reset

BOOL PokeathlonHurdleDash_HaveAllStopped(PokeathlonHurdleDash *hurdleDash) {
    s32 i;
    BOOL stopped = FALSE;
    for (i = 0; i < 3; i++) {
        if (hurdleDash->runners[i].mon.state != 4) {
            break;
        }
    }
    if (i == 3) {
        stopped = TRUE;
    }
    return stopped;
}

// The host runs the Pokemon of the participants without a player.
#pragma force_active on

void PokeathlonHurdleDash_UpdateCpuParticipants(PokeathlonCourseData *course) {
    u8 *block;
    PokeathlonHurdleDash *hurdleDash;
    int count;
    int participant;
    int member;
    int cpuIndex;
    PokeathlonHurdleDashPacket *packet;
    u8 *packetBlock;
    u8 participantId;
    if (PokeathlonCourse_GetNetId(course) == 0) {
        block = PokeathlonCourse_GetDataCopyArea(course);
        hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
        count = PokeathlonCourse_GetParticipantCount(course);
        participant = count;
        if (participant < 4) {
            packetBlock = block + 2 * sizeof(PokeathlonCoursePacket) + participant * sizeof(PokeathlonCoursePacket);
            do {
                packet = (PokeathlonHurdleDashPacket *)PokeathlonCourse_ToBytes(packetBlock);
                member = 0;
                cpuIndex = (participant - count) * 3;
                participantId = (u8)participant;
                do {
                    PokeathlonHurdleDash_UpdateCpuRunner(course, hurdleDash, packet, participantId, (u8)member, (u8)cpuIndex);
                    member++;
                    cpuIndex++;
                } while (member < 3);
                packetBlock += sizeof(PokeathlonCoursePacket);
                participant++;
            } while (participant < 4);
        }
    }
}

void PokeathlonHurdleDash_RevealHurdles(PokeathlonHurdleDash *hurdleDash, int distance, int lane) {
    PokeathlonHurdleDashTrack_RevealHurdles(hurdleDash->track, distance + 0x150, lane);
}

#pragma force_active reset

// Kicks up dust behind a running Pokemon, more often the faster it is boosted.
void PokeathlonHurdleDashRunner_UpdateDust(PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashRunner *runner) {
    PokeathlonHurdleDashLaneX laneX = sHurdleDashDustLaneX;
    s32 interval;
    if (runner->mon.state != 1 && runner->finished == FALSE) {
        if (runner->mon.state == 2) {
            interval = 24;
        } else {
            switch (runner->boostLevel) {
            case 0:
                interval = 8;
                break;
            case 1:
                interval = 6;
                break;
            case 2:
                interval = 6;
                break;
            case 3:
                interval = 6;
                break;
            case 4:
                interval = 4;
                break;
            default:
                GF_AssertFail();
                interval = 8;
                break;
            }
        }
        runner->dustTimer++;
        if (runner->dustTimer >= interval) {
            VecFx32 position = { 0, 0, 0 };
            runner->dustTimer = 0;
            position.x = (laneX.x[runner->lane] + (LCRandom() % 7 - 3)) << FX32_SHIFT;
            position.y = 0x188000;
            PokeathlonHurdleDashDust_Spawn(hurdleDash->dust, runner->lane, &position);
        }
    }
}

// The lane touched at x, y; 3 for none.
u8 PokeathlonHurdleDash_GetTouchedLane(PokeathlonHurdleDash *hurdleDash, s32 x, s32 y) {
    PokeathlonHurdleDashLaneX laneX = sHurdleDashTouchLaneX;
    u8 i;
    for (i = 0; i < 3; i++) {
        s32 left = laneX.x[i] - 24;
        if (left < x && x < left + 48 && y > 88 && y < 168) {
            return i;
        }
    }
    return 3;
}

void PokeathlonHurdleDash_InitTimeWindow(PokeathlonHurdleDash *hurdleDash) {
    AddWindow(hurdleDash->bgConfig, &hurdleDash->window, &sHurdleDashTimeWindowTemplate);
    BG_FillCharDataRange(hurdleDash->bgConfig, GF_BG_LYR_MAIN_3, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, hurdleDash->heapId);
    FillWindowPixelBuffer(&hurdleDash->window, 0);
}

// Prints the team's time (frames) as seconds and tenths.
void PokeathlonHurdleDash_PrintTime(PokeathlonHurdleDash *hurdleDash, s32 frames) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, hurdleDash->heapId);
    MessageFormat *messageFormat = MessageFormat_New(hurdleDash->heapId);
    String *string;
    BufferIntegerAsString(messageFormat, 0, frames / 30, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferIntegerAsString(messageFormat, 1, 10 * (frames % 30) / 30, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msg_0309_00156, hurdleDash->heapId);
    AddTextPrinterParameterizedWithColor(&hurdleDash->window, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&hurdleDash->window);
}

// A Pokemon with a jump stat stretches with the height of its jump.
void PokeathlonHurdleDashMon_ScaleForHeight(PokeathlonHurdleDashMon *mon, s32 height) {
    f32 scale;
    if (height < 0) {
        height = 0;
    }
    scale = (f32)((100.0 + height * 2.0) / 100.0);
    PokeathlonCourseObject_SetScale(mon->monObject, scale, scale);
}

// A Pokemon without a jump stat shrinks while it hops and grows back while it lands.
void PokeathlonHurdleDashMon_ScaleForHop(PokeathlonHurdleDashMon *mon, BOOL landing) {
    f32 scale;
    if (landing != FALSE) {
        scale = (f32)(100.0 - 10.0 * (mon->riseFrames - (u8)(mon->timer - 1)));
    } else {
        scale = (f32)(100.0 - 10.0 * (u8)(mon->timer - 1));
        if (scale < 0.0f) {
            scale = 10.0f;
        }
    }
    scale = (f32)(scale / 100.0);
    PokeathlonCourseObject_SetScale(mon->monObject, scale, scale);
}

static inline void FramesToSeconds(u16 frames, u8 *seconds, u8 *hundredths) {
    *seconds = (u8)(frames / 30);
    *hundredths = (u8)(_s32_div_f((u8)(frames % 30) * 100, 30));
}

// The race timer: minutes, seconds and tenths.
#pragma force_active on

void PokeathlonHurdleDash_UpdateTimerDigits(PokeathlonHurdleDash *hurdleDash) {
    u16 frames = (u16)hurdleDash->raceTime;
    u8 seconds;
    u8 hundredths;
    FramesToSeconds(frames, &seconds, &hundredths);
    Sprite_SetAnimCtrlSeq(hurdleDash->sprites[0], (u8)(seconds / 60) + 1);
    seconds = (u8)(seconds % 60);
    Sprite_SetAnimCtrlSeq(hurdleDash->sprites[2], (u8)(seconds / 10) + 1);
    Sprite_SetAnimCtrlSeq(hurdleDash->sprites[3], (u8)(seconds % 10) + 1);
    Sprite_SetAnimCtrlSeq(hurdleDash->sprites[5], (u8)(hundredths / 10) + 1);
}

#pragma force_active reset

// A Pokemon of a participant without a player (host): it accelerates to its top speed; at each hurdle it
// clears it with a boost (boostChance percent), clears it (up to clearChance) or trips.
#pragma force_active on

void PokeathlonHurdleDash_UpdateCpuRunner(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashPacket *packet, u8 participant, int member, u8 cpuIndex) {
    PokeathlonHurdleDashCpuRunner *runner = &hurdleDash->cpuRunners[cpuIndex];
    int hurdle;
    int pixels;
    f32 distance;
    if (runner->finished != TRUE && runner->state != 2 && runner->boostLevel == 0) {
        if (runner->speed < runner->topSpeed) {
            runner->speed += runner->acceleration;
            if (runner->speed > runner->topSpeed) {
                runner->speed = runner->topSpeed;
            }
        }
    }
    if (runner->state == 2) {
        runner->stunTimer--;
        if (runner->stunTimer == 0) {
            runner->state = 0;
        }
    } else {
        hurdle = PokeathlonHurdleDashTrack_GetHurdleAt(hurdleDash->track, member, runner->distance);
        if (hurdle != -1 && runner->lastHurdle != hurdle) {
            u8 random = LCRandom() % 100;
            if (random < runner->boostChance) {
                if (runner->boostLevel < 4) {
                    runner->boostLevel++;
                    runner->speed = runner->topSpeed + sHurdleDashBoostSpeeds[runner->boostLevel - 1];
                }
                PokeathlonCourse_AddMonCounter(course, participant, member, 6, 1);
                PokeathlonCourse_AddMonCounter(course, participant, member, 2, 1);
            } else if (random < runner->clearChance) {
                runner->state = 2;
                runner->stunTimer = 30;
                runner->boostLevel = 0;
                runner->speed = 0.5f;
                PokeathlonCourse_AddMonCounter(course, participant, member, 1, 1);
            } else {
                PokeathlonCourse_AddMonCounter(course, participant, member, 2, 1);
            }
            runner->lastHurdle = hurdle;
        }
    }
    distance = runner->speed + runner->speedFraction;
    pixels = distance;
    runner->speedFraction = distance - pixels;
    runner->distance += pixels;
    packet->distances[member] = runner->distance;
    if (runner->finished == FALSE && runner->distance >= 0x1000) {
        packet->finished[member] = 1;
        packet->finishTimes[member] = hurdleDash->frame;
        runner->time = hurdleDash->frame;
        runner->finished = TRUE;
        PlaySE(SEQ_SE_GS_NM12);
    }
}

#pragma force_active reset

// Top speed and acceleration from the Pokemon's stats, and a random hurdle skill.
#pragma force_active on

void PokeathlonHurdleDash_InitCpuRunners(PokeathlonCourseData *course, const PokeathlonHurdleDashTable *table) {
    PokeathlonHurdleDash *hurdleDash = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 count = PokeathlonCourse_GetParticipantCount(course);
    u8 participant;
    u8 member;
    for (participant = count; participant < 4; participant++) {
        int base = participant - count;
        for (member = 0; member < 3; member++) {
            PokeathlonHurdleDashCpuRunner *runner = &hurdleDash->cpuRunners[(u8)(member + base * 3)];
            u8 *stats = PokeathlonCourse_GetMonStats(course, participant, member);
            runner->topSpeed = table->rows[3][stats[1]] / 10.0f;
            runner->acceleration = table->rows[2][stats[4]] / 100.0f;
            switch (LCRandom() % 6) {
            case 0:
                runner->boostChance = 10;
                runner->clearChance = 50;
                break;
            case 1:
                runner->boostChance = 20;
                runner->clearChance = 50;
                break;
            case 2:
                runner->boostChance = 30;
                runner->clearChance = 60;
                break;
            case 3:
                runner->boostChance = 20;
                runner->clearChance = 70;
                break;
            case 4:
                runner->boostChance = 30;
                runner->clearChance = 70;
                break;
            case 5:
                runner->boostChance = 40;
                runner->clearChance = 80;
                break;
            default:
                GF_AssertFail();
                break;
            }
        }
    }
}

#pragma force_active reset

// A timed jump over a hurdle: the next boost level (up to 4) and its speed.
void PokeathlonHurdleDashRunner_Boost(PokeathlonCourseData *course, PokeathlonHurdleDashRunner *runner) {
    Sprite *sprite;
    if (runner->boostLevel < 4) {
        sprite = PokeathlonCourseSpritePoolSprite_GetSprite(runner->mon.boostEffect);
        runner->boostLevel++;
        if (runner->boostLevel == 1) {
            PlaySEOnPlayer(SEQ_SE_GS_FW015_1, gPokeathlonHurdleDashLaneSoundPlayers[runner->lane]);
            SetPlayerPan(gPokeathlonHurdleDashLaneSoundPlayers[runner->lane], gPokeathlonHurdleDashLanePans[runner->lane]);
            PokeathlonCourseMonObject_SetAnimState(runner->mon.monObject, 5);
            Sprite_SetAnimCtrlSeq(sprite, 1);
        } else if (runner->boostLevel == 4) {
            PlaySEOnPlayer(SEQ_SE_GS_186, gPokeathlonHurdleDashLaneSoundPlayers[runner->lane]);
            SetPlayerPan(gPokeathlonHurdleDashLaneSoundPlayers[runner->lane], gPokeathlonHurdleDashLanePans[runner->lane]);
            PokeathlonCourseMonObject_SetAnimState(runner->mon.monObject, 6);
            Sprite_SetAnimCtrlSeq(sprite, 2);
        } else if (runner->boostLevel == 2 || runner->boostLevel == 3) {
            PlaySEOnPlayer(SEQ_SE_GS_FW015_2, gPokeathlonHurdleDashLaneSoundPlayers[runner->lane]);
            SetPlayerPan(gPokeathlonHurdleDashLaneSoundPlayers[runner->lane], gPokeathlonHurdleDashLanePans[runner->lane]);
        }
        if (runner->speed < 10.0) {
            runner->speed = runner->mon.topSpeed + sHurdleDashBoostSpeeds[runner->boostLevel - 1];
        } else {
            GF_AssertFail();
        }
    }
    PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), runner->lane, 6, 1);
}

void PokeathlonHurdleDash_CountFrame(PokeathlonHurdleDash *hurdleDash) {
    s32 i;
    hurdleDash->frame++;
    for (i = 0; i < 3; i++) {
        if (hurdleDash->runners[i].finished == FALSE) {
            hurdleDash->raceTime++;
            return;
        }
    }
}

// Rounds a time in frames down to tenths of a second.
s32 PokeathlonHurdleDash_RoundTime(s32 frames) {
    return frames / 30 * 30 + 10 * (frames % 30) / 30 * 30 / 10;
}

PokeathlonHurdle *PokeathlonHurdleDash_AllocHurdles(u32 count, enum HeapID heapId) {
    u32 size = count * sizeof(PokeathlonHurdle);
    PokeathlonHurdle *hurdles = Heap_Alloc(heapId, size);
    MIi_CpuClearFast(0, (u32 *)hurdles, size);
    return hurdles;
}

void PokeathlonHurdleDash_FreeHurdles(PokeathlonHurdle *hurdles) {
    Heap_Free(hurdles);
}

PokeathlonHurdle *PokeathlonHurdleDash_GetHurdle(PokeathlonHurdle *hurdles, int index) {
    return &hurdles[index];
}

void PokeathlonHurdle_SetSprites(PokeathlonHurdle *hurdle, void *standingSprite, void *knockedSprite) {
    hurdle->standingSprite = standingSprite;
    hurdle->knockedSprite = knockedSprite;
}

#pragma force_active on

void PokeathlonHurdle_SetActive(PokeathlonHurdle *hurdle, BOOL active) {
    if (active) {
        hurdle->visible = TRUE;
        hurdle->standing = TRUE;
        PokeathlonCourseSpritePoolSprite_Show(hurdle->standingSprite, TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_Show(hurdle->knockedSprite, TRUE, FALSE);
        Sprite_SetAnimActiveFlag(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite), FALSE);
    } else {
        hurdle->visible = FALSE;
        hurdle->standing = FALSE;
        PokeathlonCourseSpritePoolSprite_Show(hurdle->standingSprite, FALSE, FALSE);
        PokeathlonCourseSpritePoolSprite_Show(hurdle->knockedSprite, FALSE, FALSE);
    }
}

#pragma force_active reset

BOOL PokeathlonHurdle_IsVisible(PokeathlonHurdle *hurdle) {
    return hurdle->visible;
}

BOOL PokeathlonHurdle_IsStanding(PokeathlonHurdle *hurdle) {
    return hurdle->standing;
}

void PokeathlonHurdle_KnockDown(PokeathlonHurdle *hurdle) {
    Sprite *sprite;
    hurdle->standing = FALSE;
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite);
    PokeathlonCourseSpritePoolSprite_Show(hurdle->standingSprite, TRUE, FALSE);
    PokeathlonCourseSpritePoolSprite_Show(hurdle->knockedSprite, TRUE, TRUE);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 1);
}

void PokeathlonHurdle_SetPosition(PokeathlonHurdle *hurdle, VecFx32 *position) {
    Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->standingSprite), position);
    Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite), position);
}

// Moves the hurdle down with the track; it is removed once it leaves the screen.
void PokeathlonHurdle_Scroll(PokeathlonHurdle *hurdle, int distance) {
    Sprite *standing;
    Sprite *knocked;
    VecFx32 position;
    if (hurdle->visible) {
        standing = PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->standingSprite);
        knocked = PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->knockedSprite);
        position = *Sprite_GetMatrixPtr(standing);
        position.y += distance << FX32_SHIFT;
        Sprite_SetMatrix(standing, &position);
        Sprite_SetMatrix(knocked, &position);
        if (position.y >= 0x208000) {
            PokeathlonHurdle_SetActive(hurdle, FALSE);
        }
    }
}

void PokeathlonHurdle_GetRect(PokeathlonHurdle *hurdle, PokeathlonHurdleDashRect *rect) {
    const VecFx32 *position = Sprite_GetMatrixPtr(PokeathlonCourseSpritePoolSprite_GetSprite(hurdle->standingSprite));
    rect->width = 64;
    rect->height = 16;
    rect->x = (s16)(position->x / FX32_ONE - 32);
    rect->y = (s16)(position->y / FX32_ONE - 16);
}

// Places the next of the lane's four hurdles y pixels below the top of the screen.
#pragma force_active on

PokeathlonHurdle *PokeathlonHurdleDashTrack_PlaceHurdle(PokeathlonHurdleDashTrack *track, int lane, int y) {
    PokeathlonHurdle *hurdle;
    VecFx32 position;
    hurdle = PokeathlonHurdleDash_GetHurdle(track->lanes[lane].hurdles, (u8)track->lanes[lane].next);
    if (PokeathlonHurdle_IsVisible(hurdle)) {
        GF_AssertFail();
        return NULL;
    }
    track->lanes[lane].next = (track->lanes[lane].next + 1) % 4;
    position = sHurdleDashNewHurdlePos;
    position.y += y << FX32_SHIFT;
    switch (lane) {
    case 0:
        position.x = 48 * FX32_ONE;
        break;
    case 1:
        position.x = 128 * FX32_ONE;
        break;
    case 2:
        position.x = 208 * FX32_ONE;
        break;
    }
    PokeathlonHurdle_SetPosition(hurdle, &position);
    PokeathlonHurdle_SetActive(hurdle, TRUE);
    return hurdle;
}

#pragma force_active reset

PokeathlonHurdleDashTrack *PokeathlonHurdleDashTrack_New(enum HeapID heapId) {
    int i;
    PokeathlonHurdleDashLane *lane;
    PokeathlonHurdleDashTrack *track = Heap_Alloc(heapId, sizeof(PokeathlonHurdleDashTrack));
    MI_CpuFill8(track, 0, sizeof(PokeathlonHurdleDashTrack));
    lane = track->lanes;
    i = 0;
    do {
        lane->hurdles = PokeathlonHurdleDash_AllocHurdles(4, heapId);
        i++;
        lane++;
    } while (i < 3);
    return track;
}

void PokeathlonHurdleDashTrack_Free(PokeathlonHurdleDashTrack *track) {
    PokeathlonHurdleDashTrack *base;
    int i;
    PokeathlonHurdleDashLane *lane;
    base = track;
    i = 0;
    lane = base->lanes;
    do {
        PokeathlonHurdleDash_FreeHurdles(lane->hurdles);
        i++;
        lane++;
    } while (i < 3);
    Heap_Free(base);
}

void PokeathlonHurdleDashTrack_CreateHurdleSprites(PokeathlonHurdleDashLane *lane, void *spritePool) {
    int i = 0;
    int j;
    do {
        j = 0;
        do {
            PokeathlonHurdle *hurdle = PokeathlonHurdleDash_GetHurdle(lane->hurdles, j);
            void *standing = PokeathlonCourseSpritePool_Take(spritePool, 101, 2);
            void *knocked = PokeathlonCourseSpritePool_Take(spritePool, 103, 6);
            PokeathlonHurdle_SetSprites(hurdle, standing, knocked);
            j++;
        } while (j < 4);
        lane++;
        i++;
    } while (i < 3);
}

PokeathlonHurdle *PokeathlonHurdleDashTrack_GetLaneHurdles(PokeathlonHurdleDashTrack *track, u32 lane) {
    if (lane >= 3) {
        GF_AssertFail();
    }
    return track->lanes[lane].hurdles;
}

void *PokeathlonHurdleDashTrack_GetLayoutToSend(PokeathlonHurdleDashTrack *track) {
    return track->layoutToSend;
}

void *PokeathlonHurdleDashTrack_GetLayout(PokeathlonHurdleDashTrack *track) {
    return track->layout;
}

int PokeathlonHurdleDashTrack_GetLayoutSize(PokeathlonHurdleDashTrack *track) {
    return sizeof(track->layout);
}

// Places a hurdle once the flag of the layout at position (8 pixels each) comes into view.
#pragma force_active on

void PokeathlonHurdleDashTrack_RevealHurdles(PokeathlonHurdleDashTrack *track, int position, int lane) {
    int flag = position / 8;
    int y;
    if (flag != 0) {
        y = position - flag * 8;
        flag--;
        if (flag < 512 && track->layout[lane][flag] != 0 && track->lastHurdle[lane] < flag) {
            track->lastHurdle[lane] = flag;
            PokeathlonHurdleDashTrack_PlaceHurdle(track, lane, y);
        }
    }
}

#pragma force_active reset

// The host picks one of the three hurdle layouts.
void PokeathlonHurdleDashTrack_LoadRandomLayout(PokeathlonHurdleDashTrack *track) {
    u8 layout = LCRandom() % 3;
    ReadWholeNarcMemberByIdPair(track->layoutToSend, NARC_a_2_2_6, (u8)layout);
}

// The flag of the layout at position, -1 if there is no hurdle there.
#pragma force_active on

int PokeathlonHurdleDashTrack_GetHurdleAt(PokeathlonHurdleDashTrack *track, int lane, int position) {
    position /= 8;
    if (!track->layout[lane][position]) {
        position = -1;
    }
    return position;
}

#pragma force_active reset

PokeathlonHurdleDashStandings *PokeathlonHurdleDashStandings_New(enum HeapID heapId) {
    PokeathlonHurdleDashStandings *standings = Heap_Alloc(heapId, sizeof(PokeathlonHurdleDashStandings));
    MIi_CpuClearFast(0, (u32 *)standings, sizeof(PokeathlonHurdleDashStandings));
    standings->heapId = heapId;
    return standings;
}

#pragma force_active on

void PokeathlonHurdleDashStandings_Free(PokeathlonHurdleDashStandings *standings) {
    int i;
    for (i = 0; i < 12; i++) {
        Sprite_Delete(standings->mons[i].icon);
        Sprite_Delete(standings->mons[i].frame);
        Sprite_Delete(standings->mons[i].rank);
    }
    SpriteTransfer_DeleteCharTransferTask(standings->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(standings->resources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(standings->resMans[i]);
    }
    SpriteList_Delete(standings->spriteList);
    Heap_Free(standings);
}

#pragma force_active reset

void PokeathlonHurdleDashStandings_Init(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons) {
    PokeathlonHurdleDashStandings_LoadResources(standings);
    PokeathlonHurdleDashStandings_InitResourcesHeader(standings);
    PokeathlonHurdleDashStandings_CreateSprites(standings, mons);
    G2dRenderer_SetSubSurfaceCoords(&standings->renderer, 0, 0);
    G2dRenderer_SetMainSurfaceCoords(&standings->renderer, 0, 192 * FX32_ONE);
}

void PokeathlonHurdleDashStandings_SetMonX(PokeathlonHurdleDashStandings *standings, int index, int x) {
    VecFx32 position;
    position.z = 0;
    position.x = x << FX32_SHIFT;
    position.y = 14 << FX32_SHIFT;
    Sprite_SetMatrix(standings->mons[index].icon, &position);
    Sprite_SetMatrix(standings->mons[index].frame, &position);
    Sprite_SetMatrix(standings->mons[index].rank, &position);
}

#pragma force_active on

void PokeathlonHurdleDashStandings_SetRank(PokeathlonHurdleDashStandings *standings, u32 index, u32 rank) {
    GF_ASSERT(rank != 0 && rank <= 8);
    Sprite_SetAnimCtrlSeq(standings->mons[index].rank, rank + 15);
}

#pragma force_active reset

void PokeathlonHurdleDashStandings_SetDrawFlag(PokeathlonHurdleDashStandings *standings, BOOL draw) {
    int i;
    for (i = 0; i < 12; i++) {
        Sprite_SetDrawFlag(standings->mons[i].icon, draw);
        Sprite_SetDrawFlag(standings->mons[i].frame, draw);
    }
}

void PokeathlonHurdleDashStandings_SetMonDrawFlag(PokeathlonHurdleDashStandings *standings, int index, BOOL draw) {
    Sprite_SetDrawFlag(standings->mons[index].icon, draw);
    Sprite_SetDrawFlag(standings->mons[index].frame, draw);
    Sprite_SetDrawFlag(standings->mons[index].rank, draw);
}

void PokeathlonHurdleDashStandings_Draw(PokeathlonHurdleDashStandings *standings) {
    SpriteList_RenderAndAnimateSprites(standings->spriteList);
}

void PokeathlonHurdleDashStandings_LoadResources(PokeathlonHurdleDashStandings *standings) {
    int i;
    standings->spriteList = G2dRenderer_Init(36, &standings->renderer, standings->heapId);
    for (i = 0; i < 6; i++) {
        standings->resMans[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, standings->heapId);
    }
    standings->resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_1_5_6, 24, FALSE, 0x80, NNS_G2D_VRAM_TYPE_2DSUB, standings->heapId);
    standings->resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_1_5_6, 21, FALSE, 0x80, NNS_G2D_VRAM_TYPE_2DSUB, 1, standings->heapId);
    standings->resources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_1_5_6, 23, FALSE, 0x80, GF_GFX_RES_TYPE_CELL, standings->heapId);
    standings->resources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(standings->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_1_5_6, 22, FALSE, 0x80, GF_GFX_RES_TYPE_ANIM, standings->heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(standings->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreateExtPlttTransferTask(standings->resources[GF_GFX_RES_TYPE_PLTT]);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_3_OFFSET, 0, standings->heapId);
}

void PokeathlonHurdleDashStandings_InitResourcesHeader(PokeathlonHurdleDashStandings *standings) {
    CreateSpriteResourcesHeader(&standings->header, 0x80, 0x80, 0x80, 0x80, -1, -1, FALSE, 0, standings->resMans[GF_GFX_RES_TYPE_CHAR], standings->resMans[GF_GFX_RES_TYPE_PLTT], standings->resMans[GF_GFX_RES_TYPE_CELL], standings->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

// The icon (its own Pokemon icon tiles, 0x200 bytes each), team frame and rank of each Pokemon; Pokemon
// that don't exist are hidden.
#pragma force_active on

void PokeathlonHurdleDashStandings_CreateSprites(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons) {
    SpriteTemplate template;
    NNSG2dCharacterData *charData;
    u32 loadSize;
    u32 flushSize;
    NARC *narc;
    void *buffer;
    u8 form;
    u32 offset;
    int x;
    int i;
    u16 species;
    u8 palette;
    narc = NARC_New(NARC_poketool_icongra_poke_icon, standings->heapId);
    buffer = Heap_AllocAtEnd(standings->heapId, 0x1000);
    PokeathlonHurdleDashStandings_InitSpriteTemplate(&template, standings, &standings->header, NNS_G2D_VRAM_TYPE_2DSUB);
    offset = 0x200;
    x = 16;
    flushSize = offset;
    loadSize = offset;
    for (i = 0; i < 12; i++, x += 32) {
        standings->mons[i].icon = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(standings->mons[i].icon, TRUE);
        Sprite_SetAnimCtrlSeq(standings->mons[i].icon, i);
        standings->mons[i].frame = Sprite_CreateAffine(&template);
        Sprite_SetAnimCtrlSeq(standings->mons[i].frame, i / 3 + 12);
        standings->mons[i].rank = Sprite_CreateAffine(&template);
        if (i < 8) {
            Sprite_SetAnimCtrlSeq(standings->mons[i].rank, i + 16);
        }
        species = mons[i].species;
        form = mons[i].form;
        if (species == SPECIES_NONE) {
            Sprite_SetDrawFlag(standings->mons[i].icon, FALSE);
            Sprite_SetDrawFlag(standings->mons[i].frame, FALSE);
            Sprite_SetDrawFlag(standings->mons[i].rank, FALSE);
        }
        Sprite_SetDrawPriority(standings->mons[i].icon, 2);
        Sprite_SetDrawPriority(standings->mons[i].frame, 3);
        Sprite_SetDrawPriority(standings->mons[i].rank, 1);
        PokeathlonHurdleDashStandings_SetMonX(standings, i, x);
        Sprite_SetDrawFlag(standings->mons[i].rank, TRUE);
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(species, FALSE, form), buffer);
        NNS_G2dGetUnpackedBGCharacterData(buffer, &charData);
        palette = GetMonIconPaletteEx(species, form, FALSE) + 3;
        DC_FlushRange(charData->pRawData, flushSize);
        GXS_LoadOBJ(charData->pRawData, offset, loadSize);
        Sprite_SetPaletteOverride(standings->mons[i].icon, palette);
        offset += 0x200;
    }
    Heap_Free(buffer);
    NARC_Delete(narc);
}

#pragma force_active reset

void PokeathlonHurdleDashStandings_InitSpriteTemplate(SpriteTemplate *template, PokeathlonHurdleDashStandings *standings, const SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen) {
    u32 zero = 0;
    template->spriteList = standings->spriteList;
    template->header = &standings->header;
    template->position.x = 240 * FX32_ONE;
    template->position.y = 14 * FX32_ONE;
    template->position.z = zero;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = zero;
    template->drawPriority = 2;
    template->whichScreen = whichScreen;
    template->heapID = standings->heapId;
}

PokeathlonHurdleDashDustLane *PokeathlonHurdleDashDust_New(enum HeapID heapId) {
    PokeathlonHurdleDashDustLane *lanes = Heap_Alloc(heapId, 3 * sizeof(PokeathlonHurdleDashDustLane));
    MI_CpuFill8(lanes, 0, 3 * sizeof(PokeathlonHurdleDashDustLane));
    return lanes;
}

#pragma force_active on

void PokeathlonHurdleDashDust_CreateSprites(PokeathlonHurdleDashDustLane *lanes, void *spritePool, u16 priority) {
    u8 i;
    u8 j;
    for (i = 0; i < 3; i++) {
        PokeathlonHurdleDashDustLane *lane = &lanes[i];
        for (j = 0; j < 5; j++) {
            PokeathlonHurdleDashDust *dust = &lane->dust[j];
            dust->sprite = PokeathlonCourseSpritePool_CreateAndTake(spritePool, priority, NNS_G2D_VRAM_TYPE_2DMAIN, 106, 10);
            PokeathlonCourseSpritePoolSprite_Show(dust->sprite, TRUE, FALSE);
            PokeathlonCourseSpritePoolSprite_SetDrawPriority(dust->sprite, 5);
        }
    }
}

#pragma force_active reset

void PokeathlonHurdleDashDust_Free(PokeathlonHurdleDashDustLane *lanes) {
    Heap_Free(lanes);
}

void PokeathlonHurdleDashDust_Spawn(PokeathlonHurdleDashDustLane *lanes, u32 lane, const VecFx32 *position) {
    PokeathlonHurdleDashDustLane *selected;
    PokeathlonHurdleDashDust *dust;
    if (lane >= 3) {
        GF_AssertFail();
    }
    selected = &lanes[lane];
    dust = &selected->dust[selected->next];
    if (dust->active == FALSE) {
        PokeathlonCourseSpritePoolSprite_Show(dust->sprite, TRUE, TRUE);
        PokeathlonCourseSpritePoolSprite_SetAnim(dust->sprite, 1);
        PokeathlonCourseSpritePoolSprite_SetPosition(dust->sprite, position);
        dust->active = TRUE;
        selected->next = (selected->next + 1) % 5;
    } else {
        GF_AssertFail();
    }
}

void PokeathlonHurdleDashDust_Scroll(PokeathlonHurdleDashDustLane *lanes, u32 lane, u32 distance) {
    PokeathlonHurdleDashDustLane *selected;
    u8 i;
    VecFx32 position;
    if (lane >= 3) {
        GF_AssertFail();
    }
    selected = &lanes[lane];
    for (i = 0; i < 5; i++) {
        PokeathlonHurdleDashDust *dust = &selected->dust[i];
        if (dust->active != FALSE) {
            position = *PokeathlonCourseSpritePoolSprite_GetPosition(dust->sprite);
            position.y += distance << FX32_SHIFT;
            PokeathlonCourseSpritePoolSprite_SetPosition(dust->sprite, &position);
            if (PokeathlonCourseSpritePoolSprite_IsAnimated(dust->sprite) == FALSE) {
                PokeathlonHurdleDashDust_Remove(dust);
            }
        }
    }
}

void PokeathlonHurdleDashDust_Remove(PokeathlonHurdleDashDust *dust) {
    dust->active = FALSE;
    PokeathlonCourseSpritePoolSprite_Show(dust->sprite, TRUE, FALSE);
}
