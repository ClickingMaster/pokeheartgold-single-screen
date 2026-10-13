#include "global.h"

#include "application/footprint_stamp.h"

#include "font.h"
#include "geometry_math.h"
#include "gf_3d_render.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "math_util.h"
#include "party.h"
#include "pokemon.h"
#include "sound_playback.h"
#include "text.h"
#include "unk_0200FA24.h"
#include "swap_3d_buffers.h"

// Declared only in other modules' private headers (src/model_animation.c, ov45, screen fade).
void ModelResource_LoadFromNarc(FootprintStampModel *model, NARC *narc, int fileId, enum HeapID heapId);
void ModelResource_Free(FootprintStampModel *model);
void ModelInstance_Init(FootprintStampModelInstance *instance, FootprintStampModel *model);
void ModelInstance_Draw(FootprintStampModelInstance *instance);
void ModelInstance_SetVisible(FootprintStampModelInstance *instance, BOOL visible);
void ModelInstance_SetPosition(FootprintStampModelInstance *instance, fx32 x, fx32 y, fx32 z);
void ModelInstance_SetScale(FootprintStampModelInstance *instance, fx32 x, fx32 y, fx32 z);
u32 WiFiPlazaLobbyClient_GetOwnId(void *unused);
int WiFiPlazaLobbyClient_IdToChannelSlot(u32 playerId);
void SetMasterBrightness(PMLCDTarget screen, int brightness);

extern const u16 sFootprintStampEmptySlotTiles[4]; // party bar tiles of an empty slot (the label has no ov89_ prefix)
extern const CameraAngle sFootprintStampCameraAngle;
extern const VecFx32 sFootprintStampCameraTarget;
extern const u8 sFootprintStampPlayerNameWindowPositions[FOOTPRINT_STAMP_PLAYERS_MAX][2];
extern const s16 sFootprintStampMyCursorPositions[FOOTPRINT_STAMP_PLAYERS_MAX][2];
extern const ManagedSpriteTemplate sFootprintStampSelectEffectTemplate;
extern const FootprintStampEffectFuncs sFootprintStampEffectFuncs[];
extern BOOL (*const sFootprintStampCameraEffectFuncs[])(FootprintStampBoard *unused, FootprintStampBoard *board, Camera *camera);

// The lobby's player id list (WiFiPlazaLobbyClient_GetChannelPlayerList).
typedef struct FootprintStampPlayerList {
    u32 unk00;
    u32 *ids; // FOOTPRINT_STAMP_PLAYERS_MAX, -1 = empty
} FootprintStampPlayerList;

void WiFiPlazaLobbyClient_GetChannelPlayerList(FootprintStampPlayerList *list);

// Player list rows on the sub screen (tiles).
typedef struct FootprintStampRow {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} FootprintStampRow;

extern const FootprintStampRow sFootprintStampPlayerRows[FOOTPRINT_STAMP_PLAYERS_MAX];

void FootprintStamp_FreePlayerSprites(FootprintStamp *work) {
    int i;
    FootprintStamp *slot = work;

    Sprite_DeleteAndFreeResources(work->myCursorSprite);
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
        Sprite_DeleteAndFreeResources(slot->playerFootprintSprites[0]);
    }
}

// Shows the party in the bar at the bottom: the footprint of each Pokemon in its ink color, empty slots hidden.
#pragma force_active on

