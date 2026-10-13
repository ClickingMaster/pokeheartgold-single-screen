#include "application/plaza_game.h"

#include "global.h"

#include "constants/gx.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0236.h"

#include "assert.h"
#include "filesystem.h"
#include "font.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_00_thumb.h"
#include "render_text.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "oam_manager.h"
#include "friend_roster.h"
#include "communication_command.h"
#include "communication_player_state.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"

// These are declared only in other modules' private headers.
struct DoubleBuffer *DoubleBuffer_New(enum HeapID heapId, void *first, void *second);
void DoubleBuffer_Delete(struct DoubleBuffer *buffer);
u16 *DoubleBuffer_GetCurrent(struct DoubleBuffer *buffer);
s16 *DoubleBuffer_GetOther(struct DoubleBuffer *buffer);
void DoubleBuffer_SetEnabled(struct DoubleBuffer *buffer, u8 enabled);
void DoubleBuffer_Swap(struct DoubleBuffer *buffer);
void sub_0202C554(FriendRoster *data, s32 index, s32 value);
void sub_0202C584(FriendRoster *data, s32 index, s32 value);
void sub_0202C5B4(FriendRoster *data, s32 index, s32 value);
void *CommPlayerInfo_GetExchangeToken(int netId);
u8 CommPlayerInfo_GetCountry(int netId);
u8 CommPlayerInfo_GetRegion(int netId);
void CommState_EndPlazaMatch(void);
int Comm_FindFriendSlot(SaveData *saveData, void *friendData, u32 *index);
void WirelessStatusIcon_ShowOnBothScreens(int a0, enum HeapID heapId);
void ov45_0222ACB8(void *lobby, u8 game, int count, u32 id0, u32 id1, u32 id2, u32 id3);

extern const u8 _0225C1E0[];
extern const u8 ov90_0225C1E4[];
extern const u8 ov90_0225C1E8[];
extern const u8 ov90_0225C1EC[];
extern const s16 ov90_0225C1F0[];
extern const fx32 ov90_0225C1F8[];
extern const fx32 ov90_0225C204[];
extern const u32 ov90_0225C210[];
extern const fx32 ov90_0225C21C[];
extern const fx32 ov90_0225C228[];
extern const fx32 ov90_0225C234[];
extern const fx32 ov90_0225C240[];
extern const fx32 ov90_0225C24C[];
extern const fx32 ov90_0225C258[];
extern const GraphicsModes ov90_0225C264;
extern const s16 ov90_0225C274[];
extern const s16 ov90_0225C276[];
extern const u8 ov90_0225C284[][4];
extern const ObjCharTransferTemplate ov90_0225C294;
extern const GraphicsModes ov90_0225C2A4;
extern const VecFx32 ov90_0225C2B4[2];
extern const GraphicsBanks ov90_0225C2CC;
extern const GraphicsBanks ov90_0225C2F4;
extern const u8 ov90_0225C310[][12];
extern const u8 ov90_0225C314[][12];
extern const u8 ov90_0225C318[][12];
extern const u8 ov90_0225C31C[][12];
extern const u8 ov90_0225C324[][12];
extern const u16 ov90_0225C34C[20][2];
extern const PlazaGameBgTemplate ov90_0225C39C[];
extern const PlazaGameBgTemplate ov90_0225C43C[];
extern const u8 ov90_0225C4FC[];

void PlazaGameRoster_Init(PlazaGameRoster *roster, u32 fromLobby, SaveData *saveData, u8 unkA, PlazaGameArgs *args) {
    int netId;
    int localNetId;
    int count;
    memset(roster, 0, sizeof(PlazaGameRoster));
    roster->count = CommPlayerInfo_CountAccepted();
    localNetId = Comm_GetLocalNetId();
    count = 0;
    for (netId = 0; netId < 4; netId++) {
        if (CommPlayerInfo_GetProfile(netId) != NULL) {
            if (localNetId == netId) {
                roster->localIndex = count;
            }
            roster->netIds[count] = netId;
            count++;
        }
    }
    GF_ASSERT(count == roster->count);
    if (fromLobby == 0) {
        roster->launchedFromField = TRUE;
    }
    roster->saveData = saveData;
    roster->unkA = unkA;
    roster->args = args;
}

int PlazaGameRoster_GetIndexOfNetId(const PlazaGameRoster *roster, u32 netId) {
    int i;
    for (i = 0; i < roster->count; i++) {
        if (netId == roster->netIds[i]) {
            return i;
        }
    }
    return 4;
}

u8 PlazaGameRoster_GetNetId(const PlazaGameRoster *roster, u32 index) {
    GF_ASSERT(index < roster->count);
    return roster->netIds[index];
}

#pragma force_active on

u32 ov90_022588A4(const PlazaGameRoster *roster, u32 netId) {
    int index;
    if (netId >= 4) {
        GF_ASSERT(netId < 4);
        return 0;
    }
    index = roster->args->unk04[netId];
    if (index == -1) {
        return 0;
    }
    return roster->args->unk08[index];
}

PlayerProfile *PlazaGameRoster_GetProfile(const PlazaGameRoster *roster, u32 netId) {
    if (netId >= 4) {
        GF_ASSERT(netId < 4);
        return NULL;
    }
    if (roster->launchedFromField == 1) {
        int index = PlazaGameRoster_GetIndexOfNetId(roster, netId);
        return index == roster->localIndex ? Save_PlayerData_GetProfile(roster->saveData) : CommPlayerInfo_GetProfile(netId);
    } else {
        GF_ASSERT(roster->args != NULL);
        return roster->args->profiles[netId];
    }
}

#pragma force_active reset

PlazaGameEntry *PlazaGameEntry_NewSwalotPlop(const PlazaGameRoster *roster, enum HeapID heapId) {
    return PlazaGameEntry_New(roster, 0, heapId);
}

PlazaGameEntry *PlazaGameEntry_NewMimeJrTop(const PlazaGameRoster *roster, enum HeapID heapId) {
    return PlazaGameEntry_New(roster, 1, heapId);
}

PlazaGameEntry *PlazaGameEntry_NewWobbuffetPop(const PlazaGameRoster *roster, enum HeapID heapId) {
    return PlazaGameEntry_New(roster, 2, heapId);
}

void PlazaGameEntry_Free(PlazaGameEntry *entry) {
    int i;
    SysTask_Destroy(entry->task);
    SysTask_Destroy(entry->vblankTask);
    PlazaGameLineScroll_Free(&entry->lineScroll);
    PlazaGameTitle_Free(&entry->title);
    for (i = 0; i < entry->roster.count; i++) {
        PlazaGamePlayer_Free(&entry->players[i]);
    }
    PlazaGameMessageBox_Free(&entry->messageBox);
    PlazaGameEntry_FreeGraphics(entry);
    PlazaGameText_Free(&entry->text);
    PlazaGameGfx_Free(&entry->gfx);
    PlazaGameSprites_Free(&entry->sprites);
    PlazaGameBg_Free(&entry->bg);
    Heap_Free(entry);
}

BOOL PlazaGameEntry_IsDone(const PlazaGameEntry *entry) {
    return entry->state >= 16;
}

u8 PlazaGameEntry_GetSelection(const PlazaGameEntry *entry) {
    GF_ASSERT(entry->selectionReceived == 1);
    return entry->selection;
}

PlazaGameResults *PlazaGameResults_NewSwalotPlop(const PlazaGameRoster *roster, const PlazaGameResultData *data, enum HeapID heapId) {
    return PlazaGameResults_New(roster, data, 0, heapId);
}

PlazaGameResults *PlazaGameResults_NewMimeJrTop(const PlazaGameRoster *roster, const PlazaGameResultData *data, enum HeapID heapId) {
    return PlazaGameResults_New(roster, data, 1, heapId);
}

PlazaGameResults *PlazaGameResults_NewWobbuffetPop(const PlazaGameRoster *roster, const PlazaGameResultData *data, enum HeapID heapId) {
    return PlazaGameResults_New(roster, data, 2, heapId);
}

#pragma force_active on

void PlazaGameResults_Free(PlazaGameResults *results) {
    int i;
    SysTask_Destroy(results->task);
    SysTask_Destroy(results->vblankTask);
    PlazaGameLineScroll_Free(&results->lineScroll);
    for (i = 0; i < results->roster.count; i++) {
        PlazaGamePlayer_Free(&results->players[i]);
    }
    PlazaGameContinue_Free(&results->continuePrompt);
    PlazaGameResults_FreeGraphics(results);
    PlazaGameMessageBox_Free(&results->messageBox);
    PlazaGameText_Free(&results->text);
    PlazaGameGfx_Free(&results->gfx);
    PlazaGameSprites_Free(&results->sprites);
    PlazaGameBg_Free(&results->bg);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GF_DestroyVramTransferManager();
    Heap_Free(results);
}

#pragma force_active reset

u8 PlazaGameResults_IsDone(const PlazaGameResults *results) {
    return results->done;
}

u8 PlazaGameResults_GetContinue(const PlazaGameResults *results) {
    return results->continueResult;
}

#pragma force_active on

void PlazaGame_CalcRanks(PlazaGameResultData *data, u32 count) {
    u8 order[4];
    int i;
    int j;
    for (i = 0; i < count; i++) {
        for (j = i; j > 0; j--) {
            if (data->scores[i] <= data->scores[order[j - 1]]) {
                break;
            }
            order[j] = order[j - 1];
        }
        order[j] = i;
    }
    for (i = 0; i < count; i++) {
        int rank = i;
        if (i > 0 && data->scores[order[i]] == data->scores[order[i - 1]]) {
            rank = data->ranks[order[i - 1]];
        }
        data->ranks[order[i]] = rank;
    }
}

#pragma force_active reset

void PlazaGameEntry_SetSelection(PlazaGameEntry *entry, int selection) {
    entry->selection = selection;
    entry->selectionReceived = 1;
}

#pragma force_active on

void PlazaGameResults_ReceiveVote(PlazaGameResults *results, int netId, int yes) {
    if (!results->roster.localIndex) {
        PlazaGameContinue_ReceiveVote(&results->continuePrompt, netId, yes, results->roster.count);
    }
}

#pragma force_active reset

void PlazaGameResults_ReceiveContinue(PlazaGameResults *results, u8 result) {
    PlazaGameContinue_SetResult(&results->continuePrompt, result);
}

BOOL PlazaGameRoster_CheckConnectionLost(PlazaGameRoster *roster) {
    BOOL result = FALSE;

    if (roster->args != NULL) {
        if (roster->count != Comm_CountConnected()) {
            result = TRUE;
        }
        if (CommState_WasWifiDisconnected() == 1) {
            result = TRUE;
        }
        if (CommState_GetWifiMatchStatus() >= 2) {
            result = TRUE;
        }
        if (result == TRUE) {
            roster->args->connectionLost = TRUE;
        }
        if (roster->args->connectionLost == TRUE) {
            result = TRUE;
        }
    }
    return result;
}

BOOL PlazaGameRoster_ProcessConnectionLost(PlazaGameRoster *roster) {
    GF_ASSERT(roster->args != NULL);
    GF_ASSERT(roster->args->connectionLost == TRUE);
    if (CommPlayerInfo_Exists() == 1) {
        CommPlayerInfo_Free();
        CommState_EndPlazaMatch();
    } else if (CommState_IsWifiInLobby() == 1) {
        return TRUE;
    }
    return FALSE;
}

PlazaGameSign *PlazaGameSign_New(SpriteList *spriteList, enum HeapID heapId) {
    PlazaGameSign *sign = Heap_Alloc(heapId, sizeof(PlazaGameSign));
    memset(sign, 0, sizeof(PlazaGameSign));
    sign->spriteList = spriteList;
    sign->narc = NARC_New(NARC_a_1_9_8, heapId);
    PlazaGameGfx_Init(&sign->gfx, 1, heapId);
    sign->gfxRes = PlazaGameGfx_Load(&sign->gfx, sign->narc, 3, 3, 2, 1, 0, 5000, heapId);
    return sign;
}

void PlazaGameSign_Free(PlazaGameSign *sign) {
    if (sign->task != NULL) {
        SysTask_Destroy(sign->task);
        sign->task = NULL;
    }
    if (sign->sprite != NULL) {
        Sprite_Delete(sign->sprite);
    }
    PlazaGameSignWindow_Free(&sign->window);
    PlazaGameGfx_Free(&sign->gfx);
    NARC_Delete(sign->narc);
    Heap_Free(sign);
}

int PlazaGameSign_GetPlttOffset(PlazaGameSign *sign) {
    return SpriteTransfer_GetPlttOffset(SpriteResourceCollection_Find(sign->gfx.managers[GF_GFX_RES_TYPE_PLTT], 5000), NNS_G2D_VRAM_TYPE_2DMAIN);
}

