#include "spin_trade_link.h"

#include "global.h"

#include "comm_state.h"
#include "communication_command.h"
#include "communication_core.h"
#include "communication_player_state.h"
#include "field_comm_cmds.h"
#include "heap.h"
#include "union_room_talk.h"
#include "wireless_manager.h"

typedef struct SpinTradeLinkPacket {
    u32 command;
    u8 data[20];
} SpinTradeLinkPacket;

struct SpinTradeLink {
    SpinTradeArgs *parent;
    SpinTradeLinkPacket packet;
    u32 blocked, count, state, leaderJoinPending, participantCount, leavingMask, unk34;
    u16 assignedCount;
    u8 pad3a[6];
    u16 receivedStatus, memberMask, flags44;
    s16 partyReceivedMask;
    u16 cancelMask, confirmMask;
    u8 *sendBuffer, *receiveBuffer;
    u32 unk54;
};

typedef struct SpinTradeJoinRequest {
    u8 sender, count, command, accepted;
} SpinTradeJoinRequest;

u32 MATH_CountPopulation(u32 value);
void CommState_SetMaxEntries(int count);
void UnionRoomObjects_ResetSlots(struct UnionRoomObjects *objects);
void SpinTradeGroup_HandleLinkEvent(struct SpinTradeGroupApp *app, int event, int value);
void SpinTradeGroup_HandleMemberEvent(struct SpinTradeGroupApp *app, int event, int value);
void SpinTrade_ReceiveMemberSlot(struct SpinTradeApp *app, void *data);
void SpinTrade_ReceivePlayerCount(struct SpinTradeApp *app, u32 data);
void SpinTrade_ReceiveJump(struct SpinTradeApp *app, u8 data);
void SpinTrade_ReceiveSpinData(struct SpinTradeApp *app, const u16 *data);
void SpinTrade_ReceiveTradeSlot(struct SpinTradeApp *app, int sender, u32 data);
void SpinTrade_ReceiveResult(struct SpinTradeApp *app, const u16 *data);
typedef void (*SpinTradeLinkPacketHandler)(int, int, void *, SpinTradeLink *);
void *SpinTradeLink_New(SpinTradeArgs *parent, enum HeapID heap);

void SpinTradeLink_RegisterCommands(SpinTradeLink *menu);

