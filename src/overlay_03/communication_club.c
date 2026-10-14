#include "global.h"

#include "constants/comm_type.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

#include "field_system.h"
#include "overlay_01.h"
#include "overlay_03.h"
#include "render_text.h"
#include "task.h"
#include "text.h"
#include "sound_playback.h"
#include "communication_player_state.h"
#include "comm_wireless.h"
#include "communication_core.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "communication_club_link.h"

typedef struct CommClubManager CommClubManager;
typedef void (*CommClubManagerTaskFunc)(SysTask *, void *);
typedef void (*UnkFuncPtr_CommClub)(CommClubManager *);

struct CommClubManager {
    String *string[8];
    Window windows[3];
    MessageFormat *messageFormat[3];
    ListMenu *listMenu;
    u32 menu; // Menu?
    ListMenuItem *items;
    FieldSystem *fieldSystem;
    CommClubManagerTaskFunc commClubTask;
    UnkFuncPtr_CommClub unk70;
    MsgData *msgData;
    PlayerProfile *playerProfile[2];
    u8 playerListed[8];
    u32 retCode;
    u16 unk8C;
    u16 connectionIndex;
    u8 connectedCount;
    u8 commType;
    u8 unk92;
    u8 unk93;
    u8 printerID;
    u8 contactNetId;
    u8 messageDelay;
    u8 infoWindowOpen;
    u8 listNeedsRedraw;
    u8 unk99;
    u16 unk9A;
    const u32 *buttonMsgIds;
    u8 buttonCount;
    u8 buttonCloseRequest;
    u16 buttonSelection;
    u32 choiceResult;
    u32 buttonsShown;
};

static void CommClub_ParentListPrintRow(ListMenu *list, u16 cursorPos_unused, u8 itemsAbove);
static void CommClub_PrintOwnProfile(CommClubManager *commClubManager_unused);
static void CommClub_ParentListMoveCursor(struct ListMenu *list_unused, s32 index_unused, u8 onInit);
static void CommClub_ParentListPrintItems(struct ListMenu *list, s32 index_unused, u8 y_unused);
static void CommClub_CreateParentList();
static void CommClub_HandleParentList(void *task, CommClubManager *commClubManager);
static void CommClub_RedrawParentList(CommClubManager *commClubManager_unused);
static void CommClub_WaitConnected(void *task, CommClubManager *commClubManager);
static void CommClub_AskForRegistration(void *task, CommClubManager *commClubManager);
static BOOL CommClub_HandleHostReply(void *task, CommClubManager *commClubManager);
static void CommClub_WaitHostReply(void *task, CommClubManager *commClubManager);
static void CommClub_OpenLeaveGroupChoice(void *task, CommClubManager *commClubManager);
static void CommClub_HandleLeaveGroupChoice(void *task, CommClubManager *commClubManager);
static void CommClub_ExitLink();
static void CommClub_SetState(void *taskFunc);
static void CommClub_Task(SysTask *task, void *data);
static void CommClub_CreateMemberList(s32 arg0_unused, CommClubManager *commClubManager);
static void CommClub_HandleMemberList(void *task, CommClubManager *commClubManager);
static void CommClub_WaitDifferentRulesAck(void *task, CommClubManager *commClubManager);
static void CommClub_WaitProblemAckThenReopen(void *task, CommClubManager *commClubManager);
static void CommClub_ReopenMemberList(void *arg0_unused, CommClubManager *commClubManager);
static void CommClub_ConfirmGroup(void *arg0_unused, CommClubManager *commClubManager);
static void CommClub_OpenConfirmGroupChoice(void *arg0_unused, CommClubManager *commClubManager);
static void CommClub_HandleConfirmGroupChoice(void *task, CommClubManager *commClubManager);
static void CommClub_GroupCancelled(void *task, CommClubManager *commClubManager);
static void CommClub_WaitGroupCancelledAck(SysTask *task, void *commClubManager_unused);
static void CommClub_AskCancelGroup(void *task, CommClubManager *commClubManager);
static void CommClub_OpenCancelGroupChoice(void *arg0_unused, CommClubManager *commClubManager);
static void CommClub_HandleCancelGroupChoice(void *task, CommClubManager *commClubManager);
static void CommClub_WaitGroupSync(void *task, CommClubManager *commClubManager);
static void CommClub_OpenContactChoice(s32 arg0_unused, CommClubManager *commClubManager);
static void CommClub_CloseMenus(void *task, CommClubManager *commClubManager);
static void CommClub_HostRepliedOk(CommClubManager *commClubManager);
static void CommClub_ChildAccepted(CommClubManager *commClubManager);
static void CommClub_WaitGroupStart(u32 arg0_unused, CommClubManager *commClubManager);
static void CommClub_WaitGroupSyncAfterReply(void *task, CommClubManager *commClubManager);
static void CommClub_ShowProblem();
static void CommClub_WaitProblemAck(SysTask *task);
static void CommClub_ShowRefused(CommClubManager *commClubManager);
static void CommClub_WaitRefusedThenParentList(SysTask *task, void *commClubManager);
static void CommClub_ResetToParentList(void *commClubManager);
static void CommClub_WaitAckThenParentList(SysTask *task, void *commClubManager);
static void CommClub_FinishGroup(CommClubManager *commClubManager_unused);
static int CommClub_GetMinPlayers();
static int CommClub_GetMaxPlayers();
static int CommClub_GetBeaconId();
static void CommClub_PrintAwaitingContact();
static BOOL CommClub_HasErrorDummy();
static void CommClub_OpenYesNoButtons(CommClubManager *commClubManager);
static BOOL CommClub_InitYesNoButtons(CommClubManager *commClubManager);
static BOOL CommClub_IsYesNoReady(CommClubManager *commClubManager);
static void CommClub_CloseYesNoButtons(CommClubManager *commClubManager);

static CommClubManager *sCommClubManager = NULL;

static const u32 sRemoveFromMembersButton = 335;
static const u32 sQuitButton = 334;

static const u32 sStartQuitButtons[2] = {
    336,
    334
};

static const u32 sJoinQuitButtons[2] = {
    333,
    334
};

static const ListMenuTemplate sParentListTemplate = {
    .items = NULL,
    .moveCursorFunc = CommClub_ParentListMoveCursor,
    .itemPrintFunc = CommClub_ParentListPrintItems,
    .window = NULL,
    .totalItems = 16,
    .maxShowed = 5,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 15,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 1,
    .fontId = 0,
    .cursorKind = 0,
    .data = NULL
};

static const ListMenuTemplate sMemberListTemplate = {
    .items = NULL,
    .moveCursorFunc = NULL,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 5,
    .maxShowed = 5,
    .header_X = 0,
    .item_X = 1,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 15,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 1,
    .fontId = 0,
    .cursorKind = 1,
    .data = NULL
};

static void CommClub_PrintMessage(s32 strno, BOOL expandPlaceholders) {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID) == FALSE) {
        RemoveTextPrinter(sCommClubManager->printerID);
    }
    if (expandPlaceholders) {
        ReadMsgDataIntoString(sCommClubManager->msgData, strno, sCommClubManager->string[4]);
        StringExpandPlaceholders(sCommClubManager->messageFormat[2], sCommClubManager->string[5], sCommClubManager->string[4]);
    } else {
        ReadMsgDataIntoString(sCommClubManager->msgData, strno, sCommClubManager->string[5]);
    }
    if (WindowIsInUse(&sCommClubManager->windows[2]) == FALSE) {
        DialogBox_AddWindowToLayer3(sCommClubManager->fieldSystem->bgConfig, &sCommClubManager->windows[2], GF_BG_LYR_MAIN_3);
    }
    DialogBox_LoadFrame(&sCommClubManager->windows[2], Save_PlayerData_GetOptionsAddr(sCommClubManager->fieldSystem->saveData));
    sCommClubManager->printerID = DialogBox_PrintMessage(&sCommClubManager->windows[2], sCommClubManager->string[5], Save_PlayerData_GetOptionsAddr(sCommClubManager->fieldSystem->saveData), TRUE);
}

