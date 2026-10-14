#include "global.h"

#include "constants/sndseq.h"

#include "error_handling.h"
#include "filesystem.h"
#include "geometry_math.h"
#include "sound_playback.h"
#include "wobbuffet_pop.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define WOBBUFFET_POP_PACK_TEXCOORD(s, t) ((u16)(fx16)((s) >> (FX32_SHIFT - 4)) | ((u32)(u16)(fx16)((t) >> (FX32_SHIFT - 4)) << 16))
#define WOBBUFFET_POP_PACK_VTX16_XY(x, y) ((u32)(u16)(x) | ((u32)(u16)(y) << 16))
#define WOBBUFFET_POP_NORMAL_FRONT        0x1FF00000

// Area the handle can be grabbed in, per balloon selection: x min, x max, y min, y max (4 bytes each).
extern const u8 sWobbuffetPopBalloonTouchLeft[];
extern const u8 sWobbuffetPopBalloonTouchRight[];
extern const u8 sWobbuffetPopBalloonTouchTop[];
extern const u8 sWobbuffetPopBalloonTouchBottom[];
extern const ManagedSpriteTemplate sWobbuffetPopHandleSpriteTemplate;

void WobbuffetPopBalloon_LoadShininessTable(void) {
    u8 buf[0x80];
    u8 *p = buf;
    u8 i;
    s32 n;

    for (i = 0; i < 0x7f; i++) {
        n = i * 2 + 1;
        p[i] = (u8)(((s64)n * n * n * n) >> 24);
    }
    p[0x7f] = 0xff;
    NNS_G3dGeBufferOP_N(G3OP_SHININESS, (u32 *)buf, G3OP_SHININESS_NPARAMS);
}

void WobbuffetPopBalloon_InitMeshVertices(WobbuffetPopBalloon *balloon) {
    s32 i;
    s32 y;

    for (i = 0, y = 0; i < WOBBUFFET_POP_MESH_ROWS; i++, y -= 0x10000) {
        balloon->vertices[i].topLeftX = 0;
        balloon->vertices[i].topLeftY = y;
        balloon->vertices[i].bottomLeftX = 0;
        balloon->vertices[i].bottomLeftY = y - 0x10000;
        balloon->vertices[i].topRightX = 0x80000;
        balloon->vertices[i].topRightY = y;
        balloon->vertices[i].bottomRightX = 0x80000;
        balloon->vertices[i].bottomRightY = y - 0x10000;
    }
}

void WobbuffetPopBalloon_InitMeshTexCoords(WobbuffetPopBalloon *balloon) {
    s32 i;
    s32 t;

    for (i = 0, t = 0; i < WOBBUFFET_POP_MESH_ROWS; i++, t += 0x10000) {
        balloon->texCoords[i].topLeftX = 0;
        balloon->texCoords[i].topLeftY = t;
        balloon->texCoords[i].bottomLeftX = 0;
        balloon->texCoords[i].bottomLeftY = t + 0x10000;
        balloon->texCoords[i].topRightX = 0x80000;
        balloon->texCoords[i].topRightY = t;
        balloon->texCoords[i].bottomRightX = 0x80000;
        balloon->texCoords[i].bottomRightY = t + 0x10000;
    }
}

