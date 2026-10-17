#include "pokeathlon/pokeathlon_block_smash.h"

#include "global.h"

#include "constants/sndseq.h"

#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_resource_header.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"

enum {
    BLOCK_STATE_FALLING = 1,
    BLOCK_STATE_LANDED,
};

// One of the four weak spot positions
typedef struct PokeathlonBlockSmashSpot {
    s16 x;
    s16 y;
} PokeathlonBlockSmashSpot;

void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, int mon, int counter, int amount);
void GF_SndSetTrackPitchBySeqNo(u16 seqNo, u32 trackMask, int pitch);
ManagedSprite *PokeathlonBlockSmash_NewMainSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);
extern const TouchscreenHitbox *gPokeathlonBlockSmashSpotHitboxes[3];

static void PokeathlonBlockSmashBlocks_UpdateDrop(PokeathlonBlockSmashBlocks *blocks);
static void PokeathlonBlockSmashBlocks_CheckEmpty(PokeathlonBlockSmashBlocks *blocks);
static PokeathlonBlockSmashBlock *PokeathlonBlockSmashBlock_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, u32 index);
static void PokeathlonBlockSmashBlock_Drop(PokeathlonBlockSmashBlock *block);
static void PokeathlonBlockSmashBlock_Delete(PokeathlonBlockSmashBlock *block);
static void PokeathlonBlockSmashBlock_Update(PokeathlonBlockSmashBlock *block);
static void PokeathlonBlockSmashBlock_MoveSpot(PokeathlonBlockSmashBlock *block);
static void PokeathlonBlockSmashBlock_UpdateSpot(PokeathlonBlockSmashBlock *block);
static BOOL PokeathlonBlockSmashBlock_IsSpotTouched(PokeathlonBlockSmashBlock *block, int kind);
static void PokeathlonBlockSmashBlock_GetPosition(PokeathlonBlockSmashBlock *block, s16 *x, s16 *y);
static u32 PokeathlonBlockSmashBlock_GetState(PokeathlonBlockSmashBlock *block);
static void PokeathlonBlockSmashFragments_Init(PokeathlonBlockSmashFragment *fragments, SpriteSystem *spriteSystem, SpriteManager *spriteManager);
static void PokeathlonBlockSmashFragments_Delete(PokeathlonBlockSmashFragment *fragments);
static void PokeathlonBlockSmashFragments_Spawn(PokeathlonBlockSmashFragment *fragments, u8 count, s16 x, s16 y);
static void PokeathlonBlockSmashFragments_Update(PokeathlonBlockSmashFragment *fragments);
static void PokeathlonBlockSmashFragments_Pause(PokeathlonBlockSmashFragment *fragments);

static const u8 sPokeathlonBlockSmashSpotHitboxCounts[3] = { 3, 3, 3 };

