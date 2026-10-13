#include "global.h"

#include "constants/sndseq.h"

#include "application/footprint_stamp.h"
#include "msgdata/msg/msg_0754.h"

#include "math_util.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "sound_playback.h"
#include "text.h"
#include "touchscreen.h"

// Declared only in other modules' private headers (src/model_animation.c, ov45).
void ModelResource_Free(FootprintStampModel *model);
void ModelInstance_Init(FootprintStampModelInstance *instance, FootprintStampModel *model);
void ModelInstance_Draw(FootprintStampModelInstance *instance);
void ModelInstance_SetVisible(FootprintStampModelInstance *instance, BOOL visible);
void ModelInstance_SetPosition(FootprintStampModelInstance *instance, fx32 x, fx32 y, fx32 z);
void ModelInstance_GetPosition(FootprintStampModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
void ModelInstance_SetScale(FootprintStampModelInstance *instance, fx32 x, fx32 y, fx32 z);
void ModelInstance_GetScale(FootprintStampModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
void ModelInstance_SetRotation(FootprintStampModelInstance *instance, u16 rotation, int axis);
u16 ModelInstance_GetRotation(FootprintStampModelInstance *instance, int axis);
int WiFiPlazaLobbyClient_IdToSlot(u32 playerId);
int WiFiPlazaLobbyClient_IdToChannelSlot(u32 playerId);
void *WiFiPlazaWork_GetPlayer(void *lobby, int slot);
void WiFiPlazaPlayer_CopyToProfile(void *record, PlayerProfile *profile, enum HeapID heapId);
int WiFiPlazaWork_IsSpecialVisitor(void *lobby, int slot);
BOOL WiFiPlaza_HasConnectionFailed(void);
int WiFiPlazaLobbyClient_SetChannelCommands(const void *commands, u32 count, void *data);
void WiFiPlazaLobbyClient_SendToChannel(int command, void *data, int size);
// Runtime 64-bit multiply, called by name in FootprintStamp_GetBounds (a plain `*` orders the operands differently).
s64 _ll_mul(s64 a, s64 b);

// Board position of the touch grid lines (x every 16 pixels, y every 16 pixels).
extern const fx32 sFootprintStampBoardXs[17];
extern const fx32 sFootprintStampBoardYs[11];
// Bounds of the three footprint sizes, relative to the stamp position (one table split over four labels).
extern const fx32 sFootprintStampSizeBoundsLeft[];
extern const fx32 sFootprintStampSizeBoundsRight[];
extern const fx32 sFootprintStampSizeBoundsBottom[];
extern const fx32 sFootprintStampSizeBoundsTop[];
extern const s16 sFootprintStampCameraChainLengths[];    // chain length that moves the camera, by player count
extern const u8 sFootprintStampNatureEffects[];     // enum FootprintStampEffect by nature
extern const u16 sFootprintStampInkColors[][2]; // ink colors by OT id % 9, two columns
extern const TouchscreenHitbox sFootprintStampPartyHitboxes[];
extern const u8 sFootprintStampSpeciesFootprints[]; // per species: 4 bytes, [0] footprint type, [1] footprint size
extern const u8 sFootprintStampSpeciesFootprintSizes[];
extern const u8 sFootprintStampLinkCommands[]; // link command table: player joined, player left, stamp received

#pragma force_active on

// Rotates the 16x16 2bpp footprint texture a quarter turn (direction 3: one way, 4: the other).
void FootprintStamp_RotateFootprintTexture(NNSG3dResTex *texture, int direction) {
    u32 offset = texture->texInfo.ofsTex;
    u32 *copy = Heap_Alloc(HEAP_ID_125, 0x40);
    int i, j, shift;
    u32 *src;
    u32 *dst;

    MI_CpuCopy16((u8 *)texture + offset, copy, 0x40);
    MI_CpuClear16((u8 *)texture + offset, 0x40);
    switch (direction) {
    case 3:
        src = copy;
        shift = 30;
        i = 0;
        for (; i < 16; i++, shift -= 2, src++) {
            j = 0;
            dst = (u32 *)((u8 *)texture + offset);
            for (; j < 32; j += 2) {
                *dst |= ((*src >> j) & 3) << shift;
                dst++;
            }
        }
        break;
    case 4: {
        u32 *in;
        u32 *out;
        in = copy;
        for (shift = 0, i = 0; i < 16; i++, shift += 2, in++) {
            j = 0;
            out = (u32 *)((u8 *)texture + offset) + 15;
            for (; j < 32; j += 2) {
                *out |= ((*in >> j) & 3) << shift;
                out--;
            }
        }
    } break;
    }
    Heap_Free(copy);
}

#pragma force_active reset

void FootprintStamp_DeleteStamp(FootprintStampObj *stamp) {
    ModelResource_Free(&stamp->model);
    Heap_Free(stamp);
}

void FootprintStampBoard_RemoveStamp(FootprintStampBoard *board, FootprintStampObj *stamp, int slot) {
    if (stamp->ready == FALSE || stamp->textureChanged == TRUE) {
        FootprintStampBoard_UnqueueTexture(board, stamp->model.texture);
    }
    FootprintStampBoard_ReleaseChain(board, stamp->chain);
    FootprintStamp_DeleteStamp(stamp);
    board->stamps[slot] = NULL;
}

void FootprintStamp_TouchToBoardPos(void *unused, int x, int y, fx32 *outX, fx32 *outY) {
    int i;

    (void)unused;
    i = x / 16;
    if ((u32)i >= 17) {
        i = 16;
    }
    *outX = sFootprintStampBoardXs[i];
    if ((u32)i < 16) {
        *outX += ((sFootprintStampBoardXs[i + 1] - sFootprintStampBoardXs[i]) / 16) * (x % 16);
    }
    i = y / 16;
    if ((u32)i >= 11) {
        i = 10;
    }
    *outY = sFootprintStampBoardYs[i];
    if ((u32)i < 10) {
        *outY += ((sFootprintStampBoardYs[i + 1] - sFootprintStampBoardYs[i]) / 16) * (y % 16);
    }
}

// Polygon ids 1-62 for the translucent effects (stamps drawn with the same id do not blend with each other).
int FootprintStampBoard_AllocPolygonId(FootprintStampBoard *board) {
    int id = 1;
    int one = 1;
    u32 used = board->polygonIds[0];

    do {
        u32 mask = one << id;
        if (!(used & mask)) {
            board->polygonIds[0] |= mask;
            return id;
        }
        id++;
    } while (id < 32);
    {
        int bit = 0;
        if (id < 63) {
            int one2 = 1;
            u32 used2 = board->polygonIds[1];
            do {
                u32 mask = one2 << bit;
                if (!(used2 & mask)) {
                    board->polygonIds[1] |= mask;
                    return id;
                }
                id++;
                bit++;
            } while (id < 63);
        }
    }
    return 0xFF;
}

void FootprintStampBoard_FreePolygonId(FootprintStampBoard *board, int id) {
    int one = 1;

    if (id == 0xFF) {
        return;
    }
    if (id < 32) {
        u32 mask = one << id;
        board->polygonIds[0] = (-1 ^ mask) & board->polygonIds[0];
    } else {
        u32 mask = one << (id - 32);
        board->polygonIds[1] = (-1 ^ mask) & board->polygonIds[1];
    }
}

#pragma force_active on

void FootprintStamp_GetBounds(FootprintStampModelInstance *instance, FootprintStampMon *mon, FootprintStampBounds *bounds, BOOL scaled) {
    u32 size = FootprintStamp_GetFootprintSize(mon->species, mon->form);
    fx32 x, y, z, scaleX, scaleY, scaleZ, length;
    s64 grow;

    GF_ASSERT(size < 3);
    ModelInstance_GetPosition(instance, &x, &y, &z);
    bounds->left = x + sFootprintStampSizeBoundsLeft[size * 4];
    bounds->right = x + sFootprintStampSizeBoundsRight[size * 4];
    bounds->bottom = y + sFootprintStampSizeBoundsBottom[size * 4];
    bounds->top = y + sFootprintStampSizeBoundsTop[size * 4];
    if (scaled == TRUE) {
        ModelInstance_GetScale(instance, &scaleX, &scaleY, &scaleZ);
        length = bounds->right - bounds->left;
        grow = (_ll_mul((s64)scaleX * 100 / FX32_ONE, length) / 100 - length) / 2;
        bounds->right += grow;
        bounds->left -= grow;
        length = bounds->bottom - bounds->top;
        grow = (_ll_mul((s64)scaleY * 100 / FX32_ONE, length) / 100 - length) / 2;
        bounds->bottom += grow;
        bounds->top -= grow;
    }
}

#pragma force_active reset

BOOL FootprintStamp_BoundsOverlap(const FootprintStampBounds *a, const FootprintStampBounds *b) {
    if (a->left <= b->right && b->left <= a->right && a->bottom >= b->top && b->bottom >= a->top) {
        return TRUE;
    }
    return FALSE;
}

void FootprintStampBoard_NewChain(FootprintStampBoard *board, FootprintStampObj *stamp) {
    int i;
    FootprintStampChain *chain = board->chains;

    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++, chain++) {
        if (chain->refs == 0) {
            stamp->chain = i;
            chain->size = 0;
            chain->refs++;
            return;
        }
    }
    GF_AssertFail();
}

#pragma force_active on
#pragma opt_common_subs off

BOOL FootprintStampBoard_AddToChain(FootprintStampBoard *board, int chain, int effect) {
    if (chain == 0xFF) {
        return FALSE;
    }
    board->chains[chain].size++;
    board->chains[chain].refs++;
    if (board->cameraEffect == FOOTPRINT_STAMP_EFFECT_NONE && board->pendingCameraEffect == FOOTPRINT_STAMP_EFFECT_NONE && board->chains[chain].size >= sFootprintStampCameraChainLengths[board->playerCount]) {
        board->pendingCameraEffect = effect;
        MI_CpuFill8(&board->flash, 0, sizeof(FootprintStampFlash));
        return TRUE;
    }
    return FALSE;
}

#pragma opt_common_subs reset
#pragma force_active reset

void FootprintStampBoard_ReleaseChain(FootprintStampBoard *board, int chain) {
    int refs;

    if (chain != 0xFF) {
        u8 *base = &board->chains[0].refs;
        int offset = chain * sizeof(FootprintStampChain);
        refs = base[offset] - 1;
        if (refs < 0) {
            GF_AssertFail();
            refs = 0;
        }
        base[offset] = refs;
    }
}

void FootprintStampBoard_StartCameraEffect(FootprintStampBoard *board) {
    u8 effect;

    MI_CpuFill8(board, 0, offsetof(FootprintStampBoard, flash));
    effect = board->pendingCameraEffect;
    board->cameraEffect = effect;
    board->pendingCameraEffect = FOOTPRINT_STAMP_EFFECT_NONE;
    board->flashDone = FALSE;
}

#pragma force_active on

BOOL FootprintStamp_SetEffect(FootprintStampObj *stamp, int effect) {
    if (stamp->effectId != FOOTPRINT_STAMP_EFFECT_NONE || stamp->pendingEffect != FOOTPRINT_STAMP_EFFECT_NONE || stamp->ready == FALSE) {
        return FALSE;
    }
    stamp->pendingEffect = effect;
    MI_CpuFill8(&stamp->effect, 0, sizeof(stamp->effect));
    return TRUE;
}

#pragma force_active reset

u8 FootprintStamp_GetNatureEffect(FootprintStampObj *stamp) {
    return sFootprintStampNatureEffects[GetNatureFromPersonality(stamp->mon.personality)];
}

#pragma force_active on

// A new stamp appears after two frames and starts its nature's effect after 40.
BOOL FootprintStamp_UpdateIdle(FootprintStampBoard *board, FootprintStampObj *stamp) {
    int *timer = &stamp->effect.idle.timer;

    stamp->effect.idle.timer++;
    if (stamp->effect.idle.timer == 2) {
        FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, (FootprintStampBounds *)(timer + 1), FALSE);
        ModelInstance_SetVisible(&stamp->instance, TRUE);
        stamp->ready = TRUE;
    }
    if (*timer > 40) {
        FootprintStampBoard_NewChain(board, stamp);
        FootprintStamp_SetEffect(stamp, FootprintStamp_GetNatureEffect(stamp));
    }
    return FALSE;
}

