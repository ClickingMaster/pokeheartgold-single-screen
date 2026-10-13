#include "gf_gfx_loader.h"
#include "overlay_42.h"

const u8 sActionAnimKinds[] = { 0, 3, 2, 4, 0, 5, 6, 8, 7, 8, 2, 6 };
const u8 sAnimKindKeepsFrame[] = { 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0 };
const BgTemplate sField2DBgLayerBgTemplate = {
    .x = 0,
    .y = 0,
    .bufferSize = 4096,
    .baseTile = 0,
    .size = 3,
    .colorMode = 0,
    .screenBase = 0,
    .charBase = 0,
    .bgExtPltt = 0,
    .priority = 0,
    .areaOver = 0,
    .dummy = 0,
    .mosaic = 0,
};

s32 Field2DCommand_CheckStand(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    int temp_r0 = Field2DObject_GetParam(arg0, 5);
    if ((temp_r0 == 0) || (temp_r0 == 4)) {
        Field2DMove_Init(arg4, arg0, arg3->unk0, arg3->unk3, arg3->unk2);
        return 1;
    }
    return 0;
}

s32 Field2DCommand_CheckTurn(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    int temp_r6 = Field2DObject_GetParam(arg0, 5);
    int temp_r0 = Field2DObject_GetParam(arg0, 6);
    if (temp_r6 == 0 && temp_r0 != arg3->unk2) {
        Field2DMove_Init(arg4, arg0, arg3->unk0, arg3->unk3, arg3->unk2);
        return 1;
    }
    return 0;
}

s32 Field2DCommand_CheckWalk(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    int temp_r0 = Field2DObject_GetParam(arg0, 5);
    if ((temp_r0 == 0) && (Field2DObject_IsMapBlockedInDirection(arg0, arg1, arg3->unk2) != 1) && (Field2DObject_IsObjectInDirection(arg0, arg2, arg3->unk2) != 1)) {
        Field2DMove_Init(arg4, arg0, arg3->unk0, arg3->unk3, arg3->unk2);
        return 1;
    }
    if ((temp_r0 == 0) && (Field2DObject_GetParam(arg0, 10) >= 8)) {
        Field2DMove_Init(arg4, arg0, 5U, arg3->unk3, arg3->unk2);
        return 1;
    }
    return 0;
}

s32 Field2DCommand_CheckRun(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    return Field2DCommand_CheckWalk(arg0, arg1, arg2, arg3, arg4);
}

s32 Field2DCommand_CheckPause(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    if (Field2DObject_GetParam(arg0, 5) == 0) {
        Field2DMove_Init(arg4, arg0, arg3->unk0, arg3->unk3, arg3->unk2);
        return 1;
    }
    return 0;
}

s32 Field2DCommand_CheckBump(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    GF_ASSERT(FALSE);
    return 0;
}

s32 Field2DCommand_CheckWalkSlow(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    return Field2DCommand_CheckWalk(arg0, arg1, arg2, arg3, arg4);
}

s32 Field2DCommand_CheckWalkInPlace(Field2DObject *arg0, Field2DMap *arg1, Field2DObjectManager *arg2, Field2DCommand *arg3, Field2DMove *arg4) {
    if (Field2DObject_GetParam(arg0, 5) == 0) {
        Field2DMove_Init(arg4, arg0, arg3->unk0, arg3->unk3, arg3->unk2);
        return TRUE;
    }
    return FALSE;
}

s32 Field2DObject_IsMapBlockedInDirection(Field2DObject *arg0, Field2DMap *arg1, u8 arg2) {
    Field2DPoint sp0 = Field2DObject_GetPos(arg0);
    sp0 = Field2D_StepInDirection(sp0, arg2);
    return Field2DMap_IsBlocked(arg1, sp0.unk0 / 16, sp0.unk2 / 16);
}

BOOL Field2DObject_IsObjectInDirection(Field2DObject *arg0, Field2DObjectManager *arg1, u8 arg2) {
    if (Field2DObject_FindObjectInDirection(arg0, arg1, arg2) != 0) {
        return TRUE;
    }
    return FALSE;
}