// The touch areas of each weak spot kind, in tiles around the spot (the last entry ends the list).
const TouchscreenHitbox gPokeathlonBlockSmashSpotHitboxes_Kind0[3] = {
    { .rect = { -2, 2, -1, 1 } },
    { .rect = { -1, 1, -2, 2 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

const TouchscreenHitbox gPokeathlonBlockSmashSpotHitboxes_Kind1[3] = {
    { .rect = { -2, 2, -2, 2 } },
    { .rect = { -1, 1, -3, 3 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

const TouchscreenHitbox gPokeathlonBlockSmashSpotHitboxes_Kind2[3] = {
    { .rect = { -2, 2, -3, 3 } },
    { .rect = { -3, 3, -2, 2 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const PokeathlonBlockSmashSpot sPokeathlonBlockSmashSpotPositions[4] = {
    { -12, -12 },
    { 2, -13 },
    { -16, -4 },
    { -2, -4 },
};

static const s16 sPokeathlonBlockSmashBlockTouchOffsets[4][2] = { { -30, 12 }, { -16, -18 }, { 16, 12 }, { 30, -18 } };

PokeathlonBlockSmashBlocks *PokeathlonBlockSmashBlocks_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course) {
    PokeathlonBlockSmashBlocks *blocks;
    s32 i;
    blocks = Heap_Alloc(heapId, sizeof(PokeathlonBlockSmashBlocks));
    MI_CpuFill8(blocks, 0, sizeof(PokeathlonBlockSmashBlocks));
    blocks->state = BLOCKS_STATE_DROPPING;
    blocks->count = 0;
    blocks->spriteSystem = spriteSystem;
    blocks->spriteManager = spriteManager;
    blocks->course = course;
    blocks->hitSprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, 7, 1);
    ManagedSprite_SetDrawFlag(blocks->hitSprite, FALSE);
    blocks->missSprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, 18, 1);
    ManagedSprite_SetDrawFlag(blocks->missSprite, FALSE);
    for (i = 0; i < 10; i++) {
        blocks->blocks[i] = PokeathlonBlockSmashBlock_New(heapId, spriteSystem, spriteManager, (u16)i);
    }
    PokeathlonBlockSmashFragments_Init(blocks->fragments, spriteSystem, spriteManager);
    return blocks;
}

void PokeathlonBlockSmashBlocks_Delete(PokeathlonBlockSmashBlocks *blocks) {
    s32 i;
    GF_ASSERT(blocks != NULL);
    GF_ASSERT(blocks->hitSprite != NULL);
    Sprite_DeleteAndFreeResources(blocks->hitSprite);
    Sprite_DeleteAndFreeResources(blocks->missSprite);
    for (i = 0; i < 10; i++) {
        PokeathlonBlockSmashBlock_Delete(blocks->blocks[i]);
    }
    PokeathlonBlockSmashFragments_Delete(blocks->fragments);
    Heap_Free(blocks);
}

void PokeathlonBlockSmashBlocks_Update(PokeathlonBlockSmashBlocks *blocks) {
    GF_ASSERT(blocks != NULL);
    switch (blocks->state) {
    case BLOCKS_STATE_DROPPING:
        PokeathlonBlockSmashBlocks_UpdateDrop(blocks);
        break;
    case BLOCKS_STATE_READY:
        PokeathlonBlockSmashBlocks_CheckEmpty(blocks);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    PokeathlonBlockSmashFragments_Update(blocks->fragments);
}

void PokeathlonBlockSmashBlocks_Pause(PokeathlonBlockSmashBlocks *blocks) {
    s32 i;
    ManagedSprite_SetAnimateFlag(blocks->missSprite, FALSE);
    ManagedSprite_SetAnimateFlag(blocks->hitSprite, FALSE);
    for (i = 0; i < 10; i++) {
        if (blocks->blocks[i]->active) {
            ManagedSprite_SetAnimateFlag(blocks->blocks[i]->sprite, FALSE);
            ManagedSprite_SetAnimateFlag(blocks->blocks[i]->spot, FALSE);
        }
    }
    PokeathlonBlockSmashFragments_Pause(blocks->fragments);
}

// Whether the touch hits the weak spot of the top block (`kind`: the spot's shape).
BOOL PokeathlonBlockSmashBlocks_IsSpotTouched(PokeathlonBlockSmashBlocks *blocks, int kind) {
    GF_ASSERT(blocks != NULL);
    GF_ASSERT(blocks->count != 0);
    if (PokeathlonBlockSmashBlock_IsSpotTouched(blocks->blocks[blocks->count - 1], kind)) {
        return TRUE;
    }
    return FALSE;
}

typedef struct PokeathlonBlockSmashVec {
    fx32 x;
    fx32 y;
    fx32 z;
} PokeathlonBlockSmashVec;

// Whether the new touch is on the top block: within its height and inside the trapezoid
// sPokeathlonBlockSmashBlockTouchOffsets (offsets from the block's position).
BOOL PokeathlonBlockSmashBlocks_IsTopBlockTouched(PokeathlonBlockSmashBlocks *blocks) {
    PokeathlonBlockSmashVec points[4];
    u32 touchX;
    u32 touchY;
    s16 position[2];
    fx32 fixedX;
    fx32 fixedY;
    int i;
    fx32 leftEdgeY;
    fx32 rightEdgeY;
    float converted;
    GF_ASSERT(blocks != NULL);
    if (blocks->state == BLOCKS_STATE_DROPPING) {
        return FALSE;
    }
    GF_ASSERT(blocks->count != 0);
    PokeathlonBlockSmashBlock_GetPosition(blocks->blocks[blocks->count - 1], &position[1], &position[0]);
    System_GetTouchNewCoords(&touchX, &touchY);
    if (touchY >= position[0] - 18 && touchY <= position[0] + 12) {
        if (touchX != 0) {
            converted = (float)(touchX << FX32_SHIFT) + 0.5f;
        } else {
            converted = (float)(touchX << FX32_SHIFT) - 0.5f;
        }
        fixedX = (s32)converted;
        if (touchY != 0) {
            converted = (float)(touchY << FX32_SHIFT) + 0.5f;
        } else {
            converted = (float)(touchY << FX32_SHIFT) - 0.5f;
        }
        fixedY = (s32)converted;
        for (i = 0; i < 4; i++) {
            if (position[1] + sPokeathlonBlockSmashBlockTouchOffsets[i][0] > 0) {
                converted = (float)((position[1] + sPokeathlonBlockSmashBlockTouchOffsets[i][0]) << FX32_SHIFT) + 0.5f;
            } else {
                converted = (float)((position[1] + sPokeathlonBlockSmashBlockTouchOffsets[i][0]) << FX32_SHIFT) - 0.5f;
            }
            points[i].x = (s32)converted;
            if (position[0] + sPokeathlonBlockSmashBlockTouchOffsets[i][1] > 0) {
                converted = (float)((position[0] + sPokeathlonBlockSmashBlockTouchOffsets[i][1]) << FX32_SHIFT) + 0.5f;
            } else {
                converted = (float)((position[0] + sPokeathlonBlockSmashBlockTouchOffsets[i][1]) << FX32_SHIFT) - 0.5f;
            }
            points[i].y = (s32)converted;
        }
        leftEdgeY = FX_Div(FX_Mul(points[1].y - points[0].y, fixedX - points[0].x), points[1].x - points[0].x);
        leftEdgeY += points[0].y;
        rightEdgeY = FX_Div(FX_Mul(points[3].y - points[2].y, fixedX - points[2].x), points[3].x - points[2].x);
        rightEdgeY += points[2].y;
        if ((fixedX >= points[1].x && fixedX <= points[2].x) || (fixedY >= leftEdgeY && fixedX >= points[0].x && fixedX <= points[1].x) || (fixedY <= rightEdgeY && fixedX >= points[2].x && fixedX <= points[3].x)) {
            return TRUE;
        }
    }
    return FALSE;
}

// A tap on the top block: `power` comes off the blocks from the top down (more on the weak spot or
// a strong hit), broken blocks throw fragments. Returns the number of blocks broken; `*cleared`
// tells whether that emptied the stack.
u16 PokeathlonBlockSmashBlocks_Hit(PokeathlonBlockSmashBlocks *blocks, int member, s16 power, int hitSpot, int strongHit, BOOL *cleared) {
    int count;
    int broken = 0, i, anim;
    int x, y;

    struct {
        s16 y, x;
    } position;

    GF_ASSERT(blocks != NULL);
    if (!blocks->count) {
        return 0;
    }
    System_GetTouchNewCoords((u32 *)&x, (u32 *)&y);
    anim = 7;
    ManagedSprite_SetPositionXY(blocks->hitSprite, (s16)x, (s16)y);
    if (hitSpot || strongHit) {
        anim = 18;
    }
    ManagedSprite_SetAnim(blocks->hitSprite, anim);
    ManagedSprite_SetDrawFlag(blocks->hitSprite, TRUE);
    if (strongHit) {
        power += 150;
        PlaySE(SEQ_SE_GS_KOUKA_H);
    } else if (hitSpot) {
        power += 160;
        PlaySE(SEQ_SE_GS_KOUKA_H);
    }
    count = blocks->count;
    for (i = 0; i < count; i++) {
        PokeathlonBlockSmashBlock *block = blocks->blocks[count - (i + 1)];
        if (power <= 0) {
            break;
        }
        block->health -= power;
        if (block->health > 0) {
            break;
        }
        if (block->health <= 0) {
            blocks->count--;
            blocks->score++;
            if (blocks->score > 200) {
                blocks->score = 200;
            }
            ManagedSprite_SetAnim(block->sprite, 0);
            ManagedSprite_GetPositionXY(block->sprite, &position.x, &position.y);
            ManagedSprite_SetDrawFlag(block->spot, FALSE);
            power = -block->health;
            broken++;
            PokeathlonBlockSmashFragments_Spawn(blocks->fragments, 4, position.x, position.y);
            PokeathlonCourse_AddMonCounter(blocks->course, (u8)PokeathlonCourse_GetNetId(blocks->course), member, 3, 1);
        }
    }
    if (broken) {
        PlaySE(SEQ_SE_GS_W085);
    } else {
        PlaySE(SEQ_SE_GS_W260);
        ManagedSprite_SetPositionXY(blocks->missSprite, (s16)x, (s16)y);
        ManagedSprite_SetAnim(blocks->missSprite, 6);
        ManagedSprite_SetDrawFlag(blocks->missSprite, TRUE);
    }
    if (!broken && blocks->count) {
        PokeathlonBlockSmashBlock *block = blocks->blocks[blocks->count - 1];
        if (block->health <= 20) {
            ManagedSprite_SetAnim(block->sprite, 3);
        } else if (block->health <= 60) {
            ManagedSprite_SetAnim(block->sprite, 2);
            PlaySE(SEQ_SE_GS_W260);
            GF_SndSetTrackPitchBySeqNo(SEQ_SE_GS_W260, 0xFFFF, 64 - ((int)LCRandom() % 128));
        }
    }
    *cleared = broken > 0 && blocks->count == 0;
    return (u16)broken;
}

u32 PokeathlonBlockSmashBlocks_GetState(PokeathlonBlockSmashBlocks *blocks) {
    GF_ASSERT(blocks != NULL);
    return blocks->state;
}

u16 PokeathlonBlockSmashBlocks_GetScore(PokeathlonBlockSmashBlocks *blocks) {
    GF_ASSERT(blocks != NULL);
    return blocks->score;
}

// Drops the next block every three frames; once all ten are dropped and none is falling any more,
// the stack is ready.
static void PokeathlonBlockSmashBlocks_UpdateDrop(PokeathlonBlockSmashBlocks *blocks) {
    int i;
    u32 timer;
    if (blocks->count >= 10) {
        for (i = 0; i < 10; i++) {
            if (PokeathlonBlockSmashBlock_GetState(blocks->blocks[i]) == BLOCK_STATE_FALLING) {
                break;
            }
        }
        if (i == 10) {
            PlaySE(SEQ_SE_GS_KI_GASYAN);
            blocks->state = BLOCKS_STATE_READY;
        }
    } else {
        timer = blocks->dropTimer;
        blocks->dropTimer++;
        if (timer >= 2) {
            blocks->dropTimer = 0;
            PokeathlonBlockSmashBlock_Drop(blocks->blocks[blocks->count]);
            PlaySE(SEQ_SE_GS_KAWARATUMU);
            blocks->count++;
        }
    }
    for (i = 0; i < 10; i++) {
        PokeathlonBlockSmashBlock_Update(blocks->blocks[i]);
    }
}

// A cleared stack is rebuilt.
static void PokeathlonBlockSmashBlocks_CheckEmpty(PokeathlonBlockSmashBlocks *blocks) {
    if (blocks->count == 0) {
        blocks->state = BLOCKS_STATE_DROPPING;
    }
}

static PokeathlonBlockSmashBlock *PokeathlonBlockSmashBlock_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, u32 index) {
    PokeathlonBlockSmashBlock *block;
    u16 priority;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    block = Heap_Alloc(heapId, sizeof(PokeathlonBlockSmashBlock));
    MI_CpuFill8(block, 0, sizeof(PokeathlonBlockSmashBlock));
    block->index = index;
    priority = (u16)(22 - index * 2);
    block->spot = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, 4, priority);
    ManagedSprite_SetDrawFlag(block->spot, FALSE);
    block->sprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 120, 0, 1, (u16)(priority + 1));
    ManagedSprite_SetDrawFlag(block->sprite, FALSE);
    ManagedSprite_SetAnimateFlag(block->sprite, TRUE);
    return block;
}

static void PokeathlonBlockSmashBlock_Drop(PokeathlonBlockSmashBlock *block) {
    GF_ASSERT(block != NULL);
    block->health = 100;
    block->fallSpeed = 0;
    block->state = BLOCK_STATE_FALLING;
    block->active = TRUE;
    ManagedSprite_SetAnim(block->sprite, 1);
    ManagedSprite_SetPositionXY(block->sprite, 120, 0);
    ManagedSprite_SetDrawFlag(block->sprite, TRUE);
    PokeathlonBlockSmashBlock_MoveSpot(block);
    PokeathlonBlockSmashBlock_UpdateSpot(block);
    ManagedSprite_SetDrawFlag(block->spot, TRUE);
}

static void PokeathlonBlockSmashBlock_Delete(PokeathlonBlockSmashBlock *block) {
    GF_ASSERT(block != NULL);
    GF_ASSERT(block->sprite != NULL);
    GF_ASSERT(block->spot != NULL);
    Sprite_DeleteAndFreeResources(block->sprite);
    Sprite_DeleteAndFreeResources(block->spot);
    Heap_Free(block);
}

// Falls with increasing speed until it lands on the block below (4 pixels per block).
static void PokeathlonBlockSmashBlock_Update(PokeathlonBlockSmashBlock *block) {
    if (block->state == BLOCK_STATE_FALLING) {
        u32 bottom = 168 - block->index * 4;
        s16 x, y;
        block->fallSpeed += 20;
        ManagedSprite_OffsetPositionXY(block->sprite, 0, block->fallSpeed);
        ManagedSprite_OffsetPositionXY(block->spot, 0, block->fallSpeed);
        ManagedSprite_GetPositionXY(block->spot, &x, &y);
        ManagedSprite_GetPositionXY(block->sprite, &x, &y);
        if ((u32)y >= bottom) {
            y = bottom;
            ManagedSprite_SetPositionXY(block->sprite, x, y);
            PokeathlonBlockSmashBlock_UpdateSpot(block);
            block->state = BLOCK_STATE_LANDED;
        }
    }
}

// Moves the weak spot to one of the other three positions, with some randomness.
static void PokeathlonBlockSmashBlock_MoveSpot(PokeathlonBlockSmashBlock *block) {
    u8 slot;
    GF_ASSERT(block != NULL);
    slot = MTRandom() % 3;
    if (slot >= block->spotSlot) {
        slot++;
    }
    block->spotSlot = slot;
    block->spotX = sPokeathlonBlockSmashSpotPositions[block->spotSlot].x + (MTRandom() & 15);
    block->spotY = sPokeathlonBlockSmashSpotPositions[block->spotSlot].y + MTRandom() % 12;
}

static void PokeathlonBlockSmashBlock_UpdateSpot(PokeathlonBlockSmashBlock *block) {
    s16 position[2];
    GF_ASSERT(block != NULL);
    PokeathlonBlockSmashBlock_GetPosition(block, &position[1], &position[0]);
    position[1] += block->spotX;
    position[0] += block->spotY;
    ManagedSprite_SetPositionXY(block->spot, position[1], position[0]);
}

// The touch areas of the weak spot (gPokeathlonBlockSmashSpotHitboxes[kind], sPokeathlonBlockSmashSpotHitboxCounts[kind] entries) around it.
static BOOL PokeathlonBlockSmashBlock_IsSpotTouched(PokeathlonBlockSmashBlock *block, int kind) {
    TouchscreenHitbox hitboxes[3] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };

    struct {
        s16 x, y;
    } position;

    u32 count;
    const TouchscreenHitbox *area;
    int i;
    GF_ASSERT(kind < 3);
    area = gPokeathlonBlockSmashSpotHitboxes[kind];
    count = (u8)(sPokeathlonBlockSmashSpotHitboxCounts[kind] - 1);
    ManagedSprite_GetPositionXY(block->spot, &position.y, &position.x);
    i = 0;
    if ((s32)count > 0) {
        do {
            hitboxes[i].rect.top = position.x + area->rect.top;
            hitboxes[i].rect.bottom = position.x + area->rect.bottom;
            hitboxes[i].rect.left = position.y + area->rect.left;
            hitboxes[i].rect.right = position.y + area->rect.right;
            i++;
            area++;
        } while (i < (s32)count);
    }
    hitboxes[count].rect.top = TOUCHSCREEN_RECTLIST_END;
    if (TouchscreenHitbox_FindRectAtTouchNew(hitboxes) != -1) {
        return TRUE;
    }
    return FALSE;
}

static void PokeathlonBlockSmashBlock_GetPosition(PokeathlonBlockSmashBlock *block, s16 *x, s16 *y) {
    GF_ASSERT(block != NULL);
    ManagedSprite_GetPositionXY(block->sprite, x, y);
}

static u32 PokeathlonBlockSmashBlock_GetState(PokeathlonBlockSmashBlock *block) {
    GF_ASSERT(block != NULL);
    return block->state;
}

static void PokeathlonBlockSmashFragments_Init(PokeathlonBlockSmashFragment *fragments, SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    int i;
    for (i = 0; i < 16; i++, fragments++) {
        fragments->sprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, (u16)(i % 3 + 28), 1);
        ManagedSprite_SetDrawFlag(fragments->sprite, FALSE);
    }
}

static void PokeathlonBlockSmashFragments_Delete(PokeathlonBlockSmashFragment *fragments) {
    s32 i;
    for (i = 0; i < 16; i++, fragments++) {
        GF_ASSERT(fragments->sprite != NULL);
        Sprite_DeleteAndFreeResources(fragments->sprite);
    }
}

// Throws `count` free fragments from (x, y), alternately to the right and to the left.
static void PokeathlonBlockSmashFragments_Spawn(PokeathlonBlockSmashFragment *fragments, u8 count, s16 x, s16 y) {
    int i;
    int spawned = 0;
    for (i = 0; i < 16; i++, fragments++) {
        if (fragments->active == 0) {
            fragments->active = 1;
            fragments->x = x;
            fragments->y = y;
            fragments->velocityX = FX32_CONST(MTRandom() % 8 + 4) * (i % 2 == 0 ? 1 : -1);
            fragments->velocityY = FX32_CONST(MTRandom() % 8 + 8);
            ManagedSprite_SetPositionXY(fragments->sprite, x, y);
            ManagedSprite_ResetSpriteAnimCtrlState(fragments->sprite);
            ManagedSprite_SetDrawFlag(fragments->sprite, TRUE);
            spawned++;
        }
        if (spawned >= count) {
            break;
        }
    }
}

// TRUE once the fragment left the screen.
static BOOL PokeathlonBlockSmashFragment_Update(PokeathlonBlockSmashFragment *fragment) {
    fragment->x += (fragment->velocityX >> FX32_SHIFT);
    fragment->velocityX = (fragment->velocityX << 2) / 3;
    fragment->y -= (fragment->velocityY >> FX32_SHIFT);
    fragment->velocityY -= 4;
    ManagedSprite_SetPositionXY(fragment->sprite, fragment->x, fragment->y);
    if (fragment->x < 0 || fragment->x > 256) {
        return TRUE;
    }
    return FALSE;
}

static void PokeathlonBlockSmashFragments_Update(PokeathlonBlockSmashFragment *fragments) {
    s32 i;
    for (i = 0; i < 16; i++, fragments++) {
        if (fragments->active == 1 && PokeathlonBlockSmashFragment_Update(fragments)) {
            ManagedSprite_SetDrawFlag(fragments->sprite, FALSE);
            fragments->active = 0;
        }
    }
}

static void PokeathlonBlockSmashFragments_Pause(PokeathlonBlockSmashFragment *fragments) {
    s32 i;
    for (i = 0; i < 16; i++, fragments++) {
        if (fragments->active == 1) {
            ManagedSprite_SetAnimateFlag(fragments->sprite, FALSE);
        }
    }
}