#pragma force_active reset

BOOL FootprintStamp_HitIdle(void) {
    return FALSE;
}

#pragma force_active on

BOOL FootprintStamp_UpdateVanish(FootprintStampBoard *board, FootprintStampObj *stamp) {
    FootprintStampVanish *vanish = &stamp->effect.vanish;

    switch (vanish->state) {
    case 0:
        vanish->alpha = 31 << 8;
        vanish->polygonId = FootprintStampBoard_AllocPolygonId(board);
        vanish->scale = FX32_ONE;
        vanish->state++;
    case 1:
        vanish->scale += 192;
        if (vanish->alpha - 192 < 256) {
            ModelInstance_SetVisible(&stamp->instance, FALSE);
            vanish->state++;
        } else {
            vanish->alpha -= 192;
            ModelInstance_SetScale(&stamp->instance, vanish->scale, vanish->scale, FX32_ONE);
        }
        break;
    default:
        FootprintStampBoard_FreePolygonId(board, vanish->polygonId);
        return TRUE;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &vanish->bounds, TRUE);
    return FALSE;
}

void FootprintStamp_DrawVanish(FootprintStampBoard *board, FootprintStampObj *stamp) {
    FootprintStampVanish *vanish = &stamp->effect.vanish;

    if (vanish->polygonId != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, vanish->polygonId, vanish->alpha >> 8, 0);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x3F000000);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x1F0000);
    }
    ModelInstance_Draw(&stamp->instance);
}

