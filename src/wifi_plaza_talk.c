#include "wifi_plaza_talk.h"

#include "global.h"

#include "constants/sndseq.h"

#include "math_util.h"
#include "overlay_42.h"
#include "sound_playback.h"

// rodata of this overlay (asm/to90_extra2491_anchor.s)
extern const u8 sWiFiPlazaInfoCounterMsgs[8];
extern const ListMenuTemplate sWiFiPlazaTalkMenuTemplate;
extern const u16 sWiFiPlazaTalkGreetingMsgs[8][8]; // by the speaker's and the listener's language
// Pairs { message, variant with the partner's name highlighted }, used when ov45_0222AB28 is set for the partner
extern const u16 sWiFiPlazaTalkMsgPairs[];
extern const u16 sWiFiPlazaTalkMsgPairsHighlighted[];

#pragma force_active on

BOOL WiFiPlazaTalk_Start(void *state, WiFiPlazaRoomWork *room, u32 playerIndex) {
    int length;
    String *text;
    int option;
    int response;
    int myToy;
    int result;
    int direction;
    int partnerIndex;
    void *partnerObj;
    void *ownObj;
    void *objManager;
    void *stateManager;
    int partnerStatus;
    u32 partnerAnswer;
    WiFiPlazaPlayer *me;
    WiFiPlazaPlayer *partner;
    u32 answer;
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaTalk *talk;

    stateManager = WiFiPlazaRoom_GetPlayerStates(room);
    talk = WiFiPlazaPlayerState_GetWork(state);
    partnerStatus = ov45_0222B034(lobby);
    partnerAnswer = ov45_0222B040(lobby);
    if (ov45_0222B06C(lobby) && talk != NULL && talk->talking) {
        WiFiPlazaPlayerState_SetStep(state, 32);
        WiFiPlazaRoom_HideWaitingIcon(room);
        talk->talking = FALSE;
    }
    if (talk != NULL) {
        me = WiFiPlazaWork_GetMyPlayer(lobby);
        if (talk->partnerObj != NULL) {
            partner = WiFiPlazaWork_GetPlayer(lobby, talk->partner);
        } else {
            partner = NULL;
        }
    }
    // the partner left the plaza
    if (WiFiPlazaPlayerState_GetStep(state) && talk->talking && partner == NULL) {
        WiFiPlazaPlayerState_SetStep(state, 32);
        WiFiPlazaRoom_HideWaitingIcon(room);
        talk->talking = FALSE;
    }

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        talk = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaTalk));
        talk->historyPending = 0;
        talk->historyAvatar = 0;
        talk->historyId = 0;
        WiFiPlazaTalkReply_Init(&talk->reply);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownObj = WiFiPlazaObj_GetOwnPlayer(objManager);
        partnerObj = WiFiPlazaObj_FindFacing(objManager, ownObj);
        if (partnerObj == NULL) {
            talk->talking = FALSE;
            WiFiPlazaPlayerState_SetStep(state, 38);
            break;
        }
        partnerIndex = WiFiPlazaObj_GetParam(partnerObj, 4);
        direction = ov42_022282A4(WiFiPlazaObj_GetParam(ownObj, 6));
        ov45_0222AE64(lobby);
        result = WiFiPlazaTalk_Init(talk, lobby, partnerIndex);
        talk->partnerObj = partnerObj;
        ov49_02259130(partnerObj, 0);
        WiFiPlazaObj_SetDirection(partnerObj, direction);
        WiFiPlazaPlayerState_SetActive(stateManager, partnerIndex, 0);
        PlaySE(SEQ_SE_DP_BUTTON9);
        ov45_0222A5E8(lobby, 9);
        if (result == TRUE) {
            WiFiPlazaRoom_ShowPlayerCard(room, partnerIndex, 0);
            WiFiPlazaPlayerState_SetStep(state, 1);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 34);
        }
        break;
    case 1:
        if (ov45_0222AE74(lobby, talk->partner) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, 2);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 34);
        }
        break;
    case 2:
        switch (partnerStatus) {
        case 2:
            ov45_0222B0E8(lobby, talk->partner);
            WiFiPlazaPlayerState_SetStep(state, 3);
            break;
        case 0:
            WiFiPlazaPlayerState_SetStep(state, 34);
            break;
        case 1:
            break;
        case 3:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, 32);
            break;
        }
        break;
    case 3: // greetings
        IncrementGameStat47(lobby);
        ov45_0222B118(lobby, 7);
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->myTimeOfDay, me, partner);
        WiFiPlazaTalk_SetNextStep(talk, 128, 4, state, 37);
        break;
    case 4:
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->partnerTimeOfDay, partner, me);
        WiFiPlazaTalk_SetNextStep(talk, 128, 5, state, 37);
        ov45_0222AED8(lobby, 0);
        break;
    case 5:
        switch (partnerStatus) {
        case 2:
            WiFiPlazaRoom_HideWaitingIcon(room);
            if (ov45_0222B028(lobby)) {
                WiFiPlazaPlayerState_SetStep(state, 6);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 8);
            }
            break;
        case 3:
            if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
                WiFiPlazaRoom_ShowWaitingIcon(room);
            }
            WiFiPlazaTalk_UpdatePartnerWait(talk, room);
            break;
        case 0:
        case 1:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, 32);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case 6:
        if (WiFiPlazaTalk_PrintTimeRemark(talk, lobby, room) == TRUE) {
            WiFiPlazaTalk_SetNextStep(talk, 128, 7, state, 37);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 7);
        }
        break;
    case 7: // "Hello! Nice to meet you."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 40));
        WiFiPlazaTalk_SetNextStep(talk, 128, 9, state, 37);
        break;
    case 8: // "Hi, ...! It's good to see you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_BufferPlayerName(room, talk->me, 1);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetReunionMsg(room, talk->partner, talk->me));
        WiFiPlazaTalk_SetNextStep(talk, 128, 9, state, 37);
        break;
    case 9: // "Yes? What's up?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 46));
        WiFiPlazaTalk_SetNextStep(talk, 128, 11, state, 37);
        break;
    case 10: // "Is there anything else?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 54));
        WiFiPlazaTalk_SetNextStep(talk, 128, 11, state, 37);
        break;
    case 11: // TAP TOYS / CHAT / CANCEL
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 3, 515, 0);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaTalk_StartMenuTimer(talk);
        WiFiPlazaPlayerState_SetStep(state, 12);
        break;
    case 12: {
        int done;

        switch (WiFiPlazaRoom_ProcessList(room)) {
        case 0:
            WiFiPlazaPlayerState_SetStep(state, 13);
            done = TRUE;
            break;
        case 1:
            WiFiPlazaPlayerState_SetStep(state, 19);
            ov45_0222AB94(lobby, playerIndex, WiFiPlazaObj_GetParam(talk->partnerObj, 4));
            done = TRUE;
            break;
        case 2:
            ov45_0222AF80(lobby);
            WiFiPlazaPlayerState_SetStep(state, 33);
            done = TRUE;
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            done = FALSE;
            break;
        }
        if (done == TRUE) {
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
        }
        break;
    }
    case 13: { // compare Tap Toys
        myToy = ov45_0222AAC8(me);
        if (myToy == ov45_0222AAC8(partner)) {
            talk->sameToy = TRUE;
            WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
            WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, FALSE);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 507));
            WiFiPlazaTalk_SetNextStep(talk, 128, 18, state, 37);
            ov45_0222AED8(lobby, 4);
        } else {
            talk->sameToy = FALSE;
            WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
            WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
            WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 2, FALSE);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 508));
            WiFiPlazaTalk_SetNextStep(talk, 128, 14, state, 37);
        }
        break;
    }
    case 14: // GET / CANCEL
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 2, 518, 1);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaTalk_StartMenuTimer(talk);
        WiFiPlazaPlayerState_SetStep(state, 15);
        break;
    case 15: {
        int done;

        switch (WiFiPlazaRoom_ProcessList(room)) {
        case 0: {
            int partnerIndex2;

            ov45_0222AED8(lobby, 3);
            WiFiPlazaPlayerState_SetStep(state, 16);
            partnerIndex2 = WiFiPlazaObj_GetParam(talk->partnerObj, 4);
            ov45_0222ABD0(lobby, playerIndex, partnerIndex2, ov45_0222AAC8(partner));
            done = TRUE;
            break;
        }
        case 1:
            WiFiPlazaPlayerState_SetStep(state, 10);
            done = TRUE;
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            done = FALSE;
            break;
        }
        if (done == TRUE) {
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
        }
        break;
    }
    case 16: // "Received a ... from ...!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, FALSE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 510));
        WiFiPlazaTalk_SetNextStep(talk, 128, 17, state, 37);
        ov45_0222A72C(lobby, ov45_0222AAC8(partner));
        break;
    case 17:
        PlaySE(SEQ_SE_PL_EFF03_4);
        WiFiPlazaPlayerState_SetStep(state, 18);
        break;
    case 18:
        switch (partnerStatus) {
        case 2:
            WiFiPlazaTalk_SetHistory(talk, partner);
            if (talk->sameToy == TRUE) {
                WiFiPlazaPlayerState_SetStep(state, 33);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 10);
            }
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        case 3:
            if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
                WiFiPlazaRoom_ShowWaitingIcon(room);
            }
            WiFiPlazaTalk_UpdatePartnerWait(talk, room);
            break;
        case 0:
        case 1:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, 32);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case 19: // chat: ask a question
        ov45_0222AED8(lobby, 5);
        WiFiPlazaPlayerState_SetStep(state, 20);
        break;
    case 20: // four questions picked at random out of 30
        WiFiPlazaTalkMenu_InitRandom(&talk->menu, room, 30, 4, 519, 0);
        WiFiPlazaRoom_OpenListAt(room, &talk->menu.template, 0, 0, 8, 3, 23);
        WiFiPlazaPlayerState_SetStep(state, 21);
        WiFiPlazaTalk_StartMenuTimer(talk);
        break;
    case 21: {
        response = WiFiPlazaRoom_ProcessList(room);
        switch (response) {
        case LIST_CANCEL:
        case LIST_NOTHING_CHOSEN:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            break;
        default:
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            talk->question = response;
            WiFiPlazaPlayerState_SetStep(state, 22);
            break;
        }
        break;
    }
    case 22: // "What about you?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 477 + talk->question));
        WiFiPlazaTalk_SetNextStep(talk, 128, 23, state, 37);
        break;
    case 23: // our own answer
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 4, (talk->question << 2) + 115, 0);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaPlayerState_SetStep(state, 24);
        break;
    case 24: {
        int question;

        option = WiFiPlazaRoom_ProcessList(room);
        switch (option) {
        case 0:
        case 1:
        case 2:
        case 3:
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
            question = talk->question << 2;
            talk->myAnswer = option + question;
            WiFiPlazaPlayerState_SetStep(state, 25);
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            break;
        }
        break;
    }
    case 25: // "Oh, your favorite color's ..."; our answer goes out once the partner asks for it
        WiFiPlazaTalkReply_Start(&talk->reply, lobby, (u16)(talk->myAnswer + 6), 2);
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 357 + talk->myAnswer));
        WiFiPlazaTalk_SetNextStep(talk, 128, 26, state, 37);
        break;
    case 26: // "Well, if it were me..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 85));
        WiFiPlazaTalk_SetNextStep(talk, 128, 27, state, 37);
        break;
    case 27:
        if (WiFiPlazaTalkReply_IsDone(&talk->reply) == TRUE) {
            if (WiFiPlazaTalkReply_GetStatus(&talk->reply) == 2) {
                WiFiPlazaPlayerState_SetStep(state, 28);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 32);
                WiFiPlazaRoom_HideWaitingIcon(room);
            }
        } else if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
            WiFiPlazaRoom_ShowWaitingIcon(room);
        }
        break;
    case 28:
        switch (partnerStatus) {
        case 2:
            WiFiPlazaTalk_SetHistory(talk, partner);
            WiFiPlazaPlayerState_SetStep(state, 29);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        case 3:
            if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
                WiFiPlazaRoom_ShowWaitingIcon(room);
            }
            WiFiPlazaTalk_UpdatePartnerWait(talk, room);
            break;
        case 0:
        case 1:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, 32);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case 29: // the partner's answer
        answer = (u16)(partnerAnswer - 6);
        if (partnerAnswer >= 126 || partnerAnswer < 6) {
            WiFiPlazaPlayerState_SetStep(state, 35);
            break;
        }
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, answer + 236));
        if (answer == talk->myAnswer) {
            WiFiPlazaTalk_SetNextStep(talk, 128, 30, state, 37);
        } else {
            WiFiPlazaTalk_SetNextStep(talk, 128, 31, state, 37);
        }
        break;
    case 30: // "That's the same as me!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 51));
        WiFiPlazaTalk_SetNextStep(talk, 128, 10, state, 37);
        break;
    case 31: // "I see..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 50));
        WiFiPlazaTalk_SetNextStep(talk, 128, 10, state, 37);
        break;
    case 32: { // "Sorry! Something's come up! I can't talk now!", shown for the time it takes to read
        u32 wait;

        WiFiPlazaRoom_CloseList(room, 0, 0);
        WiFiPlazaTalkMenu_Free(&talk->menu, room);
        text = WiFiPlazaTalk_GetMsg(room, talk->partner, 45);
        WiFiPlazaRoom_PrintMessage(room, text);
        talk->talking = FALSE;
        length = String_GetLength(text);
        wait = ((u32)(WiFiPlazaRoom_GetTextSpeed(room) * length) >> 1) + 60;
        if (wait < 128) {
            wait = 128;
        } else if (wait > 255) {
            wait = 255;
        }
        WiFiPlazaTalk_SetNextStep(talk, (u8)wait, 38, state, 36);
        ov45_0222AFC4(lobby);
        break;
    }
    case 33: // "See you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 43));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, 38, state, 36);
        break;
    case 34: // "...This Trainer seems to be busy."
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 42));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, 38, state, 36);
        break;
    case 35: // "Hmm... That's hard to say..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 356));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, 38, state, 36);
        break;
    case 36:
        WiFiPlazaTalk_WaitTimer(talk, state);
        break;
    case 37:
        WiFiPlazaTalk_WaitMessage(talk, state, room);
        break;
    case 38:
        if (talk->historyPending == TRUE) {
            ov45_0222A704(lobby, talk->historyAvatar, talk->historyId);
        }
        ov45_0222AE64(lobby);
        WiFiPlazaRoom_CloseMessage(room);
        ov45_0222A5E8(lobby, 1);
        {
            void *objManager2 = WiFiPlazaRoom_GetObjManager(room);
            WiFiPlazaObj_SetHandler(objManager2, WiFiPlazaObj_GetOwnPlayer(objManager2), TRUE);
        }
        if (talk->partnerObj != NULL) {
            ov49_02259130(talk->partnerObj, 1);
            WiFiPlazaPlayerState_SetActive(stateManager, WiFiPlazaObj_GetParam(talk->partnerObj, 4), 1);
            WiFiPlazaRoom_ShowOwnCard(room);
        }
        WiFiPlazaTalk_Free(talk);
        WiFiPlazaPlayerState_FreeWork(state);
        return TRUE;
    default:
        break;
    }
    WiFiPlazaTalk_CountMenuTimer(talk);
    WiFiPlazaTalkReply_Update(&talk->reply, lobby);
    return FALSE;
}