void FootprintStamp_ShowParty(FootprintStamp *work) {
    int i;
    u16 *unfaded;
    int found = 0;
    FootprintStamp *mon;
    FootprintStamp *slot;
    int column;
    u16 *pltt;
    u16 *faded;
    int j;

    unfaded = PaletteData_GetUnfadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);
    faded = PaletteData_GetFadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);
    for (i = 0, column = 0, mon = work, slot = work, pltt = unfaded; i < FOOTPRINT_STAMP_PARTY_SLOTS; mon = (FootprintStamp *)((u8 *)mon + sizeof(FootprintStampMon)), slot = (FootprintStamp *)((u8 *)slot + 4), column += 4, pltt++, faded++, i++) {
        if (mon->party[0].species == SPECIES_NONE || mon->party[0].species > NATIONAL_DEX_COUNT) {
            ManagedSprite_SetDrawFlag(slot->inkSprites[0], FALSE);
            ManagedSprite_SetDrawFlag(slot->slotSprites[0], FALSE);
            ManagedSprite_SetDrawFlag(slot->footprintSprites[0], FALSE);
            for (j = 0; j < 4; j++) {
                FillBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_2, sFootprintStampEmptySlotTiles[j], column, j + 20, 4, 1, 17);
            }
        } else {
            FootprintStamp_LoadFootprintIcon(mon->party[0].species, mon->party[0].form, slot->footprintSprites[0], work->narc, work->footprintNarc, 1, work->unk19E0);
            pltt[17] = mon->party[0].color;
            faded[17] = mon->party[0].color;
            BlendPalette(unfaded + (i + 17), unfaded + (i + 33), 1, 12, RGB_BLACK);
            faded[33] = pltt[33];
            if (!found) {
                found++;
                work->selectedSlot = (u8)i;
            }
        }
    }
    FootprintStamp_HighlightPartySlot(work, 0);
    ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_MAIN_2);
}

#pragma force_active reset

// Copies the footprint of a Pokemon (or the generic one) into the 2x2-tile character of a sprite.
BOOL FootprintStamp_LoadFootprintIcon(int species, int form, ManagedSprite *sprite, NARC *narc, NARC *footprintNarc, int screen, void *unk19E0) {
    void *file;
    NNSG2dCharacterData *charData;
    u8 *bottom;
    u8 *top;
    u32 vram;

    if (species == SPECIES_NONE || species > NATIONAL_DEX_COUNT) {
        return FALSE;
    }
    if (FootprintStamp_GetFootprintType(species, form, (u32)unk19E0) == 1) {
        file = GfGfxLoader_LoadFromOpenNarc(footprintNarc, species + 3, TRUE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
        DC_FlushRange(charData->pRawData, 0x100);
        top = charData->pRawData;
        bottom = top + 0x80;
    } else {
        file = GfGfxLoader_LoadFromOpenNarc(narc, 16, FALSE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
        DC_FlushRange(charData->pRawData, 0x80);
        top = charData->pRawData;
        bottom = top;
        top += 0x40;
    }
    if (screen == NNS_G2D_VRAM_TYPE_2DMAIN) {
        vram = HW_OBJ_VRAM;
    } else {
        vram = HW_DB_OBJ_VRAM;
    }
    {
        NNSG2dImageProxy *proxy = Sprite_GetImageProxy(sprite->sprite);
        MIi_CpuCopy16((u16 *)bottom, (u16 *)(vram + proxy->vramLocation.baseAddrOfVram[screen]), 0x40);
        MIi_CpuCopy16((u16 *)top, (u16 *)(vram + 0x40 + proxy->vramLocation.baseAddrOfVram[screen]), 0x40);
    }
    Heap_Free(file);
    return TRUE;
}

void FootprintStamp_CreateCamera(FootprintStamp *work) {
    VecFx32 target = sFootprintStampCameraTarget;
    fx32 width, height;
    Camera *camera;

    camera = Camera_New(HEAP_ID_125);
    work->camera = camera;
    Camera_Init_FromTargetDistanceAndAngle(&target, 0x7C000, &sFootprintStampCameraAngle, 0xFA4, 0, FALSE, work->camera);
    Camera_SetPerspectiveClippingPlane(FX32_ONE, 0x384000, work->camera);
    Camera_SetStaticPtr(work->camera);
    Math_CalcPerspectiveViewSize(Camera_GetPerspectiveAngle(work->camera), Camera_GetDistance(work->camera), 0x1555, &width, &height);
    work->viewWidth = width;
    work->viewHeight = height;
}

void FootprintStamp_FreeCamera(FootprintStamp *work) {
    Camera_Delete(work->camera);
}

#pragma force_active on

void FootprintStamp_LoadBoardModel(FootprintStamp *work, NARC *narc) {
    int fileId = work->args->unk04 == 0 ? 28 : 27;

    ModelResource_LoadFromNarc(&work->boardModel, narc, fileId, HEAP_ID_125);
    ModelInstance_Init(&work->boardInstance, &work->boardModel);
    ModelInstance_SetPosition(&work->boardInstance, 0, 0, 0);
    ModelInstance_SetScale(&work->boardInstance, FX32_ONE, FX32_ONE, FX32_ONE);
    ModelInstance_SetVisible(&work->boardInstance, TRUE);
}

#pragma force_active reset

void FootprintStamp_FreeBoardModel(FootprintStamp *work) {
    ModelResource_Free(&work->boardModel);
}

#pragma force_active on

void FootprintStamp_Draw3D(FootprintStamp *work) {
    VecFx32 scale, trans;
    MtxFx33 rot;
    u32 op;

    scale.x = FX32_ONE;
    scale.y = FX32_ONE;
    scale.z = FX32_ONE;
    trans.x = 0;
    trans.y = 0;
    trans.z = 0;
    MTX_Identity33_(&rot);
    Thunk_G3X_Reset();
    Camera_SetStaticPtr(work->camera);
    Camera_ApplyPerspectiveType(0, work->camera);
    Camera_PushLookAtToNNSGlb();
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, -FX32_ONE, 0);
    NNS_G3dGlbLightColor(GX_LIGHTID_0, GX_RGB(28, 28, 28));
    NNS_G3dGlbMaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    NNS_G3dGlbSetBaseTrans(&trans);
    NNS_G3dGlbSetBaseRot(&rot);
    NNS_G3dGlbSetBaseScale(&scale);
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    ModelInstance_Draw(&work->boardInstance);
    FootprintStampBoard_Draw(&work->board);
    op = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &op, 1);
}

