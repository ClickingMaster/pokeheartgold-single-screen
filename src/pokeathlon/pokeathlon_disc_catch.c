#include "pokeathlon/pokeathlon_disc_catch.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "geometry_math.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "sprite_resource_header.h"
#include "wireless_status_icon.h"

// Course helpers (OVY_96, not yet tidied).
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, int timer);
void PokeathlonCourse_InitMarkers(PokeathlonCourseData *course);
void PokeathlonCourse_FreeMarkers(PokeathlonCourseData *course);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, int planes);
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
void **PokeathlonCourse_CreateHudSprites(PokeathlonCourseData *course, int baseY, void *resources, void *spriteList);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int stat, int amount);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u8 score);
u8 *PokeathlonCourseMonGfx_GetInfo(void *mon);
int PokeathlonCourseMonGfx_GetTopRow(void *mon);
void PokeathlonCourseScoreList_Init(void *graphics);
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, void *graphics, u32 participant, u32 score, u8 idx, u8 digits, u32 msgno);
void PokeathlonCourseScoreList_Rank(void *graphics, u32 mode);
void PokeathlonCourseScoreList_AddPointsToTotals(void *graphics);
void PokeathlonCourseScoreList_SaveEventPoints(void *graphics, u32 event);
void *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, int flags, int charVram);
void PokeathlonCourseSprites_FreeResources(void *resources);
void PokeathlonCourseMonObjects_Free(void *monPics);
BOOL PokeathlonCourseMonObjects_IsLoaded(void *monPics);
void *PokeathlonCourseMonObjects_Get(void *monPics, u8 index);
void *PokeathlonCourseMonObject_GetGfx(void *monPic);
void PokeathlonCourseMonObject_SetDrawFlag(void *monPic, u32 visible);
void PokeathlonCourseMonObject_SetDrawPriority(void *monPic, u32 priority);
void PokeathlonCourseMonObject_SetAffineOverwriteMode(void *monPic, int mode);
void PokeathlonCourseMonObject_SetAffineMatrix(void *monPic, VecFx32 *center);
void PokeathlonCourseMonObject_SetAnimState(void *monPic, u32 anim);
void PokeathlonCourseObject_GetCollisionCircle(void *monPic, int x, int y, fx32 *outX, fx32 *outY, int *radius);
void *PokeathlonCourseObject_GetCollisionRadius(void *monPic);
void PokeathlonCourseObject_SetCenterPosition(void *monPic, s32 x, s32 y, u32 moveShadow);
void PokeathlonCourseObject_CenterToGround(void *monPic, s32 x, s32 y, s32 *outX, s32 *outY);
void PokeathlonCourseObject_GroundToCenter(void *monPic, s32 x, s32 y, s32 *outX, s32 *outY);
BOOL PokeathlonCourseObject_IsInTouchRange(void *monPic, fx32 x, fx32 y, s32 targetX, s32 targetY);
void PokeathlonCourseObject_SetScale(void *monPic, f32 scaleX, f32 scaleY);
void PokeathlonCourseObjects_PauseAnims(void *monPics, int paused);
void PokeathlonEvent_CalcPoints(void *graphics, u32 event);

// The runtime's division, called directly once.
s32 _s32_div_f(s32 dividend, s32 divisor);

// NitroSDK (no header declares it)
void MATH_QSort(void *head, u32 num, u32 size, s32 (*compare)(const void *, const void *), void *work);

// The rest of Disc Catch (pokeathlon_disc_catch_play.h), declared as the calls were built.
u32 PokeathlonDiscCatch_UpdateFrame(PokeathlonCourseData *course);
PokeathlonDiscCatchStats *PokeathlonDiscCatch_GetStats(PokeathlonCourseData *course, u8 participant, u8 member);
u16 PokeathlonDiscCatchStats_GetJumpDuration(PokeathlonDiscCatchStats *stats);
void PokeathlonDiscCatch_CreateMonPics(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_InitMons(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_UpdateField(PokeathlonDiscCatchTeam *teams, PokeathlonCourseData *course);
void PokeathlonDiscCatch_WriteFramePacket(PokeathlonDiscCatchField *field, PokeathlonCourseData *course);
void PokeathlonDiscCatchDiscs_Init(PokeathlonDiscCatchDiscs *discs, PokeathlonDiscCatchTeam *teams);
void PokeathlonDiscCatchSprites_Init(PokeathlonDiscCatchSprites *sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonDiscCatchScoreboard *scoreboard);
void PokeathlonDiscCatchDiscs_Update(PokeathlonDiscCatchDiscs *discs, u8 *area, int seconds);
void PokeathlonDiscCatchSprites_Update(PokeathlonDiscCatchSprites *sprites, PokeathlonCourseData *course);
void PokeathlonDiscCatchSprites_Delete(PokeathlonDiscCatchSprites *sprites);
void PokeathlonDiscCatch_AddScoreWindow(PokeathlonDiscCatch *work);
PokeathlonDiscCatchScoreboard *PokeathlonDiscCatchScoreboard_New(SpriteSystem *spriteSystem, SpriteManager *spriteManager, BgConfig *bgConfig, MsgData *msgData, MessageFormat *messageFormat, u8 participant, enum HeapID heapId, PokeathlonCourseData *course);
void PokeathlonDiscCatchScoreboard_Delete(PokeathlonDiscCatchScoreboard *scoreboard);
void PokeathlonDiscCatchScoreboard_Pause(PokeathlonDiscCatchScoreboard *scoreboard);
void PokeathlonDiscCatchScoreboard_SetOwnScore(PokeathlonDiscCatchScoreboard *scoreboard, u8 participant, u16 score);
u16 PokeathlonDiscCatchScoreboard_GetOwnScore(PokeathlonDiscCatchScoreboard *scoreboard);
PokeathlonDiscCatchCpu *PokeathlonDiscCatchCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonDiscCatchCpu_Delete(PokeathlonDiscCatchCpu *cpu);
void PokeathlonDiscCatchCpu_Update(PokeathlonDiscCatchCpu *cpu);

extern const VecFx32 gPokeathlonDiscCatchTouchDistances;
extern const VecFx32 gPokeathlonDiscCatchFacingReference;
extern const GraphicsModes gPokeathlonDiscCatchGraphicsModes;
extern const SpriteResourceCountsListUnion gPokeathlonDiscCatchSpriteResourceCounts;
extern const OamManagerParam gPokeathlonDiscCatchOamManagerParam;
extern const OamCharTransferParam gPokeathlonDiscCatchOamCharTransferParam;
extern const u32 gPokeathlonDiscCatchBgLayers[7];
extern const GraphicsBanks gPokeathlonDiscCatchGraphicsBanks;
extern const s16 gPokeathlonDiscCatchMonStartPositions[DISC_CATCH_PARTICIPANTS][DISC_CATCH_TEAM_SIZE][2]; // where the Pokemon start
extern const s16 ov96_0221CFBE[][3][2];                                           // their y (the second half of each entry)
extern const ManagedSpriteTemplate gPokeathlonDiscCatchDiscTopSpriteTemplate;
extern const ManagedSpriteTemplate gPokeathlonDiscCatchDiscTouchSpriteTemplate;
extern const ManagedSpriteTemplate gPokeathlonDiscCatchEffectSpriteTemplate;

typedef struct PokeathlonDiscCatchBgTemplates {
    BgTemplate templates[7];
} PokeathlonDiscCatchBgTemplates;

extern const PokeathlonDiscCatchBgTemplates gPokeathlonDiscCatchBgTemplates;

// The loops over the teams' results step a pointer to the result itself.
#define NEXT_TEAM_RESULT(result) ((PokeathlonDiscCatchResult *)((u8 *)(result) + sizeof(PokeathlonDiscCatchTeam)))

// A Pokemon sprite and the y its draw order goes by.
typedef struct PokeathlonDiscCatchDrawOrder {
    u8 y;
    void *monPic;
} PokeathlonDiscCatchDrawOrder;

#pragma force_active on

void PokeathlonDiscCatch_ShakeTask(SysTask *task, PokeathlonDiscCatchShake *shake) {
    int sign, offset;
    if (shake->flip == 0) {
        sign = 1;
    } else {
        sign = -1;
    }
    offset = shake->distance * sign;
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, offset + 8);
    ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, offset);
    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(shake->spriteSystem), 0, -offset << FX32_SHIFT);
    if (shake->frames % 2 == 1) {
        shake->distance--;
    }
    shake->flip ^= 1;
    shake->frames--;
    if (shake->frames <= 0) {
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 8);
        ScheduleSetBgPosText(shake->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 0);
        G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(shake->spriteSystem), 0, 0);
        shake->task = NULL;
        SysTask_Destroy(task);
    }
}

