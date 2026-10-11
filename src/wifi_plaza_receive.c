#include "global.h"

#include "wifi_plaza_internal.h"

// Receive callbacks of the exchange with one other player (WiFiPlazaWork.unk_1c0; WiFiPlazaLobbyClient_SendToPlayer sends its messages):
// id is the sender, data the received words.

#pragma force_active on

// Message 0: the other player asks for an exchange; answered with ov45_0222BE08 (TRUE = accepted).
void ov45_0222C688(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    u32 slot = ov45_0222EC68(id);
    BOOL ready;
    BOOL busy;

    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->unk_52c = 1;
    }
    ready = ov45_0222A920(&work->profile.player);
    busy = ov45_0222AFF8(work);
    if (ready != TRUE || slot == (u32)-1 || data[1] != 1 || busy == TRUE || ov45_0222B048(work, slot) == FALSE) {
        ov45_0222BE08(work, id, FALSE);
    } else {
        ov45_0222BE08(work, id, TRUE);
        if (ov45_0222BD74(&work->unk_1c0, slot) == FALSE) {
            work->unk_1c0.unk_0b = TRUE;
        } else {
            work->unk_1c0.unk_0b = FALSE;
        }
        work->unk_1c0.unk_04 = slot;
        work->unk_1c0.unk_06 = 2;
        work->unk_1c0.unk_0a = 2;
        work->unk_1c0.unk_07 = 0;
        ov45_0222BE48(&work->unk_1c0);
    }
}

// Message 1: the answer of the player asked.
void ov45_0222C73C(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    int slot = ov45_0222EC68(id);
    if (work->unk_1c0.unk_0a != 0 && work->unk_1c0.unk_04 == slot) {
        if (*data == 1) {
            if (ov45_0222BD74(&work->unk_1c0, slot) == FALSE) {
                work->unk_1c0.unk_0b = TRUE;
            } else {
                work->unk_1c0.unk_0b = FALSE;
            }
            work->unk_1c0.unk_06 = 2;
        } else {
            ov45_0222BD4C(&work->unk_1c0);
        }
        ov45_0222BE54(&work->unk_1c0);
    }
}

#pragma force_active reset

// Message 2: the state of the other player (2 or 3), or the end of the exchange.
void ov45_0222C79C(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    int slot;
    if (work->unk_1c0.unk_0a != 0) {
        slot = ov45_0222EC68(id);
        if (work->unk_1c0.unk_04 == slot) {
            if ((u16)(data[1] + 0xFFFE) <= 1) { // 2 or 3
                work->unk_1c0.unk_06 = data[1];
                work->unk_1c0.unk_08 = data[0];
                ov45_0222BE54(&work->unk_1c0);
                return;
            }
            ov45_0222BD5C(&work->unk_1c0);
            return;
        }
        if (slot == -1) {
            ov45_0222BD5C(&work->unk_1c0);
        }
    }
}

void ov45_0222C800(int id, u16 *data, u32 size, WiFiPlazaWork *work) {
    if (work->unk_1c0.unk_06 != 0) {
        int slot = ov45_0222EC68(id);
        if (work->unk_1c0.unk_04 == slot) {
            ov45_0222BD4C(&work->unk_1c0);
        }
    }
}

#pragma force_active on

// A lobby message: handled by ov45_02254A60[type] (ov45_0222BE9C ...).
void ov45_0222C828(int id, WiFiPlazaLobbyMessage *msg, u32 size, WiFiPlazaWork *work) {
    ov45_02254A60[msg->type](work, msg);
}

#pragma force_active reset