#pragma force_active on

void PlazaGameSign_StartCountdown(PlazaGameSign *sign) {
    GF_ASSERT(sign->task == NULL);
    sign->task = SysTask_CreateOnMainQueue(PlazaGameSign_CountdownTask, sign, 0);
    sign->state = 1;
}

void PlazaGameSign_StartFinish(PlazaGameSign *sign) {
    GF_ASSERT(sign->task == NULL);
    sign->task = SysTask_CreateOnMainQueue(PlazaGameSign_FinishTask, sign, 0);
    sign->state = 1;
    PlaySE(SEQ_SE_DP_PINPON);
}

#pragma force_active reset

BOOL PlazaGameSign_IsDone(const PlazaGameSign *sign) {
    if (sign->state == 0) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

void PlazaGameMotion_Init(PlazaGameMotion *motion, fx32 start, fx32 end, fx32 velocity, int duration) {
    fx32 distance = end - start - (fx32)(((s64)velocity * (duration << FX32_SHIFT) + 0x800) >> FX32_SHIFT);
    fx32 accel = FX_Div((fx32)(((s64)distance * (2 * FX32_ONE) + 0x800) >> FX32_SHIFT), (duration * duration) << FX32_SHIFT);
    motion->current = start;
    motion->start = start;
    motion->velocity = velocity;
    motion->accel = accel;
    motion->duration = duration;
}

int PlazaGameMotion_Update(PlazaGameMotion *motion, int frame) {
    int done;
    fx32 linear;
    fx32 quadratic;
    if (frame >= motion->duration) {
        frame = motion->duration;
        done = 1;
    } else {
        done = 0;
    }
    linear = (fx32)(((s64)motion->velocity * (frame << FX32_SHIFT) + 0x800) >> FX32_SHIFT);
    quadratic = (fx32)(((s64)motion->accel * ((frame * frame) << FX32_SHIFT) + 0x800) >> FX32_SHIFT);
    quadratic = FX_Div(quadratic, 2 * FX32_ONE);
    motion->current = motion->start + (linear + quadratic);
    return done;
}

#pragma force_active reset

void PlazaGame_OffsetScreenTiles(NNSG2dScreenData *screen, u16 offset) {
    int i;
    int count = screen->szByte / 2;
    u16 *tiles = (u16 *)screen->rawData;
    for (i = 0; i < count; i++) {
        tiles[i] += offset;
    }
}

void PlazaGameGfx_Init(PlazaGameGfx *gfx, int count, enum HeapID heapId) {
    int i;
    for (i = 0; i < 4; i++) {
        gfx->managers[i] = Create2DGfxResObjMan(count, (GfGfxResType)i, heapId);
    }
    gfx->records = Heap_Alloc(heapId, count * sizeof(PlazaGameGfxRes));
    memset(gfx->records, 0, count * sizeof(PlazaGameGfxRes));
    gfx->count = count;
}

void PlazaGameGfx_Free(PlazaGameGfx *gfx) {
    u32 i;
    for (i = 0; i < gfx->count; i++) {
        if (gfx->records[i].active == 1) {
            PlazaGameGfx_Unload(gfx, &gfx->records[i]);
        }
    }
    {
        int j;
        for (j = 0; j < 4; j++) {
            Destroy2DGfxResObjMan(gfx->managers[j]);
        }
    }
    Heap_Free(gfx->records);
}

PlazaGameGfxRes *PlazaGameGfx_Load(PlazaGameGfx *gfx, NARC *narc, int plttFile, int plttCount, int charFile, int cellFile, int animFile, int id, enum HeapID heapId) {
    u32 i;
    PlazaGameGfxRes *gfxRes = NULL;
    for (i = 0; i < gfx->count; i++) {
        if (gfx->records[i].active == 0) {
            gfxRes = &gfx->records[i];
            break;
        }
    }
    GF_ASSERT(gfxRes != NULL);
    PlazaGameGfx_LoadInto(gfx, gfxRes, narc, plttFile, plttCount, charFile, cellFile, animFile, id, heapId);
    return gfxRes;
}

Sprite *PlazaGameGfx_CreateSprite(PlazaGameGfxRes *gfxRes, SpriteList *spriteList, s16 x, s16 y, u16 priority, enum HeapID heapId) {
    SimpleSpriteTemplate template;
    template.spriteList = spriteList;
    template.header = &gfxRes->header;
    template.position.x = x << FX32_SHIFT;
    template.position.y = y << FX32_SHIFT;
    template.priority = priority;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = heapId;
    return Sprite_Create(&template);
}

#pragma force_active on

// The volatile copies of id and heapId keep them on the stack as in the original.
void PlazaGameGfx_LoadInto(PlazaGameGfx *gfx, PlazaGameGfxRes *gfxRes, NARC *narc, int plttFile, int plttCount, int charFile, int cellFile, int animFile, volatile int id, volatile enum HeapID heapId) {
    enum HeapID heap;
    int resId;
    GF_ASSERT(!gfxRes->active);
    gfxRes->active = 1;
    resId = id;
    heap = heapId;
    gfxRes->resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->managers[GF_GFX_RES_TYPE_CHAR], narc, charFile, FALSE, resId, NNS_G2D_VRAM_TYPE_2DMAIN, heap);
    gfxRes->resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(gfx->managers[GF_GFX_RES_TYPE_PLTT], narc, plttFile, FALSE, resId, NNS_G2D_VRAM_TYPE_2DMAIN, plttCount, heap);
    gfxRes->resources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->managers[GF_GFX_RES_TYPE_CELL], narc, cellFile, FALSE, resId, GF_GFX_RES_TYPE_CELL, heap);
    gfxRes->resources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->managers[GF_GFX_RES_TYPE_ANIM], narc, animFile, FALSE, resId, GF_GFX_RES_TYPE_ANIM, heap);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(gfxRes->resources[GF_GFX_RES_TYPE_CHAR]));
    GF2DGfxResObj_FreeData(gfxRes->resources[GF_GFX_RES_TYPE_CHAR]);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(gfxRes->resources[GF_GFX_RES_TYPE_PLTT]));
    GF2DGfxResObj_FreeData(gfxRes->resources[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&gfxRes->header, resId, resId, resId, resId, -1, -1, FALSE, 1, gfx->managers[GF_GFX_RES_TYPE_CHAR], gfx->managers[GF_GFX_RES_TYPE_PLTT], gfx->managers[GF_GFX_RES_TYPE_CELL], gfx->managers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

#pragma force_active reset

void PlazaGameGfx_Unload(PlazaGameGfx *gfx, PlazaGameGfxRes *gfxRes) {
    int i;
    GF_ASSERT(gfxRes->active == 1);
    gfxRes->active = 0;
    SpriteTransfer_DeleteCharTransferTask(gfxRes->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(gfxRes->resources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(gfx->managers[i], gfxRes->resources[i]);
    }
}

void PlazaGameBg_Init(PlazaGameBg *bg, const GraphicsModes *modes, const PlazaGameBgTemplate *templates, u32 count, enum HeapID heapId) {
    u32 i;
    const PlazaGameBgTemplate *bgTemplate;
    const PlazaGameBgTemplate *current;
    SetBothScreensModesAndDisable(modes);
    bg->bgConfig = BgConfig_Alloc(heapId);
    bg->templates = templates;
    bg->count = count;
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    bgTemplate = templates;
    for (i = 0; i < count; i++) {
        current = &templates[i];
        InitBgFromTemplate(bg->bgConfig, (u8)current->bgId, &bgTemplate->template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange((u8)current->bgId, 0x20, 0, heapId);
        BgClearTilemapBufferAndCommit(bg->bgConfig, (u8)current->bgId);
        BgSetPosTextAndCommit(bg->bgConfig, (u8)current->bgId, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(bg->bgConfig, (u8)current->bgId, BG_POS_OP_SET_Y, 0);
        bgTemplate++;
    }
}

void PlazaGameBg_Free(PlazaGameBg *bg) {
    u32 i;
    BgSetPosTextAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    for (i = 0; i < bg->count; i++) {
        FreeBgTilemapBuffer(bg->bgConfig, (u8)bg->templates[i].bgId);
    }
    Heap_Free(bg->bgConfig);
}

void PlazaGameBg_Update(PlazaGameBg *bg) {
    DoScheduledBgGpuUpdates(bg->bgConfig);
}

#pragma force_active on

void PlazaGameSprites_Init(PlazaGameSprites *sprites, int count, int charCount, int plttCount, enum HeapID heapId) {
    ObjCharTransferTemplate template;
    enum HeapID heap;
    NNS_G2dInitOamManagerModule();
    // Read through a volatile pointer: the original reloads heapId from the stack here.
    heap = *(volatile enum HeapID *)&heapId;
    OamManager_Create(0, 126, 0, 31, 0, 126, 0, 31, heap);
    template = ov90_0225C294;
    template.maxTasks = charCount;
    template.heapID = heap;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(plttCount, heap);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    sprites->spriteList = G2dRenderer_Init(count, &sprites->renderer, heap);
    WirelessStatusIcon_Create();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

#pragma force_active reset

void PlazaGameSprites_Free(PlazaGameSprites *sprites) {
    SpriteList_Delete(sprites->spriteList);
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void PlazaGameSprites_Update(PlazaGameSprites *sprites) {
    SpriteList_RenderAndAnimateSprites(sprites->spriteList);
}

void PlazaGameSprites_ApplyOam(PlazaGameSprites *sprites) {
    OamManager_ApplyAndResetBuffers();
}

#pragma force_active on

void PlazaGameText_Init(PlazaGameText *text, enum HeapID heapId) {
    text->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0236_bin, heapId);
    text->msgFormat = MessageFormat_New_Custom(8, 64, heapId);
    text->string = String_New(128, heapId);
    text->buffer = String_New(128, heapId);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, heapId);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, heapId);
}

#pragma force_active reset

void PlazaGameText_Free(PlazaGameText *text) {
    String_Delete(text->buffer);
    String_Delete(text->string);
    MessageFormat_Delete(text->msgFormat);
    DestroyMsgData(text->msgData);
}

void PlazaGameText_ResetBuffers(PlazaGameText *text) {
    MessageFormat_ResetBuffers(text->msgFormat);
}

void PlazaGameText_BufferPlayerName(PlazaGameText *text, PlayerProfile *profile) {
    BufferPlayersName(text->msgFormat, 0, profile);
}

void PlazaGameText_BufferCountry(PlazaGameText *text, int country) {
    BufferCountryName(text->msgFormat, 0, country);
}

void PlazaGameText_BufferCity(PlazaGameText *text, int country, int region) {
    BufferCityName(text->msgFormat, 0, country, region);
}

void PlazaGameText_BufferNumber(PlazaGameText *text, int value, int digits) {
    BufferIntegerAsString(text->msgFormat, 0, value, digits, PRINTING_MODE_RIGHT_ALIGN, TRUE);
}

void PlazaGameText_BufferTwoDigitNumber(PlazaGameText *text, int value) {
    BufferIntegerAsString(text->msgFormat, 0, value, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
}

void PlazaGameText_BufferTapToyName(PlazaGameText *text, int tapToy) {
    BufferWiFiPlazaInstrumentName(text->msgFormat, 1, tapToy);
}

void PlazaGameText_BufferActivityName(PlazaGameText *text, int game) {
    BufferWiFiPlazaActivityName(text->msgFormat, 0, game);
}

void PlazaGameText_Format(PlazaGameText *text, String *dest, int msgNo) {
    ReadMsgDataIntoString(text->msgData, msgNo, text->buffer);
    StringExpandPlaceholders(text->msgFormat, dest, text->buffer);
}

void PlazaGameText_Print(PlazaGameText *text, int msgNo, Window *window, u8 x, u8 y) {
    PlazaGameText_PrintWithColor(text, msgNo, window, x, y, MAKE_TEXT_COLOR(15, 14, 0));
}

int PlazaGameText_PrintRightAligned(PlazaGameText *text, int msgNo, Window *window, u8 right, u8 y) {
    int x;
    ReadMsgDataIntoString(text->msgData, msgNo, text->buffer);
    StringExpandPlaceholders(text->msgFormat, text->string, text->buffer);
    x = right - FontID_String_GetWidth(0, text->string, 0);
    if (x < 0) {
        x = 0;
    }
    return AddTextPrinterParameterizedWithColor(window, 0, text->string, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
}

int PlazaGameText_PrintMessage(PlazaGameText *text, int msgNo, Window *window, String *string, int textSpeed) {
    ReadMsgDataIntoString(text->msgData, msgNo, text->buffer);
    StringExpandPlaceholders(text->msgFormat, string, text->buffer);
    return AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
}

int PlazaGameText_PrintWithColor(PlazaGameText *text, int msgNo, Window *window, u8 x, u8 y, u32 color) {
    ReadMsgDataIntoString(text->msgData, msgNo, text->buffer);
    StringExpandPlaceholders(text->msgFormat, text->string, text->buffer);
    return AddTextPrinterParameterizedWithColor(window, 0, text->string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void PlazaGameMessageBox_Init(PlazaGameMessageBox *messageBox, PlazaGameBg *bg, SaveData *saveData, u32 colorVariant, enum HeapID heapId) {
    int frame = Options_GetFrame(Save_PlayerData_GetOptionsAddr(saveData));
    Window *window;
    int i;
    TextFlags_BeginAutoScroll(1);
    LoadUserFrameGfx2(bg->bgConfig, GF_BG_LYR_MAIN_2, 1, 13, frame, heapId);
    LoadUserFrameGfx2(bg->bgConfig, GF_BG_LYR_SUB_0, 1, 13, frame, heapId);
    window = messageBox->windows;
    i = 0;
    do {
        int bgId;
        u8 y;
        if (i == 0) {
            bgId = GF_BG_LYR_MAIN_2;
            y = 19;
        } else {
            bgId = GF_BG_LYR_SUB_0;
            y = 1;
        }
        AddWindowParameterized(bg->bgConfig, window, bgId, 2, y, 27, 4, 12, 31);
        FillWindowPixelBuffer(window, 0);
        messageBox->strings[i] = String_New(128, heapId);
        window++;
        i++;
    } while (i < 2);
    messageBox->colorVariant = colorVariant;
}

void PlazaGameMessageBox_Free(PlazaGameMessageBox *messageBox) {
    int i = 0;
    do {
        PlazaGameMessageBox_Hide(messageBox, i);
        String_Delete(messageBox->strings[i]);
        RemoveWindow(&messageBox->windows[i]);
        i++;
    } while (i < 2);
    TextFlags_EndAutoScroll();
}

void PlazaGameMessageBox_Show(PlazaGameMessageBox *messageBox, PlazaGameText *text, int msgNo, int index) {
    u32 i;
    u16 printerId;
    Window *window;

    if (messageBox->colorVariant == 1) {
        for (i = 0; i < 20; i++) {
            if (msgNo == ov90_0225C34C[i][0]) {
                msgNo = ov90_0225C34C[i][1];
            }
        }
    }
    printerId = messageBox->printerIds[index];
    if (TextPrinterCheckActive((u8)printerId)) {
        RemoveTextPrinter((u8)messageBox->printerIds[index]);
    }
    PlazaGameMessageBox_HideWaitingIcon(messageBox, index);
    window = &messageBox->windows[index];
    FillWindowPixelBuffer(window, 15);
    messageBox->printerIds[index] = PlazaGameText_PrintMessage(text, msgNo, window, messageBox->strings[index], 1);
    DrawFrameAndWindow2(window, TRUE, 1, 13);
    ScheduleWindowCopyToVram(window);
}

void PlazaGameMessageBox_Hide(PlazaGameMessageBox *messageBox, int index) {
    PlazaGameMessageBox_HideWaitingIcon(messageBox, index);
    if (TextPrinterCheckActive(messageBox->printerIds[index])) {
        RemoveTextPrinter(messageBox->printerIds[index]);
    }
    ClearFrameAndWindow2(&messageBox->windows[index], TRUE);
    ClearWindowTilemapAndScheduleTransfer(&messageBox->windows[index]);
}

BOOL PlazaGameMessageBox_IsPrinterFinished(PlazaGameMessageBox *messageBox, int index) {
    if (!TextPrinterCheckActive(messageBox->printerIds[index])) {
        return TRUE;
    }
    return FALSE;
}

void PlazaGameMessageBox_ShowWaitingIcon(PlazaGameMessageBox *messageBox, int index) {
    WaitingIcon **waitingIcon = &messageBox->waitingIcons[index];
    if (*waitingIcon == NULL) {
        *waitingIcon = WaitingIcon_New(&messageBox->windows[index], 1);
    }
}

void PlazaGameMessageBox_HideWaitingIcon(PlazaGameMessageBox *messageBox, int index) {
    if (messageBox->waitingIcons[index] != NULL) {
        // render_window.h declares this without its parameter.
        ((void (*)(WaitingIcon *))WaitingIcon_DeleteWithoutRestore)(messageBox->waitingIcons[index]);
        messageBox->waitingIcons[index] = NULL;
    }
}

#pragma force_active on
#pragma opt_lifetimes off

PlazaGameEntry *PlazaGameEntry_New(const PlazaGameRoster *roster, u16 game, enum HeapID heapId) {
    PlazaGameEntry *entry;
    Sound_SetSceneAndPlayBGM(19, SEQ_GS_WIFIGAME, 0);
    entry = Heap_Alloc(heapId, sizeof(PlazaGameEntry));
    memset(entry, 0, sizeof(PlazaGameEntry));
    memcpy(&entry->roster, roster, sizeof(PlazaGameRoster));
    entry->game = game;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK);
    reg_GXS_DB_DISPCNT &= ~(REG_GXS_DB_DISPCNT_W0_MASK | REG_GXS_DB_DISPCNT_W1_MASK | REG_GXS_DB_DISPCNT_OW_MASK);
    GfGfx_SetBanks(&ov90_0225C2CC);
    entry->heapId = heapId;
    PlazaGameEntry_InitCommands(entry);
    PlazaGamePlayerInfo_Init(&entry->info, &entry->roster);
    PlazaGameBg_Init(&entry->bg, &ov90_0225C2A4, ov90_0225C39C, 5, heapId);
    PlazaGameSprites_Init(&entry->sprites, 16, 1, 1, heapId);
    PlazaGameGfx_Init(&entry->gfx, 1, heapId);
    PlazaGameText_Init(&entry->text, heapId);
    PlazaGameEntry_LoadGraphics(entry, heapId);
    PlazaGameMessageBox_Init(&entry->messageBox, &entry->bg, entry->roster.saveData, ov90_022588A4(&entry->roster, PlazaGameRoster_GetNetId(&entry->roster, entry->roster.localIndex)), heapId);
    entry->task = SysTask_CreateOnMainQueue(PlazaGameEntry_MainTask, entry, 0);
    entry->vblankTask = SysTask_CreateOnVWaitQueue(PlazaGameEntry_VBlankTask, entry, 0);
    if (entry->roster.unkA) {
        WifiConnection_StartVoiceChat(heapId);
    }
    {
        GameStats *gameStats = Save_GameStats_Get(entry->roster.saveData);
        if (entry->roster.launchedFromField) {
            GameStats_AddScore(gameStats, 39);
        }
    }
    return entry;
}

#pragma opt_lifetimes reset
#pragma force_active reset

void PlazaGameEntry_LoadGraphics(PlazaGameEntry *entry, enum HeapID heapId) {
    NARC *narc = NARC_New(NARC_a_1_9_8, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, entry->bg.bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapId);
    entry->screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, FALSE, &entry->screen, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, entry->bg.bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, heapId);
    PlazaGame_LoadGamePalette(narc, entry->game, heapId);
    entry->gfxRes = PlazaGameGfx_Load(&entry->gfx, narc, 11, 5, 12, 13, 14, 5000, heapId);
    PlazaGameTitle_Init(&entry->title, &entry->bg, &entry->text, entry->game, narc, heapId);
    NARC_Delete(narc);
}

void PlazaGameEntry_FreeGraphics(PlazaGameEntry *entry) {
    Heap_Free(entry->screenRaw);
}

#pragma force_active on

void PlazaGameEntry_MainTask(SysTask *task, void *data) {
    PlazaGameEntry *entry = data;
    u32 selection;
    int i;
    int showCountries;
    switch (entry->state) {
    case 0:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xFFFF, 6, 1, (enum HeapID)entry->heapId);
        PlazaGameTitle_StartSlideIn(&entry->title, &entry->bg, (enum HeapID)entry->heapId);
        entry->state++;
        break;
    case 1:
        PlazaGameTitle_UpdateSlideIn(&entry->title, &entry->bg);
        if (IsPaletteFadeFinished() == TRUE) {
            entry->state++;
        }
        break;
    case 2:
        if (PlazaGameTitle_UpdateSlideIn(&entry->title, &entry->bg) == TRUE) {
            entry->state++;
        }
        break;
    case 3:
        PlazaGameMessageBox_Show(&entry->messageBox, &entry->text, msg_0236_00000, 0);
        entry->state++;
        break;
    case 4:
        if (PlazaGameMessageBox_IsPrinterFinished(&entry->messageBox, 0)) {
            entry->state++;
            entry->timer = 64;
        }
        break;
    case 5:
        if (entry->timer != 0) {
            entry->timer--;
            if (entry->timer == 0) {
                PlazaGameMessageBox_Hide(&entry->messageBox, 0);
                PlazaGameTitle_StartSlideOut(&entry->title, &entry->bg);
                entry->state++;
            }
        }
        break;
    case 6:
        if (PlazaGameTitle_UpdateSlideOut(&entry->title, &entry->bg) == TRUE) {
            entry->state++;
        }
        break;
    case 7:
        PlazaGameLineScroll_Init(&entry->lineScroll, &entry->bg, (enum HeapID)entry->heapId);
        showCountries = PlazaGamePlayerInfo_HasDifferentCountries(&entry->roster, entry->info.countries);
        for (i = 0; i < entry->roster.count; i++) {
            PlazaGamePlayer_Init(&entry->players[i], entry->roster.count, entry->game, entry->screen, &entry->bg, i, entry->roster.localIndex, &entry->sprites, entry->gfxRes, (enum HeapID)entry->heapId);
            PlazaGamePlayer_PrintName(&entry->players[i], &entry->text, entry->info.profiles[i], 8, 0, ov90_022588A4(&entry->roster, PlazaGameRoster_GetNetId(&entry->roster, i)));
            PlazaGamePlayer_PrintLocation(&entry->players[i], &entry->text, showCountries, entry->info.countries[i], entry->info.regions[i], 8);
            PlazaGamePlayer_StartSlideIn(&entry->players[i], -8, i * 3);
        }
        for (i = 0; i < entry->roster.count; i++) {
            PlazaGamePlayer_ScheduleTransfer(&entry->players[i], &entry->bg);
            PlazaGamePlayer_ShowSprite(&entry->players[i]);
        }
        entry->timer = 92;
        entry->state++;
        break;
    case 8:
        if (PlazaGameEntry_UpdatePlayers(entry) == TRUE) {
            entry->state++;
        }
        break;
    case 9:
        entry->timer--;
        if (entry->timer == 0) {
            entry->state++;
            LinkSync_StartSync(0x81);
            if (entry->roster.unkA) {
                WifiConnection_StopVoiceChat();
            }
        }
        break;
    case 10:
        if (LinkSync_IsSynced(0x81)) {
            entry->state++;
        }
        break;
    case 11:
        for (i = 0; i < entry->roster.count; i++) {
            PlazaGamePlayer_StartSlideOut(&entry->players[i], i * 3);
        }
        entry->state++;
        entry->timer = 8;
        break;
    case 12:
        entry->timer--;
        PlazaGameEntry_UpdatePlayers(entry);
        if (entry->timer == 0) {
            entry->state++;
        }
        break;
    case 13:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, (enum HeapID)entry->heapId);
        PlazaGameEntry_UpdatePlayers(entry);
        if (entry->roster.localIndex == 0) {
            switch (entry->game) {
            case 0:
                selection = PlazaGame_PickWeighted(ov90_0225C1E8, 3);
                break;
            case 1:
                selection = PlazaGame_PickWeighted(_0225C1E0, 2);
                break;
            case 2:
                selection = PlazaGame_PickWeighted(ov90_0225C1E4, 3);
                break;
            }
            Comm_SendCommand(26, &selection, sizeof(selection));
        }
        entry->state++;
        break;
    case 14:
        PlazaGameEntry_UpdatePlayers(entry);
        if (IsPaletteFadeFinished() == TRUE) {
            PlazaGameLineScroll_Free(&entry->lineScroll);
            entry->state++;
        }
        break;
    case 15:
        if (entry->selectionReceived == 1) {
            entry->state++;
        }
        break;
    case 16:
        break;
    }
    PlazaGameSprites_Update(&entry->sprites);
    ScheduleSetBgPosText(entry->bg.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_ADD_Y, 2);
    ScheduleSetBgPosText(entry->bg.bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_ADD_Y, 2);
}

#pragma force_active reset

void PlazaGameEntry_VBlankTask(SysTask *task, void *data) {
    PlazaGameEntry *entry = data;
    PlazaGameBg_Update(&entry->bg);
    PlazaGameSprites_ApplyOam(&entry->sprites);
    PlazaGameLineScroll_Swap(&entry->lineScroll);
}

u32 PlazaGame_PickWeighted(const u8 *weights, u32 count) {
    u32 random = MTRandom() % 100;
    u32 sum = 0;
    u32 i;

    for (i = 0; i < count; i++) {
        sum += weights[i];
        if (sum > random) {
            return i;
        }
    }
    GF_AssertFail();
    return count - 1;
}

#pragma force_active on

BOOL PlazaGameEntry_UpdatePlayers(PlazaGameEntry *entry) {
    int i;
    BOOL allDone;
    allDone = TRUE;
    i = 0;
    if (i < entry->roster.count) {
        // The sprite check walks its own pointer over the rows, as the original does.
        PlazaGamePlayer *player = entry->players;
        u8 *row = (u8 *)entry;
        PlazaGamePlayer *scrollPlayer = player;
        do {
            if (((PlazaGameEntry *)row)->players[0].sprite != NULL) {
                BOOL done = PlazaGamePlayer_UpdateSlide(player);
                PlazaGameLineScroll_SetPlayer(&entry->lineScroll, scrollPlayer);
                if (!done) {
                    allDone = FALSE;
                }
            } else {
                allDone = FALSE;
            }
            row += sizeof(PlazaGamePlayer);
            player++;
            scrollPlayer++;
            i++;
        } while (i < entry->roster.count);
    }
    return allDone;
}

void PlazaGamePlayer_Init(PlazaGamePlayer *player, int count, int game, NNSG2dScreenData *screen, PlazaGameBg *bg, int index, u32 localIndex, PlazaGameSprites *sprites, PlazaGameGfxRes *gfxRes, enum HeapID heapId) {
    u32 tableOffset;
    int row;
    int anim;
    int y;
    int seat;
    s16 spriteY;

    switch (game) {
    case 0:
    case 2:
        seat = PlazaGame_GetRelativeSeat(count, index, localIndex);
        break;
    case 1:
        if (localIndex > index) {
            seat = index + 1;
        } else if (localIndex == index) {
            seat = 0;
        } else {
            seat = index;
        }
        break;
    default:
        seat = index;
        break;
    }
    SetBgPriority(GF_BG_LYR_MAIN_1, 2);
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    tableOffset = count * 12;
    row = ((const u8 *)ov90_0225C310 + tableOffset)[seat];
    AddWindowParameterized(bg->bgConfig, &player->window, GF_BG_LYR_MAIN_0, 5, row + 1, 26, 4, 12, index * 104 + 513);
    FillWindowPixelBuffer(&player->window, 0);
    CopyToBgTilemapRect(bg->bgConfig, GF_BG_LYR_MAIN_1, 0, row, 32, 6, screen->rawData, 0, ((const u8 *)ov90_0225C314 + tableOffset)[index], screen->screenWidth >> 3, screen->screenHeight >> 3);
    y = row * 8;
    anim = ((const u8 *)ov90_0225C318 + tableOffset)[seat];
    spriteY = ov90_0225C276[anim * 2];
    Sprite_SetAnimCtrlSeq(player->sprite = PlazaGameGfx_CreateSprite(gfxRes, sprites->spriteList, (u8)ov90_0225C274[anim * 2], (u8)(spriteY + y), 0, heapId), anim);
    Sprite_SetPalOffset(player->sprite, index);
    Sprite_SetDrawFlag(player->sprite, FALSE);
    Sprite_SetAnimActiveFlag(player->sprite, TRUE);
    y += 21;
    player->rankSprite = PlazaGameGfx_CreateSprite(gfxRes, sprites->spriteList, 24, (u8)y, 0, heapId);
    Sprite_SetAnimCtrlSeq(player->rankSprite, 0);
    Sprite_SetDrawFlag(player->rankSprite, FALSE);
    player->index = index;
    player->seat = seat;
    player->count = count;
}

#pragma force_active reset

void PlazaGamePlayer_PrintName(PlazaGamePlayer *player, PlazaGameText *text, PlayerProfile *profile, int x, s16 y, int highlight) {
    PlazaGameText_BufferPlayerName(text, profile);
    if (highlight == 1) {
        PlazaGameText_PrintWithColor(text, msg_0236_00010, &player->window, x, y, MAKE_TEXT_COLOR(5, 6, 0));
    } else {
        PlazaGameText_Print(text, msg_0236_00010, &player->window, x, y);
    }
}

void PlazaGamePlayer_PrintLocation(PlazaGamePlayer *player, PlazaGameText *text, int showCountry, int country, int region, s16 x) {
    if (showCountry != 0) {
        if (country != 0) {
            PlazaGameText_ResetBuffers(text);
            PlazaGameText_BufferCountry(text, country);
            PlazaGameText_Print(text, msg_0236_00011, &player->window, x, 16);
        }
    } else {
        if (country != 0) {
            PlazaGameText_ResetBuffers(text);
            PlazaGameText_BufferCity(text, country, region);
            PlazaGameText_Print(text, msg_0236_00012, &player->window, x, 16);
        }
    }
}

void PlazaGamePlayer_ShowRank(PlazaGamePlayer *player, int rank) {
    Sprite_SetDrawFlag(player->rankSprite, TRUE);
    Sprite_SetAnimCtrlSeq(player->rankSprite, rank + 4);
    Sprite_SetAnimActiveFlag(player->rankSprite, TRUE);
}

void PlazaGamePlayer_PrintScore(PlazaGamePlayer *player, PlazaGameText *text, int score, int right, s16 y, int digits) {
    PlazaGameText_BufferNumber(text, score, digits);
    if (digits == 5) {
        PlazaGameText_PrintRightAligned(text, msg_0236_00014, &player->window, right, y);
    } else {
        PlazaGameText_PrintRightAligned(text, msg_0236_00013, &player->window, right, y);
    }
}

void PlazaGamePlayer_ScheduleTransfer(PlazaGamePlayer *player, PlazaGameBg *bg) {
    ScheduleWindowCopyToVram(&player->window);
    ScheduleBgTilemapBufferTransfer(bg->bgConfig, GF_BG_LYR_MAIN_1);
}

void PlazaGamePlayer_ShowSprite(PlazaGamePlayer *player) {
    Sprite_SetDrawFlag(player->sprite, TRUE);
}

void PlazaGamePlayer_Free(PlazaGamePlayer *player) {
    if (player->sprite != NULL) {
        Sprite_Delete(player->sprite);
        player->sprite = NULL;
    }
    if (player->rankSprite != NULL) {
        Sprite_Delete(player->rankSprite);
        player->rankSprite = NULL;
    }
    if (WindowIsInUse(&player->window) == TRUE) {
        RemoveWindow(&player->window);
    }
}

#pragma force_active on

void PlazaGamePlayer_StartSlideIn(PlazaGamePlayer *player, int x, s16 delay) {
    float end;
    if (x > 0) {
        end = 0.5f + (float)(x << FX32_SHIFT);
    } else {
        end = (float)(x << FX32_SHIFT) - 0.5f;
    }
    PlazaGameMotion_Init(&player->motion, -255 * FX32_ONE, (int)end, 28 * FX32_ONE, 8);
    player->frame = 0;
    player->delay = delay;
    player->se = SEQ_SE_PL_MNGM_IN;
    PlazaGamePlayer_SetSpritePos(player, -(player->motion.current >> FX32_SHIFT));
}

#pragma force_active reset

void PlazaGamePlayer_StartSlideOut(PlazaGamePlayer *player, s16 delay) {
    PlazaGameMotion_Init(&player->motion, player->motion.current, -255 * FX32_ONE, 0, 8);
    player->frame = 0;
    player->delay = delay;
    player->se = SEQ_SE_PL_MNGM_OUT;
    PlazaGamePlayer_SetSpritePos(player, -(player->motion.current >> FX32_SHIFT));
}

int PlazaGamePlayer_UpdateSlide(PlazaGamePlayer *player) {
    int done;
    if (player->delay <= 0) {
        if (player->frame == 0) {
            PlaySE(player->se);
        }
        player->frame++;
        done = PlazaGameMotion_Update(&player->motion, player->frame);
        PlazaGamePlayer_SetSpritePos(player, -(player->motion.current >> FX32_SHIFT));
        return done;
    } else {
        player->delay--;
        return 0;
    }
}

#pragma force_active on
#pragma opt_lifetimes off

void PlazaGamePlayer_SetSpritePos(PlazaGamePlayer *player, u32 x) {
    VecFx32 pos;
    const u8(*rows)[12] = ov90_0225C31C;
    u32 table = player->count - 1;
    const u8(*anims)[12] = ov90_0225C324;
    u8 anim = anims[table][player->seat];
    u8 y = ov90_0225C276[anim * 2] + rows[table][player->seat] * 8;
    u8 spriteX = ov90_0225C274[anim * 2];
    pos.x = (spriteX + x) << FX32_SHIFT;
    pos.y = y << FX32_SHIFT;
    Sprite_SetMatrix(player->sprite, &pos);
    x += 24;
    table = player->count - 1;
    y = rows[table][player->seat] * 8 + 21;
    pos.x = x << FX32_SHIFT;
    pos.y = y << FX32_SHIFT;
    Sprite_SetMatrix(player->rankSprite, &pos);
}

#pragma opt_lifetimes reset

int PlazaGame_GetRelativeSeat(u32 count, int index, u32 localIndex) {
    int localSeat;
    int seat;
    u32 i;
    int result;
    for (i = 0; i < count; i++) {
        if (index == ov90_0225C284[count - 1][i]) {
            seat = i;
        }
        if (localIndex == ov90_0225C284[count - 1][i]) {
            localSeat = i;
        }
    }
    result = seat - localSeat;
    if (result < 0) {
        result += count;
    }
    return result;
}

#pragma force_active reset

void PlazaGameLineScroll_Init(PlazaGameLineScroll *lineScroll, PlazaGameBg *bg, enum HeapID heapId) {
    int i;

    GF_ASSERT(lineScroll->buffer == NULL);
    lineScroll->bg = bg;
    BgSetPosTextAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, -255);
    BgSetPosTextAndCommit(lineScroll->bg->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, -255);
    for (i = 0; i < 2; i++) {
        MIi_CpuClear16(0xFF01, lineScroll->buffers[i], sizeof(lineScroll->buffers[i]));
    }
    lineScroll->buffer = DoubleBuffer_New(heapId, lineScroll->buffers[0], lineScroll->buffers[1]);
    DoubleBuffer_SetEnabled(lineScroll->buffer, 0);
    Main_SetHBlankIntrCB(PlazaGameLineScroll_HBlank, lineScroll);
}

void PlazaGameLineScroll_Free(PlazaGameLineScroll *lineScroll) {
    if (lineScroll->buffer != NULL) {
        Main_SetHBlankIntrCB(NULL, NULL);
        DoubleBuffer_Delete(lineScroll->buffer);
        lineScroll->buffer = NULL;
    }
}

#pragma force_active on

void PlazaGameLineScroll_SetPlayer(PlazaGameLineScroll *lineScroll, PlazaGamePlayer *player) {
    u16 *lines;
    int i;
    int stride;
    GF_ASSERT(lineScroll->buffer);
    lines = DoubleBuffer_GetCurrent(lineScroll->buffer);
    stride = 12;
    for (i = 0; i < 48; i++) {
        int offset = i - 1;
        int line = offset + ov90_0225C31C[player->count - 1][player->seat] * 8;
        if (line < 0) {
            line += 192;
        }
        lines[line] = player->motion.current >> FX32_SHIFT;
    }
    DoubleBuffer_SetEnabled(lineScroll->buffer, 1);
}

#pragma force_active reset

void PlazaGameLineScroll_Swap(PlazaGameLineScroll *lineScroll) {
    struct DoubleBuffer *buffer = lineScroll->buffer;
    if (buffer != NULL) {
        DoubleBuffer_Swap(buffer);
        DoubleBuffer_SetEnabled(lineScroll->buffer, 0);
    }
}

void PlazaGameLineScroll_HBlank(void *data) {
    PlazaGameLineScroll *lineScroll = data;
    s16 *values;
    s32 line = reg_GX_VCOUNT;

    if (line < 192) {
        values = DoubleBuffer_GetOther(lineScroll->buffer);
        if ((reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) != 0) {
            BgSetPosTextAndCommit(lineScroll->bg->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, values[line]);
            BgSetPosTextAndCommit(lineScroll->bg->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, values[line]);
        }
    }
}

#pragma force_active on

void PlazaGamePlayerInfo_Init(PlazaGamePlayerInfo *info, const PlazaGameRoster *roster) {
    int i;
    for (i = 0; i < roster->count; i++) {
        info->profiles[i] = PlazaGameRoster_GetProfile(roster, roster->netIds[i]);
        GF_ASSERT(info->profiles[i]);
        info->countries[i] = CommPlayerInfo_GetCountry(roster->netIds[i]);
        info->regions[i] = CommPlayerInfo_GetRegion(roster->netIds[i]);
    }
}

int PlazaGamePlayerInfo_HasDifferentCountries(const PlazaGameRoster *roster, const u8 *countries) {
    int i;
    int result;
    result = 0;
    i = 0;
    // The count is reloaded every iteration, as the original does.
    for (; i < *(const volatile u8 *)&roster->count - 1; i++) {
        int next = countries[i + 1];
        int current = countries[i];
        if (current == next && current != 0 && next != 0) {
        } else {
            result = 1;
        }
    }
    return result;
}

#pragma force_active reset

u32 PlazaGame_GetPoweredUpTapToy(u32 tapToy) {
    u32 level = tapToy % 3;
    u32 kind = tapToy / 3;
    if (level + 1 < 3) {
        level++;
    }
    kind += kind * 2;
    kind += level;
    return kind;
}

void PlazaGameTitle_Init(PlazaGameTitle *title, PlazaGameBg *bg, PlazaGameText *text, int game, NARC *narc, enum HeapID heapId) {
    AddWindowParameterized(bg->bgConfig, &title->window, GF_BG_LYR_MAIN_1, 6, 11, 20, 10, 8, 513);
    FillWindowPixelBuffer(&title->window, 0);
    title->string = String_New(128, heapId);
    PlazaGameText_BufferActivityName(text, game);
    PlazaGameText_Format(text, title->string, msg_0236_00016);
    title->game = game;
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 7, bg->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 10, bg->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);
}

void PlazaGameTitle_Free(PlazaGameTitle *title) {
    RemoveWindow(&title->window);
    String_Delete(title->string);
}

void PlazaGameTitle_StartSlideIn(PlazaGameTitle *title, PlazaGameBg *bg, enum HeapID heapId) {
    u32 width;
    title->frame = 0;
    title->state = 0;
    PlazaGameMotion_Init(&title->motion, ov90_0225C258[title->game], ov90_0225C228[title->game], ov90_0225C21C[title->game], 16);
    PlazaGameTitle_SetScroll(title, bg, title->motion.current >> FX32_SHIFT);
    FontID_Alloc(2, heapId);
    width = FontID_String_GetWidth(2, title->string, 0);
    AddTextPrinterParameterizedWithColor(&title->window, 2, title->string, 80 - (width >> 1), 0, TEXT_SPEED_INSTANT, ov90_0225C210[title->game], NULL);
    FontID_Release(2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
}

void PlazaGameTitle_StartSlideOut(PlazaGameTitle *title, PlazaGameBg *bg) {
    title->frame = 0;
    title->state = 0;
    PlazaGameMotion_Init(&title->motion, ov90_0225C228[title->game], ov90_0225C1F8[title->game], ov90_0225C204[title->game], 8);
    PlazaGameTitle_ScheduleScroll(title, bg, title->motion.current >> FX32_SHIFT);
}

BOOL PlazaGameTitle_UpdateSlideIn(PlazaGameTitle *title, PlazaGameBg *bg) {
    int done;

    switch (title->state) {
    case 0:
        done = PlazaGameMotion_Update(&title->motion, title->frame);
        PlazaGameTitle_ScheduleScroll(title, bg, title->motion.current >> FX32_SHIFT);
        title->frame++;
        if (done == 1) {
            PlaySE(SEQ_SE_PL_FCALL);
            PlazaGameMotion_Init(&title->motion, ov90_0225C228[title->game], ov90_0225C234[title->game], ov90_0225C240[title->game], 4);
            title->state++;
            title->frame = 0;
        }
        break;
    case 1:
        done = PlazaGameMotion_Update(&title->motion, title->frame);
        PlazaGameTitle_ScheduleScroll(title, bg, title->motion.current >> FX32_SHIFT);
        title->frame++;
        if (done == 1) {
            PlazaGameMotion_Init(&title->motion, ov90_0225C234[title->game], ov90_0225C228[title->game], ov90_0225C24C[title->game], 4);
            title->state++;
            title->frame = 0;
        }
        break;
    case 2:
        done = PlazaGameMotion_Update(&title->motion, title->frame);
        PlazaGameTitle_ScheduleScroll(title, bg, title->motion.current >> FX32_SHIFT);
        title->frame++;
        if (done == 1) {
            title->state++;
        }
        break;
    case 3:
        return TRUE;
    }
    return FALSE;
}

BOOL PlazaGameTitle_UpdateSlideOut(PlazaGameTitle *title, PlazaGameBg *bg) {
    int done;
    switch (title->state) {
    case 0:
        done = PlazaGameMotion_Update(&title->motion, title->frame);
        PlazaGameTitle_ScheduleScroll(title, bg, title->motion.current >> FX32_SHIFT);
        title->frame++;
        if (done == 1) {
            title->state++;
        }
        break;
    case 1:
        ClearWindowTilemapAndCopyToVram(&title->window);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        PlazaGameTitle_SetScroll(title, bg, 0);
        BgClearTilemapBufferAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_0);
        BgClearTilemapBufferAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        return TRUE;
    }
    return FALSE;
}

void PlazaGameTitle_SetScroll(PlazaGameTitle *title, PlazaGameBg *bg, int value) {
    switch (title->game) {
    case 0:
    case 1:
        BgSetPosTextAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, value);
        BgSetPosTextAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, value);
        break;
    case 2:
        BgSetPosTextAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, value);
        BgSetPosTextAndCommit(bg->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, value);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void PlazaGameTitle_ScheduleScroll(PlazaGameTitle *title, PlazaGameBg *bg, int value) {
    switch (title->game) {
    case 0:
    case 1:
        ScheduleSetBgPosText(bg->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, value);
        ScheduleSetBgPosText(bg->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, value);
        break;
    case 2:
        ScheduleSetBgPosText(bg->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, value);
        ScheduleSetBgPosText(bg->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, value);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void PlazaGame_LoadGamePalette(NARC *narc, int game, enum HeapID heapId) {
    int offset;
    int palette;
    switch (game) {
    case 0:
        palette = 1;
        break;
    case 1:
        palette = 2;
        break;
    case 2:
        palette = 0;
        break;
    default:
        palette = 1;
        break;
    }
    offset = palette * 0x20;
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(narc, 5, GF_PAL_LOCATION_MAIN_BG, offset, GF_PAL_SLOT_5_OFFSET, 0x20, heapId);
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(narc, 5, GF_PAL_LOCATION_SUB_BG, offset, GF_PAL_SLOT_5_OFFSET, 0x20, heapId);
}

typedef struct PlazaGameRosterBytes {
    u8 bytes[sizeof(PlazaGameRoster)];
} PlazaGameRosterBytes;

typedef struct PlazaGameResultDataBytes {
    u8 bytes[sizeof(PlazaGameResultData)];
} PlazaGameResultDataBytes;

#pragma force_active on

PlazaGameResults *PlazaGameResults_New(const PlazaGameRoster *roster, const PlazaGameResultData *data, u8 game, enum HeapID heapId) {
    PlazaGameResults *results = Heap_Alloc(heapId, sizeof(PlazaGameResults));
    memset(results, 0, sizeof(PlazaGameResults));
    // Both are copied byte by byte.
    *(PlazaGameResultDataBytes *)&results->data = *(const PlazaGameResultDataBytes *)data;
    *(PlazaGameRosterBytes *)&results->roster = *(const PlazaGameRosterBytes *)roster;
    results->game = game;
    PlazaGameResults_InitCommands(results);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK);
    reg_GXS_DB_DISPCNT &= ~(REG_GXS_DB_DISPCNT_W0_MASK | REG_GXS_DB_DISPCNT_W1_MASK | REG_GXS_DB_DISPCNT_OW_MASK);
    GfGfx_SetBanks(&ov90_0225C2F4);
    results->heapId = heapId;
    PlazaGamePlayerInfo_Init(&results->info, &results->roster);
    GF_CreateVramTransferManager(16, heapId);
    PlazaGameBg_Init(&results->bg, &ov90_0225C264, ov90_0225C43C, 6, heapId);
    PlazaGameSprites_Init(&results->sprites, 32, 2, 2, heapId);
    PlazaGameGfx_Init(&results->gfx, 2, heapId);
    PlazaGameText_Init(&results->text, heapId);
    PlazaGameResults_LoadGraphics(results, heapId);
    PlazaGameMessageBox_Init(&results->messageBox, &results->bg, results->roster.saveData, ov90_022588A4(&results->roster, PlazaGameRoster_GetNetId(&results->roster, results->roster.localIndex)), heapId);
    PlazaGameContinue_Init(&results->continuePrompt, &results->bg, results->roster.unkA, heapId);
    results->showCountries = PlazaGamePlayerInfo_HasDifferentCountries(&results->roster, results->info.countries);
    switch (results->game) {
    case 0:
    case 1:
        results->task = SysTask_CreateOnMainQueue(PlazaGameResults_RankingTask, results, 0);
        break;
    case 2:
        results->task = SysTask_CreateOnMainQueue(PlazaGameResults_TeamTask, results, 0);
        break;
    default:
        GF_ASSERT(FALSE);
        results->task = SysTask_CreateOnMainQueue(PlazaGameResults_RankingTask, results, 0);
        break;
    }
    results->vblankTask = SysTask_CreateOnVWaitQueue(PlazaGameResults_VBlankTask, results, 0);
    if (results->roster.unkA) {
        WifiConnection_StartVoiceChat(heapId);
    }
    return results;
}

#pragma force_active reset

void PlazaGameResults_LoadGraphics(PlazaGameResults *results, enum HeapID heapId) {
    NARC *narc = NARC_New(NARC_a_1_9_8, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x120, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x120, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, results->bg.bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, results->bg.bgConfig, GF_BG_LYR_SUB_1, 0, 0x4000, FALSE, heapId);
    results->screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, FALSE, &results->screen, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, results->bg.bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, results->bg.bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, heapId);
    PlazaGame_LoadGamePalette(narc, results->game, heapId);
    results->gfxRes = PlazaGameGfx_Load(&results->gfx, narc, 11, 5, 12, 13, 14, 5000, heapId);
    PlazaGameResults_LoadFlashPalette(results, narc, heapId);
    PlazaGameBalloons_Init(&results->balloons, &results->bg, &results->sprites, &results->gfx, narc, heapId);
    NARC_Delete(narc);
}

void PlazaGameResults_FreeGraphics(PlazaGameResults *results) {
    PlazaGameBalloons_Free(&results->balloons);
    Heap_Free(results->screenRaw);
    PlazaGameResults_FreeFlashPalette(results);
}

void PlazaGameResults_RankingTask(SysTask *task, void *data) {
    PlazaGameResults *results = data;

    switch (results->state) {
    case 0: {
        int i;
        int threshold;
        results->prevTapToy = results->roster.args->tapToy;
        if (results->roster.launchedFromField == 1) {
            results->tapToyPoweredUp = FALSE;
        } else {
            threshold = PlazaGameResults_GetScoreThreshold(results);
            i = results->roster.localIndex;
            if (results->data.ranks[i] == 0 && results->data.scores[i] > (u32)threshold) {
                results->roster.args->tapToy = PlazaGame_GetPoweredUpTapToy(results->roster.args->tapToy);
                results->tapToyPoweredUp = TRUE;
            } else {
                results->tapToyPoweredUp = FALSE;
            }
            if (results->roster.localIndex == 0) {
                PlazaGameResults_ReportWinners(results);
            }
        }
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xFFFF, 6, 1, (enum HeapID)results->heapId);
        results->state++;
        break;
    }
    case 1:
        if (IsPaletteFadeFinished()) {
            results->state++;
        }
        break;
    case 2:
        PlazaGameMessageBox_Show(&results->messageBox, &results->text, msg_0236_00001, 0);
        results->state++;
        break;
    case 3:
        if (PlazaGameMessageBox_IsPrinterFinished(&results->messageBox, 0)) {
            results->timer = 64;
            results->state++;
        }
        break;
    case 4: {
        int i;
        u32 threshold;
        BOOL noWinner;
        if (results->timer == 0) {
            break;
        }
        results->timer--;
        if (results->timer != 0) {
            break;
        }
        noWinner = TRUE;
        PlazaGameMessageBox_Hide(&results->messageBox, 0);
        PlazaGameLineScroll_Init(&results->lineScroll, &results->bg, (enum HeapID)results->heapId);
        threshold = PlazaGameResults_GetScoreThreshold(results);
        for (i = 0; i < results->roster.count; i++) {
            if (results->data.scores[i] > threshold) {
                noWinner = FALSE;
            }
        }
        if (noWinner == TRUE) {
            results->delay = 16;
            results->state = 22;
        } else {
            results->state++;
            results->rankStep = 4;
        }
        break;
    }
    case 5: {
        int i;
        BOOL found = FALSE;
        do {
            results->rankStep--;
            for (i = 0; i < results->roster.count; i++) {
                if (results->rankStep == results->data.ranks[i]) {
                    found = TRUE;
                }
            }
        } while (found == FALSE);
        switch (results->rankStep) {
        case 2:
        case 3:
            results->delay = 16;
            break;
        case 0:
        case 1:
            results->delay = 32;
            break;
        default:
            GF_AssertFail();
            break;
        }
        PlazaGameResults_UpdatePlayers(results);
        results->state++;
        break;
    }
    case 6: {
        int i;
        BOOL show;
        results->delay--;
        PlazaGameResults_UpdatePlayers(results);
        if (results->delay == 0) {
            for (i = 0; i < results->roster.count; i++) {
                show = FALSE;
                if (results->rankStep == results->data.ranks[i]) {
                    show = TRUE;
                } else if (results->rankStep == 1 && results->data.ranks[i] == 0) {
                    show = TRUE;
                }
                if (show) {
                    PlazaGameResults_ShowPlayer(results, i, results->data.ranks[i]);
                }
            }
            if (results->rankStep > 1) {
                results->state = 5;
            } else {
                results->state++;
            }
        }
        break;
    }
    case 7:
        if (PlazaGameResults_UpdatePlayers(results)) {
            results->state++;
        }
        break;
    case 8:
        results->state++;
        PlazaGameResults_StartFlash(results);
        results->timer = 30;
        break;
    case 9:
        if (results->timer != 0) {
            results->timer--;
        }
        if (results->timer == 0) {
            results->state = 10;
            results->timer = 0;
        }
        break;
    case 10: {
        int i;
        int msgNo;
        i = results->roster.localIndex;
        if (results->data.ranks[i] == 0) {
            msgNo = msg_0236_00017;
        } else {
            msgNo = msg_0236_00018;
        }
        PlazaGameText_BufferPlayerName(&results->text, results->info.profiles[i]);
        PlazaGameMessageBox_Show(&results->messageBox, &results->text, msgNo, 0);
        results->state++;
        break;
    }
    case 11:
        if (PlazaGameMessageBox_IsPrinterFinished(&results->messageBox, 0) && PlazaGameResults_IsFanfareDone(results) == TRUE) {
            if (results->roster.launchedFromField == 1) {
                results->timer = 102;
                results->state = 14;
            } else {
                results->timer = 102;
                results->state++;
            }
        }
        break;
    case 12: {
        int i;
        int msgNo;
        if (results->timer != 0) {
            results->timer--;
            break;
        }
        if (results->tapToyPoweredUp != 0) {
            PlazaGameText_BufferTapToyName(&results->text, results->roster.args->tapToy);
            if (results->prevTapToy == results->roster.args->tapToy) {
                msgNo = msg_0236_00019;
            } else {
                msgNo = msg_0236_00002;
            }
        } else {
            msgNo = msg_0236_00009;
        }
        i = results->roster.localIndex;
        PlazaGameText_BufferPlayerName(&results->text, results->info.profiles[i]);
        PlazaGameMessageBox_Show(&results->messageBox, &results->text, msgNo, 0);
        results->state = 13;
        break;
    }
    case 13:
        if (PlazaGameMessageBox_IsPrinterFinished(&results->messageBox, 0)) {
            results->timer = 102;
            results->state++;
        }
        break;
    case 14:
        if (results->timer == 0) {
            break;
        }
        results->timer--;
        if (results->timer != 0) {
            break;
        }
        PlazaGameMessageBox_Hide(&results->messageBox, 0);
        results->timer = 30;
        results->state++;
        break;
    case 15:
        if (results->timer != 0) {
            results->timer--;
            break;
        }
        LinkSync_StartSync(0x82);
        if (results->roster.unkA != 0) {
            WifiConnection_StopVoiceChat();
        }
        results->state++;
        break;
    case 16:
        if (LinkSync_IsSynced(0x82)) {
            if (results->roster.launchedFromField != 0) {
                results->state = 19;
            } else {
                results->state++;
            }
        }
        break;
    case 17:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, (enum HeapID)results->heapId);
        results->state++;
        break;
    case 18:
        if (IsPaletteFadeFinished()) {
            results->state = 21;
        }
        break;
    case 19:
        PlazaGameResults_RecordFriendPlays(results);
        results->state++;
        break;
    case 20:
        if (PlazaGameContinue_Run(&results->continuePrompt, &results->messageBox, &results->text, results->game, (enum HeapID)results->heapId)) {
            results->continueResult = PlazaGameContinue_GetResult(&results->continuePrompt);
            results->state++;
        }
        break;
    case 21:
        PlazaGameLineScroll_Free(&results->lineScroll);
        results->done = TRUE;
        break;
    case 22:
        results->delay--;
        if (results->delay == 0) {
            results->state++;
        }
        break;
    case 23: {
        int i;
        for (i = 0; i < results->roster.count; i++) {
            results->data.ranks[i] = results->roster.count - 1;
            PlazaGameResults_ShowPlayer(results, i, results->data.ranks[i]);
        }
        results->state = 7;
        break;
    }
    default:
        break;
    }
    PlazaGameResults_UpdateFlash(results);
    PlazaGameSprites_Update(&results->sprites);
    ScheduleSetBgPosText(results->bg.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_ADD_Y, 2);
    ScheduleSetBgPosText(results->bg.bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_ADD_Y, 2);
}