#pragma force_active reset

BOOL PokeathlonDiscCatchShake_IsDone(PokeathlonDiscCatchShake *shake) {
    GF_ASSERT(shake != NULL);
    return shake->task == NULL;
}

void PokeathlonDiscCatchShake_Init(PokeathlonDiscCatchShake *shake, BgConfig *bgConfig, SpriteSystem *spriteSystem) {
    shake->bgConfig = bgConfig;
    shake->spriteSystem = spriteSystem;
}

void PokeathlonDiscCatchShake_Start(PokeathlonDiscCatchShake *shake, u8 distance, u8 frames) {
    GF_ASSERT(shake->bgConfig != NULL);
    GF_ASSERT(shake->spriteSystem != NULL);
    if (shake->task != NULL) {
        SysTask_Destroy(shake->task);
    }
    shake->flip = 0;
    shake->distance = distance;
    shake->frames = frames;
    shake->task = SysTask_CreateOnMainQueue((SysTaskFunc)PokeathlonDiscCatch_ShakeTask, shake, 2);
}

// Squashes the Pokemon for two frames and stretches it back.
void PokeathlonDiscCatch_SquashTask(SysTask *task, void *data) {
    PokeathlonDiscCatchSquash *squash = data;
    switch (squash->state) {
    case 0:
        squash->scale = 1.0 - (double)squash->frames * 0.3;
        if (squash->frames > 1) {
            squash->frames = 0;
            squash->state++;
            return;
        }
        PokeathlonCourseObject_SetScale(squash->monPic, 1.0f, squash->scale);
        squash->frames++;
        return;
    case 1:
        squash->scale = 1.0 + (double)squash->frames * 0.3;
        if (squash->frames > 1) {
            squash->task = NULL;
            PokeathlonCourseObject_SetScale(squash->monPic, 1.0f, 1.0f);
            SysTask_Destroy(task);
            return;
        }
        PokeathlonCourseObject_SetScale(squash->monPic, 1.0f, squash->scale);
        squash->frames++;
        return;
    default:
        GF_ASSERT(FALSE);
        return;
    }
}

void PokeathlonDiscCatch_StartSquash(PokeathlonDiscCatchSquash *squash, void *monPic) {
    if (squash->task != NULL) {
        SysTask_Destroy(squash->task);
    }
    squash->monPic = monPic;
    squash->frames = 0;
    squash->state = 0;
    squash->task = SysTask_CreateOnMainQueue(PokeathlonDiscCatch_SquashTask, squash, 0);
}

#pragma force_active on

// Scales the Pokemon about its middle while it falls off the field, about its feet otherwise.
void PokeathlonDiscCatch_SetMonScaleCenter(void *monPic, int falling) {
    void *mon = PokeathlonCourseMonObject_GetGfx(monPic);
    int height = PokeathlonCourseMonGfx_GetTopRow(mon);
    int size;
    VecFx32 center = { 0, 0, 0 };
    if (*(u16 *)(PokeathlonCourseMonGfx_GetInfo(mon) + 4)) {
        size = 64;
        PokeathlonCourseMonObject_SetAffineOverwriteMode(monPic, 1);
    } else {
        size = 32;
        PokeathlonCourseMonObject_SetAffineOverwriteMode(monPic, 2);
    }
    if (falling) {
        center.x = (size / 2) << FX32_SHIFT;
        center.y = (height + (size - height) / 2) << FX32_SHIFT;
    } else {
        center.x = (size / 2) << FX32_SHIFT;
        center.y = (size - 1) << FX32_SHIFT;
    }
    PokeathlonCourseMonObject_SetAffineMatrix(monPic, &center);
}

BOOL PokeathlonDiscCatchMon_IsTouching(PokeathlonDiscCatchMon *mon, PokeathlonDiscCatchMon *other) {
    VecFx32 distance, center, otherCenter;
    int radius, otherRadius, sum;
    PokeathlonCourseObject_GetCollisionCircle(mon->monPic, mon->position.x, mon->position.y, &center.x, &center.y, &radius);
    PokeathlonCourseObject_GetCollisionCircle(other->monPic, other->position.x, other->position.y, &otherCenter.x, &otherCenter.y, &otherRadius);
    sum = (radius + otherRadius) << FX32_SHIFT;
    center.z = 0;
    otherCenter.z = 0;
    VEC_Subtract(&center, &otherCenter, &distance);
    if (VEC_Mag(&distance) <= sum) {
        return TRUE;
    }
    return FALSE;
}

void PokeathlonDiscCatchMon_Jump(PokeathlonDiscCatchMon *mon) {
    if (PokeathlonDiscCatch_GetStats(mon->course, mon->participant, mon->member)->jump) {
        PokeathlonCourse_AddMonCounter(mon->course, mon->participant, mon->member, 2, 1);
        mon->jumpTimer = PokeathlonDiscCatchStats_GetJumpDuration(mon->stats);
        mon->state = DISC_CATCH_MON_JUMPING;
    }
    mon->jumpSignal = 10;
}

#pragma force_active reset

BOOL PokeathlonDiscCatchMon_IsInPlay(PokeathlonDiscCatchMon *mon) {
    u32 state;
    BOOL result;
    GF_ASSERT(mon != NULL);
    state = mon->state;
    result = TRUE;
    if (state != DISC_CATCH_MON_ON_FIELD && state != DISC_CATCH_MON_JUMPING) {
        result = FALSE;
    }
    return result;
}

