#include "msgdata/msg/msg_0766.h"
#include "msgdata/msg/msg_0777.h"

#include "wifi_plaza_talk.h"

#include "global.h"

#include "constants/sndseq.h"

#include "math_util.h"
#include "overlay_42.h"
#include "sound_playback.h"

enum WiFiPlazaTalkStep {
    WIFI_PLAZA_TALK_START,
    WIFI_PLAZA_TALK_REQUEST,
    WIFI_PLAZA_TALK_WAIT_ACCEPT,
    WIFI_PLAZA_TALK_GREET,
    WIFI_PLAZA_TALK_PARTNER_GREETING,
    WIFI_PLAZA_TALK_WAIT_GREETINGS,
    WIFI_PLAZA_TALK_TIME_REMARK,
    WIFI_PLAZA_TALK_FIRST_MEETING,
    WIFI_PLAZA_TALK_REUNION,
    WIFI_PLAZA_TALK_WHATS_UP,
    WIFI_PLAZA_TALK_ANYTHING_ELSE,
    WIFI_PLAZA_TALK_OPEN_MENU,
    WIFI_PLAZA_TALK_HANDLE_MENU,
    WIFI_PLAZA_TALK_COMPARE_TAP_TOYS,
    WIFI_PLAZA_TALK_OPEN_TAP_TOY_MENU,
    WIFI_PLAZA_TALK_HANDLE_TAP_TOY_MENU,
    WIFI_PLAZA_TALK_RECEIVE_TAP_TOY,
    WIFI_PLAZA_TALK_TAP_TOY_SOUND,
    WIFI_PLAZA_TALK_WAIT_TAP_TOY_DONE,
    WIFI_PLAZA_TALK_START_CHAT,
    WIFI_PLAZA_TALK_OPEN_QUESTION_MENU,
    WIFI_PLAZA_TALK_HANDLE_QUESTION_MENU,
    WIFI_PLAZA_TALK_WHAT_ABOUT_YOU,
    WIFI_PLAZA_TALK_OPEN_ANSWER_MENU,
    WIFI_PLAZA_TALK_HANDLE_ANSWER_MENU,
    WIFI_PLAZA_TALK_SAY_ANSWER,
    WIFI_PLAZA_TALK_IF_IT_WERE_ME,
    WIFI_PLAZA_TALK_WAIT_ANSWER_SENT,
    WIFI_PLAZA_TALK_WAIT_PARTNER_ANSWER,
    WIFI_PLAZA_TALK_PARTNER_ANSWER,
    WIFI_PLAZA_TALK_SAME_ANSWER,
    WIFI_PLAZA_TALK_OTHER_ANSWER,
    WIFI_PLAZA_TALK_INTERRUPTED,
    WIFI_PLAZA_TALK_GOODBYE,
    WIFI_PLAZA_TALK_BUSY,
    WIFI_PLAZA_TALK_HARD_TO_SAY,
    WIFI_PLAZA_TALK_WAIT_TIMER,
    WIFI_PLAZA_TALK_WAIT_MESSAGE,
    WIFI_PLAZA_TALK_END,
};

enum WiFiPlazaAnswerStep {
    WIFI_PLAZA_ANSWER_START,
    WIFI_PLAZA_ANSWER_PARTNER_GREETING,
    WIFI_PLAZA_ANSWER_GREET,
    WIFI_PLAZA_ANSWER_WAIT_GREETINGS,
    WIFI_PLAZA_ANSWER_TIME_REMARK,
    WIFI_PLAZA_ANSWER_FIRST_MEETING,
    WIFI_PLAZA_ANSWER_REUNION,
    WIFI_PLAZA_ANSWER_HAVE_A_MOMENT,
    WIFI_PLAZA_ANSWER_MOVING_ON,
    WIFI_PLAZA_ANSWER_WAIT_PARTNER_CHOICE,
    WIFI_PLAZA_ANSWER_NICE_TAP_TOY,
    WIFI_PLAZA_ANSWER_CAN_YOU_SHARE,
    WIFI_PLAZA_ANSWER_SHARED_TAP_TOY,
    WIFI_PLAZA_ANSWER_THANK_YOU,
    WIFI_PLAZA_ANSWER_SAME_TAP_TOY,
    WIFI_PLAZA_ANSWER_LETS_CHAT,
    WIFI_PLAZA_ANSWER_WAIT_QUESTION,
    WIFI_PLAZA_ANSWER_PARTNER_QUESTION,
    WIFI_PLAZA_ANSWER_IF_IT_WERE_ME,
    WIFI_PLAZA_ANSWER_PARTNER_ANSWER,
    WIFI_PLAZA_ANSWER_WHAT_ABOUT_YOU,
    WIFI_PLAZA_ANSWER_OPEN_ANSWER_MENU,
    WIFI_PLAZA_ANSWER_HANDLE_ANSWER_MENU,
    WIFI_PLAZA_ANSWER_SAY_ANSWER,
    WIFI_PLAZA_ANSWER_SAME_ANSWER,
    WIFI_PLAZA_ANSWER_THANKS_FOR_ANSWERING,
    WIFI_PLAZA_ANSWER_INTERRUPTED,
    WIFI_PLAZA_ANSWER_GOODBYE,
    WIFI_PLAZA_ANSWER_NO_NEED_TO_ANSWER,
    WIFI_PLAZA_ANSWER_WAIT_TIMER,
    WIFI_PLAZA_ANSWER_WAIT_MESSAGE,
    WIFI_PLAZA_ANSWER_END,
};

