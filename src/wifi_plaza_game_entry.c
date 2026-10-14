#include "msgdata/msg/msg_0776.h"
#include "msgdata/msg/msg_0777.h"

#include "wifi_plaza_game_entry.h"

#include "global.h"

#include "constants/sndseq.h"

#include "comm_state.h"
#include "communication_player_state.h"
#include "font.h"
#include "link_synchronization.h"
#include "sound_playback.h"
#include "system.h"
#include "communication_core.h"

// Communication calls without a public declaration
void CommPlayerInfo_SetProfile(void *profile);
void CommState_StartPlazaMatch(u8 slot);
void CommState_EndPlazaMatch(void);
s32 CommState_GetPlazaMatchStatus(void);
void CommState_SetPlazaLinkMode(void);

// rodata of this overlay (asm/wifi_plaza_map_rodata.s)
extern const u8 sWiFiPlazaGameStatusHeights[2];
extern const ListMenuTemplate sWiFiPlazaGameEntryMenuTemplate;

BOOL WiFiPlazaGameEntry_Register(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, int playerIndex) {
    u8 memberIds[4];
    WiFiPlazaWork *lobby;
    s32 connection;
    u32 memberCount __attribute__((aligned(8)));
    s32 myNetId;
    s32 hostFound;
    s32 hostId;
    s32 failed;
    int savedPlayerIndex;
    WiFiPlazaPlayerState *savedState;
    u16 *packet;
    s32 netId;
    s32 timeout;
    s32 yesNo;
    s32 linkState;
    String *message;
    String *message2;
    s32 msgId;
    s32 i;
    s32 msgId2;
    s32 closeMenu;
    s32 received;
    s8 delay;
    u32 step;
    u32 members3;
    u32 linkError;
    u32 step2;
    u32 menuResult;
    u32 linkError2;
    u32 accepted2;
    u32 accepted;
    u32 members;
    u32 members2;
    WiFiPlazaGameEntry *entry;
    WiFiPlazaGameEntryParam *param;

    savedState = state;
    savedPlayerIndex = playerIndex;
    param = WiFiPlazaPlayerState_GetArg(state);
    entry = WiFiPlazaPlayerState_GetWork(state);
    lobby = WiFiPlazaRoom_GetLobby(room);

    // while waiting or linking: give up when the session or the link broke down
    step = WiFiPlazaPlayerState_GetStep(state);
    switch (step) {
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 26:
    case 27:
        if ((u32)entry->members > WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot)) {
            param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
        } else if ((s32)entry->members > Comm_CountConnected()) {
            param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
        } else if (CommState_GetPlazaMatchStatus() == 0) {
            param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
        } else {
        case 8:
            if (WiFiPlazaLobbyClient_IsMatchHost() == 1 && WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot) <= 1) {
                param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
                WiFiPlazaPlayerState_SetStep(savedState, 23);
            } else {
                timeout = entry->timeout - 1;
                entry->timeout = timeout;
                if (timeout < 0) {
                    param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
                    WiFiPlazaPlayerState_SetStep(savedState, 23);
                } else {
                case 7:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                    linkError = CommState_GetWifiMatchStatus();
                    switch (linkError) {
                    case 0:
                    case 1:
                        break;
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
                        WiFiPlazaPlayerState_SetStep(savedState, 23);
                        break;
                    }
                }
            }
        }
        break;
    }

    step2 = WiFiPlazaPlayerState_GetStep(savedState);
    switch (step2) {
    case 0:
        WiFiPlazaPlayerState_AllocWork(savedState, sizeof(WiFiPlazaGameEntry));
        WiFiPlazaPlayerState_NextStep(savedState);
    default:
        break;
    case 1:
        if (WiFiPlazaWork_IsGameEntryCoolingDown(lobby) != 0) {
            param->result = WIFI_PLAZA_GAME_ENTRY_NOT_READY;
            WiFiPlazaPlayerState_SetStep(savedState, 28);
        } else if (WiFiPlazaWork_GetEventScheduleMode5(lobby) != 0) {
            param->result = WIFI_PLAZA_GAME_ENTRY_PLAZA_CLOSING;
            WiFiPlazaPlayerState_SetStep(savedState, 28);
        } else {
            PlaySE(SEQ_SE_DP_DECIDE);
            WiFiPlazaPlayerState_NextStep(savedState);
        }
        break;
    case 2: // "Would you like to play ...?"
        WiFiPlazaRoom_BufferActivityName(room, param->game, 0);
        WiFiPlazaGameEntry_PrintMessage(entry, savedState, room, msg_0776_00000, 3);
        break;
    case 3: // JOIN / RULES / CANCEL
        WiFiPlazaGameEntry_InitMenu(entry, room, 3, 1);
        WiFiPlazaRoom_OpenList(room, &entry->menuTemplate, 0, 0);
        WiFiPlazaPlayerState_SetStep(savedState, 4);
        break;
    case 4:
        closeMenu = 0;
        menuResult = WiFiPlazaRoom_ProcessList(room);
        switch (menuResult) {
        case LIST_CANCEL:
        case 2:
            param->result = WIFI_PLAZA_GAME_ENTRY_CANCELLED;
            WiFiPlazaPlayerState_SetStep(savedState, 28);
            closeMenu = 1;
            break;
        case 1:
            WiFiPlazaPlayerState_SetStep(savedState, 5);
            closeMenu = 1;
            break;
        case 0:
            WiFiPlazaPlayerState_SetStep(savedState, 6);
            closeMenu = 1;
            break;
        }
        if (closeMenu == 1) {
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            WiFiPlazaGameEntry_FreeMenu(entry, room);
        }
        break;
    case 5: // rules
        WiFiPlazaRoom_BufferActivityName(room, param->game, 0);
        WiFiPlazaGameEntry_PrintMessage(entry, savedState, room, param->game + 0x22, 2);
        break;
    case 6: // join
        if (WiFiPlazaWork_GetEventScheduleMode5(lobby) != 0) {
            param->result = WIFI_PLAZA_GAME_ENTRY_PLAZA_CLOSING;
            WiFiPlazaPlayerState_SetStep(savedState, 28);
        } else if (WiFiPlazaLobbyClient_IsMatchActive(param->slot) == TRUE) {
            if (WiFiPlazaLobbyClient_GetMatchFreePlaces(param->slot) == 0 || WiFiPlazaLobbyClient_IsMatchOpen(param->slot) == FALSE) {
                param->result = WIFI_PLAZA_GAME_ENTRY_SESSION_FULL;
                WiFiPlazaPlayerState_SetStep(savedState, 28);
            } else if ((s16)WiFiPlazaLobbyClient_GetMatchTimer(param->slot) == 0) {
                param->result = WIFI_PLAZA_GAME_ENTRY_NOT_READY;
                WiFiPlazaPlayerState_SetStep(savedState, 28);
            } else {
                goto join;
            }
        } else {
join:
            entry->timeLeft = WiFiPlazaLobbyClient_GetMatchTimer(param->slot);
            entry->timeout = 30 * 30;
            if (entry->timeLeft > 330) {
                message = WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00010);
            } else {
                entry->startingSoon = TRUE;
                message = WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00011);
            }
            WiFiPlazaRoom_PrintMessageInstant(room, message);
            WiFiPlazaRoom_ShowWaitingIcon(room);
            CommState_StartPlazaMatch(param->slot);
            if (WiFiPlazaLobbyClient_IsMatchHost() == 1) {
                WiFiPlaza_SendActivityNews(lobby, param->game, 1, savedPlayerIndex, 0, 0, 0, 0);
                entry->reportedMembers = 1;
            }
            WiFiPlazaGameStatus_Open(&entry->status, room, param->game, TRUE, entry->timeLeft);
            WiFiPlazaGameStatus_UpdateInPlay(&entry->status, room, TRUE);
            WiFiPlazaPlayerState_SetStep(savedState, 7);
        }
        break;
    case 7: // waiting for members
        connection = CommState_GetPlazaMatchStatus();
        members = WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot);
        entry->timeLeft = WiFiPlazaLobbyClient_GetMatchTimer(param->slot);
        WiFiPlazaGameStatus_SetFrames(&entry->status, entry->timeLeft);
        if (entry->timeLeft <= 330 && entry->startingSoon == FALSE) {
            WiFiPlazaRoom_HideWaitingIcon(room);
            entry->startingSoon = TRUE;
            WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00011));
            WiFiPlazaRoom_ShowWaitingIcon(room);
        }
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        if (connection == 0) {
            param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
        } else if (connection == 2) {
            param->result = WIFI_PLAZA_GAME_ENTRY_STARTED;
            if (members == 4) {
                msgId2 = msg_0776_00016;
            } else {
                msgId2 = msg_0776_00018;
            }
            WiFiPlazaRoom_HideWaitingIcon(room);
            WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 0, msgId2));
            WiFiPlazaRoom_ShowWaitingIcon(room);
            WiFiPlazaPlayerState_SetStep(savedState, 9);
            entry->members = members;
            WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
            WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        } else if (entry->startingSoon == FALSE && (gSystem.newKeys & PAD_BUTTON_B)) {
            PlaySE(SEQ_SE_DP_SELECT);
            if (WiFiPlazaLobbyClient_IsMatchHost() == 0) {
                param->result = WIFI_PLAZA_GAME_ENTRY_DROPPED_OUT;
                WiFiPlazaRoom_HideWaitingIcon(room);
                WiFiPlazaPlayerState_SetStep(savedState, 23);
            } else {
                param->result = WIFI_PLAZA_GAME_ENTRY_DROPPED_OUT;
                WiFiPlazaRoom_HideWaitingIcon(room);
                WiFiPlazaPlayerState_SetStep(savedState, 16);
            }
        } else if (WiFiPlazaLobbyClient_IsMatchHost() == 1) {
            if (entry->reportedMembers != members) {
                entry->reportedMembers = members;
                if (members != 4) {
                    WiFiPlaza_SendActivityNews(lobby, param->game, members, savedPlayerIndex, 0, 0, 0, 0);
                }
            }
            if (entry->timeLeft == 0 && WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot) <= 1) {
                param->result = WIFI_PLAZA_GAME_ENTRY_NO_MEMBERS;
                WiFiPlazaPlayerState_SetStep(savedState, 23);
            } else {
                goto checkSession;
            }
        } else {
checkSession:
            if (WiFiPlazaLobbyClient_IsMatchActive(param->slot) == TRUE && WiFiPlazaLobbyClient_IsMatchOpen(param->slot) == FALSE) {
                param->result = WIFI_PLAZA_GAME_ENTRY_IN_PLAY;
                WiFiPlazaPlayerState_SetStep(savedState, 23);
            } else if (entry->timeLeft == 0) {
                WiFiPlazaPlayerState_SetStep(savedState, 8);
                WiFiPlazaRoom_HideWaitingIcon(room);
                WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00018));
                WiFiPlazaRoom_ShowWaitingIcon(room);
            }
        }
        break;
    case 8: // time is up, waiting for the link
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        linkState = CommState_GetPlazaMatchStatus();
        members2 = WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot);
        linkError2 = CommState_GetWifiMatchStatus();
        switch (linkError2) {
        case 3:
        case 4:
            param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
            break;
        }
        if (linkState == 0) {
            param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
        } else if (linkState == 2) {
            param->result = WIFI_PLAZA_GAME_ENTRY_STARTED;
            WiFiPlazaPlayerState_SetStep(savedState, 9);
            entry->members = members2;
        } else if (WiFiPlazaLobbyClient_IsMatchActive(param->slot) != TRUE || WiFiPlazaLobbyClient_IsMatchOpen(param->slot) != FALSE) {

        } else {
            param->result = WIFI_PLAZA_GAME_ENTRY_IN_PLAY;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
        }
        break;
    case 9: // linked: exchange the player info
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), param->playerStatus);
        CommState_SetPlazaLinkMode();
        CommPlayerInfo_Init(WiFiPlazaWork_GetSaveData(WiFiPlazaRoom_GetLobby(room)), 0);
        CommPlayerInfo_SetProfile(WiFiPlazaWork_GetOwnPlayerProfile(lobby));
        Comm_SetWifiContinuousSend(0);
        WiFiPlazaGameEntry_StartSync(entry, savedState, 10, 17);
        break;
    case 10:
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        CommPlayerInfo_SendLocal();
        CommPlayerInfo_Accept(Comm_GetLocalNetId());
        WiFiPlazaPlayerState_SetStep(savedState, 11);
        break;
    case 11:
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        netId = CommPlayerInfo_FindReceived();
        if (netId != 0xFF) {
            do {
                CommPlayerInfo_Accept(netId);
                netId = CommPlayerInfo_FindReceived();
            } while (netId != 0xFF);
        }
        accepted = CommPlayerInfo_CountAccepted();
        if (accepted >= WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot)) {
            if (WiFiPlazaLobbyClient_IsMatchHost() == 1 && entry->entriesClosed == FALSE) {
                WiFiPlazaLobbyClient_CloseMatch();
                entry->entriesClosed = TRUE;
            }
            if (WiFiPlazaLobbyClient_IsCurrentMatchClosed() == TRUE) {
                WiFiPlazaPlayerState_SetStep(savedState, 12);
            }
        } else if (WiFiPlazaLobbyClient_IsCurrentMatchClosed() == TRUE) {
            param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
            WiFiPlazaPlayerState_SetStep(savedState, 23);
        }
        break;
    case 12:
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        LinkSync_ClearPackets();
        WiFiPlazaWork_ClearGameMembers(lobby);
        WiFiPlazaGameEntry_StartSync(entry, savedState, 13, 14);
        break;
    case 13: // send our lobby id
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        entry->lobbyId = WiFiPlazaWork_GetOwnId(lobby);
        if (LinkSync_SendPacket(Comm_GetLocalNetId(), (s16 *)&entry->lobbyId) != TRUE) {

        } else {
            WiFiPlazaPlayerState_SetStep(savedState, 14);
        }
        break;
    case 14: // receive the members' lobby ids
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, FALSE);
        failed = FALSE;
        accepted2 = CommPlayerInfo_CountAccepted();
        memberCount = accepted2;
        myNetId = Comm_GetLocalNetId();
        received = 0;
        hostFound = received;
        hostId = WiFiPlazaLobbyClient_GetMatchHostId();
        i = received;
        if ((s32)accepted2 > 0) {
nextMember:
            if (myNetId != i) {
                packet = LinkSync_GetPacket(i);
                if (packet != 0) {
                    if (WiFiPlazaLobbyClient_IdToSlot(*(s32 *)packet) == -1) {
                        failed = TRUE;
                        param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
                        WiFiPlazaPlayerState_SetStep(savedState, 23);
                    } else {
                        WiFiPlazaWork_SetGameMember(lobby, *(s32 *)packet, i);
                        if (*(s32 *)packet == hostId) {
                            hostFound = TRUE;
                        }
                        received += 1;
                        goto next;
                    }
                } else {
                    goto next;
                }
            } else {
                WiFiPlazaWork_SetOwnGameMember(lobby, i);
                if (WiFiPlazaLobbyClient_IsMatchHost() == 1) {
                    hostFound = TRUE;
                }
                received += 1;
next:
                i += 1;
                if (i < (s32)memberCount) {
                    goto nextMember;
                }
            }
        }
        if (failed == FALSE && received == memberCount) {
            if (hostFound == TRUE) {
                WiFiPlazaPlayerState_SetStep(savedState, 15);
            } else {
                param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
                WiFiPlazaPlayerState_SetStep(savedState, 23);
            }
        }
        break;
    case 15: // start the game
        CommState_SetErrorWatch(0, 1);
        if (WiFiPlazaLobbyClient_IsMatchHost() == 1) {
            WiFiPlazaWork_GetGameMembers(lobby, memberIds);
            WiFiPlaza_SendActivityNews(lobby, param->game, WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot), memberIds[0], memberIds[1], memberIds[2], memberIds[3], 1);
        }
        WiFiPlazaRoom_HideWaitingIcon(room);
        WiFiPlazaGameEntry_StartSync(entry, savedState, 28, 18);
        break;
    case 16: // the host drops out: "If you drop out, the group will be disbanded. Is that OK?"
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00026));
        WiFiPlazaPlayerState_SetStep(savedState, 17);
        WiFiPlazaGameStatus_Close(&entry->status, room);
        break;
    case 17:
        if (WiFiPlazaGameEntry_UpdateWaiting(entry, room, param, savedState, savedPlayerIndex) == FALSE && WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_SetStep(savedState, 18);
        }
        break;
    case 18:
        WiFiPlazaRoom_OpenYesNoMenuDefaultNo(room);
        WiFiPlazaPlayerState_SetStep(savedState, 19);
        break;
    case 19:
        yesNo = WiFiPlazaRoom_GetYesNoResult(room);
        switch (yesNo) {
        case 0:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(savedState, 23);
            WiFiPlazaWork_StartGameEntryCooldown(lobby);
            break;
        case 1:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(savedState, 20);
            break;
        case 2:
            if (WiFiPlazaGameEntry_UpdateWaiting(entry, room, param, savedState, savedPlayerIndex) != TRUE) {

            } else {
                WiFiPlazaRoom_CloseYesNoMenu(room);
            }
            break;
        }
        break;
    case 20: // stays: back to waiting
        entry->timeLeft = WiFiPlazaLobbyClient_GetMatchTimer(param->slot);
        WiFiPlazaGameStatus_Open(&entry->status, room, param->game, TRUE, entry->timeLeft);
        WiFiPlazaGameStatus_UpdateInPlay(&entry->status, room, TRUE);
        if (entry->timeLeft > 330) {
            message2 = WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00010);
        } else {
            entry->startingSoon = TRUE;
            message2 = WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00011);
        }
        WiFiPlazaRoom_PrintMessageInstant(room, message2);
        WiFiPlazaRoom_ShowWaitingIcon(room);
        WiFiPlazaPlayerState_SetStep(savedState, 7);
        break;
    case 21: // the session filled up while the question was open
        members3 = WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot);
        param->result = WIFI_PLAZA_GAME_ENTRY_STARTED;
        if (members3 == 4) {
            msgId = msg_0776_00016;
        } else {
            msgId = msg_0776_00018;
        }
        WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 0, msgId));
        WiFiPlazaRoom_ShowWaitingIcon(room);
        WiFiPlazaPlayerState_SetStep(savedState, 9);
        WiFiPlazaGameStatus_Open(&entry->status, room, param->game, TRUE, 0);
        WiFiPlazaGameStatus_UpdateInPlay(&entry->status, room, TRUE);
        break;
    case 22: // the time ran out while the question was open
        WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00018));
        WiFiPlazaRoom_ShowWaitingIcon(room);
        WiFiPlazaGameStatus_Open(&entry->status, room, param->game, TRUE, 0);
        WiFiPlazaGameStatus_SetFrames(&entry->status, 0);
        WiFiPlazaGameStatus_Update(&entry->status, room, TRUE);
        WiFiPlazaPlayerState_SetStep(savedState, 8);
        break;
    case 23: // failed: end the link
        CommPlayerInfo_Free();
        CommState_EndPlazaMatch();
        WiFiPlazaPlayerState_SetStep(savedState, 24);
        break;
    case 24:
        if (CommState_GetPlazaMatchStatus() == 0) {
            WiFiPlazaPlayerState_SetStep(savedState, 28);
        }
        break;
    case 25: // waiting for the message
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_SetStep(savedState, entry->nextStep);
        }
        break;
    case 26: // link sync
        delay = entry->syncDelay;
        if (delay > 0) {
            entry->syncDelay = delay - 1;
        }
        if (entry->syncDelay == 0) {
            LinkSync_StartSync(entry->syncId);
            WiFiPlazaPlayerState_SetStep(savedState, 27);
        }
        break;
    case 27:
        entry->syncTimer++;
        if (entry->syncTimer >= 150) {
            LinkSync_StartSync(entry->syncId);
            entry->syncTimer = 0;
        }
        if (LinkSync_IsSynced(entry->syncId) != FALSE) {
            WiFiPlazaPlayerState_SetStep(savedState, entry->nextStep);
        }
        break;
    case 28:
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaGameEntry_FreeMenu(entry, room);
        WiFiPlazaGameStatus_Close(&entry->status, room);
        WiFiPlazaRoom_CloseYesNoMenu(room);
        WiFiPlazaPlayerState_FreeWork(savedState);
        return TRUE;
    }
    return FALSE;
}

