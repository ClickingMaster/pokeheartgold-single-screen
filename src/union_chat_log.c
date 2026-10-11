#include "union_chat_log.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0738_UNION.h"

#include "dialog_box.h"
#include "field_link_messages_internal.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail_message.h"
#include "overlay_01.h"
#include "pm_string.h"
#include "save_palpad.h"
#include "sound_playback.h"
#include "sprite_transfer.h"
#include "systask_environment.h"
#include "system.h"
#include "task.h"
#include "text.h"
#include "touchscreen.h"

// Link player record of the communication layer (wireless_connection_state_8.c); only the chat
// message and its sender are read here.
typedef struct UnionChatLogPeerChat {
    u32 trainerId;
    u32 unk_04;
    MailMessage message;
} UnionChatLogPeerChat;

typedef struct UnionChatLogPeer {
    u8 unk_00[0x50];
    UnionChatLogPeerChat chat; // 0x50
} UnionChatLogPeer;

// wireless_connection_state_8.c; its header declares sub_02035798 with another return type than
// field_link_messages_internal.h, so the two are declared here.
extern UnionChatLogPeer *sub_02035754(int index);
extern PlayerProfile *sub_02035784(void);

extern const int sUnionChatLogRowPressBgOffsets[];
extern const s32 sUnionChatLogSpriteYOffsets[];
extern const TouchscreenHitbox sUnionChatLogScrollbarHitboxes[];
extern const BgTemplate sUnionChatLogBgTemplateSub2;
extern const BgTemplate sUnionChatLogBgTemplateSub3;
extern const BgTemplate sUnionChatLogBgTemplateSub1;
extern const BgTemplate sUnionChatLogBgTemplateSub0;
extern const TouchscreenHitbox sUnionChatLogButtonHitboxes[];

void UnionChatLog_PrintMenuButton(UnionChatLogData *data);
void UnionChatLog_Update(UnionChatLogData *data, BgConfig *bgConfig);
void UnionChatLog_Task(SysTask *task, void *taskData);
void UnionChatLog_InitText(UnionChatLogData *data);
void UnionChatLog_AddWindows(BgConfig *bgConfig, UnionChatLogRow *rows, Window *menuWindow);
void UnionChatLog_RemoveWindows(UnionChatLogRow *rows, Window *menuWindow);
void UnionChatLog_FreeBgs(BgConfig *bgConfig);
void UnionChatLog_InitBgs(BgConfig *bgConfig);
void UnionChatLog_InitSpriteResources(UnionChatLogData *data);
void UnionChatLog_CreateSprites(UnionChatLogData *data);
int UnionChatLog_NextLogIndex(UnionChatLogMessages *log, int index);
int UnionChatLog_AddLogIndex(int index, int offset);
void UnionChatLog_DrawRow(UnionChatLogData *data, int row, MessageSlot *slot);
void UnionChatLog_DrawRows(UnionChatLogData *data);
void UnionChatLog_SetThumbY(Sprite *sprite, int y);
void UnionChatLog_UpdateScrollbar(UnionChatLogData *data);
int UnionChatLog_HandleTouchButtons(UnionChatLogData *data);
int UnionChatLog_HandleTouchScrollbar(UnionChatLogData *data);
void UnionChatLog_HandleScrollKeys(UnionChatLogData *data);
void UnionChatLog_HandleInput(UnionChatLogData *data);
void UnionChatLog_UpdateRowPress(BgConfig *bgConfig, NNSG2dScreenData *screenData, UnionChatLogRow *rows, int heldRow, int count, int *pressSum);
String *UnionChatLog_GetFriendOfText(SavePalPad *palPad, u32 trainerId, MessageFormat *msgFormat, MsgData *msgData, PlayerProfile *profile);
void UnionChatLog_AddEntry(UnionChatLogData *data, u32 trainerId, MailMessage *mail, PlayerProfile *profile);
BOOL UnionChatLog_IsNewMessage(UnionChatLogData *data, MailMessage *mail, u32 trainerId);
void UnionChatLog_AppendMessage(UnionChatLogData *data, PlayerProfile *profile, MailMessage *mail, u32 trainerId);
void UnionChatLog_CollectMessages(UnionChatLogData *data);
void UnionChatLog_RequestRedraw(UnionChatLogData *data);
void UnionChatLog_InitTouchRepeat(UnionChatLogData *data);
void UnionChatLog_UpdateTouchRepeat(UnionChatLogData *data);
BOOL UnionChatLog_IsTouchRepeatTick(UnionChatLogData *data);
void UnionChatLog_SetThumbFollowsScroll(UnionChatLogData *data, BOOL follow);
BOOL UnionChatLog_GetThumbFollowsScroll(UnionChatLogData *data);
void UnionChatLog_PressArrow(UnionChatLogData *data, int arrow);