void Field2DMove_Init(Field2DMove *arg0, Field2DObject *arg1, u16 arg2, u8 arg3, s32 arg4) {
    arg0->unk0 = Field2DObject_GetPos(arg1);
    arg0->unk4 = arg2;
    arg0->unk7 = arg3;
    arg0->unk6 = arg4;
}

Field2DObjectSpriteManager *Field2DObjectSpriteManager_New(SpriteList *arg0, PaletteData *arg1, u16 arg2, s8 arg3, enum HeapID heapID) {
    Field2DObjectSpriteManager *temp_r4 = Heap_Alloc(heapID, sizeof(Field2DObjectSpriteManager));
    temp_r4->charSprites = CharSpriteManager_New(arg0, arg1, arg2, heapID);
    temp_r4->maxSprites = arg2;
    Field2DObjectSprite *temp_r0 = Heap_Alloc(heapID, 20 * temp_r4->maxSprites);
    temp_r4->sprites = temp_r0;
    memset(temp_r0, 0, 20 * temp_r4->maxSprites);
    temp_r4->bgPriority = 2;
    temp_r4->vramType = arg3;
    return temp_r4;
}

Field2DObjectSpriteManager *Field2DObjectSpriteManager_NewWithShadow(SpriteList *arg0, PaletteData *arg1, u16 arg2, s32 arg3, s32 arg4, s32 arg5, enum HeapID heapID) {
    Field2DObjectSpriteManager *temp_r4 = Heap_Alloc(heapID, sizeof(Field2DObjectSpriteManager));
    temp_r4->charSprites = CharSpriteManager_New(arg0, arg1, arg2, heapID);
    temp_r4->maxSprites = arg2;
    Field2DObjectSprite *temp_r0 = Heap_Alloc(heapID, 20 * temp_r4->maxSprites);
    temp_r4->sprites = temp_r0;
    memset(temp_r0, 0, 20 * temp_r4->maxSprites);
    temp_r4->bgPriority = 2;
    temp_r4->vramType = arg5;
    CharSpriteManager_LoadChar(temp_r4->charSprites, arg3, temp_r4->vramType, arg4, heapID);
    CharSpriteManager_LoadShadow(temp_r4->charSprites, temp_r4->vramType, 40001, heapID);
    return temp_r4;
}

void Field2DObjectSpriteManager_Free(Field2DObjectSpriteManager *arg0) {
    for (int i = 0; i < arg0->maxSprites; i++) {
        if (Field2DObjectSprite_IsFree(&arg0->sprites[i]) == 0) {
            Field2DObjectSprite_Delete(&arg0->sprites[i]);
        }
    }
    CharSpriteManager_UnloadShadow(arg0->charSprites);
    Field2DObjectSpriteManager_UnloadAllChars(arg0);
    CharSpriteManager_Free(arg0->charSprites);
    Heap_Free(arg0->sprites);
    Heap_Free(arg0);
}

void Field2DObjectSpriteManager_LoadChar(Field2DObjectSpriteManager *arg0, s32 arg1, s32 arg2, enum HeapID heapID) {
    if (CharSpriteManager_IsCharLoaded(arg0->charSprites, arg1) == 0) {
        CharSpriteManager_LoadChar(arg0->charSprites, arg1, arg0->vramType, arg2, heapID);
    }
}

void Field2DObjectSpriteManager_UnloadChar(Field2DObjectSpriteManager *arg0, s32 arg1) {
    CharSpriteManager_UnloadChar(arg0->charSprites, arg1);
}

s32 Field2DObjectSpriteManager_IsCharLoaded(Field2DObjectSpriteManager *arg0, s32 arg1) {
    return CharSpriteManager_IsCharLoaded(arg0->charSprites, arg1);
}

void Field2DObjectSpriteManager_UnloadAllChars(Field2DObjectSpriteManager *arg0) {
    CharSpriteManager_UnloadAllChars(arg0->charSprites);
}

void Field2DObjectSpriteManager_LoadAvatarChars(Field2DObjectSpriteManager *arg0, s32 arg1, enum HeapID heapID) {
    CharSpriteManager_LoadAvatarChars(arg0->charSprites, arg0->vramType, arg1, heapID);
}

