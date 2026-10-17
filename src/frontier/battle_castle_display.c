#include "global.h"

#include "constants/sndseq.h"

#include "frontier/battle_castle.h"

#include "bg_window.h"
#include "char_tiles.h"
#include "communication_core.h"
#include "communication_player_state.h"
#include "font.h"
#include "frontier_save_access.h"
#include "frontier_stat_ids_legacy.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "party.h"
#include "party_menu.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "save.h"
#include "save_frontier.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "vram_transfer_manager.h"

#define TEXT_SPEED_NOTRANSFER 0xFF

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

// Frontier helpers
void Frontier_SetSpriteAnim(Sprite *sprite, int value);
void Frontier_SetSpritePosition(Sprite *sprite, s16 x, s16 y, int value);

static Sprite *BattleCastle_CreateSprite(BattleCastleSpriteGfx *gfx, int charId, int plttId, int cellId, int animSeq, int drawPriority, int priority, u8 screen);
static void BattleCastle_InitSpriteTransfer(void);
static void BattleCastle_LoadPartyListGfx(BattleCastleSpriteGfx *gfx);
static void BattleCastle_LoadMonPicGfx(BattleCastleSpriteGfx *gfx);

typedef struct BattleCastleWindowSet {
    const WindowTemplate *templates;
    u32 count;
} BattleCastleWindowSet;

static const u8 sBattleCastleSpriteResManSizes[] = { 14, 14, 14, 14 };

static const WindowTemplate sBattleCastlePartyAppWindowTemplates[70];
static const WindowTemplate sBattleCastleOpponentAppWindowTemplates[35];

// Window sets of BattleCastle_AddWindows: the party app's and the opponent app's windows
static const BattleCastleWindowSet sBattleCastleWindowSets[] = {
    { sBattleCastlePartyAppWindowTemplates,    NELEMS(sBattleCastlePartyAppWindowTemplates)    },
    { sBattleCastleOpponentAppWindowTemplates, NELEMS(sBattleCastleOpponentAppWindowTemplates) },
};

static const TileRect sBattleCastleMonPicRect = { 0, 0, 10, 10 };

static const ObjCharTransferTemplate sBattleCastleObjCharTransferTemplate = {
    .maxTasks = 32,
    .sizeMain = 0x400,
    .sizeSub = 0x400,
    .heapID = HEAP_ID_107,
};