void UnionChatLog_PrintMenuButton(UnionChatLogData *data) {
    String *string = NewString_ReadMsgData(data->msgData, msg_0738_UNION_00219);
    AddTextPrinterParameterizedWithColor(&data->menuWindow, 0, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    String_Delete(string);
}

void UnionChatLog_Update(UnionChatLogData *data, BgConfig *bgConfig) {
    if ((gSystem.newKeys & PAD_BUTTON_X) && ov01_021F6B10(data->fieldSystem) == TRUE) {
        data->state = UNION_CHAT_LOG_STATE_CLOSE;
    }
    UnionChatLog_UpdateTouchRepeat(data);
    UnionChatLog_CollectMessages(data);
    UnionChatLog_UpdateScrollbar(data);
    UnionChatLog_HandleInput(data);
    UnionChatLog_DrawRows(data);
    UnionChatLog_UpdateArrows(data);
}

void UnionChatLog_Task(SysTask *task, void *taskData) {
    UnionChatLogData *data = taskData;
    FieldSystem *fieldSystem = data->fieldSystem;
    BgConfig *bgConfig = data->bgConfig;
    if (fieldSystem->unk84 == NULL) {
        return;
    }
    switch (data->state) {
    case 0:
    case 1:
        break;
    case UNION_CHAT_LOG_STATE_RUNNING:
        if (!FieldSystem_TaskIsRunning(fieldSystem)) {
            UnionChatLog_Update(data, bgConfig);
        }
        SpriteList_RenderAndAnimateSprites(data->spriteList);
        break;
    case UNION_CHAT_LOG_STATE_CLOSE:
        ov01_021F6A9C(fieldSystem, 0, NULL);
        data->state = UNION_CHAT_LOG_STATE_CLOSING;
        break;
    case UNION_CHAT_LOG_STATE_CLOSING:
    case UNION_CHAT_LOG_STATE_FREED:
        break;
    }
}

void UnionChatLog_InitText(UnionChatLogData *data) {
    data->redraw = FALSE;
    data->count = 0;
    data->prevCount = 0;
    data->scroll = 0;
    data->scrollbarVisible = FALSE;
    data->msgFormat = MessageFormat_New(HEAP_ID_FIELD1);
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0738_UNION_bin, HEAP_ID_FIELD1);
    data->screenFile = GfGfxLoader_GetScrnData(NARC_a_0_7_3, 3, TRUE, &data->screenData, HEAP_ID_FIELD1);
}

// Per row: the speaker's name, the chat message and the "X's friend" line, on sub BG 0-2.
void UnionChatLog_AddWindows(BgConfig *bgConfig, UnionChatLogRow *rows, Window *menuWindow) {
    u32 nameTile;
    u32 messageTile;
    u32 messageY;
    u32 friendOfTile;
    u32 friendOfTileStart;
    int i;
    int nameY;
    UnionChatLogRow *row;
    nameTile = 0xA0;
    messageTile = 0xD0;
    messageY = 5;
    friendOfTileStart = 0x265;
    row = rows;
    i = 0;
    nameY = 3;
    friendOfTile = friendOfTileStart;
    for (; i < UNION_CHAT_LOG_NUM_ROWS; i++) {
        AddWindowParameterized(bgConfig, &row->name, i + GF_BG_LYR_SUB_0, 1, nameY, 8, 2, 12, nameTile);
        FillWindowPixelBuffer(&row->name, 0);
        CopyWindowToVram(&row->name);
        AddWindowParameterized(bgConfig, &row->message, i + GF_BG_LYR_SUB_0, 2, messageY, 27, 5, 12, messageTile);
        FillWindowPixelBuffer(&row->message, 0);
        CopyWindowToVram(&row->message);
        AddWindowParameterized(bgConfig, &row->friendOf, i + GF_BG_LYR_SUB_0, 12, nameY, 15, 2, 12, friendOfTile);
        FillWindowPixelBuffer(&row->friendOf, 0);
        CopyWindowToVram(&row->friendOf);
        nameTile += 8 * 2;
        messageTile += 27 * 5;
        messageY += 7;
        nameY += 7;
        friendOfTile += 15 * 2;
        row++;
    }
    AddWindowParameterized(bgConfig, menuWindow, GF_BG_LYR_SUB_0, 8, 0, 7, 2, 12, 0x2BF);
    FillWindowPixelBuffer(menuWindow, 0);
}