BOOL SpinTradeLink_SendPacket(SpinTradeLink *menu, u32 command, void *data, u32 size);
BOOL SpinTradeLink_SendCommand(SpinTradeLink *menu, u32 command, void *data, u32 size);
static void SpinTradeLink_ReceivePacket(int sender, int size, void *data, void *context);
static void SpinTradeLink_ReceiveParty(int sender, int size, void *data, void *context);
static void SpinTradeLink_HandleLeaderMenuState(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleSyncCount(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleStartSync(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleClearMessage(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleJoinRequest(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleMemberJoined(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleCloseYesNo(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleJoinNotice(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleStatus(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleJoin(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleSlot(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleCount(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleJump(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleSpin(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleTradeSlot(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleResult(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleCancel(int sender, int unused, void *data, SpinTradeLink *menu);
static void SpinTradeLink_HandleConfirm(int sender, int unused, void *data, SpinTradeLink *menu);
static u32 SpinTradeLink_GetPacketSize(void);
static u32 SpinTradeLink_GetPartySize(void);
static void *SpinTradeLink_GetPartyRecvBuffer(int sender, void *context, int size);
int SpinTradeLink_CountMemberProfiles(void);
void *SpinTradeLink_GetReceivedParty(SpinTradeLink *menu, int sender);

static const SpinTradeLinkPacketHandler sSpinTradeLinkPacketHandlers[] = {
    SpinTradeLink_HandleMemberJoined,
    SpinTradeLink_HandleClearMessage,
    SpinTradeLink_HandleJoinRequest,
    SpinTradeLink_HandleCloseYesNo,
    SpinTradeLink_HandleJoinNotice,
    SpinTradeLink_HandleStartSync,
    SpinTradeLink_HandleSyncCount,
    SpinTradeLink_HandleLeaderMenuState,
    SpinTradeLink_HandleStatus,
    SpinTradeLink_HandleJoin,
    SpinTradeLink_HandleJump,
    SpinTradeLink_HandleSpin,
    SpinTradeLink_HandleSlot,
    SpinTradeLink_HandleCount,
    SpinTradeLink_HandleTradeSlot,
    SpinTradeLink_HandleResult,
    SpinTradeLink_HandleCancel,
    SpinTradeLink_HandleConfirm,
};

static const CommCommand sSpinTradeLinkCommands[] = {
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { CommCmd_RecvIgnored,         CommCmd_GetSize0,            NULL                             },
    { SpinTradeLink_ReceivePacket, SpinTradeLink_GetPacketSize, NULL                             },
    { SpinTradeLink_ReceiveParty,  SpinTradeLink_GetPartySize,  SpinTradeLink_GetPartyRecvBuffer },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
    { NULL,                        NULL,                        NULL                             },
};

void *SpinTradeLink_New(SpinTradeArgs *parent, enum HeapID heap) {
    SpinTradeLink *menu = Heap_Alloc(heap, sizeof(SpinTradeLink));
    GF_ASSERT(menu);
    memset(menu, 0, sizeof(SpinTradeLink));
    menu->parent = parent;
    menu->sendBuffer = Heap_Alloc(heap, 0x1bd0);
    menu->receiveBuffer = Heap_Alloc(heap, 0x1bd0);
    return menu;
}

void SpinTradeLink_Free(SpinTradeLink *menu) {
    Heap_Free(menu->sendBuffer);
    Heap_Free(menu->receiveBuffer);
    Heap_Free(menu);
}

void SpinTradeLink_RegisterCommands(SpinTradeLink *menu) {
    Comm_RegisterCommandTable(sSpinTradeLinkCommands, NELEMS(sSpinTradeLinkCommands), menu);
}

void SpinTradeLink_Close(SpinTradeLink *menu) {
    CommState_SetMaxEntries(2);
    CommState_ResumeAlternateSearch();
    UnionRoomObjects_ResetSlots(menu->parent->input.communication);
    UnionRoom_SetBeaconActivity(UNION_ACTIVITY_NONE);
}

BOOL SpinTradeLink_SendPacket(SpinTradeLink *menu, u32 command, void *data, u32 size) {
    SpinTradeLinkPacket *packet;
    GF_ASSERT(command < 18);
    GF_ASSERT(size + 4 <= 24);
    packet = &menu->packet;
    packet->command = command;
    memcpy(packet->data, data, size);
    return Comm_SendCommand(0x82, packet, 24);
}

BOOL SpinTradeLink_SendCommand(SpinTradeLink *menu, u32 command, void *data, u32 size) {
    if (menu->blocked == 1) {
        return FALSE;
    }
    return SpinTradeLink_SendPacket(menu, command, data, size);
}

static void SpinTradeLink_ReceivePacket(int sender, int size, void *data, void *context) {
    SpinTradeLinkPacket *packet = data;
    SpinTradeLink *menu = context;

    if (packet->command >= 18) {
        GF_ASSERT(FALSE);
        return;
    }
    sSpinTradeLinkPacketHandlers[packet->command](sender, size, packet->data, menu);
}

static void SpinTradeLink_ReceiveParty(int sender, int size, void *data, void *context) {
    SpinTradeLink *menu = context;

    menu->partyReceivedMask |= 1 << sender;
    memcpy(SpinTradeLink_GetReceivedParty(menu, sender), data, 0x590);
}

static void SpinTradeLink_HandleLeaderMenuState(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (sender == 0) {
        menu->leaderJoinPending = *(u8 *)data;
    }
}

static void SpinTradeLink_HandleSyncCount(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->count++;
}

static void SpinTradeLink_HandleStartSync(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() != 0) {
        menu->count = 0;
        menu->state = 1;
        SpinTradeGroup_HandleLinkEvent(menu->parent->groupApp, 31, (u8)sender);
    }
}

static void SpinTradeLink_HandleClearMessage(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTradeGroup_HandleLinkEvent(menu->parent->groupApp, 2, 0);
}

static void SpinTradeLink_HandleJoinRequest(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTradeJoinRequest *src = data;
    if (sender != 0) {
        SpinTradeJoinRequest packet;
        if (Comm_GetLocalNetId() != 0) {
            return;
        }
        packet = *src;
        packet.sender = sender;
        packet.count = menu->participantCount;
        switch (src->command) {
        case 0:
            if (menu->participantCount != Comm_CountConnected() || menu->participantCount != SpinTradeLink_CountMemberProfiles() || menu->participantCount != MATH_CountPopulation(Wireless_GetConnectedBitmap())) {
                packet.accepted = 0;
            } else {
                menu->leavingMask |= 1 << sender;
                packet.accepted = 1;
                CommState_SetMaxEntries(Comm_CountConnected());
            }
            break;
        case 1:
            break;
        }
        SpinTradeLink_SendCommand(menu, 2, &packet, 4);
        return;
    }
    switch (src->command) {
    case 0: {
        int id = src->sender;
        if (id != Comm_GetLocalNetId()) {
            return;
        }
        if (src->accepted == 0) {
            SpinTradeGroup_HandleLinkEvent(menu->parent->groupApp, 8, id);
            return;
        }
        menu->assignedCount = src->count;
        SpinTradeGroup_HandleLinkEvent(menu->parent->groupApp, 7, src->sender);
        return;
    }
    case 1:
        SpinTradeGroup_HandleLinkEvent(menu->parent->groupApp, 19, src->sender);
        break;
    }
}

static void SpinTradeLink_HandleMemberJoined(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTradeGroup_HandleMemberEvent(menu->parent->groupApp, 1, *(u8 *)data);
    if (Comm_GetLocalNetId() == 0) {
        menu->unk34 = 0;
    }
}

static void SpinTradeLink_HandleCloseYesNo(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() != 0) {
        SpinTradeGroup_HandleLinkEvent(menu->parent->groupApp, 13, 0);
    }
}

static void SpinTradeLink_HandleJoinNotice(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() == 0) {
        u8 id = sender;
        SpinTradeLink_SendCommand(menu, 0, &id, 1);
    }
}

static void SpinTradeLink_HandleStatus(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->receivedStatus |= *(u16 *)data;
}

static void SpinTradeLink_HandleJoin(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->memberMask |= 1 << sender;
}

static void SpinTradeLink_HandleSlot(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveMemberSlot(menu->parent->tradeApp, data);
}

static void SpinTradeLink_HandleCount(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceivePlayerCount(menu->parent->tradeApp, *(u32 *)data);
}

static void SpinTradeLink_HandleJump(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveJump(menu->parent->tradeApp, *(u8 *)data);
}

static void SpinTradeLink_HandleSpin(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() != 0) {
        SpinTrade_ReceiveSpinData(menu->parent->tradeApp, data);
    }
}

static void SpinTradeLink_HandleTradeSlot(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveTradeSlot(menu->parent->tradeApp, sender, *(u32 *)data);
}

static void SpinTradeLink_HandleResult(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveResult(menu->parent->tradeApp, data);
}

static void SpinTradeLink_HandleCancel(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->cancelMask |= 1 << sender;
}

static void SpinTradeLink_HandleConfirm(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->confirmMask |= 1 << sender;
}

static u32 SpinTradeLink_GetPacketSize(void) {
    return 24;
}

static u32 SpinTradeLink_GetPartySize(void) {
    return 0x590;
}

static void *SpinTradeLink_GetPartyRecvBuffer(int sender, void *context, int size) {
    SpinTradeLink *menu = context;

    return menu->sendBuffer + sender * 0x590;
}

int SpinTradeLink_CountMemberProfiles(void) {
    int i, count = 0;
    for (i = 0; i < 5; i++) {
        if (CommPlayerInfo_GetProfile(i) != NULL) {
            count++;
        }
    }
    return count;
}

void *SpinTradeLink_GetReceivedParty(SpinTradeLink *menu, int sender) {
    return menu->receiveBuffer + sender * 0x590;
}