#pragma force_active reset

#pragma opt_common_subs off
#pragma opt_strength_reduction off
#pragma opt_rotateloops off
#pragma force_active on

void FootprintStamp_CreateWindows(FootprintStamp *work) {
    Window *windows;
    int i;
    const u8(*pos)[2];

    AddWindowParameterized(work->bgConfig, &work->msgWindow, GF_BG_LYR_MAIN_1, 2, 1, 27, 4, 13, 0x5B);
    FillWindowPixelBuffer(&work->msgWindow, 15);
    i = 0;
    pos = sFootprintStampPlayerNameWindowPositions;
    windows = work->playerNameWindows;
    for (; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        AddWindowParameterized(work->bgConfig, &windows[i], GF_BG_LYR_SUB_1, pos[i][0], pos[i][1], 8, 2, 13, 0x200 + i * 16);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

#pragma force_active reset
#pragma opt_common_subs reset
#pragma opt_strength_reduction reset
#pragma opt_rotateloops reset

void FootprintStamp_FreeWindows(FootprintStamp *work) {
    int i;
    Window *windows;

    RemoveWindow(&work->msgWindow);
    windows = work->playerNameWindows;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        RemoveWindow(&windows[i]);
    }
}

GF3DVramMan *FootprintStamp_Create3DVramMan(enum HeapID heapId) {
    return GF_3DVramMan_Create(heapId, 0, 2, 0, 2, FootprintStamp_Setup3D);
}

void FootprintStamp_Setup3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7FFF, 63, FALSE);
    *(vu32 *)0x04000580 = 0xBFFF0000; // G3_ViewPort(0, 0, 255, 191)
}

void FootprintStamp_Free3DVramMan(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

void FootprintStamp_InitLinkFilter(FootprintStamp *work) {
    work->linkFilter.myId = WiFiPlazaLobbyClient_GetOwnId(work);
}

void FootprintStampMon_InitDummy(void *unused, FootprintStampMon *mon) {
    MI_CpuFill8(mon, 0, sizeof(FootprintStampMon));
    mon->species = SPECIES_MEWTWO;
    mon->color = 31;
    mon->personality = 257;
    mon->form = 0;
}

FootprintStampLinkFilter *FootprintStamp_GetLinkFilter(FootprintStamp *work) {
    return &work->linkFilter;
}

void FootprintStamp_LoadParty(u32 inkColumn, SaveData *saveData, FootprintStampMon *party) {
    Party *playerParty;
    Pokemon *mon;
    int count;
    int i;

    MI_CpuFill8(party, 0, FOOTPRINT_STAMP_PARTY_SLOTS * sizeof(FootprintStampMon));
    playerParty = SaveArray_Party_Get(saveData);
    count = Party_GetCount(playerParty);
    i = 0;
    if (count > 0) {
        do {
            mon = Party_GetMonByIndex(playerParty, i);
            party->species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG, NULL);
            party->personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
            party->form = GetMonData(mon, MON_DATA_FORM, NULL);
            party->color = FootprintStamp_GetInkColor(inkColumn, GetMonData(mon, MON_DATA_OT_ID, NULL));
            i++;
            party++;
        } while (i < count);
    }
}