// rodata of this overlay (asm/wifi_plaza_map_rodata.s)
extern const u8 sWiFiPlazaInfoCounterMsgs[8];
extern const ListMenuTemplate sWiFiPlazaTalkMenuTemplate;
extern const u16 sWiFiPlazaTalkGreetingMsgs[8][8]; // by the speaker's and the listener's language
// Pairs { message, variant with the partner's name highlighted }, used when WiFiPlazaWork_IsSpecialVisitor is set for the partner
extern const u16 sWiFiPlazaTalkMsgPairs[];
extern const u16 sWiFiPlazaTalkMsgPairsHighlighted[];

#pragma force_active on

BOOL WiFiPlazaTalk_Start(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 playerIndex) {
    int length;
    String *text;
    int option;
    int response;
    int myToy;
    int result;
    int direction;
    int partnerIndex;
    WiFiPlazaObj *partnerObj;
    WiFiPlazaObj *ownObj;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaPlayerStateManager *stateManager;
    int partnerStatus;
    u32 partnerAnswer;
    WiFiPlazaPlayer *me;
    WiFiPlazaPlayer *partner;
    u32 answer;
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaTalk *talk;

    stateManager = WiFiPlazaRoom_GetPlayerStates(room);
    talk = WiFiPlazaPlayerState_GetWork(state);
    partnerStatus = WiFiPlazaWork_GetTalkState(lobby);
    partnerAnswer = WiFiPlazaWork_GetTalkPartnerMessage(lobby);
    if (WiFiPlazaWork_IsTalkOver(lobby) && talk != NULL && talk->talking) {
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_INTERRUPTED);
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
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_INTERRUPTED);
        WiFiPlazaRoom_HideWaitingIcon(room);
        talk->talking = FALSE;
    }

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_TALK_START:
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
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_END);
            break;
        }
        partnerIndex = WiFiPlazaObj_GetParam(partnerObj, 4);
        direction = Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(ownObj, 6));
        WiFiPlazaWork_ResetTalk(lobby);
        result = WiFiPlazaTalk_Init(talk, lobby, partnerIndex);
        talk->partnerObj = partnerObj;
        WiFiPlazaObj_SetAvatarMode(partnerObj, 0);
        WiFiPlazaObj_SetDirection(partnerObj, direction);
        WiFiPlazaPlayerState_SetActive(stateManager, partnerIndex, 0);
        PlaySE(SEQ_SE_DP_BUTTON9);
        WiFiPlazaWork_SetActivity(lobby, 9);
        if (result == TRUE) {
            WiFiPlazaRoom_ShowPlayerCard(room, partnerIndex, 0);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_REQUEST);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_BUSY);
        }
        break;
    case WIFI_PLAZA_TALK_REQUEST:
        if (WiFiPlazaWork_StartTalk(lobby, talk->partner) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_WAIT_ACCEPT);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_BUSY);
        }
        break;
    case WIFI_PLAZA_TALK_WAIT_ACCEPT:
        switch (partnerStatus) {
        case 2:
            WiFiPlazaWork_SetTalkedWith(lobby, talk->partner);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_GREET);
            break;
        case 0:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_BUSY);
            break;
        case 1:
            break;
        case 3:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_INTERRUPTED);
            break;
        }
        break;
    case WIFI_PLAZA_TALK_GREET: // greetings
        IncrementGameStat47(lobby);
        WiFiPlazaWork_MarkActivityDone(lobby, 7);
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->myTimeOfDay, me, partner);
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_PARTNER_GREETING, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_PARTNER_GREETING:
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->partnerTimeOfDay, partner, me);
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_WAIT_GREETINGS, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        WiFiPlazaWork_AdvanceTalk(lobby, 0);
        break;
    case WIFI_PLAZA_TALK_WAIT_GREETINGS:
        switch (partnerStatus) {
        case 2:
            WiFiPlazaRoom_HideWaitingIcon(room);
            if (WiFiPlazaWork_IsFirstTalk(lobby)) {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_TIME_REMARK);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_REUNION);
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
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_INTERRUPTED);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case WIFI_PLAZA_TALK_TIME_REMARK:
        if (WiFiPlazaTalk_PrintTimeRemark(talk, lobby, room) == TRUE) {
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_FIRST_MEETING, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_FIRST_MEETING);
        }
        break;
    case WIFI_PLAZA_TALK_FIRST_MEETING: // "Hello! Nice to meet you."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00040));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_WHATS_UP, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_REUNION: // "Hi, ...! It's good to see you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_BufferPlayerName(room, talk->me, 1);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetReunionMsg(room, talk->partner, talk->me));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_WHATS_UP, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_WHATS_UP: // "Yes? What's up?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00046));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_OPEN_MENU, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_ANYTHING_ELSE: // "Is there anything else?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00054));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_OPEN_MENU, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_OPEN_MENU: // TAP TOYS / CHAT / CANCEL
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 3, 515, 0);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaTalk_StartMenuTimer(talk);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_HANDLE_MENU);
        break;
    case WIFI_PLAZA_TALK_HANDLE_MENU: {
        int done;

        switch (WiFiPlazaRoom_ProcessList(room)) {
        case 0:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_COMPARE_TAP_TOYS);
            done = TRUE;
            break;
        case 1:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_START_CHAT);
            WiFiPlaza_SendChatNews(lobby, playerIndex, WiFiPlazaObj_GetParam(talk->partnerObj, 4));
            done = TRUE;
            break;
        case 2:
            WiFiPlazaWork_CancelTalkRequest(lobby);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_GOODBYE);
            done = TRUE;
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            done = FALSE;
            break;
        }
        if (done == TRUE) {
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
        }
        break;
    }
    case WIFI_PLAZA_TALK_COMPARE_TAP_TOYS: { // compare Tap Toys
        myToy = WiFiPlazaPlayer_GetTapToy(me);
        if (myToy == WiFiPlazaPlayer_GetTapToy(partner)) {
            talk->sameToy = TRUE;
            WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
            WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, FALSE);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00507));
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_WAIT_TAP_TOY_DONE, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
            WiFiPlazaWork_AdvanceTalk(lobby, 4);
        } else {
            talk->sameToy = FALSE;
            WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
            WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
            WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 2, FALSE);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00508));
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_OPEN_TAP_TOY_MENU, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        }
        break;
    }
    case WIFI_PLAZA_TALK_OPEN_TAP_TOY_MENU: // GET / CANCEL
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 2, 518, 1);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaTalk_StartMenuTimer(talk);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_HANDLE_TAP_TOY_MENU);
        break;
    case WIFI_PLAZA_TALK_HANDLE_TAP_TOY_MENU: {
        int done;

        switch (WiFiPlazaRoom_ProcessList(room)) {
        case 0: {
            int partnerIndex2;

            WiFiPlazaWork_AdvanceTalk(lobby, 3);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_RECEIVE_TAP_TOY);
            partnerIndex2 = WiFiPlazaObj_GetParam(talk->partnerObj, 4);
            WiFiPlaza_SendTapToyTradeNews(lobby, playerIndex, partnerIndex2, WiFiPlazaPlayer_GetTapToy(partner));
            done = TRUE;
            break;
        }
        case 1:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_ANYTHING_ELSE);
            done = TRUE;
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            done = FALSE;
            break;
        }
        if (done == TRUE) {
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
        }
        break;
    }
    case WIFI_PLAZA_TALK_RECEIVE_TAP_TOY: // "Received a ... from ...!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, FALSE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00510));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_TAP_TOY_SOUND, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        WiFiPlazaWork_SetTapToy(lobby, WiFiPlazaPlayer_GetTapToy(partner));
        break;
    case WIFI_PLAZA_TALK_TAP_TOY_SOUND:
        PlaySE(SEQ_SE_PL_EFF03_4);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_WAIT_TAP_TOY_DONE);
        break;
    case WIFI_PLAZA_TALK_WAIT_TAP_TOY_DONE:
        switch (partnerStatus) {
        case 2:
            WiFiPlazaTalk_SetHistory(talk, partner);
            if (talk->sameToy == TRUE) {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_GOODBYE);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_ANYTHING_ELSE);
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
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_INTERRUPTED);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case WIFI_PLAZA_TALK_START_CHAT: // chat: ask a question
        WiFiPlazaWork_AdvanceTalk(lobby, 5);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_OPEN_QUESTION_MENU);
        break;
    case WIFI_PLAZA_TALK_OPEN_QUESTION_MENU: // four questions picked at random out of 30
        WiFiPlazaTalkMenu_InitRandom(&talk->menu, room, 30, 4, 519, 0);
        WiFiPlazaRoom_OpenListAt(room, &talk->menu.template, 0, 0, 8, 3, 23);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_HANDLE_QUESTION_MENU);
        WiFiPlazaTalk_StartMenuTimer(talk);
        break;
    case WIFI_PLAZA_TALK_HANDLE_QUESTION_MENU: {
        response = WiFiPlazaRoom_ProcessList(room);
        switch (response) {
        case LIST_CANCEL:
        case LIST_NOTHING_CHOSEN:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            break;
        default:
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            talk->question = response;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_WHAT_ABOUT_YOU);
            break;
        }
        break;
    }
    case WIFI_PLAZA_TALK_WHAT_ABOUT_YOU: // "What about you?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 477 + talk->question));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_OPEN_ANSWER_MENU, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_OPEN_ANSWER_MENU: // our own answer
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 4, (talk->question << 2) + 115, 0);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_HANDLE_ANSWER_MENU);
        break;
    case WIFI_PLAZA_TALK_HANDLE_ANSWER_MENU: {
        int question;

        option = WiFiPlazaRoom_ProcessList(room);
        switch (option) {
        case 0:
        case 1:
        case 2:
        case 3:
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
            question = talk->question << 2;
            talk->myAnswer = option + question;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_SAY_ANSWER);
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            break;
        }
        break;
    }
    case WIFI_PLAZA_TALK_SAY_ANSWER: // "Oh, your favorite color's ..."; our answer goes out once the partner asks for it
        WiFiPlazaTalkReply_Start(&talk->reply, lobby, (u16)(talk->myAnswer + 6), 2);
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, 357 + talk->myAnswer));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_IF_IT_WERE_ME, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_IF_IT_WERE_ME: // "Well, if it were me..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 85));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_WAIT_ANSWER_SENT, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_WAIT_ANSWER_SENT:
        if (WiFiPlazaTalkReply_IsDone(&talk->reply) == TRUE) {
            if (WiFiPlazaTalkReply_GetStatus(&talk->reply) == 2) {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_WAIT_PARTNER_ANSWER);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_INTERRUPTED);
                WiFiPlazaRoom_HideWaitingIcon(room);
            }
        } else if (!WiFiPlazaRoom_IsWaitingIconShown(room)) {
            WiFiPlazaRoom_ShowWaitingIcon(room);
        }
        break;
    case WIFI_PLAZA_TALK_WAIT_PARTNER_ANSWER:
        switch (partnerStatus) {
        case 2:
            WiFiPlazaTalk_SetHistory(talk, partner);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_PARTNER_ANSWER);
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
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_INTERRUPTED);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case WIFI_PLAZA_TALK_PARTNER_ANSWER: // the partner's answer
        answer = (u16)(partnerAnswer - 6);
        if (partnerAnswer >= 126 || partnerAnswer < 6) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_TALK_HARD_TO_SAY);
            break;
        }
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, answer + 236));
        if (answer == talk->myAnswer) {
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_SAME_ANSWER, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        } else {
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_OTHER_ANSWER, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        }
        break;
    case WIFI_PLAZA_TALK_SAME_ANSWER: // "That's the same as me!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00051));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_ANYTHING_ELSE, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_OTHER_ANSWER: // "I see..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00050));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_ANYTHING_ELSE, state, WIFI_PLAZA_TALK_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_TALK_INTERRUPTED: { // "Sorry! Something's come up! I can't talk now!", shown for the time it takes to read
        u32 wait;

        WiFiPlazaRoom_CloseList(room, NULL, NULL);
        WiFiPlazaTalkMenu_Free(&talk->menu, room);
        text = WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00045);
        WiFiPlazaRoom_PrintMessage(room, text);
        talk->talking = FALSE;
        length = String_GetLength(text);
        wait = ((u32)(WiFiPlazaRoom_GetTextSpeed(room) * length) >> 1) + 60;
        if (wait < 128) {
            wait = 128;
        } else if (wait > 255) {
            wait = 255;
        }
        WiFiPlazaTalk_SetNextStep(talk, (u8)wait, WIFI_PLAZA_TALK_END, state, WIFI_PLAZA_TALK_WAIT_TIMER);
        WiFiPlazaWork_EndTalk(lobby);
        break;
    }
    case WIFI_PLAZA_TALK_GOODBYE: // "See you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00043));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_END, state, WIFI_PLAZA_TALK_WAIT_TIMER);
        break;
    case WIFI_PLAZA_TALK_BUSY: // "...This Trainer seems to be busy."
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00042));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_END, state, WIFI_PLAZA_TALK_WAIT_TIMER);
        break;
    case WIFI_PLAZA_TALK_HARD_TO_SAY: // "Hmm... That's hard to say..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00356));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_TALK_END, state, WIFI_PLAZA_TALK_WAIT_TIMER);
        break;
    case WIFI_PLAZA_TALK_WAIT_TIMER:
        WiFiPlazaTalk_WaitTimer(talk, state);
        break;
    case WIFI_PLAZA_TALK_WAIT_MESSAGE:
        WiFiPlazaTalk_WaitMessage(talk, state, room);
        break;
    case WIFI_PLAZA_TALK_END:
        if (talk->historyPending == TRUE) {
            WiFiPlazaWork_AddHistory(lobby, talk->historyAvatar, talk->historyId);
        }
        WiFiPlazaWork_ResetTalk(lobby);
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaWork_SetActivity(lobby, 1);
        {
            WiFiPlazaObjManager *objManager2 = WiFiPlazaRoom_GetObjManager(room);
            WiFiPlazaObj_SetHandler(objManager2, WiFiPlazaObj_GetOwnPlayer(objManager2), TRUE);
        }
        if (talk->partnerObj != NULL) {
            WiFiPlazaObj_SetAvatarMode(talk->partnerObj, 1);
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

BOOL WiFiPlazaTalk_Answer(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaTalk *talk = WiFiPlazaPlayerState_GetWork(state);
    u32 partnerStatus = WiFiPlazaWork_GetTalkState(lobby);
    u32 partnerAnswer = WiFiPlazaWork_GetTalkPartnerMessage(lobby);
    WiFiPlazaPlayer *me;
    WiFiPlazaPlayer *partner;

    if (WiFiPlazaWork_IsTalkOver(lobby) && talk != NULL && talk->talking == TRUE) {
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
        WiFiPlazaRoom_HideWaitingIcon(room);
        talk->talking = FALSE;
    }
    if (talk != NULL) {
        me = WiFiPlazaWork_GetMyPlayer(lobby);
        partner = WiFiPlazaWork_GetPlayer(lobby, talk->partner);
    }
    // the partner left the plaza
    if (WiFiPlazaPlayerState_GetStep(state) != WIFI_PLAZA_ANSWER_START && talk->talking == TRUE && partner == NULL) {
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
        WiFiPlazaRoom_HideWaitingIcon(room);
        talk->talking = FALSE;
    }

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_ANSWER_START: {
        u32 partnerIndex;
        int result;

        talk = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaTalk));
        talk->historyPending = 0;
        talk->historyAvatar = 0;
        talk->historyId = 0;
        partnerIndex = WiFiPlazaWork_GetTalkPartner(lobby);
        result = WiFiPlazaTalk_Init(talk, lobby, partnerIndex);
        PlaySE(SEQ_SE_DP_BUTTON9);
        WiFiPlazaWork_SetActivity(lobby, 9);
        if (result == TRUE) {
            WiFiPlazaRoom_ShowPlayerCard(room, partnerIndex, 0);
            WiFiPlazaWork_SetTalkedWith(lobby, partnerIndex);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_PARTNER_GREETING);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
        }
        break;
    }
    case WIFI_PLAZA_ANSWER_PARTNER_GREETING: // greetings
        if (!WiFiPlazaWork_HasTalkPartnerArrived(lobby)) {
            break;
        }
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->partnerTimeOfDay, partner, me);
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_GREET, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_GREET:
        IncrementGameStat47(lobby);
        WiFiPlazaWork_MarkActivityDone(lobby, 7);
        WiFiPlazaTalk_PrintGreeting(talk, lobby, room, talk->myTimeOfDay, me, partner);
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_WAIT_GREETINGS, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_WAIT_GREETINGS:
        switch (partnerStatus) {
        case 3:
            WiFiPlazaRoom_HideWaitingIcon(room);
            WiFiPlazaWork_AdvanceTalk(lobby, 0);
            if (WiFiPlazaWork_IsFirstTalk(lobby)) {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_TIME_REMARK);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_REUNION);
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
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case WIFI_PLAZA_ANSWER_TIME_REMARK:
        if (WiFiPlazaTalk_PrintTimeRemark(talk, lobby, room) == TRUE) {
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_FIRST_MEETING, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_FIRST_MEETING);
        }
        break;
    case WIFI_PLAZA_ANSWER_FIRST_MEETING: // "Hello! Nice to meet you."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00040));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_HAVE_A_MOMENT, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_REUNION: // "Hi, ...! It's good to see you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_BufferPlayerName(room, talk->me, 1);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetReunionMsg(room, talk->partner, talk->me));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_HAVE_A_MOMENT, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_HAVE_A_MOMENT: // "Hi, do you have a moment?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00047));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_WAIT_PARTNER_CHOICE, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_MOVING_ON: // "Moving on..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00053));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_WAIT_PARTNER_CHOICE, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_WAIT_PARTNER_CHOICE: // what the partner chose
        switch (partnerStatus) {
        case 3:
            switch (partnerAnswer) {
            case 3: // gets our Tap Toy
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_NICE_TAP_TOY);
                WiFiPlazaWork_IncPartnerTalkCount(lobby);
                talk->answered = TRUE;
                WiFiPlazaTalk_SetHistory(talk, partner);
                break;
            case 4: // has the same Tap Toy
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_SAME_TAP_TOY);
                WiFiPlazaWork_IncPartnerTalkCount(lobby);
                talk->answered = TRUE;
                WiFiPlazaTalk_SetHistory(talk, partner);
                break;
            case 5: // asks a question
                WiFiPlazaWork_AdvanceTalk(lobby, 1);
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_LETS_CHAT);
                break;
            default:
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
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
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_GOODBYE);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        case 1:
        case 4:
        default:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case WIFI_PLAZA_ANSWER_NICE_TAP_TOY: // "You have a ...? That must be nice!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00511));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_CAN_YOU_SHARE, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_CAN_YOU_SHARE: // "...Can you share your ... with me?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00512));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_SHARED_TAP_TOY, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_SHARED_TAP_TOY: // "Shared the ... with ..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00513));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_THANK_YOU, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_THANK_YOU: // "Thank you! I'll take good care of it!"
        PlaySE(SEQ_SE_PL_EFF03_4);
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00514));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_MOVING_ON, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        WiFiPlazaWork_AdvanceTalk(lobby, 2);
        WiFiPlazaRoom_SetPlayerCardIcon(room, WiFiPlazaPlayer_GetTapToy(WiFiPlazaWork_GetMyPlayer(lobby)));
        break;
    case WIFI_PLAZA_ANSWER_SAME_TAP_TOY: // "Oh! You've got a ... just like me."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaTalk_BufferInstrument(talk, lobby, room, 1, TRUE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00507));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_GOODBYE, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        WiFiPlazaWork_AdvanceTalk(lobby, 2);
        break;
    case WIFI_PLAZA_ANSWER_LETS_CHAT: // "Let's chat!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00687));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_WAIT_QUESTION, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_WAIT_QUESTION:
        switch (partnerStatus) {
        case 3:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_PARTNER_QUESTION);
            WiFiPlazaRoom_HideWaitingIcon(room);
            WiFiPlazaWork_IncPartnerTalkCount(lobby);
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
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
            WiFiPlazaRoom_HideWaitingIcon(room);
            break;
        }
        break;
    case WIFI_PLAZA_ANSWER_PARTNER_QUESTION: // the partner's question and answer
        if (partnerAnswer < 6 || partnerAnswer >= 126) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_INTERRUPTED);
            break;
        }
        talk->partnerAnswer = partnerAnswer - 6;
        talk->question = (u32)talk->partnerAnswer >> 2;
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 55));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_IF_IT_WERE_ME, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_IF_IT_WERE_ME: // "Well, if it were me..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 85));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_PARTNER_ANSWER, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_PARTNER_ANSWER:
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->partnerAnswer + 236));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_WHAT_ABOUT_YOU, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_WHAT_ABOUT_YOU: // "What about you?"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->question + 477));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_OPEN_ANSWER_MENU, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_OPEN_ANSWER_MENU: // four answers + "Stop chatting"
        WiFiPlazaTalkMenu_Init(&talk->menu, room, 5, talk->question * 4 + 115, 2);
        WiFiPlazaRoom_OpenList(room, &talk->menu.template, 0, 0);
        WiFiPlazaTalk_StartMenuTimer(talk);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_HANDLE_ANSWER_MENU);
        break;
    case WIFI_PLAZA_ANSWER_HANDLE_ANSWER_MENU: {
        u32 choice = WiFiPlazaRoom_ProcessList(room);
        int done;

        switch (choice) {
        case 0:
        case 1:
        case 2:
        case 3:
            talk->myAnswer = choice + talk->question * 4;
            WiFiPlazaWork_AdvanceTalk(lobby, (u16)(talk->myAnswer + 6));
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_SAY_ANSWER);
            done = TRUE;
            break;
        case 4:
            talk->myAnswer = 126;
            WiFiPlazaWork_AdvanceTalk(lobby, talk->myAnswer);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ANSWER_NO_NEED_TO_ANSWER);
            done = TRUE;
            break;
        default:
            WiFiPlazaTalk_UpdateMenuWait(talk, room);
            done = FALSE;
            break;
        }
        if (done) {
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            WiFiPlazaTalkMenu_Free(&talk->menu, room);
            WiFiPlazaTalk_StopMenuTimer(talk);
            WiFiPlazaTalk_SetHistory(talk, partner);
        }
        break;
    }
    case WIFI_PLAZA_ANSWER_SAY_ANSWER: // "Oh, your favorite color's ..."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, talk->myAnswer + 357));
        if (talk->partnerAnswer == talk->myAnswer) {
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_SAME_ANSWER, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        } else {
            WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_THANKS_FOR_ANSWERING, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        }
        break;
    case WIFI_PLAZA_ANSWER_SAME_ANSWER: // "That's the same as me!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00051));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_THANKS_FOR_ANSWERING, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_THANKS_FOR_ANSWERING: // "Thanks for answering!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00052));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_MOVING_ON, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_INTERRUPTED: { // "Sorry! Something's come up!", shown for the time it takes to read
        String *string;
        u32 length;
        u32 duration;

        WiFiPlazaRoom_CloseList(room, NULL, NULL);
        WiFiPlazaTalkMenu_Free(&talk->menu, room);
        string = WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00045);
        WiFiPlazaRoom_PrintMessage(room, string);
        talk->talking = FALSE;
        length = String_GetLength(string);
        duration = (WiFiPlazaRoom_GetTextSpeed(room) * length >> 1) + 60;
        if (duration < 128) {
            duration = 128;
        } else if (duration > 255) {
            duration = 255;
        }
        WiFiPlazaTalk_SetNextStep(talk, (u8)duration, WIFI_PLAZA_ANSWER_END, state, WIFI_PLAZA_ANSWER_WAIT_TIMER);
        WiFiPlazaWork_EndTalk(lobby);
        break;
    }
    case WIFI_PLAZA_ANSWER_GOODBYE: // "See you again!"
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00043));
        talk->talking = FALSE;
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_END, state, WIFI_PLAZA_ANSWER_WAIT_TIMER);
        break;
    case WIFI_PLAZA_ANSWER_NO_NEED_TO_ANSWER: // "Don't worry. You don't have to answer if you don't want to."
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00049));
        WiFiPlazaTalk_SetNextStep(talk, 128, WIFI_PLAZA_ANSWER_GOODBYE, state, WIFI_PLAZA_ANSWER_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ANSWER_WAIT_TIMER:
        WiFiPlazaTalk_WaitTimer(talk, state);
        break;
    case WIFI_PLAZA_ANSWER_WAIT_MESSAGE:
        WiFiPlazaTalk_WaitMessage(talk, state, room);
        break;
    case WIFI_PLAZA_ANSWER_END:
        if (talk->historyPending == TRUE) {
            WiFiPlazaWork_AddHistory(lobby, talk->historyAvatar, talk->historyId);
        }
        if (!talk->answered) {
            WiFiPlazaWork_IncTalkCount(lobby, talk->partner);
        }
        WiFiPlazaWork_ResetTalk(lobby);
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaTalk_Free(talk);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaWork_SetActivity(lobby, 1);
        {
            WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
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

BOOL WiFiPlazaTalk_InfoCounter(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room) {
    WiFiPlazaInfoCounter *counter = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    u32 selected;
    u32 choice;

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaInfoCounter));
        PlaySE(SEQ_SE_DP_SELECT);
        if (WiFiPlazaWork_IsClosing(lobby) == 1) {
            WiFiPlazaPlayerState_SetStep(state, 2);
        } else if (WiFiPlazaWork_IsParadeStarted(lobby) == TRUE) {
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
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
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
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
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
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
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
        WiFiPlazaObjManager *objManager;

        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaRoom_CloseMessage(room);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        WiFiPlazaObj_SetHandler(objManager, WiFiPlazaObj_GetOwnPlayer(objManager), TRUE);
        return TRUE;
    }
    }
    return FALSE;
}

