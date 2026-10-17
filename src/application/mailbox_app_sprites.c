#include "global.h"

#include "application/mailbox_app_internal.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail.h"
#include "pokemon_icon_idx.h"
#include "sprite_resource_header.h"
#include "sprite_system.h"

static void MailboxAppData_InitSpriteSystem(MailboxAppData *data);
static void MailboxAppData_FreeSpriteSystem(MailboxAppData *data);
static ManagedSprite *MailboxAppData_NewSprite(MailboxAppData *data, const ManagedSpriteTemplate *template);
static void MailboxAppData_DeleteSprite(MailboxAppData *data, u32 index);
static void MailboxAppData_DeleteAllSprites(MailboxAppData *data);
static void MailboxAppData_OpenMonIconNarc(MailboxAppData *data);
static void MailboxAppData_CloseMonIconNarc(MailboxAppData *data);
static void MailboxAppData_LoadMonIconResources(MailboxAppData *data);
static void MailboxAppData_LoadSpriteResources(MailboxAppData *data);

static const OamCharTransferParam sMailboxOamCharTransferParam = {
    .maxTasks = 0x400,
    .sizeMain = 0x10000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_32K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_32K,
};

static const SpriteResourceCountsListUnion sMailboxSpriteResourceCounts = { 4, 2, 2, 2, 0, 0 };

static const OamManagerParam sMailboxOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 128,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 0,
    .numOBJsub = 128,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const ManagedSpriteTemplate sMailboxSpriteTemplates[MAILBOX_NUM_SPRITES] = {
    { 24,  176, 0, 0, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, { 0x37F3, 0x37F1, 0x37F1, 0x37F1, -1, -1 }, 3, 0 },
    { 56,  176, 0, 1, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, { 0x37F3, 0x37F1, 0x37F1, 0x37F1, -1, -1 }, 3, 0 },
    { 64,  16,  0, 5, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, { 0x37F3, 0x37F1, 0x37F1, 0x37F1, -1, -1 }, 1, 0 },
    { 224, 176, 0, 2, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, { 0x37F3, 0x37F1, 0x37F1, 0x37F1, -1, -1 }, 3, 0 },
    { 128, 160, 0, 0, 0, 0, NNS_G2D_VRAM_TYPE_2DSUB,  { 0x37F0, 0x37F0, 0x37F0, 0x37F0, -1, -1 }, 1, 0 },
    { 88,  160, 0, 0, 0, 0, NNS_G2D_VRAM_TYPE_2DSUB,  { 0x37F1, 0x37F0, 0x37F0, 0x37F0, -1, -1 }, 1, 0 },
    { 48,  160, 0, 0, 0, 0, NNS_G2D_VRAM_TYPE_2DSUB,  { 0x37F2, 0x37F0, 0x37F0, 0x37F0, -1, -1 }, 1, 0 },
};

void MailboxApp_CreateSprites(MailboxApp *app) {
    u32 i;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    MailboxAppData_OpenMonIconNarc(app->data);
    MailboxAppData_InitSpriteSystem(app->data);
    MailboxAppData_LoadSpriteResources(app->data);
    MailboxAppData_LoadMonIconResources(app->data);
    for (i = 0; i < MAILBOX_NUM_SPRITES; i++) {
        app->data->sprites[i] = MailboxAppData_NewSprite(app->data, &sMailboxSpriteTemplates[i]);
    }
    if (app->data->mailCount <= MAILBOX_MAIL_PER_PAGE) {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, FALSE);
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, FALSE);
    } else if (app->page == 0) {
        MailboxAppData_SetSpriteDrawFlag(app->data, 0, FALSE);
    } else {
        MailboxAppData_SetSpriteDrawFlag(app->data, 1, FALSE);
    }
    MailboxAppData_SetSpriteDrawFlag(app->data, 4, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 5, FALSE);
    MailboxAppData_SetSpriteDrawFlag(app->data, 6, FALSE);
}

void MailboxApp_DeleteSprites(MailboxApp *app) {
    MailboxAppData_DeleteAllSprites(app->data);
    MailboxAppData_FreeSpriteSystem(app->data);
    MailboxAppData_CloseMonIconNarc(app->data);
}

void MailboxAppData_TickSprites(MailboxAppData *data) {
    u32 i;

    for (i = 0; i < MAILBOX_NUM_SPRITES; i++) {
        if (data->sprites[i] != NULL) {
            ManagedSprite_TickFrame(data->sprites[i]);
        }
    }
}

static void MailboxAppData_InitSpriteSystem(MailboxAppData *data) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamCharTransferParam;
    SpriteResourceCountsListUnion resourceCounts;

    data->spriteSystem = SpriteSystem_Alloc(HEAP_ID_157);
    data->spriteManager = SpriteManager_New(data->spriteSystem);
    oamManagerParam = sMailboxOamManagerParam;
    oamCharTransferParam = sMailboxOamCharTransferParam;
    SpriteSystem_Init(data->spriteSystem, &oamManagerParam, &oamCharTransferParam, 0x20);
    resourceCounts = sMailboxSpriteResourceCounts;
    SpriteSystem_InitSprites(data->spriteSystem, data->spriteManager, MAILBOX_NUM_SPRITES);
    SpriteSystem_InitManagerWithCapacities(data->spriteSystem, data->spriteManager, &resourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(data->spriteSystem), 0, FX32_CONST(512));
}