// Puts a stamp on the board (own or received) and shows its footprint next to the player's name.
BOOL FootprintStamp_PlaceStamp(FootprintStamp *work, FootprintStampMon *mon, u32 playerId) {
    BOOL result;
    int index;
    u8 *sprites;
    u32 offset;

    result = FootprintStampBoard_AddStamp(work, &work->board, mon, work->narc, work->footprintNarc, work->unk19E0);
    if (result == TRUE) {
        index = WiFiPlazaLobbyClient_IdToChannelSlot(playerId);
        if (index != -1) {
            if (mon->species != work->playerSpecies[index] || mon->color != work->playerColors[index]) {
                work->playerSpecies[index] = mon->species;
                work->playerColors[index] = mon->color;
                sprites = (u8 *)work->playerFootprintSprites;
                offset = index * 4;
                FootprintStamp_LoadFootprintIcon(mon->species, mon->form, *(ManagedSprite **)(sprites + offset), work->narc, work->footprintNarc, NNS_G2D_VRAM_TYPE_2DSUB, work->unk19E0);
                ManagedSprite_SetDrawFlag(*(ManagedSprite **)(sprites + offset), TRUE);
                PaletteData_FillPaletteInBuffer(work->plttData, PLTTBUF_SUB_BG, PLTTSEL_BOTH, mon->color, index + 33, index + 34);
            }
        }
        PlaySE(SEQ_SE_DP_BOX02);
    }
    return result;
}

#pragma opt_common_subs off
#pragma opt_strength_reduction off
#pragma opt_rotateloops off
#pragma opt_lifetimes off
#pragma opt_propagation off
// Syncs the player list with the lobby: prints joined players' names, clears the rows of players who left.
// Returns 1 if someone joined, 2 if someone left, else 0.
#pragma force_active on

int FootprintStamp_UpdatePlayers(FootprintStamp *work) {
    int joined;
    int left;
    FootprintStampPlayerList list;
    const FootprintStampRow *rows;
    long i;
    u16 *tilemap;
    int count;
    const FootprintStampRow *row;
    u32 size;
    int y;

    joined = 0;
    left = 0;
    WiFiPlazaLobbyClient_GetChannelPlayerList(&list);
    rows = sFootprintStampPlayerRows;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        if (work->playerIds[i] != list.ids[i]) {
            if (list.ids[i] != (u32)-1) {
                FootprintStamp_PrintPlayerName(work->msgData, work->msgFormat, work->playerNameWindows, work->args->lobby, list.ids[i]);
                tilemap = GetBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_2);
                row = &rows[i];
                for (y = row->y; y < row->y + row->height; y++) {
                    size = row->width * 2;
                    MIi_CpuCopy16(&work->subTilemap[y * 32 + ((const u16 *)sFootprintStampPlayerRows)[i * 4]], &tilemap[y * 32 + ((const u16 *)sFootprintStampPlayerRows)[i * 4]], size);
                }
                ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_SUB_2);
                joined++;
            } else {
                FootprintStamp_ClearPlayerNameWindow(work->playerNameWindows, i);
                work->playerSpecies[i] = SPECIES_NONE;
                work->playerColors[i] = 0;
                ManagedSprite_SetDrawFlag(work->playerFootprintSprites[i], FALSE);
                FillBgTilemapRect(work->bgConfig, GF_BG_LYR_SUB_2, 0, sFootprintStampPlayerRows[i].x, sFootprintStampPlayerRows[i].y, sFootprintStampPlayerRows[i].width, sFootprintStampPlayerRows[i].height, 16);
                ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_SUB_2);
                left++;
            }
            if (work->linkFilter.myId == list.ids[i]) {
                ManagedSprite_SetPositionXYWithSubscreenOffset(work->myCursorSprite, sFootprintStampMyCursorPositions[i][0], sFootprintStampMyCursorPositions[i][1], 0x110000);
                ManagedSprite_SetDrawFlag(work->myCursorSprite, TRUE);
            }
        }
        work->playerIds[i] = list.ids[i];
    }
    count = 0;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        if (work->playerIds[i] != (u32)-1) {
            count++;
        }
    }
    work->board.playerCount = count;
    if (joined > 0) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        return 1;
    }
    if (left > 0) {
        PlaySE(SEQ_SE_DP_BUTTON9);
        return 2;
    }
    return 0;
}