static const WindowTemplate sBattleCastleOpponentAppWindowTemplates[] = {
    { GF_BG_LYR_MAIN_1, 1,  1,  30, 2,  14, 1     },
    { GF_BG_LYR_MAIN_1, 26, 19, 4,  3,  14, 0x3D  },
    { GF_BG_LYR_MAIN_1, 0,  4,  32, 2,  14, 0x49  },
    { GF_BG_LYR_MAIN_1, 0,  9,  32, 2,  14, 0x89  },
    { GF_BG_LYR_MAIN_0, 4,  10, 26, 14, 14, 1     },
    { GF_BG_LYR_MAIN_0, 23, 15, 8,  8,  14, 1     },
    { GF_BG_LYR_MAIN_0, 22, 9,  9,  8,  14, 0x16D },
    { GF_BG_LYR_MAIN_0, 2,  19, 27, 4,  13, 0x1B5 },
    { GF_BG_LYR_MAIN_0, 2,  19, 20, 4,  13, 0x221 },
    { GF_BG_LYR_MAIN_0, 2,  19, 17, 4,  13, 0x271 },
    { GF_BG_LYR_MAIN_0, 24, 13, 7,  4,  14, 0x2B5 },
    { GF_BG_LYR_MAIN_0, 24, 11, 7,  6,  14, 0x2D1 },
    { GF_BG_LYR_SUB_1,  13, 1,  8,  2,  15, 0x3F0 },
    { GF_BG_LYR_SUB_1,  21, 1,  1,  2,  15, 0x3EE },
    { GF_BG_LYR_SUB_1,  23, 1,  3,  2,  15, 0x3E8 },
    { GF_BG_LYR_SUB_1,  26, 1,  3,  2,  15, 0x3E2 },
    { GF_BG_LYR_SUB_1,  13, 4,  7,  2,  15, 0x3D4 },
    { GF_BG_LYR_SUB_1,  20, 4,  11, 2,  15, 0x3BE },
    { GF_BG_LYR_SUB_1,  13, 7,  6,  2,  15, 0x3B2 },
    { GF_BG_LYR_SUB_1,  20, 7,  8,  2,  15, 0x3A2 },
    { GF_BG_LYR_SUB_1,  13, 10, 6,  2,  15, 0x396 },
    { GF_BG_LYR_SUB_1,  19, 10, 12, 2,  15, 0x37E },
    { GF_BG_LYR_SUB_1,  1,  11, 2,  2,  15, 0x37A },
    { GF_BG_LYR_SUB_1,  4,  11, 7,  2,  15, 0x36C },
    { GF_BG_LYR_SUB_1,  1,  13, 6,  2,  15, 0x358 },
    { GF_BG_LYR_SUB_1,  8,  13, 3,  2,  15, 0x352 },
    { GF_BG_LYR_SUB_1,  1,  17, 7,  2,  15, 0x344 },
    { GF_BG_LYR_SUB_1,  8,  17, 3,  2,  15, 0x33E },
    { GF_BG_LYR_SUB_1,  1,  15, 6,  2,  15, 0x332 },
    { GF_BG_LYR_SUB_1,  8,  15, 3,  2,  15, 0x32C },
    { GF_BG_LYR_SUB_1,  1,  19, 7,  2,  15, 0x31E },
    { GF_BG_LYR_SUB_1,  8,  19, 3,  2,  15, 0x318 },
    { GF_BG_LYR_SUB_1,  1,  21, 6,  2,  15, 0x30C },
    { GF_BG_LYR_SUB_1,  8,  21, 3,  2,  15, 0x306 },
    { GF_BG_LYR_SUB_1,  13, 14, 18, 8,  15, 0x276 },
};