#pragma force_active reset

BOOL FootprintStamp_HitVanish(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    return FootprintStamp_BoundsOverlap(&stamp->effect.vanish.bounds, &other->effect.idle.bounds) == TRUE;
}

#pragma force_active on

BOOL FootprintStamp_UpdateSplit(FootprintStampBoard *board, FootprintStampObj *stamp) {
    fx32 x, y, z;
    int i;
    FootprintStampSplit *split = &stamp->effect.split;
    FootprintStampModelInstance *pieces = split->pieces;

    switch (split->state) {
    case 0:
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        for (i = 0; i < 3; i++) {
            ModelInstance_Init(&pieces[i], &stamp->model);
            ModelInstance_SetPosition(&pieces[i], x, y, z);
        }
        split->state++;
    case 1: {
        // the stamp and its three copies move apart diagonally
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        ModelInstance_SetPosition(&stamp->instance, (s32)((u32)x - 0x2000), (s32)((u32)y - 0x2000), z);
        ModelInstance_GetPosition(&pieces[0], &x, &y, &z);
        ModelInstance_SetPosition(&pieces[0], (s32)((u32)x + 0x2000), (s32)((u32)y - 0x2000), z);
        ModelInstance_GetPosition(&pieces[1], &x, &y, &z);
        ModelInstance_SetPosition(&pieces[1], (s32)((u32)x - 0x2000), (s32)((u32)y + 0x2000), z);
        ModelInstance_GetPosition(&pieces[2], &x, &y, &z);
        ModelInstance_SetPosition(&pieces[2], (s32)((u32)x + 0x2000), (s32)((u32)y + 0x2000), z);
        split->timer++;
        if (split->timer > 30) {
            return TRUE;
        }
    } break;
    default:
        break;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &split->bounds[0], FALSE);
    for (i = 0; i < 3; i++, pieces++) {
        FootprintStamp_GetBounds(pieces, &stamp->mon, &split->bounds[i + 1], FALSE);
    }
    return FALSE;
}

#pragma force_active reset

void FootprintStamp_DrawSplit(FootprintStampBoard *board, FootprintStampObj *stamp) {
    int i;
    FootprintStampModelInstance *piece;

    piece = (FootprintStampModelInstance *)stamp;
    i = 0;
    piece = (FootprintStampModelInstance *)((u8 *)piece + offsetof(FootprintStampObj, effect.split.pieces));
    do {
        ModelInstance_Draw(piece);
        i++;
        piece++;
    } while (i < 3);
    ModelInstance_Draw(&stamp->instance);
}

#pragma force_active on

BOOL FootprintStamp_HitSplit(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    FootprintStampIdle *idle = &other->effect.idle;
    int i = 0;
    FootprintStampBounds *bounds = stamp->effect.split.bounds;

    for (; i < 4; i++) {
        if (FootprintStamp_BoundsOverlap(bounds, &idle->bounds) == TRUE) {
            return TRUE;
        }
        bounds++;
    }
    return FALSE;
}

// Turns the footprint a quarter turn, then zigzags away to the right.
BOOL FootprintStamp_UpdateZigzag(FootprintStampBoard *board, FootprintStampObj *stamp) {
    fx32 x, y, z;
    int *state = &stamp->effect.zigzag.state;

    switch (state[0]) {
    case 0: {
        u16 angle = ModelInstance_GetRotation(&stamp->instance, 2) - 0xAAA;
        if (angle <= 0xE000) {
            FootprintStamp_RotateFootprintTexture(stamp->model.texture, stamp->effectId);
            FootprintStampBoard_QueueTexture(board, stamp->model.texture);
            stamp->textureChanged = TRUE;
            angle += 0x4000;
            state[0]++;
        }
        ModelInstance_SetRotation(&stamp->instance, angle, 2);
        break;
    }
    case 1: {
        u16 angle;
        stamp->textureChanged = FALSE;
        angle = ModelInstance_GetRotation(&stamp->instance, 2) - 0xAAA;
        if (angle >= 0xC000) {
            state[0]++;
            angle = 0;
        }
        ModelInstance_SetRotation(&stamp->instance, angle, 2);
        break;
    }
    case 2:
        state[1]++;
        if (state[1] < 4) {
            break;
        }
        state[1] = 0;
        state[0]++;
    case 3:
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        if (x < -0x50000 || x > 0x50000 || y > 0x30000 || y < -0x30000) {
            return TRUE;
        }
        if (state[2] < 5) {
            ModelInstance_SetPosition(&stamp->instance, x + 0x3000, y - 0x3000, z);
        } else {
            ModelInstance_SetPosition(&stamp->instance, x + 0x3000, y + 0x3000, z);
        }
        state[2]++;
        if (state[2] >= 10) {
            state[2] = 0;
        }
        break;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, (FootprintStampBounds *)&state[3], FALSE);
    return FALSE;
}