#pragma force_active reset
#pragma opt_common_subs reset
#pragma opt_strength_reduction reset
#pragma opt_rotateloops reset
#pragma opt_lifetimes reset
#pragma opt_propagation reset

void FootprintStamp_ShowPlayerName(FootprintStamp *work, u32 playerId) {
    FootprintStamp_PrintPlayerName(work->msgData, work->msgFormat, work->playerNameWindows, work->args->lobby, playerId);
}

void FootprintStamp_ClearPlayerName(FootprintStamp *work, u32 index) {
    FootprintStamp_ClearPlayerNameWindow(work->playerNameWindows, index);
}

#pragma force_active on

void FootprintStamp_StartSelectEffect(FootprintStamp *work, int slot) {
    ManagedSpriteTemplate template = sFootprintStampSelectEffectTemplate;
    int i;
    FootprintStamp *effect;

    i = 0;
    effect = work;
    for (; i < 3; i++, effect = (FootprintStamp *)((u8 *)effect + 4)) {
        if (effect->selectSprites[0] == NULL) {
            template.x = (slot << 5) + 16;
            template.y = 176;
            work->selectSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
            break;
        }
    }
    FootprintStamp_HighlightPartySlot(work, slot);
    PlaySE(SEQ_SE_DP_BUTTON3);
}

#pragma force_active reset

void FootprintStamp_HighlightPartySlot(FootprintStamp *work, int slot) {
    u16 *unfaded = PaletteData_GetUnfadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);
    u16 *faded = PaletteData_GetFadedBuf(work->plttData, PLTTBUF_MAIN_OBJ);

    MIi_CpuCopy16(unfaded + 17, faded + 17, FOOTPRINT_STAMP_PARTY_SLOTS * sizeof(u16));
    slot += 17;
    BlendPalette(unfaded + slot, faded + slot, 1, 6, RGB_BLACK);
}

void FootprintStamp_UpdateSelectEffects(FootprintStamp *work) {
    int i;
    FootprintStamp *effect = work;

    for (i = 0; i < 3; i++, effect = (FootprintStamp *)((u8 *)effect + 4)) {
        ManagedSprite *sprite = effect->selectSprites[0];
        if (sprite != NULL) {
            if (!ManagedSprite_IsAnimated(sprite)) {
                Sprite_DeleteAndFreeResources(effect->selectSprites[0]);
                effect->selectSprites[0] = NULL;
            } else {
                Sprite_TickFrame(effect->selectSprites[0]->sprite);
            }
        }
    }
}

#pragma force_active on

void FootprintStamp_CreateTextSprite(FootprintStamp *work, FootprintStampTextSprite *text, String *string, int font, int color, int priority, int plttResId, int x, int y, BOOL centered) {
    TextOBJTemplate template;
    Window window;
    ObjCharSpaceReservation charTransfer;
    int width, tiles;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    TextOBJ *textObj;

    if (text->textObj != NULL) {
        GF_AssertFail();
    }
    bgConfig = work->bgConfig;
    spriteManager = work->spriteManager;
    FootprintStamp_GetTextSize(string, font, &width, &tiles);
    InitWindow(&window);
    AddTextWindowTopLeftCorner(bgConfig, &window, (u8)tiles, 2, 0, 0);
    AddTextPrinterParameterizedWithColorAndSpacing(&window, font, string, 0, 0, TEXT_SPEED_NOTRANSFER, color, 0, 0, NULL);
    ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_125), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &charTransfer);
    if (centered == TRUE) {
        x -= width / 2;
    }
    template.fontSystem = work->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(spriteManager, plttResId);
    template.sprite = NULL;
    template.offset = charTransfer.offset;
    template.x = x;
    template.y = y - 8;
    template.unk_20 = 0;
    template.unk_24 = 51;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_125;
    textObj = FontOAM_Create(&template);
    TextOBJ_SetPalOffsetRespectVramOffset(textObj, priority);
    TextOBJ_SetPosition(textObj, x, y - 8);
    RemoveWindow(&window);
    text->textObj = textObj;
    text->charTransfer = charTransfer;
    text->width = width;
}