#pragma force_active reset

#pragma force_active on

BOOL WiFiPlazaTalk_Answer(void *state, WiFiPlazaRoomWork *room) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaTalk *talk = WiFiPlazaPlayerState_GetWork(state);
    u32 partnerStatus = ov45_0222B034(lobby);
    u32 partnerAnswer = ov45_0222B040(lobby);
    WiFiPlazaPlayer *me;
    WiFiPlazaPlayer *partner;

    if (ov45_0222B06C(lobby) && talk != NULL && talk->talking == TRUE) {
        WiFiPlazaPlayerState_SetStep(state, 26);
        WiFiPlazaRoom_HideWaitingIcon(room);
        talk->talking = FALSE;
    }
    if (talk != NULL) {
        me = WiFiPlazaWork_GetMyPlayer(lobby);
        partner = WiFiPlazaWork_GetPlayer(lobby, talk->partner);
    }
    // the partner left the plaza
    if (WiFiPlazaPlayerState_GetStep(state) != 0 && talk->talking == TRUE && partner == NULL) {
        WiFiPlazaPlayerState_SetStep(state, 26);
        WiFiPlazaRoom_HideWaitingIcon(room);
        talk->talking = FALSE;
    }

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0: {
        u32 partnerIndex;
        int result;

        talk = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaTalk));
        talk->historyPending = 0;
        talk->historyAvatar = 0;
        talk->historyId = 0;
        partnerIndex = ov45_0222B020(lobby);
        result = WiFiPlazaTalk_Init(talk, lobby, partnerIndex);
        PlaySE(SEQ_SE_DP_BUTTON9);
        ov45_0222A5E8(lobby, 9);
        if (result == TRUE) {
            WiFiPlazaRoom_ShowPlayerCard(room, partnerIndex, 0);
            ov45_0222B0E8(lobby, partnerIndex);
            WiFiPlazaPlayerState_SetStep(state, 1);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 26);
        }
        break;
    }
    case 1: // greetings
        if (!ov45_0222B0B0(lobby)) {
            break;
        }
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->partnerTimeOfDay, partner, me);
        WiFiPlazaTalk_SetNextStep(talk, 128, 2, state, 30);
        break;
    case 2:
        IncrementGameStat47(lobby);
        ov45_0222B118(lobby, 7);
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->myTimeOfDay, me, partner);
        WiFiPlazaTalk_SetNextStep(talk, 128, 3, state, 30);
        break;
    case 3:
        switch (partnerStatus) {
        case 3:
            WiFiPlazaRoom_HideWaitingIcon(room);
            ov45_0222AED8(lobby, 0);
            if (ov45_0222B028(lobby)) {
                WiFiPlazaPlayerState_SetStep(state, 4);
            } else {
                WiFiPlazaPlayerState_SetStep(state, 6);
            }
            break;
        case 2:
            if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
                WiFiPlazaRoom_ShowWaitingIcon(room);
            }
            WiFiPlazaTalk_UpdatePartnerWait(talk, room);
            break;
        case 0:
        case 1:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, 26);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case 4:
        if (WiFiPlazaTalk_PrintTimeRemark(talk, lobby, room) == TRUE) {
            WiFiPlazaTalk_SetNextStep(talk, 128, 5, state, 30);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 5);
        }
        break;
    case 5: // "Hello! Nice to meet you."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 40));
        WiFiPlazaTalk_SetNextStep(talk, 128, 7, state, 30);
        break;
    case 6: // "Hi, ...! It's good to see you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_BufferPlayerName(room, talk->me, 1);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetReunionMsg(room, talk->partner, talk->me));
        WiFiPlazaTalk_SetNextStep(talk, 128, 7, state, 30);
        break;
    case 7: // "Hi, do you have a moment?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 47));
        WiFiPlazaTalk_SetNextStep(talk, 128, 9, state, 30);
        break;
    case 8: // "Moving on..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 53));
        WiFiPlazaTalk_SetNextStep(talk, 128, 9, state, 30);
        break;
    case 9: // what the partner chose
        switch (partnerStatus) {
        case 3:
            switch (partnerAnswer) {
            case 3: // gets our Tap Toy
                WiFiPlazaPlayerState_SetStep(state, 10);
                ov45_0222B0BC(lobby);
                talk->answered = TRUE;
                WiFiPlazaTalk_SetHistory(talk, partner);
                break;
            case 4: // has the same Tap Toy
                WiFiPlazaPlayerState_SetStep(state, 14);
                ov45_0222B0BC(lobby);
                talk->answered = TRUE;
                WiFiPlazaTalk_SetHistory(talk, partner);
                break;
            case 5: // asks a question
                ov45_0222AED8(lobby, 1);
                WiFiPlazaPlayerState_SetStep(state, 15);
                break;
            default:
                WiFiPlazaPlayerState_SetStep(state, 26);
                WiFiPlazaRoom_HideWaitingIcon(room);
                break;
            }
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        case 2:
            if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
                WiFiPlazaRoom_ShowWaitingIcon(room);
            }
            WiFiPlazaTalk_UpdatePartnerWait(talk, room);
            break;
        case 0:
            WiFiPlazaPlayerState_SetStep(state, 27);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        case 1:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, 26);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case 10: // "You have a ...? That must be nice!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 511));
        WiFiPlazaTalk_SetNextStep(talk, 128, 11, state, 30);
        break;
    case 11: // "...Can you share your ... with me?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 512));
        WiFiPlazaTalk_SetNextStep(talk, 128, 12, state, 30);
        break;
    case 12: // "Shared the ... with ..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 513));
        WiFiPlazaTalk_SetNextStep(talk, 128, 13, state, 30);
        break;
    case 13: // "Thank you! I'll take good care of it!"
        PlaySE(SEQ_SE_PL_EFF03_4);
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 514));
        WiFiPlazaTalk_SetNextStep(talk, 128, 8, state, 30);
        ov45_0222AED8(lobby, 2);
        WiFiPlazaRoom_SetPlayerCardIcon(room, ov45_0222AAC8(WiFiPlazaWork_GetMyPlayer(lobby)));
        break;
    case 14: // "Oh! You've got a ... just like me."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 507));
        WiFiPlazaTalk_SetNextStep(talk, 128, 27, state, 30);
        ov45_0222AED8(lobby, 2);
        break;
    case 15: // "Let's chat!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 687));
        WiFiPlazaTalk_SetNextStep(talk, 128, 16, state, 30);
        break;
    case 16:
        switch (partnerStatus) {
        case 3:
            WiFiPlazaPlayerState_SetStep(state, 17);
            WiFiPlazaRoom_HideWaitingIcon(room);
            ov45_0222B0BC(lobby);
            talk->answered = TRUE;
            break;
        case 2:
            if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
                WiFiPlazaRoom_ShowWaitingIcon(room);
            }
            WiFiPlazaTalk_UpdatePartnerWait(talk, room);
            break;
        case 0:
        case 1:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, 26);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case 17: // the partner's question and answer
        if (partnerAnswer < 6 || partnerAnswer >= 126) {
            WiFiPlazaPlayerState_SetStep(state, 26);
            break;
        }
        talk->partnerAnswer = partnerAnswer - 6;
        talk->question = (u32)talk->partnerAnswer >> 2;
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 55));
        WiFiPlazaTalk_SetNextStep(talk, 128, 18, state, 30);
        break;
    case 18: // "Well, if it were me..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 85));
        WiFiPlazaTalk_SetNextStep(talk, 128, 19, state, 30);
        break;
    case 19:
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->partnerAnswer + 236));
        WiFiPlazaTalk_SetNextStep(talk, 128, 20, state, 30);
        break;
    case 20: // "What about you?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 477));
        WiFiPlazaTalk_SetNextStep(talk, 128, 21, state, 30);
        break;
    case 21: // four answers + "Stop chatting"
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 5, talk->question * 4 + 115, 2);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaTalk_StartMenuTimer(talk);
        WiFiPlazaPlayerState_SetStep(state, 22);
        break;
    case 22: {
        u32 choice = WiFiPlazaRoom_ProcessList(room);
        int done;

        switch (choice) {
        case 0:
        case 1:
        case 2:
        case 3:
            talk->myAnswer = choice + talk->question * 4;
            ov45_0222AED8(lobby, (u16)(talk->myAnswer + 6));
            WiFiPlazaPlayerState_SetStep(state, 23);
            done = TRUE;
            break;
        case 4:
            talk->myAnswer = 126;
            ov45_0222AED8(lobby, talk->myAnswer);
            WiFiPlazaPlayerState_SetStep(state, 28);
            done = TRUE;
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            done = FALSE;
            break;
        }
        if (done) {
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
            WiFiPlazaTalk_SetHistory(talk, partner);
        }
        break;
    }
    case 23: // "Oh, your favorite color's ..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->myAnswer + 357));
        if (talk->partnerAnswer == talk->myAnswer) {
            WiFiPlazaTalk_SetNextStep(talk, 128, 24, state, 30);
        } else {
            WiFiPlazaTalk_SetNextStep(talk, 128, 25, state, 30);
        }
        break;
    case 24: // "That's the same as me!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 51));
        WiFiPlazaTalk_SetNextStep(talk, 128, 25, state, 30);
        break;
    case 25: // "Thanks for answering!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 52));
        WiFiPlazaTalk_SetNextStep(talk, 128, 8, state, 30);
        break;
    case 26: { // "Sorry! Something's come up!", shown for the time it takes to read
        String *string;
        u32 length;
        u32 duration;

        WiFiPlazaRoom_CloseList(room, 0, 0);
        WiFiPlazaTalkMenu_Free(&talk->menu, room);
        string = WiFiPlazaTalk_GetMsg(room, talk->partner, 45);
        WiFiPlazaRoom_PrintMessage(room, string);
        talk->talking = FALSE;
        length = String_GetLength(string);
        duration = (WiFiPlazaRoom_GetTextSpeed(room) * length >> 1) + 60;
        if (duration < 128) {
            duration = 128;
        } else if (duration > 255) {
            duration = 255;
        }
        WiFiPlazaTalk_SetNextStep(talk, (u8)duration, 31, state, 29);
        ov45_0222AFC4(lobby);
        break;
    }
    case 27: // "See you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 43));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, 31, state, 29);
        break;
    case 28: // "Don't worry. You don't have to answer if you don't want to."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 49));
        WiFiPlazaTalk_SetNextStep(talk, 128, 27, state, 30);
        break;
    case 29:
        WiFiPlazaTalk_WaitTimer(talk, state);
        break;
    case 30:
        WiFiPlazaTalk_WaitMessage(talk, state, room);
        break;
    case 31:
        if (talk->historyPending == TRUE) {
            ov45_0222A704(lobby, talk->historyAvatar, talk->historyId);
        }
        if (!talk->answered) {
            ov45_0222B0D8(lobby, talk->partner);
        }
        ov45_0222AE64(lobby);
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaTalk_Free(talk);
        WiFiPlazaPlayerState_FreeWork(state);
        ov45_0222A5E8(lobby, 1);
        {
            void *objManager = WiFiPlazaRoom_GetObjManager(room);
            WiFiPlazaObj_SetHandler(objManager, WiFiPlazaObj_GetOwnPlayer(objManager), TRUE);
        }
        WiFiPlazaRoom_ShowOwnCard(room);
        return TRUE;
    }
    WiFiPlazaTalk_CountMenuTimer(talk);
    return FALSE;
}

