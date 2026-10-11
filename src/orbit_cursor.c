#include "orbit_cursor.h"

#include "global.h"

#include "math_util.h"

// Sprite chain that follows a target point and then circles it (sin/cos around sprites[0]).

typedef struct Vec2f {
    float x;
    float y;
} Vec2f;

fx32 FX_Sqrt(fx32 x);
fx32 GF_SinDeg(u16 deg);
fx32 GF_CosDeg(u16 deg);
u16 FX_Atan2Idx(fx32 y, fx32 x);

BOOL OrbitCursor_StepToward(s16 x, s16 y, float fx, float fy, float *outX, float *outY, float speed, s16 minDist);

OrbitCursor *OrbitCursor_New(volatile int count, volatile enum HeapID heapId, volatile int resBase, volatile int unk1C, fx32 yOffset, SpriteSystem *spriteSys, SpriteManager *spriteMan, PaletteData *plttData) {
    enum HeapID heap = heapId;
    OrbitCursor *motion = Heap_Alloc(heap, sizeof(OrbitCursor));

    motion->count = count;
    motion->heapId = heap;
    motion->resBase = resBase;
    motion->unk_1C = unk1C;
    motion->yOffset = yOffset;
    motion->spriteSys = spriteSys;
    motion->spriteMan = spriteMan;
    motion->plttData = plttData;
    motion->state = 0;
    motion->radiusX = 15;
    motion->radiusY = 15;
    motion->task = NULL;
    motion->active = 0;
    return motion;
}

const int gOrbitCursorPaletteMembers[] = {
    0x61,
    0x62,
    0x63,
    0x64,
    0x65,
    0x80,
    0x85,
};

BOOL OrbitCursor_StepToward(s16 x, s16 y, float fx, float fy, float *outX, float *outY, float speed, s16 minDist) {
    Vec2f v;
    Vec2f d;
    Vec2f o;
    float s;
    float dist;

    o.x = 0.0f;
    o.y = 0.0f;
    d.x = (float)x - fx;
    d.y = (float)y - fy;
    v.x = 0.0f;
    v.y = 0.0f;

    s = d.x * d.x + d.y * d.y;
    dist = (float)FX_Sqrt((fx32)(s > 0.0f ? 0.5f + 4096.0f * s : 4096.0f * s - 0.5f)) / 4096.0f;
    if (dist < speed || (float)minDist > dist || dist == 0.0f) {
        return FALSE;
    }
    v.x = d.x * speed / dist;
    v.y = d.y * speed / dist;
    *outX = v.x + o.x;
    *outY = v.y + o.y;
    return TRUE;
}

#pragma force_active on

void OrbitCursor_Task(SysTask *task, void *data) {
    OrbitCursor *p = data;
    fx32 posX, posY;
    float outX, outY;
    s16 pos[2];
    int i;
    int done;
    float fx, fy;
    if (p->active == 0) {
        return;
    }
    done = 0;
    for (i = 0; i < p->count; i++) {
        if (i != 0) {
            p->sprites[i].x = p->sprites[i - 1].x;
            p->sprites[i].y = p->sprites[i - 1].y;
        }
        ManagedSprite_GetPositionFxXYWithSubscreenOffset(p->sprites[i].sprite, &posX, &posY, p->yOffset);
        fx = (float)posX / 4096.0f;
        fy = (float)posY / 4096.0f;
        if (OrbitCursor_StepToward(p->sprites[i].x, p->sprites[i].y, fx, fy, &outX, &outY, (8.0f - 0.5f * (float)(i - done)) / 2.0f, 16) && p->sprites[i].unk_08 == 0) {
            float ry = outY > 0.0f ? 0.5f + 4096.0f * outY : 4096.0f * outY - 0.5f;
            ManagedSprite_AddSpritePrecisePositionXY(p->sprites[i].sprite, (fx32)(outX > 0.0f ? 0.5f + 4096.0f * outX : 4096.0f * outX - 0.5f), (fx32)ry);
            continue;
        }
        switch (p->sprites[i].unk_08) {
        case 0:
            ManagedSprite_GetPositionXYWithSubscreenOffset(p->sprites[i].sprite, &pos[1], &pos[0], p->yOffset);
            if (p->unk_E8 == 0xFF) {
                int d;
                float ax, ay;
                p->unk_E8 = i;
                d = p->sprites[0].x - pos[1];
                ax = d > 0 ? 0.5f + (float)(d << 12) : (float)(d << 12) - 0.5f;
                d = p->sprites[0].y - pos[0];
                ay = d > 0 ? 0.5f + (float)(d << 12) : (float)(d << 12) - 0.5f;
                p->sprites[i].unk_0C = FX_Atan2Idx((fx32)ay, (fx32)ax);
            } else {
                p->sprites[i].unk_0C = p->sprites[p->unk_E8].unk_0C - (720 / p->count) * p->unk_EC;
            }
            p->sprites[i].unk_0C %= 720;
            p->unk_EC++;
            p->sprites[i].unk_08++;
            break;
        case 1:
            p->sprites[i].unk_0C += 4;
            p->sprites[i].unk_0C %= 720;
            {
                fx32 x = (p->sprites[0].x << 12) + GF_SinDeg(p->sprites[i].unk_0C) * p->radiusX;
                fx32 y = (p->sprites[0].y << 12) + GF_CosDeg(p->sprites[i].unk_0C) * p->radiusY;
                ManagedSprite_SetPositionFxXYWithSubscreenOffset(p->sprites[i].sprite, x, y, p->yOffset);
            }
            break;
        }
        done++;
    }
}