void PokeathlonDiscCatchMon_SetTarget(PokeathlonDiscCatchMon *mon, const s16 *position) {
    mon->target.x = position[0] << FX32_SHIFT;
    mon->target.y = position[1] << FX32_SHIFT;
    mon->dragged = TRUE;
}

#pragma force_active on

// 4: right, 2: down, 3: left, 1: up
int PokeathlonDiscCatch_GetFacing(const VecFx32 *direction) {
    VecFx32 reference = gPokeathlonDiscCatchFacingReference;
    VecFx32 vec = { 0 };
    u16 angle;
    vec.x = direction->y;
    vec.z = direction->x;
    vec.y = 0;
    angle = CalcAngleBetweenVecs(&reference, &vec);
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

// Walks towards the target; while knocked back the velocity runs down first.
#pragma opt_propagation off

void PokeathlonDiscCatchMon_Move(PokeathlonDiscCatchMon *mon) {
    VecFx32 delta, normal, step, zero2, zero1, deceleration, push, zero3;
    int distance;
    if (!mon->state) {
        return;
    }
    VEC_Subtract(&mon->target, &mon->position, &delta);
    distance = VEC_Mag(&delta);
    mon->moving = distance > 0;
    if (mon->knockedBack) {
        {
            int zero = 0;
            volatile VecFx32 *p = &zero1;
            p->x = zero;
            volatile VecFx32 *q = &zero2;
            p->y = zero;
            q->x = zero;
            p->z = zero;
            q->y = zero;
            q->z = zero;
        }
        VEC_Normalize(&mon->velocity, &zero2);
        VEC_MultAdd(FX32_CONST(0.8), &zero2, &zero1, &deceleration);
        deceleration.x = deceleration.x < 0 ? -deceleration.x : deceleration.x;
        deceleration.y = deceleration.y < 0 ? -deceleration.y : deceleration.y;
        if (mon->dragged) {
            int scale = mon->stats->power / 100;
            VEC_Normalize(&delta, &normal);
            VEC_MultAdd(scale, &mon->direction, &zero1, &push);
            deceleration.x += push.x < 0 ? -push.x : push.x;
            deceleration.y += push.y < 0 ? -push.y : push.y;
        }
        PokeathlonDiscCatch_SlowDown(&mon->velocity, deceleration.x, deceleration.y);
        VEC_Add(&mon->position, &mon->velocity, &mon->position);
        mon->target = mon->position;
        if (mon->velocity.x || mon->velocity.y) {
            return;
        }
        mon->knockedBack = FALSE;
    }
    if (mon->moving) {
        VEC_Normalize(&delta, &normal);
        if (!mon->knockedBack) {
            mon->facing = PokeathlonDiscCatch_GetFacing(&normal);
        }
        int speed = mon->stats->speed;
        if (distance <= speed) {
            step = delta;
        } else {
            volatile VecFx32 *z = &zero3;
            z->x = 0;
            z->y = 0;
            z->z = 0;
            VEC_MultAdd(speed, &normal, &zero3, &step);
        }
        VEC_Add(&mon->position, &step, &mon->position);
        mon->direction = normal;
    }
}

#pragma opt_propagation reset
#pragma force_active reset

BOOL PokeathlonDiscCatchMon_IsOffField(PokeathlonDiscCatchMon *mon, fx32 x, fx32 y) {
    s32 fieldX;
    s32 fieldY;
    PokeathlonCourseObject_CenterToGround(mon->monPic, x >> FX32_SHIFT, y >> FX32_SHIFT, &fieldX, &fieldY);
    if (fieldX <= 0 || fieldY <= 0 || fieldX >= 255 || fieldY >= 255) {
        return TRUE;
    }
    if (fieldX < 8 || fieldX >= 248 || fieldY < 8 || fieldY >= 180) {
        return TRUE;
    }
    return FALSE;
}

void PokeathlonDiscCatchMon_Respawn(PokeathlonDiscCatchMon *mon) {
    u8 index;
    int member;
    int participant;
    s32 x;
    s32 y;
    VecFx32 zero;

    index = mon->index;
    member = index % 3;
    participant = index / 3;
    PokeathlonCourseObject_GroundToCenter(mon->monPic, gPokeathlonDiscCatchMonStartPositions[participant][member][0], ov96_0221CFBE[participant][member][0], &x, &y);
    mon->position.x = x << FX32_SHIFT;
    mon->position.y = y << FX32_SHIFT;
    mon->target.x = x << FX32_SHIFT;
    mon->target.y = y << FX32_SHIFT;
    zero = (VecFx32) { 0, 0, 0 };
    mon->velocity = zero;
    mon->facing = 2;
    mon->knockedBack = FALSE;
}

// The update called the movement through a declaration that also passes the course.
typedef void (*PokeathlonDiscCatchMoveFunc)(PokeathlonDiscCatchMon *mon, PokeathlonCourseData *course);
#define MoveWithCourse ((PokeathlonDiscCatchMoveFunc)PokeathlonDiscCatchMon_Move)

#pragma force_active on

void PokeathlonDiscCatchMon_Update(PokeathlonDiscCatchMon *mon, PokeathlonCourseData *course) {
    switch (mon->state) {
    case DISC_CATCH_MON_RESPAWNED:
        mon->state = DISC_CATCH_MON_ON_FIELD;
        break;
    case DISC_CATCH_MON_FALLEN:
        mon->respawnWait--;
        if (mon->respawnWait == 0) {
            PokeathlonDiscCatchMon_Respawn(mon);
            mon->state = DISC_CATCH_MON_RESPAWNED;
        }
        break;
    case DISC_CATCH_MON_ON_FIELD:
        MoveWithCourse(mon, course);
        if (PokeathlonDiscCatchMon_IsOffField(mon, mon->position.x, mon->position.y)) {
            PokeathlonCourse_AddMonCounter(mon->course, mon->participant, mon->member, 1, 1);
            PokeathlonCourse_AddMonCounter(mon->course, mon->participant, mon->member, 5, 1);
            if (mon->draggedOff) {
                PokeathlonCourse_AddMonCounter(mon->course, mon->participant, mon->member, 8, 1);
            }
            mon->draggedOff = FALSE;
            mon->respawnWait = 90;
            mon->state = DISC_CATCH_MON_FALLEN;
        }
        break;
    case DISC_CATCH_MON_JUMPING:
        mon->jumpTimer--;
        if (mon->jumpTimer == 0) {
            mon->state = DISC_CATCH_MON_ON_FIELD;
        }
        MoveWithCourse(mon, course);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

#pragma force_active reset

// Of a team's three Pokemon, the nearest one the touch at (x, y) hits; DISC_CATCH_NO_MON if none.
int PokeathlonDiscCatch_FindTouchedMon(PokeathlonDiscCatchMon *mons, s16 x, s16 y) {
    VecFx32 distances;
    fx32 min;
    int best;
    int i;

    best = DISC_CATCH_NO_MON;
    distances = gPokeathlonDiscCatchTouchDistances;
    min = 1 << 20;
    for (i = 0; i < DISC_CATCH_TEAM_SIZE; i++) {
        if (PokeathlonCourseObject_IsInTouchRange(mons->monPic, mons->position.x, mons->position.y, x, y)) {
            VecFx32 delta = { 0, 0, 0 };
            fx32 difference;
            difference = (x << FX32_SHIFT) - mons->position.x;
            if (difference < 0) {
                difference = -difference;
            }
            delta.x = difference;
            difference = (y << FX32_SHIFT) - mons->position.y;
            if (difference < 0) {
                difference = -difference;
            }
            delta.y = difference;
            ((fx32 *)&distances)[i] = VEC_Mag(&delta);
        }
        mons++;
    }
    for (i = 0; i < DISC_CATCH_TEAM_SIZE; i++) {
        if (((fx32 *)&distances)[i] < min) {
            min = ((fx32 *)&distances)[i];
            best = i;
        }
    }
    return (s8)best;
}

// mode 0: on the top screen, 1: on the touch screen, 2: both.
void PokeathlonDiscCatch_ShowDisc(PokeathlonDiscCatchDiscSprites *sprites, u32 mode, BOOL draw) {
    BOOL hidden;
    switch (mode) {
    case 0:
        ManagedSprite_SetDrawFlag(sprites->disc[0], draw);
        hidden = draw == FALSE;
        ManagedSprite_SetDrawFlag(sprites->disc[1], hidden);
        ManagedSprite_SetDrawFlag(sprites->shadow[0], draw);
        ManagedSprite_SetDrawFlag(sprites->shadow[1], hidden);
        break;
    case 1:
        hidden = draw == FALSE;
        ManagedSprite_SetDrawFlag(sprites->disc[0], hidden);
        ManagedSprite_SetDrawFlag(sprites->disc[1], draw);
        ManagedSprite_SetDrawFlag(sprites->shadow[0], hidden);
        ManagedSprite_SetDrawFlag(sprites->shadow[1], draw);
        break;
    case 2:
        ManagedSprite_SetDrawFlag(sprites->disc[0], draw);
        ManagedSprite_SetDrawFlag(sprites->disc[1], draw);
        ManagedSprite_SetDrawFlag(sprites->shadow[0], draw);
        ManagedSprite_SetDrawFlag(sprites->shadow[1], draw);
        break;
    }
}

void PokeathlonDiscCatch_ScaleDisc(PokeathlonDiscCatchDiscSprites *sprites, f32 scale) {
    ManagedSprite_SetAffineScale(sprites->disc[0], scale, scale);
    ManagedSprite_SetAffineScale(sprites->disc[1], scale, scale);
}

void PokeathlonDiscCatch_ShowPoints(PokeathlonDiscCatchDiscSprites *sprites, int state, s16 x, s16 y) {
    int anim = state - DISC_CATCH_DISC_CAUGHT_1;
    GF_ASSERT(anim < 4);
    ManagedSprite_SetPositionXYWithSubscreenOffset(sprites->points, x, y, FX32_CONST(480));
    ManagedSprite_SetPositionXYWithSubscreenOffset(sprites->sparkle, x, y, FX32_CONST(480));
    ManagedSprite_SetAnim(sprites->points, anim);
    ManagedSprite_ResetSpriteAnimCtrlState(sprites->sparkle);
    ManagedSprite_SetDrawFlag(sprites->points, TRUE);
    ManagedSprite_SetDrawFlag(sprites->sparkle, TRUE);
}

void PokeathlonDiscCatch_PlayCatchSound(u32 state) {
    s16 pitch;
    switch (state) {
    case DISC_CATCH_DISC_CAUGHT_1:
        pitch = 0;
        break;
    case DISC_CATCH_DISC_CAUGHT_2:
        pitch = 128;
        break;
    case DISC_CATCH_DISC_CAUGHT_3:
        pitch = 0;
        break;
    case DISC_CATCH_DISC_CAUGHT_5:
        pitch = 192;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    PlaySE_SetPitch(SEQ_SE_GS_POINT1, pitch);
}

#pragma force_active on

// Shows the discs of the frame packet.
void PokeathlonDiscCatch_UpdateDiscSprites(PokeathlonDiscCatchSprites *sprites, PokeathlonDiscCatchDiscPacket *discs, PokeathlonCourseData *course) {
    u8 myId = PokeathlonCourse_GetNetId(course);
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonDiscCatchDiscSprites *disc;
    int i;
    s16 y, screenY;
    int screen;
    s16 x;

    for (i = 0, disc = sprites->discs; i < DISC_CATCH_DISCS; i++, disc++, discs++) {
        y = discs->progress * 8 / 2;
        if (discs->state == DISC_CATCH_DISC_LANDED) {
            y += 480;
        }
        screenY = y;
        if (y >= 288) {
            screen = 1;
        } else {
            screen = 0;
        }
        if (screen) {
            screenY -= 288;
        }
        x = PokeathlonDiscCatch_GetDiscX(y, discs->path);
        if (discs->state == DISC_CATCH_DISC_NONE) {
            PokeathlonDiscCatch_ShowDisc(disc, 2, FALSE);
            ManagedSprite_SetDrawFlag(disc->points, FALSE);
            ManagedSprite_SetDrawFlag(disc->sparkle, FALSE);
        } else if (discs->state >= DISC_CATCH_DISC_CAUGHT_1 && disc->shownState < DISC_CATCH_DISC_CAUGHT_1) {
            PokeathlonDiscCatch_ShowDisc(disc, 2, FALSE);
            PokeathlonDiscCatch_ShowPoints(disc, discs->state, x, screenY - 40);
            if (discs->owner == myId) {
                PokeathlonDiscCatchShake_Start(&work->shake, 4, 4);
                PokeathlonDiscCatch_PlayCatchSound(discs->state);
            } else {
                ManagedSprite_SetDrawFlag(disc->points, FALSE);
            }
            PlaySE(SEQ_SE_GS_W280);
        } else if (discs->state == DISC_CATCH_DISC_FLYING) {
            PokeathlonDiscCatch_ShowDisc(disc, screen, TRUE);
            ManagedSprite_SetDrawFlag(disc->points, FALSE);
            ManagedSprite_SetDrawFlag(disc->sparkle, FALSE);
        }
        if (disc->shownState == DISC_CATCH_DISC_NONE && discs->state == DISC_CATCH_DISC_FLYING) {
            disc->angle = 0;
            PokeathlonDiscCatch_ScaleDisc(disc, 1.2f);
            PlaySE(SEQ_SE_GS_W030);
        }
        if (discs->state == DISC_CATCH_DISC_FLYING || discs->state == DISC_CATCH_DISC_LANDED) {
            if (screen && x > 16) {
                ManagedSprite_SetAnimNoRestart(disc->shadow[screen], 1);
            }
            {
                f32 scale = 1.2f + (f32)(GF_SinDeg(disc->angle) / 18) / 4096.0f;
                disc->angle += 10;
                PokeathlonDiscCatch_ScaleDisc(disc, scale);
            }
            ManagedSprite_SetPositionXYWithSubscreenOffset(disc->disc[screen], x, screenY - 40, FX32_CONST(480));
            ManagedSprite_SetPositionXYWithSubscreenOffset(disc->shadow[screen], x, screenY, FX32_CONST(480));
        }
        disc->shownState = discs->state;
    }
}

#pragma force_active reset

// The lower Pokemon is drawn in front.
s32 PokeathlonDiscCatch_CompareDrawOrder(const void *a, const void *b) {
    if (*(const u8 *)a == *(const u8 *)b) {
        return 0;
    }
    if (*(const u8 *)a < *(const u8 *)b) {
        return 1;
    }
    return -1;
}

// Shows the Pokemon of the frame packet: positions, facings, jumps and falls, and the draw order.
void PokeathlonDiscCatch_UpdateMonSprites(PokeathlonDiscCatchSprites *sprites, PokeathlonDiscCatchFrame *frame, PokeathlonCourseData *course) {
    PokeathlonDiscCatchDrawOrder order[DISC_CATCH_MONS] = { 0 };
    s32 x;
    s32 y;
    s32 fieldX;
    s32 fieldY;
    f32 scale;
    PokeathlonDiscCatch *work;
    PokeathlonDiscCatchMonSprites *mon;
    u8 fallen;
    PokeathlonDiscCatchDrawOrder *entry;
    void *monPic;
    u8 monY;
    int i;
    int shift;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    mon = sprites->mons;
    i = 0;
    entry = order;
    shift = 0;
    for (; i < DISC_CATCH_MONS; i++) {
        monPic = PokeathlonCourseMonObjects_Get(work->monPics, (u8)i);
        GF_ASSERT(mon != NULL);
        GF_ASSERT(monPic != NULL);
        monY = frame->monY[i];
        PokeathlonCourseObject_GroundToCenter(monPic, frame->monX[i], monY, &x, &y);
        entry->y = monY;
        entry->monPic = monPic;
        fallen = (u8)((frame->fallen >> i) & 1);
        if (fallen == 1 && mon->fallen == 0) {
            PokeathlonDiscCatch_SetMonScaleCenter(monPic, TRUE);
            mon->falling = TRUE;
            mon->fallFrames = 6;
            mon->fallScale = FX32_ONE;
            PlaySE(SEQ_SE_GS_PUSH02);
        } else if (fallen == 0 && mon->fallen == 1) {
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->splash, (s16)x, (s16)y, FX32_CONST(480));
            ManagedSprite_SetDrawFlag(mon->splash, TRUE);
            ManagedSprite_ResetSpriteAnimCtrlState(mon->splash);
            PokeathlonDiscCatch_SetMonScaleCenter(monPic, FALSE);
            PokeathlonCourseObject_SetScale(monPic, 1.0f, 1.0f);
            PokeathlonCourseMonObject_SetDrawFlag(monPic, TRUE);
            mon->falling = FALSE;
            PlaySE(SEQ_SE_GS_BOWA2);
        }
        mon->fallen = fallen;
        if (mon->falling) {
            mon->fallFrames--;
            if (mon->fallFrames == 0) {
                PokeathlonCourseMonObject_SetDrawFlag(monPic, FALSE);
            } else {
                f32 fallScale;
                mon->fallScale -= FX32_CONST(0.1);
                fallScale = (f32)mon->fallScale / 4096.0f;
                PokeathlonCourseObject_SetScale(monPic, fallScale, fallScale);
                PokeathlonCourseObject_CenterToGround(monPic, frame->monX[i], frame->monY[i], &fieldX, &fieldY);
                if (fieldY >= 180) {
                    u32 sink = (6 - mon->fallFrames) * 4;
                    u32 sinkY = monY;
                    sinkY += sink;
                    PokeathlonCourseObject_SetCenterPosition(monPic, x, sinkY, 1);
                }
            }
        } else {
            u8 facing = (u8)((frame->facings >> shift) & 3);
            PokeathlonCourseObject_SetCenterPosition(monPic, x, y, 1);
            PokeathlonCourseMonObject_SetAnimState(monPic, facing + 1);
            if ((frame->jumped >> i) & 1) {
                PokeathlonDiscCatchStats *stats;
                int participant;
                participant = i / 3;
                stats = PokeathlonDiscCatch_GetStats(course, (u8)participant, (u8)(i % 3));
                if (participant == (int)PokeathlonCourse_GetNetId(course) && mon->jumping == 0 && mon->squashed == 0) {
                    if (stats->jump == 0) {
                        PlaySE(SEQ_SE_DP_BOX03);
                    } else {
                        PlaySE(SEQ_SE_GS_UFO_JUMP);
                    }
                }
                if (stats->jump == 0) {
                    PokeathlonDiscCatch_StartSquash(&mon->squash, monPic);
                    mon->squashed = TRUE;
                } else {
                    mon->jumping = TRUE;
                }
            } else {
                mon->squashed = FALSE;
            }
            if (mon->jumping) {
                PokeathlonDiscCatchStats *stats;
                u32 frames;
                stats = PokeathlonDiscCatch_GetStats(course, (u8)(i / 3), (u8)(i % 3));
                PokeathlonCourseObject_SetCenterPosition(monPic, x, y + PokeathlonDiscCatch_GetJumpOffset(stats, mon->jumpFrame, &scale), 0);
                PokeathlonCourseObject_SetScale(monPic, scale, scale);
                frames = PokeathlonDiscCatchStats_GetJumpDuration(stats);
                mon->jumpFrame++;
                if (mon->jumpFrame > frames) {
                    mon->jumping = FALSE;
                    mon->jumpFrame = 0;
                    PokeathlonCourseObject_SetCenterPosition(monPic, x, y, 0);
                    PokeathlonCourseObject_SetScale(monPic, 1.0f, 1.0f);
                }
            }
        }
        entry++;
        shift += 2;
        mon++;
    }
    MATH_QSort(order, DISC_CATCH_MONS, sizeof(PokeathlonDiscCatchDrawOrder), PokeathlonDiscCatch_CompareDrawOrder, NULL);
    shift = 0;
    entry = order;
    for (; shift < DISC_CATCH_MONS; shift++) {
        GF_ASSERT(entry->monPic != NULL);
        PokeathlonCourseMonObject_SetDrawPriority(entry->monPic, shift + 7);
        entry++;
    }
}

// Event state 0: fades in, then waits for the course to start the event.
u32 PokeathlonDiscCatch_Intro(PokeathlonCourseData *course, u8 *state) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonDiscCatch_AnimateBg(&work->bgFrameTimer, work->bgConfig, work->heapId);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, work->heapId);
        (*state)++;
    case 1:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 2:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 0;
}

// The time is up: stops the sprites and reports the own score.
void PokeathlonDiscCatch_EndGame(PokeathlonCourseData *course) {
    u8 score;
    PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    score = (u8)PokeathlonDiscCatchScoreboard_GetOwnScore(((PokeathlonDiscCatch *)PokeathlonCourse_GetHeapAllocPtr4(course))->scoreboard);
    PokeathlonCourse_SetRecordScore(course, score);
    PokeathlonDiscCatch_PrintScore(course, score);
}

// Event state 1: runs the game until the host reports the end and the shake is over.
u32 PokeathlonDiscCatch_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonDiscCatch_AnimateBg(&work->bgFrameTimer, work->bgConfig, work->heapId);
    switch (*state) {
    case 0:
        PokeathlonDiscCatch_UpdateHost(course);
        if (PokeathlonDiscCatch_UpdateFrame(course)) {
            (*state)++;
        }
        break;
    case 1: {
        PokeathlonDiscCatchFrame *frame;
        PokeathlonDiscCatch_UpdateHost(course);
        frame = (PokeathlonDiscCatchFrame *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
        PokeathlonDiscCatchScoreboard_SetOwnScore(work->scoreboard, (u8)frame->scoreParticipant, (u16)frame->score);
        if (PokeathlonCourse_RunFinish(course)) {
            if (PokeathlonDiscCatchShake_IsDone(&work->shake)) {
                BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
                PokeathlonCourse_SetStateField07(course, 2);
            }
        }
        break;
    }
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 0;
}

// Event state 2: after the fade the host records the teams' scores.
u32 PokeathlonDiscCatch_Finish(PokeathlonCourseData *course, u8 *state) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (*state == 0) {
        if (IsPaletteFadeFinished()) {
            u32 isGuest = PokeathlonCourse_GetNetId(course);
            work = PokeathlonCourse_GetHeapAllocPtr4(course);
            if (isGuest == 0) {
                int i;
                PokeathlonDiscCatchResult *result;
                for (i = 0, result = &work->field.teams[0].result; i < DISC_CATCH_PARTICIPANTS; i++) {
                    PokeathlonCourse_SetField5E0_AtIndex(course, (u8)i, (u16)result->score);
                    result = NEXT_TEAM_RESULT(result);
                }
            }
            return 1;
        }
    } else {
        GF_ASSERT(FALSE);
    }
    return 0;
}

// Event template entry 0.
u32 PokeathlonDiscCatch_Init(PokeathlonCourseData *course) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_142, 0x60000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        PokeathlonDiscCatch_SetVramBanks();
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        work = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonDiscCatch));
        MI_CpuFill8(work, 0, sizeof(PokeathlonDiscCatch));
        work->heapId = HEAP_ID_142;
        work->course = course;
        work->timer = 1800;
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        work->bgConfig = BgConfig_Alloc(work->heapId);
        PokeathlonCourse_SetHudPlanes(course, 4);
        PokeathlonDiscCatch_InitBgs(work);
        PokeathlonDiscCatch_LoadBgGraphics(work);
        PokeathlonDiscCatch_AddScoreWindow(work);
        WirelessStatusIcon_CreateOnScreenIfConnected(2);
        PokeathlonDiscCatch_InitSpriteSystem(work);
        PokeathlonDiscCatch_CreateMonPics(work);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        if (PokeathlonCourseMonObjects_IsLoaded(work->monPics)) {
            PokeathlonDiscCatch_LoadSpriteResources(work);
            Sprite_SetDrawPriority(*PokeathlonCourse_CreateHudSprites(course, 0, work->monPicResources, SpriteManager_GetSpriteList(work->spriteManager)), 0);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 3: {
        u8 participant = (u8)PokeathlonCourse_GetNetId(course);
        work->scoreboard = PokeathlonDiscCatchScoreboard_New(work->spriteSystem, work->spriteManager, work->bgConfig, work->msgData, work->messageFormat, participant, work->heapId, course);
        PokeathlonDiscCatchSprites_Init(&work->sprites, work->spriteSystem, work->spriteManager, work->scoreboard);
        work->cpu = PokeathlonDiscCatchCpu_New(course, work->heapId);
        PokeathlonCourse_InitMarkers(course);
        PokeathlonDiscCatchShake_Init(&work->shake, work->bgConfig, work->spriteSystem);
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)work->bgConfig); // the callback gets the BgConfig
        PokeathlonCourse_SetField1F4(course, 1);
        PokeathlonDiscCatch_InitMons(work);
        if (PokeathlonCourse_GetNetId(course) == 0) {
            PokeathlonDiscCatchFrame *frame = (PokeathlonDiscCatchFrame *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
            frame->seconds = (work->timer + 30) / 30;
            PokeathlonDiscCatch_WriteFramePacket(&work->field, course);
        }
        PokeathlonDiscCatchDiscs_Init(&work->discs, work->field.teams);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        return 1;
    }
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 0;
}