#pragma force_active on

void PlazaGameResults_TeamTask(SysTask *task, void *data) {
    PlazaGameResults *results = data;
    int i;
    switch (results->state) {
    case 0:
        if (results->roster.launchedFromField == 1) {
            results->tapToyPoweredUp = FALSE;
        } else {
            if (results->data.total >= 10) {
                results->roster.args->tapToy = PlazaGame_GetPoweredUpTapToy(results->roster.args->tapToy);
                results->tapToyPoweredUp = TRUE;
            } else {
                results->tapToyPoweredUp = FALSE;
            }
            if (results->roster.localIndex == 0) {
                PlazaGameResults_ReportTeamWin(results);
            }
        }
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0xFFFF, 6, 1, (enum HeapID)results->heapId);
        results->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            results->state++;
            results->rankStep = 4;
        }
        break;
    case 2:
        PlazaGameMessageBox_Show(&results->messageBox, &results->text, msg_0236_00001, 0);
        results->state++;
        break;
    case 3:
        if (PlazaGameMessageBox_IsPrinterFinished(&results->messageBox, 0)) {
            results->timer = 102;
            results->state++;
        }
        break;
    case 4:
        if (results->timer != 0) {
            results->timer--;
            if (results->timer == 0) {
                PlazaGameMessageBox_Hide(&results->messageBox, 0);
                results->state++;
            }
        }
        break;
    case 5:
        PlazaGameLineScroll_Init(&results->lineScroll, &results->bg, (enum HeapID)results->heapId);
        for (i = 0; i < results->roster.count; i++) {
            PlazaGamePlayer_Init(&results->players[i], results->roster.count, results->game, results->screen, &results->bg, i, results->roster.localIndex, &results->sprites, results->gfxRes, (enum HeapID)results->heapId);
            PlazaGamePlayer_PrintName(&results->players[i], &results->text, results->info.profiles[i], 8, 0, ov90_022588A4(&results->roster, PlazaGameRoster_GetNetId(&results->roster, i)));
            PlazaGamePlayer_PrintLocation(&results->players[i], &results->text, results->showCountries, results->info.countries[i], results->info.regions[i], 8);
            PlazaGamePlayer_ScheduleTransfer(&results->players[i], &results->bg);
            PlazaGamePlayer_ShowSprite(&results->players[i]);
            PlazaGamePlayer_StartSlideIn(&results->players[i], -8, i * 3);
        }
        results->state++;
        results->timer = 16;
        break;
    case 6:
        if (PlazaGameResults_UpdatePlayers(results) == TRUE) {
            results->timer--;
            if (results->timer == 0) {
                PlazaGameBalloons_Start(&results->balloons, results->data.total);
                results->state++;
            }
        }
        break;
    case 7:
        if (PlazaGameBalloons_Update(&results->balloons, &results->bg) == TRUE) {
            if (results->tapToyPoweredUp == TRUE) {
                PlazaGameResults_PlayFanfare(results);
            }
            results->state++;
        }
        break;
    case 8:
        PlazaGameText_BufferTwoDigitNumber(&results->text, results->data.total);
        PlazaGameMessageBox_Show(&results->messageBox, &results->text, msg_0236_00007, 0);
        results->timer = 102;
        results->state++;
        break;
    case 9:
        if (PlazaGameMessageBox_IsPrinterFinished(&results->messageBox, 0) == TRUE) {
            if (results->timer == 0) {
                if (PlazaGameResults_IsFanfareDone(results) == TRUE) {
                    if (results->roster.launchedFromField == 1) {
                        results->timer = 102;
                        results->state = 12;
                    } else {
                        results->state++;
                    }
                }
            } else {
                results->timer--;
            }
        }
        break;
    case 10:
        PlazaGameMessageBox_Show(&results->messageBox, &results->text, results->tapToyPoweredUp ? msg_0236_00008 : msg_0236_00009, 0);
        results->state++;
        break;
    case 11:
        if (PlazaGameMessageBox_IsPrinterFinished(&results->messageBox, 0)) {
            results->timer = 102;
            if (results->tapToyPoweredUp) {
                results->state = 13;
            } else {
                results->state++;
            }
        }
        break;
    case 12:
        if (results->timer != 0) {
            results->timer--;
            if (results->timer == 0) {
                results->state = 14;
            }
        }
        break;
    case 13:
        if (results->timer != 0) {
            results->timer--;
        }
        if (results->timer == 0) {
            results->state = 14;
        }
        break;
    case 14:
        if (results->roster.unkA) {
            WifiConnection_StopVoiceChat();
        }
        LinkSync_StartSync(0x82);
        results->state++;
        break;
    case 15:
        if (LinkSync_IsSynced(0x82)) {
            if (results->roster.launchedFromField) {
                results->state = 18;
            } else {
                results->state++;
            }
        }
        break;
    case 16:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, (enum HeapID)results->heapId);
        results->state++;
        break;
    case 17:
        if (IsPaletteFadeFinished()) {
            results->state = 20;
        }
        break;
    case 18:
        PlazaGameResults_RecordFriendPlays(results);
        results->state++;
        break;
    case 19:
        if (PlazaGameContinue_Run(&results->continuePrompt, &results->messageBox, &results->text, results->game, (enum HeapID)results->heapId)) {
            results->continueResult = PlazaGameContinue_GetResult(&results->continuePrompt);
            results->state++;
        }
        break;
    case 20:
        PlazaGameLineScroll_Free(&results->lineScroll);
        results->done = TRUE;
        break;
    }
    PlazaGameResults_UpdateFlash(results);
    PlazaGameSprites_Update(&results->sprites);
    ScheduleSetBgPosText(results->bg.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_ADD_Y, 2);
    ScheduleSetBgPosText(results->bg.bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_ADD_Y, 2);
}