void UnionChatLog_RemoveWindows(UnionChatLogRow *rows, Window *menuWindow) {
    int i;
    UnionChatLogRow *row = rows;
    RemoveWindow(menuWindow);
    for (i = 0; i < UNION_CHAT_LOG_NUM_ROWS; i++, row++) {
        RemoveWindow(&row->message);
        RemoveWindow(&row->name);
        RemoveWindow(&row->friendOf);
    }
}

UnionChatLogData *UnionChatLog_Init(FieldSystem *fieldSystem) {
    SysTask *task = CreateSysTaskAndEnvironment(UnionChatLog_Task, sizeof(UnionChatLogData), 4, HEAP_ID_FIELD1);
    UnionChatLogData *data = SysTask_GetData(task);
    data->fieldSystem = fieldSystem;
    data->bgConfig = fieldSystem->bgConfig;
    data->unionRoom = (Manager *)fieldSystem->unk84;
    data->unk_08 = fieldSystem->unk80;
    data->profile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    data->state = UNION_CHAT_LOG_STATE_RUNNING;
    data->task = task;
    data->log = (UnionChatLogMessages *)((Manager *)fieldSystem->unk84)->messages;
    UnionChatLog_InitBgs(data->bgConfig);
    UnionChatLog_InitText(data);
    SetKeyRepeatTimers(4, 8);
    UnionChatLog_InitTouchRepeat(data);
    UnionChatLog_SetThumbFollowsScroll(data, TRUE);
    UnionChatLog_InitSpriteResources(data);
    UnionChatLog_CreateSprites(data);
    FontID_SetAccessDirect(1, HEAP_ID_FIELD1);
    UnionChatLog_AddWindows(data->bgConfig, data->rows, &data->menuWindow);
    UnionChatLog_PrintMenuButton(data);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    UnionChatLog_RequestRedraw(data);
    return data;
}

void UnionChatLog_Exit(UnionChatLogData *data) {
    int i;
    BgConfig *bgConfig;
    // running, close requested or closing
    if (data->state - UNION_CHAT_LOG_STATE_RUNNING <= UNION_CHAT_LOG_STATE_CLOSING - UNION_CHAT_LOG_STATE_RUNNING) {
        bgConfig = data->bgConfig;
        data->state = UNION_CHAT_LOG_STATE_FREED;
        FontID_SetAccessLazy(1);
        SpriteTransfer_DeleteCharTransferTask(data->resObjs[GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_DeletePlttTransferTask(data->resObjs[GF_GFX_RES_TYPE_PLTT]);
        for (i = 0; i < 4; i++) {
            Destroy2DGfxResObjMan(data->resMans[i]);
        }
        SpriteList_Delete(data->spriteList);
        DestroyMsgData(data->msgData);
        MessageFormat_Delete(data->msgFormat);
        UnionChatLog_RemoveWindows(data->rows, &data->menuWindow);
        UnionChatLog_FreeBgs(bgConfig);
        Heap_Free(data->screenFile);
        DestroySysTaskAndEnvironment(data->task);
    } else {
        GF_AssertFail();
    }
}

void UnionChatLog_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
}

void UnionChatLog_InitBgs(BgConfig *bgConfig) {
    BgTemplate bg4;
    BgTemplate bg5;
    BgTemplate bg6;
    BgTemplate bg7;
    UnionChatLog_FreeBgs(bgConfig);
    bg4 = sUnionChatLogBgTemplateSub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    bg5 = sUnionChatLogBgTemplateSub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    bg6 = sUnionChatLogBgTemplateSub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bg6, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    bg7 = sUnionChatLogBgTemplateSub3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bg7, GF_BG_TYPE_TEXT);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfxLoader_GXLoadPal(NARC_a_0_7_3, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_FIELD1);
    GfGfxLoader_LoadCharData(NARC_a_0_7_3, 2, bgConfig, GF_BG_LYR_SUB_3, 0, 0x1400, TRUE, HEAP_ID_FIELD1);
    GfGfxLoader_LoadScrnData(NARC_a_0_7_3, 4, bgConfig, GF_BG_LYR_SUB_3, 0, 0x600, TRUE, HEAP_ID_FIELD1);
    FieldMessage_LoadTextPalettes(GF_PAL_LOCATION_SUB_BG, FALSE);
}