static void CommClub_CreateListMenu(ListMenuTemplate listMenuTemplate, u8 x, u8 y, u8 width, u8 height, u16 baseTile) {
    if (WindowIsInUse(&sCommClubManager->windows[0]) == FALSE) {
        AddWindowParameterized(sCommClubManager->fieldSystem->bgConfig, &sCommClubManager->windows[0], 3, x, y, width, height, 13, baseTile);
    }
    DrawFrameAndWindow1(&sCommClubManager->windows[0], TRUE, 985, HEAP_ID_FIELD2);
    ListMenuTemplate temp = listMenuTemplate;
    temp.items = sCommClubManager->items;
    temp.window = &sCommClubManager->windows[0];
    sCommClubManager->listMenu = ListMenuInit(&temp, 0, 0, HEAP_ID_FIELD1);
    CopyWindowToVram(&sCommClubManager->windows[0]);
}

static void CommClub_NewManager(FieldSystem *fieldSystem) {
    if (sCommClubManager) {
        GF_AssertFail();
    }
    CommClubManager *commClubManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(CommClubManager));
    sCommClubManager = commClubManager;
    MI_CpuFill8(commClubManager, 0, sizeof(CommClubManager));
    sCommClubManager->retCode = 0;
    sCommClubManager->fieldSystem = fieldSystem;
    sCommClubManager->infoWindowOpen = 0;
    sCommClubManager->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0182_bin, HEAP_ID_FIELD1);
    sCommClubManager->playerProfile[0] = Save_PlayerData_GetProfile(FieldSystem_GetSaveData(sCommClubManager->fieldSystem));
    sCommClubManager->playerProfile[1] = PlayerProfile_New(HEAP_ID_FIELD1);
    InitWindow(&sCommClubManager->windows[0]);
    InitWindow(&sCommClubManager->windows[1]);
    InitWindow(&sCommClubManager->windows[2]);
    sCommClubManager->messageFormat[0] = MessageFormat_New(HEAP_ID_FIELD1);
    sCommClubManager->messageFormat[1] = MessageFormat_New(HEAP_ID_FIELD1);
    sCommClubManager->messageFormat[2] = MessageFormat_New(HEAP_ID_FIELD1);
    sCommClubManager->listNeedsRedraw = 0;
    int i;
    for (i = 0; i < 8; i++) {
        sCommClubManager->playerListed[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        sCommClubManager->string[i] = String_New(140, HEAP_ID_FIELD1);
    }
}

static void CommClub_DeleteManager() {
    for (int i = 0; i < 8; i++) {
        String_Delete(sCommClubManager->string[i]);
    };

    Heap_Free(sCommClubManager->playerProfile[1]);

    if (sCommClubManager->messageFormat[0]) {
        MessageFormat_Delete(sCommClubManager->messageFormat[0]);
        sCommClubManager->messageFormat[0] = NULL;
    }
    if (sCommClubManager->messageFormat[1]) {
        MessageFormat_Delete(sCommClubManager->messageFormat[1]);
        sCommClubManager->messageFormat[1] = NULL;
    }
    if (sCommClubManager->messageFormat[2]) {
        MessageFormat_Delete(sCommClubManager->messageFormat[2]);
        sCommClubManager->messageFormat[2] = NULL;
    }

    DestroyMsgData(sCommClubManager->msgData);

    if (sCommClubManager->windows[2].pixelBuffer) {
        RemoveWindow(&sCommClubManager->windows[2]);
    }

    Heap_Free(sCommClubManager);
    sCommClubManager = NULL;
}

static void CommClub_StartChild(CommClubManager *commClubManager) {
    CommClubLink_StartChild(commClubManager->fieldSystem, commClubManager->commType, CommClub_GetBeaconId());
}

static void CommClub_ParentListMoveCursor(struct ListMenu *list_unused, s32 index_unused, u8 onInit) {
    sCommClubManager->listNeedsRedraw = 1;
    if (onInit == FALSE) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

static void CommClub_ParentListPrintItems(struct ListMenu *list, s32 index_unused, u8 y_unused) {
    for (u32 i = 0; i < ListMenuGetTemplateField(list, LISTMENUATTR_MAXSHOWED); i++) {
        CommClub_ParentListPrintRow(list, 0, i);
    };
}

static void CommClub_ParentListPrintRow(ListMenu *list, u16 cursorPos_unused, u8 itemsAbove) {
    s32 res = CommWireless_CountParents();
    u16 cursorPos = 0;
    if (sCommClubManager->listMenu) {
        ListMenuGetScrollAndRow(sCommClubManager->listMenu, &cursorPos, NULL);
    }
    cursorPos += itemsAbove;
    FillWindowPixelRect(&sCommClubManager->windows[0], 15, 8, itemsAbove * 16, (GetWindowWidth(&sCommClubManager->windows[0]) * 8) - 8, 16);
    if (cursorPos < res) {
        CommWireless_CopyParentProfile(cursorPos, sCommClubManager->playerProfile[1]);
        BufferIntegerAsString(sCommClubManager->messageFormat[0], 0, cursorPos + 1, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        BufferPlayersName(sCommClubManager->messageFormat[0], 1, sCommClubManager->playerProfile[1]);
        ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_00076, sCommClubManager->string[1]); // {STRVAR_1 51, 0, 0}:{STRVAR_1 3, 1, 0}
        StringExpandPlaceholders(sCommClubManager->messageFormat[0], sCommClubManager->string[0], sCommClubManager->string[1]);
        AddTextPrinterParameterized(&sCommClubManager->windows[0], 0, sCommClubManager->string[0], 8, itemsAbove * 16, TEXT_SPEED_NOTRANSFER, NULL);
        BufferIntegerAsString(sCommClubManager->messageFormat[0], 2, PlayerProfile_GetTrainerID(sCommClubManager->playerProfile[1]) % 0x10000, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_ID_1, sCommClubManager->string[3]); // ID{STRVAR_1 54, 2, 0}
        StringExpandPlaceholders(sCommClubManager->messageFormat[0], sCommClubManager->string[2], sCommClubManager->string[3]);
        AddTextPrinterParameterized(&sCommClubManager->windows[0], 0, sCommClubManager->string[2], 88, itemsAbove * 16, TEXT_SPEED_NOTRANSFER, NULL);
    } else {
        BufferIntegerAsString(sCommClubManager->messageFormat[0], 0, cursorPos + 1, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_00077, sCommClubManager->string[1]); // {STRVAR_1 51, 0, 0}:
        StringExpandPlaceholders(sCommClubManager->messageFormat[0], sCommClubManager->string[0], sCommClubManager->string[1]);
        AddTextPrinterParameterized(&sCommClubManager->windows[0], 0, sCommClubManager->string[0], 8, itemsAbove * 16, TEXT_SPEED_NOTRANSFER, NULL);
    }
}

static void CommClub_ShowParentList(CommClubManager *commClubManager) {
    if (WindowIsInUse(&sCommClubManager->windows[1]) == FALSE) {
        AddWindowParameterized(sCommClubManager->fieldSystem->bgConfig, &sCommClubManager->windows[1], 3, 23, 2, 8, 4, 13, 201);
    }
    DrawFrameAndWindow1(&sCommClubManager->windows[1], TRUE, 985, HEAP_ID_FIELD2);
    FillWindowPixelBuffer(&sCommClubManager->windows[1], 15);
    CopyWindowToVram(&sCommClubManager->windows[1]);
    commClubManager->infoWindowOpen = 1;
    SysTask_CreateOnMainQueue(CommClub_Task, commClubManager, 0);
    CommClub_PrintOwnProfile(sCommClubManager);

    u16 sChooseText[NUM_COMM_TYPES] = {
        CommunicationClub_Text_00048_Dummy,
        CommunicationClub_Text_Choose_SingleBattle, // Choose the Single Battle you would like to join.
        CommunicationClub_Text_Choose_DoubleBattle, // Choose the Double Battle you would like to join.
        CommunicationClub_Text_Choose_MixBattle,    // Choose the Mix Battle you would like to join.
        CommunicationClub_Text_Choose_MultiBattle,  // Choose the Multi Battle you would like to join.
        CommunicationClub_Text_Choose_MultiBattle,  // Choose the Multi Battle you would like to join.
        CommunicationClub_Text_00056_Dummy,
        CommunicationClub_Text_00057_Dummy,
        CommunicationClub_Text_00058_Dummy, // Unreachable.
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_ChooseFriend_BattleTower, // Choose the friend you want to take to the Battle Tower.
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_ChooseFriend_BattleFactory_Lv50, // Choose the friend to play with at the Battle Factory--Level 50.
        CommunicationClub_Text_ChooseFriend_BattleFactory_Open, // Choose the friend to play with at the Battle Factory--Open Level.
        0,
        CommunicationClub_Text_ChooseFriend_BattleHall,   // Choose the friend to play with at the Battle Hall.
        CommunicationClub_Text_ChooseFriend_BattleCastle, // Choose the friend to play with at the Battle Castle.
        CommunicationClub_Text_ChooseFriend_BattleArcade, // Choose the friend to play with at the Battle Arcade.
        0,
        0,
        0,
        0,
        CommunicationClub_Text_Choose_FlatBattle,     // Choose the Flat Battle you would like to join.
        CommunicationClub_Text_Choose_FlatBattle,     // Choose the Flat Battle you would like to join.
        CommunicationClub_Text_Choose_SafariZoneLink, // Choose the Safari Zone Link you would like to join.
        CommunicationClub_Text_Choose_Pokeathlon      // Unreachable.
    };

    if (sCommClubManager->commType == COMM_TYPE_8) {
        CommClub_PrintMessage(sCommClubManager->unk92 + 122, FALSE);
    } else if (sCommClubManager->commType == COMM_TYPE_POKEATHLON) {
        CommClub_PrintMessage(sCommClubManager->unk92 + CommunicationClub_Text_ChooseLeader_JumpCourse, FALSE);
    } else {
        CommClub_PrintMessage(sChooseText[sCommClubManager->commType], FALSE);
    }
    CommClub_SetState(&CommClub_CreateParentList);
}

static void CommClub_PrintOwnProfile(CommClubManager *commClubManager_unused) {
    BufferPlayersName(sCommClubManager->messageFormat[1], 0, sCommClubManager->playerProfile[0]);
    BufferIntegerAsString(sCommClubManager->messageFormat[1], 1, PlayerProfile_GetTrainerID(sCommClubManager->playerProfile[0]) % 0x10000, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_ID_2, sCommClubManager->string[6]); // {STRVAR_1 3, 0, 0}\nID{STRVAR_1 54, 1, 0}
    StringExpandPlaceholders(sCommClubManager->messageFormat[1], sCommClubManager->string[7], sCommClubManager->string[6]);
    AddTextPrinterParameterized(&sCommClubManager->windows[1], 0, sCommClubManager->string[7], 2, 2, 0, NULL);
}

static void CommClub_CreateParentList() {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        sCommClubManager->items = ListMenuItems_New(16, HEAP_ID_FIELD1);
        for (int i = 0; i < 16; i++) {
            BufferIntegerAsString(sCommClubManager->messageFormat[0], 0, i + 1, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
            ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_00077, sCommClubManager->string[1]); // {STRVAR_1 51, 0, 0}:
            StringExpandPlaceholders(sCommClubManager->messageFormat[0], sCommClubManager->string[0], sCommClubManager->string[1]);
            ListMenuItems_AddItem(sCommClubManager->items, sCommClubManager->string[0], 0);
        };

        CommClub_CreateListMenu(sParentListTemplate, 1, 2, 20, 10, 1);

        sCommClubManager->buttonMsgIds = sJoinQuitButtons;
        sCommClubManager->buttonCount = 2;
        sCommClubManager->buttonSelection = 0xFFFF;
        sCommClubManager->buttonCloseRequest = 0;
        FieldSystem_SwitchBottomScreenApp(sCommClubManager->fieldSystem, 7, &sCommClubManager->buttonMsgIds);
        CommClub_SetState(&CommClub_HandleParentList);
    }
}