BOOL WiFiPlazaTalk_Reception(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room) {
    WiFiPlazaWork *lobby;
    String *message;

    WiFiPlazaPlayerState_GetWork(state);
    lobby = WiFiPlazaRoom_GetLobby(room);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case 0:
        PlaySE(SEQ_SE_DP_SELECT);
        if (WiFiPlazaWork_IsClosing(lobby) == 0) {
            if (WiFiPlazaWork_IsParadeStarted(lobby) == TRUE) { // the Pokemon Parade is on
                message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00078);
                WiFiPlazaRoom_PrintMessage(room, message);
                WiFiPlazaPlayerState_SetStep(state, 1);
                goto done;
            }
        } else { // the visit is nearly over
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00003);
            WiFiPlazaRoom_PrintMessage(room, message);
            WiFiPlazaPlayerState_SetStep(state, 1);
            goto done;
        }
        // what the receptionist recommends
        switch (WiFiPlazaWork_GetNextUndoneActivity(lobby)) {
        case 0:
            WiFiPlazaRoom_BufferActivityName(room, 0, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00073);
            break;
        case 1:
            WiFiPlazaRoom_BufferActivityName(room, 1, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00073);
            break;
        case 2:
            WiFiPlazaRoom_BufferActivityName(room, 2, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00073);
            break;
        case 3:
            WiFiPlazaRoom_BufferActivityName(room, 5, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00077);
            break;
        case 4:
            WiFiPlazaRoom_BufferActivityName(room, 6, 0);
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00077);
            break;
        case 5: // Footprint Stamp
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00074);
            break;
        case 6: // Plaza Survey
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00079);
            break;
        case 7: // chatting with friends
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00075);
            break;
        case 8:
        default:
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00076);
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
        WiFiPlazaObjManager *objManager;
        WiFiPlazaObj *ownObj;

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

    localTime.time = WiFiPlazaPlayer_GetLocalTime(player);
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

    if (WiFiPlazaWork_IsSpecialVisitor(WiFiPlazaRoom_GetLobby(room), playerIndex) == 1) {
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
    int speakerHighlighted = WiFiPlazaWork_IsSpecialVisitor(lobby, speaker);
    int listenerHighlighted = WiFiPlazaWork_IsSpecialVisitor(lobby, listener);
    u32 msgId = msg_0766_00041;

    if (speakerHighlighted == 1 && listenerHighlighted == 1) {
        msgId = msg_0766_00682;
    } else if (speakerHighlighted == 1 && listenerHighlighted == 0) {
        msgId = msg_0766_00550;
    } else if (speakerHighlighted == 0 && listenerHighlighted == 1) {
        msgId = msg_0766_00681;
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
    talk->me = WiFiPlazaWork_GetOwnSlot(lobby);
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

void WiFiPlazaTalk_SetNextStep(WiFiPlazaTalk *talk, u8 frames, u8 nextStep, WiFiPlazaPlayerState *state, u8 waitStep) {
    talk->timer = frames;
    talk->nextStep = nextStep;
    WiFiPlazaPlayerState_SetStep(state, waitStep);
}

void WiFiPlazaTalk_WaitTimer(WiFiPlazaTalk *talk, WiFiPlazaPlayerState *state) {
    if (talk->timer != 0) {
        talk->timer--;
    } else {
        WiFiPlazaPlayerState_SetStep(state, talk->nextStep);
    }
}

void WiFiPlazaTalk_WaitMessage(WiFiPlazaTalk *talk, WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room) {
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
            msgId = msg_0766_00039;
            break;
        case 1:
            msgId = msg_0766_00037;
            break;
        case 2:
            msgId = msg_0766_00038;
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
        instrument = WiFiPlazaPlayer_GetTapToy(partner);
    } else {
        instrument = WiFiPlazaPlayer_GetTapToy(me);
    }
    WiFiPlazaRoom_BufferInstrumentName(room, instrument, slot);
}

void WiFiPlazaTalk_UpdatePartnerWait(WiFiPlazaTalk *talk, WiFiPlazaRoomWork *room) {
    if (WiFiPlazaWork_GetTalkTimeoutCounter(WiFiPlazaRoom_GetLobby(room)) == 450) { // "Umm... Hold on a second..."
        WiFiPlazaRoom_HideWaitingIcon(room);
        WiFiPlazaRoom_BufferPlayerName(room, talk->partner, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00684));
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
        message = WiFiPlazaTalk_GetMsg(room, talk->partner, msg_0766_00683);
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
    u32 avatar = WiFiPlazaPlayer_GetAvatarIndex(partner);

    talk->historyPending = TRUE;
    talk->historyAvatar = avatar;
    talk->historyId = WiFiPlazaPlayer_GetLobbyId(partner);
}

void WiFiPlazaTalkMenu_Init(WiFiPlazaTalkMenu *menu, WiFiPlazaRoomWork *room, u32 count, u32 firstMsg, u32 lastItem) {
    u32 i;

    WiFiPlazaRoom_CreateListItems(room, count);
    menu->count = count;
    for (i = 0; i < count; i++) {
        String *text;

        if (lastItem != 0 && i == count - 1) {
            if (lastItem == 1) {
                text = WiFiPlazaRoom_GetMsgString(room, 3, msg_0766_00517); // CANCEL
            } else {
                text = WiFiPlazaRoom_GetMsgString(room, 3, msg_0766_00235); // Stop chatting
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
                text = WiFiPlazaRoom_GetMsgString(room, 3, msg_0766_00517);
            } else {
                text = WiFiPlazaRoom_GetMsgString(room, 3, msg_0766_00235);
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
        WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00039), i);
    }
    WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00037), 3); // EXIT
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
        WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00038), i);
    }
    WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00037), 3); // EXIT
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
    reply->startStatus = WiFiPlazaWork_GetTalkState(lobby);
}

void WiFiPlazaTalkReply_Update(WiFiPlazaTalkReply *reply, WiFiPlazaWork *lobby) {
    if (reply->active != FALSE) {
        reply->status = WiFiPlazaWork_GetTalkState(lobby);
        if (reply->status == reply->awaited) {
            WiFiPlazaWork_AdvanceTalk(lobby, reply->value);
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