static const WindowTemplate sBattleCastlePartyAppWindowTemplates[] = {
    { GF_BG_LYR_MAIN_1, 1,  1,  30, 2,  14, 1     },
    { GF_BG_LYR_MAIN_1, 26, 19, 4,  3,  14, 0x3D  },
    { GF_BG_LYR_MAIN_1, 0,  4,  32, 2,  14, 0x49  },
    { GF_BG_LYR_MAIN_1, 0,  9,  32, 2,  14, 0x89  },
    { GF_BG_LYR_MAIN_0, 5,  10, 24, 14, 14, 1     },
    { GF_BG_LYR_MAIN_0, 12, 2,  19, 12, 14, 1     },
    { GF_BG_LYR_MAIN_0, 2,  19, 27, 4,  13, 0x1EF },
    { GF_BG_LYR_MAIN_0, 2,  19, 20, 4,  13, 0x25B },
    { GF_BG_LYR_MAIN_0, 2,  19, 17, 4,  13, 0x2AB },
    { GF_BG_LYR_MAIN_0, 23, 17, 8,  6,  14, 0x2EF },
    { GF_BG_LYR_MAIN_0, 20, 7,  11, 10, 14, 0x31F },
    { GF_BG_LYR_MAIN_0, 22, 9,  9,  8,  14, 0x31F },
    { GF_BG_LYR_MAIN_0, 7,  17, 23, 6,  13, 0x25B },
    { GF_BG_LYR_MAIN_0, 24, 13, 7,  4,  14, 0x2E5 },
    { GF_BG_LYR_MAIN_0, 24, 11, 7,  6,  14, 0x301 },
    { GF_BG_LYR_MAIN_0, 1,  1,  8,  4,  14, 0x32B },
    { GF_BG_LYR_MAIN_0, 1,  7,  10, 2,  14, 0x34B },
    { GF_BG_LYR_MAIN_0, 1,  13, 11, 2,  14, 0x35F },
    { GF_BG_LYR_SUB_1,  13, 1,  8,  2,  15, 0x3F0 },
    { GF_BG_LYR_SUB_1,  21, 1,  1,  2,  15, 0x3EE },
    { GF_BG_LYR_SUB_1,  23, 1,  3,  2,  15, 0x3E8 },
    { GF_BG_LYR_SUB_1,  26, 1,  3,  2,  15, 0x3E2 },
    { GF_BG_LYR_SUB_1,  13, 4,  7,  2,  15, 0x3D4 },
    { GF_BG_LYR_SUB_1,  20, 4,  11, 2,  15, 0x3BE },
    { GF_BG_LYR_SUB_1,  13, 7,  6,  2,  15, 0x3B2 },
    { GF_BG_LYR_SUB_1,  20, 7,  8,  2,  15, 0x3A2 },
    { GF_BG_LYR_SUB_1,  13, 10, 6,  2,  15, 0x396 },
    { GF_BG_LYR_SUB_1,  19, 10, 12, 2,  15, 0x37E },
    { GF_BG_LYR_SUB_1,  1,  11, 2,  2,  15, 0x37A },
    { GF_BG_LYR_SUB_1,  4,  11, 7,  2,  15, 0x36C },
    { GF_BG_LYR_SUB_1,  1,  13, 6,  2,  15, 0x358 },
    { GF_BG_LYR_SUB_1,  8,  13, 3,  2,  15, 0x352 },
    { GF_BG_LYR_SUB_1,  1,  17, 7,  2,  15, 0x344 },
    { GF_BG_LYR_SUB_1,  8,  17, 3,  2,  15, 0x33E },
    { GF_BG_LYR_SUB_1,  1,  15, 6,  2,  15, 0x332 },
    { GF_BG_LYR_SUB_1,  8,  15, 3,  2,  15, 0x32C },
    { GF_BG_LYR_SUB_1,  1,  19, 7,  2,  15, 0x31E },
    { GF_BG_LYR_SUB_1,  8,  19, 3,  2,  15, 0x318 },
    { GF_BG_LYR_SUB_1,  1,  21, 7,  2,  15, 0x30A },
    { GF_BG_LYR_SUB_1,  8,  21, 3,  2,  15, 0x304 },
    { GF_BG_LYR_SUB_1,  13, 14, 11, 2,  15, 0x2EE },
    { GF_BG_LYR_SUB_1,  13, 16, 11, 2,  15, 0x2D8 },
    { GF_BG_LYR_SUB_1,  13, 18, 11, 2,  15, 0x2C2 },
    { GF_BG_LYR_SUB_1,  13, 20, 11, 2,  15, 0x2AC },
    { GF_BG_LYR_SUB_1,  26, 14, 5,  2,  15, 0x2A2 },
    { GF_BG_LYR_SUB_1,  26, 16, 5,  2,  15, 0x298 },
    { GF_BG_LYR_SUB_1,  26, 18, 5,  2,  15, 0x28E },
    { GF_BG_LYR_SUB_1,  26, 20, 5,  2,  15, 0x284 },
    { GF_BG_LYR_MAIN_0, 3,  4,  13, 5,  14, 1     },
    { GF_BG_LYR_MAIN_0, 19, 4,  13, 5,  14, 0x42  },
    { GF_BG_LYR_MAIN_0, 3,  9,  13, 5,  14, 0x83  },
    { GF_BG_LYR_MAIN_0, 19, 9,  13, 5,  14, 0xC4  },
    { GF_BG_LYR_MAIN_0, 3,  14, 13, 5,  14, 0x105 },
    { GF_BG_LYR_MAIN_0, 19, 14, 13, 5,  14, 0x146 },
    { GF_BG_LYR_MAIN_0, 10, 8,  12, 2,  14, 0x187 },
    { GF_BG_LYR_MAIN_0, 22, 8,  5,  2,  14, 0x19F },
    { GF_BG_LYR_MAIN_0, 14, 21, 4,  2,  14, 0x1A9 },
    { GF_BG_LYR_MAIN_0, 26, 21, 5,  2,  14, 0x1B1 },
    { GF_BG_LYR_MAIN_0, 2,  1,  12, 2,  14, 0x1BB },
    { GF_BG_LYR_MAIN_0, 16, 1,  8,  2,  14, 0x1D3 },
    { GF_BG_LYR_MAIN_0, 24, 1,  6,  2,  14, 0x1E3 },
    { GF_BG_LYR_SUB_3,  4,  17, 27, 6,  15, 0x35E },
    { GF_BG_LYR_SUB_3,  13, 5,  8,  2,  15, 0x34E },
    { GF_BG_LYR_SUB_3,  21, 5,  1,  2,  15, 0x34C },
    { GF_BG_LYR_SUB_3,  23, 5,  3,  2,  15, 0x346 },
    { GF_BG_LYR_SUB_3,  26, 5,  3,  2,  15, 0x340 },
    { GF_BG_LYR_SUB_3,  13, 8,  2,  2,  15, 0x33C },
    { GF_BG_LYR_SUB_3,  16, 8,  8,  2,  15, 0x32C },
    { GF_BG_LYR_SUB_3,  13, 11, 6,  2,  15, 0x320 },
    { GF_BG_LYR_SUB_3,  19, 11, 12, 2,  15, 0x308 },
};