Field2DObjectSprite *Field2DObjectSpriteManager_CreateSprite(Field2DObjectSpriteManager *arg0, Field2DObject *arg1, u32 arg2, enum HeapID heapID) {
    Field2DObjectSprite *temp_r4 = Field2DObjectSpriteManager_GetFreeSprite(arg0);
    temp_r4->action = Field2DObject_GetParam(arg1, 5);
    temp_r4->direction = Field2DObject_GetParam(arg1, 6);
    temp_r4->id = Field2DObject_GetParam(arg1, 4);
    temp_r4->object = arg1;
    temp_r4->priorityMode = arg2;
    temp_r4->autoUpdate = 1;
    temp_r4->savedAnimFrame = 0;
    temp_r4->savedAnimKind = 0;

    s16 sp4[4];
    sp4[0] = Field2DObject_GetParam(arg1, 0);
    sp4[1] = Field2DObject_GetParam(arg1, 1);
    sp4[2] = Field2DObjectSprite_CalcDrawPriority(sp4[1], temp_r4->priorityMode);
    sp4[3] = arg0->bgPriority;
    temp_r4->charSprite = CharSpriteManager_CreateSprite(arg0->charSprites, sp4, Field2DObject_GetParam(arg1, 7), heapID);
    Field2DObjectSprite_Update(temp_r4);
    return temp_r4;
}

void Field2DObjectSprite_Delete(Field2DObjectSprite *arg0) {
    CharSprite_Delete(arg0->charSprite);
    memset(arg0, 0, sizeof(Field2DObjectSprite));
}

void Field2DObjectSpriteManager_Update(Field2DObjectSpriteManager *arg0) {
    for (int i = 0; i < arg0->maxSprites; i++) {
        if (Field2DObjectSprite_IsFree(&arg0->sprites[i]) == 0) {
            Field2DObjectSprite_Update(&arg0->sprites[i]);
        }
    }
}

void Field2DObjectSprite_Update(Field2DObjectSprite *arg0) {
    if (arg0->autoUpdate != 0) {
        s32 temp_r6 = Field2DObject_GetParam(arg0->object, 5);
        s32 temp_r4 = Field2DObject_GetParam(arg0->object, 6);
        s16 temp_r1 = Field2DObject_GetParam(arg0->object, 8);
        if ((arg0->action != temp_r6) || (arg0->direction != temp_r4) || (temp_r1 == 0)) {
            arg0->action = temp_r6;
            arg0->direction = temp_r4;
            Field2DObjectSprite_StartAnim(arg0, Field2DObjectSprite_GetAnimKindForAction(arg0->charSprite, temp_r6), temp_r4);
        }
        Field2DPoint sp0;
        Field2DObjectSprite_GetDrawPos(arg0, &sp0.unk2, &sp0.unk0);
        CharSprite_SetPosition(arg0->charSprite, sp0.unk2, sp0.unk0);
        u16 r1 = Field2DObjectSprite_CalcDrawPriority(sp0.unk0, arg0->priorityMode);
        CharSprite_SetDrawPriority(arg0->charSprite, r1);
        CharSprite_UpdateAnim(arg0->charSprite);
    }
}

void Field2DObjectSprite_SetAutoUpdate(Field2DObjectSprite *arg0, s8 arg1) {
    arg0->autoUpdate = arg1;
}

void Field2DObjectSprite_StartLoopAnim(Field2DObjectSprite *arg0) {
    Field2DObjectSprite_StartAnim(arg0, 1U, arg0->direction);
}

void Field2DObjectSprite_UpdateAnim(Field2DObjectSprite *arg0) {
    CharSprite_UpdateAnim(arg0->charSprite);
}

void Field2DObjectSprite_SyncAnim(Field2DObjectSprite *arg0) {
    Field2DObjectSprite_StartAnim(arg0, Field2DObjectSprite_GetAnimKindForAction(arg0->charSprite, arg0->action), arg0->direction);
    Field2DObjectSprite_Update(arg0);
}

void Field2DObjectSprite_SetPosition(Field2DObjectSprite *arg0, Field2DPoint arg1) {
    CharSprite_SetPosition(arg0->charSprite, arg1.unk0, arg1.unk2);
}

s32 Field2DObjectSprite_IsVisible(Field2DObjectSprite *arg0) {
    return CharSprite_IsVisible(arg0->charSprite);
}

