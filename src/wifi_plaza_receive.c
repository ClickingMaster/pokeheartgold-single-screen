#include "global.h"

#include "wifi_plaza_internal.h"

// Receive callbacks of the exchange with one other player (WiFiPlazaWork.unk_1c0; WiFiPlazaLobbyClient_SendToPlayer sends its messages):
// id is the sender, data the received words.

#pragma force_active on

// Message 0: the other player asks for an exchange; answered with WiFiPlazaWork_SendTalkAnswer (TRUE = accepted).
void WiFiPlaza_OnTalkRequest(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    u32 slot = WiFiPlazaLobbyClient_IdToSlot(id);
    BOOL ready;
    BOOL busy;

    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->unk_52c = 1;
    }
    ready = ov45_0222A920(&work->profile.player);
    busy = WiFiPlazaWork_IsTalking(work);
    if (ready != TRUE || slot == (u32)-1 || data[1] != 1 || busy == TRUE || WiFiPlazaWork_CanTalkWith(work, slot) == FALSE) {
        WiFiPlazaWork_SendTalkAnswer(work, id, FALSE);
    } else {
        WiFiPlazaWork_SendTalkAnswer(work, id, TRUE);
        if (WiFiPlazaTalkSession_HasTalkedWith(&work->unk_1c0, slot) == FALSE) {
            work->unk_1c0.unk_0b = TRUE;
        } else {
            work->unk_1c0.unk_0b = FALSE;
        }
        work->unk_1c0.unk_04 = slot;
        work->unk_1c0.unk_06 = 2;
        work->unk_1c0.unk_0a = 2;
        work->unk_1c0.unk_07 = 0;
        WiFiPlazaTalkSession_StartTimeout(&work->unk_1c0);
    }
}

// Message 1: the answer of the player asked.
void WiFiPlaza_OnTalkAnswer(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (work->unk_1c0.unk_0a != 0 && work->unk_1c0.unk_04 == slot) {
        if (*data == 1) {
            if (WiFiPlazaTalkSession_HasTalkedWith(&work->unk_1c0, slot) == FALSE) {
                work->unk_1c0.unk_0b = TRUE;
            } else {
                work->unk_1c0.unk_0b = FALSE;
            }
            work->unk_1c0.unk_06 = 2;
        } else {
            WiFiPlazaTalkSession_Reset(&work->unk_1c0);
        }
        WiFiPlazaTalkSession_StopTimeout(&work->unk_1c0);
    }
}

#pragma force_active reset

// Message 2: the state of the other player (2 or 3), or the end of the exchange.
void WiFiPlaza_OnTalkState(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    int slot;
    if (work->unk_1c0.unk_0a != 0) {
        slot = WiFiPlazaLobbyClient_IdToSlot(id);
        if (work->unk_1c0.unk_04 == slot) {
            if ((u16)(data[1] + 0xFFFE) <= 1) { // 2 or 3
                work->unk_1c0.unk_06 = data[1];
                work->unk_1c0.unk_08 = data[0];
                WiFiPlazaTalkSession_StopTimeout(&work->unk_1c0);
                return;
            }
            WiFiPlazaTalkSession_Abort(&work->unk_1c0);
            return;
        }
        if (slot == -1) {
            WiFiPlazaTalkSession_Abort(&work->unk_1c0);
        }
    }
}

void WiFiPlaza_OnTalkCancel(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    if (work->unk_1c0.unk_06 != 0) {
        int slot = WiFiPlazaLobbyClient_IdToSlot(id);
        if (work->unk_1c0.unk_04 == slot) {
            WiFiPlazaTalkSession_Reset(&work->unk_1c0);
        }
    }
}

#pragma force_active on

// A lobby message: handled by ov45_02254A60[type] (WiFiPlaza_HandleChatNews ...).
void WiFiPlaza_OnLobbyMessage(int id, WiFiPlazaLobbyMessage *msg, u32 size, WiFiPlazaWork *work) {
    ov45_02254A60[msg->type](work, msg);
}

#pragma force_active reset