static void CommClub_HandleParentList(void *task, CommClubManager *commClubManager) {
    CommClub_PrintOwnProfile(commClubManager);
    if (FieldSystem_GetBottomScreenAppState(commClubManager->fieldSystem) == TRUE) {
        if (Comm_HasLinkError()) {
            sCommClubManager->retCode = 3;
            CommClub_ExitLink();
            CommClub_CloseMenus(task, commClubManager);
            return;
        }
        int listInput;
        if (CommWireless_IsParentListChanged() == 0) {
            listInput = ListMenu_ProcessInput(sCommClubManager->listMenu);
            if (listInput == -1) {
                if (commClubManager->buttonSelection == 0) {
                    u16 currentItem;
                    ListMenuGetCurrentItemArrayId(sCommClubManager->listMenu, &currentItem);
                    listInput = currentItem;
                } else if (commClubManager->buttonSelection == 1) {
                    listInput = -2;
                }
            }
        } else {
            listInput = -1;
        }
        switch (listInput) {
        case -1:
            CommClub_RedrawParentList(commClubManager);
            break;

        case -2:
            PlaySE(SEQ_SE_DP_SELECT);
            sCommClubManager->retCode = 1;
            CommClub_ExitLink();
            CommClub_CloseMenus(task, commClubManager);
            break;
        default:
            PlaySE(SEQ_SE_DP_SELECT);
            ListMenuGetCurrentItemArrayId(commClubManager->listMenu, &commClubManager->connectionIndex);
            if (CommWireless_CountParents() > commClubManager->connectionIndex) {
                CommClub_RedrawParentList(commClubManager);
                commClubManager->connectionIndex = CommWireless_GetParentSlot(commClubManager->connectionIndex);
                CommClubLink_ConnectToParent(commClubManager->connectionIndex);
                LinkSync_ClearValues();
                CommClub_SetState(&CommClub_WaitConnected);
            }
        }
    }
}

static void CommClub_RedrawParentList(CommClubManager *commClubManager_unused) {
    if (CommWireless_IsParentListChanged() || sCommClubManager->listNeedsRedraw) {
        sCommClubManager->listNeedsRedraw = 0;
        CommWireless_ClearParentListChanged();
        FillWindowPixelRect(&sCommClubManager->windows[0], 15, 8, 0, 12, 80);
        CommClub_ParentListPrintItems(sCommClubManager->listMenu, LISTMENUATTR_MOVECURSORFUNC, 0);
        CopyWindowToVram(&sCommClubManager->windows[0]);
    }
}

static void CommClub_WaitConnected(void *task, CommClubManager *commClubManager) {
    if (Comm_HasLinkError()) {
        CommClub_CloseMenus(task, commClubManager);
        CommClub_PrintMessage(0, FALSE);
        CommClub_ResetToParentList(commClubManager);
        sCommClubManager->buttonCloseRequest = 1;
    } else {
        if (CommPlayerInfo_GetProfile(Comm_GetLocalNetId())) {
            LinkSync_ClearValues();
            CommClub_AskForRegistration(task, commClubManager);
        }
    }
}

static void CommClub_AskForRegistration(void *task, CommClubManager *commClubManager) {
    if (Comm_HasLinkError()) {
        CommClub_CloseMenus(task, commClubManager);
        CommClub_PrintMessage(0, FALSE);
        CommClub_ResetToParentList(commClubManager);
        sCommClubManager->buttonCloseRequest = 1;
        return;
    }
    CommWireless_CopyParentProfile(commClubManager->connectionIndex, sCommClubManager->playerProfile[1]);
    BufferPlayersName(commClubManager->messageFormat[2], 1, sCommClubManager->playerProfile[1]);
    if (CommClub_GetMinPlayers() <= 2) {
        CommClub_PrintMessage(1, TRUE);
    } else {
        if (DialogBox_IsPrintFinished(sCommClubManager->printerID) == FALSE) {
            RemoveTextPrinter(sCommClubManager->printerID);
        }
        ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_AskingForRegistration, sCommClubManager->string[4]); // Asking {STRVAR_1 3, 1, 0} to do member\nregistration. Please wait.
        StringExpandPlaceholders(sCommClubManager->messageFormat[2], sCommClubManager->string[5], sCommClubManager->string[4]);
        if (WindowIsInUse(&sCommClubManager->windows[2]) == FALSE) {
            DialogBox_AddWindowToLayer3(sCommClubManager->fieldSystem->bgConfig, &sCommClubManager->windows[2], GF_BG_LYR_MAIN_3);
        }
        DialogBox_LoadFrame(&sCommClubManager->windows[2], Save_PlayerData_GetOptionsAddr(sCommClubManager->fieldSystem->saveData));
        TextFlags_SetCanABSpeedUpPrint(TRUE);
        TextFlags_SetAutoScrollParam(0);
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        sCommClubManager->printerID = AddTextPrinterParameterized(&sCommClubManager->windows[2], 1, sCommClubManager->string[5], 0, 0, 1, NULL);
    }
    sCommClubManager->buttonsShown = 0;
    sCommClubManager->buttonCloseRequest = 1;
    CommClub_SetState(&CommClub_WaitHostReply);
}