#pragma force_active reset

void PlazaGameResults_VBlankTask(SysTask *task, void *data) {
    PlazaGameResults *results = data;
    GF_RunVramTransferTasks();
    PlazaGameBg_Update(&results->bg);
    PlazaGameSprites_ApplyOam(&results->sprites);
    PlazaGameLineScroll_Swap(&results->lineScroll);
}

void PlazaGameResults_LoadFlashPalette(PlazaGameResults *results, NARC *narc, enum HeapID heapId) {
    results->flashPlttRaw = GfGfxLoader_GetPlttDataFromOpenNarc(narc, 5, &results->flashPltt, heapId);
}

void PlazaGameResults_StartFlash(PlazaGameResults *results) {
    int i;
    int count;

    results->flashEnabled = 1;
    count = results->roster.count;
    for (i = 0; i < count; i++) {
        if (results->data.ranks[i] == 0 && results->roster.localIndex == i) {
            PlazaGameResults_PlayFanfare(results);
            return;
        }
    }
}

#pragma force_active on

static inline u8 *PlazaGameResults_GetFlashColors(PlazaGameResults *results, u8 *data, u32 slot) {
    if (results->flashFrame == 0) {
        return data + (slot + 3) * 32;
    } else {
        return data + (slot + 7) * 32;
    }
}

