#include "record_mixing/record_mixing_link_internal.h"

static void RecordMixingLink_RecvRecords(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvLeaderMenuState(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvStartMixing(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvClearMessage(int sender, int size, void *data, void *context);
static void RecordMixingLink_HandleJoinRequest(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvMemberJoined(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvUnused108(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvUnused109(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvCloseYesNo(int sender, int size, void *data, void *context);
static void RecordMixingLink_RecvJoinNotice(int sender, int size, void *data, void *context);
static void RecordMixingLink_SendRecords(RecordMixingApp *room, int unused);
static u32 RecordMixingLink_GetSize0(void);
static u32 RecordMixingLink_GetSize1(void);
static u32 RecordMixingLink_GetJoinRequestSize(void);
static void *RecordMixingLink_GetRecvBuffer(int sender, void *context, int size);

static const CommCommand sRecordMixingLinkCommands[] = {
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { RecordMixingLink_RecvUnused108,       RecordMixingLink_GetSize1,           NULL                           },
    { RecordMixingLink_RecvUnused109,       RecordMixingLink_GetSize1,           NULL                           },
    { RecordMixingLink_RecvMemberJoined,    RecordMixingLink_GetSize1,           NULL                           },
    { RecordMixingLink_RecvClearMessage,    RecordMixingLink_GetSize0,           NULL                           },
    { RecordMixingLink_HandleJoinRequest,   RecordMixingLink_GetJoinRequestSize, NULL                           },
    { RecordMixingLink_RecvCloseYesNo,      RecordMixingLink_GetSize0,           NULL                           },
    { RecordMixingLink_RecvJoinNotice,      RecordMixingLink_GetSize0,           NULL                           },
    { RecordMixingLink_RecvStartMixing,     RecordMixingLink_GetSize0,           NULL                           },
    { RecordMixingLink_RecvRecords,         CommCmd_GetSize3008,                 RecordMixingLink_GetRecvBuffer },
    { RecordMixingLink_RecvLeaderMenuState, RecordMixingLink_GetSize1,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
    { CommCmd_RecvIgnored,                  RecordMixingLink_GetSize0,           NULL                           },
};

void RecordMixingLink_RegisterCommands(RecordMixingApp *room) {
    Comm_RegisterCommandTable(sRecordMixingLinkCommands, NELEMS(sRecordMixingLinkCommands), room);
}

static void RecordMixingLink_RecvRecords(int sender, int size, void *data, void *context) {
    RecordMixingApp *room = context;

    room->recordsReceived++;
}

static void RecordMixingLink_RecvLeaderMenuState(int sender, int size, void *data, void *context) {
    RecordMixingApp *room = context;
    u8 *state = data;

    if (sender == 0) {
        room->leaderJoinPending = *state;
    }
}

static void RecordMixingLink_RecvStartMixing(int sender, int size, void *data, void *context) {
    RecordMixingApp *room = context;

    room->recordsReceived = 0;
    room->isMixing = 1;
    RecordMixingLink_SendRecords(room, Comm_GetLocalNetId());
    RecordMixing_StartFlash(room);
    RecordMixing_SetStateFromLink(room, 25, (u8)sender);
    CommState_SetErrorWatch(1, 1);
    PlaySE(0x657);
}

static void RecordMixingLink_RecvClearMessage(int sender, int size, void *data, void *context) {
    RecordMixingApp *room = context;

    RecordMixing_SetStateFromLink(room, 2, 0);
}

static void RecordMixingLink_HandleJoinRequest(int sender, int size, void *data, void *context) {
    RecordMixingApp *room = context;
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

static void RecordMixingLink_RecvMemberJoined(int sender, int size, void *data, void *context) {
    RecordMixingApp *room = context;
    u8 *member = data;

    RecordMixing_SetStateFromLinkInMenu(room, 1, *member);
    if (Comm_GetLocalNetId() == 0) {
        room->unk_0384 = 0;
    }
}

static void RecordMixingLink_RecvUnused108(int sender, int size, void *data, void *context) {
}

static void RecordMixingLink_RecvUnused109(int sender, int size, void *data, void *context) {
}

static void RecordMixingLink_RecvCloseYesNo(int sender, int size, void *data, void *context) {
    RecordMixingApp *room = context;

    if (Comm_GetLocalNetId() != 0) {
        RecordMixing_SetStateFromLink(room, 13, 0);
    }
}

static void RecordMixingLink_RecvJoinNotice(int sender, int size, void *data, void *context) {
    if (Comm_GetLocalNetId() == 0) {
        u8 id = sender;
        Comm_SendCommand(110, &id, 1);
    }
}

static void RecordMixingLink_SendRecords(RecordMixingApp *room, int unused) {
    int i;
    u32 checksum = 0;
    u32 *p = room->sendBuffer.records;
    for (i = 0; i < 0x2ee; i++) {
        checksum ^= *p;
        p++;
    }
    room->sendBuffer.checksum = checksum;
    room->sendBuffer.random = LCRandom();
    Comm_SendCommandNoCopy(116, &room->sendBuffer, 0xbc0);
}

static u32 RecordMixingLink_GetSize0(void) {
    return 0;
}

static u32 RecordMixingLink_GetSize1(void) {
    return 1;
}

static u32 RecordMixingLink_GetJoinRequestSize(void) {
    return 4;
}

static void *RecordMixingLink_GetRecvBuffer(int sender, void *context, int size) {
    RecordMixingApp *room = context;

    return room->receiveBuffers[sender];
}