// Sprite renderer and resources: the castle sprites (id 0), the type icons (id 2, NARC a/0/0/8),
// the party list item marks (id 1), the selected item icon (id 3), the six item icons (ids 4-9)
// and the four party mon icons (ids 10-13)
void BattleCastle_InitSpriteGfx(BattleCastleSpriteGfx *gfx, Party *party, u8 isLink) {
    int i;
    NARC *narc;

    GF_CreateVramTransferManager(32, HEAP_ID_107);
    BattleCastle_InitSpriteTransfer();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_107);
    gfx->spriteList = G2dRenderer_Init(40, &gfx->renderer, HEAP_ID_107);
    for (i = 0; i < 4; i++) {
        gfx->resMan[i] = Create2DGfxResObjMan(sBattleCastleSpriteResManSizes[i], (GfGfxResType)i, HEAP_ID_107);
    }
    BattleCastle_LoadMonPicGfx(gfx);
    gfx->resObj[0][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_8_4, 15, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_107);
    gfx->resObj[0][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_1_8_4, 55, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 4, HEAP_ID_107);
    gfx->resObj[0][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_1_8_4, 17, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    gfx->resObj[0][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_1_8_4, 16, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);

    narc = NARC_New(NARC_itemtool_itemdata_item_icon, HEAP_ID_107);
    for (i = 4; i <= 9; i++) {
        gfx->resObj[i][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, GetItemIndexMapping(ITEM_NONE, ITEMNARC_NCGR), FALSE, i, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_107);
        gfx->resObj[i][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(ITEM_NONE, ITEMNARC_NCLR), FALSE, i, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_107);
    }
    gfx->resObj[4][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, GetItemIconCell(), FALSE, 4, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    gfx->resObj[4][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, GetItemIconAnim(), FALSE, 4, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    gfx->resObj[3][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(ITEM_NONE, ITEMNARC_NCLR), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_107);
    NARC_Delete(narc);

    gfx->resObj[3][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_8_4, 36, TRUE, 3, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_107);
    gfx->resObj[3][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_1_8_4, 38, TRUE, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    gfx->resObj[3][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_1_8_4, 37, TRUE, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    BattleCastle_LoadPartyListGfx(gfx);

    narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_107);
    gfx->resObj[10][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_107);
    gfx->resObj[10][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, GetMonIconCell32KNaix(), FALSE, 5, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    gfx->resObj[10][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, GetMonIconAnim32KNaix(), FALSE, 5, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    for (i = 0; i < 4; i++) {
        Pokemon *mon;

        if (i == 3) {
            // without the link partner, the fourth icon repeats the first mon
            if (isLink == FALSE) {
                mon = Party_GetMonByIndex(party, 0);
            } else {
                mon = Party_GetMonByIndex(party, i);
            }
        } else {
            mon = Party_GetMonByIndex(party, i);
        }
        gfx->resObj[10 + i][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, Pokemon_GetIconNaix(mon), FALSE, i + 10, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_107);
    }
    NARC_Delete(narc);

    for (i = 0; i < 14; i++) {
        SpriteTransfer_CreateCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
    }
    for (i = 0; i < 11; i++) {
        SpriteTransfer_CreateExtPlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static Sprite *BattleCastle_CreateSprite(BattleCastleSpriteGfx *gfx, int charId, int plttId, int cellId, int animSeq, int drawPriority, int priority, u8 screen) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, charId, plttId, cellId, cellId, -1, -1, FALSE, priority, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = gfx->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = drawPriority;
    if (screen == 0) {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }
    template.heapID = HEAP_ID_107;
    if (screen == 1) {
        template.position.y += 192 * FX32_ONE;
    }
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    return sprite;
}

void BattleCastle_FreeSpriteGfx(BattleCastleSpriteGfx *gfx) {
    u8 i;

    for (i = 0; i < 14; i++) {
        SpriteTransfer_DeleteCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
    }
    for (i = 0; i < 11; i++) {
        SpriteTransfer_DeletePlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resMan[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

// Item icon graphics (resource id resId) of the given item
void BattleCastle_LoadItemIconChar(BattleCastleSpriteGfx *gfx, int resId, u16 item) {
    SpriteResource *res = SpriteResourceCollection_Find(gfx->resMan[GF_GFX_RES_TYPE_CHAR], resId);
    int fileId = GetItemIndexMapping(item, ITEMNARC_NCGR);

    ReplaceCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], res, NARC_itemtool_itemdata_item_icon, fileId, FALSE, HEAP_ID_107);
    SpriteTransfer_ReplaceCharData(res);
}

void BattleCastle_LoadItemIconPalette(BattleCastleSpriteGfx *gfx, int resId, u16 item) {
    SpriteResource *res = SpriteResourceCollection_Find(gfx->resMan[GF_GFX_RES_TYPE_PLTT], resId);
    int fileId = GetItemIndexMapping(item, ITEMNARC_NCLR);

    ReplacePlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], res, NARC_itemtool_itemdata_item_icon, fileId, FALSE, HEAP_ID_107);
    SpriteTransfer_ReplacePlttData(res);
}

static void BattleCastle_InitSpriteTransfer(void) {
    ObjCharTransferTemplate template = sBattleCastleObjCharTransferTemplate;

    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_64K);
    ObjPlttTransfer_Init(32, HEAP_ID_107);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

static void BattleCastle_LoadPartyListGfx(BattleCastleSpriteGfx *gfx) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, HEAP_ID_107);

    gfx->resObj[1][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, PartyMenu_GetHeldItemIconNCGR(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_107);
    gfx->resObj[1][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_graphic_plist_gra, PartyMenu_GetHeldItemIconNCLR(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_107);
    gfx->resObj[1][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, PartyMenu_GetHeldItemIconNCER(), FALSE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    gfx->resObj[1][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, PartyMenu_GetHeldItemIconNANR(), FALSE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    NARC_Delete(narc);
}

static void BattleCastle_LoadMonPicGfx(BattleCastleSpriteGfx *gfx) {
    NARC *narc = NARC_New(NARC_a_0_0_8, HEAP_ID_107);

    gfx->resObj[2][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, 0x4C, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_107);
    gfx->resObj[2][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], narc, 0x4B, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_107);
    gfx->resObj[2][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, 0x4D, FALSE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_107);
    gfx->resObj[2][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, 0x4E, FALSE, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_107);
    NARC_Delete(narc);
}

BattleCastleSprite *BattleCastleSprite_New(BattleCastleSpriteGfx *gfx, int charId, int plttId, int cellId, int animSeq, s16 x, s16 y, int priority, int unused) {
    BattleCastleSprite *sprite = Heap_Alloc(HEAP_ID_107, sizeof(BattleCastleSprite));
    VecFx32 pos;

    memset(sprite, 0, sizeof(BattleCastleSprite));
    sprite->x = x;
    sprite->y = y;
    sprite->sprite = BattleCastle_CreateSprite(gfx, charId, plttId, cellId, animSeq, 0, priority, 0);
    pos.x = x << FX32_SHIFT;
    pos.y = y << FX32_SHIFT;
    Sprite_SetMatrix(sprite->sprite, &pos);
    return sprite;
}

BattleCastleSprite *BattleCastleSprite_NewSubScreen(BattleCastleSpriteGfx *gfx, int charId, int plttId, int cellId, int animSeq, s16 x, s16 y, int priority, int unused) {
    BattleCastleSprite *sprite = Heap_Alloc(HEAP_ID_107, sizeof(BattleCastleSprite));
    VecFx32 pos;

    memset(sprite, 0, sizeof(BattleCastleSprite));
    sprite->x = x;
    sprite->y = y;
    sprite->sprite = BattleCastle_CreateSprite(gfx, charId, plttId, cellId, animSeq, 0, priority, 1);
    pos.x = x << FX32_SHIFT;
    pos.y = (y << FX32_SHIFT) + 192 * FX32_ONE;
    Sprite_SetMatrix(sprite->sprite, &pos);
    return sprite;
}

int BattleCastleSprite_Delete(BattleCastleSprite *sprite) {
    if (sprite == NULL) {
        GF_AssertFail();
        return 0;
    }
    Sprite_Delete(sprite->sprite);
    Heap_Free(sprite);
    return 0;
}

void BattleCastleSprite_SetDrawFlag(BattleCastleSprite *sprite, int draw) {
    Sprite_SetDrawFlag(sprite->sprite, draw);
}

void BattleCastleSprite_SetPosition(BattleCastleSprite *sprite, int x, int y) {
    VecFx32 pos = *Sprite_GetMatrixPtr(sprite->sprite);

    pos.x = x << FX32_SHIFT;
    pos.y = y << FX32_SHIFT;
    Sprite_SetMatrix(sprite->sprite, &pos);
}

void BattleCastleSprite_SetPositionSubScreen(BattleCastleSprite *sprite, int x, int y) {
    VecFx32 pos = *Sprite_GetMatrixPtr(sprite->sprite);

    pos.x = x << FX32_SHIFT;
    pos.y = (y << FX32_SHIFT) + 192 * FX32_ONE;
    Sprite_SetMatrix(sprite->sprite, &pos);
}

void BattleCastleSprite_SetAnim(BattleCastleSprite *sprite, int seq) {
    Sprite_SetAnimationFrame(sprite->sprite, 0);
    Sprite_SetAnimCtrlSeq(sprite->sprite, seq);
}

void BattleCastleSprite_SetMonIconPalette(BattleCastleSprite *sprite, Pokemon *mon) {
    int palette = Pokemon_GetIconPalette(mon);
    Sprite_SetPalOffsetRespectVramOffset(sprite->sprite, palette);
}

void BattleCastleSprite_ChangeAnim(BattleCastleSprite *sprite, int value) {
    Frontier_SetSpriteAnim(sprite->sprite, value);
}

void BattleCastleSprite_SetBounce(BattleCastleSprite *sprite, int value) {
    Frontier_SetSpritePosition(sprite->sprite, sprite->x, sprite->y, value);
}

int BattleCastleSprite_IsAnimated(BattleCastleSprite *sprite) {
    return Sprite_IsAnimated(sprite->sprite);
}

void BattleCastleSprite_ShowAnim11WithSE(BattleCastleSprite *sprite, int x, u32 y) {
    BattleCastleSprite_SetAnim(sprite, 11);
    BattleCastleSprite_SetPosition(sprite, x, y);
    BattleCastleSprite_SetDrawFlag(sprite, TRUE);
    PlaySE(SEQ_SE_DP_PIRORIRO2);
    PlaySE(SEQ_SE_DP_DANSA4);
}

// Front picture of a mon into the sub screen sprite; species SPECIES_NONE shows a placeholder
void BattleCastleSprite_LoadMonPic(BattleCastleSprite *sprite, BoxPokemon *boxMon, u16 species, u32 personality) {
    PokepicTemplate pokepic;
    TileRect rect = sBattleCastleMonPicRect;
    NNSG2dCharacterData *charData;
    void *file;
    void *buffer;
    u32 imageLocation;
    u32 paletteLocation;
    int narcId;
    int plttId;

    buffer = Heap_AllocAtEnd(HEAP_ID_107, 0xC80);
    if (species != SPECIES_NONE) {
        GetBoxmonSpriteCharAndPlttNarcIds(&pokepic, boxMon, 2, FALSE);
        CharTiles_LoadMonRectAsObjFromNarc((NarcId)pokepic.narcID, pokepic.charDataID, HEAP_ID_107, &rect, buffer, personality, FALSE, 2, species);
        narcId = pokepic.narcID;
        plttId = pokepic.palDataID;
    } else {
        file = GfGfxLoader_GetCharData(NARC_a_1_8_4, 0x27, TRUE, &charData, HEAP_ID_107);
        MI_CpuCopy32(charData->pRawData, buffer, 0xC80);
        Heap_Free(file);
        narcId = NARC_a_1_8_4;
        plttId = 0x3D;
    }
    imageLocation = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    DC_FlushRange(buffer, 0xC80);
    GXS_LoadOBJ(buffer, imageLocation, 0xC80);
    paletteLocation = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    GfGfxLoader_GXLoadPal((NarcId)narcId, plttId, GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)paletteLocation, 0x20, HEAP_ID_107);
    Heap_Free(buffer);
}

// Translucent (mode 1) or normal sprites, while a list menu covers them
void BattleCastle_SetSpritesTranslucent(BattleCastleSprite **sprites, u32 count, int mode) {
    u32 i;
    int oamMode = mode == 1;

    for (i = 0; i < count; i++) {
        Sprite_SetOamMode(sprites[i]->sprite, (GXOamMode)oamMode);
    }
}

// Party index of a grid slot
u32 BattleCastle_GetMonIndex(u32 numMons, u32 slot) {
    return slot;
}

// Slot within its own side (the partner's slots follow the player's)
u32 BattleCastle_GetSideSlot(u32 numOwnSlots, u32 slot) {
    if (slot >= numOwnSlots) {
        slot = (u8)(slot - numOwnSlots);
    }
    return slot;
}

u8 BattleCastle_GetRank(SaveData *saveData, u8 frontierType, u8 kind) {
    FrontierSave *frontierSave = Save_Frontier_GetStatic(saveData);
    int stat = BattleCastle_GetRankStat(frontierType, kind);

    return FrontierSave_GetStat(frontierSave, stat, FrontierStat_GetFriendIndex(BattleCastle_GetRankStat(frontierType, kind)));
}

void BattleCastle_PlaySEOnInput(int input, u16 seqNo) {
    if (input != LIST_NOTHING_CHOSEN) {
        PlaySE(seqNo);
    }
}

void BattleCastle_BufferPartnerName(MessageFormat *msgFormat, int field) {
    PlayerProfile *profile = CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1);
    BufferPlayersName(msgFormat, field, profile);
}

void BattleCastle_ClearBgToggleRequests(u32 *requests) {
    *requests = 0;
}

// Request a BG layer toggle (value = GF_PLANE_TOGGLE_*), applied in VBlank by BattleCastle_ApplyBgToggleRequests
void BattleCastle_RequestBgToggle(int layer, int value, u32 *requests) {
    int shift;
    u32 request;
    u32 mask;
    u32 bits;

    shift = layer * 4;
    request = value + 1;
    mask = 0xF;
    bits = request << shift;
    *requests = (*requests & ((mask << shift) ^ 0xFFFFFFFF)) | bits;
}

void BattleCastle_ApplyBgToggleRequests(u32 *requests) {
    u16 i;

    for (i = 0; i <= 7; i++) {
        u16 request = (*requests >> (i * 4)) & 0xF;
        if (request) {
            ToggleBgLayer((u8)i, (u8)(request - 1));
        }
    }
    BattleCastle_ClearBgToggleRequests(requests);
}

void BattleCastlePrompt_Init(BattleCastlePrompt *prompt) {
    prompt->prompt = YesNoPrompt_Create(HEAP_ID_107);
    prompt->active = FALSE;
}

void BattleCastlePrompt_Free(BattleCastlePrompt *prompt) {
    YesNoPrompt_Destroy(prompt->prompt);
}

void BattleCastlePrompt_Show(BattleCastlePrompt *prompt, BgConfig *bgConfig) {
    u16 tileStart;
    u16 tileCount;
    YesNoPromptTemplate template;

    prompt->bgConfig = bgConfig;
    BattleCastle_GetMenuTiles(&tileStart, &tileCount);
    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = tileStart;
    template.plttSlot = 11;
    template.x = 25;
    template.y = 10;
    template.ignoreTouchFlag = FALSE;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt->prompt, &template);
    prompt->active = TRUE;
}

void BattleCastlePrompt_Hide(BattleCastlePrompt *prompt) {
    if (prompt->active != FALSE) {
        YesNoPrompt_Reset(prompt->prompt);
        BgCommitTilemapBufferToVram(prompt->bgConfig, GF_BG_LYR_MAIN_0);
        prompt->active = FALSE;
    }
}

// Window set 0 (party app, 70 windows) or 1 (opponent app, 35 windows)
void BattleCastle_AddWindows(BgConfig *bgConfig, Window *windows, u32 set) {
    const WindowTemplate *templates = sBattleCastleWindowSets[set].templates;
    u8 i = 0;
    u32 count = sBattleCastleWindowSets[set].count;

    for (; i < count; i++) {
        AddWindow(bgConfig, &windows[i], &templates[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void BattleCastle_RemoveWindows(Window *windows, u32 set) {
    u16 i;
    u32 count = sBattleCastleWindowSets[set].count;

    i = 0;
    for (; i < count; i++) {
        RemoveWindow(&windows[i]);
    }
}

void BattleCastle_DrawWindowFrame(Window *window, int frame) {
    LoadUserFrameGfx2(window->bgConfig, (GFBgLayer)GetWindowBgId(window), 0x3D9, 10, (u8)frame, HEAP_ID_107);
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, TRUE, 0x3D9, 10);
    ScheduleWindowCopyToVram(window);
}

// VRAM tiles of the list menus and Yes/No prompts
void BattleCastle_GetMenuTiles(u16 *tileStart, u16 *tileCount) {
    *tileCount = 240;
    *tileStart = 0x2E9;
}

void BattleCastle_PrintString(Window *window, String *string, int x, int y, int fontId, int color, int alignment) {
    if (alignment == CASTLE_ALIGN_RIGHT) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (alignment == CASTLE_ALIGN_CENTER) {
        x -= FontID_String_GetWidth(fontId, string, 0) >> 1;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void BattleCastle_PrintMsg(Window *window, MsgData *msgData, s32 msgNo, int x, int y, int fontId, int color, int alignment) {
    String *string = NewString_ReadMsgData(msgData, msgNo);

    BattleCastle_PrintString(window, string, x, y, fontId, color, alignment);
    String_Delete(string);
}