void WobbuffetPopBalloon_DrawMesh(WobbuffetPopBalloon *balloon) {
    s16 x;
    s16 y;
    s32 i;

    for (i = 0; i < WOBBUFFET_POP_MESH_ROWS; i++) {
        u32 begin = GX_BEGIN_QUADS;

        NNS_G3dGeBufferOP_N(G3OP_BEGIN, &begin, G3OP_BEGIN_NPARAMS);
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            WobbuffetPopBalloon_ToVtx16(balloon->vertices[i].topLeftX, balloon->vertices[i].topLeftY, &x, &y);
            texcoord = WOBBUFFET_POP_PACK_TEXCOORD(balloon->texCoords[i].topLeftX, balloon->texCoords[i].topLeftY);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = WOBBUFFET_POP_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = WOBBUFFET_POP_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            WobbuffetPopBalloon_ToVtx16(balloon->vertices[i].bottomLeftX, balloon->vertices[i].bottomLeftY, &x, &y);
            texcoord = WOBBUFFET_POP_PACK_TEXCOORD(balloon->texCoords[i].bottomLeftX, balloon->texCoords[i].bottomLeftY);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = WOBBUFFET_POP_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = WOBBUFFET_POP_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            WobbuffetPopBalloon_ToVtx16(balloon->vertices[i].bottomRightX, balloon->vertices[i].bottomRightY, &x, &y);
            texcoord = WOBBUFFET_POP_PACK_TEXCOORD(balloon->texCoords[i].bottomRightX, balloon->texCoords[i].bottomRightY);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = WOBBUFFET_POP_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = WOBBUFFET_POP_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        {
            u32 texcoord;
            u32 normal;
            u32 vtx[2];

            WobbuffetPopBalloon_ToVtx16(balloon->vertices[i].topRightX, balloon->vertices[i].topRightY, &x, &y);
            texcoord = WOBBUFFET_POP_PACK_TEXCOORD(balloon->texCoords[i].topRightX, balloon->texCoords[i].topRightY);
            NNS_G3dGeBufferOP_N(G3OP_TEXCOORD, &texcoord, G3OP_TEXCOORD_NPARAMS);
            normal = WOBBUFFET_POP_NORMAL_FRONT;
            NNS_G3dGeBufferOP_N(G3OP_NORMAL, &normal, G3OP_NORMAL_NPARAMS);
            vtx[0] = WOBBUFFET_POP_PACK_VTX16_XY(x, y);
            vtx[1] = 0;
            NNS_G3dGeBufferOP_N(G3OP_VTX_16, vtx, G3OP_VTX_16_NPARAMS);
        }
        NNS_G3dGeBufferOP_N(G3OP_END, NULL, G3OP_END_NPARAMS);
    }
}

// Mesh coordinates (0..128 per axis) to VTX_16 coordinates around the origin.
void WobbuffetPopBalloon_ToVtx16(s32 x, s32 y, s16 *outX, s16 *outY) {
    s32 tx = ((s64)x << 12) / 0x80000;
    s32 ty = ((s64)y << 12) / 0x80000;

    if (tx > 0x7fff) {
        GF_AssertFail();
    }
    if (tx < -0x8000) {
        GF_AssertFail();
    }
    if (ty > 0x7fff) {
        GF_AssertFail();
    }
    if (ty < -0x8000) {
        GF_AssertFail();
    }
    if (tx - 0x800 > 0x7fff) {
        GF_AssertFail();
    }
    if (tx - 0x800 < -0x8000) {
        GF_AssertFail();
    }
    if (ty + 0x800 > 0x7fff) {
        GF_AssertFail();
    }
    if (ty + 0x800 < -0x8000) {
        GF_AssertFail();
    }
    *outX = tx - 0x800;
    *outY = ty + 0x800;
}

int WobbuffetPopBalloon_TryGrabHandle(WobbuffetPopBalloon *balloon) {
    s32 idx;
    s32 y;
    s32 mid;

    if (balloon->grabbing == 1 || balloon->touch[3] == 0) {
        return 0;
    }
    idx = balloon->selection;
    y = sWobbuffetPopBalloonTouchTop[idx * 4] + (balloon->pushDepth >> 12);
    if (balloon->touch[0] < sWobbuffetPopBalloonTouchLeft[idx * 4] || balloon->touch[0] > sWobbuffetPopBalloonTouchRight[idx * 4] || balloon->touch[1] < y || balloon->touch[1] > sWobbuffetPopBalloonTouchBottom[idx * 4]) {
        return 0;
    }
    balloon->grabbing = 1;
    balloon->grabX = balloon->touch[0];
    balloon->grabY = balloon->touch[1];
    balloon->grabOffsetY = balloon->touch[1] - y;
    mid = balloon->vertices[0].topRightY + (balloon->vertices[0].topLeftY - balloon->vertices[0].topRightY) / 2;
    balloon->pushDepth = -mid;
    balloon->grabAreaPercent = WobbuffetPopBalloon_CalcMeshAreaPercent(balloon);
    return 1;
}

int WobbuffetPopBalloon_UpdateDrag(WobbuffetPopBalloon *balloon) {
    s32 mode = 0;
    s32 amount;
    s32 result;

    if (balloon->grabbing == 1) {
        s32 previous = balloon->lastTouchY;
        s32 current = balloon->touch[1];

        if (current < previous) {
            balloon->playingPumpSe = 0;
            return 0;
        }
        if (current == previous) {
            balloon->playingPumpSe = 0;
            return 1;
        }
        if (previous != -1 && current > previous && !IsSEPlaying(SEQ_SE_PL_BALLOON02)) {
            PlaySE(SEQ_SE_PL_BALLOON02);
            balloon->playingPumpSe = 1;
        }
        amount = ((balloon->touch[1] - balloon->grabOffsetY) << 12) - (balloon->pushDepth + (sWobbuffetPopBalloonTouchTop[balloon->selection * 4] << 12));
        balloon->lastTouchY = balloon->touch[1];
    } else {
        mode = 1;
        amount = -balloon->reboundSpeed;
    }
    result = WobbuffetPopBalloon_DeformMesh(balloon, mode, amount);
    if (result != 0) {
        if (result != 1 && result != 2) {
            result = result;
        } else {
            WobbuffetPopBalloon_ReleaseTouch(balloon);
        }
    }
    balloon->pushDepth = -(balloon->vertices[0].topRightY + (balloon->vertices[0].topLeftY - balloon->vertices[0].topRightY) / 2);
    return 1;
}