#pragma force_active reset

void FootprintStamp_FreeTextSprite(FootprintStampTextSprite *text) {
    FontOAM_Delete(text->textObj);
    ObjCharTransfer_FreeCharSpace(&text->charTransfer);
}

void FootprintStamp_GetTextSize(String *string, int font, int *width, int *tiles) {
    int textWidth = FontID_String_GetWidth(font, string, 0);
    int textTiles = textWidth / 8;

    if (FX_ModS32(textWidth, 8) != 0) {
        textTiles++;
    }
    *width = textWidth;
    *tiles = textTiles;
}

static inline int FootprintStamp_InkGaugeY(int y) {
    return y + 176;
}

// Refills the ink and moves the ink level shown above the party slots towards it.
#pragma force_active on

void FootprintStamp_UpdateInkGauge(FootprintStamp *work) {
    int i, x;
    s16 y;
    FootprintStamp *slot;

    if (work->ink == work->inkShown) {
        work->ink += 0x88;
        if (work->ink > FOOTPRINT_STAMP_INK_MAX) {
            work->ink = FOOTPRINT_STAMP_INK_MAX;
        }
        work->inkShown = work->ink;
    } else {
        work->inkShown -= work->inkStep;
        if (work->inkShown < work->ink) {
            work->inkShown = work->ink;
        }
    }
    i = 0;
    y = FootprintStamp_InkGaugeY(32 - (work->inkShown >> 8));
    x = 16;
    slot = work;
    do {
        ManagedSprite_SetPositionXYWithSubscreenOffset(slot->inkSprites[0], x, y, 0x110000);
        i++;
        x += 32;
        slot = (FootprintStamp *)((u8 *)slot + 4);
    } while (i < FOOTPRINT_STAMP_PARTY_SLOTS);
}

#pragma force_active reset

BOOL FootprintStamp_UseInk(FootprintStamp *work, fx32 amount) {
    fx32 diff;

    if (work->ink < amount) {
        return FALSE;
    }
    if (work->inkShown < work->ink) {
        GF_ASSERT(FALSE);
        work->inkShown = work->ink;
    }
    work->ink -= amount;
    diff = work->inkShown - work->ink;
    work->inkStep = diff / 4;
    return TRUE;
}

void ov89_0225A468(FootprintStamp *work) {
}

void FootprintStampBoard_Init(FootprintStampBoard *board, void *unk19E0) {
    MI_CpuFill8(board, 0, sizeof(FootprintStampBoard));
}

void FootprintStampBoard_Free(FootprintStampBoard *board) {
    int i;
    FootprintStampBoard *slot = board;

    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++, slot = (FootprintStampBoard *)((u8 *)slot + 4)) {
        if (slot->stamps[0] != NULL) {
            FootprintStamp_DeleteStamp(slot->stamps[0]);
        }
    }
}

BOOL FootprintStampBoard_AddStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0) {
    int i;

    if (board->cameraEffect != FOOTPRINT_STAMP_EFFECT_NONE) {
        return FALSE;
    }
    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        if (board->stamps[i] == NULL) {
            board->stamps[i] = FootprintStampBoard_CreateStamp(work, board, mon, narc, footprintNarc, unk19E0);
            if (board->stamps[i] != NULL) {
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}

#pragma force_active on

void FootprintStampBoard_Update(FootprintStampBoard *board, Camera *camera, int phase, u32 inkColumn) {
    int i;

    if (board->pendingCameraEffect) {
        FootprintStampBoard_Flash(board, &board->flash, phase, inkColumn);
    }
    if (board->cameraEffect) {
        if (sFootprintStampCameraEffectFuncs[board->cameraEffect](board, board, camera) == TRUE) {
            board->cameraEffect = FOOTPRINT_STAMP_EFFECT_NONE;
        }
    }
    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        FootprintStampObj *stamp = board->stamps[i];
        if (stamp) {
            if (stamp->effectId == FOOTPRINT_STAMP_EFFECT_NONE && stamp->pendingEffect != FOOTPRINT_STAMP_EFFECT_NONE) {
                stamp->effectId = stamp->pendingEffect;
            }
            if (sFootprintStampEffectFuncs[stamp->effectId].update(board, stamp) == TRUE) {
                FootprintStampBoard_RemoveStamp(board, stamp, i);
            }
        }
    }
}

#pragma force_active reset

void FootprintStampBoard_Draw(FootprintStampBoard *board) {
    int i;

    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        FootprintStampObj *stamp = board->stamps[i];
        if (stamp != NULL) {
            u32 effect = stamp->effectId;
            if (sFootprintStampEffectFuncs[effect].draw != NULL) {
                sFootprintStampEffectFuncs[effect].draw(board, stamp);
            } else {
                ModelInstance_Draw(&stamp->instance);
            }
        }
    }
}

