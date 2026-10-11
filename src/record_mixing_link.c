#include "record_mixing_link_internal.h"

void RecordMixingLink_RegisterCommands(RecordMixingLinkWork *room) {
    Comm_RegisterCommandTable(gRecordMixingLinkCommands, 110, room);
}

void sub_0208FB78(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    room->count++;
}

void sub_0208FB88(int sender, int unused, u8 *data, RecordMixingLinkWork *room) {
    if (sender == 0) {
        room->remote = *data;
    }
}

void sub_0208FB98(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    room->count = 0;
    room->active = 1;
    RecordMixingLink_SendRecords(room, Comm_GetLocalNetId());
    RecordMixing_StartFlash(room);
    ov73_021E705C(room, 25, (u8)sender);
    sub_020398D4(1, 1);
    PlaySE(0x657);
}

void sub_0208FBE0(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    ov73_021E705C(room, 2, 0);
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
            if (room->participantCount != Comm_CountConnected() || room->participantCount != ov73_021E7488() || room->participantCount != MATH_CountPopulation(sub_02033250())) {
                packet.accepted = 0;
            } else {
                room->participantMask |= 1 << sender;
                packet.accepted = 1;
                sub_02038C1C(Comm_CountConnected());
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
            ov73_021E705C(room, 8, id);
            return;
        }
        room->assignedCount = src->count;
        ov73_021E705C(room, 7, src->sender);
        return;
    }
    case 1:
        ov73_021E705C(room, 19, src->sender);
        break;
    }
}

void sub_0208FCDC(int sender, int unused, u8 *data, RecordMixingLinkWork *room) {
    ov73_021E7120(room, 1, *data);
    if (Comm_GetLocalNetId() == 0) {
        room->flag = 0;
    }
}

void sub_0208FCFC(int sender, int unused, void *data, RecordMixingLinkWork *room) {
}

void sub_0208FD00(int sender, int unused, void *data, RecordMixingLinkWork *room) {
}

void sub_0208FD04(int sender, int unused, void *data, RecordMixingLinkWork *room) {
    if (Comm_GetLocalNetId() != 0) {
        ov73_021E705C(room, 13, 0);
    }
}

void sub_0208FD1C(int sender) {
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
    sub_02036FD8(116, room->send, 0xbc0);
}

u32 sub_0208FD7C(void) {
    return 0;
}

u32 sub_0208FD80(void) {
    return 1;
}

u32 sub_0208FD84(void) {
    return 4;
}

void *RecordMixingLink_GetRecvBuffer(int sender, RecordMixingLinkWork *room) {
    return room->receive[sender];
}