#pragma force_active reset

BOOL FootprintStamp_HitZigzag(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    return FootprintStamp_BoundsOverlap(&stamp->effect.zigzag.bounds, &other->effect.idle.bounds) == TRUE;
}

#pragma force_active on

// Turns the footprint a quarter turn the other way, then waves away to the left.
BOOL FootprintStamp_UpdateWave(FootprintStampBoard *board, FootprintStampObj *stamp) {
    FootprintStampWave *wave = &stamp->effect.wave;
    fx32 x, y, z;

    switch (wave->state) {
    case 0: {
        u16 angle = ModelInstance_GetRotation(&stamp->instance, 2) + 0xAAA;
        if (angle >= 0x2000) {
            FootprintStamp_RotateFootprintTexture(stamp->model.texture, stamp->effectId);
            angle -= 0x4000;
            FootprintStampBoard_QueueTexture(board, stamp->model.texture);
            stamp->textureChanged = TRUE;
            wave->state++;
        }
        ModelInstance_SetRotation(&stamp->instance, angle, 2);
        break;
    }
    case 1: {
        u16 angle;
        stamp->textureChanged = FALSE;
        angle = ModelInstance_GetRotation(&stamp->instance, 2) + 0xAAA;
        if (angle < 0x4000) {
            wave->state++;
            angle = 0;
        }
        ModelInstance_SetRotation(&stamp->instance, angle, 2);
        break;
    }
    case 2:
        wave->timer++;
        if (wave->timer < 4) {
            break;
        }
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        wave->baseY = y;
        wave->timer = 0;
        wave->state++;
    case 3: {
        fx32 sine;
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        if (x < -0x50000 || x > 0x50000 || y > 0x30000 || y < -0x30000) {
            return TRUE;
        }
        wave->angle += 0x8000;
        if (wave->angle >= 0x168000) {
            wave->angle -= 0x168000;
        }
        sine = GF_SinDegFX32(wave->angle);
        ModelInstance_SetPosition(&stamp->instance, x - 0x1000, wave->baseY + (fx32)(((s64)sine * 0x8000 + 0x800) >> 12), z);
        break;
    }
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &wave->bounds, FALSE);
    return FALSE;
}

#pragma force_active reset

BOOL FootprintStamp_HitWave(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    return FootprintStamp_BoundsOverlap(&stamp->effect.wave.bounds, &other->effect.idle.bounds) == TRUE;
}

#pragma force_active on

// Rises off the board, leaving a trail of three copies.
BOOL FootprintStamp_UpdateRise(FootprintStampBoard *board, FootprintStampObj *stamp) {
    FootprintStampRise *rise = &stamp->effect.rise;
    fx32 x, y, z;
    int i;
    FootprintStampModelInstance *copy;

    switch (rise->state) {
    case 0:
        rise->polygonId = (u8)FootprintStampBoard_AllocPolygonId(board);
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        for (i = 0, copy = rise->trail; i < 3; i++, copy++) {
            ModelInstance_Init(copy, &stamp->model);
            ModelInstance_SetPosition(copy, x, y, z);
            ModelInstance_SetVisible(copy, FALSE);
        }
        rise->state++;
    case 1:
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        if (x < -327680 || x > 327680 || y > 196608 || y < -196608 || rise->stop == TRUE) {
            for (i = 0, copy = rise->trail; i < 3; i++, copy++) {
                ModelInstance_SetVisible(copy, FALSE);
            }
            ModelInstance_SetVisible(&stamp->instance, FALSE);
            rise->state++;
        } else {
            ModelInstance_SetPosition(&stamp->instance, x, y + 10240, z);
            if (rise->timer % 3 == 0) {
                ModelInstance_SetPosition(&rise->trail[rise->trailIndex], x, y + 10240, z);
                ModelInstance_SetVisible(&rise->trail[rise->trailIndex], TRUE);
                rise->trailIndex++;
                if (rise->trailIndex >= 3) {
                    rise->trailIndex = 0;
                }
            }
            rise->timer++;
        }
        break;
    default:
        FootprintStampBoard_FreePolygonId(board, rise->polygonId);
        return TRUE;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &rise->bounds, FALSE);
    return FALSE;
}

#pragma force_active reset

void FootprintStamp_DrawRise(FootprintStampBoard *board, FootprintStampObj *stamp) {
    int i;
    FootprintStampRise *rise = &stamp->effect.rise;
    FootprintStampModelInstance *copy;
    u8 polygonId = rise->polygonId;

    if (polygonId != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, polygonId, 8, 0);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x3F000000);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x001F0000);
    }
    for (i = 0, copy = rise->trail; i < 3; i++, copy++) {
        ModelInstance_Draw(copy);
    }
    polygonId = rise->polygonId;
    if (polygonId != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, polygonId, 31, 0);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x3F000000);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x001F0000);
    }
    ModelInstance_Draw(&stamp->instance);
}

BOOL FootprintStamp_HitRise(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    return FootprintStamp_BoundsOverlap(&stamp->effect.rise.bounds, &other->effect.idle.bounds) == TRUE;
}

#pragma force_active on