// Idle stamps touched by a stamp playing an effect take over that effect and join its chain; once a chain
// is long enough, the other stamps are removed and the chain's camera move starts.
#pragma force_active on

void FootprintStampBoard_ResolveChains(FootprintStampBoard *board) {
    int i;
    int numActive;
    int numIdle;
    int m;
    int j;
    FootprintStampObj *stamp;
    FootprintStampObj *other;

    numActive = 0;
    numIdle = 0;
    for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
        stamp = board->stamps[i];
        if (stamp != NULL) {
            if (stamp->effectId != FOOTPRINT_STAMP_EFFECT_NONE) {
                board->sorted[numActive] = stamp;
                numActive++;
            } else {
                board->sorted[FOOTPRINT_STAMP_STAMPS_MAX - 1 - numIdle] = stamp;
                numIdle++;
            }
        }
    }
    for (j = 0; j < numIdle; j++) {
        other = board->sorted[FOOTPRINT_STAMP_STAMPS_MAX - 1 - j];
        for (m = 0; m < numActive; m++) {
            stamp = board->sorted[m];
            if (sFootprintStampEffectFuncs[stamp->effectId].hit(board, stamp, other) == TRUE && FootprintStamp_SetEffect(other, stamp->effectId) == TRUE && other->chain == 0xFF) {
                other->chain = stamp->chain;
                FootprintStampBoard_AddToChain(board, stamp->chain, stamp->effectId);
                break;
            }
        }
    }
    if (board->pendingCameraEffect != FOOTPRINT_STAMP_EFFECT_NONE && board->flashDone == TRUE) {
        for (i = 0; i < FOOTPRINT_STAMP_STAMPS_MAX; i++) {
            stamp = board->stamps[i];
            if (stamp != NULL && stamp->effectId != board->pendingCameraEffect && stamp->pendingEffect != board->pendingCameraEffect && FootprintStamp_SetEffect(stamp, board->pendingCameraEffect) == FALSE) {
                FootprintStampBoard_RemoveStamp(board, stamp, i);
            }
        }
        FootprintStampBoard_StartCameraEffect(board);
    }
}

#pragma force_active reset

// Uploads the textures of new or redrawn stamps, and the flash brightness.
void FootprintStampBoard_VBlank(FootprintStampBoard *board, int phase) {
    int i;
    NNSG3dResTex **texture = board->pendingTextures;

    for (i = 0; i < FOOTPRINT_STAMP_TEXTURES_MAX; i++, texture++) {
        if (*texture != NULL) {
            DC_FlushRange(*texture, (*texture)->header.size);
            NNS_G3dTexLoad(*texture, TRUE);
            NNS_G3dPlttLoad(*texture, TRUE);
            *texture = NULL;
        }
    }
    if (board->brightnessChanged != FALSE && phase != 3) {
        SetMasterBrightness(PM_LCD_TOP, board->brightness);
        board->brightnessChanged = FALSE;
    }
}

#pragma force_active on