#pragma force_active reset

// Prints a message and waits for it (step 23), then goes on with step `next`
#define PRINT_STAFF_MSG(room, work, msgId, next)                                            \
    do {                                                                                    \
        WiFiPlazaRoom_PrintMessage((room), WiFiPlazaRoom_GetMsgString((room), 1, (msgId))); \
        (work)->nextStep = (next);                                                          \
    } while (0)

typedef struct WiFiPlazaInfoCounter {
    u32 nextStep;
    WiFiPlazaTalkMenu menu;
} WiFiPlazaInfoCounter;

BOOL WiFiPlazaTalk_InfoCounter(void *state, WiFiPlazaRoomWork *room) {
    WiFiPlazaInfoCounter *counter = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    u32 selected;
    u32 choice;

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaInfoCounter));
        PlaySE(SEQ_SE_DP_SELECT);
        if (ov45_0222A330(lobby) == 1) {
            WiFiPlazaPlayerState_SetStep(state, 2);
        } else if (ov45_0222A374(lobby) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, 1);
        } else {
            WiFiPlazaPlayerState_SetStep(state, 3);
        }
        break;
    case 1: // "The Pokemon Parade is on now!"
        PRINT_STAFF_MSG(room, counter, 0x4E, 24);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 2: // "Your Wi-Fi Plaza visit is nearly over."
        PRINT_STAFF_MSG(room, counter, 3, 24);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 3: // "This is the general-inquiries counter."
        PRINT_STAFF_MSG(room, counter, 0xF, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 4: // "What would you like to know about?"
        PRINT_STAFF_MSG(room, counter, 0x10, 5);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 5:
        WiFiPlazaTalkMenu_InitInfoCounter(&counter->menu, room);
        WiFiPlazaRoom_OpenListAt(room, &counter->menu.template, 0, 0, 16, 1, 15);
        WiFiPlazaPlayerState_SetStep(state, 6);
        break;
    case 6:
        selected = FALSE;
        choice = WiFiPlazaRoom_ProcessList(room);
        switch (choice) {
        default:
            if (choice == (u32)LIST_CANCEL) {
                goto cancelTopics;
            }
            break;
        case 0: // Tap Toys
            WiFiPlazaPlayerState_SetStep(state, 7);
            selected = TRUE;
            break;
        case 1: // Plaza Games
            WiFiPlazaPlayerState_SetStep(state, 8);
            selected = TRUE;
            break;
        case 2: // Visitor Profiles
            WiFiPlazaPlayerState_SetStep(state, 13);
            selected = TRUE;
            break;
        case 3: // Plaza News
            WiFiPlazaPlayerState_SetStep(state, 14);
            selected = TRUE;
            break;
        case 4: // Footprint Stamp
            WiFiPlazaPlayerState_SetStep(state, 15);
            selected = TRUE;
            break;
        case 5: // Plaza Survey
            WiFiPlazaPlayerState_SetStep(state, 21);
            selected = TRUE;
            break;
        case 6: // Time Events
            WiFiPlazaPlayerState_SetStep(state, 16);
            selected = TRUE;
            break;
cancelTopics:
            PlaySE(SEQ_SE_DP_SELECT);
        case 7: // Exit
            WiFiPlazaPlayerState_SetStep(state, 22);
            selected = TRUE;
            break;
        }
        if (selected == TRUE) {
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&counter->menu, room);
        }
        break;
    case 7:
        PRINT_STAFF_MSG(room, counter, 0x17, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 8:
        WiFiPlazaTalkMenu_InitPlazaGames(&counter->menu, room);
        WiFiPlazaRoom_OpenList(room, &counter->menu.template, 0, 0);
        WiFiPlazaPlayerState_SetStep(state, 9);
        break;
    case 9:
        selected = FALSE;
        choice = WiFiPlazaRoom_ProcessList(room);
        switch (choice) {
        default:
            if (choice == (u32)LIST_CANCEL) {
                goto cancelGames;
            }
            break;
        case 0:
            WiFiPlazaPlayerState_SetStep(state, 10);
            selected = TRUE;
            break;
        case 1:
            WiFiPlazaPlayerState_SetStep(state, 11);
            selected = TRUE;
            break;
        case 2:
            WiFiPlazaPlayerState_SetStep(state, 12);
            selected = TRUE;
            break;
cancelGames:
            PlaySE(SEQ_SE_DP_SELECT);
        case 3:
            WiFiPlazaPlayerState_SetStep(state, 4);
            selected = TRUE;
            break;
        }
        if (selected == TRUE) {
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&counter->menu, room);
        }
        break;
    case 10:
        WiFiPlazaRoom_BufferActivityName(room, 0, 0);
        PRINT_STAFF_MSG(room, counter, 0x18, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 11:
        WiFiPlazaRoom_BufferActivityName(room, 1, 0);
        PRINT_STAFF_MSG(room, counter, 0x19, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 12:
        WiFiPlazaRoom_BufferActivityName(room, 2, 0);
        PRINT_STAFF_MSG(room, counter, 0x1A, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 13:
        PRINT_STAFF_MSG(room, counter, 0x12, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 14:
        PRINT_STAFF_MSG(room, counter, 0x13, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 15:
        PRINT_STAFF_MSG(room, counter, 0x14, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 16:
        WiFiPlazaTalkMenu_InitTimeEvents(&counter->menu, room);
        WiFiPlazaRoom_OpenList(room, &counter->menu.template, 0, 0);
        WiFiPlazaPlayerState_SetStep(state, 17);
        break;
    case 17:
        selected = FALSE;
        choice = WiFiPlazaRoom_ProcessList(room);
        switch (choice) {
        default:
            if (choice == (u32)LIST_CANCEL) {
                goto cancelEvents;
            }
            break;
        case 0:
            WiFiPlazaPlayerState_SetStep(state, 18);
            selected = TRUE;
            break;
        case 1:
            WiFiPlazaPlayerState_SetStep(state, 19);
            selected = TRUE;
            break;
        case 2:
            WiFiPlazaPlayerState_SetStep(state, 20);
            selected = TRUE;
            break;
cancelEvents:
            PlaySE(SEQ_SE_DP_SELECT);
        case 3:
            WiFiPlazaPlayerState_SetStep(state, 4);
            selected = TRUE;
            break;
        }
        if (selected == TRUE) {
            WiFiPlazaRoom_CloseList(room, 0, 0);
            WiFiPlazaTalkMenu_Free(&counter->menu, room);
        }
        break;
    case 18:
        PRINT_STAFF_MSG(room, counter, 0x1B, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 19:
        PRINT_STAFF_MSG(room, counter, 0x1C, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 20:
        PRINT_STAFF_MSG(room, counter, 0x1D, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 21:
        PRINT_STAFF_MSG(room, counter, 0x1E, 4);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 22: // "We hope you'll enjoy yourself in the Wi-Fi Plaza"
        PRINT_STAFF_MSG(room, counter, 0x16, 24);
        WiFiPlazaPlayerState_SetStep(state, 23);
        break;
    case 23:
        if (WiFiPlazaRoom_IsMessageFinished(room)) {
            WiFiPlazaPlayerState_SetStep(state, counter->nextStep);
        }
        break;
    case 24: {
        void *objManager;

        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaRoom_CloseMessage(room);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        WiFiPlazaObj_SetHandler(objManager, WiFiPlazaObj_GetOwnPlayer(objManager), TRUE);
        return TRUE;
    }
    }
    return FALSE;
}

BOOL WiFiPlazaTalk_Reception(void *state, WiFiPlazaRoomWork *room) {
    WiFiPlazaWork *lobby;
    String *message;

    WiFiPlazaPlayerState_GetWork(state);
    lobby = WiFiPlazaRoom_GetLobby(room);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        PlaySE(SEQ_SE_DP_SELECT);
        if (ov45_0222A330(lobby) == 0) {
            if (ov45_0222A374(lobby) == TRUE) { // the Pokemon Parade is on
                message = WiFiPlazaRoom_GetMsgString(room, 1, 0x4E);
                WiFiPlazaRoom_PrintMessage(room, message);
                WiFiPlazaPlayerState_SetStep(state, 1);
                goto done;
            }
        } else { // the visit is nearly over
            message = WiFiPlazaRoom_GetMsgString(room, 1, 3);
            WiFiPlazaRoom_PrintMessage(room, message);
            WiFiPlazaPlayerState_SetStep(state, 1);
            goto done;
        }
        // what the receptionist recommends
        switch (ov45_0222B134(lobby)) {
        case 0:
            WiFiPlazaRoom_BufferActivityName(room, 0, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x49);
            break;
        case 1:
            WiFiPlazaRoom_BufferActivityName(room, 1, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x49);
            break;
        case 2:
            WiFiPlazaRoom_BufferActivityName(room, 2, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x49);
            break;
        case 3:
            WiFiPlazaRoom_BufferActivityName(room, 5, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x4D);
            break;
        case 4:
            WiFiPlazaRoom_BufferActivityName(room, 6, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x4D);
            break;
        case 5: // Footprint Stamp
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x4A);
            break;
        case 6: // Plaza Survey
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x4F);
            break;
        case 7: // chatting with friends
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x4B);
            break;
        case 8:
        default:
            message = WiFiPlazaRoom_GetMsgString(room, 1, 0x4C);
            break;
        }
        WiFiPlazaRoom_PrintMessage(room, message);
        WiFiPlazaPlayerState_SetStep(state, 1);
        break;
    case 1:
        if (WiFiPlazaRoom_IsMessageFinished(room)) {
            WiFiPlazaPlayerState_SetStep(state, 2);
        }
        break;
    case 2: {
        void *objManager;
        void *ownObj;

        WiFiPlazaRoom_CloseMessage(room);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownObj = WiFiPlazaObj_GetOwnPlayer(objManager);
        WiFiPlazaObj_SetHandler(objManager, ownObj, TRUE);
        return TRUE;
    }
    }
done:
    return FALSE;
}

int WiFiPlazaTalk_GetTimeOfDay(WiFiPlazaPlayer *player) {
    // the player's local time; the hour is its low byte
    struct {
        u32 time;
        u32 copy;
    } localTime;

    localTime.time = ov45_0222A9C8(player);
    localTime.copy = localTime.time;
    switch (*(u8 *)&localTime.copy) {
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        return 0; // morning
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        return 1; // day
    case 0:
    case 1:
    case 2:
    case 3:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
        return 2; // night
    default:
        GF_AssertFail();
        return 1;
    }
}

String *WiFiPlazaTalk_GetMsg(WiFiPlazaRoomWork *room, u32 playerIndex, u32 msgId) {
    u32 i;

    if (ov45_0222AB28(WiFiPlazaRoom_GetLobby(room), playerIndex) == 1) {
        for (i = 0; i < 0x163; i++) {
            if (msgId == sWiFiPlazaTalkMsgPairs[i * 2]) {
                msgId = sWiFiPlazaTalkMsgPairsHighlighted[i * 2];
                break;
            }
        }
    }
    return WiFiPlazaRoom_GetMsgString(room, 3, msgId);
}

String *WiFiPlazaTalk_GetReunionMsg(WiFiPlazaRoomWork *room, u32 speaker, u32 listener) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    int speakerHighlighted = ov45_0222AB28(lobby, speaker);
    int listenerHighlighted = ov45_0222AB28(lobby, listener);
    u32 msgId = 0x29;

    if (speakerHighlighted == 1 && listenerHighlighted == 1) {
        msgId = 0x2AA;
    } else if (speakerHighlighted == 1 && listenerHighlighted == 0) {
        msgId = 0x226;
    } else if (speakerHighlighted == 0 && listenerHighlighted == 1) {
        msgId = 0x2A9;
    }
    return WiFiPlazaRoom_GetMsgString(room, 3, msgId);
}

BOOL WiFiPlazaTalk_Init(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, s32 partner) {
    WiFiPlazaPlayer *me;
    WiFiPlazaPlayer *partnerPlayer;

    if (partner == -1) {
        GF_AssertFail();
    }
    talk->partner = partner;
    talk->me = ov45_0222A53C(lobby);
    talk->talking = TRUE;
    me = WiFiPlazaWork_GetMyPlayer(lobby);
    partnerPlayer = WiFiPlazaWork_GetPlayer(lobby, talk->partner);
    if (partnerPlayer == NULL) {
        return FALSE;
    }
    talk->myTimeOfDay = WiFiPlazaTalk_GetTimeOfDay(me);
    talk->partnerTimeOfDay = WiFiPlazaTalk_GetTimeOfDay(partnerPlayer);
    return TRUE;
}

void WiFiPlazaTalk_Free(WiFiPlazaTalk *talk) {
}

void WiFiPlazaTalk_SetNextStep(WiFiPlazaTalk *talk, u8 frames, u8 nextStep, void *state, u8 waitStep) {
    talk->timer = frames;
    talk->nextStep = nextStep;
    WiFiPlazaPlayerState_SetStep(state, waitStep);
}

void WiFiPlazaTalk_WaitTimer(WiFiPlazaTalk *talk, void *state) {
    if (talk->timer != 0) {
        talk->timer--;
    } else {
        WiFiPlazaPlayerState_SetStep(state, talk->nextStep);
    }
}

void WiFiPlazaTalk_WaitMessage(WiFiPlazaTalk *talk, void *state, WiFiPlazaRoomWork *room) {
    if (WiFiPlazaRoom_IsMessageFinished(room)) {
        WiFiPlazaPlayerState_SetStep(state, talk->nextStep);
    }
}

void WiFiPlazaTalk_PrintGreeting(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, u32 timeOfDay, WiFiPlazaPlayer *speaker, WiFiPlazaPlayer *listener) {
    u32 speakerLanguage;
    u32 listenerLanguage;
    u32 msgId;

    WiFiPlazaTalk_BufferGreeting(room, 0, timeOfDay, speaker);
    WiFiPlazaTalk_BufferGreeting(room, 1, timeOfDay, listener);
    speakerLanguage = WiFiPlazaPlayer_GetLanguage(speaker);
    listenerLanguage = WiFiPlazaPlayer_GetLanguage(listener);
    msgId = sWiFiPlazaTalkGreetingMsgs[speakerLanguage][listenerLanguage];
    WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 3, msgId));
}

