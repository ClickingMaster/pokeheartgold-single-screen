#include "pokeathlon/pokeathlon_block_smash_teams.h"

#include <nnsys/g2d_internal.h>

#include "global.h"

#include "constants/sndseq.h"

#include "assert.h"
#include "filesystem.h"
#include "heap.h"
#include "math_util.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Course helpers (OVY_96, not yet tidied).
u8 *ov96_021E8A20(void *block);
u8 *PokeathlonCourse_GetMonStats(PokeathlonCourseData *course, u32 participant, u32 member);
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, u8 participant, u8 member, void *out);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, u8 member, int kind, int amount);
u16 *PokeathlonCourseMonGfx_GetInfo(void *mon);
void *PokeathlonCourseMonObjects_Get(void *monPics, int index);
void *PokeathlonCourseMonObject_GetGfx(void *monPic);
void PokeathlonCourseMonObject_SetDrawFlag(void *monPic, u8 visible);
void PokeathlonCourseMonObject_SetShadowDrawFlag(void *monPic, int a1);
void PokeathlonCourseMonObject_SetDrawPriority(void *monPic, int priority);
void PokeathlonCourseMonObject_SetAnimState(void *monPic, int a1);
void PokeathlonCourseMonObject_SetAnim(void *monPic, int anim);
void ov96_021EAF94(void *monPic, int x, int y);
void ov96_021EEBE4(ManagedSprite *sprite, void *mon, int a2, int a3, enum HeapID heapId);

extern u32 ov96_0221E5A4;
extern const s32 ov96_0221CE08[3];
extern const u16 ov96_0221CDE8[3];
extern const s16 ov96_0221CDEE[3];
extern const u16 ov96_0221CDF4[5];
extern const s16 ov96_0221CDFE[5];
extern const u32 ov96_0221CE14[4];
extern const s32 ov96_0221CE24[4];
extern const u32 ov96_0221CE34[5];
extern const s32 ov96_0221CE48[5];
extern const s32 ov96_0221CE5C[10];
extern const ManagedSpriteTemplate ov96_0221CE84;
extern const ManagedSpriteTemplate ov96_0221CEB8;

// The stamina cost was called through a declaration taking the stamina as u32 and the frames as u16
// (the callers narrow the frames); the definition takes s32 and u32.
typedef u8 (*PokeathlonBlockSmashTakeStaminaFunc)(u32 stamina, u16 frames);
#define TakeStaminaAsU16 ((PokeathlonBlockSmashTakeStaminaFunc)PokeathlonBlockSmash_TakeStamina)

// The stamina recovery was called through a declaration taking the active Pokemon as u8.
typedef void (*PokeathlonBlockSmashRecoverStaminaFunc)(PokeathlonBlockSmashMember *members, u32 frames, u8 active);
#define RecoverStaminaAsU8 ((PokeathlonBlockSmashRecoverStaminaFunc)PokeathlonBlockSmash_RecoverStamina)

// The rows' sprites were made through a declaration taking x as s16.
typedef ManagedSprite *(*PokeathlonBlockSmashNewSpriteFunc)(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, int y, int anim, int priority);
#define NewMainSpriteAtS16 ((PokeathlonBlockSmashNewSpriteFunc)PokeathlonBlockSmash_NewMainSprite)

// ... and the own team's through one taking plain ints.
typedef ManagedSprite *(*PokeathlonBlockSmashNewSpriteIntFunc)(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int x, int y, int anim, int priority);
#define NewMainSpriteInt ((PokeathlonBlockSmashNewSpriteIntFunc)PokeathlonBlockSmash_NewMainSprite)
#define NewSubSpriteInt  ((PokeathlonBlockSmashNewSpriteIntFunc)PokeathlonBlockSmash_NewSubSprite)

// The ratings table (NARC a/1/7/0 file 5): by rating 0-4.
typedef struct PokeathlonBlockSmashRatingTable {
    u32 power[5];
    u32 stamina[5];
    u32 kind[5];
} PokeathlonBlockSmashRatingTable;

typedef struct PokeathlonBlockSmashAnimElement {
    u8 unk0[4];
    s16 x;
    s16 y;
} PokeathlonBlockSmashAnimElement;

#pragma force_active on

PokeathlonBlockSmashMon *PokeathlonBlockSmashMon_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course) {
    u8 participant = ov96_021E5F24(course);
    PokeathlonBlockSmashMon *mon = Heap_Alloc(heapId, sizeof(PokeathlonBlockSmashMon));
    s16 x, y, monX, monY;
    MI_CpuFill8(mon, 0, sizeof(PokeathlonBlockSmashMon));
    mon->heapId = heapId;
    mon->spriteSystem = spriteSystem;
    mon->spriteManager = spriteManager;
    mon->course = course;
    mon->gauge = 0;
    PokeathlonBlockSmash_InitMembers(mon->members, participant, course);
    PokeathlonBlockSmashMon_CreateSprites(mon->sprites, mon->spriteSystem, mon->spriteManager);
    ManagedSprite_GetPositionXYWithSubscreenOffset(mon->sprites[5], &x, &y, FX32_CONST(256));
    y -= 24;
    mon->sprites[7] = NewSubSpriteInt(spriteSystem, spriteManager, x, y, 17, 0);
    ManagedSprite_SetDrawFlag(mon->sprites[7], FALSE);
    ManagedSprite_GetPositionXYWithSubscreenOffset(mon->sprites[6], &x, &y, FX32_CONST(256));
    y -= 24;
    mon->sprites[8] = NewSubSpriteInt(spriteSystem, spriteManager, x, y, 17, 0);
    ManagedSprite_SetDrawFlag(mon->sprites[8], FALSE);
    ManagedSprite_GetPositionXY(mon->sprites[0], &monX, &monY);
    mon->sprites[1] = NewMainSpriteInt(spriteSystem, spriteManager, monX, monY, 17, 26);
    ManagedSprite_SetDrawFlag(mon->sprites[1], FALSE);
    mon->sprites[3] = NewMainSpriteInt(spriteSystem, spriteManager, monX, monY, 15, 23);
    ManagedSprite_SetDrawFlag(mon->sprites[3], FALSE);
    mon->sprites[4] = NewMainSpriteInt(spriteSystem, spriteManager, monX, monY, 27, 24);
    ManagedSprite_SetDrawFlag(mon->sprites[4], FALSE);
    monY -= 24;
    mon->sprites[2] = NewMainSpriteInt(spriteSystem, spriteManager, monX, monY, 24, 25);
    ManagedSprite_SetDrawFlag(mon->sprites[2], FALSE);
    PokeathlonBlockSmashMon_SetMonSprite(mon, 0, participant, 0, TRUE);
    PokeathlonBlockSmashMon_SetMonSprite(mon, 5, participant, 2, FALSE);
    PokeathlonBlockSmashMon_SetMonSprite(mon, 6, participant, 1, FALSE);
    return mon;
}

#pragma force_active reset

void PokeathlonBlockSmashMon_Delete(PokeathlonBlockSmashMon *mon) {
    s32 i;
    PokeathlonBlockSmashMon *cursor;
    GF_ASSERT(mon != NULL);
    i = 0;
    cursor = mon;
    for (; i < 9; i++, cursor = (PokeathlonBlockSmashMon *)((u8 *)cursor + sizeof(ManagedSprite *))) {
        ManagedSprite *sprite = cursor->sprites[0];
        if (sprite != NULL) {
            Sprite_DeleteAndFreeResources(sprite);
        }
    }
    Heap_Free(mon);
}