// Falls off the board swaying from side to side.
BOOL FootprintStamp_UpdateFall(FootprintStampBoard *board, FootprintStampObj *stamp) {
    fx32 x, y, z;
    FootprintStampFall *fall = &stamp->effect.fall;

    switch (fall->state) {
    case 0:
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        fall->baseX = x;
        fall->state++;
    case 1: {
        fx32 angle, sine, offset;
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        if (x < -0x50000 || x > 0x50000 || y > 0x30000 || y < -0x30000) {
            return TRUE;
        }
        // the angle is re-read from memory each time, as in the original (volatile)
        angle = *(volatile fx32 *)&fall->angle + 0x8000;
        *(volatile fx32 *)&fall->angle = angle;
        if (angle >= (360 << FX32_SHIFT)) {
            *(volatile fx32 *)&fall->angle -= 360 << FX32_SHIFT;
        }
        sine = GF_SinDegFX32(*(volatile fx32 *)&fall->angle);
        offset = (fx32)(((s64)sine * 0x8000 + 0x800) >> FX32_SHIFT);
        ModelInstance_SetPosition(&stamp->instance, fall->baseX + offset, y - FX32_ONE, z);
        break;
    }
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &fall->bounds, FALSE);
    return FALSE;
}

#pragma force_active reset

BOOL FootprintStamp_HitFall(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    return FootprintStamp_BoundsOverlap(&stamp->effect.fall.bounds, &other->effect.idle.bounds) == TRUE;
}

#pragma force_active on

BOOL FootprintStamp_UpdateGrow(FootprintStampBoard *board, FootprintStampObj *stamp) {
    FootprintStampGrow *grow = &stamp->effect.grow;

    switch (grow->state) {
    case 0:
        grow->scale = FX32_ONE;
        grow->state++;
    case 1:
        grow->timer++;
        if (grow->timer > 8) {
            return TRUE;
        }
        grow->scale += 768;
        ModelInstance_SetScale(&stamp->instance, grow->scale, grow->scale, FX32_ONE);
        break;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &grow->bounds, TRUE);
    return FALSE;
}

#pragma force_active reset

BOOL FootprintStamp_HitGrow(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    return FootprintStamp_BoundsOverlap(&stamp->effect.grow.bounds, &other->effect.idle.bounds) == TRUE;
}

#pragma force_active on

// Two fading copies move apart sideways.
BOOL FootprintStamp_UpdateSpreadX(FootprintStampBoard *board, FootprintStampObj *stamp) {
    FootprintStampSpread *spread = &stamp->effect.spread;
    fx32 x, y, z;
    int i;

    switch (spread->state) {
    case 0:
        spread->polygonId = FootprintStampBoard_AllocPolygonId(board);
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        for (i = 0; i < 2; i++) {
            ModelInstance_Init(&spread->copies[i], &stamp->model);
            ModelInstance_SetPosition(&spread->copies[i], x, y, z);
            ModelInstance_SetVisible(&spread->copies[i], TRUE);
        }
        spread->alpha = 31 << 8;
        spread->state++;
    case 1:
        if ((int)spread->alpha - 160 < 256) {
            for (i = 0; i < 2; i++) {
                ModelInstance_SetVisible(&spread->copies[i], FALSE);
            }
            ModelInstance_SetVisible(&stamp->instance, FALSE);
            spread->state++;
        } else {
            fx32 offset;
            spread->alpha -= 160;
            spread->angle += 0x20000;
            if (spread->angle >= 0x168000) {
                spread->angle -= 0x168000;
            }
            offset = (fx32)(((s64)GF_SinDegFX32(spread->angle) * 0x10000 + 0x800) >> 12);
            ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
            for (i = 0; i < 2; i++) {
                if (i & 1) {
                    ModelInstance_SetPosition(&spread->copies[i], x + offset, y, z);
                } else {
                    ModelInstance_SetPosition(&spread->copies[i], x - offset, y, z);
                }
            }
        }
        break;
    default:
        FootprintStampBoard_FreePolygonId(board, spread->polygonId);
        return TRUE;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &spread->bounds[0], FALSE);
    for (i = 0; i < 2; i++) {
        FootprintStamp_GetBounds(&spread->copies[i], &stamp->mon, &spread->bounds[i + 1], FALSE);
    }
    return FALSE;
}

#pragma force_active reset

void FootprintStamp_DrawSpreadX(FootprintStampBoard *board, FootprintStampObj *stamp) {
    int i;
    FootprintStampSpread *spread;

    spread = &stamp->effect.spread;
    if (spread->polygonId != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, spread->polygonId, spread->alpha >> 8, 0);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x3F << 24);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x1F << 16);
    }
    i = 0;
    do {
        ModelInstance_Draw(&spread->copies[0]);
        i++;
        spread = (FootprintStampSpread *)((u8 *)spread + sizeof(FootprintStampModelInstance));
    } while (i < 2);
    ModelInstance_Draw(&stamp->instance);
}

BOOL FootprintStamp_HitSpreadX(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    int i;
    FootprintStampBounds *bounds;
    FootprintStampIdle *idle;

    idle = &other->effect.idle;
    i = 0;
    bounds = stamp->effect.spread.bounds;
    do {
        if (FootprintStamp_BoundsOverlap(bounds, &idle->bounds) == TRUE) {
            return TRUE;
        }
        i++;
        bounds++;
    } while (i < 3);
    return FALSE;
}

#pragma force_active on

// Two fading copies move apart up and down.
BOOL FootprintStamp_UpdateSpreadY(FootprintStampBoard *board, FootprintStampObj *stamp) {
    FootprintStampSpread *spread = &stamp->effect.spread;
    fx32 x, y, z;
    int i;

    switch (spread->state) {
    case 0:
        spread->polygonId = FootprintStampBoard_AllocPolygonId(board);
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        for (i = 0; i < 2; i++) {
            ModelInstance_Init(&spread->copies[i], &stamp->model);
            ModelInstance_SetPosition(&spread->copies[i], x, y, z);
            ModelInstance_SetVisible(&spread->copies[i], TRUE);
        }
        spread->alpha = 31 << 8;
        spread->state++;
    case 1:
        if ((int)spread->alpha - 160 < 256) {
            for (i = 0; i < 2; i++) {
                ModelInstance_SetVisible(&spread->copies[i], FALSE);
            }
            ModelInstance_SetVisible(&stamp->instance, FALSE);
            spread->state++;
        } else {
            fx32 offset;
            spread->alpha -= 160;
            spread->angle += 0x20000;
            if (spread->angle >= 0x168000) {
                spread->angle -= 0x168000;
            }
            offset = (fx32)(((s64)GF_SinDegFX32(spread->angle) * 0x10000 + 0x800) >> 12);
            ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
            for (i = 0; i < 2; i++) {
                if (i & 1) {
                    ModelInstance_SetPosition(&spread->copies[i], x, y + offset, z);
                } else {
                    ModelInstance_SetPosition(&spread->copies[i], x, y - offset, z);
                }
            }
        }
        break;
    default:
        FootprintStampBoard_FreePolygonId(board, spread->polygonId);
        return TRUE;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &spread->bounds[0], FALSE);
    for (i = 0; i < 2; i++) {
        FootprintStamp_GetBounds(&spread->copies[i], &stamp->mon, &spread->bounds[i + 1], FALSE);
    }
    return FALSE;
}

