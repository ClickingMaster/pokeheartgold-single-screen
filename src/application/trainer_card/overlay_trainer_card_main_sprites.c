#include "global.h"

#include "application/trainer_card/overlay_trainer_card_main_internal.h"

#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite_transfer.h"

typedef struct TrainerCardSpritePosition {
    s32 x;
    s32 y;
} TrainerCardSpritePosition;

typedef struct TrainerCardButtonPosition {
    u8 x;
    u8 y;
} TrainerCardButtonPosition;

typedef struct TrainerCardObjVramTypes {
    u32 types[2];
} TrainerCardObjVramTypes;

typedef struct TrainerCardObjFiles {
    u8 fileIds[2][4];
} TrainerCardObjFiles;

typedef struct TrainerCardBadgePalettes {
    u32 fileIds[16];
} TrainerCardBadgePalettes;

static void TrainerCardMain_InitObjTransfer(void);

static const u8 sTrainerCardResourceCounts[4] = { 1, 2, 1, 1 };

static const TrainerCardObjVramTypes sTrainerCardObjVramTypes = {
    .types = { NNS_G2D_VRAM_TYPE_2DMAIN, NNS_G2D_VRAM_TYPE_2DSUB },
};

static const TrainerCardObjFiles sTrainerCardObjFileIds = {
    .fileIds = { { 46, 30, 58, 59 }, { 45, 29, 56, 57 } },
};

static const ObjCharTransferTemplate sTrainerCardObjCharTransferTemplate = {
    .maxTasks = 2,
    .sizeMain = 0,
    .sizeSub = 0,
    .heapID = HEAP_ID_25,
};

static const TrainerCardBadgePalettes sTrainerCardBadgePaletteFileIds = {
    .fileIds = { 32, 33, 34, 35, 36, 37, 38, 39, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static const TrainerCardSpritePosition sTrainerCardBadgeSpritePositions[16] = {
    { 96,  48  },
    { 144, 48  },
    { 192, 48  },
    { 240, 48  },
    { 96,  88  },
    { 144, 88  },
    { 192, 88  },
    { 240, 88  },
    { 96,  136 },
    { 144, 136 },
    { 192, 136 },
    { 240, 136 },
    { 96,  176 },
    { 144, 176 },
    { 192, 176 },
    { 240, 176 },
};

static u8 sTrainerCardButtonSpriteAnims[4] = { 0, 4, 0, 0 };

static TrainerCardButtonPosition sTrainerCardButtonSpritePositions[14] = {
    { 192, 168 },
    { 0,   168 },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
    { 0,   0   },
};

void TrainerCardMain_InitSprites(TrainerCardMainSprites *sprites) {
    TrainerCardBadgePalettes paletteFiles;
    TrainerCardObjVramTypes vramTypes;
    TrainerCardObjFiles files;
    NARC *narc;
    int i;
    u8 j;

    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    TrainerCardMain_InitObjTransfer();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_25);
    sprites->spriteList = G2dRenderer_Init(33, &sprites->renderer, HEAP_ID_25);
    G2dRenderer_SetSubSurfaceCoords(&sprites->renderer, 0, FX32_CONST(224));
    for (i = 0; i < 4; i++) {
        sprites->resourceManagers[0][i] = Create2DGfxResObjMan(sTrainerCardResourceCounts[i], (GfGfxResType)i, HEAP_ID_25);
        sprites->resourceManagers[1][i] = Create2DGfxResObjMan(sTrainerCardResourceCounts[i], (GfGfxResType)i, HEAP_ID_25);
    }
    i = 0;
    vramTypes = sTrainerCardObjVramTypes;
    files = sTrainerCardObjFileIds;
    for (; i < 2; i++) {
        sprites->resourceObjects[i][0] = AddCharResObjFromNarc(sprites->resourceManagers[i][0], NARC_a_0_4_9, files.fileIds[i][0], FALSE, i, vramTypes.types[i], HEAP_ID_25);
        sprites->resourceObjects[i][1] = AddPlttResObjFromNarc(sprites->resourceManagers[i][1], NARC_a_0_4_9, files.fileIds[i][1], FALSE, i, vramTypes.types[i], 16, HEAP_ID_25);
        sprites->resourceObjects[i][2] = AddCellOrAnimResObjFromNarc(sprites->resourceManagers[i][2], NARC_a_0_4_9, files.fileIds[i][2], FALSE, i, GF_GFX_RES_TYPE_CELL, HEAP_ID_25);
        sprites->resourceObjects[i][3] = AddCellOrAnimResObjFromNarc(sprites->resourceManagers[i][3], NARC_a_0_4_9, files.fileIds[i][3], FALSE, i, GF_GFX_RES_TYPE_ANIM, HEAP_ID_25);
        SpriteTransfer_CreateCharTransferTask(sprites->resourceObjects[i][0]);
        SpriteTransfer_CreateExtPlttTransferTask(sprites->resourceObjects[i][1]);
    }
    paletteFiles = sTrainerCardBadgePaletteFileIds;
    narc = NARC_New(NARC_a_0_4_9, HEAP_ID_25);
    for (j = 0; j < 16; j++) {
        sprites->paletteFiles[j] = NARC_AllocAndReadWholeMember(narc, paletteFiles.fileIds[j], HEAP_ID_25);
        if (sprites->paletteFiles[j] != NULL) {
            if (!NNS_G2dGetUnpackedPaletteData(sprites->paletteFiles[j], &sprites->palettes[j])) {
                Heap_Free(sprites->palettes[j]);
                GF_AssertFail();
            }
        } else {
            GF_AssertFail();
        }
    }
    NARC_Delete(narc);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void TrainerCardMain_CreateBadgeSprites(TrainerCardMainSprites *sprites, const u8 *badgeFlags, int showKantoBadges) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    int yOffset;
    int i;
    TrainerCardMainSprites *cursor; // advanced by one Sprite pointer per badge
    const TrainerCardSpritePosition *position;

    CreateSpriteResourcesHeader(&header, 0, 0, 0, 0, -1, -1, 0, 1, sprites->resourceManagers[0][0], sprites->resourceManagers[0][1], sprites->resourceManagers[0][2], sprites->resourceManagers[0][3], NULL, NULL);
    template.spriteList = sprites->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 2;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_25;
    if (showKantoBadges == 0) {
        yOffset = 40;
    } else {
        yOffset = 0;
    }
    cursor = sprites;
    position = sTrainerCardBadgeSpritePositions;
    for (i = 0; i < 16; i++, position++, cursor = (TrainerCardMainSprites *)((u8 *)cursor + sizeof(Sprite *))) {
        template.position.x = position->x << FX32_SHIFT;
        template.position.y = (yOffset + position->y) << FX32_SHIFT;
        cursor->badgeSprites[0] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(cursor->badgeSprites[0], TRUE);
        Sprite_SetAnimCtrlSeq(cursor->badgeSprites[0], i);
        if (badgeFlags[i] == 0 || (showKantoBadges == 0 && i > 7)) {
            Sprite_SetDrawFlag(cursor->badgeSprites[0], FALSE);
        }
    }
}

void TrainerCardMain_CreateButtonSprites(TrainerCardMainSprites *sprites) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    int i;

    CreateSpriteResourcesHeader(&header, 1, 1, 1, 1, -1, -1, 0, 1, sprites->resourceManagers[1][0], sprites->resourceManagers[1][1], sprites->resourceManagers[1][2], sprites->resourceManagers[1][3], NULL, NULL);
    template.spriteList = sprites->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_25;
    for (i = 0; i < 2; i++) {
        template.position.x = sTrainerCardButtonSpritePositions[i].x *FX32_ONE;
        template.position.y = sTrainerCardButtonSpritePositions[i].y *FX32_ONE + FX32_CONST(224);
        sprites->buttonSprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(sprites->buttonSprites[i], TRUE);
        Sprite_SetAnimCtrlSeq(sprites->buttonSprites[i], sTrainerCardButtonSpriteAnims[i]);
        Sprite_SetDrawPriority(sprites->buttonSprites[i], 2 - i);
        Sprite_SetDrawFlag(sprites->buttonSprites[i], FALSE);
    }
}