#pragma force_active on

// Per frame; `stackDropping`: a new stack is being dropped (no hits).
void PokeathlonBlockSmashMon_Update(PokeathlonBlockSmashMon *mon, BOOL stackDropping) {
    NNSG2dCellAnimation *cellAnim;
    PokeathlonBlockSmashAnimElement *element;

    struct {
        s16 y, x;
    } position;

    GF_ASSERT(mon != NULL);
    switch (mon->mode) {
    case BLOCK_SMASH_MON_READY:
        PokeathlonBlockSmashMon_UpdateReady(mon, stackDropping);
        break;
    case BLOCK_SMASH_MON_HIT:
        PokeathlonBlockSmashMon_UpdateHit(mon);
        break;
    case BLOCK_SMASH_MON_EXHAUSTED:
        PokeathlonBlockSmashMon_UpdateExhausted(mon);
        break;
    case BLOCK_SMASH_MON_SWITCHING:
        PokeathlonBlockSmashMon_UpdateSwitch(mon);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    ov96_0221E5A4++;
    if (stackDropping == FALSE) {
        PokeathlonBlockSmashMon_UpdateGauge(mon);
    }
    PokeathlonBlockSmashMon_UpdateTired(mon);
    RecoverStaminaAsU8(mon->members, mon->frames, mon->member);
    mon->frames++;
    if (ManagedSprite_IsAnimated(mon->sprites[0])) {
        cellAnim = Sprite_GetCellAnim(mon->sprites[0]->sprite);
        element = (PokeathlonBlockSmashAnimElement *)NNS_G2dGetAnimCtrlCurrentElement(&cellAnim->animCtrl);
        GF_ASSERT(cellAnim->animCtrl.bActive);
        ManagedSprite_GetPositionXY(mon->sprites[0], &position.x, &position.y);
        position.x += element->x;
        position.y += element->y;
        ManagedSprite_SetPositionXY(mon->sprites[1], position.x, position.y);
        position.y -= 24;
        ManagedSprite_SetPositionXY(mon->sprites[2], position.x, position.y);
    }
}

// After a hit: the stamina cost (an exhausted Pokemon stops), the gauge, the hit animation.
void PokeathlonBlockSmashMon_OnHit(PokeathlonBlockSmashMon *mon, BOOL hitSpot, u8 broken) {
    ov96_0221E5A4 = 0;
    GF_ASSERT(mon->mode != BLOCK_SMASH_MON_EXHAUSTED);
    if (mon->mode == BLOCK_SMASH_MON_HIT) {
        PokeathlonBlockSmashMember *member = &mon->members[mon->member];
        member->stamina = TakeStaminaAsU16(member->stamina, mon->modeFrames);
        if (member->stamina == 0) {
            PokeathlonBlockSmashMon_SetMode(mon, BLOCK_SMASH_MON_EXHAUSTED);
            return;
        }
    }
    if (!mon->gaugeFull) {
        PokeathlonBlockSmashMon_AddGauge(mon, hitSpot, broken);
    }
    ManagedSprite_SetAnim(mon->sprites[0], 2);
    PokeathlonCourse_AddMonCounter(mon->course, ov96_021E5F24(mon->course), mon->member, 4, 1);
    PokeathlonBlockSmashMon_SetMode(mon, BLOCK_SMASH_MON_HIT);
}

#pragma force_active reset

void PokeathlonBlockSmashMon_StartSwitch(PokeathlonBlockSmashMon *mon) {
    GF_ASSERT(mon->mode != BLOCK_SMASH_MON_SWITCHING);
    PokeathlonBlockSmashMon_SetMode(mon, BLOCK_SMASH_MON_SWITCHING);
}

void PokeathlonBlockSmashMon_SetAnimate(PokeathlonBlockSmashMon *mon, BOOL animate) {
    s32 i;
    PokeathlonBlockSmashMon *cursor = mon;
    GF_ASSERT(mon != NULL);
    for (i = 0; i < 9; i++, cursor = (PokeathlonBlockSmashMon *)((u8 *)cursor + sizeof(ManagedSprite *))) {
        if (cursor->sprites[0] != NULL) {
            ManagedSprite_SetAnimateFlag(cursor->sprites[0], animate);
        }
    }
}

u32 PokeathlonBlockSmashMon_GetMode(PokeathlonBlockSmashMon *mon) {
    GF_ASSERT(mon != NULL);
    return mon->mode;
}

#pragma force_active on

BOOL PokeathlonBlockSmashMon_CanSwitch(PokeathlonBlockSmashMon *mon) {
    BOOL result = TRUE;
    int mode = mon->mode;
    if (mode != BLOCK_SMASH_MON_READY && mode != BLOCK_SMASH_MON_HIT) {
        result = FALSE;
    }
    return result;
}

PokeathlonBlockSmashMember *PokeathlonBlockSmashMon_GetActiveMember(PokeathlonBlockSmashMon *mon) {
    GF_ASSERT(mon);
    return &mon->members[mon->member];
}

#pragma force_active reset

u32 PokeathlonBlockSmashMon_IsGaugeFull(PokeathlonBlockSmashMon *mon) {
    GF_ASSERT(mon != NULL);
    return mon->gaugeFull;
}

u32 PokeathlonBlockSmashMon_IsTired(PokeathlonBlockSmashMon *mon) {
    GF_ASSERT(mon != NULL);
    return mon->tired;
}

u16 PokeathlonBlockSmashMon_GetGauge(PokeathlonBlockSmashMon *mon) {
    GF_ASSERT(mon != NULL);
    return mon->gauge;
}

u8 PokeathlonBlockSmashMon_GetActiveMemberIndex(PokeathlonBlockSmashMon *mon) {
    GF_ASSERT(mon != NULL);
    return mon->member;
}

// Ready, or two frames into the hit animation.
u32 PokeathlonBlockSmashMon_CanHit(PokeathlonBlockSmashMon *mon) {
    u32 mode, result;
    GF_ASSERT(mon != NULL);
    result = TRUE;
    mode = mon->mode;
    if (mode != BLOCK_SMASH_MON_READY && (mode != BLOCK_SMASH_MON_HIT || mon->modeFrames < 2)) {
        result = FALSE;
    }
    return result;
}

PokeathlonBlockSmashRivals *PokeathlonBlockSmashRivals_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course, void *monPics) {
    PokeathlonBlockSmashRatingTable table;
    u32 maxStamina[3];
    PokeathlonBlockSmashRivals *rivals;
    PokeathlonBlockSmashRival *row;
    u32 self;
    s32 i;
    s32 k;
    u32 *out;
    self = (u8)ov96_021E5F24(course);
    ReadWholeNarcMemberByIdPair(&table, NARC_a_1_7_0, 5);
    rivals = Heap_Alloc(heapId, sizeof(PokeathlonBlockSmashRivals));
    MI_CpuFill8(rivals, 0, sizeof(PokeathlonBlockSmashRivals));
    rivals->heapId = heapId;
    rivals->spriteSystem = spriteSystem;
    rivals->spriteManager = spriteManager;
    rivals->course = course;
    rivals->monPics = monPics;
    PokeathlonBlockSmashRivals_InitMonPics(monPics, self);
    row = rivals->rows;
    for (i = 0; i < 4; i++) {
        if (i == self) {
            continue;
        }
        for (k = 0, out = maxStamina; k < 3; k++, out++) {
            *out = table.stamina[PokeathlonCourse_GetMonStats(course, i, k)[3]];
        }
        PokeathlonBlockSmashRival_Init(row, spriteSystem, spriteManager, rivals->monPics, (u8)i, course, maxStamina);
        row++;
    }
    return rivals;
}