BOOL WiFiPlazaGameEntry_EndLink(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room) {
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        CommState_SetErrorWatch(0, 0);
        if (CommState_IsWifiInLobby() == FALSE) {
            CommPlayerInfo_Free();
            CommState_EndPlazaMatch();
        }
        WiFiPlazaRoom_SetMode(room, 1);
        WiFiPlazaPlayerState_SetStep(state, 1);
        break;
    case 1:
        if (CommState_IsWifiInLobby() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL WiFiPlazaGameEntry_ReadSign(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room) {
    WiFiPlazaGameStatus *status = WiFiPlazaPlayerState_GetWork(state);
    u32 game;
    u32 msgId;
    WiFiPlazaGameStatus *newStatus;
    WiFiPlazaObjManager *objManager;

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        newStatus = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaGameStatus));
        switch (WiFiPlazaRoom_GetActionTile(room)) {
        case 0x27:
            msgId = msg_0777_00042;
            game = 0;
            break;
        case 0x28:
            msgId = msg_0777_00041;
            game = 1;
            break;
        case 0x29:
            msgId = msg_0777_00043;
            game = 2;
            break;
        default:
            GF_AssertFail();
            return TRUE;
        }
        WiFiPlazaRoom_BufferActivityName(room, game, 0);
        WiFiPlazaRoom_PrintSign(room, WiFiPlazaRoom_GetMsgString(room, 1, msgId));
        WiFiPlazaGameStatus_Open(newStatus, room, game, FALSE, 0);
        WiFiPlazaGameStatus_UpdateInPlay(newStatus, room, TRUE);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case 1:
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN | PAD_KEY_LEFT | PAD_KEY_RIGHT)) != 0) {
            WiFiPlazaGameStatus_Close(status, room);
            WiFiPlazaRoom_CloseMessage(room);
            objManager = WiFiPlazaRoom_GetObjManager(room);
            WiFiPlazaObj_SetHandler(objManager, WiFiPlazaObj_GetOwnPlayer(objManager), TRUE);
            WiFiPlazaPlayerState_FreeWork(state);
            return TRUE;
        }
        WiFiPlazaGameStatus_UpdateInPlay(status, room, FALSE);
        break;
    }
    return FALSE;
}