#pragma force_active reset

void FootprintStamp_DrawSpreadY(FootprintStampBoard *board, FootprintStampObj *stamp) {
    int i;
    FootprintStampSpread *spread;

    spread = &stamp->effect.spread;
    if (spread->polygonId != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, spread->polygonId, spread->alpha >> 8, 0);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x3F << 24);
        NNSi_G3dModifyPolygonAttrMask(stamp->model.model, 0, 0x1F << 16);
    }
    i = 0;
    do {
        ModelInstance_Draw(&spread->copies[0]);
        i++;
        spread = (FootprintStampSpread *)((u8 *)spread + sizeof(FootprintStampModelInstance));
    } while (i < 2);
    ModelInstance_Draw(&stamp->instance);
}

BOOL FootprintStamp_HitSpreadY(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    int i;
    FootprintStampBounds *bounds;
    FootprintStampIdle *idle;

    idle = &other->effect.idle;
    i = 0;
    bounds = stamp->effect.spread.bounds;
    do {
        if (FootprintStamp_BoundsOverlap(bounds, &idle->bounds) == TRUE) {
            return TRUE;
        }
        i++;
        bounds++;
    } while (i < 3);
    return FALSE;
}

// Stretches downwards.
BOOL FootprintStamp_UpdateStretch(FootprintStampBoard *board, FootprintStampObj *stamp) {
    fx32 x;
    fx32 y;
    fx32 z;
    FootprintStampStretch *stretch;
    int offset;

    stretch = &stamp->effect.stretch;
    switch (stretch->state) {
    case 0:
        stretch->scale = FX32_ONE;
        stretch->state++;
    case 1:
        stretch->timer++;
        if (stretch->timer > 30) {
            return TRUE;
        }
        stretch->scale += 384;
        ModelInstance_SetScale(&stamp->instance, FX32_ONE, stretch->scale, FX32_ONE);
        offset = -(((stretch->scale - FX32_ONE) << FX32_SHIFT) / FX32_ONE);
        ModelInstance_GetPosition(&stamp->instance, &x, &y, &z);
        ModelInstance_SetPosition(&stamp->instance, x, y + offset, z);
        break;
    }
    FootprintStamp_GetBounds(&stamp->instance, &stamp->mon, &stretch->bounds, TRUE);
    return FALSE;
}

BOOL FootprintStamp_HitStretch(FootprintStampBoard *board, FootprintStampObj *stamp, FootprintStampObj *other) {
    return FootprintStamp_BoundsOverlap(&stamp->effect.stretch.bounds, &other->effect.idle.bounds) == TRUE;
}

// Brightness flash (twice) announcing a camera move; inkColumn 0 darkens, otherwise brightens.
BOOL FootprintStampBoard_Flash(FootprintStampBoard *board, FootprintStampFlash *flash, int phase, u32 inkColumn) {
    int sign;

    if (phase == 3) {
        return FALSE;
    }
    sign = inkColumn == 0 ? -1 : 1;
    switch (flash->state) {
    case 0:
        flash->brightness += 0x200;
        if (flash->brightness >= 0x800) {
            flash->brightness = 0x800;
            flash->state++;
        }
        board->brightnessChanged = TRUE;
        board->brightness = (flash->brightness >> 8) * sign;
        break;
    case 1:
        flash->brightness -= 0x200;
        if (flash->brightness <= 0) {
            flash->brightness = 0;
            flash->count++;
            if (flash->count < 2) {
                flash->state = 0;
            } else {
                flash->state++;
            }
        }
        board->brightnessChanged = TRUE;
        board->brightness = (flash->brightness >> 8) * sign;
        break;
    default:
        board->flashDone = TRUE;
        return TRUE;
    }
    return FALSE;
}