void UnionChatLog_InitSpriteResources(UnionChatLogData *data) {
    int i;
    data->spriteList = G2dRenderer_Init(10, &data->renderer, HEAP_ID_FIELD1);
    for (i = 0; i < 4; i++) {
        data->resMans[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, HEAP_ID_FIELD1);
    }
    data->resObjs[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CHAR], NARC_a_0_7_3, 5, TRUE, 0x3E7, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_FIELD1);
    data->resObjs[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_PLTT], NARC_a_0_7_3, 1, FALSE, 0x3E7, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_FIELD1);
    data->resObjs[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_CELL], NARC_a_0_7_3, 6, TRUE, 0x3E7, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    data->resObjs[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(data->resMans[GF_GFX_RES_TYPE_ANIM], NARC_a_0_7_3, 7, TRUE, 0x3E7, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(data->resObjs[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(data->resObjs[GF_GFX_RES_TYPE_PLTT]);
}

// Up arrow, down arrow and scrollbar thumb on the right edge of the sub screen.
void UnionChatLog_CreateSprites(UnionChatLogData *data) {
    SpriteTemplate template;
    int i;
    CreateSpriteResourcesHeader(&data->header, 0x3E7, 0x3E7, 0x3E7, 0x3E7, -1, -1, 0, 0, data->resMans[GF_GFX_RES_TYPE_CHAR], data->resMans[GF_GFX_RES_TYPE_PLTT], data->resMans[GF_GFX_RES_TYPE_CELL], data->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = data->spriteList;
    template.header = &data->header;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_FIELD1;
    for (i = 0; i < 3; i++) {
        float y;
        template.position.x = FX32_CONST(232);
        if (sUnionChatLogSpriteYOffsets[i] > 0) {
            y = (float)(sUnionChatLogSpriteYOffsets[i] << FX32_SHIFT) + 0.5f;
        } else {
            y = (float)(sUnionChatLogSpriteYOffsets[i] << FX32_SHIFT) - 0.5f;
        }
        template.position.y = (s32)y + FX32_CONST(192);
        data->sprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(data->sprites[i], TRUE);
        Sprite_SetAnimCtrlSeq(data->sprites[i], i);
    }
}

int UnionChatLog_NextLogIndex(UnionChatLogMessages *log, int index) {
    index++;
    if (index == UNION_CHAT_LOG_NUM_MESSAGES) {
        index = 0;
    }
    return index;
}

int UnionChatLog_AddLogIndex(int index, int offset) {
    int sum = index + offset;
    if (sum >= UNION_CHAT_LOG_NUM_MESSAGES) {
        sum -= UNION_CHAT_LOG_NUM_MESSAGES;
    }
    return sum;
}

void UnionChatLog_DrawRow(UnionChatLogData *data, int row, MessageSlot *slot) {
    data->rows[row].gender = slot->unk10;
    CopyToBgTilemapRect(data->bgConfig, GF_BG_LYR_SUB_3, 0, 7 * row + 2, 32, 7, data->screenData->rawData, 0, data->rows[row].gender * 24, 32, 48);
    FillWindowPixelBuffer(&data->rows[row].name, 0);
    FillWindowPixelBuffer(&data->rows[row].message, 0);
    FillWindowPixelBuffer(&data->rows[row].friendOf, 0);
    AddTextPrinterParameterizedWithColor(&data->rows[row].name, 1, slot->string0, 0, 1, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    AddTextPrinterParameterizedWithColor(&data->rows[row].message, 1, slot->string1, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&data->rows[row].name);
    ScheduleWindowCopyToVram(&data->rows[row].message);
    if (slot->string2 != NULL) {
        AddTextPrinterParameterizedWithColor(&data->rows[row].friendOf, 1, slot->string2, 0, 1, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 2, 0), NULL);
    }
    ScheduleWindowCopyToVram(&data->rows[row].friendOf);
}

void UnionChatLog_DrawRows(UnionChatLogData *data) {
    int index;
    int count;
    int i;
    void *buffer;

    index = UnionChatLog_AddLogIndex(data->log->head, data->scroll);
    count = data->log->count;
    if (count > UNION_CHAT_LOG_NUM_ROWS) {
        count = UNION_CHAT_LOG_NUM_ROWS;
    }
    if (data->scroll != data->drawnScroll) {
        UnionChatLog_RequestRedraw(data);
        data->drawnScroll = data->scroll;
    }
    if (data->redraw == TRUE) {
        for (i = 0; i < count; i++) {
            UnionChatLog_DrawRow(data, i, &data->log->slots[index]);
            index = UnionChatLog_NextLogIndex(data->log, index);
        }
        buffer = GetBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_3);
        DC_FlushRange(buffer, 0x600);
        buffer = GetBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_3);
        BgCopyOrUncompressTilemapBufferRangeToVram(data->bgConfig, GF_BG_LYR_SUB_3, buffer, 0x600, 0);
        data->redraw = FALSE;
    }
}

void UnionChatLog_SetThumbY(Sprite *sprite, int y) {
    VecFx32 pos;
    float fy;
    pos.x = FX32_CONST(232);
    if (y > 0) {
        fy = (float)(y << FX32_SHIFT) + 0.5f;
    } else {
        fy = (float)(y << FX32_SHIFT) - 0.5f;
    }
    pos.y = FX32_CONST(192) + (s32)fy;
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
}

// The scrollbar appears once there are more messages than rows; while the view is at the
// bottom it follows new messages.
void UnionChatLog_UpdateScrollbar(UnionChatLogData *data) {
    data->count = data->log->count;
    if (data->count > UNION_CHAT_LOG_NUM_ROWS && data->prevCount <= UNION_CHAT_LOG_NUM_ROWS) {
        data->scrollbarVisible = TRUE;
        data->scroll = data->count - UNION_CHAT_LOG_NUM_ROWS;
    }
    Sprite_SetDrawFlag(data->sprites[UNION_CHAT_LOG_SPRITE_SCROLLBAR], data->scrollbarVisible);
    if (data->scrollbarVisible != FALSE && UnionChatLog_GetThumbFollowsScroll(data) == TRUE) {
        UnionChatLog_SetThumbY(data->sprites[UNION_CHAT_LOG_SPRITE_SCROLLBAR], 48 + (data->scroll * 96) / (data->count - UNION_CHAT_LOG_NUM_ROWS));
    }
    data->prevCount = data->count;
}

// force_active only sets a section flag here; kept so the object stays identical.
#pragma force_active on

// Buttons: 0/1 arrows, 2-4 rows, 5 scrollbar, 6 "Menu" bar. Touching a row makes the avatar of
// the message's sender react in the Union Room.
int UnionChatLog_HandleTouchButtons(UnionChatLogData *data) {
    int hit = TouchscreenHitbox_FindRectAtTouchHeld(sUnionChatLogButtonHitboxes);
    BOOL tick = UnionChatLog_IsTouchRepeatTick(data);
    if (hit != -1) {
        switch (hit) {
        case 0:
            UnionChatLog_PressArrow(data, hit);
            if (tick == TRUE) {
                if (data->scroll) {
                    PlaySE(SEQ_SE_DP_BUTTON3);
                    data->scroll--;
                }
                data->touchedButton = hit - 2;
            }
            break;
        case 1:
            UnionChatLog_PressArrow(data, hit);
            if (tick == TRUE) {
                if ((int)data->scroll < (int)data->count - UNION_CHAT_LOG_NUM_ROWS) {
                    PlaySE(SEQ_SE_DP_BUTTON3);
                    data->scroll++;
                }
                data->touchedButton = hit - 2;
            }
            break;
        case 5:
            data->touchedButton = hit - 2;
            break;
        case 6:
            if (gSystem.touchNew && ov01_021F6B10(data->fieldSystem) == TRUE) {
                PlaySE(SEQ_SE_DP_WIN_OPEN);
                data->state = UNION_CHAT_LOG_STATE_CLOSE;
            }
            break;
        default:
            if (gSystem.touchNew) {
                if (data->log->count >= hit - 1) {
                    int index = UnionChatLog_AddLogIndex(data->log->head, data->scroll + hit - 2);
                    int i;
                    for (i = 0; i < 10; i++) {
                        UnionChatLogPeer *peer = sub_02035754(i);
                        if (peer) {
                            register UnionChatLogPeerChat *chat = &peer->chat;
                            if (data->unionRoom->slots[i].state == 2 && chat->trainerId == data->log->slots[index].unkC) {
                                PlaySE(SEQ_SE_DP_BUTTON3);
                                data->unionRoom->slots[i].trigger = 1;
                                break;
                            }
                        }
                    }
                    if (PlayerProfile_GetTrainerID(sub_02035784()) == data->log->slots[index].unkC) {
                        PlaySE(SEQ_SE_DP_BUTTON3);
                        data->unionRoom->slots[50].trigger = 1;
                    }
                }
                data->touchedButton = hit - 2;
            }
            break;
        }
    }
    return hit;
}

#pragma force_active reset

// Hitbox 0 is the scrollbar strip (dragging the thumb), 1-3 the rows.
int UnionChatLog_HandleTouchScrollbar(UnionChatLogData *data) {
    u32 x;
    u32 y;
    int hit = TouchscreenHitbox_FindRectAtTouchHeld(sUnionChatLogScrollbarHitboxes);
    if (hit != -1) {
        if (hit == 0) {
            int count;
            int height;
            int i;
            int top;
            int bottom;
            UnionChatLog_SetThumbFollowsScroll(data, FALSE);
            System_GetTouchHeldCoords(&x, &y);
            UnionChatLog_SetThumbY(data->sprites[UNION_CHAT_LOG_SPRITE_SCROLLBAR], y);
            if (data->count > UNION_CHAT_LOG_NUM_ROWS) {
                count = data->count - 2;
                height = 96 / count;
                i = 0;
                if (count > 0) {
                    u32 touchY = y;
                    top = i;
                    bottom = i;
                    do {
                        if (touchY >= top + 48 && touchY < bottom + height + 48) {
                            data->scroll = i;
                            break;
                        }
                        i++;
                        top += height;
                        bottom += height;
                    } while (i < count);
                }
            }
        } else if (data->log->count >= hit && data->touchedButton == hit - 1) {
            int *pressTimer = &data->rows[hit - 1].pressTimer;
            if (*pressTimer < 5) {
                (*pressTimer)++;
            }
        }
    } else {
        UnionChatLog_SetThumbFollowsScroll(data, TRUE);
    }
    return hit;
}

void UnionChatLog_HandleScrollKeys(UnionChatLogData *data) {
    u32 held = gSystem.heldKeys;
    if (held & PAD_BUTTON_L) {
        UnionChatLog_PressArrow(data, 0);
        if ((gSystem.newAndRepeatedKeys & PAD_BUTTON_L) && data->scroll) {
            --data->scroll;
            PlaySE(SEQ_SE_DP_BUTTON3);
        }
    } else if (held & PAD_BUTTON_R) {
        UnionChatLog_PressArrow(data, 1);
        if ((gSystem.newAndRepeatedKeys & PAD_BUTTON_R) && data->scroll < data->count - UNION_CHAT_LOG_NUM_ROWS) {
            ++data->scroll;
            PlaySE(SEQ_SE_DP_BUTTON3);
        }
    }
}

void UnionChatLog_HandleInput(UnionChatLogData *data) {
    int buttonHit;
    int scrollbarHit = -1;

    if (!FieldSystem_TaskIsRunning(data->fieldSystem)) {
        buttonHit = UnionChatLog_HandleTouchButtons(data);
        scrollbarHit = UnionChatLog_HandleTouchScrollbar(data);
        if (buttonHit == -1 && scrollbarHit == -1) {
            UnionChatLog_HandleScrollKeys(data);
        }
    }
    UnionChatLog_UpdateRowPress(data->bgConfig, data->screenData, data->rows, scrollbarHit - 1, data->log->count, &data->pressSum);
}

#pragma force_active on

// A pressed row slides down with its press timer; rows that are not held count back.
void UnionChatLog_UpdateRowPress(BgConfig *bgConfig, NNSG2dScreenData *screenData, UnionChatLogRow *rows, int heldRow, int count, int *pressSum) {
    int i;
    int total = 0;
    UnionChatLogRow *row;
    if (count > UNION_CHAT_LOG_NUM_ROWS) {
        count = UNION_CHAT_LOG_NUM_ROWS;
    }
    i = 0;
    if (count > 0) {
        row = rows;
        do {
            if (heldRow != i && row->pressTimer != 0) {
                row->pressTimer--;
            }
            total += row->pressTimer;
            i++;
            row++;
        } while (i < count);
    }
    if (total == 0 && *pressSum == 0) {
        *pressSum = total;
        return;
    }
    *pressSum = total;
    {
        int y;
        const void *screen;
        i = 0;
        if (count > 0) {
            screen = screenData->rawData;
            y = 2;
            do {
                int frame = rows->pressTimer / 2;
                CopyToBgTilemapRect(bgConfig, GF_BG_LYR_SUB_3, 0, y, 32, 7, screen, 0, frame * 8 + 24 * rows->gender, 32, 48);
                BgSetPosTextAndCommit(bgConfig, i + GF_BG_LYR_SUB_0, BG_POS_OP_SET_Y, sUnionChatLogRowPressBgOffsets[frame]);
                i++;
                rows++;
                y += 7;
            } while (i < count);
        }
    }
    DC_FlushRange(GetBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3), 0x600);
    BgCopyOrUncompressTilemapBufferRangeToVram(bgConfig, GF_BG_LYR_SUB_3, GetBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3), 0x600, 0);
}