static void MailboxAppData_FreeSpriteSystem(MailboxAppData *data) {
    SpriteSystem_FreeResourcesAndManager(data->spriteSystem, data->spriteManager);
    SpriteSystem_Free(data->spriteSystem);
}

static ManagedSprite *MailboxAppData_NewSprite(MailboxAppData *data, const ManagedSpriteTemplate *template) {
    if (template->vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        return SpriteSystem_NewSprite(data->spriteSystem, data->spriteManager, template);
    }
    return SpriteSystem_NewSpriteWithYOffset(data->spriteSystem, data->spriteManager, template, FX32_CONST(512));
}

static void MailboxAppData_DeleteSprite(MailboxAppData *data, u32 index) {
    if (data->sprites[index] != NULL) {
        Sprite_DeleteAndFreeResources(data->sprites[index]);
        data->sprites[index] = NULL;
    }
}

static void MailboxAppData_DeleteAllSprites(MailboxAppData *data) {
    u32 i;

    for (i = 0; i < MAILBOX_NUM_SPRITES; i++) {
        MailboxAppData_DeleteSprite(data, i);
    }
}

void MailboxAppData_SetSpriteAnim(MailboxAppData *data, u32 index, u32 anim) {
    ManagedSprite_SetAnimationFrame(data->sprites[index], 0);
    ManagedSprite_SetAnim(data->sprites[index], anim);
}

void MailboxAppData_SetSpriteDrawFlag(MailboxAppData *data, u32 index, BOOL draw) {
    if (draw == TRUE) {
        ManagedSprite_SetDrawFlag(data->sprites[index], TRUE);
    } else {
        ManagedSprite_SetDrawFlag(data->sprites[index], FALSE);
    }
}

void MailboxAppData_SetSpritePosition(MailboxAppData *data, u32 index, s16 x, s16 y) {
    ManagedSprite_SetPositionXY(data->sprites[index], x, y);
}

void MailboxAppData_SetSpriteTranslucent(MailboxAppData *data, u32 index, BOOL blend) {
    if (blend == TRUE) {
        ManagedSprite_SetOamMode(data->sprites[index], GX_OAM_MODE_XLU);
    } else {
        ManagedSprite_SetOamMode(data->sprites[index], GX_OAM_MODE_NORMAL);
    }
}

static void MailboxAppData_OpenMonIconNarc(MailboxAppData *data) {
    data->monIconNarc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_157);
}

static void MailboxAppData_CloseMonIconNarc(MailboxAppData *data) {
    NARC_Delete(data->monIconNarc);
}

static void MailboxAppData_LoadMonIconResources(MailboxAppData *data) {
    u32 resId;

    for (resId = 0x37F0; resId <= 0x37F2; resId++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconNaixEx(SPECIES_NONE, FALSE, 0), FALSE, NNS_G2D_VRAM_TYPE_2DSUB, resId);
    }
    SpriteSystem_LoadPlttResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0x37F0);
    SpriteSystem_LoadCellResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconStaticCellNaix(), FALSE, 0x37F0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(data->spriteSystem, data->spriteManager, data->monIconNarc, GetMonIconCell32KNaix(), FALSE, 0x37F0);
}

// Shows the icons of the (up to three) Pokemon the mail's author attached.
void MailboxApp_ShowMailMonIcons(MailboxApp *app) {
    union {
        u16 raw;
        struct {
            u16 species : 12;
            u16 palette : 4;
        };
    } icon;
    u32 i;
    register Mail *mail = app->data->mail[app->selectedMail];
    NNSG2dCharacterData *charData;
    void *charFile;
    u32 imageLoc;
    u32 iconId;

    for (i = 0; i < 3; i++) {
        icon.raw = Mail_GetMonIconData(mail, (u8)i, 2, Mail_GetFormFlags(mail));
        if (icon.raw == 0xFFFF || (iconId = icon.species) == 7) {
            MailboxAppData_SetSpriteDrawFlag(app->data, i + 4, FALSE);
        } else {
            imageLoc = NNS_G2dGetImageLocation(Sprite_GetImageProxy(app->data->sprites[i + 4]->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
            charFile = GfGfxLoader_GetCharDataFromOpenNarc(app->data->monIconNarc, iconId, FALSE, &charData, HEAP_ID_157);
            DC_FlushRange(charData->pRawData, 0x200);
            GXS_LoadOBJ(charData->pRawData, imageLoc, 0x200);
            Heap_Free(charFile);
            ManagedSprite_SetPaletteOverride(app->data->sprites[i + 4], icon.palette);
            MailboxAppData_SetSpriteDrawFlag(app->data, i + 4, TRUE);
        }
    }
}

static void MailboxAppData_LoadSpriteResources(MailboxAppData *data) {
    NARC *narc = NARC_New(NARC_a_2_5_0, HEAP_ID_157);

    SpriteSystem_LoadCharResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 7, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0x37F3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 10, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0x37F1);
    SpriteSystem_LoadCellResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 8, TRUE, 0x37F1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(data->spriteSystem, data->spriteManager, narc, 9, TRUE, 0x37F1);
    NARC_Delete(narc);
}

void MailboxAppData_MoveCursorToCloseButton(MailboxAppData *data) {
    MailboxAppData_SetSpritePosition(data, 2, 224, 176);
    MailboxAppData_SetSpriteAnim(data, 2, 4);
}