void WiFiPlazaTalk_BufferGreeting(WiFiPlazaRoomWork *room, u32 slot, u32 timeOfDay, WiFiPlazaPlayer *player) {
    switch (WiFiPlazaPlayer_GetLanguage(player)) {
    case LANGUAGE_JAPANESE:
        WiFiPlazaRoom_BufferJPGreeting(room, slot, timeOfDay);
        return;
    case LANGUAGE_ENGLISH:
        WiFiPlazaRoom_BufferENGreeting(room, slot, timeOfDay);
        return;
    case LANGUAGE_FRENCH:
        WiFiPlazaRoom_BufferFRGreeting(room, slot, timeOfDay);
        return;
    case LANGUAGE_ITALIAN:
        WiFiPlazaRoom_BufferITGreeting(room, slot, timeOfDay);
        return;
    case LANGUAGE_GERMAN:
        WiFiPlazaRoom_BufferDEGreeting(room, slot, timeOfDay);
        return;
    case LANGUAGE_SPANISH:
        WiFiPlazaRoom_BufferSPGreeting(room, slot, timeOfDay);
        return;
    default:
        WiFiPlazaRoom_BufferENGreeting(room, slot, timeOfDay);
        return;
    }
}

BOOL WiFiPlazaTalk_PrintTimeRemark(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room) {
    WiFiPlazaPlayer *me = WiFiPlazaWork_GetMyPlayer(lobby);
    WiFiPlazaPlayer *partner = WiFiPlazaWork_GetPlayer(lobby, talk->partner);
    u32 msgId;

    if (WiFiPlazaPlayer_IsLanguageValid(me) == FALSE) {
        return FALSE;
    }
    if (WiFiPlazaPlayer_IsLanguageValid(partner) == FALSE) {
        return FALSE;
    }
    // "...Wow, it's that time there?"
    if (talk->myTimeOfDay != talk->partnerTimeOfDay) {
        switch (talk->partnerTimeOfDay) {
        case 0:
            msgId = 0x27;
            break;
        case 1:
            msgId = 0x25;
            break;
        case 2:
            msgId = 0x26;
            break;
        }
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 3, msgId));
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaTalk_BufferInstrument(WiFiPlazaTalk *talk, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, u32 slot, u32 mine) {
    WiFiPlazaPlayer *me = WiFiPlazaWork_GetMyPlayer(lobby);
    WiFiPlazaPlayer *partner = WiFiPlazaWork_GetPlayer(lobby, talk->partner);
    u32 instrument;

    if (mine == FALSE) {
        instrument = ov45_0222AAC8(partner);
    } else {
        instrument = ov45_0222AAC8(me);
    }
    WiFiPlazaRoom_BufferInstrumentName(room, instrument, slot);
}