#pragma force_active reset

// "{name}'s friend" under a message whose sender is in the Pal Pad: the player's own name when
// the sender registered the player, else the Pal Pad entry that knows the sender. NULL otherwise.
String *UnionChatLog_GetFriendOfText(SavePalPad *palPad, u32 trainerId, MessageFormat *msgFormat, MsgData *msgData, PlayerProfile *profile) {
    int status = 0;
    String *result = NULL;

    if (trainerId != PlayerProfile_GetTrainerID(profile)) {
        status = PalPad_PlayerIdIsFriendOrMutual(palPad, trainerId);
    }
    if (status > 0) {
        if (status == 1) {
            BufferPlayersName(msgFormat, 0, profile);
        } else if (status >= 2) {
            int index = status - 2;
            String *string = String_New(10, HEAP_ID_87);
            CopyU16ArrayToString(string, PalPad_GetNthEntry(palPad, index)->name);
            BufferString(msgFormat, 0, string, 0, 0, PalPadEntry_GetFromUnk68Array(palPad, index));
            String_Delete(string);
        }
        result = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0738_UNION_00208, HEAP_ID_87);
    }
    return result;
}

#pragma force_active on

void UnionChatLog_AddEntry(UnionChatLogData *data, u32 trainerId, MailMessage *mail, PlayerProfile *profile) {
    UnionChatLogMessages *log = data->log;
    SavePalPad *palPad = data->unionRoom->palPad;
    int *index;

    if (log->count == UNION_CHAT_LOG_NUM_MESSAGES) {
        index = &log->head;
    } else {
        index = &log->count;
    }
    if (log->slots[*index].string1 != NULL) {
        String_Delete(log->slots[*index].string1);
    }
    if (log->slots[*index].string2 != NULL) {
        String_Delete(log->slots[*index].string2);
    }
    CopyU16ArrayToString(log->slots[*index].string0, PlayerProfile_GetNamePtr(profile));
    log->slots[*index].message = *mail;
    log->slots[*index].unkC = trainerId;
    log->slots[*index].unk10 = PlayerProfile_GetTrainerGender(profile);
    log->slots[*index].string1 = MailMsg_GetExpandedString(mail, HEAP_ID_87);
    log->slots[*index].string2 = UnionChatLog_GetFriendOfText(palPad, trainerId, data->msgFormat, data->msgData, data->profile);
    (*index)++;
    if (log->head == UNION_CHAT_LOG_NUM_MESSAGES) {
        log->head = 0;
    }
}