FootprintStampObj *FootprintStampBoard_CreateStamp(FootprintStamp *work, FootprintStampBoard *board, const FootprintStampMon *mon, NARC *narc, NARC *footprintNarc, void *unk19E0) {
    FootprintStampObj *stamp = Heap_Alloc(HEAP_ID_125, sizeof(FootprintStampObj));
    fx32 x, y;

    MI_CpuFill8(stamp, 0, sizeof(FootprintStampObj));
    stamp->chain = 0xFF;
    stamp->mon = *mon;
    if (stamp->mon.species > NATIONAL_DEX_COUNT) {
        GF_AssertFail();
        stamp->mon.species = SPECIES_DITTO;
    }
    if (!FootprintStamp_LoadStampModel(board, &stamp->model, narc, footprintNarc, &stamp->mon, unk19E0)) {
        return NULL;
    }
    ModelInstance_Init(&stamp->instance, &stamp->model);
    FootprintStamp_TouchToBoardPos(work, mon->x, mon->y, &x, &y);
    ModelInstance_SetPosition(&stamp->instance, x, y, 0x10000);
    ModelInstance_SetScale(&stamp->instance, FX32_ONE, FX32_ONE, FX32_ONE);
    ModelInstance_SetVisible(&stamp->instance, FALSE);
    return stamp;
}

#pragma force_active reset

#pragma force_active on

BOOL FootprintStamp_LoadStampModel(FootprintStampBoard *board, FootprintStampModel *model, NARC *narc, NARC *footprintNarc, FootprintStampMon *mon, void *unk19E0) {
    model->file = GfGfxLoader_LoadFromOpenNarc(narc, 29, FALSE, HEAP_ID_125, FALSE);
    GF_ASSERT(model->file);
    model->set = NNS_G3dGetMdlSet(model->file);
    model->model = NNS_G3dGetMdlByIdx(model->set, 0);
    model->texture = NNS_G3dGetTex(model->file);
    if (model->texture) {
        int allocated;
        FootprintStamp_DrawFootprintTexture(model->texture, narc, footprintNarc, mon, (u32)unk19E0);
        allocated = FootprintStamp_AllocTextureVram(model->texture);
        if (!allocated || !FootprintStampBoard_QueueTexture(board, model->texture)) {
            if (allocated == TRUE) {
                NNSG3dTexKey texKey, tex4x4Key;
                NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
                NNS_GfdDefaultFuncFreeTexVram(texKey);
                NNS_GfdDefaultFuncFreeTexVram(tex4x4Key);
                NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(model->texture));
            }
            if (model->file) {
                Heap_Free(model->file);
            }
            memset(model, 0, sizeof(FootprintStampModel));
            return FALSE;
        }
        GF3dRender_BindModelSet(model->file, model->texture);
    }
    return TRUE;
}

#pragma force_active reset

BOOL FootprintStampBoard_QueueTexture(FootprintStampBoard *board, NNSG3dResTex *texture) {
    int i;

    for (i = 0; i < FOOTPRINT_STAMP_TEXTURES_MAX; i++) {
        if (board->pendingTextures[i] == NULL) {
            board->pendingTextures[i] = texture;
            return TRUE;
        }
    }
    return FALSE;
}

void FootprintStampBoard_UnqueueTexture(FootprintStampBoard *board, NNSG3dResTex *texture) {
    int i;

    for (i = 0; i < FOOTPRINT_STAMP_TEXTURES_MAX; i++) {
        if (board->pendingTextures[i] == texture) {
            board->pendingTextures[i] = NULL;
            return;
        }
    }
}

BOOL FootprintStamp_AllocTextureVram(NNSG3dResTex *texture) {
    u32 texSize;
    u32 plttSize;
    NNSG3dTexKey texKey;
    NNSG3dPlttKey plttKey;

    texture->texInfo.vramKey = 0;
    texSize = NNS_G3dTexGetRequiredSize(texture);
    plttSize = NNS_G3dPlttGetRequiredSize(texture);
    texKey = NNS_GfdDefaultFuncAllocTexVram(texSize, FALSE, 0);
    if (texKey == 0) {
        return FALSE;
    }
    plttKey = NNS_GfdDefaultFuncAllocPlttVram(plttSize, texture->tex4x4Info.flag & NNS_G3D_RESPLTT_USEPLTT4, 0);
    if (plttKey == 0) {
        NNS_GfdDefaultFuncFreeTexVram(texKey);
        return FALSE;
    }
    NNS_G3dTexSetTexKey(texture, texKey, 0);
    NNS_G3dPlttSetPlttKey(texture, plttKey);
    return TRUE;
}