void PokeathlonBlockSmashRivals_Delete(PokeathlonBlockSmashRivals *rivals) {
    s32 i;
    PokeathlonBlockSmashRivals *cursor;
    PokeathlonBlockSmashRivals *row; // row->rows[0] is the j-th row
    s32 j;
    GF_ASSERT(rivals != NULL);
    row = rivals;
    for (j = 0; j < 3; j++, row = (PokeathlonBlockSmashRivals *)((u8 *)row + sizeof(PokeathlonBlockSmashRival))) {
        for (i = 0, cursor = row; i < 6; i++, cursor = (PokeathlonBlockSmashRivals *)((u8 *)cursor + sizeof(ManagedSprite *))) {
            if (cursor->rows[0].sprites[0] != NULL) {
                Sprite_DeleteAndFreeResources(cursor->rows[0].sprites[0]);
            }
        }
    }
    Heap_Free(rivals);
}

void PokeathlonBlockSmashRivals_Update(PokeathlonBlockSmashRivals *rivals) {
    s32 i;
    PokeathlonBlockSmashRival *row;
    for (i = 0, row = rivals->rows; i < 3; i++, row++) {
        PokeathlonBlockSmashRival_Update(row);
    }
}

void PokeathlonBlockSmashRivals_Pause(PokeathlonBlockSmashRivals *rivals) {
    s32 i;
    PokeathlonBlockSmashRival *cursor;
    PokeathlonBlockSmashRival *row;
    s32 j;
    for (j = 0, row = rivals->rows; j < 3; j++, row++) {
        PokeathlonCourseMonObject_SetAnim(row->monPics[row->member], 12);
        if (row->switchTask != NULL) {
            SysTask_Destroy(row->switchTask);
            row->switchTask = NULL;
        }
        for (i = 0, cursor = row; i < 6; i++, cursor = (PokeathlonBlockSmashRival *)((u8 *)cursor + sizeof(ManagedSprite *))) {
            ManagedSprite_SetAnimateFlag(cursor->sprites[0], FALSE);
        }
    }
}

void PokeathlonBlockSmashMon_UpdateReady(PokeathlonBlockSmashMon *mon, BOOL stackDropping) {
    if (stackDropping == FALSE && !ManagedSprite_IsAnimated(mon->sprites[0])) {
        ManagedSprite_SetAnim(mon->sprites[0], 1);
    }
}

void PokeathlonBlockSmashMon_UpdateHit(PokeathlonBlockSmashMon *mon) {
    mon->modeFrames++;
    if (mon->modeFrames >= 10) {
        PokeathlonBlockSmashMon_SetMode(mon, BLOCK_SMASH_MON_READY);
    }
}

void PokeathlonBlockSmashMon_UpdateExhausted(PokeathlonBlockSmashMon *mon) {
    if (mon->members[mon->member].stamina >= 8) {
        PokeathlonBlockSmashMon_SetMode(mon, BLOCK_SMASH_MON_READY);
    }
}

#pragma force_active on

// The switch: smoke, the ball flies up, the waiting Pokemon move up, the next one comes out.
void PokeathlonBlockSmashMon_UpdateSwitch(PokeathlonBlockSmashMon *mon) {
    struct {
        s16 y, x;
    } p2, p3, p4;

    s16 delta;
    s8 member;
    ManagedSprite *tmp;
    switch (mon->switchState) {
    case 0:
        ManagedSprite_ResetSpriteAnimCtrlState(mon->sprites[3]);
        ManagedSprite_SetDrawFlag(mon->sprites[3], TRUE);
        PlaySE(SEQ_SE_GS_W100);
        mon->switchState++;
        break;
    case 1:
        if (++mon->switchFrames >= 2) {
            ManagedSprite_ResetSpriteAnimCtrlState(mon->sprites[4]);
            ManagedSprite_SetAnimateFlag(mon->sprites[4], FALSE);
            ManagedSprite_SetDrawFlag(mon->sprites[4], TRUE);
            ManagedSprite_SetDrawFlag(mon->sprites[0], FALSE);
            ManagedSprite_SetDrawFlag(mon->sprites[1], FALSE);
            ManagedSprite_SetDrawFlag(mon->sprites[2], FALSE);
            mon->switchFrames = 0;
            mon->switchState++;
        }
        break;
    case 2:
        ManagedSprite_GetPositionXY(mon->sprites[4], &p2.x, &p2.y);
        delta = (-64 - p2.y) / 4;
        if (p2.y <= -52 || delta == 0) {
            member = (s8)(mon->member - 1);
            if (member < 0) {
                member = 2;
            }
            PokeathlonBlockSmashMon_SetMonSprite(mon, 6, (u8)ov96_021E5F24(mon->course), (u8)member, FALSE);
            p2.x = 40;
            p2.y = -40;
            PokeathlonBlockSmashMon_SetWaitingPosition(mon, 6, 40, -40);
            mon->switchState++;
        } else {
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[4], p2.x, (s16)(p2.y + delta), FX32_CONST(256));
            ManagedSprite_GetPositionXYWithSubscreenOffset(mon->sprites[6], &p2.x, &p2.y, FX32_CONST(256));
            {
                int dist = 272 - p2.y;
                if (dist < 0) {
                    dist = -dist;
                }
                p2.y += dist >> 2;
            }
            p2.y = p2.y > 216 ? 216 : p2.y;
            PokeathlonBlockSmashMon_SetWaitingPosition(mon, 6, p2.x, p2.y);
        }
        break;
    case 3:
        ManagedSprite_GetPositionXYWithSubscreenOffset(mon->sprites[5], &p3.x, &p3.y, FX32_CONST(256));
        {
            int dist = 136 - p3.y;
            if (dist < 0) {
                dist = -dist;
            }
            delta = (s16)(dist >> 1);
        }
        if (p3.y < 136) {
            if (delta == 0) {
                delta = 1;
            }
            p3.y += delta;
            PokeathlonBlockSmashMon_SetWaitingPosition(mon, 5, p3.x, p3.y);
            ManagedSprite_GetPositionXYWithSubscreenOffset(mon->sprites[6], &p3.x, &p3.y, FX32_CONST(256));
            p3.y += delta;
            PokeathlonBlockSmashMon_SetWaitingPosition(mon, 6, p3.x, p3.y);
        }
        if (++mon->switchFrames >= 4) {
            tmp = mon->sprites[5];
            mon->sprites[5] = mon->sprites[6];
            mon->sprites[6] = tmp;
            tmp = mon->sprites[7];
            mon->sprites[7] = mon->sprites[8];
            mon->sprites[8] = tmp;
            PokeathlonBlockSmashMon_SetWaitingPosition(mon, 5, 40, 48);
            PokeathlonBlockSmashMon_SetWaitingPosition(mon, 6, 40, 136);
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[4], 136, -8, FX32_CONST(256));
            PlaySE(SEQ_SE_GS_NAGERU);
            mon->switchFrames = 0;
            mon->switchState++;
        }
        break;
    case 4:
        ManagedSprite_GetPositionXY(mon->sprites[4], &p4.x, &p4.y);
        {
            int dist = -24 - p4.y;
            if (dist < 0) {
                dist = -dist;
            }
            delta = (s16)dist;
        }
        if (delta != 0) {
            delta /= 2;
        }
        p4.y += delta + 2;
        if (p4.y >= 112) {
            PlaySE(SEQ_SE_GS_BOWA2);
            ManagedSprite_SetPositionXY(mon->sprites[4], 136, 112);
            ManagedSprite_SetAnimateFlag(mon->sprites[4], TRUE);
            ManagedSprite_ResetSpriteAnimCtrlState(mon->sprites[3]);
            mon->switchState++;
        } else {
            ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[4], p4.x, p4.y, FX32_CONST(256));
        }
        break;
    case 5:
        if (++mon->switchFrames >= 2) {
            ManagedSprite_SetDrawFlag(mon->sprites[4], FALSE);
            PokeathlonBlockSmashMon_SetMonSprite(mon, 0, (u8)ov96_021E5F24(mon->course), (u8)mon->member, TRUE);
            mon->switchFrames = 0;
            mon->switchState++;
        }
        break;
    case 6:
        PokeathlonBlockSmashMon_SetMode(mon, BLOCK_SMASH_MON_READY);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