// Event template entry 1.
u32 PokeathlonDiscCatch_Draw(PokeathlonCourseData *course) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    GF_ASSERT(work != NULL);
    SpriteSystem_DrawSprites(work->spriteManager);
    return 1;
}

// Event template entry 2.
u32 PokeathlonDiscCatch_ShowResults(PokeathlonCourseData *course) {
    void *graphics = PokeathlonCourse_GetGraphicsSystem(course);
    int i;
    PokeathlonCourseScoreList_Init(graphics);
    for (i = 0; i < DISC_CATCH_PARTICIPANTS; i++) {
        PokeathlonCourse_AddScoreEntry(course, graphics, i, PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i), 0, 3, 295);
    }
    PokeathlonCourseScoreList_Rank(graphics, 1);
    PokeathlonEvent_CalcPoints(graphics, 4);
    PokeathlonCourseScoreList_AddPointsToTotals(graphics);
    PokeathlonCourseScoreList_SaveEventPoints(graphics, 4);
    return 1;
}

// Event template entry 3: frees everything Init created.
u32 PokeathlonDiscCatch_Exit(PokeathlonCourseData *course) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonCourse_FreeMarkers(course);
    PokeathlonDiscCatchCpu_Delete(work->cpu);
    PokeathlonDiscCatchScoreboard_Delete(work->scoreboard);
    PokeathlonDiscCatchSprites_Delete(&work->sprites);
    PokeathlonDiscCatch_FreeSpriteSystem(work);
    PokeathlonDiscCatch_FreeBgs(work);
    WirelessStatusIcon_Destroy();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_142);
    return 1;
}