void Field2DObjectSprite_SetVisible(Field2DObjectSprite *arg0, u16 arg1) {
    CharSprite_SetVisible(arg0->charSprite, arg1);
}

u16 Field2DObjectSprite_GetDrawPriority(Field2DObjectSprite *arg0) {
    return CharSprite_GetDrawPriority(arg0->charSprite);
}

void Field2DObjectSprite_FaceDirection(Field2DObjectSprite *arg0, s32 arg1) {
    Field2DObjectSprite_StartAnim(arg0, 0, arg1);
    CharSprite_UpdateAnim(arg0->charSprite);
}

u32 Field2DObjectSprite_CalcDrawPriority(s16 arg0, u8 arg1) {
    if (arg1 == 1) {
        return 40000 - (arg0 * 2);
    }
    return 40001 - (arg0 * 2);
}

u8 Field2DObjectSprite_GetPaletteOffset(Field2DObjectSprite *arg0) {
    return Sprite_GetPalOffset(CharSprite_GetSprite(arg0->charSprite));
}

void Field2DObjectSprite_SetShadowPosition(Field2DObjectSprite *arg0, Field2DPoint arg1) {
    CharSprite_SetShadowPosition(arg0->charSprite, arg1.unk0, arg1.unk2);
}

s32 Field2DObjectSprite_IsFree(Field2DObjectSprite *arg0) {
    if (arg0->charSprite == 0) {
        return 1;
    }
    return 0;
}