void WiFiPlazaGameEntry_PrintMessage(WiFiPlazaGameEntry *entry, WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 msgId, u32 nextStep) {
    WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, msgId));
    entry->nextStep = nextStep;
    WiFiPlazaPlayerState_SetStep(state, 25);
}

void WiFiPlazaGameEntry_StartSync(WiFiPlazaGameEntry *entry, WiFiPlazaPlayerState *state, u8 nextStep, u8 syncId) {
    entry->nextStep = nextStep;
    entry->syncId = syncId;
    entry->syncDelay = 0;
    entry->syncTimer = 0;
    WiFiPlazaPlayerState_SetStep(state, 26);
}

void WiFiPlazaGameEntry_InitMenu(WiFiPlazaGameEntry *entry, WiFiPlazaRoomWork *room, u32 count, u32 firstMsg) {
    u32 i;

    WiFiPlazaRoom_CreateListItems(room, count);
    entry->menuCount = count;
    for (i = 0; i < count; i++) {
        WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 0, firstMsg + i), i);
    }
    entry->menuTemplate = sWiFiPlazaGameEntryMenuTemplate;
    entry->menuTemplate.totalItems = entry->menuCount;
    if (entry->menuTemplate.maxShowed > entry->menuCount) {
        entry->menuTemplate.maxShowed = entry->menuCount;
    }
    entry->menuTemplate.items = WiFiPlazaRoom_GetListItems(room);
}

