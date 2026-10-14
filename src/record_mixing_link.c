#include "record_mixing_link_internal.h"

void RecordMixingLink_RegisterCommands(RecordMixingLinkWork *room) {
    Comm_RegisterCommandTable(gRecordMixingLinkCommands, 110, room);
}

void RecordMixingLink_RecvRecords(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    room->recordsReceived++;
}

void RecordMixingLink_RecvLeaderMenuState(int sender, int unused, u8 *data, RecordMixingLinkWork *room) {
    if (sender == 0) {
        room->leaderJoinPending = *data;
    }
}

void RecordMixingLink_RecvStartMixing(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    room->recordsReceived = 0;
    room->isMixing = 1;
    RecordMixingLink_SendRecords(room, Comm_GetLocalNetId());
    RecordMixing_StartFlash(room);
    RecordMixing_SetStateFromLink(room, 25, (u8)sender);
    CommState_SetErrorWatch(1, 1);
    PlaySE(0x657);
}

void RecordMixingLink_RecvClearMessage(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    RecordMixing_SetStateFromLink(room, 2, 0);
}

void RecordMixingLink_HandleJoinRequest(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    RecordMixingJoinRequest *src = data;
    if (sender != 0) {
        RecordMixingJoinRequest packet;
        if (Comm_GetLocalNetId() != 0) {
            return;
        }
        packet = *src;
        packet.sender = sender;
        packet.count = room->participantCount;
        switch (src->command) {
        case 0:
            if (room->participantCount != Comm_CountConnected() || room->participantCount != RecordMixing_GetMemberCount() || room->participantCount != MATH_CountPopulation(Wireless_GetConnectedBitmap())) {
                packet.accepted = 0;
            } else {
                room->leavingMask |= 1 << sender;
                packet.accepted = 1;
                CommState_SetMaxEntries(Comm_CountConnected());
            }
            break;
        case 1:
            break;
        }
        Comm_SendCommand(112, &packet, 4);
        return;
    }
    switch (src->command) {
    case 0: {
        int id = src->sender;
        if (id != Comm_GetLocalNetId()) {
            return;
        }
        if (src->accepted == 0) {
            RecordMixing_SetStateFromLink(room, 8, id);
            return;
        }
        room->assignedCount = src->count;
        RecordMixing_SetStateFromLink(room, 7, src->sender);
        return;
    }
    case 1:
        RecordMixing_SetStateFromLink(room, 19, src->sender);
        break;
    }
}

void RecordMixingLink_RecvMemberJoined(int sender, int unused, u8 *data, RecordMixingLinkWork *room) {
    RecordMixing_SetStateFromLinkInMenu(room, 1, *data);
    if (Comm_GetLocalNetId() == 0) {
        room->flag = 0;
    }
}

void RecordMixingLink_RecvUnused108(int sender, int unused, void *data, RecordMixingLinkWork *room) {
}

void RecordMixingLink_RecvUnused109(int sender, int unused, void *data, RecordMixingLinkWork *room) {
}

void RecordMixingLink_RecvCloseYesNo(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    if (Comm_GetLocalNetId() != 0) {
        RecordMixing_SetStateFromLink(room, 13, 0);
    }
}

void RecordMixingLink_RecvJoinNotice(int sender) {
    if (Comm_GetLocalNetId() == 0) {
        u8 id = sender;
        Comm_SendCommand(110, &id, 1);
    }
}

void RecordMixingLink_SendRecords(RecordMixingLinkWork *room, int unused) {
    int i;
    u32 checksum = 0;
    u32 *p = room->send;
    for (i = 0; i < 0x2ee; i++) {
        checksum ^= *p;
        p++;
    }
    room->checksum = checksum;
    room->random = LCRandom();
    Comm_SendCommandNoCopy(116, room->send, 0xbc0);
}

u32 RecordMixingLink_GetSize0(void) {
    return 0;
}

u32 RecordMixingLink_GetSize1(void) {
    return 1;
}

u32 RecordMixingLink_GetJoinRequestSize(void) {
    return 4;
}

void *RecordMixingLink_GetRecvBuffer(int sender, RecordMixingLinkWork *room) {
    return room->receive[sender];
}