s32 WobbuffetPopBalloon_DeformMesh(WobbuffetPopBalloon *balloon, s32 mode, fx32 amount) {
    s32 hi;
    s32 lo;
    BOOL flag;
    s32 a;
    s32 b;
    s32 mid;
    s32 newMid;
    s32 i;
    s32 deltaMid;
    s32 angleA;
    s32 angleB;

    a = -balloon->vertices[0].topLeftY;
    b = -balloon->vertices[0].topRightY;
    mid = b + (a - b) / 2;
    flag = FALSE;

    switch (WobbuffetPopBalloon_GetPushSide(balloon, mode)) {
    case 0:
        hi = a + amount;
        lo = b + amount;
        break;
    case 1:
        flag = WobbuffetPopBalloon_CalcSidePush(balloon, amount, a, b, &hi, &lo);
        break;
    case 2:
        flag = WobbuffetPopBalloon_CalcSidePush(balloon, amount, b, a, &lo, &hi);
        break;
    }

    newMid = lo + (hi - lo) / 2;
    if (mid == newMid && a == hi && b == lo) {
        return 0;
    }
    if (newMid < 0) {
        newMid = 0;
    }
    if (hi < 0) {
        hi = 0;
    }
    if (lo < 0) {
        lo = 0;
    }
    if (newMid > FX32_CONST(100)) {
        newMid = FX32_CONST(100);
        flag = TRUE;
    }
    if (hi > FX32_CONST(100)) {
        hi = FX32_CONST(100);
        flag = TRUE;
    }
    if (lo > FX32_CONST(100)) {
        lo = FX32_CONST(100);
        flag = TRUE;
    }

    switch (WobbuffetPopBalloon_GetPushSide(balloon, mode)) {
    default:
    case 0:
        angleB = FX_Atan2Idx(lo - newMid, FX32_CONST(64));
        angleA = (u16)(angleB + 0x8000);
        break;
    case 2:
        angleB = FX_Atan2Idx(lo - newMid, FX32_CONST(64));
        angleA = (u16)(angleB + 0x8000);
        break;
    case 1:
        angleA = FX_Atan2Idx(hi - newMid, -FX32_CONST(64));
        angleB = (u16)(angleA + 0x8000);
        break;
    }

    balloon->vertices[0].topLeftX = (FX_CosIdx(angleA) << 6) + FX32_CONST(64);
    balloon->vertices[0].topLeftY = -(newMid + (FX_SinIdx(angleA) << 6));
    balloon->vertices[0].topRightX = (FX_CosIdx(angleB) << 6) + FX32_CONST(64);
    balloon->vertices[0].topRightY = -(newMid + (FX_SinIdx(angleB) << 6));
    deltaMid = (FX32_CONST(112) - newMid) / 7;
    if ((u32)angleB > 0x8000) {
        a = -(0x10000 - angleB) / 7;
    } else {
        a = angleB / 7;
    }
    b = a;
    mid = deltaMid;
    for (i = 1; i < 7; i++) {
        balloon->vertices[i].topLeftX = (FX_CosIdx(angleA - b) << 6) + FX32_CONST(64);
        balloon->vertices[i].topLeftY = -(newMid + (FX_SinIdx(angleA - b) << 6) + mid);
        balloon->vertices[i].topRightX = (FX_CosIdx(angleB - b) << 6) + FX32_CONST(64);
        balloon->vertices[i].topRightY = -(newMid + (FX_SinIdx(angleB - b) << 6) + mid);
        balloon->vertices[i - 1].bottomLeftX = balloon->vertices[i].topLeftX;
        balloon->vertices[i - 1].bottomLeftY = balloon->vertices[i].topLeftY;
        balloon->vertices[i - 1].bottomRightX = balloon->vertices[i].topRightX;
        balloon->vertices[i - 1].bottomRightY = balloon->vertices[i].topRightY;
        b += a;
        mid += deltaMid;
    }
    return flag == TRUE ? 2 : 0;
}