Field2DObjectSprite *Field2DObjectSpriteManager_GetFreeSprite(Field2DObjectSpriteManager *arg0) {
    for (int i = 0; i < arg0->maxSprites; i++) {
        if (Field2DObjectSprite_IsFree(&arg0->sprites[i]) == 1) {
            return &arg0->sprites[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

u8 Field2DObjectSprite_GetAnimKindForAction(CharSprite *arg0, s32 arg1) {
    if (CharSprite_GetAnimSet(arg0) == 2) {
        return 0;
    }
    return sActionAnimKinds[arg1];
}

void Field2DObjectSprite_GetDrawPos(Field2DObjectSprite *arg0, s16 *arg1, s16 *arg2) {
    Field2DPoint sp0 = Field2DObject_GetDrawPos(arg0->object);
    *arg1 = sp0.unk0;
    *arg2 = sp0.unk2;
}

void Field2DObjectSprite_StartAnim(Field2DObjectSprite *arg0, u8 arg1, s32 arg2) {
    u16 temp_r6;
    u16 temp_r7;

    temp_r7 = CharSprite_GetAnimFrame(arg0->charSprite);
    temp_r6 = CharSprite_GetAnimKind(arg0->charSprite);
    CharSprite_StartAnim(arg0->charSprite, arg1, arg2);
    if (temp_r6 != arg1) {
        if ((sAnimKindKeepsFrame[arg1] == 1) && (arg0->savedAnimKind == arg1)) {
            CharSprite_SetAnimFrame(arg0->charSprite, arg0->savedAnimFrame);
        }
        if (sAnimKindKeepsFrame[temp_r6] == 1) {
            arg0->savedAnimKind = temp_r6;
            arg0->savedAnimFrame = temp_r7;
        }
    }
}

void Field2DCamera_CenterOnObject(Field2DPoint *arg0, Field2DObject *arg1) {
    Field2DPoint sp0 = Field2DObject_GetDrawPos(arg1);
    Field2DCamera_Set(arg0, sp0.unk2 - 96, sp0.unk0 - 112);
}

void Field2DCamera_Init(Field2DPoint *arg0) {
    memset(arg0, 0, 4);
}

void Field2DCamera_Set(Field2DPoint *arg0, s16 arg1, s16 arg2) {
    arg0->unk0 = arg1;
    arg0->unk2 = arg2;
}

s16 Field2DCamera_GetY(Field2DPoint *arg0) {
    return arg0->unk0;
}

s16 Field2DCamera_GetX(Field2DPoint *arg0) {
    return arg0->unk2;
}

Field2DBgLayer *Field2DBgLayer_New(G2dRenderer *g2dRenderer, BgConfig *bgConfig, const Field2DBgLayerTemplate *arg2, enum HeapID heapID) {
    Field2DBgLayer *temp_r0 = Heap_Alloc(heapID, sizeof(Field2DBgLayer));
    memset(temp_r0, 0, sizeof(Field2DBgLayer));
    temp_r0->g2dRenderer = g2dRenderer;
    temp_r0->bgConfig = bgConfig;
    temp_r0->screen = arg2->unk_0;
    temp_r0->bgLayer = arg2->unk_1;
    Field2DBgLayer_InitBg(temp_r0->bgConfig, temp_r0->bgLayer, arg2);
    temp_r0->screenFile = GfGfxLoader_GetScrnData((NarcId)arg2->narcId, arg2->fileId, arg2->compressed, &temp_r0->nnsg2dScreenData, heapID);
    return temp_r0;
}

void Field2DBgLayer_Free(Field2DBgLayer *arg0) {
    Heap_Free(arg0->screenFile);
    Heap_Free(arg0);
}

void Field2DBgLayer_SetCamera(Field2DBgLayer *arg0, Field2DPoint *arg1) {
    s16 temp_r4;
    s16 temp_r5;
    temp_r5 = Field2DCamera_GetY(arg1);
    temp_r4 = Field2DCamera_GetX(arg1);
    if (arg0->screen == 0) {
        f32 var_r0;
        f32 var_r0_2;
        if (temp_r5 > 0) {
            var_r0 = 0.5f + (temp_r5 << 12);
        } else {
            var_r0 = (temp_r5 << 12) - 0.5f;
        }
        if (temp_r4 > 0) {
            var_r0_2 = 0.5f + (temp_r4 << 12);
        } else {
            var_r0_2 = (temp_r4 << 12) - 0.5f;
        }
        G2dRenderer_SetMainSurfaceCoords(arg0->g2dRenderer, var_r0_2, var_r0);
    } else {
        f32 var_r0_3;
        f32 var_r0_4;
        if (temp_r5 > 0) {
            var_r0_3 = 0.5f + (temp_r5 << 12);
        } else {
            var_r0_3 = (temp_r5 << 12) - 0.5f;
        }
        if (temp_r4 > 0) {
            var_r0_4 = 0.5f + (temp_r4 << 12);
        } else {
            var_r0_4 = (temp_r4 << 12) - 0.5f;
        }
        G2dRenderer_SetSubSurfaceCoords(arg0->g2dRenderer, var_r0_4, (fx32)var_r0_3 + 0xC0000);
    }
    Field2DBgLayer_Scroll(arg0, temp_r4, temp_r5);
}

void Field2DBgLayer_InitBg(BgConfig *bgConfig, s32 arg1, const Field2DBgLayerTemplate *arg2) {
    BgTemplate bgTemplate = sField2DBgLayerBgTemplate;
    bgTemplate.colorMode = arg2->colorMode;
    bgTemplate.screenBase = arg2->screenBase;
    bgTemplate.charBase = arg2->charBase;
    bgTemplate.bgExtPltt = arg2->bgExtPltt;
    bgTemplate.priority = arg2->priority;
    bgTemplate.areaOver = 0;
    bgTemplate.mosaic = arg2->mosaic;
    FreeBgTilemapBuffer(bgConfig, arg1);
    InitBgFromTemplate(bgConfig, arg1, &bgTemplate, GF_BG_TYPE_TEXT);
}

void Field2DBgLayer_Scroll(Field2DBgLayer *arg0, s16 arg1, s16 arg2) {
    s16 temp_r3 = arg1 / 8;
    s16 temp_r0 = arg2 / 8;
    s16 temp_r6 = arg1 % 8;
    s16 temp_r4_3 = arg2 % 8;

    if ((temp_r3 != arg0->tileX) || (temp_r0 != arg0->tileY)) {
        arg0->tileX = temp_r3;
        arg0->tileY = temp_r0;
        Field2DBgLayer_CopyVisibleTiles(arg0->bgConfig, arg0->bgLayer, arg0->nnsg2dScreenData, -temp_r3, -temp_r0);
        ScheduleBgTilemapBufferTransfer(arg0->bgConfig, arg0->bgLayer);
    }
    ScheduleSetBgPosText(arg0->bgConfig, arg0->bgLayer, BG_POS_OP_SET_X, temp_r6);
    ScheduleSetBgPosText(arg0->bgConfig, arg0->bgLayer, BG_POS_OP_SET_Y, temp_r4_3);
}

void Field2DBgLayer_CopyVisibleTiles(BgConfig *bgConfig, s32 arg1, NNSG2dScreenData *nnsg2dScreenData, s16 arg3, s16 arg4) {
    s16 sp34 = nnsg2dScreenData->screenWidth / 8;
    s16 sp30 = nnsg2dScreenData->screenHeight / 8;

    s16 sp2C;
    if (arg3 < 0) {
        sp2C = -arg3;
    } else {
        sp2C = 0;
    }
    s16 sp28;
    if (arg4 < 0) {
        sp28 = -arg4;
    } else {
        sp28 = 0;
    }
    if (arg3 <= 0) {
        arg3 = 0;
    }
    if (arg4 <= 0) {
        arg4 = 0;
    }
    s16 var_r7 = 33 - arg3;
    s16 var_r6 = 25 - arg4;
    s32 temp_r1 = sp2C + var_r7;
    if (sp34 < temp_r1) {
        var_r7 -= temp_r1 - sp34;
    }
    s32 temp_r1_2 = sp28 + var_r6;
    if (sp30 < temp_r1_2) {
        var_r6 -= temp_r1_2 - sp30;
    }
    FillBgTilemapRect(bgConfig, arg1, 0, 0, 0, 33, 25, 17);
    Field2DBgLayer_CopyTilemapRect(bgConfig, arg1, arg3, arg4, var_r7, var_r6, nnsg2dScreenData->rawData, sp2C, sp28, sp34, sp30);
}

void Field2DBgLayer_CopyTilemapRect(BgConfig *bgConfig, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, void *arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA) {
    s8 sp48;
    s8 sp44;
    u8 sp40 = Field2DBgLayer_GetScreenBlockLayout(arg9, argA);
    if (sp40 == 0) {
        CopyToBgTilemapRect(bgConfig, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, argA);
        return;
    }
    sp48 = arg5;
    while (sp48 > 0) {
        int sp4C = arg8 % 32;
        if (sp4C + sp48 <= 32) {
            sp44 = sp48;
            sp48 = 0;
        } else {
            sp44 = 32 - sp4C;
            sp48 -= sp44;
        }
        s8 r1 = arg8 / 32;
        s8 r4 = arg4;
        s8 r5 = arg7;
        s8 r7 = arg2;
        while (r4 > 0) {
            s8 r6;
            if ((r5 % 32) + r4 <= 32) {
                r6 = r4;
                r4 = 0;
            } else {
                r6 = 32 - (r5 % 32);
                r4 = r4 - r6;
            }
            Field2DPoint sp4;
            void *temp = Field2DBgLayer_GetScreenBlock(arg6, (s8)(r5 / 32), r1, sp40, arg9, argA, &sp4);
            CopyToBgTilemapRect(bgConfig, arg1, r7, arg3, r6, sp44, temp, r5 % 32, sp4C, sp4.unk0, sp4.unk2);
            r5 += r6;
            r7 += r6;
        }
        arg8 += sp44;
        arg3 += sp44;
    }
}

u8 Field2DBgLayer_GetScreenBlockLayout(u8 arg0, u8 arg1) {
    if (arg0 > 64) {
        GF_ASSERT(FALSE);
    }
    if (arg1 > 64) {
        GF_ASSERT(FALSE);
    }
    if (arg0 <= 32) {
        if (arg1 <= 32) {
            return 0;
        }
        return 2;
    }
    if (arg1 <= 32) {
        return 1;
    }
    return 3;
}

void *Field2DBgLayer_GetScreenBlock(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, Field2DPoint *arg6) {
    switch (arg3) {
    case 0:
        arg6->unk0 = arg4;
        arg6->unk2 = arg5;
        return arg0;
    case 1:
        int var_r4;
        if (((arg1 + 1) << 5) <= arg4) {
            var_r4 = 32;
        } else {
            var_r4 = arg4 - (arg1 << 5);
        }
        arg6->unk0 = var_r4;
        arg6->unk2 = arg5;
        return arg0 + arg1 * ((s16)(arg5 << 6));
    case 2:
        arg6->unk0 = arg4;
        int var_r1;
        if (((arg2 + 1) << 5) <= arg5) {
            var_r1 = 32;
        } else {
            var_r1 = arg5 - (arg2 << 5);
        }
        arg6->unk2 = var_r1;
        return arg0 + arg2 * ((s16)(arg4 << 6));
    case 3:
        int var_r5;
        if (((arg1 + 1) << 5) <= arg4) {
            var_r5 = 32;
        } else {
            var_r5 = arg4 - (arg1 << 5);
        }
        arg6->unk0 = var_r5;
        if (((arg2 + 1) << 5) <= arg5) {
            var_r5 = 32;
        } else {
            var_r5 = arg5 - (arg2 << 5);
        }
        arg6->unk2 = var_r5;
        u16 var_r2;
        if (arg2 == 0) {
            var_r2 = 0;
            if (arg1 != 0) {
                var_r2 += 2048;
            }
        } else {
            var_r2 = (arg4 << 6);
            if (arg1 != 0) {
                var_r2 = var_r2 + (arg5 << 6);
            }
        }
        return arg0 + var_r2;
    default:
        GF_ASSERT(FALSE);
    }
    GF_ASSERT(FALSE);
    return NULL;
}

Field2DCommandQueue *Field2DMoveQueue_New(s32 arg0, enum HeapID heapID) {
    Field2DCommandQueue *temp_r0 = Heap_Alloc(heapID, sizeof(Field2DCommandQueue));
    GF_ASSERT(temp_r0);
    s32 temp_r1 = arg0 + 1;
    temp_r0->capacity = temp_r1;
    void *temp_r0_2 = Heap_Alloc(heapID, temp_r1 * 8);
    temp_r0->entries = temp_r0_2;
    memset(temp_r0_2, 0, temp_r0->capacity * 8);
    temp_r0->head = 0;
    temp_r0->tail = 0;
    return temp_r0;
}

void Field2DMoveQueue_Free(u32 *arg0) {
    Heap_Free((void *)*arg0);
    Heap_Free(arg0);
}

void Field2DMoveQueue_Push(Field2DMoveQueue *arg0, Field2DMoveQueueEntry *arg1) {
    Field2DMoveQueueEntry sp0;
    if (arg0->head == ((arg0->tail + 1) % arg0->capacity)) {
        Field2DMoveQueue_Pop(arg0, &sp0);
    }
    arg0->entries[arg0->tail] = *arg1;
    arg0->tail = (arg0->tail + 1) % arg0->capacity;
}

s32 Field2DMoveQueue_Pop(Field2DMoveQueue *arg0, Field2DMoveQueueEntry *arg1) {
    if (arg0->tail == arg0->head) {
        return 0;
    }
    *arg1 = arg0->entries[arg0->head];
    arg0->head = (arg0->head + 1) % arg0->capacity;
    return 1;
}

Field2DCommandQueue *Field2DCommandQueue_New(s32 arg0, enum HeapID heapID) {
    Field2DCommandQueue *temp_r0 = Heap_Alloc(heapID, sizeof(Field2DCommandQueue));
    GF_ASSERT(temp_r0);
    temp_r0->capacity = arg0 + 1;
    temp_r0->entries = Heap_Alloc(heapID, (arg0 + 1) * 4);
    memset(temp_r0->entries, 0, temp_r0->capacity * 4);
    temp_r0->head = 0;
    temp_r0->tail = 0;
    return temp_r0;
}

void Field2DCommandQueue_Free(Field2DCommandQueue *arg0) {
    Heap_Free(arg0->entries);
    Heap_Free(arg0);
}

void Field2DCommandQueue_Push(Field2DCommandQueue *arg0, Field2DCommand *arg1) {
    Field2DCommand sp0;
    if (arg0->head == ((arg0->tail + 1) % arg0->capacity)) {
        Field2DCommandQueue_Pop(arg0, &sp0);
    }
    arg0->entries[arg0->tail] = arg1[0];
    arg0->tail = (arg0->tail + 1) % arg0->capacity;
}

u32 Field2DCommandQueue_Pop(Field2DCommandQueue *arg0, Field2DCommand *arg1) {
    if (arg0->tail == arg0->head) {
        return 0;
    }
    *arg1 = arg0->entries[arg0->head];
    arg0->head = (arg0->head + 1) % arg0->capacity;
    return 1;
}