void PlazaGameResults_UpdateFlash(PlazaGameResults *results) {
    if (results->flashEnabled != 0) {
        if (results->flashFrame == 0 || results->flashFrame == 8) {
            int i;
            const u8 *slot;
            i = 0;
            if (i < results->roster.count) {
                slot = ov90_0225C1EC;
                do {
                    if (results->data.ranks[i] == 0) {
                        u32 index = *slot;
                        u32 dest = index * 32;
                        u8 *colors = results->flashPltt->pRawData;
                        GF_ASSERT(GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, dest, PlazaGameResults_GetFlashColors(results, colors, index), 32));
                    }
                    slot++;
                    i++;
                } while (i < results->roster.count);
            }
        }
        results->flashFrame = ((int)results->flashFrame + 1) % 16;
    }
}

#pragma force_active reset

void PlazaGameResults_FreeFlashPalette(PlazaGameResults *results) {
    Heap_Free(results->flashPlttRaw);
}

#pragma force_active on

void PlazaGameContinue_Init(PlazaGameContinue *prompt, PlazaGameBg *bg, u8 unk01, enum HeapID heapId) {
    memset(prompt, 0, sizeof(PlazaGameContinue));
    prompt->template.bgConfig = bg->bgConfig;
    prompt->template.bgId = GF_BG_LYR_SUB_0;
    prompt->template.tileStart = 139;
    prompt->template.plttSlot = 2;
    prompt->template.x = 24;
    prompt->template.y = 8;
    prompt->template.shapeParam = 0;
    prompt->prompt = YesNoPrompt_Create(heapId);
    prompt->unk01 = unk01;
    prompt->allYes = TRUE;
}