static BOOL CommClub_HandleHostReply(void *task, CommClubManager *commClubManager) {
    if (Comm_HasLinkError()) {
        CommClub_CloseMenus(task, commClubManager);
        CommClub_PrintMessage(0, FALSE);
        CommClub_ResetToParentList(commClubManager);
    } else if (LinkSync_GetValue(0, 3) == Comm_GetLocalNetId()) {
        LinkSync_ClearValues();
        CommClub_CloseMenus(task, commClubManager);
        CommClub_ShowRefused(commClubManager);
    } else if (LinkSync_GetValue(0, 5) != -1) {
        LinkSync_ClearValues();
        CommClub_CloseMenus(task, commClubManager);
        CommClub_ShowRefused(commClubManager);
    } else if (LinkSync_GetValue(0, 4) == Comm_GetLocalNetId()) {
        CommClub_PrintMessage(116, FALSE);
        CommClub_SetState(&CommClub_WaitDifferentRulesAck);
    } else if (LinkSync_GetValue(0, 2) == Comm_GetLocalNetId()) {
        LinkSync_ClearValues();
        commClubManager->connectedCount = CommPlayerInfo_CountAccepted();
        if (DialogBox_IsPrintFinished(sCommClubManager->printerID) == FALSE) {
            RemoveTextPrinter(sCommClubManager->printerID);
        }
        CommClub_CloseMenus(task, commClubManager);
        CommClub_ChildAccepted(commClubManager);
    } else if (CommClub_HasErrorDummy()) {
        CommClub_CloseMenus(task, commClubManager);
        CommClub_ShowRefused(commClubManager);
    } else {
        return FALSE;
    }
    return TRUE;
}

static void CommClub_WaitHostReply(void *task, CommClubManager *unk_ovo3) {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        if (unk_ovo3->buttonsShown == 0) {
            sCommClubManager->buttonMsgIds = &sRemoveFromMembersButton;
            sCommClubManager->buttonCount = 1;
            sCommClubManager->buttonSelection = 0xFFFF;
            sCommClubManager->buttonCloseRequest = 0;
            FieldSystem_SwitchBottomScreenApp(sCommClubManager->fieldSystem, 7, &sCommClubManager->buttonMsgIds);
            unk_ovo3->buttonsShown = 1;
        } else if (FieldSystem_GetBottomScreenAppState(unk_ovo3->fieldSystem) == TRUE) {
            if (CommClub_HandleHostReply(task, unk_ovo3)) {
                unk_ovo3->buttonCloseRequest = 2;
            } else if (PAD_BUTTON_B & gSystem.newKeys || unk_ovo3->buttonSelection == 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                unk_ovo3->buttonsShown = 0;
                CommClub_PrintMessage(3, FALSE);
                CommClub_SetState(&CommClub_OpenLeaveGroupChoice);
            }
        }
    }
}

static void CommClub_OpenLeaveGroupChoice(void *task, CommClubManager *commClubManager) {
    if (CommClub_HandleHostReply(task, commClubManager)) {
        commClubManager->buttonCloseRequest = 2;
        return;
    }
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        CommClub_OpenYesNoButtons(commClubManager);
        CommClub_SetState(&CommClub_HandleLeaveGroupChoice);
    }
}

static void CommClub_HandleLeaveGroupChoice(void *task, CommClubManager *commClubManager) {
    if (commClubManager->buttonsShown == 0) {
        if (CommClub_InitYesNoButtons(commClubManager) == TRUE) {
            commClubManager->buttonsShown = 1;
        } else {
            return;
        }
    }
    if (CommClub_IsYesNoReady(commClubManager)) {
        if (CommClub_HandleHostReply(task, commClubManager)) {
            CommClub_CloseYesNoButtons(commClubManager);
        } else if (FieldSystem_GetTouchscreenChoiceMenuState(commClubManager->fieldSystem) == 6) {
            if (commClubManager->choiceResult == 0) {
                CommClub_CloseMenus(task, commClubManager);
                CommClub_PrintMessage(6, FALSE);
                CommClub_ResetToParentList(commClubManager);
            } else if (commClubManager->choiceResult == 1) {
                CommClub_SetState(&CommClub_AskForRegistration);
            }
        }
    }
}

static void CommClub_MemberListPrintRow(ListMenu *listMenu_unused, u16 arg1_unused, u8 row) {
    u16 playerIndex = 0;
    if (sCommClubManager->listMenu) {
        ListMenuGetScrollAndRow(sCommClubManager->listMenu, &playerIndex, NULL);
    }
    playerIndex += row;
    if (CommPlayerInfo_GetProfile(playerIndex)) {
        BufferPlayersName(sCommClubManager->messageFormat[0], 0, CommPlayerInfo_GetProfile(playerIndex));
        ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_00079, sCommClubManager->string[1]); // {STRVAR_1 3, 0, 0}
        StringExpandPlaceholders(sCommClubManager->messageFormat[0], sCommClubManager->string[0], sCommClubManager->string[1]);
        s32 rowY = row * 16;
        AddTextPrinterParameterized(&sCommClubManager->windows[0], 0, sCommClubManager->string[0], 8, rowY, 0, NULL);
        BufferIntegerAsString(sCommClubManager->messageFormat[0], 2, PlayerProfile_GetTrainerID_VisibleHalf(CommPlayerInfo_GetProfile(playerIndex)), 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(sCommClubManager->msgData, CommunicationClub_Text_ID_1, sCommClubManager->string[3]); // ID{STRVAR_1 54, 2, 0}
        StringExpandPlaceholders(sCommClubManager->messageFormat[0], sCommClubManager->string[2], sCommClubManager->string[3]);
        AddTextPrinterParameterized(&sCommClubManager->windows[0], 0, sCommClubManager->string[2], 72, rowY, 0, NULL);
    }
}

static void CommClub_StartParent(CommClubManager *commClubManager) {
    CommClubLink_StartParent(commClubManager->fieldSystem, commClubManager->commType, CommClub_GetBeaconId());
}

static void CommClub_ExitLink() {
    CommClubLink_Exit();
}

static void CommClub_SetState(void *taskFunc) {
    sCommClubManager->commClubTask = taskFunc;
}

static void CommClub_StartHosting(CommClubManager *commClubManager) {
    SysTask_CreateOnMainQueue(&CommClub_Task, commClubManager, 0);
    CommClub_PrintAwaitingContact();
    if (WindowIsInUse(&sCommClubManager->windows[1]) == FALSE) {
        AddWindowParameterized(sCommClubManager->fieldSystem->bgConfig, &sCommClubManager->windows[1], 3, 22, 2, 9, 4, 13, 205);
    }
    DrawFrameAndWindow1(&sCommClubManager->windows[1], TRUE, 985, 11);
    FillWindowPixelBuffer(&sCommClubManager->windows[1], 15);
    CopyWindowToVram(&sCommClubManager->windows[1]);
    commClubManager->infoWindowOpen = 1;
    CommPlayerInfo_Accept(0);
    CommClub_SetState(&CommClub_CreateMemberList);
}

static void CommClub_PrintAwaitingContact() {
    u16 sAwaitingContactText[NUM_COMM_TYPES] = {
        CommunicationClub_Text_00007_Dummy,
        CommunicationClub_Text_AwaitingContact_SingleBattle, // Single Battle! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_DoubleBattle, // Double Battle! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_MixBattle,    // Mix Battle! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_MultiBattle,  // Multi Battle! Awaiting contact from your friends.
        CommunicationClub_Text_AwaitingContact_MultiBattle,  // Multi Battle! Awaiting contact from your friends.
        CommunicationClub_Text_ProceedWhenReady,             // When the group is ready: A Button: Proceed   B Button: Cancel
        CommunicationClub_Text_00016_Dummy,
        CommunicationClub_Text_ProceedWhenReady_2, // When the group is ready: A Button: Proceed   B Button: Cancel
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_AwaitingContact_BattleTower, // Battle Tower! Awaiting contact from your friend.
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_AwaitingContact_BattleFactory_Lv50, // Battle Factory--Level 50! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_BattleFactory_Open, // Battle Factory--Open Level! Awaiting contact from your friend.
        0,
        CommunicationClub_Text_AwaitingContact_BattleHall,   // Battle Hall! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_BattleCastle, // Battle Castle! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_BattleArcade, // Battle Arcade! Awaiting contact from your friend.
        0,
        0,
        0,
        0,
        CommunicationClub_Text_AwaitingContact_FlatBattle,     // Flat Battle! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_FlatBattle,     // Flat Battle! Awaiting contact from your friend.
        CommunicationClub_Text_AwaitingContact_SafariZoneLink, // Safari Zone Link! Awaiting contact from your friend.
        0
    };

    if (sCommClubManager->commType >= NUM_COMM_TYPES) {
        GF_AssertFail();
    }

    if (sCommClubManager->commType == COMM_TYPE_8) {
        if (CommPlayerInfo_CountAccepted() > 1) {
            CommClub_PrintMessage(sCommClubManager->unk92 + 127, FALSE);
        } else {
            CommClub_PrintMessage(sCommClubManager->unk92 + 117, FALSE);
        }
    } else if (sCommClubManager->commType == COMM_TYPE_POKEATHLON) {
        if (CommPlayerInfo_CountAccepted() > 1) {
            CommClub_PrintMessage(sCommClubManager->unk92 + 154, FALSE);
        } else {
            CommClub_PrintMessage(sCommClubManager->unk92 + 142, FALSE);
        }
    } else {
        CommClub_PrintMessage(sAwaitingContactText[sCommClubManager->commType], FALSE);
    }
}