// x of a disc on its path: a slant by the path (0..14, 7 straight down) plus a wave.
s16 PokeathlonDiscCatch_GetDiscX(int progress, int path) {
    // (the division is called directly: it comes before the sine)
    s16 x = (s16)(_s32_div_f(38 * (progress * (path - 7)), 1000) + 128);
    return (s16)(x + ((GF_SinDeg((u16)(progress << 1)) << 6) >> 16));
}

BOOL PokeathlonDiscCatch_IsMonInPlay(PokeathlonDiscCatch *work, u32 participant, u32 member) {
    u32 state;
    BOOL result;
    GF_ASSERT(work != NULL);
    GF_ASSERT(participant < DISC_CATCH_PARTICIPANTS);
    GF_ASSERT(member < DISC_CATCH_TEAM_SIZE);
    result = TRUE;
    state = work->field.teams[participant].mons[member].state;
    if (state != DISC_CATCH_MON_ON_FIELD && state != DISC_CATCH_MON_JUMPING) {
        result = FALSE;
    }
    return result;
}

PokeathlonDiscCatchDisc *PokeathlonDiscCatch_GetDisc(PokeathlonDiscCatch *work, u32 index) {
    GF_ASSERT(work != NULL);
    GF_ASSERT(index < DISC_CATCH_DISCS);
    return &work->discs.discs[index];
}