#pragma force_active reset

BOOL UnionChatLog_IsNewMessage(UnionChatLogData *data, MailMessage *mail, u32 trainerId) {
    int i;

    if (!MailMsg_IsInit(mail)) {
        return FALSE;
    }
    for (i = 0; i < data->log->count; i++) {
        MessageSlot *slot = &data->log->slots[i];
        if (trainerId == slot->unkC && MailMsg_Compare(mail, &slot->message)) {
            break;
        }
    }
    if (i != data->log->count && data->log->count != 0) {
        return FALSE;
    }
    // The result of this comparison is not used.
    if (trainerId != data->log->slots[i].unkC) {
        MailMsg_Compare(mail, &data->log->slots[i].message);
    }
    return TRUE;
}

void UnionChatLog_AppendMessage(UnionChatLogData *data, PlayerProfile *profile, MailMessage *mail, u32 trainerId) {
    BOOL atEnd = FALSE;

    if (data->scroll == data->count - UNION_CHAT_LOG_NUM_ROWS) {
        atEnd = TRUE;
    }
    UnionChatLog_AddEntry(data, trainerId, mail, profile);
    if (data->scrollbarVisible != FALSE && atEnd) {
        data->scroll = data->log->count - UNION_CHAT_LOG_NUM_ROWS;
    }
    UnionChatLog_RequestRedraw(data);
}