#pragma force_active reset

void OrbitCursor_ReloadPalette(OrbitCursor *p, int a1) {
    SpriteSystem *ss = p->spriteSys;
    NARC *narc;
    SpriteManager *sm = p->spriteMan;
    PaletteData *pd = p->plttData;
    narc = p->narc;

    SpriteManager_UnloadPlttObjById(sm, 0x56CF);
    SpriteManager_UnloadPlttObjById(sm, 0x56D0);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(pd, PLTTBUF_MAIN_OBJ, ss, sm, narc, gOrbitCursorPaletteMembers[a1], FALSE, 1, 1, 0x56CF);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(pd, PLTTBUF_SUB_OBJ, ss, sm, narc, gOrbitCursorPaletteMembers[a1], FALSE, 1, 2, 0x56D0);
}

void OrbitCursor_LoadResources(OrbitCursor *p, int a1) {
    int resBase = p->resBase;
    NARC *narc;
    SpriteSystem *ss = p->spriteSys;
    int resId = resBase + 0x56CE;
    SpriteManager *sm = p->spriteMan;
    PaletteData *pd = p->plttData;
    narc = p->narc;

    if (resBase == 1) {
        if (p->unk_1C == 0) {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(pd, PLTTBUF_MAIN_OBJ, ss, sm, narc, gOrbitCursorPaletteMembers[a1], FALSE, 1, 1, resId);
        } else {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(pd, PLTTBUF_MAIN_OBJ, ss, sm, narc, 0x60, FALSE, 1, 1, resId);
        }
        SpriteSystem_LoadCharResObjFromOpenNarc(ss, sm, narc, 0x5F, FALSE, 1, resId);
    } else {
        if (p->unk_1C == 0) {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(pd, PLTTBUF_SUB_OBJ, ss, sm, narc, gOrbitCursorPaletteMembers[a1], FALSE, 1, 2, resId);
        } else {
            SpriteSystem_LoadPaletteBufferFromOpenNarc(pd, PLTTBUF_SUB_OBJ, ss, sm, narc, 0x60, FALSE, 1, 2, resId);
        }
        SpriteSystem_LoadCharResObjFromOpenNarc(ss, sm, narc, 0x5F, FALSE, 2, resId);
    }
    SpriteSystem_LoadCellResObjFromOpenNarc(ss, sm, narc, 0x5D, FALSE, resId);
    SpriteSystem_LoadAnimResObjFromOpenNarc(ss, sm, narc, 0x5E, FALSE, resId);
}

void OrbitCursor_CreateSprites(OrbitCursor *p) {
    ManagedSpriteTemplate tmpl;
    SpriteSystem *ss = p->spriteSys;
    SpriteManager *sm = p->spriteMan;
    int i = 0;

    tmpl.x = 0x80;
    tmpl.y = 0x60;
    tmpl.z = 0;
    tmpl.animation = 0;
    tmpl.drawPriority = 0;
    tmpl.vram = (NNS_G2D_VRAM_TYPE)p->resBase;
    tmpl.bgPriority = 0;
    tmpl.vramTransfer = 0;
    tmpl.pal = 0;
    tmpl.resIdList[0] = p->resBase + 0x56CE;
    tmpl.resIdList[1] = p->resBase + 0x56CE;
    tmpl.resIdList[2] = p->resBase + 0x56CE;
    tmpl.resIdList[3] = p->resBase + 0x56CE;
    tmpl.resIdList[4] = -1;
    tmpl.resIdList[5] = -1;

    for (i = 0; i < p->count; i++) {
        p->sprites[i].sprite = SpriteSystem_NewSprite(ss, sm, &tmpl);
        ManagedSprite_TickFrame(p->sprites[i].sprite);
        ManagedSprite_SetPositionXY(p->sprites[i].sprite, 0x80, 0x60);
    }
}