#pragma force_active reset

// The tired animation of the active Pokemon and of the waiting ones (slower with less stamina).
void PokeathlonBlockSmashMon_UpdateTired(PokeathlonBlockSmashMon *mon) {
    u32 tiredAnim;
    u32 anim;
    PokeathlonBlockSmashMember *member;
    ManagedSprite *sprite;
    s32 i;
    s32 next;
    if (mon->mode == BLOCK_SMASH_MON_SWITCHING) {
        return;
    }
    for (i = 0, member = mon->members; i < 3; i++, member++) {
        if (i == mon->member) {
            tiredAnim = 26;
            anim = 24;
            sprite = mon->sprites[2];
            mon->tired = FALSE;
        } else {
            next = mon->member + 1;
            if (next >= 3) {
                next = 0;
            }
            sprite = mon->sprites[7 + (next == i)];
            anim = 17;
        }
        if (member->stamina == 0) {
            ManagedSprite_SetAnimNoRestart(sprite, tiredAnim);
            if (!ManagedSprite_GetDrawFlag(sprite)) {
                ManagedSprite_SetDrawFlag(sprite, TRUE);
            }
            if (!IsSEPlaying(SEQ_SE_GS_W146)) {
                PlaySE(SEQ_SE_GS_W146);
            }
        } else if (PokeathlonBlockSmash_SetTiredAnimSpeed(sprite, member->stamina << FX32_SHIFT, member->maxStamina << FX32_SHIFT)) {
            ManagedSprite_SetAnimNoRestart(sprite, anim);
            ManagedSprite_SetDrawFlag(sprite, TRUE);
            if (i == mon->member) {
                mon->tired = TRUE;
            }
        } else {
            ManagedSprite_SetDrawFlag(sprite, FALSE);
            ManagedSprite_SetAnimSpeed(sprite, FX32_ONE);
        }
    }
}

void PokeathlonBlockSmashRival_Init(PokeathlonBlockSmashRival *row, SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *monPics, u8 participant, PokeathlonCourseData *course, u32 *maxStamina) {
    s32 i;
    s32 x;
    u32 who;
    MI_CpuFill8(row, 0, sizeof(PokeathlonBlockSmashRival));
    who = participant;
    row->participant = who;
    row->course = course;
    row->column = who;
    if (row->participant > ov96_021E5F24(course)) {
        row->column = row->column - 1;
    }
    for (i = 0; i < 3; i++) {
        row->monPics[i] = PokeathlonCourseMonObjects_Get(monPics, (u8)(i + row->column * 3));
        row->maxStamina[i] = maxStamina[i];
    }
    ((PokeathlonBlockSmashReport *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x50 + who * 0x28))->stamina = maxStamina[0];
    x = row->column * 64 + 72;
    row->sprites[0] = NewMainSpriteAtS16(spriteSystem, spriteManager, x, 56, 12, 29);
    row->sprites[1] = NewMainSpriteAtS16(spriteSystem, spriteManager, x, 56, 5, 28);
    ManagedSprite_SetDrawFlag(row->sprites[1], FALSE);
    row->sprites[3] = NewMainSpriteAtS16(spriteSystem, spriteManager, x, 40, 16, 31);
    ManagedSprite_SetAffineOverwriteMode(row->sprites[3], 1);
    ManagedSprite_SetAffineScale(row->sprites[3], 0.7f, 0.7f);
    ManagedSprite_SetDrawFlag(row->sprites[3], FALSE);
    row->sprites[2] = NewMainSpriteAtS16(spriteSystem, spriteManager, x, 40, 24, 31);
    ManagedSprite_SetDrawFlag(row->sprites[2], FALSE);
    row->sprites[4] = NewMainSpriteAtS16(spriteSystem, spriteManager, x, 40, 14, 30);
    ManagedSprite_SetDrawFlag(row->sprites[4], FALSE);
    row->sprites[5] = NewMainSpriteAtS16(spriteSystem, spriteManager, x, 40, 27, 30);
    ManagedSprite_SetDrawFlag(row->sprites[5], FALSE);
}

void PokeathlonBlockSmashRival_ShowStaminaMark(PokeathlonBlockSmashRival *row, int anim) {
    s16 y;
    ManagedSprite *sprite;
    s16 x;
    x = (s16)(row->column * 64 + 72);
    sprite = row->sprites[2];
    y = 40;
    if (PokeathlonCourseMonGfx_GetInfo(PokeathlonCourseMonObject_GetGfx(row->monPics[row->member]))[2] != 0) {
        y -= 16;
    }
    ManagedSprite_SetPositionXY(sprite, x, y);
    ManagedSprite_SetAnimNoRestart(sprite, anim);
    ManagedSprite_SetDrawFlag(sprite, TRUE);
}

void PokeathlonBlockSmashRival_UpdateStatus(PokeathlonBlockSmashRival *row, PokeathlonBlockSmashReport *report) {
    u32 stamina;
    if (report->unk1A) {
        ManagedSprite_SetAnimNoRestart(row->sprites[3], 16);
        ManagedSprite_SetDrawFlag(row->sprites[3], TRUE);
    } else if (report->gauge >= 50) {
        PokeathlonBlockSmash_ShowGaugeEffect(row->sprites[3], (u16)report->gauge, FALSE);
    } else {
        ManagedSprite_SetDrawFlag(row->sprites[3], FALSE);
    }
    stamina = report->stamina;
    if (stamina == 0) {
        PokeathlonBlockSmashRival_ShowStaminaMark(row, 26);
        ManagedSprite_SetDrawFlag(row->sprites[3], FALSE);
        PokeathlonCourseMonObject_SetAnim(row->monPics[row->member], 12);
        return;
    }
    if (PokeathlonBlockSmash_SetTiredAnimSpeed(row->sprites[2], stamina << FX32_SHIFT, row->maxStamina[row->member] << FX32_SHIFT)) {
        PokeathlonBlockSmashRival_ShowStaminaMark(row, 24);
        PokeathlonCourseMonObject_SetAnim(row->monPics[row->member], 0);
        return;
    }
    if (report->unk1A) {
        PokeathlonCourseMonObject_SetAnim(row->monPics[row->member], 8);
        return;
    }
    ManagedSprite_SetDrawFlag(row->sprites[2], FALSE);
    PokeathlonCourseMonObject_SetAnim(row->monPics[row->member], 4);
}