void WiFiPlazaGameEntry_FreeMenu(WiFiPlazaGameEntry *entry, WiFiPlazaRoomWork *room) {
    WiFiPlazaRoom_DeleteListItems(room);
}

BOOL WiFiPlazaGameEntry_UpdateWaiting(WiFiPlazaGameEntry *entry, WiFiPlazaRoomWork *room, WiFiPlazaGameEntryParam *param, WiFiPlazaPlayerState *state, int playerIndex) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    s32 connection = CommState_GetPlazaMatchStatus();
    u32 members = WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot);

    if (connection == 0) {
        param->result = WIFI_PLAZA_GAME_ENTRY_LINK_ERROR;
        WiFiPlazaPlayerState_SetStep(state, 23);
        return TRUE;
    }
    if (connection == 2) {
        entry->members = members;
        WiFiPlazaPlayerState_SetStep(state, 21);
        return TRUE;
    }
    if (entry->reportedMembers != members) {
        entry->reportedMembers = members;
        if (members != 4) {
            WiFiPlaza_SendActivityNews(lobby, param->game, members, playerIndex, 0, 0, 0, 0);
        }
    }
    entry->timeLeft = WiFiPlazaLobbyClient_GetMatchTimer(param->slot);
    if (entry->timeLeft == 0) {
        if (WiFiPlazaLobbyClient_GetMatchPlayerCount(param->slot) <= 1) {
            param->result = WIFI_PLAZA_GAME_ENTRY_NO_MEMBERS;
            WiFiPlazaPlayerState_SetStep(state, 23);
            return TRUE;
        }
        WiFiPlazaPlayerState_SetStep(state, 22);
        return TRUE;
    }
    if (entry->timeLeft <= 330) {
        WiFiPlazaPlayerState_SetStep(state, 20);
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaGameStatus_Open(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 game, u32 showSeconds, s32 frames) {
    GF_ASSERT(showSeconds < 2);
    status->sessionActive = 0;
    status->space = 0;
    status->accepting = 0;
    status->game = 0;
    ((u8 *)&status->seconds)[0] = 0;
    ((u8 *)&status->seconds)[1] = 0;
    status->showSeconds = 0;
    status->redraw = 0;
    status->showSeconds = showSeconds;
    status->game = game;
    WiFiPlazaGameStatus_SetFrames(status, frames);
    WiFiPlazaRoom_OpenTextWindow(room, 18, 3, 13, sWiFiPlazaGameStatusHeights[showSeconds]);
}

void WiFiPlazaGameStatus_Close(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room) {
    status->sessionActive = 0;
    status->space = 0;
    status->accepting = 0;
    status->game = 0;
    ((u8 *)&status->seconds)[0] = 0;
    ((u8 *)&status->seconds)[1] = 0;
    status->showSeconds = 0;
    status->redraw = 0;
    WiFiPlazaRoom_CloseTextWindow(room);
}

void WiFiPlazaGameStatus_SetFrames(WiFiPlazaGameStatus *status, s32 frames) {
    s16 seconds = frames / 30;

    if (seconds != status->seconds) {
        status->seconds = seconds;
        status->redraw |= WIFI_PLAZA_GAME_STATUS_SECONDS;
    }
}

void WiFiPlazaGameStatus_Update(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 force) {
    WiFiPlazaGameStatus_Draw(status, room, force, FALSE);
}

void WiFiPlazaGameStatus_UpdateInPlay(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 force) {
    WiFiPlazaGameStatus_Draw(status, room, force, TRUE);
}

void WiFiPlazaGameStatus_Draw(WiFiPlazaGameStatus *status, WiFiPlazaRoomWork *room, u32 force, u32 showInPlay) {
    u32 slot;
    u32 value;
    u32 sessionActive;
    volatile u32 savedShowInPlay = showInPlay;
    String *message;
    u8 x;

    if (force != FALSE) {
        status->redraw = 0xFF;
    }
    switch (status->game) {
    case 0:
        slot = 0;
        break;
    case 1:
        slot = 1;
        break;
    case 2:
        slot = 2;
        break;
    default:
        GF_AssertFail();
        slot = 2;
        break;
    }

    sessionActive = WiFiPlazaLobbyClient_IsMatchActive(slot);
    if (status->sessionActive != sessionActive) {
        status->redraw |= WIFI_PLAZA_GAME_STATUS_STATE;
    }
    status->sessionActive = sessionActive;

    value = WiFiPlazaLobbyClient_IsMatchOpen(slot);
    if (status->accepting != value) {
        status->redraw |= WIFI_PLAZA_GAME_STATUS_STATE;
    }
    status->accepting = value;

    if (sessionActive == TRUE) {
        value = WiFiPlazaLobbyClient_GetMatchFreePlaces(slot);
    } else {
        value = 4;
    }
    if (status->space != value) {
        status->redraw |= WIFI_PLAZA_GAME_STATUS_COUNTS;
    }
    status->space = value;

    if (status->redraw & WIFI_PLAZA_GAME_STATUS_TITLE) {
        WiFiPlazaRoom_FillTextWindowRect(room, 0, 0, 104, 16);
        WiFiPlazaRoom_BufferActivityName(room, status->game, 0);
        message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00054);
        WiFiPlazaRoom_PrintTextWindow(room, message, 0, 0);
    }
    if (status->redraw & WIFI_PLAZA_GAME_STATUS_STATE) {
        WiFiPlazaRoom_FillTextWindowRect(room, 0, 16, 104, 16);
        if (savedShowInPlay == TRUE && status->accepting == FALSE && status->sessionActive == TRUE) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00069); // "GAME IN PLAY..."
            x = 104 - FontID_String_GetWidth(0, message, 0);
            WiFiPlazaRoom_PrintTextWindow(room, message, x, 16);
        }
    }
    if (status->redraw & WIFI_PLAZA_GAME_STATUS_COUNTS) {
        WiFiPlazaRoom_FillTextWindowRect(room, 0, 32, 104, 32);
        WiFiPlazaRoom_BufferInt(room, status->space, 1, 1, 2);
        WiFiPlazaRoom_BufferInt(room, 4 - status->space, 1, 0, 2);
        message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00068); // "PLAYERS: / SPACE:"
        WiFiPlazaRoom_PrintTextWindow(room, message, 0, 32);
    }
    if (status->showSeconds == TRUE && (status->redraw & WIFI_PLAZA_GAME_STATUS_SECONDS)) {
        WiFiPlazaRoom_FillTextWindowRect(room, 0, 64, 104, 16);
        WiFiPlazaRoom_BufferInt(room, status->seconds, 2, 0, 2);
        message = WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00015); // "... seconds to go"
        WiFiPlazaRoom_PrintTextWindow(room, message, 0, 64);
    }
    status->redraw = 0;
}