void WiFiPlazaTalk_UpdatePartnerWait(WiFiPlazaTalk *talk, WiFiPlazaRoomWork *room) {
    if (ov45_0222B094(WiFiPlazaRoom_GetLobby(room)) == 450) { // "Umm... Hold on a second..."
        WiFiPlazaRoom_HideWaitingIcon(room);
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 0x2AC));
        WiFiPlazaRoom_ShowWaitingIcon(room);
    }
}

void WiFiPlazaTalk_StartMenuTimer(WiFiPlazaTalk *talk) {
    talk->menuTimer = 0;
    talk->menuOpen = TRUE;
}

void WiFiPlazaTalk_StopMenuTimer(WiFiPlazaTalk *talk) {
    talk->menuTimer = 0;
    talk->menuOpen = FALSE;
}

void WiFiPlazaTalk_UpdateMenuWait(WiFiPlazaTalk *talk, WiFiPlazaRoomWork *room) {
    String *message;

    if (talk->menuOpen == TRUE && (s16)talk->menuTimer == 450) { // "What's wrong? Are you OK?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        message = WiFiPlazaTalk_GetMsg(room, talk->partner, 0x2AB);
        WiFiPlazaRoom_PrintMessage(room, message);
    }
}

void WiFiPlazaTalk_CountMenuTimer(WiFiPlazaTalk *talk) {
    s16 frames;

    if (talk->menuOpen == TRUE) {
        frames = talk->menuTimer;
        if (frames <= 900) {
            talk->menuTimer = frames + 1;
        }
    }
}