void OrbitCursor_DeleteSprites(OrbitCursor *p) {
    int i;
    for (i = 0; i < p->count; i++) {
        SpriteManager_UnloadCharObjById(p->spriteMan, p->resBase + 0x56CE);
        SpriteManager_UnloadCellObjById(p->spriteMan, p->resBase + 0x56CE);
        SpriteManager_UnloadAnimObjById(p->spriteMan, p->resBase + 0x56CE);
        Sprite_DeleteAndFreeResources(p->sprites[i].sprite);
    }
}

BOOL OrbitCursor_Start(OrbitCursor *p, int a1) {
    p->narc = NARC_New((NarcId)0xBF, p->heapId);
    OrbitCursor_LoadResources(p, a1);
    OrbitCursor_CreateSprites(p);
    p->task = SysTask_CreateOnVBlankQueue(OrbitCursor_Task, p, 0x1000);
    return TRUE;
}

BOOL OrbitCursor_SetActive(OrbitCursor *p, int a1) {
    p->active = a1;
    return TRUE;
}

BOOL OrbitCursor_SetTarget(OrbitCursor *p, int x, int y) {
    int i;
    p->sprites[0].x = x;
    p->sprites[0].y = y;
    p->unk_E8 = 0xFF;
    p->unk_EC = 0;
    for (i = 0; i < p->count; i++) {
        p->sprites[i].unk_08 = 0;
    }
    return TRUE;
}

BOOL OrbitCursor_MoveTarget(OrbitCursor *p, int x, int y) {
    int i;
    if (!OrbitCursor_IsNewTarget(p, x, y)) {
        return FALSE;
    }
    p->sprites[0].x = x;
    p->sprites[0].y = y;
    p->unk_E8 = 0xFF;
    p->unk_EC = 0;
    for (i = 0; i < p->count; i++) {
        p->sprites[i].unk_08 = 0;
    }
    return TRUE;
}

BOOL OrbitCursor_IsNewTarget(OrbitCursor *p, int x, int y) {
    if (p->sprites[0].x == x && p->sprites[0].y == y) {
        return FALSE;
    }
    return TRUE;
}

BOOL OrbitCursor_SetPosition(OrbitCursor *p, s16 x, s16 y) {
    int i;
    p->unk_E8 = 0xFF;
    p->unk_EC = 0;
    for (i = 0; i < p->count; i++) {
        p->sprites[i].x = x;
        p->sprites[i].y = y;
        ManagedSprite_SetPositionXYWithSubscreenOffset(p->sprites[i].sprite, x, y, p->yOffset);
    }
    return TRUE;
}

BOOL OrbitCursor_Delete(OrbitCursor *p) {
    switch (p->state) {
    case 0:
        OrbitCursor_SetActive(p, FALSE);
        p->state++;
        break;
    case 1:
        SysTask_Destroy(p->task);
        p->state++;
        break;
    default:
        OrbitCursor_DeleteSprites(p);
        NARC_Delete(p->narc);
        Heap_Free(p);
        return FALSE;
    }
    return TRUE;
}

BOOL OrbitCursor_SetVisible(OrbitCursor *p, int flag) {
    int i;
    for (i = 0; i < p->count; i++) {
        ManagedSprite_SetDrawFlag(p->sprites[i].sprite, flag);
    }
    return TRUE;
}

void OrbitCursor_SetRadius(OrbitCursor *p, int a1, int a2) {
    if (a1 != 0 && a2 != 0) {
        p->radiusX = a1;
        p->radiusY = a2;
    } else {
        p->radiusX = 15;
        p->radiusY = 15;
    }
}

void sub_02087A30(OrbitCursor *p) {
    int i;
    for (i = 0; i < p->count; i++) {
        ManagedSprite_SetAnim(p->sprites[i].sprite, 1);
    }
}

void sub_02087A54(OrbitCursor *p) {
    int i;
    for (i = 0; i < p->count; i++) {
        ManagedSprite_SetAnim(p->sprites[i].sprite, 0);
    }
}