static void CommClub_PrintPlayerCount(CommClubManager *commClubManager_unused) {
    s32 num;
    s32 strno;

    if (CommPlayerInfo_CountAccepted() >= CommClub_GetMinPlayers()) {
        num = CommPlayerInfo_CountAccepted();
        strno = CommunicationClub_Text_NumPlayers; // {STRVAR_1 50, 0, 0} Players
    } else {
        num = CommClub_GetMinPlayers() - CommPlayerInfo_CountAccepted();
        strno = CommunicationClub_Text_NumNeeded; // {STRVAR_1 50, 0, 0} more needed.
    }
    BufferIntegerAsString(sCommClubManager->messageFormat[1], 0, num, 2, PRINTING_MODE_5_UNUSED, TRUE);
    FillWindowPixelBuffer(&sCommClubManager->windows[1], 15);
    ReadMsgDataIntoString(sCommClubManager->msgData, strno, sCommClubManager->string[7]);
    StringExpandPlaceholders(sCommClubManager->messageFormat[1], sCommClubManager->string[6], sCommClubManager->string[7]);
    AddTextPrinterParameterized(&sCommClubManager->windows[1], 0, sCommClubManager->string[6], 2, 2, 0, NULL);
}

static void CommClub_PrintContacted(u32 netId, CommClubManager *commClubManager) {
    u16 sContactedText[NUM_COMM_TYPES] = {
        CommunicationClub_Text_00024_Dummy,
        CommunicationClub_Text_Contacted_SingleBattle, // {STRVAR_1 3, 1, 0} has contacted you. Start a Single Battle?
        CommunicationClub_Text_Contacted_DoubleBattle, // {STRVAR_1 3, 1, 0} has contacted you. Start a Double Battle?
        CommunicationClub_Text_Contacted_MixBattle,    // {STRVAR_1 3, 1, 0} has contacted you. Start a Mix Battle?
        CommunicationClub_Text_Contacted_MultiBattle,  // {STRVAR_1 3, 1, 0} has contacted you. Let {STRVAR_1 3, 1, 0} join your group?
        CommunicationClub_Text_Contacted_MultiBattle,  // {STRVAR_1 3, 1, 0} has contacted you. Let {STRVAR_1 3, 1, 0} join your group?
        CommunicationClub_Text_00037_Dummy,
        CommunicationClub_Text_Contacted_MultiBattle, // {STRVAR_1 3, 1, 0} has contacted you. Let {STRVAR_1 3, 1, 0} join your group?
        CommunicationClub_Text_Contacted_MultiBattle, // {STRVAR_1 3, 1, 0} has contacted you. Let {STRVAR_1 3, 1, 0} join your group?
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_Contacted_BattleTower, // {STRVAR_1 3, 1, 0} has contacted you. Take Battle Tower challenge?
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_Contacted_BattleFactory, // {STRVAR_1 3, 1, 0} has contacted you. Take Battle Factory challenge?
        CommunicationClub_Text_Contacted_BattleFactory, // {STRVAR_1 3, 1, 0} has contacted you. Take Battle Factory challenge?
        0,
        CommunicationClub_Text_Contacted_BattleHall,   // {STRVAR_1 3, 1, 0} has contacted you. Start the Battle Hall challenge?
        CommunicationClub_Text_Contacted_BattleCastle, // {STRVAR_1 3, 1, 0} has contacted you. Take Battle Castle challenge?
        CommunicationClub_Text_Contacted_BattleArcade, // {STRVAR_1 3, 1, 0} has contacted you. Take Battle Arcade challenge?
        0,
        0,
        0,
        0,
        CommunicationClub_Text_Contacted_FlatBattle,     // {STRVAR_1 3, 1, 0} has contacted you. Start a Flat Battle?
        CommunicationClub_Text_Contacted_FlatBattle,     // {STRVAR_1 3, 1, 0} has contacted you. Start a Flat Battle?
        CommunicationClub_Text_Contacted_SafariZoneLink, // {STRVAR_1 3, 1, 0} has contacted you. Start a Safari Zone Link?
        CommunicationClub_Text_Contacted_Pokeathlon      // {STRVAR_1 3, 1, 0} has contacted you. Let {STRVAR_1 3, 1, 0} join your group?
    };

    PlayerProfile *profile = CommPlayerInfo_GetProfile(netId);
    if (profile) {
        BufferPlayersName(commClubManager->messageFormat[2], 1, profile);
    }
    CommClub_PrintMessage(sContactedText[commClubManager->commType], TRUE);
}

static void CommClub_Task(SysTask *task, void *data) {
    if (sCommClubManager->commClubTask) {
        sCommClubManager->commClubTask(task, data);
    }
}

static void CommClub_CreateMemberList(s32 arg0_unused, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        sCommClubManager->items = ListMenuItems_New(5, HEAP_ID_FIELD1);
        int i;
        for (i = 0; i < 5; i++) {
            ListMenuItems_AppendFromMsgData(sCommClubManager->items, sCommClubManager->msgData, 82, 0);
        }
        sCommClubManager->listNeedsRedraw = 1;
        for (i = 0; i < 8; i++) {
            sCommClubManager->playerListed[i] = 0;
        }

        CommClub_CreateListMenu(sMemberListTemplate, 1, 2, 17, 12, 1);
        CommClub_SetState(&CommClub_HandleMemberList);
        if (CommClub_GetMaxPlayers() > 2) {
            sCommClubManager->buttonMsgIds = sStartQuitButtons;
            sCommClubManager->buttonCount = 2;
        } else {
            sCommClubManager->buttonMsgIds = &sQuitButton;
            sCommClubManager->buttonCount = 1;
        }
        sCommClubManager->buttonSelection = 0xFFFF;
        sCommClubManager->buttonCloseRequest = 0;
        FieldSystem_SwitchBottomScreenApp(sCommClubManager->fieldSystem, 7, &sCommClubManager->buttonMsgIds);
    }
}

