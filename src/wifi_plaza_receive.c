#include "global.h"

#include "wifi_plaza_internal.h"

// Receive callbacks of the exchange with one other player (WiFiPlazaWork.talk; WiFiPlazaLobbyClient_SendToPlayer sends its messages):
// id is the sender, data the received words.

#pragma force_active on

// Message 0: the other player asks for an exchange; answered with WiFiPlazaWork_SendTalkAnswer (TRUE = accepted).
void WiFiPlaza_OnTalkRequest(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    u32 slot = WiFiPlazaLobbyClient_IdToSlot(id);
    BOOL ready;
    BOOL busy;

    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->hasError = 1;
    }
    ready = ov45_0222A920(&work->profile.player);
    busy = WiFiPlazaWork_IsTalking(work);
    if (ready != TRUE || slot == (u32)-1 || data[1] != 1 || busy == TRUE || WiFiPlazaWork_CanTalkWith(work, slot) == FALSE) {
        WiFiPlazaWork_SendTalkAnswer(work, id, FALSE);
    } else {
        WiFiPlazaWork_SendTalkAnswer(work, id, TRUE);
        if (WiFiPlazaTalkSession_HasTalkedWith(&work->talk, slot) == FALSE) {
            work->talk.isFirstTalk = TRUE;
        } else {
            work->talk.isFirstTalk = FALSE;
        }
        work->talk.partnerSlot = slot;
        work->talk.state = 2;
        work->talk.role = 2;
        work->talk.unk_07 = 0;
        WiFiPlazaTalkSession_StartTimeout(&work->talk);
    }
}

// Message 1: the answer of the player asked.
void WiFiPlaza_OnTalkAnswer(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (work->talk.role != 0 && work->talk.partnerSlot == slot) {
        if (*data == 1) {
            if (WiFiPlazaTalkSession_HasTalkedWith(&work->talk, slot) == FALSE) {
                work->talk.isFirstTalk = TRUE;
            } else {
                work->talk.isFirstTalk = FALSE;
            }
            work->talk.state = 2;
        } else {
            WiFiPlazaTalkSession_Reset(&work->talk);
        }
        WiFiPlazaTalkSession_StopTimeout(&work->talk);
    }
}

#pragma force_active reset

// Message 2: the state of the other player (2 or 3), or the end of the exchange.
void WiFiPlaza_OnTalkState(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    int slot;
    if (work->talk.role != 0) {
        slot = WiFiPlazaLobbyClient_IdToSlot(id);
        if (work->talk.partnerSlot == slot) {
            if ((u16)(data[1] + 0xFFFE) <= 1) { // 2 or 3
                work->talk.state = data[1];
                work->talk.partnerMessage = data[0];
                WiFiPlazaTalkSession_StopTimeout(&work->talk);
                return;
            }
            WiFiPlazaTalkSession_Abort(&work->talk);
            return;
        }
        if (slot == -1) {
            WiFiPlazaTalkSession_Abort(&work->talk);
        }
    }
}

void WiFiPlaza_OnTalkCancel(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    if (work->talk.state != 0) {
        int slot = WiFiPlazaLobbyClient_IdToSlot(id);
        if (work->talk.partnerSlot == slot) {
            WiFiPlazaTalkSession_Reset(&work->talk);
        }
    }
}

#pragma force_active on

// A lobby message: handled by ov45_02254A60[type] (WiFiPlaza_HandleChatNews ...).
void WiFiPlaza_OnLobbyMessage(int id, WiFiPlazaLobbyMessage *msg, u32 size, WiFiPlazaWork *work) {
    ov45_02254A60[msg->type](work, msg);
}

#pragma force_active reset