#pragma force_active reset

void PlazaGameContinue_Free(PlazaGameContinue *prompt) {
    YesNoPrompt_Destroy(prompt->prompt);
}

#pragma force_active on

// The volatile heapId keeps it on the stack as in the original.
BOOL PlazaGameContinue_Run(PlazaGameContinue *prompt, PlazaGameMessageBox *messageBox, PlazaGameText *text, int game, volatile enum HeapID heapId) {
    switch (prompt->state) {
    case 0: {
        enum HeapID heap = heapId;
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, heap);
        WirelessStatusIcon_ShowOnBothScreens(0, heapId);
        if (prompt->unk01) {
            WifiConnection_StartVoiceChat(heap);
        }
        prompt->state++;
    } break;
    case 1:
        if (IsPaletteFadeFinished()) {
            prompt->state++;
        }
        break;
    case 2:
        PlazaGameText_BufferActivityName(text, game);
        PlazaGameMessageBox_Show(messageBox, text, msg_0236_00003, 1);
        prompt->state++;
        break;
    case 3:
        if (PlazaGameMessageBox_IsPrinterFinished(messageBox, 1) == TRUE) {
            prompt->state++;
        }
        break;
    case 4:
        YesNoPrompt_InitFromTemplate(prompt->prompt, &prompt->template);
        prompt->state++;
        break;
    case 5: {
        int response = YesNoPrompt_HandleInput(prompt->prompt);
        if ((u32)response - 1u <= 1u) {
            if (response == YESNORESPONSE_YES) {
                Comm_SendCommand(22, NULL, 0);
            } else if (response == YESNORESPONSE_NO) {
                Comm_SendCommand(23, NULL, 0);
            }
            PlazaGameMessageBox_Show(messageBox, text, msg_0236_00015, 1);
            PlazaGameMessageBox_ShowWaitingIcon(messageBox, 1);
            YesNoPrompt_Reset(prompt->prompt);
            prompt->state++;
        }
    } break;
    case 6:
        if (prompt->received) {
            if (prompt->result) {
                prompt->state = 10;
            } else {
                prompt->state++;
            }
        }
        break;
    case 7:
        PlazaGameMessageBox_Show(messageBox, text, msg_0236_00006, 1);
        prompt->state++;
        break;
    case 8:
        if (PlazaGameMessageBox_IsPrinterFinished(messageBox, 1) == TRUE) {
            prompt->state++;
            prompt->timer = 102;
        }
        break;
    case 9:
        prompt->timer--;
        if (prompt->timer == 0) {
            prompt->state++;
        }
        break;
    case 10:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, heapId);
        prompt->state++;
        break;
    case 11:
        if (IsPaletteFadeFinished()) {
            WirelessStatusIcon_Destroy();
            if (prompt->unk01) {
                WifiConnection_StopVoiceChat();
            }
            prompt->state++;
        }
        break;
    case 12:
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