void WiFiPlazaTalk_SetHistory(WiFiPlazaTalk *talk, WiFiPlazaPlayer *partner) {
    u32 avatar = ov45_0222A9A4(partner);

    talk->historyPending = TRUE;
    talk->historyAvatar = avatar;
    talk->historyId = ov45_0222A99C(partner);
}

void WiFiPlazaTalkMenu_Init(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room, u32 count, u32 firstMsg, u32 lastItem) {
    u32 i;

    WiFiPlazaRoom_CreateListItems(room, count);
    menu->count = count;
    for (i = 0; i < count; i++) {
        String *text;

        if (lastItem != 0 && i == count - 1) {
            if (lastItem == 1) {
                text = WiFiPlazaRoom_GetMsgString(room, 3, 0x205); // CANCEL
            } else {
                text = WiFiPlazaRoom_GetMsgString(room, 3, 0xEB); // Stop chatting
            }
        } else {
            text = WiFiPlazaRoom_GetMsgString(room, 3, firstMsg + i);
        }
        WiFiPlazaRoom_AddListItem(room, text, i);
    }
    menu->template = sWiFiPlazaTalkMenuTemplate;
    menu->template.totalItems = menu->count;
    if (menu->template.maxShowed > menu->count) {
        menu->template.maxShowed = menu->count;
    }
    menu->template.items = WiFiPlazaRoom_GetListItems(room);
}