static void CommClub_HandleMemberList(void *task, CommClubManager *commClubManager) {
    if (FieldSystem_GetBottomScreenAppState(commClubManager->fieldSystem) != 1) {
        return;
    }

    CommClub_PrintPlayerCount(commClubManager);

    for (int i = 0; i < 8; i++) {
        if (CommPlayerInfo_IsReceived(i) && i != 0) {
            sCommClubManager->playerListed[i] = 1;
            sCommClubManager->listNeedsRedraw = 1;
        } else if (CommPlayerInfo_GetProfile(i) && sCommClubManager->playerListed[i] == 0) {
            sCommClubManager->playerListed[i] = 1;
            sCommClubManager->listNeedsRedraw = 1;
        } else if (CommPlayerInfo_GetProfile(i) == 0 && sCommClubManager->playerListed[i] != 0) {
            sCommClubManager->playerListed[i] = 0;
            sCommClubManager->listNeedsRedraw = 1;
        }
    };

    if (sCommClubManager->listNeedsRedraw) {
        sCommClubManager->listNeedsRedraw = 0;
        RedrawListMenu(sCommClubManager->listMenu);
        for (u32 i = 0; i < ListMenuGetTemplateField(sCommClubManager->listMenu, LISTMENUATTR_MAXSHOWED); i++) {
            CommClub_MemberListPrintRow(sCommClubManager->listMenu, 0, i);
        };
    }

    s32 listState = ListMenu_ProcessInput(commClubManager->listMenu);
    if (CommPlayerInfo_FindReceived() != 0xFF) {
        commClubManager->contactNetId = CommPlayerInfo_FindReceived();
        PlaySE(SEQ_SE_DP_PC_LOGIN);
        commClubManager->buttonCloseRequest = 1;
        CommClub_PrintContacted(commClubManager->contactNetId, commClubManager);
        CommClub_SetState(&CommClub_OpenContactChoice);
        return;
    }
    if (Comm_HasLinkError()) {
        CommClub_ExitLink();
        CommClub_CloseMenus(task, commClubManager);
        sCommClubManager->retCode = 3;
        return;
    }

    if (listState == LIST_NOTHING_CHOSEN) {
        if (CommClub_GetMaxPlayers() > 2) {
            if (sCommClubManager->buttonSelection == 0) {
                listState = LIST_NO_MULTIPLE_SCROLL;
            } else if (sCommClubManager->buttonSelection == 1) {
                listState = LIST_CANCEL;
            }
        } else if (sCommClubManager->buttonSelection == 0) {
            listState = LIST_CANCEL;
        }
    }

    switch (listState) {
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);
        CommClub_SetState(&CommClub_AskCancelGroup);
        break;
    default:
        if (CommClub_GetMinPlayers() <= CommPlayerInfo_CountAccepted()) {
            u16 sConfirmGroupText[NUM_COMM_TYPES] = {
                0,
                0,
                0,
                CommunicationClub_Text_ConfirmGroup_MultiBattle, // Multi Battle! Is this group OK?
                CommunicationClub_Text_ConfirmGroup_MultiBattle, // Multi Battle! Is this group OK?
                CommunicationClub_Text_ConfirmGroup_MultiBattle, // Multi Battle! Is this group OK?
                CommunicationClub_Text_ConfirmGroup,             // Is this group OK?
                CommunicationClub_Text_00040_Dummy,
                CommunicationClub_Text_00041_Dummy,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                CommunicationClub_Text_ConfirmGroup_Pokeathlon // Pokéathlon! Is this group OK?
            };
            PlaySE(SEQ_SE_DP_SELECT);
            commClubManager->connectedCount = CommPlayerInfo_CountAccepted();
            CommClub_PrintMessage(sConfirmGroupText[commClubManager->commType], FALSE);
            CommClub_SetState(&CommClub_OpenConfirmGroupChoice);
        }
        break;
    case LIST_NOTHING_CHOSEN:
        break;
    }
}

static void CommClub_HandleContactChoice(void *task, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    if (commClubManager->buttonsShown == 0) {
        if (CommClub_InitYesNoButtons(commClubManager) == TRUE) {
            commClubManager->buttonsShown = 1;
        } else {
            return;
        }
    }

    if (CommClub_IsYesNoReady(commClubManager) == FALSE) {
        return;
    }
    int i;
    if (Comm_IsNetIdConnected(commClubManager->contactNetId) == FALSE) {
        CommClub_CloseYesNoButtons(commClubManager);
        if (commClubManager->commType == COMM_TYPE_8 || commClubManager->commType == COMM_TYPE_POKEATHLON) {
            for (i = 1; i < CommClub_GetMaxPlayers(); i++) {
                if (Comm_IsNetIdConnected(i)) {
                    CommPlayerInfo_ClearPlayer(i);
                    LinkSync_SendValue(5, i);
                }
            }
            CommClub_PrintMessage(0, FALSE);
            CommClub_SetState(&CommClub_WaitProblemAckThenReopen);
        } else {
            CommPlayerInfo_ClearPlayer(commClubManager->contactNetId);
            CommClub_PrintMessage(0, FALSE);
            CommClub_SetState(&CommClub_WaitProblemAckThenReopen);
        }
    } else if (Comm_HasLinkError() || CommClub_HasErrorDummy()) {
        CommClub_CloseYesNoButtons(commClubManager);
        CommPlayerInfo_ClearPlayer(commClubManager->contactNetId);
        CommClub_PrintMessage(0, FALSE);
        CommClub_SetState(&CommClub_WaitProblemAckThenReopen);
    } else if (FieldSystem_GetTouchscreenChoiceMenuState(commClubManager->fieldSystem) == 6) {
        if (commClubManager->choiceResult == 0) {
            if ((commClubManager->commType == COMM_TYPE_SINGLE_BATTLE || commClubManager->commType == COMM_TYPE_DOUBLE_BATTLE || commClubManager->commType == COMM_TYPE_FLAT_BATTLE_1 || commClubManager->commType == COMM_TYPE_FLAT_BATTLE_2) && CommPlayerInfo_RulesetsMatch() == 0) {
                CommClub_PrintMessage(116, FALSE);
                CommClub_SetState(&CommClub_WaitDifferentRulesAck);
                LinkSync_SendValue(4, commClubManager->contactNetId);
            } else {
                CommPlayerInfo_Accept(commClubManager->contactNetId);
                LinkSync_SendValue(2, commClubManager->contactNetId);
                if (CommClub_GetMaxPlayers() == CommPlayerInfo_CountAccepted()) {
                    commClubManager->connectedCount = CommPlayerInfo_CountAccepted();
                    if (CommClub_GetMaxPlayers() > 2) {
                        CommClub_SetState(&CommClub_ConfirmGroup);
                    } else {
                        CommClub_CloseMenus(task, commClubManager);
                        CommClub_HostRepliedOk(commClubManager);
                    }
                } else {
                    CommClub_PrintAwaitingContact();
                    CommClub_SetState(&CommClub_ReopenMemberList);
                }
            }
        } else if (commClubManager->choiceResult == 1) {
            if (commClubManager->commType == COMM_TYPE_8 || commClubManager->commType == COMM_TYPE_POKEATHLON) {
                for (i = 1; i < CommClub_GetMaxPlayers(); i++) {
                    if (Comm_IsNetIdConnected(i)) {
                        CommPlayerInfo_ClearPlayer(i);
                        LinkSync_SendValue(5, i);
                    }
                }
                CommClub_PrintAwaitingContact();
                CommClub_SetState(&CommClub_ReopenMemberList);
            } else {
                CommPlayerInfo_ClearPlayer(commClubManager->contactNetId);
                LinkSync_SendValue(3, commClubManager->contactNetId);
                CommClub_PrintAwaitingContact();
                CommClub_SetState(&CommClub_ReopenMemberList);
            }
        }
    }
}

static void CommClub_WaitDifferentRulesAck(void *task, CommClubManager *commClubManager) {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        CommClub_CloseMenus(task, commClubManager);
        CommClub_ExitLink();
        sCommClubManager->retCode = 4;
    }
}

static void CommClub_WaitProblemAckThenReopen(void *task, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        CommClub_PrintAwaitingContact();
        CommClub_SetState(&CommClub_ReopenMemberList);
    }
}

static void CommClub_ReopenMemberList(void *arg0_unused, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        CommClub_SetState(&CommClub_HandleMemberList);
        if (CommClub_GetMaxPlayers() > 2) {
            sCommClubManager->buttonMsgIds = sStartQuitButtons;
            sCommClubManager->buttonCount = 2;
        } else {
            sCommClubManager->buttonMsgIds = &sQuitButton;
            sCommClubManager->buttonCount = 1;
        }
        sCommClubManager->buttonSelection = 0xFFFF;
        sCommClubManager->buttonCloseRequest = 0;
        FieldSystem_SwitchBottomScreenApp(sCommClubManager->fieldSystem, 7, &sCommClubManager->buttonMsgIds);
    }
}

static void CommClub_ConfirmGroup(void *arg0_unused, CommClubManager *commClubManager) {
    u16 sConfirmGroupText[NUM_COMM_TYPES] = {
        0,
        0,
        0,
        CommunicationClub_Text_ConfirmGroup_MultiBattle, // Multi Battle! Is this group OK?
        CommunicationClub_Text_ConfirmGroup_MultiBattle, // Multi Battle! Is this group OK?
        CommunicationClub_Text_ConfirmGroup_MultiBattle, // Multi Battle! Is this group OK?
        CommunicationClub_Text_ConfirmGroup,             // Is this group OK?
        CommunicationClub_Text_00040_Dummy,
        CommunicationClub_Text_00041_Dummy,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_ConfirmGroup_Pokeathlon // Pokéathlon! Is this group OK?
    };
    CommClub_PrintPlayerCount(commClubManager);
    CommClub_PrintMessage(sConfirmGroupText[commClubManager->commType], FALSE);
    CommClub_SetState(&CommClub_OpenConfirmGroupChoice);
}