void PokeathlonDiscCatch_GetMonPosition(PokeathlonDiscCatch *work, u32 participant, u32 member, s16 *position, void **radius) {
    PokeathlonDiscCatchMon *mon;
    GF_ASSERT(work != NULL);
    GF_ASSERT(participant < DISC_CATCH_PARTICIPANTS);
    GF_ASSERT(member < DISC_CATCH_TEAM_SIZE);
    mon = &work->field.teams[participant].mons[member];
    position[0] = (s16)(mon->position.x >> FX32_SHIFT);
    position[1] = (s16)(mon->position.y >> FX32_SHIFT);
    *radius = PokeathlonCourseObject_GetCollisionRadius(mon->monPic);
}

// How many teams have more points than the participant's.
u8 PokeathlonDiscCatch_CountTeamsAhead(PokeathlonCourseData *course, u32 participant) {
    u8 count = 0;
    PokeathlonDiscCatch *work;
    int i;
    u16 score;
    PokeathlonDiscCatchResult *result;
    GF_ASSERT(course != NULL);
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    GF_ASSERT(work != NULL);
    i = 0;
    score = (u16)work->field.teams[participant].result.score;
    result = &work->field.teams[0].result;
    for (; i < DISC_CATCH_PARTICIPANTS; i++) {
        u16 other = (u16)result->score;
        if (i != (int)participant && score < other) {
            count++;
        }
        result = NEXT_TEAM_RESULT(result);
    }
    return count;
}