void WiFiPlazaTalkMenu_InitRandom(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room, u32 range, u32 count, u32 firstMsg, u32 lastItem) {
    u32 i;

    WiFiPlazaRoom_CreateListItemsWithValue(room, count, range);
    menu->count = count;
    for (i = 0; i < count; i++) {
        String *text;
        u32 choice;

        if (lastItem != 0 && i == count - 1) {
            choice = range;
            if (lastItem == 1) {
                text = WiFiPlazaRoom_GetMsgString(room, 3, 0x205);
            } else {
                text = WiFiPlazaRoom_GetMsgString(room, 3, 0xEB);
            }
        } else {
            do {
                choice = MTRandom() % range;
            } while (WiFiPlazaRoom_HasListItemValue(room, choice) == TRUE);
            text = WiFiPlazaRoom_GetMsgString(room, 3, firstMsg + choice);
        }
        WiFiPlazaRoom_AddListItem(room, text, choice);
    }
    menu->template = sWiFiPlazaTalkMenuTemplate;
    menu->template.totalItems = menu->count;
    if (menu->template.maxShowed > menu->count) {
        menu->template.maxShowed = menu->count;
    }
    menu->template.items = WiFiPlazaRoom_GetListItems(room);
}

void WiFiPlazaTalkMenu_InitInfoCounter(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room) {
    s32 i;

    WiFiPlazaRoom_CreateListItems(room, 8);
    menu->count = 8;
    for (i = 0; i < 8; i++) {
        WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, sWiFiPlazaInfoCounterMsgs[i]), i);
    }
    menu->template = sWiFiPlazaTalkMenuTemplate;
    menu->template.totalItems = menu->count;
    menu->template.maxShowed = menu->count;
    menu->template.items = WiFiPlazaRoom_GetListItems(room);
}