static void CommClub_OpenConfirmGroupChoice(void *arg0_unused, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    CommState_SetEntry(0);
    ListMenu_ProcessInput(commClubManager->listMenu);
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        CommClub_OpenYesNoButtons(commClubManager);
        CommClub_SetState(&CommClub_HandleConfirmGroupChoice);
    }
}

static void CommClub_HandleConfirmGroupChoice(void *task, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    if (commClubManager->buttonsShown == 0) {
        if (CommClub_InitYesNoButtons(commClubManager) == TRUE) {
            commClubManager->buttonsShown = 1;
        } else {
            return;
        }
    }

    if (CommClub_IsYesNoReady(commClubManager)) {
        if (Comm_HasLinkError() || commClubManager->connectedCount != Comm_CountConnected()) {
            CommClub_CloseYesNoButtons(commClubManager);
            CommClub_SetState(&CommClub_GroupCancelled);
        } else {
            if (FieldSystem_GetTouchscreenChoiceMenuState(commClubManager->fieldSystem) == 6) {
                if (commClubManager->choiceResult == 0) {
                    if (commClubManager->commType == COMM_TYPE_8 || commClubManager->commType == COMM_TYPE_POKEATHLON) {
                        for (int i = 1; i < Comm_CountConnected(); i++) {
                            if (Comm_IsNetIdConnected(i) == 0) {
                                CommClub_SetState(&CommClub_GroupCancelled);
                                return;
                            }
                        }
                    }
                    LinkSync_StartSync(10);
                    CommClub_SetState(&CommClub_WaitGroupSync);
                } else if (commClubManager->choiceResult == 1) {
                    CommState_SetEntry(1);
                    CommClub_ExitLink();
                    CommClub_CloseMenus(task, commClubManager);
                    sCommClubManager->retCode = 1;
                }
            }
        }
    }
}

static void CommClub_GroupCancelled(void *task, CommClubManager *commClubManager) {
    u16 sGroupCancelledText[NUM_COMM_TYPES] = {
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_GroupCancelled, // A member has dropped out. Please repeat the registration process from the beginning.
        CommunicationClub_Text_00066_Dummy,
        CommunicationClub_Text_00067_Dummy,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_GroupCancelled_Pokeathlon // A Pokéathlon involving this group will be canceled.
    };

    CommClub_CloseMenus(task, commClubManager);
    if (sCommClubManager->commType == COMM_TYPE_8) {
        CommClub_PrintMessage(sCommClubManager->unk92 + 137, FALSE);
    } else {
        CommClub_PrintMessage(sGroupCancelledText[commClubManager->commType], FALSE);
    }
    CommClubLink_Exit();
    SysTask_CreateOnMainQueue(&CommClub_WaitGroupCancelledAck, commClubManager, 0);
}

static void CommClub_WaitGroupCancelledAck(SysTask *task, void *commClubManager_unused) {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        sCommClubManager->retCode = 1;
        SysTask_Destroy(task);
    }
}

static void CommClub_AskCancelGroup(void *task, CommClubManager *commClubManager) {
    u16 sCancelGroupText[NUM_COMM_TYPES] = {
        0,
        0,
        0,
        CommunicationClub_Text_CancelGroup_MultiBattle, // Cancel a Multi Battle involving this group?
        CommunicationClub_Text_CancelGroup_MultiBattle, // Cancel a Multi Battle involving this group?
        CommunicationClub_Text_CancelGroup_MultiBattle, // Cancel a Multi Battle involving this group?
        CommunicationClub_Text_00044_Dummy,
        CommunicationClub_Text_00045_Dummy,
        CommunicationClub_Text_00046_Dummy,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        CommunicationClub_Text_CancelGroup_Pokeathlon // Cancel holding a Pokéathlon with this group?
    };
    CommClub_PrintPlayerCount(commClubManager);
    if (CommPlayerInfo_CountAccepted() > 1) {
        CommClub_PrintMessage(sCancelGroupText[commClubManager->commType], FALSE);
        CommClub_SetState(&CommClub_OpenCancelGroupChoice);
    } else {
        CommClub_ExitLink();
        CommClub_CloseMenus(task, commClubManager);
        sCommClubManager->retCode = 1;
    }
}

static void CommClub_OpenCancelGroupChoice(void *arg0_unused, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    ListMenu_ProcessInput(commClubManager->listMenu);
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        CommClub_OpenYesNoButtons(commClubManager);
        CommClub_SetState(&CommClub_HandleCancelGroupChoice);
    }
}

static void CommClub_HandleCancelGroupChoice(void *task, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    if (commClubManager->buttonsShown == 0) {
        if (CommClub_InitYesNoButtons(commClubManager) == TRUE) {
            commClubManager->buttonsShown = 1;
        } else {
            return;
        }
    }

    if (CommClub_IsYesNoReady(commClubManager)) {
        if (Comm_HasLinkError()) {
            CommClub_CloseYesNoButtons(commClubManager);
            CommClub_PrintAwaitingContact();
            CommClub_SetState(&CommClub_ReopenMemberList);
        } else if (FieldSystem_GetTouchscreenChoiceMenuState(commClubManager->fieldSystem) == 6) {
            if (commClubManager->choiceResult == 0) {
                CommClub_ExitLink();
                CommClub_CloseMenus(task, commClubManager);
                sCommClubManager->retCode = 1;
            } else if (commClubManager->choiceResult == 1) {
                CommClub_PrintAwaitingContact();
                CommClub_SetState(&CommClub_ReopenMemberList);
            }
        }
    }
}

static void CommClub_WaitGroupSync(void *task, CommClubManager *commClubManager) {
    if (Comm_HasLinkError() || commClubManager->connectedCount != Comm_CountConnected()) {
        CommClub_SetState(&CommClub_GroupCancelled);
    } else {
        CommClub_PrintPlayerCount(commClubManager);
        ListMenu_ProcessInput(commClubManager->listMenu);
        if (LinkSync_IsSynced(10) && LinkSync_Exists()) {
            CommClub_CloseMenus(task, commClubManager);
            CommClub_FinishGroup(commClubManager);
        }
    }
}

static void CommClub_OpenContactChoice(s32 arg0_unused, CommClubManager *commClubManager) {
    CommClub_PrintPlayerCount(commClubManager);
    ListMenu_ProcessInput(commClubManager->listMenu);
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        CommClub_OpenYesNoButtons(commClubManager);
        CommClub_SetState(&CommClub_HandleContactChoice);
    }
}

static void CommClub_CloseMenus(void *task, CommClubManager *commClubManager) {
    FillWindowPixelBuffer(&sCommClubManager->windows[2], 15);
    EraseFrameAndWindow1(&commClubManager->windows[0], FALSE);
    ListMenuItems_Delete(commClubManager->items);
    DestroyListMenu(commClubManager->listMenu, NULL, NULL);
    commClubManager->listMenu = NULL;
    ClearWindowTilemapAndCopyToVram(&commClubManager->windows[0]);
    RemoveWindow(&commClubManager->windows[0]);
    if (commClubManager->infoWindowOpen) {
        EraseFrameAndWindow1(&sCommClubManager->windows[1], 0);
        ClearWindowTilemapAndCopyToVram(&sCommClubManager->windows[1]);
        RemoveWindow(&sCommClubManager->windows[1]);
        commClubManager->infoWindowOpen = 0;
    }
    if (task) {
        SysTask_Destroy(task);
    }
}

static void CommClub_HostRepliedOk(CommClubManager *commClubManager) {
    BufferPlayersName(commClubManager->messageFormat[2], 1, CommPlayerInfo_GetProfile(1));
    CommClub_PrintMessage(69, TRUE);
    SysTask_CreateOnMainQueue(&CommClub_Task, commClubManager, 0);
    CommClub_SetState(&CommClub_WaitGroupStart);
}

static void CommClub_ChildAccepted(CommClubManager *commClubManager) {
    CommWireless_CopyParentProfile(commClubManager->connectionIndex, sCommClubManager->playerProfile[1]);
    BufferPlayersName(commClubManager->messageFormat[2], 1, sCommClubManager->playerProfile[1]);
    CommClub_PrintMessage(4, TRUE);
    commClubManager->connectedCount = Comm_CountConnected();
    SysTask_CreateOnMainQueue(&CommClub_Task, commClubManager, 0);
    CommClub_SetState(&CommClub_WaitGroupStart);
    sCommClubManager->messageDelay = 90;
}