BOOL FootprintStampCamera_ZoomIn(FootprintStampBoard *unused, FootprintStampZoom *move, Camera *camera) {
    switch (move->state) {
    case 0:
        move->distance = Camera_GetDistance(camera);
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        Camera_AdjustDistance(-0x1AAA, camera);
        move->timer++;
        if (move->timer >= 30) {
            move->timer = 0;
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer > 4) {
            move->timer = 0;
            move->state++;
        }
        break;
    case 3:
        Camera_AdjustDistance(0x8555, camera);
        move->timer++;
        if (move->timer >= 6 || Camera_GetDistance(camera) >= move->distance) {
            Camera_SetDistance(move->distance, camera);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL FootprintStampCamera_Roll(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampRoll *move = &board->roll;
    int angle;

    switch (move->state) {
    case 0:
        PlaySE(SEQ_SE_PL_TIMER03);
        move->state++;
    case 1:
        move->phase++;
        angle = move->phase << 10;
        if (angle < 0x2000) {
            move->up.y = FX32_ONE - angle;
        } else {
            move->up.y = angle - 0x3000;
        }
        if (angle < 0x1000) {
            move->up.x = angle;
        } else if (angle < 0x2000) {
            move->up.x = 0x2000 - angle;
        } else if (angle < 0x3000) {
            move->up.x = -(angle - 0x2000);
        } else {
            move->up.x = angle - 0x4000;
        }
        if (move->phase >= 16) {
            move->up.y = FX32_ONE;
            move->up.x = 0;
            move->phase = 0;
            move->count++;
            if (move->count >= 2) {
                move->state++;
            }
        }
        Camera_SetLookAtCamUp(&move->up, camera);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL FootprintStampCamera_TurnLeft(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampTurn *move = &board->turnLeft;
    CameraAngle delta = { 0, 0, 0, 0 };
    CameraAngle angle;

    switch (move->state) {
    case 0:
        angle = Camera_GetAngle(camera);
        move->angle = angle;
        move->current = move->angle.y;
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        delta.y -= 0x1000;
        move->current -= 0x1000;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current <= move->angle.y - 0x2000) {
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer >= 16) {
            move->state++;
        }
        break;
    case 3:
        delta.y += 0x200;
        move->current += 0x200;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current >= move->angle.y) {
            Camera_SetAnglePos(&move->angle, camera);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL FootprintStampCamera_TurnRight(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampTurn *move = &board->turnRight;
    CameraAngle delta = { 0, 0, 0, 0 };
    CameraAngle angle;

    switch (move->state) {
    case 0:
        angle = Camera_GetAngle(camera);
        move->angle = angle;
        move->current = move->angle.y;
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        delta.y += 0x1000;
        move->current += 0x1000;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current >= move->angle.y + 0x2000) {
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer >= 16) {
            move->state++;
        }
        break;
    case 3:
        delta.y -= 0x200;
        move->current -= 0x200;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current <= move->angle.y) {
            Camera_SetAnglePos(&move->angle, camera);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL FootprintStampCamera_PitchUp(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampTurn *move = &board->pitchUp;
    CameraAngle delta = { 0, 0, 0, 0 };
    CameraAngle angle;

    switch (move->state) {
    case 0:
        angle = Camera_GetAngle(camera);
        move->angle = angle;
        move->current = move->angle.x;
        PlaySE(SEQ_SE_PL_TIMER03);
        move->state++;
    case 1:
        delta.x += 0xAAA;
        move->current += 0xAAA;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current >= move->angle.x + 0x2000) {
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer >= 16) {
            move->state++;
        }
        break;
    case 3:
        delta.x -= 0x200;
        move->current -= 0x200;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current <= move->angle.x) {
            Camera_SetAnglePos(&move->angle, camera);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL FootprintStampCamera_Sway(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampSway *move = &board->sway;
    CameraAngle target = { 0, 0, 0, 0 };
    CameraAngle angle;
    fx32 sine;

    switch (move->state) {
    case 0:
        angle = Camera_GetAngle(camera);
        move->angle = angle;
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        move->phase += 0x18000;
        if (move->phase >= 0x168000) {
            move->phase -= 0x168000;
            move->count++;
            if (move->count >= 2) {
                move->state++;
                move->phase = 0;
            }
        }
        sine = GF_SinDegFX32(move->phase);
        target = move->angle;
        target.y = FX_Mul(sine, FX32_ONE);
        Camera_SetAngleTarget(&target, camera);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL FootprintStampCamera_ZoomOut(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampZoom *move = &board->zoomOut;

    switch (move->state) {
    case 0:
        move->distance = Camera_GetDistance(camera);
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        Camera_AdjustDistance(0x1AAA, camera);
        move->timer++;
        if (move->timer >= 30) {
            move->timer = 0;
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer > 4) {
            move->timer = 0;
            move->state++;
        }
        break;
    case 3:
        Camera_AdjustDistance(-0x8555, camera);
        move->timer++;
        if (move->timer >= 6 || Camera_GetDistance(camera) <= move->distance) {
            Camera_SetDistance(move->distance, camera);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL FootprintStampCamera_ShakeX(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampShake *move = &board->shakeX;
    VecFx32 offset = { 0, 0, 0 };
    VecFx32 position;
    VecFx32 target;

    switch (move->state) {
    case 0:
        position = Camera_GetLookAtCamPos(camera);
        move->position = position;
        target = Camera_GetLookAtCamTarget(camera);
        move->target = target;
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        move->phase += 0x20000;
        if (move->phase >= 0x168000) {
            move->phase -= 0x168000;
            move->count++;
            if (move->count >= 4) {
                move->state++;
                move->phase = 0;
            }
        }
        offset.x = FX_Mul(GF_SinDegFX32(move->phase), 0x10000);
        Camera_SetLookAtCamTarget(&move->target, camera);
        Camera_SetLookAtCamPos(&move->position, camera);
        Camera_OffsetLookAtPosAndTarget(&offset, camera);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL FootprintStampCamera_ShakeY(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampShake *move = &board->shakeY;
    VecFx32 offset = { 0, 0, 0 };
    VecFx32 position;
    VecFx32 target;

    switch (move->state) {
    case 0:
        position = Camera_GetLookAtCamPos(camera);
        move->position = position;
        target = Camera_GetLookAtCamTarget(camera);
        move->target = target;
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        move->phase += 0x20000;
        if (move->phase >= 0x168000) {
            move->phase -= 0x168000;
            move->count++;
            if (move->count >= 4) {
                move->state++;
                move->phase = 0;
            }
        }
        offset.y = FX_Mul(GF_SinDegFX32(move->phase), 0x8000);
        Camera_SetLookAtCamTarget(&move->target, camera);
        Camera_SetLookAtCamPos(&move->position, camera);
        Camera_OffsetLookAtPosAndTarget(&offset, camera);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL FootprintStampCamera_PitchDown(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera) {
    FootprintStampTurn *move = &board->pitchDown;
    CameraAngle delta = { 0, 0, 0, 0 };
    CameraAngle angle;

    switch (move->state) {
    case 0:
        angle = Camera_GetAngle(camera);
        move->angle = angle;
        move->current = move->angle.x;
        move->state++;
        PlaySE(SEQ_SE_PL_TIMER03);
    case 1:
        delta.x -= 0xAAA;
        move->current -= 0xAAA;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current <= move->angle.x - 0x2000) {
            move->state++;
        }
        break;
    case 2:
        move->timer++;
        if (move->timer >= 16) {
            move->state++;
        }
        break;
    case 3:
        delta.x += 0x200;
        move->current += 0x200;
        Camera_AdjustAnglePos(&delta, camera);
        if (move->current >= move->angle.x) {
            Camera_SetAnglePos(&move->angle, camera);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void FootprintStamp_PrintPlayerName(MsgData *msgData, MessageFormat *msgFormat, Window *windows, void *lobby, u32 playerId) {
    u32 index = WiFiPlazaLobbyClient_IdToChannelSlot(playerId);
    int slot;
    void *record;
    PlayerProfile *profile;
    String *template;
    String *name;
    String *string;
    int gender;
    u32 color;
    Window *window;

    if (index == (u32)-1) {
        return;
    }
    slot = WiFiPlazaLobbyClient_IdToSlot(playerId);
    if (slot == -1) {
        return;
    }
    record = WiFiPlazaWork_GetPlayer(lobby, slot);
    if (record == NULL) {
        return;
    }
    if (index >= FOOTPRINT_STAMP_PLAYERS_MAX) {
        GF_AssertFail();
        return;
    }
    profile = PlayerProfile_New(HEAP_ID_125);
    WiFiPlazaPlayer_CopyToProfile(record, profile, HEAP_ID_125);
    name = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_125);
    gender = PlayerProfile_GetTrainerGender(profile);
    BufferString(msgFormat, 0, name, gender, 1, 2);
    template = NewString_ReadMsgData(msgData, msg_0754_00000);
    string = String_New(0x40, HEAP_ID_125);
    StringExpandPlaceholders(msgFormat, string, template);
    window = &windows[index];
    FillWindowPixelBuffer(window, 0);
    color = WiFiPlazaWork_IsSpecialVisitor(lobby, slot) == 1 ? MAKE_TEXT_COLOR(7, 8, 0) : MAKE_TEXT_COLOR(1, 2, 0);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_INSTANT, color, NULL);
    CopyWindowToVram(window);
    String_Delete(name);
    String_Delete(template);
    String_Delete(string);
    Heap_Free(profile);
}

void FootprintStamp_ClearPlayerNameWindow(Window *windows, u32 index) {
    if (index >= FOOTPRINT_STAMP_PLAYERS_MAX) {
        GF_AssertFail();
    } else {
        ClearWindowTilemapAndCopyToVram(&windows[index]);
    }
}

u16 FootprintStamp_GetInkColor(u32 column, u32 otId) {
    return sFootprintStampInkColors[otId % 9][column];
}

u32 FootprintStamp_GetTouchedPartySlot(const FootprintStampMon *party, int current) {
    u32 ret = FOOTPRINT_STAMP_TOUCH_NONE;
    int slot = TouchscreenHitbox_FindRectAtTouchNew(sFootprintStampPartyHitboxes);

    if (slot < FOOTPRINT_STAMP_PARTY_SLOTS) {
        if (current != slot && party[slot].species != SPECIES_NONE && party[slot].species <= NATIONAL_DEX_COUNT) {
            ret = slot;
        }
    } else {
        if (slot == FOOTPRINT_STAMP_PARTY_SLOTS) {
            ret = FOOTPRINT_STAMP_TOUCH_EXIT;
        } else {
            return ret;
        }
    }
    return ret;
}

// 0: generic footprint, 1: the species' own footprint.
u32 FootprintStamp_GetFootprintType(u32 species, int form, u32 unk19E0) {
    if (species == SPECIES_GIRATINA && form > 0) {
        return 0;
    }
    if (species == SPECIES_ARCEUS && unk19E0 == 0) {
        return 0;
    }
    return sFootprintStampSpeciesFootprints[species * 4];
}

u32 FootprintStamp_GetFootprintSize(u32 species, int form) {
    if (species == SPECIES_GIRATINA && form > 0) {
        return 2;
    }
    return sFootprintStampSpeciesFootprintSizes[species * 4];
}

int FootprintStamp_RegisterLinkCommands(FootprintStamp *work) {
    return WiFiPlazaLobbyClient_SetChannelCommands(sFootprintStampLinkCommands, 3, work);
}

BOOL FootprintStamp_IsFromOtherPlayer(u32 playerId, const FootprintStampLinkFilter *filter) {
    int index;

    if (filter->myId == playerId) {
        return FALSE;
    }
    if (filter->active == FALSE) {
        return FALSE;
    }
    // called with a second argument the function does not take
    index = ((int (*)(u32, BOOL))WiFiPlazaLobbyClient_IdToChannelSlot)(playerId, filter->active);
    if (index != -1) {
        return TRUE;
    }
    return FALSE;
}

BOOL FootprintStamp_SendStamp(FootprintStampMon *mon) {
    // called with an argument the function does not take
    if (((BOOL(*)(FootprintStampMon *))WiFiPlaza_HasConnectionFailed)(mon) == TRUE) {
        return FALSE;
    }
    WiFiPlazaLobbyClient_SendToChannel(2, mon, sizeof(FootprintStampMon));
    return TRUE;
}

void FootprintStamp_HandlePlayerJoined(u32 playerId, void *data, u32 unused, FootprintStamp *work) {
    FootprintStampLinkFilter *filter = FootprintStamp_GetLinkFilter(work);

    if (FootprintStamp_IsFromOtherPlayer(playerId, filter)) {
        FootprintStamp_ShowPlayerName(work, playerId);
    }
}

void FootprintStamp_HandlePlayerLeft(u32 playerId, void *data, u32 unused, FootprintStamp *work) {
    FootprintStampLinkFilter *filter = FootprintStamp_GetLinkFilter(work);

    if (FootprintStamp_IsFromOtherPlayer(playerId, filter)) {
        FootprintStamp_ClearPlayerName(work, WiFiPlazaLobbyClient_IdToChannelSlot(playerId));
    }
}

void FootprintStamp_HandleStampReceived(u32 playerId, void *data, u32 unused, FootprintStamp *work) {
    FootprintStampLinkFilter *filter = FootprintStamp_GetLinkFilter(work);

    if (FootprintStamp_IsFromOtherPlayer(playerId, filter)) {
        FootprintStamp_PlaceStamp(work, data, playerId);
    }
}