void WiFiPlazaTalkMenu_InitTimeEvents(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room) {
    s32 i;

    WiFiPlazaRoom_CreateListItems(room, 4);
    menu->count = 4;
    for (i = 0; i < 3; i++) {
        WiFiPlazaRoom_BufferEventName(room, i, 0);
        WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, 0x27), i);
    }
    WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, 0x25), 3); // EXIT
    menu->template = sWiFiPlazaTalkMenuTemplate;
    menu->template.totalItems = menu->count;
    menu->template.maxShowed = menu->count;
    menu->template.items = WiFiPlazaRoom_GetListItems(room);
}

void WiFiPlazaTalkMenu_InitPlazaGames(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room) {
    s32 i;

    WiFiPlazaRoom_CreateListItems(room, 4);
    menu->count = 4;
    for (i = 0; i < 3; i++) {
        WiFiPlazaRoom_BufferActivityName(room, i, 0);
        WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, 0x26), i);
    }
    WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, 0x25), 3); // EXIT
    menu->template = sWiFiPlazaTalkMenuTemplate;
    menu->template.totalItems = menu->count;
    menu->template.maxShowed = menu->count;
    menu->template.items = WiFiPlazaRoom_GetListItems(room);
}

void WiFiPlazaTalkMenu_Free(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room) {
    WiFiPlazaRoom_DeleteListItems(room);
}

void WiFiPlazaTalkReply_Init(WiFiPlazaTalkReply *reply) {
    reply->active = FALSE;
}

void WiFiPlazaTalkReply_Start(WiFiPlazaTalkReply *reply, WiFiPlazaWork *lobby, u16 value, u16 awaited) {
    if (reply->active != FALSE) {
        GF_AssertFail();
    }
    reply->active = TRUE;
    reply->value = value;
    reply->awaited = awaited;
    reply->startStatus = ov45_0222B034(lobby);
}

void WiFiPlazaTalkReply_Update(WiFiPlazaTalkReply *reply, WiFiPlazaWork *lobby) {
    if (reply->active != FALSE) {
        reply->status = ov45_0222B034(lobby);
        if (reply->status == reply->awaited) {
            ov45_0222AED8(lobby, reply->value);
            reply->active = FALSE;
            return;
        }
        if (reply->status != reply->startStatus) {
            reply->active = FALSE;
        }
    }
}

BOOL WiFiPlazaTalkReply_IsDone(WiFiPlazaTalkReply *reply) {
    return reply->active != TRUE;
}

u16 WiFiPlazaTalkReply_GetStatus(WiFiPlazaTalkReply *reply) {
    return reply->status;
}