static void CommClub_WaitGroupStart(u32 arg0_unused, CommClubManager *commClubManager) {
    if (Comm_GetLocalNetId() == FALSE && commClubManager->connectedCount != Comm_CountConnected()) {
        CommClub_SetState(&CommClub_ShowProblem);
    } else if (CommClub_HasErrorDummy() || Comm_HasLinkError()) {
        CommClub_SetState(&CommClub_ShowProblem);
    } else if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        LinkSync_StartSync(10);
        CommClub_SetState(&CommClub_WaitGroupSyncAfterReply);
    }
}

static void CommClub_WaitGroupSyncAfterReply(void *task, CommClubManager *commClubManager) {
    if (Comm_GetLocalNetId() == 0 && commClubManager->connectedCount != Comm_CountConnected()) {
        CommClub_SetState(&CommClub_ShowProblem);
    } else if (CommClub_HasErrorDummy() || Comm_HasLinkError()) {
        CommClub_SetState(&CommClub_ShowProblem);
        return;
    } else if (Comm_IsNetIdConnected(0) == 0) {
        CommClub_SetState(&CommClub_ShowProblem);
        return;
    } else if (LinkSync_GetValue(0, 5) != -1) {
        CommClub_SetState(&CommClub_ShowProblem);
        return;
    } else if (LinkSync_IsSynced(10) && LinkSync_Exists()) {
        if (DialogBox_IsPrintFinished(sCommClubManager->printerID) == FALSE) {
            RemoveTextPrinter(sCommClubManager->printerID);
        }
        commClubManager->connectedCount = Comm_CountConnected();
        CommClub_FinishGroup(commClubManager);
        SysTask_Destroy(task);
        return;
    }

    if (sCommClubManager->messageDelay != 0) {
        sCommClubManager->messageDelay--;
        if (sCommClubManager->messageDelay == 0) {
            u16 sGroupWaitingText[NUM_COMM_TYPES] = {
                0,
                0,
                0,
                CommunicationClub_Text_GroupWaiting_MultiBattle, // Multi Battle! Waiting for the rest of the group.
                CommunicationClub_Text_GroupWaiting_MultiBattle, // Multi Battle! Waiting for the rest of the group.
                CommunicationClub_Text_GroupWaiting_MultiBattle, // Multi Battle! Waiting for the rest of the group.
                CommunicationClub_Text_GroupWaiting,             // Waiting for the rest of the group.
                CommunicationClub_Text_00072_Dummy,
                CommunicationClub_Text_00073_Dummy,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                CommunicationClub_Text_GroupWaiting_Pokeathlon // Pokéathlon! Waiting for the rest of the group.
            };
            if (sCommClubManager->commType == COMM_TYPE_8) {
                CommClub_PrintMessage(sCommClubManager->unk92 + 132, FALSE);
            } else if (sCommClubManager->commType == COMM_TYPE_POKEATHLON) {
                CommClub_PrintMessage(sCommClubManager->unk92 + 160, FALSE);
            } else {
                CommClub_PrintMessage(sGroupWaitingText[commClubManager->commType], FALSE);
            }
        }
    }
}

static void CommClub_ShowProblem() {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        CommClub_PrintMessage(0, FALSE);
        CommClub_SetState(&CommClub_WaitProblemAck);
    }
}

static void CommClub_WaitProblemAck(SysTask *task) {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        SysTask_Destroy(task);
        CommClub_ExitLink();
        sCommClubManager->retCode = 1;
    }
}

static void CommClub_ShowRefused(CommClubManager *commClubManager) {
    CommWireless_CopyParentProfile(commClubManager->connectionIndex, sCommClubManager->playerProfile[1]);
    BufferPlayersName(commClubManager->messageFormat[2], 1, sCommClubManager->playerProfile[1]);
    CommClub_PrintMessage(5, TRUE);
    CommClubLink_RestartScan();
    SysTask_CreateOnMainQueue(&CommClub_WaitRefusedThenParentList, commClubManager, 0);
}

static void CommClub_WaitRefusedThenParentList(SysTask *task, void *commClubManager) {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID)) {
        CommClubLink_RestartScan();
        CommClub_ShowParentList(commClubManager);
        SysTask_Destroy(task);
    }
}

static void CommClub_ResetToParentList(void *commClubManager) {
    CommClubLink_RestartScan();
    for (int i = 0; i < 8; i++) {
        CommPlayerInfo_ClearPlayer(i);
    }
    SysTask_CreateOnMainQueue(&CommClub_WaitAckThenParentList, commClubManager, 0);
}

static void CommClub_WaitAckThenParentList(SysTask *task, void *commClubManager) {
    if (DialogBox_IsPrintFinished(sCommClubManager->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        CommClub_ShowParentList(commClubManager);
        SysTask_Destroy(task);
    }
}

static void CommClub_FinishGroup(CommClubManager *commClubManager_unused) {
    CommState_SetEntry(0);
    sCommClubManager->retCode = 2;
    CommState_SetErrorWatch(1, 1);
    CommPlayerInfo_SendLocal();
    CommWireless_SetLostWhenAlone(1);
}

static int CommClub_GetMinPlayers() {
    return Comm_GetMinPlayers(sCommClubManager->commType);
}

static int CommClub_GetMaxPlayers() {
    return Comm_GetMaxPlayers(sCommClubManager->commType);
}

static int CommClub_GetBeaconId() {
    return sCommClubManager->unk92 + (sCommClubManager->unk93 * 16);
}

void CommClub_StartJoin(FieldSystem *fieldSystem, u32 commType, u16 arg2, u16 arg3) {
    CommClub_NewManager(fieldSystem);
    sCommClubManager->commType = commType;
    if (commType == COMM_TYPE_MIX_BATTLE || commType == COMM_TYPE_MULTI_BATTLE_1) {
        fieldSystem->linkBattleRuleset = NULL;
    }
    sCommClubManager->unk92 = arg2;
    sCommClubManager->unk93 = arg3;
    CommClub_StartChild(sCommClubManager);
}

void CommClub_OnChildStarted() {
    CommClub_ShowParentList(sCommClubManager);
}

u32 CommClub_PollJoinResult() {
    u32 retCode = sCommClubManager->retCode; // Needs to be reserved as the pointer will be null after the next line.
    if (retCode) {
        CommClub_DeleteManager();
    }
    return retCode;
}

void CommClub_StartHost(FieldSystem *fieldSystem, u32 commType, u16 arg2, u16 arg3) {
    CommClub_NewManager(fieldSystem);
    sCommClubManager->commType = commType;
    if (commType == COMM_TYPE_MIX_BATTLE || commType == COMM_TYPE_MULTI_BATTLE_1) {
        fieldSystem->linkBattleRuleset = NULL;
    }
    sCommClubManager->unk92 = arg2;
    sCommClubManager->unk93 = arg3;
    CommClub_StartParent(sCommClubManager);
}

void CommClub_OnParentConnected() {
    CommClub_StartHosting(sCommClubManager);
}

u32 CommClub_PollHostResult() {
    u32 retCode = sCommClubManager->retCode;
    if (retCode) {
        CommClub_DeleteManager();
    }
    return retCode;
}

static BOOL CommClub_HasErrorDummy() {
    return FALSE;
}

static void CommClub_OpenYesNoButtons(CommClubManager *commClubManager) {
    FieldSystem_SwitchBottomScreenApp(commClubManager->fieldSystem, 3, NULL);
    commClubManager->buttonsShown = 0;
    commClubManager->choiceResult = 0;
}

static BOOL CommClub_InitYesNoButtons(CommClubManager *commClubManager) {
    int unkInt = FieldSystem_GetBottomScreenAppId(commClubManager->fieldSystem);
    BOOL unkBool = FieldSystem_GetBottomScreenAppState(commClubManager->fieldSystem);
    if (unkInt == 3 && unkBool == TRUE) {
        FieldSystem_SetTouchscreenChoiceMenuState(commClubManager->fieldSystem, 3, 3, &commClubManager->choiceResult);
        return TRUE;
    }
    return FALSE;
}

static BOOL CommClub_IsYesNoReady(CommClubManager *commClubManager) {
    return FieldSystem_GetTouchscreenChoiceMenuState(commClubManager->fieldSystem) >= 4;
}

static void CommClub_CloseYesNoButtons(CommClubManager *commClubManager) {
    FieldSystem_SetTouchscreenChoiceMenuState(commClubManager->fieldSystem, 3, 11, &commClubManager->choiceResult);
}