void UnionChatLog_CollectMessages(UnionChatLogData *data) {
    int i;
    MailMessage *mail;

    if (!FieldSystem_TaskIsRunning(data->fieldSystem)) {
        for (i = 0; i < 16; i++) {
            UnionChatLogPeer *peer = sub_02035754(i);
            if (peer != NULL) {
                UnionChatLogPeerChat *chat = &peer->chat;
                MailMessage *message = &chat->message;
                if (UnionChatLog_IsNewMessage(data, message, chat->trainerId)) {
                    PlayerProfile *profile = sub_02035798(i);
                    UnionChatLog_AppendMessage(data, profile, message, chat->trainerId);
                }
            }
        }
        mail = sub_0205AA84(data->unk_08);
        if (mail != NULL) {
            UnionChatLog_AppendMessage(data, data->profile, mail, PlayerProfile_GetTrainerID(data->profile));
        }
    }
}

void UnionChatLog_RequestRedraw(UnionChatLogData *data) {
    data->redraw = TRUE;
}

void UnionChatLog_InitTouchRepeat(UnionChatLogData *data) {
    data->touchTick = FALSE;
    data->touchFirstDelay = 8;
    data->touchInterval = 4;
    data->touchTimer = data->touchFirstDelay;
}

void UnionChatLog_UpdateTouchRepeat(UnionChatLogData *data) {
    data->touchTick = FALSE;
    if (gSystem.touchNew != 0) {
        data->touchTick = TRUE;
        return;
    }
    if (gSystem.touchHeld != 0) {
        data->touchTimer--;
        if (data->touchTimer < 0) {
            data->touchTick = TRUE;
            data->touchTimer = data->touchInterval;
        }
    } else {
        data->touchTimer = data->touchFirstDelay;
    }
}

BOOL UnionChatLog_IsTouchRepeatTick(UnionChatLogData *data) {
    return data->touchTick;
}

void UnionChatLog_SetThumbFollowsScroll(UnionChatLogData *data, BOOL follow) {
    data->thumbFollowsScroll = follow;
}

BOOL UnionChatLog_GetThumbFollowsScroll(UnionChatLogData *data) {
    return data->thumbFollowsScroll;
}

void UnionChatLog_PressArrow(UnionChatLogData *data, int arrow) {
    int frame = Sprite_GetAnimationFrame(data->sprites[arrow]);
    u16 anim = (int)Sprite_GetAnimationNumber(data->sprites[arrow]); // the int cast keeps a 16-bit truncation
    if (frame != 0 || anim != arrow + 4) {
        Sprite_SetAnimCtrlSeq(data->sprites[arrow], arrow + 4);
    }
    data->arrowPressed[arrow] = TRUE;
}