// A falling block whenever the participant's score passes a multiple of 2 (anim by the score).
void PokeathlonBlockSmashRival_UpdateScore(PokeathlonBlockSmashRival *row, PokeathlonBlockSmashReport *report) {
    u32 half;
    u32 rem;
    s32 anim;
    if (row->score != report->score) {
        rem = report->score % 10;
        half = rem >> 1;
        anim = 12 - half;
        if (rem != 0) {
            ManagedSprite_SetAnimNoRestart(row->sprites[0], anim);
        } else {
            ManagedSprite_SetAnimNoRestart(row->sprites[0], 13);
        }
        ManagedSprite_SetPositionXY(row->sprites[1], row->column * 64 + 72, half * 3 + 48);
        ManagedSprite_SetAnim(row->sprites[1], 5);
        ManagedSprite_SetDrawFlag(row->sprites[1], TRUE);
        row->score = report->score;
    }
}

static inline s16 AbsS16(s32 x) {
    if (x < 0) {
        x = -x;
    }
    return x;
}

// The switch of a rival's Pokemon (the same steps as the own one, smaller).
void PokeathlonBlockSmashRival_SwitchTask(SysTask *task, void *data) {
    PokeathlonBlockSmashRival *row = data;
    ManagedSprite *sprite;
    switch (row->switchState) {
    case 0:
        sprite = row->sprites[4];
        ManagedSprite_ResetSpriteAnimCtrlState(sprite);
        ManagedSprite_SetDrawFlag(sprite, TRUE);
        row->switchState++;
        return;
    case 1:
        row->switchFrames++;
        if (row->switchFrames < 2) {
            break;
        }
        sprite = row->sprites[5];
        PokeathlonCourseMonObject_SetDrawFlag(row->monPics[row->member], FALSE);
        ManagedSprite_SetAnim(sprite, 27);
        ManagedSprite_SetAnimateFlag(sprite, FALSE);
        ManagedSprite_SetDrawFlag(sprite, TRUE);
        row->switchFrames = 0;
        row->switchState++;
        return;
    case 2: {
        s16 x, y;
        s16 step;
        ManagedSprite_GetPositionXY(row->sprites[5], &x, &y);
        step = (-64 - y) / 4;
        if (y <= -64 || step == 0) {
            row->switchState++;
            return;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(row->sprites[5], x, y + step, FX32_CONST(256));
        return;
    }
    case 3:
        row->switchFrames++;
        if (row->switchFrames < 4) {
            break;
        }
        row->switchFrames = 0;
        row->switchState++;
        return;
    case 4: {
        s16 x, y;
        ManagedSprite_GetPositionXY(row->sprites[5], &x, &y);
        {
            s16 step = AbsS16(-24 - y);
            if (step != 0) {
                step /= 2;
            }
            y += step + 2;
        }
        if (y >= 48) {
            x = 72 + row->column * 64;
            y = 48;
            ManagedSprite_SetPositionXY(row->sprites[5], x, y);
            ManagedSprite_SetAnimateFlag(row->sprites[5], TRUE);
            ManagedSprite_ResetSpriteAnimCtrlState(row->sprites[4]);
            row->switchState++;
            return;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(row->sprites[5], x, y, FX32_CONST(256));
        return;
    }
    case 5:
        row->switchFrames++;
        if (row->switchFrames < 2) {
            break;
        }
        ManagedSprite_SetDrawFlag(row->sprites[5], FALSE);
        PokeathlonCourseMonObject_SetDrawFlag(row->monPics[row->newMember], TRUE);
        PokeathlonCourseMonObject_SetShadowDrawFlag(row->monPics[row->newMember], 0);
        row->member = row->newMember;
        row->switchFrames = 0;
        row->switchState++;
        return;
    case 6:
        row->switchTask = NULL;
        row->switching = FALSE;
        SysTask_Destroy(task);
        return;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

void PokeathlonBlockSmashRival_StartSwitch(PokeathlonBlockSmashRival *row) {
    row->switchState = 0;
    row->switchFrames = 0;
    row->switching = TRUE;
    row->switchTask = SysTask_CreateOnMainQueue(PokeathlonBlockSmashRival_SwitchTask, row, 1);
}

// Follows the participant's entry in the host's frame.
void PokeathlonBlockSmashRival_Update(PokeathlonBlockSmashRival *row) {
    PokeathlonBlockSmashReport *reports;
    u32 participant;
    reports = (PokeathlonBlockSmashReport *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(row->course) + 0xF0);
    participant = row->participant;
    if (row->member == reports[participant].member) {
        PokeathlonBlockSmashRival_UpdateStatus(row, &reports[participant]);
    } else {
        ManagedSprite_SetDrawFlag(row->sprites[2], FALSE);
        ManagedSprite_SetDrawFlag(row->sprites[3], FALSE);
    }
    PokeathlonBlockSmashRival_UpdateScore(row, &reports[participant]);
    if (row->switching == FALSE && row->member != reports[participant].member) {
        GF_ASSERT(reports[participant].member < 3);
        row->newMember = reports[participant].member;
        PokeathlonBlockSmashRival_StartSwitch(row);
    }
}

// The active Pokemon and the two waiting ones.
void PokeathlonBlockSmashMon_CreateSprites(ManagedSprite **sprites, SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    ManagedSpriteTemplate template = { 0 };
    s32 i;
    int y;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = 10002;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = 10003;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = 10002;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = 10002;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.x = 136;
    template.y = 112;
    template.drawPriority = 27;
    template.bgPriority = 2;
    sprites[0] = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(256));
    ManagedSprite_SetAnimateFlag(sprites[0], TRUE);
    for (i = 0, y = 48; i < 2; i++, y += 88, sprites++) {
        template.resIdList[GF_GFX_RES_TYPE_CHAR] = 10003 + i;
        template.resIdList[GF_GFX_RES_TYPE_PLTT] = 10004 + i;
        template.resIdList[GF_GFX_RES_TYPE_CELL] = 10002;
        template.resIdList[GF_GFX_RES_TYPE_ANIM] = 10002;
        template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        template.x = 40;
        template.y = (s16)y;
        template.drawPriority = i + 1;
        sprites[5] = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(256));
        ManagedSprite_SetAnimateFlag(sprites[5], TRUE);
    }
}

void PokeathlonBlockSmashMon_SetMonSprite(PokeathlonBlockSmashMon *mon, int sprite, u8 participant, u8 member, BOOL a4) {
    u32 info[4];
    enum HeapID heapId;
    ManagedSprite *managedSprite;
    heapId = mon->heapId;
    managedSprite = mon->sprites[sprite];
    PokeathlonCourse_GetMonInfo(mon->course, participant, member, info);
    ov96_021EEBE4(managedSprite, info, a4, 0, heapId);
}

void PokeathlonBlockSmashMon_SetMode(PokeathlonBlockSmashMon *mon, int mode) {
    s32 gauge;
    switch (mode) {
    case BLOCK_SMASH_MON_EXHAUSTED:
        PokeathlonCourse_AddMonCounter(mon->course, (u8)ov96_021E5F24(mon->course), (u8)mon->member, 1, 1);
        mon->members[mon->member].stamina = 0;
        mon->frames = 0;
        break;
    case BLOCK_SMASH_MON_SWITCHING:
        PokeathlonCourse_AddMonCounter(mon->course, (u8)ov96_021E5F24(mon->course), (u8)mon->member, 7, 1);
        mon->switchFrames = 0;
        gauge = mon->gauge - 10;
        if (gauge > 100) {
            gauge = 100;
        } else if (gauge < 0) {
            gauge = 0;
        }
        mon->gauge = gauge;
        if (mon->gaugeFull) {
            PokeathlonBlockSmashMon_EndFullGauge(mon);
        }
        mon->member = mon->member + 1;
        if (mon->member >= 3) {
            mon->member = 0;
        }
        break;
    }
    mon->mode = mode;
    mon->switchState = 0;
    mon->modeFrames = 0;
}

// The gauge empties by 1 every 2 frames; a full one lasts 90 frames.
void PokeathlonBlockSmashMon_UpdateGauge(PokeathlonBlockSmashMon *mon) {
    if (mon->mode == BLOCK_SMASH_MON_SWITCHING) {
        return;
    }
    if (mon->gaugeFull) {
        mon->gaugeTimer++;
        if (mon->gaugeTimer >= 90) {
            PokeathlonBlockSmashMon_EndFullGauge(mon);
        }
    } else if (mon->gauge != 0) {
        mon->gaugeTimer++;
        if (mon->gaugeTimer >= 2) {
            mon->gaugeTimer = 0;
            mon->gauge--;
        }
        if (mon->gauge >= 50) {
            PokeathlonBlockSmash_ShowGaugeEffect(mon->sprites[1], mon->gauge, TRUE);
        }
    } else {
        ManagedSprite_SetDrawFlag(mon->sprites[1], FALSE);
    }
}

void PokeathlonBlockSmashMon_AddGauge(PokeathlonBlockSmashMon *mon, BOOL hitSpot, u8 broken) {
    GF_ASSERT(!mon->gaugeFull);
    if (hitSpot) {
        mon->gauge += 30;
    }
    if (mon->gauge > 100) {
        mon->gauge = 100;
    }
    if (mon->gauge >= 100) {
        PokeathlonBlockSmashMon_StartFullGauge(mon);
    }
}

// Slows the animation down with the stamina left; TRUE (tired) below 40 %.
BOOL PokeathlonBlockSmash_SetTiredAnimSpeed(ManagedSprite *sprite, fx32 stamina, fx32 maxStamina) {
    s32 speed;
    s32 percent = FX_Div(stamina, maxStamina) * 100 >> FX32_SHIFT;
    if (percent <= 40) {
        speed = (100 - percent) * 2;
        ManagedSprite_SetAnimSpeed(sprite, FX32_CONST(speed) / 100);
        return TRUE;
    }
    return FALSE;
}

void PokeathlonBlockSmashMon_StartFullGauge(PokeathlonBlockSmashMon *mon) {
    mon->gaugeFull = TRUE;
    mon->gaugeTimer = 0;
    ManagedSprite_SetAnim(mon->sprites[1], 16);
    ManagedSprite_SetAnimSpeed(mon->sprites[1], FX32_ONE);
    PlaySE(SEQ_SE_GS_186_2);
}

void PokeathlonBlockSmashMon_EndFullGauge(PokeathlonBlockSmashMon *mon) {
    mon->gaugeFull = FALSE;
    mon->gaugeTimer = 0;
    mon->gauge = 0;
    mon->members[mon->member].stamina = 10;
    ManagedSprite_SetDrawFlag(mon->sprites[1], FALSE);
}

// Places the other teams' Pokemon pictures (three per row, the first one shown).
void PokeathlonBlockSmashRivals_InitMonPics(void *monPics, u32 self) {
    void *pic;
    u32 base;
    s32 i;
    int x;
    s32 j;
    for (j = 0, base = 0, x = 72; j < 4; j++) {
        if (j == self) {
            continue;
        }
        for (i = 0; i < 3; i++) {
            pic = PokeathlonCourseMonObjects_Get(monPics, (u8)(i + base));
            PokeathlonCourseMonObject_SetDrawPriority(pic, 32);
            ov96_021EAF94(pic, x, 48);
            PokeathlonCourseMonObject_SetAnimState(pic, 2);
            PokeathlonCourseMonObject_SetDrawFlag(pic, (u8)(i == 0));
            PokeathlonCourseMonObject_SetShadowDrawFlag(pic, 0);
        }
        base += 3;
        x += 64;
    }
}

// From a gauge of 50 on, faster with a fuller gauge.
void PokeathlonBlockSmash_ShowGaugeEffect(ManagedSprite *sprite, u16 gauge, BOOL playSE) {
    s32 speed = 0;
    s32 over = gauge - 50;
    if (over) {
        speed = (over << FX32_SHIFT) / 10;
    }
    if (speed < FX32_CONST(0.6)) {
        speed = FX32_CONST(0.6);
    }
    ManagedSprite_SetAnimSpeed(sprite, speed);
    ManagedSprite_SetAnimNoRestart(sprite, 17);
    if (playSE && !ManagedSprite_GetDrawFlag(sprite)) {
        PlaySE(SEQ_SE_GS_W082);
    }
    ManagedSprite_SetDrawFlag(sprite, TRUE);
}

// A waiting Pokemon and its mark above it.
void PokeathlonBlockSmashMon_SetWaitingPosition(PokeathlonBlockSmashMon *mon, int sprite, s16 x, s16 y) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(mon->sprites[sprite], x, y, FX32_CONST(256));
    ManagedSprite_SetPositionXYWithSubscreenOffset((&mon->sprites[2])[sprite], x, y - 24, FX32_CONST(256));
}

#pragma force_active on

// Only the host plays the participants without a player.
PokeathlonBlockSmashCpu *PokeathlonBlockSmashCpu_New(PokeathlonCourseData *course, enum HeapID heapId) {
    u32 i;
    PokeathlonBlockSmashCpuPlayer *player;
    int participant = PokeathlonCourse_GetParticipantCount(course);
    PokeathlonBlockSmashCpu *cpu = Heap_Alloc(heapId, sizeof(PokeathlonBlockSmashCpu));
    MI_CpuFill8(cpu, 0, sizeof(PokeathlonBlockSmashCpu));
    cpu->heapId = heapId;
    cpu->course = course;
    cpu->count = 4 - participant;
    GF_ASSERT(cpu->count <= 3);
    i = 0;
    if (i < cpu->count) {
        player = cpu->players;
        do {
            PokeathlonBlockSmashCpuPlayer_Init(player, participant, course);
            participant++;
            i++;
            player++;
        } while (i < cpu->count);
    }
    return cpu;
}

#pragma force_active reset

void PokeathlonBlockSmashCpu_Delete(PokeathlonBlockSmashCpu *cpu) {
    Heap_Free(cpu);
}

#pragma force_active on

void PokeathlonBlockSmashCpu_Update(PokeathlonBlockSmashCpu *cpu) {
    u32 i = 0;
    PokeathlonBlockSmashCpuPlayer *player;
    if (i < cpu->count) {
        player = cpu->players;
        do {
            PokeathlonBlockSmashCpuPlayer_WriteReport(player, PokeathlonBlockSmashCpuPlayer_Update(player), cpu->course);
            i++;
            player++;
        } while (i < cpu->count);
    }
}

void PokeathlonBlockSmashCpu_WriteReports(PokeathlonBlockSmashCpu *cpu) {
    u32 i = 0;
    PokeathlonBlockSmashCpuPlayer *player;
    if (i < cpu->count) {
        player = cpu->players;
        do {
            PokeathlonBlockSmashCpuPlayer_WriteReport(player, 0, cpu->course);
            i++;
            player++;
        } while (i < cpu->count);
    }
}

#pragma force_active reset

void PokeathlonBlockSmashCpuPlayer_Init(PokeathlonBlockSmashCpuPlayer *player, u8 participant, PokeathlonCourseData *course) {
    MI_CpuFill8(player, 0, sizeof(*player));
    player->course = course;
    player->participant = participant;
    player->blockHealth = 100;
    PokeathlonBlockSmash_InitMembers(player->members, participant, course);
}

// Returns 2 when the CPU hit the weak spot this frame.
u32 PokeathlonBlockSmashCpuPlayer_Update(PokeathlonBlockSmashCpuPlayer *player) {
    u32 result = 0;
    u32 mode;
    GF_ASSERT(player != NULL);
    mode = player->mode;
    switch (mode) {
    case BLOCK_SMASH_CPU_READY:
        result = PokeathlonBlockSmashCpuPlayer_UpdateReady(player);
        break;
    case BLOCK_SMASH_CPU_SWITCHING:
        result = PokeathlonBlockSmashCpuPlayer_UpdateSwitch(player);
        break;
    case BLOCK_SMASH_CPU_EXHAUSTED:
        result = PokeathlonBlockSmashCpuPlayer_UpdateExhausted(player);
        break;
    case BLOCK_SMASH_CPU_NEW_STACK:
        result = PokeathlonBlockSmashCpuPlayer_UpdateNewStack(player);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    if (player->mode != BLOCK_SMASH_CPU_NEW_STACK) {
        PokeathlonBlockSmashCpuPlayer_UpdateGauge(player);
    }
    RecoverStaminaAsU8(player->members, player->frames, player->member);
    player->frames++;
    return result;
}

// Every 4-6 frames: switch when tired, else hit (on the weak spot by chance).
u32 PokeathlonBlockSmashCpuPlayer_UpdateReady(PokeathlonBlockSmashCpuPlayer *player) {
    u32 result = 0;
    player->modeFrames++;
    if (player->modeFrames >= player->wait) {
        if (PokeathlonBlockSmashCpuPlayer_DecideSwitch(player) != 0) {
            PokeathlonBlockSmashCpuPlayer_EndFullGauge(player);
            PokeathlonCourse_AddMonCounter(player->course, player->participant, player->member, 7, 1);
            PokeathlonBlockSmashCpuPlayer_SetMode(player, BLOCK_SMASH_CPU_SWITCHING);
        } else {
            if (PokeathlonBlockSmashCpuPlayer_DecideSpotHit(player) != 0) {
                result += 2;
            }
            if (player->recentlyHit != 0) {
                if (player->sinceHit >= 2 && PokeathlonBlockSmashCpuPlayer_DecideHitAgain(player) != 0) {
                    PokeathlonBlockSmashCpuPlayer_Hit(player, result & 2);
                }
            } else {
                PokeathlonBlockSmashCpuPlayer_Hit(player, result & 2);
            }
        }
        player->modeFrames = 0;
        player->wait = MTRandom() % 3 + 4;
    }
    if (player->recentlyHit != 0) {
        player->sinceHit++;
        if (player->sinceHit >= 10) {
            player->sinceHit = 0;
            player->recentlyHit = 0;
        }
    }
    return result;
}

u32 PokeathlonBlockSmashCpuPlayer_UpdateSwitch(PokeathlonBlockSmashCpuPlayer *player) {
    player->modeFrames++;
    if (player->modeFrames >= 26) {
        PokeathlonBlockSmashCpuPlayer_SetMode(player, BLOCK_SMASH_CPU_READY);
    }
    return 0;
}

u32 PokeathlonBlockSmashCpuPlayer_UpdateNewStack(PokeathlonBlockSmashCpuPlayer *player) {
    player->modeFrames++;
    if (player->modeFrames >= 42) {
        PokeathlonBlockSmashCpuPlayer_SetMode(player, BLOCK_SMASH_CPU_READY);
    }
    return 0;
}

u32 PokeathlonBlockSmashCpuPlayer_UpdateExhausted(PokeathlonBlockSmashCpuPlayer *player) {
    if (player->members[player->member].stamina >= 8) {
        PokeathlonBlockSmashCpuPlayer_SetMode(player, BLOCK_SMASH_CPU_READY);
    }
    return 0;
}

#pragma force_active on

// Hitting again right after a hit: more likely with more stamina and a fuller gauge.
BOOL PokeathlonBlockSmashCpuPlayer_DecideHitAgain(PokeathlonBlockSmashCpuPlayer *player) {
    int chance = 0;
    u8 roll = MTRandom() % 100;
    int i;
    GF_ASSERT(player != NULL);
    for (i = 0; i < 5; i++) {
        if (player->members[player->member].stamina >= ov96_0221CE34[i]) {
            chance += ov96_0221CE48[i];
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (player->gauge >= ov96_0221CE14[i]) {
            chance += ov96_0221CE24[i];
            break;
        }
    }
    if (roll < chance) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

// The chance to hit the weak spot depends on the Pokemon's spot kind (ov96_0221CE08).
u32 PokeathlonBlockSmashCpuPlayer_DecideSpotHit(PokeathlonBlockSmashCpuPlayer *player) {
    s32 chance = 0;
    u8 roll = (u8)(MTRandom() % 100);
    u8 kind = (u8)player->members[player->member].kind;
    GF_ASSERT(kind < 3);
    chance += ov96_0221CE08[kind];
    if (roll < chance) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// Switching: more likely with less stamina and a fuller gauge.
BOOL PokeathlonBlockSmashCpuPlayer_DecideSwitch(PokeathlonBlockSmashCpuPlayer *player) {
    int chance = 0;
    u8 roll = MTRandom() % 100;
    int i;
    GF_ASSERT(player != NULL);
    for (i = 0; i < 5; i++) {
        if (player->members[player->member].stamina < ov96_0221CDF4[i]) {
            chance += ov96_0221CDFE[i];
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (player->gauge >= ov96_0221CDE8[i]) {
            chance += ov96_0221CDEE[i];
            break;
        }
    }
    if (roll < chance) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void PokeathlonBlockSmashCpuPlayer_SetMode(PokeathlonBlockSmashCpuPlayer *player, int mode) {
    if (mode != BLOCK_SMASH_CPU_SWITCHING) {
        if (mode == BLOCK_SMASH_CPU_EXHAUSTED) {
            player->frames = 0;
        }
    } else {
        int gauge = player->gauge - 10;
        if (gauge > 100) {
            gauge = 100;
        } else if (gauge < 0) {
            gauge = 0;
        }
        player->gauge = gauge;
        player->member++;
        if (player->member >= 3) {
            player->member = 0;
        }
    }
    player->mode = mode;
    player->modeFrames = 0;
}

#pragma force_active on

// Like PokeathlonBlockSmashBlocks_Hit for the CPU: the power goes into blocks of 100 health.
void PokeathlonBlockSmashCpuPlayer_Hit(PokeathlonBlockSmashCpuPlayer *player, int hitSpot) {
    u16 broken = 0;
    s16 power;
    PokeathlonBlockSmashMember *members;
    u32 member;
    GF_ASSERT(player);
    members = player->members;
    member = player->member;
    power = members[member].power;
    if (hitSpot) {
        power = (s16)(power + 160);
    }
    while (1) {
        player->blockHealth -= power;
        if (player->blockHealth > 0) {
            break;
        }
        power = (s16)-player->blockHealth;
        player->blockHealth = 100;
        PokeathlonCourse_AddMonCounter(player->course, (u8)player->participant, (u8)player->member, 3, 1);
        broken++;
    }
    player->score += broken;
    if (!player->gaugeFull) {
        if (player->recentlyHit) {
            members[member].stamina = TakeStaminaAsU16(members[member].stamina, (u16)player->sinceHit);
            if (!members[member].stamina) {
                PokeathlonCourse_AddMonCounter(player->course, (u8)player->participant, (u8)player->member, 1, 1);
                PokeathlonBlockSmashCpuPlayer_SetMode(player, BLOCK_SMASH_CPU_EXHAUSTED);
                return;
            }
        }
        PokeathlonBlockSmashCpuPlayer_AddGauge(player, hitSpot, broken);
    }
    if (broken && player->score && player->score % 10 == 0) {
        PokeathlonBlockSmashCpuPlayer_SetMode(player, BLOCK_SMASH_CPU_NEW_STACK);
    }
    player->recentlyHit = 1;
    player->sinceHit = 0;
    PokeathlonCourse_AddMonCounter(player->course, (u8)player->participant, (u8)player->member, 4, 1);
}

// The CPU participant's report in its link data block (written by the host).
void PokeathlonBlockSmashCpuPlayer_WriteReport(PokeathlonBlockSmashCpuPlayer *player, u32 result, PokeathlonCourseData *course) {
    PokeathlonBlockSmashReport *report = (PokeathlonBlockSmashReport *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(course) + 0x50 + player->participant * 0x28);
    PokeathlonBlockSmashMember *member = &player->members[player->member];
    report->score = player->score;
    report->stamina = member->stamina;
    report->gauge = player->gauge;
    report->unk1A = player->gaugeFull;
    report->switchingFrame = result & 2;
    report->member = player->member;
    report->switching = player->mode == BLOCK_SMASH_CPU_SWITCHING;
}

#pragma force_active reset

void PokeathlonBlockSmashCpuPlayer_AddGauge(PokeathlonBlockSmashCpuPlayer *player, int hitSpot, u32 broken) {
    GF_ASSERT(!player->gaugeFull);
    if (hitSpot) {
        player->gauge += 30;
    }
    if (player->gauge > 100) {
        player->gauge = 100;
    }
    if (player->gauge >= 100) {
        player->gaugeFull = TRUE;
        player->gaugeTimer = 0;
    }
}

void PokeathlonBlockSmashCpuPlayer_UpdateGauge(PokeathlonBlockSmashCpuPlayer *player) {
    if (player->gaugeFull) {
        player->gaugeTimer++;
        if (player->gaugeTimer >= 90) {
            PokeathlonBlockSmashCpuPlayer_EndFullGauge(player);
        }
    } else if (player->gauge != 0) {
        player->gaugeTimer++;
        if (player->gaugeTimer >= 2) {
            player->gaugeTimer = 0;
            player->gauge--;
        }
    }
}

#pragma force_active on

void PokeathlonBlockSmashCpuPlayer_EndFullGauge(PokeathlonBlockSmashCpuPlayer *player) {
    PokeathlonBlockSmashMember *member = &player->members[player->member];
    player->gaugeFull = FALSE;
    player->gaugeTimer = 0;
    player->gauge = 0;
    member->stamina = 10;
}

#pragma force_active reset

// The touch screen's sprites (resources 10000).
ManagedSprite *PokeathlonBlockSmash_NewMainSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority) {
    ManagedSpriteTemplate template = ov96_0221CE84;
    ManagedSprite *sprite;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.x = x;
    template.y = y;
    template.animation = anim;
    template.drawPriority = priority;
    template.bgPriority = 1;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(256));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

// The top screen's sprites (resources 10001).
ManagedSprite *PokeathlonBlockSmash_NewSubSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority) {
    ManagedSpriteTemplate template = ov96_0221CEB8;
    ManagedSprite *sprite;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    template.x = x;
    template.y = y;
    template.animation = anim;
    template.drawPriority = priority;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &template, FX32_CONST(256));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

// The team's power, stamina and weak spot kind from the Pokemon's ratings (NARC a/1/7/0 file 5:
// 5 values each).
void PokeathlonBlockSmash_InitMembers(PokeathlonBlockSmashMember *members, u32 participant, PokeathlonCourseData *course) {
    u32 table[15];
    int i;
    u8 *ratings;
    ReadWholeNarcMemberByIdPair(table, NARC_a_1_7_0, 5);
    for (i = 0; i < 3; i++) {
        ratings = PokeathlonCourse_GetMonStats(course, participant, i);
        GF_ASSERT(ratings[0] < 5);
        GF_ASSERT(ratings[3] < 5);
        GF_ASSERT(ratings[4] < 5);
        members->power = table[ratings[0]];
        members->maxStamina = table[5 + ratings[3]];
        members->kind = table[10 + ratings[4]];
        members->stamina = members->maxStamina;
        members++;
    }
}

#pragma force_active on

// Every 30 frames +5 stamina for the waiting Pokemon, every 60 frames +8 for the active one.
void PokeathlonBlockSmash_RecoverStamina(PokeathlonBlockSmashMember *members, u32 frames, int active) {
    int i = 0;
    u32 every60 = frames % 60, every30 = frames % 30;
    for (; i < 3; i++, members++) {
        u16 stamina = members->stamina, maxStamina = members->maxStamina;
        if (frames != 0 && stamina < maxStamina) {
            if (i == active) {
                if (every60 == 0) {
                    members->stamina += 8;
                }
            } else if (every30 == 0) {
                members->stamina += 5;
            }
            if (members->stamina > maxStamina) {
                members->stamina = maxStamina;
            }
        }
    }
}

#pragma force_active reset

// A hit's stamina cost (ov96_0221CE5C, by the frames since the last hit: faster hits cost more).
s32 PokeathlonBlockSmash_TakeStamina(s32 stamina, u32 frames) {
    s32 result;
    GF_ASSERT(frames < 10);
    result = stamina - ov96_0221CE5C[frames];
    if (result <= 0) {
        result = 0;
    }
    return result;
}