// 0: pushed in the middle (or released), 1: left of the handle, 2: right of it.
s32 WobbuffetPopBalloon_GetPushSide(WobbuffetPopBalloon *balloon, s32 mode) {
    s32 base;
    s32 cur;

    if (mode == 1) {
        return 0;
    }
    base = balloon->handleX;
    cur = balloon->grabX;
    if (cur < base - 0x10) {
        return 1;
    }
    if (cur > base + 0x10) {
        return 2;
    }
    return 0;
}

// The pushed side moves by the full amount (up to 100), the other side by a quarter of it.
s32 WobbuffetPopBalloon_CalcSidePush(WobbuffetPopBalloon *balloon, s32 amount, s32 pushed, s32 other, s32 *outPushed, s32 *outOther) {
    BOOL ret = FALSE;
    s32 sum;

    *outPushed = pushed;
    sum = pushed + amount;
    if (sum > 0x64000) {
        amount -= sum - 0x64000;
        ret = TRUE;
    }

    *outOther = other;
    *outPushed = pushed + amount;

    if (other > pushed) {
        *outOther = other;
    } else {
        *outOther = other - (amount * 25) / 100;
    }

    return ret;
}

// Area of the mesh outline: (top width + bottom width) * height / 2.
s32 WobbuffetPopBalloon_CalcMeshArea(WobbuffetPopBalloon *balloon) {
    s32 mid;
    s32 span;

    mid = balloon->vertices[0].topRightY + (balloon->vertices[0].topLeftY - balloon->vertices[0].topRightY) / 2;
    span = (balloon->vertices[0].topRightX - balloon->vertices[0].topLeftX) + (balloon->vertices[6].bottomRightX - balloon->vertices[6].bottomLeftX);
    return FX_Mul(span, -(balloon->vertices[6].bottomLeftY - mid)) / 2;
}

s32 WobbuffetPopBalloon_CalcMeshAreaPercent(WobbuffetPopBalloon *balloon) {
    return (s32)((s64)WobbuffetPopBalloon_CalcMeshArea(balloon) * 100 / 0x3200000);
}

void WobbuffetPop_LoadHandleSpriteResources(WobbuffetPop *game) {
    NARC *narc = NARC_New(NARC_a_1_9_9, HEAP_ID_117);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(game->paletteData, PLTTBUF_MAIN_OBJ, game->spriteSystem, game->spriteManager, narc, 0x3a, FALSE, 1, 1, 0x2715);
    SpriteSystem_LoadCharResObjFromOpenNarc(game->spriteSystem, game->spriteManager, narc, 0x37, FALSE, 1, 0x2713);
    SpriteSystem_LoadCellResObjFromOpenNarc(game->spriteSystem, game->spriteManager, narc, 0x39, FALSE, 0x2713);
    SpriteSystem_LoadAnimResObjFromOpenNarc(game->spriteSystem, game->spriteManager, narc, 0x38, FALSE, 0x2713);
    NARC_Delete(narc);
}

void WobbuffetPop_FreeHandleSpriteResources(WobbuffetPop *game) {
    SpriteManager_UnloadCharObjById(game->spriteManager, 0x2713);
    SpriteManager_UnloadCellObjById(game->spriteManager, 0x2713);
    SpriteManager_UnloadAnimObjById(game->spriteManager, 0x2713);
    SpriteManager_UnloadPlttObjById(game->spriteManager, 0x2715);
}

ManagedSprite *WobbuffetPop_CreateHandleSprite(WobbuffetPop *game) {
    ManagedSpriteTemplate tmpl = sWobbuffetPopHandleSpriteTemplate;
    ManagedSprite *sprite;

    sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &tmpl);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    Sprite_TickFrame(sprite->sprite);
    return sprite;
}

void WobbuffetPopBalloon_UpdateHandleSprite(WobbuffetPopBalloon *balloon, ManagedSprite *sprite) {
    s32 anim;

    if (balloon->grabbing != 0) {
        anim = WobbuffetPopBalloon_GetPushSide(balloon, 0) + 1;
    } else {
        anim = 0;
    }
    ManagedSprite_SetPositionXYWithSubscreenOffset(sprite, (s16)balloon->handleX, (s16)(sWobbuffetPopBalloonTouchTop[balloon->selection * 4] + (balloon->pushDepth >> 12)), 0x160000);
    ManagedSprite_SetAnim(sprite, anim);
    Sprite_TickFrame(sprite->sprite);
}

void WobbuffetPop_DeleteHandleSprite(ManagedSprite *sprite) {
    Sprite_DeleteAndFreeResources(sprite);
}