void PokeathlonDiscCatch_SetMonTarget(PokeathlonDiscCatch *work, const s16 *position, u32 participant, u32 member) {
    PokeathlonDiscCatchMon_SetTarget(&work->field.teams[participant].mons[member], position);
}

void PokeathlonDiscCatch_MakeMonJump(PokeathlonDiscCatch *work, u32 participant, u32 member) {
    PokeathlonDiscCatchMon_Jump(&work->field.teams[participant].mons[member]);
}

u8 PokeathlonDiscCatch_GetTeamScore(PokeathlonDiscCatch *work, int participant) {
    return work->field.teams[participant].result.score;
}

#pragma force_active on

void PokeathlonDiscCatch_SetVramBanks(void) {
    GraphicsBanks banks = gPokeathlonDiscCatchGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

#pragma force_active reset

int PokeathlonDiscCatch_Square(int value) {
    return value * value;
}

#pragma force_active on

// Moves the value towards 0 by step.
void PokeathlonDiscCatch_Approach0(fx32 *value, fx32 step) {
    if (*value != 0) {
        int magnitude = *value < 0 ? -*value : *value;
        if (magnitude < step) {
            *value = 0;
            return;
        }
        *value -= (*value > 0 ? 1 : -1) * step;
    }
}

// The height of a jumping Pokemon at the frame and its scale.
int PokeathlonDiscCatch_GetJumpOffset(PokeathlonDiscCatchStats *stats, int frame, f32 *scale) {
    int offset = 0;
    f32 result = 1.0f;
    int half, curve;
    f32 height;
    int fxHeight;
    GF_ASSERT(stats != NULL);
    half = stats->jumpFrames;
    curve = PokeathlonDiscCatch_Square(frame - half) - PokeathlonDiscCatch_Square(half);
    if (curve != 0) {
        if (stats->jumpHeight != 0) {
            height = 0.5f + (f32)(stats->jumpHeight << FX32_SHIFT);
        } else {
            height = (f32)(stats->jumpHeight << FX32_SHIFT) - 0.5f;
        }
        fxHeight = (int)height;
        if (fxHeight != 0) {
            offset = (fxHeight / 10) * curve;
            if (offset <= FX32_CONST(-24)) {
                offset = FX32_CONST(-24);
            }
            result = result - (f32)((offset - FX32_CONST(4)) / 100) / 4096.0f;
        }
    }
    *scale = result;
    return offset >> FX32_SHIFT;
}

#pragma force_active reset

void PokeathlonDiscCatch_SlowDown(VecFx32 *velocity, fx32 stepX, fx32 stepY) {
    PokeathlonDiscCatch_Approach0(&velocity->x, stepX);
    PokeathlonDiscCatch_Approach0(&velocity->y, stepY);
}

void PokeathlonDiscCatch_ClampVelocity(VecFx32 *velocity, fx32 limit) {
    fx32 negLimit = -limit;

    if (velocity->x > 0) {
        if (velocity->x > limit) {
            velocity->x = limit;
        }
    } else if (velocity->x < negLimit) {
        velocity->x = negLimit;
    }

    if (velocity->y > 0) {
        if (velocity->y > limit) {
            velocity->y = limit;
        }
    } else if (velocity->y < negLimit) {
        velocity->y = negLimit;
    }
}

#pragma force_active on

void PokeathlonDiscCatch_PrintScore(PokeathlonCourseData *course, int score) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    MessageFormat *messageFormat = work->messageFormat;
    MsgData *msgData = work->msgData;
    String *string;
    FillWindowPixelBuffer(&work->window, 0);
    BufferIntegerAsString(messageFormat, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, msg_0309_00160, work->heapId);
    AddTextPrinterParameterizedWithColor(&work->window, 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(&work->window);
}

void PokeathlonDiscCatch_InitSpriteSystem(PokeathlonDiscCatch *work) {
    SpriteResourceCountsListUnion counts = gPokeathlonDiscCatchSpriteResourceCounts;
    OamManagerParam oamParam = gPokeathlonDiscCatchOamManagerParam;
    OamCharTransferParam transferParam = gPokeathlonDiscCatchOamCharTransferParam;
    transferParam.maxTasks = 128;
    work->spriteSystem = SpriteSystem_Alloc(work->heapId);
    work->spriteManager = SpriteManager_New(work->spriteSystem);
    SpriteSystem_Init(work->spriteSystem, &oamParam, &transferParam, 32);
    SpriteSystem_InitSprites(work->spriteSystem, work->spriteManager, 128);
    SpriteSystem_InitManagerWithCapacities(work->spriteSystem, work->spriteManager, &counts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, FX32_CONST(480));
    work->monPicResources = PokeathlonCourseSprites_CreateResources(work->heapId, 2727, 1);
}

#pragma force_active reset

void PokeathlonDiscCatch_FreeSpriteSystem(PokeathlonDiscCatch *work) {
    PokeathlonCourseMonObjects_Free(work->monPics);
    PokeathlonCourseSprites_FreeResources(work->monPicResources);
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
}

// Stops all animations (the time is up).
void PokeathlonDiscCatch_StopAnims(PokeathlonDiscCatch *work) {
    int i;
    int j;
    PokeathlonDiscCatchDiscSprites *disc;
    BOOL animate0;
    BOOL animate1;

    PokeathlonCourseObjects_PauseAnims(work->monPics, 1);
    PokeathlonDiscCatchScoreboard_Pause(work->scoreboard);

    for (i = 0; i < DISC_CATCH_MONS; i++) {
        ManagedSprite_SetAnimateFlag(work->sprites.mons[i].splash, FALSE);
    }

    i = 0;
    animate0 = FALSE;
    animate1 = FALSE;
    disc = work->sprites.discs;
    for (; i < DISC_CATCH_DISCS; disc++, i++) {
        ManagedSprite **sprites = disc->disc;

        for (j = 0; j < 2; j++) {
            ManagedSprite_SetAnimateFlag(sprites[j], animate0);
            ManagedSprite_SetAnimateFlag(sprites[j + 2], animate1);
        }
    }
}

#pragma force_active on
#pragma opt_propagation off

void PokeathlonDiscCatch_LoadSpriteResources(PokeathlonDiscCatch *work) {
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    int vram;

    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 22, TRUE, vram, 10000);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 19, FALSE, 2, vram, 10000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 21, TRUE, 10000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 20, TRUE, 10000);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 26, TRUE, vram, 10001);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 23, FALSE, 4, vram, 10001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 25, TRUE, 10001);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 24, TRUE, 10001);
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 18, TRUE, vram, 10002);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 15, FALSE, 1, vram, 10002);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 18, TRUE, vram, 10003);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 15, FALSE, 1, vram, 10003);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 17, TRUE, 10002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_5, 16, TRUE, 10002);
}