u8 PlazaGameContinue_GetResult(const PlazaGameContinue *prompt) {
    return prompt->result;
}

#pragma force_active on

void PlazaGameContinue_ReceiveVote(PlazaGameContinue *prompt, int netId, int yes, int count) {
    int bit = 1 << netId;
    int i;
    u32 votes;
    if (!(prompt->votes & bit)) {
        if (yes == 0) {
            prompt->allYes = FALSE;
        }
        prompt->votes |= bit;
    }
    votes = 0;
    for (i = votes; (int)i < 4; i++) {
        if (prompt->votes & (1 << i)) {
            votes++;
        }
    }
    if (votes == count) {
        if (prompt->allYes) {
            Comm_SendCommand(24, NULL, 0);
        } else {
            Comm_SendCommand(25, NULL, 0);
        }
    }
}

#pragma force_active reset

void PlazaGameContinue_SetResult(PlazaGameContinue *prompt, u8 result) {
    prompt->received = TRUE;
    prompt->result = result;
}

#pragma force_active on

void PlazaGameBalloons_Init(PlazaGameBalloons *balloons, PlazaGameBg *bg, PlazaGameSprites *sprites, PlazaGameGfx *gfx, NARC *narc, enum HeapID heapId) {
    s16 spriteY;
    int y;
    int base;
    int row;
    memset(balloons, 0, sizeof(PlazaGameBalloons));
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 24, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, 32, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 25, bg->bgConfig, GF_BG_LYR_MAIN_2, 139, 0, FALSE, heapId);
    balloons->screenRaw = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 26, FALSE, &balloons->screen, heapId);
    balloons->gfxRes = PlazaGameGfx_Load(gfx, narc, 15, 1, 16, 17, 18, 5001, heapId);
    y = 40;
    row = 0;
    base = 0;
    do {
        int col;
        int x = 74;
        col = 0;
        spriteY = y;
        for (; col < 5; col++, x += 24) {
            // Addressed as the struct shifted by whole sprite slots, as the original does.
            PlazaGameBalloons *slot = (PlazaGameBalloons *)((u8 *)balloons + 4 * (col + base));
            slot->sprites[0] = PlazaGameGfx_CreateSprite(balloons->gfxRes, sprites->spriteList, (s16)x, spriteY, 0, heapId);
            Sprite_SetDrawFlag(slot->sprites[0], FALSE);
            Sprite_SetAnimCtrlSeq(slot->sprites[0], row);
            Sprite_SetPriority(slot->sprites[0], 0);
        }
        y += 36;
        base += 5;
        row++;
    } while (row < 3);
}

#pragma force_active reset

void PlazaGameBalloons_Free(PlazaGameBalloons *balloons) {
    int i;
    for (i = 0; i < 15; i++) {
        Sprite_Delete(balloons->sprites[i]);
    }
    Heap_Free(balloons->screenRaw);
}

void PlazaGameBalloons_Start(PlazaGameBalloons *balloons, int total) {
    balloons->total = total;
    balloons->state = 0;
    balloons->count = 0;
}