void TrainerCardMain_FreeSprites(TrainerCardMainSprites *sprites) {
    u8 i;
    for (i = 0; i < 16; i++) {
        Heap_Free(sprites->paletteFiles[i]);
    }
    SpriteTransfer_DeleteCharTransferTask(sprites->resourceObjects[0][0]);
    SpriteTransfer_DeleteCharTransferTask(sprites->resourceObjects[1][0]);
    SpriteTransfer_DeletePlttTransferTask(sprites->resourceObjects[0][1]);
    SpriteTransfer_DeletePlttTransferTask(sprites->resourceObjects[1][1]);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(sprites->resourceManagers[0][i]);
        Destroy2DGfxResObjMan(sprites->resourceManagers[1][i]);
    }
    G2dRenderer_SetSubSurfaceCoords(&sprites->renderer, 0, FX32_CONST(192));
    SpriteList_Delete(sprites->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
}

void TrainerCardMain_SetButtonSpriteAnim(TrainerCardMainSprites *sprites, int button, int sequence, int draw) {
    Sprite_SetAnimCtrlSeq(sprites->buttonSprites[button], sequence);
    Sprite_ResetAnimCtrlState(sprites->buttonSprites[button]);
    Sprite_SetDrawFlag(sprites->buttonSprites[button], draw);
}

static void TrainerCardMain_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sTrainerCardObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(2, HEAP_ID_25);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    thunk_ClearMainOAM(HEAP_ID_25);
    thunk_ClearSubOAM(HEAP_ID_25);
}

void TrainerCardMain_InitUnk33A0Pairs(u8 *pairs) {
    int i;
    for (i = 0; i < 4; i++) {
        pairs[0] = i;
        pairs[1] = 0xff;
        pairs += 2;
    }
}