#pragma opt_propagation reset
#pragma force_active reset

// A disc sprite (the top screen's template, or the touch screen's for onTouchScreen).
ManagedSprite *PokeathlonDiscCatch_NewDiscSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority, BOOL onTouchScreen) {
    ManagedSpriteTemplate topTemplate = gPokeathlonDiscCatchDiscTopSpriteTemplate;
    ManagedSpriteTemplate touchTemplate = gPokeathlonDiscCatchDiscTouchSpriteTemplate;
    ManagedSpriteTemplate *template = onTouchScreen ? &touchTemplate : &topTemplate;
    ManagedSprite *sprite;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template->x = x;
    template->y = y;
    template->animation = anim;
    template->drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, template, FX32_CONST(480));
    ManagedSprite_SetAffineOverwriteMode(sprite, 2);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

ManagedSprite *PokeathlonDiscCatch_NewEffectSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority) {
    ManagedSpriteTemplate template = gPokeathlonDiscCatchEffectSpriteTemplate;
    ManagedSprite *sprite;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.x = x;
    template.y = y;
    template.animation = anim;
    template.drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(480));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

void PokeathlonDiscCatch_InitBgs(PokeathlonDiscCatch *work) {
    BgConfig *bgConfig = work->bgConfig;
    enum HeapID heapId = work->heapId;
    GraphicsModes modes = gPokeathlonDiscCatchGraphicsModes;
    int i;

    SetBothScreensModesAndDisable(&modes);

    PokeathlonDiscCatchBgTemplates templates = gPokeathlonDiscCatchBgTemplates;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, (u8)gPokeathlonDiscCatchBgLayers[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, (u8)gPokeathlonDiscCatchBgLayers[i]);
        BG_ClearCharDataRange((u8)gPokeathlonDiscCatchBgLayers[i], 0x20, 0, heapId);
    }
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, work->heapId);
    work->messageFormat = MessageFormat_New(work->heapId);
    FontID_Alloc(4, work->heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonDiscCatch_FreeBgs(PokeathlonDiscCatch *work) {
    int i;

    RemoveWindow(&work->window);
    MessageFormat_Delete(work->messageFormat);
    DestroyMsgData(work->msgData);
    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(work->bgConfig, (u8)gPokeathlonDiscCatchBgLayers[i]);
    }
    FontID_Release(4);
    Heap_Free(work->bgConfig);
}

#pragma force_active on

void PokeathlonDiscCatch_LoadBgGraphics(PokeathlonDiscCatch *work) {
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 4, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 2, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 5, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 1, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_5, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_ADD_Y, 8);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 4, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 11, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_5, 11, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 5, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 12, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_5, 13, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_5, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
}

#pragma force_active reset

// The water of both screens' background: the next of 6 tilemaps every 4 frames.
void PokeathlonDiscCatch_AnimateBg(u8 *timer, BgConfig *bgConfig, enum HeapID heapId) {
    GF_ASSERT(timer != NULL);
    GF_ASSERT(bgConfig != NULL);

    timer[0]++;
    if (timer[0] >= 4) {
        GfGfxLoader_LoadScrnData(NARC_a_2_3_5, timer[1] + 5, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, heapId);
        GfGfxLoader_LoadScrnData(NARC_a_2_3_5, timer[1] + 5, bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, heapId);
        timer[1] = (u8)((timer[1] + 1) % 6);
        timer[0] = 0;
    }
}

#pragma force_active on

// Host: counts the time down, takes in the participants' touches, runs the field and writes the frame packet.
void PokeathlonDiscCatch_UpdateHost(PokeathlonCourseData *course) {
    PokeathlonDiscCatch *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u8 *area = PokeathlonCourse_GetDataCopyArea(course);
    int seconds, i;
    PokeathlonDiscCatchFrame *frame;
    u8 *input;
    PokeathlonDiscCatchTeam *team;

    if (PokeathlonCourse_GetNetId(course)) {
        return;
    }
    seconds = 0;
    frame = (PokeathlonDiscCatchFrame *)PokeathlonCourse_ToBytes(area + 0x28);
    if (frame->finished) {
        PokeathlonDiscCatch_WriteFramePacket(&work->field, course);
        return;
    }
    if (work->timer > 0) {
        work->timer--;
        seconds = (work->timer + 30) / 30;
        frame->seconds = seconds;
    }
    {
        PokeathlonDiscCatchFrame *dest = (PokeathlonDiscCatchFrame *)PokeathlonCourse_ToBytes(area + 0x50);
        PokeathlonDiscCatchFrame *src = (PokeathlonDiscCatchFrame *)PokeathlonCourse_ToBytes(area);
        *dest = *src;
    }
    i = 0;
    input = area + 0x50;
    team = work->field.teams;
    for (; i < DISC_CATCH_PARTICIPANTS; i++) {
        PokeathlonDiscCatchFrame *touch = (PokeathlonDiscCatchFrame *)PokeathlonCourse_ToBytes(input);
        if (touch->touching) {
            if (team->touchNew && team->touching) {
                team->touchNew = FALSE;
            } else if (!team->touchNew && !team->touching) {
                team->touchNew = TRUE;
                team->touching = TRUE;
            }
            team->touchX = touch->touchX;
            team->touchY = touch->touchY;
            team->touchFrames++;
        } else {
            team->lastTouchFrames = team->touchFrames;
            team->touchFrames = 0;
            team->touchNew = FALSE;
            team->touching = FALSE;
        }
        input += 0x28;
        team++;
    }
    PokeathlonDiscCatchCpu_Update(work->cpu);
    PokeathlonDiscCatchDiscs_Update(&work->discs, area, seconds);
    PokeathlonDiscCatch_UpdateField(work->field.teams, course);
    if (work->timer <= 0) {
        frame->finished = TRUE;
    }
    PokeathlonDiscCatch_WriteFramePacket(&work->field, course);
}

#pragma force_active reset