BOOL PlazaGameBalloons_Update(PlazaGameBalloons *balloons, PlazaGameBg *bg) {
    int i;
    switch (balloons->state) {
    case 0:
        PlazaGame_OffsetScreenTiles(balloons->screen, 139);
        LoadRectToBgTilemapRect(bg->bgConfig, GF_BG_LYR_MAIN_2, balloons->screen->rawData, 0, 0, 32, 20);
        BgTilemapRectChangePalette(bg->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 20, 14);
        ScheduleBgTilemapBufferTransfer(bg->bgConfig, GF_BG_LYR_MAIN_2);
        balloons->state++;
        break;
    case 1:
        if (balloons->count == balloons->total) {
            balloons->state = 3;
            break;
        }
        PlaySE(SEQ_SE_PL_BALLOON07);
        Sprite_SetDrawFlag(balloons->sprites[balloons->count % 15], TRUE);
        balloons->timer = 8;
        balloons->state++;
        break;
    case 2:
        balloons->timer--;
        if (balloons->timer != 0) {
            break;
        }
        balloons->count++;
        if (balloons->count % 15 == 0 && balloons->count != balloons->total) {
            for (i = 0; i < 15; i++) {
                Sprite_SetDrawFlag(balloons->sprites[i], FALSE);
                Sprite_SetAnimCtrlSeq(balloons->sprites[i], 2);
            }
        }
        balloons->state = 1;
        break;
    case 3:
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

void PlazaGameResults_ShowPlayer(PlazaGameResults *results, int index, int rank) {
    int offset = index * sizeof(PlazaGamePlayer);
    PlazaGamePlayer *players = results->players;
    u32 highlight;
    int digits;
    PlazaGamePlayer_Init((PlazaGamePlayer *)((u8 *)players + offset), results->roster.count, results->game, results->screen, &results->bg, index, results->roster.localIndex, &results->sprites, results->gfxRes, (enum HeapID)results->heapId);
    highlight = ov90_022588A4(&results->roster, PlazaGameRoster_GetNetId(&results->roster, index));
    PlazaGamePlayer_PrintName((PlazaGamePlayer *)((u8 *)players + offset), &results->text, results->info.profiles[index], 8, 0, highlight);
    PlazaGamePlayer_PrintLocation((PlazaGamePlayer *)((u8 *)players + offset), &results->text, results->showCountries, results->info.countries[index], results->info.regions[index], 8);
    if (results->game == 0) {
        digits = 5;
    } else {
        digits = 6;
    }
    PlazaGamePlayer_PrintScore((PlazaGamePlayer *)((u8 *)players + offset), &results->text, results->data.scores[index], (s16)(176 - ov90_0225C1F0[4 - (rank + 1)]), 0, digits);
    PlazaGamePlayer_ScheduleTransfer((PlazaGamePlayer *)((u8 *)players + offset), &results->bg);
    PlazaGamePlayer_ShowRank((PlazaGamePlayer *)((u8 *)players + offset), rank);
    PlazaGamePlayer_StartSlideIn((PlazaGamePlayer *)((u8 *)players + offset), ov90_0225C1F0[rank], 0);
}

#pragma force_active reset

BOOL PlazaGameResults_UpdatePlayers(PlazaGameResults *results) {
    int i;
    BOOL allDone;
    PlazaGamePlayer *player;
    u8 *row;
    PlazaGamePlayer *scrollPlayer;

    allDone = TRUE;
    i = 0;
    if (i < *(volatile u8 *)&results->roster.count) {
        player = results->players;
        // The sprite check walks its own pointer over the rows, as the original does.
        row = (u8 *)results;
        scrollPlayer = player;
        do {
            if (((PlazaGameResults *)row)->players[0].sprite != NULL) {
                BOOL done = PlazaGamePlayer_UpdateSlide(player);
                PlazaGameLineScroll_SetPlayer(&results->lineScroll, scrollPlayer);
                if (!done) {
                    allDone = FALSE;
                }
            } else {
                allDone = FALSE;
            }
            row += sizeof(PlazaGamePlayer);
            player++;
            scrollPlayer++;
            i++;
        } while (i < *(volatile u8 *)&results->roster.count);
    }
    return allDone;
}

void PlazaGameResults_PlayFanfare(PlazaGameResults *results) {
    if (results->fanfareState == 0) {
        PlayFanfare(SEQ_ME_MINIGAME);
        results->fanfareState = 1;
    }
}

BOOL PlazaGameResults_IsFanfareDone(PlazaGameResults *results) {
    if (results->fanfareState == 1 && !IsFanfarePlaying()) {
        results->fanfareState = 0;
    }
    return results->fanfareState != 1;
}

void PlazaGameResults_RecordFriendPlays(PlazaGameResults *results) {
    FriendRoster *friends = Save_FriendRoster_Get(results->roster.saveData);
    int i;
    for (i = 0; i < results->roster.count; i++) {
        u32 index;
        void *friendData = CommPlayerInfo_GetExchangeToken(results->roster.netIds[i]);
        int result = Comm_FindFriendSlot(results->roster.saveData, friendData, &index);
        switch (result) {
        case 0:
        case 1:
            switch (results->game) {
            case 0:
                sub_0202C554(friends, index, 1);
                break;
            case 1:
                sub_0202C584(friends, index, 1);
                break;
            case 2:
                sub_0202C5B4(friends, index, 1);
                break;
            }
            break;
        }
    }
}

int PlazaGameResults_GetScoreThreshold(PlazaGameResults *results) {
    switch (results->game) {
    case 0:
        return 0;
    case 1:
        return 0;
    case 2:
        GF_AssertFail();
        return 0;
    }
}

#pragma force_active on

void PlazaGameResults_ReportWinners(PlazaGameResults *results) {
    u32 ids[4] = { 0 };
    int i;
    int count;
    int threshold;

    count = 0;
    threshold = PlazaGameResults_GetScoreThreshold(results);
    for (i = 0; i < results->roster.count; i++) {
        if (results->data.ranks[i] == 0 && results->data.scores[i] != threshold) {
            ids[count] = results->roster.args->unk04[i];
            count++;
        }
    }
    if (count != 0) {
        ov45_0222ACB8(results->roster.args->unk30, results->game, count, ids[0], ids[1], ids[2], ids[3]);
    }
}

#pragma force_active reset

void PlazaGameResults_ReportTeamWin(PlazaGameResults *results) {
    if (results->roster.count != 0 && results->data.total >= 10) {
        PlazaGameArgs *args = results->roster.args;
        ov45_0222ACB8(args->unk30, results->game, results->roster.count, args->unk04[0], args->unk04[1], args->unk04[2], args->unk04[3]);
    }
}

static inline void PlazaGameSignWindow_SetInsidePlanes(u32 planes, BOOL effect) {
    u32 value = (reg_G2_WININ & ~0x3F) | planes;
    if (effect) {
        value |= 0x20;
    }
    reg_G2_WININ = (u16)value;
}

#pragma force_active on
#pragma opt_arithtransformation off

void PlazaGameSignWindow_Init(PlazaGameSignWindow *window, PlazaGameGfxRes *gfxRes, SpriteList *spriteList, enum HeapID heapId) {
    s32 i;
    u32 windows;
    memset(window->lines, 0, sizeof(window->lines));
    memset(window->pendingLines, 0, sizeof(window->pendingLines));
    window->step = 0;
    window->offset = 0;
    window->task = NULL;
    for (i = 0; i < 2; i++) {
        window->sprites[i] = PlazaGameGfx_CreateSprite(gfxRes, spriteList, (s16)(ov90_0225C2B4[i].x >> FX32_SHIFT), (s16)(ov90_0225C2B4[i].y >> FX32_SHIFT), 0, heapId);
        Sprite_SetAnimCtrlSeq(window->sprites[i], 5);
        Sprite_SetPriority(window->sprites[i], 0);
    }
    windows = (reg_GX_DISPCNT & 0xE000) >> 13;
    reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | ((windows & ~1) << 13);
    PlazaGameSignWindow_SetInsidePlanes(31, TRUE);
    reg_G2_WIN0H = 0;
    reg_G2_WIN0V = 0;
    *(u8 *)&window->savedOutside = *(vu8 *)&reg_G2_WINOUT;
    window->savedWindows = ((reg_GX_DISPCNT & 0xE000) >> 13) & 2;
    window->signOutside.planes = 15;
    window->signOutside.effect = 1;
    Main_SetHBlankIntrCB(PlazaGameSignWindow_HBlank, window);
    Sprite_SetFlipMode(window->sprites[1], 2);
    PlazaGameSignWindow_UpdateLines(window);
}

#pragma opt_arithtransformation reset
#pragma force_active reset

BOOL PlazaGameSignWindow_Open(PlazaGameSignWindow *window) {
    BOOL done = FALSE;
    window->step++;
    if (window->step >= 8) {
        window->step = 8;
        done = TRUE;
    }
    PlazaGameSignWindow_Move(window, 1);
    return done;
}

BOOL PlazaGameSignWindow_Close(PlazaGameSignWindow *window) {
    BOOL done = FALSE;
    window->step--;
    if (window->step <= 0) {
        done = TRUE;
        window->step = 1;
        Sprite_SetDrawFlag(window->sprites[0], FALSE);
        Sprite_SetDrawFlag(window->sprites[1], FALSE);
    }
    PlazaGameSignWindow_Move(window, -1);
    return done;
}

#pragma force_active on

void PlazaGameSignWindow_Move(PlazaGameSignWindow *window, int delta) {
    int offset;
    int i;
    VecFx32 pos;
    window->offset = window->step * 23 / 8;
    if (window->step + delta <= 8 && window->step + delta >= 0) {
        offset = (window->step + delta) * 23 / 8;
    } else {
        offset = window->offset;
    }
    for (i = 0; i < 2; i++) {
        pos = ov90_0225C2B4[i];
        if (i == 0) {
            float distance;
            if (offset > 0) {
                distance = 0.5f + (float)(offset << FX32_SHIFT);
            } else {
                distance = (float)(offset << FX32_SHIFT) - 0.5f;
            }
            pos.y -= (int)distance;
        } else {
            float distance;
            if (offset > 0) {
                distance = 0.5f + (float)(offset << FX32_SHIFT);
            } else {
                distance = (float)(offset << FX32_SHIFT) - 0.5f;
            }
            pos.y += (int)distance;
        }
        Sprite_SetMatrix(window->sprites[i], &pos);
    }
    PlazaGameSignWindow_UpdateLines(window);
}

#pragma force_active reset

void PlazaGameSignWindow_Free(PlazaGameSignWindow *window) {
    int i;
    if (window->task != NULL) {
        SysTask_Destroy(window->task);
        window->task = NULL;
    }
    for (i = 0; i < 2; i++) {
        if (window->sprites[i] != NULL) {
            Sprite_Delete(window->sprites[i]);
            window->sprites[i] = NULL;
        }
    }
    HBlankInterruptDisable();
    reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | (window->savedWindows << 13);
    {
        u32 value = (reg_G2_WINOUT & ~0x3F) | window->savedOutside.planes;
        if (window->savedOutside.effect) {
            value |= 0x20;
        }
        reg_G2_WINOUT = value;
    }
}

void PlazaGameSignWindow_UpdateLines(PlazaGameSignWindow *window) {
    int i;
    memset(window->pendingLines, 0, sizeof(window->pendingLines));
    for (i = 0; i < 23 - window->offset; i++) {
        window->pendingLines[73 + i] = 1;
        window->pendingLines[121 - i] = 1;
    }
    if (window->task == NULL) {
        window->task = SysTask_CreateOnVWaitQueue(PlazaGameSignWindow_CommitLinesTask, window, 0);
    }
}

void PlazaGameSignWindow_CommitLinesTask(SysTask *task, void *data) {
    PlazaGameSignWindow *window = data;
    memcpy(window->lines, window->pendingLines, sizeof(window->lines));
    SysTask_Destroy(window->task);
    window->task = NULL;
}

void PlazaGameSignWindow_HBlank(void *data) {
    PlazaGameSignWindow *window = data;
    int line;
    u32 windows;
    line = ((int)reg_GX_VCOUNT + 1) % 256;
    if (line < 192) {
        windows = (reg_GX_DISPCNT & 0xE000) >> 13;
        if ((reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) != 0) {
            if (line < 73 || line > 121) {
                u32 value = (reg_G2_WINOUT & ~0x3F) | window->savedOutside.planes;
                if (window->savedOutside.effect) {
                    value |= 0x20;
                }
                reg_G2_WINOUT = value;
                windows |= window->savedWindows;
            } else {
                u32 value = (reg_G2_WINOUT & ~0x3F) | window->signOutside.planes;
                if (window->signOutside.effect) {
                    value |= 0x20;
                }
                reg_G2_WINOUT = value;
                windows &= ~(window->savedWindows | 2);
            }
            if (window->lines[line] == 1) {
                reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | ((windows | 1) << 13);
            } else {
                reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFFF1FFF) | ((windows & ~1) << 13);
            }
        }
    }
}

void PlazaGameSign_CountdownTask(SysTask *task, void *data) {
    PlazaGameSign *sign = data;
    switch (sign->state) {
    case 1:
        PlazaGameSignWindow_Init(&sign->window, sign->gfxRes, sign->spriteList, sign->heapId);
        sign->sprite = PlazaGameGfx_CreateSprite(sign->gfxRes, sign->spriteList, 128, 97, 64, sign->heapId);
        Sprite_SetPriority(sign->sprite, 0);
        sign->state++;
        break;
    case 2:
        if (PlazaGameSignWindow_Open(&sign->window) == TRUE) {
            sign->state++;
            sign->timer = 0;
        }
        break;
    case 3: {
        fx32 prevFrame = Sprite_GetAnimationFrame(sign->sprite);
        fx32 frame;
        Sprite_UpdateAnim(sign->sprite, 2 * FX32_ONE);
        frame = Sprite_GetAnimationFrame(sign->sprite);
        if (prevFrame != frame) {
            switch (frame) {
            case 7:
                PlaySE(SEQ_SE_DP_DECIDE);
                break;
            case 9:
                PlaySE(SEQ_SE_DP_DECIDE);
                break;
            case 11:
                PlaySE(SEQ_SE_DP_DECIDE);
                break;
            case 13:
                PlaySE(SEQ_SE_DP_DECIDE);
                break;
            case 15:
                PlaySE(SEQ_SE_DP_CON_016);
                break;
            }
        }
        if (frame == 15) {
            if (sign->timer == 0) {
                Sprite_SetPalOffsetRespectVramOffset(sign->sprite, 0);
            } else if (sign->timer == 4) {
                Sprite_SetPalOffsetRespectVramOffset(sign->sprite, 1);
            }
            sign->timer++;
            if (sign->timer >= 8) {
                sign->timer = 0;
            }
        }
        if (!Sprite_IsAnimated(sign->sprite)) {
            sign->state++;
            Sprite_SetPalOffsetRespectVramOffset(sign->sprite, 0);
        }
    } break;
    case 4:
        if (PlazaGameSignWindow_Close(&sign->window) == TRUE) {
            Sprite_Delete(sign->sprite);
            sign->sprite = NULL;
            sign->state++;
            sign->timer = 0;
        }
        break;
    case 5:
        sign->timer++;
        if (sign->timer >= 2) {
            sign->state = 0;
            PlazaGameSignWindow_Free(&sign->window);
            SysTask_Destroy(sign->task);
            sign->task = NULL;
        }
        break;
    }
}

void PlazaGameSign_FinishTask(SysTask *task, void *data) {
    PlazaGameSign *sign = data;
    switch (sign->state) {
    case 1:
        PlazaGameSignWindow_Init(&sign->window, sign->gfxRes, sign->spriteList, sign->heapId);
        sign->sprite = PlazaGameGfx_CreateSprite(sign->gfxRes, sign->spriteList, 128, 97, 64, sign->heapId);
        Sprite_SetAnimCtrlSeq(sign->sprite, 1);
        Sprite_SetPriority(sign->sprite, 0);
        sign->state++;
        break;
    case 2:
        if (PlazaGameSignWindow_Open(&sign->window) == TRUE) {
            Sprite_SetAnimActiveFlag(sign->sprite, TRUE);
            sign->state++;
            sign->timer = 0;
        }
        break;
    case 3:
        sign->timer++;
        if (sign->timer >= 50) {
            sign->state++;
        }
        break;
    case 4:
        if (PlazaGameSignWindow_Close(&sign->window) == TRUE) {
            Sprite_Delete(sign->sprite);
            sign->sprite = NULL;
            sign->timer = 0;
            sign->state++;
        }
        break;
    case 5:
        sign->timer++;
        if (sign->timer >= 2) {
            sign->state = 0;
            PlazaGameSignWindow_Free(&sign->window);
            SysTask_Destroy(sign->task);
            sign->task = NULL;
        }
        break;
    }
}

const void *PlazaGame_GetCommandTable(void) {
    return ov90_0225C4FC;
}

int PlazaGame_GetCommandCount(void) {
    return 5;
}

void PlazaGameResults_InitCommands(PlazaGameResults *results) {
    Comm_RegisterCommandTable(PlazaGame_GetCommandTable(), PlazaGame_GetCommandCount(), results);
}

void PlazaGameEntry_InitCommands(PlazaGameEntry *entry) {
    Comm_RegisterCommandTable(PlazaGame_GetCommandTable(), PlazaGame_GetCommandCount(), entry);
}

int PlazaGame_GetCommandSizeNone(void) {
    return 0;
}

int PlazaGame_GetCommandSizeU32(void) {
    return 4;
}

void PlazaGame_HandleSelectionCommand(int netId, int size, int *data, PlazaGameEntry *entry) {
    PlazaGameEntry_SetSelection(entry, *data);
}

void PlazaGame_HandleVoteYesCommand(int netId, int size, void *data, PlazaGameResults *results) {
    PlazaGameResults_ReceiveVote(results, netId, TRUE);
}

void PlazaGame_HandleVoteNoCommand(int netId, int size, void *data, PlazaGameResults *results) {
    PlazaGameResults_ReceiveVote(results, netId, FALSE);
}

void PlazaGame_HandleContinueCommand(int netId, int size, void *data, PlazaGameResults *results) {
    PlazaGameResults_ReceiveContinue(results, TRUE);
}

void PlazaGame_HandleQuitCommand(int netId, int size, void *data, PlazaGameResults *results) {
    PlazaGameResults_ReceiveContinue(results, FALSE);
}
