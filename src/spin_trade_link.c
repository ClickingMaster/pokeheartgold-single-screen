#include "spin_trade_link.h"

#include "global.h"

#include "communication_player_state.h"
#include "heap.h"
#include "wireless_manager.h"
#include "communication_command.h"
#include "unk_02035900.h"
#include "comm_state.h"
#include "union_room_talk.h"

typedef struct SpinTradeLinkPacket {
    u32 command;
    u8 data[20];
} SpinTradeLinkPacket;

typedef struct SpinTradeLink {
    SpinTradeArgs *parent;
    SpinTradeLinkPacket packet;
    u32 blocked, count, state, remoteValue, participantCount, participantMask, unk34;
    u16 unk38;
    u8 pad3a[6];
    u16 receivedStatus, memberMask, flags44;
    s16 partyReceivedMask;
    u16 cancelMask, confirmMask;
    u8 *sendBuffer, *receiveBuffer;
    u32 unk54;
} SpinTradeLink;

typedef struct SpinTradeJoinRequest {
    u8 sender, count, command, accepted;
} SpinTradeJoinRequest;

u32 MATH_CountPopulation(u32 value);
void CommState_SetMaxEntries(int count);
void UnionRoomObjects_ResetSlots(void *ptr);
void SpinTradeGroup_HandleLinkEvent(void *ptr, int event, int value);
void SpinTradeGroup_HandleMemberEvent(void *ptr, int event, int value);
void SpinTrade_ReceiveMemberSlot(void *ptr, void *data);
void SpinTrade_ReceivePlayerCount(void *ptr, u32 data);
void SpinTrade_ReceiveJump(void *ptr, u8 data);
void SpinTrade_ReceiveSpinData(void *ptr, void *data);
void SpinTrade_ReceiveTradeSlot(void *ptr, int sender, u32 data);
void SpinTrade_ReceiveResult(void *ptr, void *data);
extern const u32 gSpinTradeLinkCommands[];
typedef void (*SpinTradeLinkPacketHandler)(int, int, void *, SpinTradeLink *);
extern const SpinTradeLinkPacketHandler gSpinTradeLinkPacketHandlers[];
void *SpinTradeLink_New(SpinTradeArgs *parent, enum HeapID heap);

void SpinTradeLink_RegisterCommands(SpinTradeLink *menu);

BOOL SpinTradeLink_SendPacket(SpinTradeLink *menu, u32 command, void *data, u32 size);
BOOL SpinTradeLink_SendCommand(SpinTradeLink *menu, u32 command, void *data, u32 size);
void SpinTradeLink_ReceivePacket(int sender, int unused, SpinTradeLinkPacket *packet, SpinTradeLink *menu);
void SpinTradeLink_ReceiveParty(int sender, int unused, void *data, SpinTradeLink *menu);
void sub_02096DA8(int sender, int unused, void *data, SpinTradeLink *menu);
void sub_02096DB4(int sender, int unused, void *data, SpinTradeLink *menu);
void sub_02096DBC(int sender, int unused, void *data, SpinTradeLink *menu);
void sub_02096DE4(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleJoinRequest(int sender, int unused, void *data, SpinTradeLink *menu);
void sub_02096ED0(int sender, int unused, void *data, SpinTradeLink *menu);
void sub_02096EF0(int sender, int unused, void *data, SpinTradeLink *menu);
void sub_02096F0C(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleStatus(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleJoin(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleSlot(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleCount(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleJump(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleSpin(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleTradeSlot(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleResult(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleCancel(int sender, int unused, void *data, SpinTradeLink *menu);
void SpinTradeLink_HandleConfirm(int sender, int unused, void *data, SpinTradeLink *menu);
u32 SpinTradeLink_GetPacketSize(void);
u32 SpinTradeLink_GetPartySize(void);
void *SpinTradeLink_GetPartyRecvBuffer(int sender, SpinTradeLink *menu);
int SpinTradeLink_CountMemberProfiles(void);
void *SpinTradeLink_GetReceivedParty(SpinTradeLink *menu, int sender);

void *SpinTradeLink_New(SpinTradeArgs *parent, enum HeapID heap) {
    SpinTradeLink *menu = Heap_Alloc(heap, sizeof(SpinTradeLink));
    GF_ASSERT(menu);
    memset(menu, 0, sizeof(SpinTradeLink));
    menu->parent = parent;
    menu->sendBuffer = Heap_Alloc(heap, 0x1bd0);
    menu->receiveBuffer = Heap_Alloc(heap, 0x1bd0);
    return menu;
}

void SpinTradeLink_Free(void *ptr) {
    SpinTradeLink *menu = ptr;
    Heap_Free(menu->sendBuffer);
    Heap_Free(menu->receiveBuffer);
    Heap_Free(menu);
}

void SpinTradeLink_RegisterCommands(SpinTradeLink *menu) {
    Comm_RegisterCommandTable(gSpinTradeLinkCommands, 0x87, menu);
}

void SpinTradeLink_Close(void *ptr) {
    SpinTradeLink *menu = ptr;
    CommState_SetMaxEntries(2);
    CommState_ResumeAlternateSearch();
    UnionRoomObjects_ResetSlots(menu->parent->input.communication);
    UnionRoom_SetBeaconActivity(0);
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

void SpinTradeLink_ReceivePacket(int sender, int unused, SpinTradeLinkPacket *packet, SpinTradeLink *menu) {
    if (packet->command >= 18) {
        GF_ASSERT(FALSE);
        return;
    }
    gSpinTradeLinkPacketHandlers[packet->command](sender, unused, packet->data, menu);
}

void SpinTradeLink_ReceiveParty(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->partyReceivedMask |= 1 << sender;
    memcpy(SpinTradeLink_GetReceivedParty(menu, sender), data, 0x590);
}

void sub_02096DA8(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (sender == 0) {
        menu->remoteValue = *(u8 *)data;
    }
}

void sub_02096DB4(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->count++;
}

void sub_02096DBC(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() != 0) {
        menu->count = 0;
        menu->state = 1;
        SpinTradeGroup_HandleLinkEvent(menu->parent->field38, 31, (u8)sender);
    }
}

void sub_02096DE4(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTradeGroup_HandleLinkEvent(menu->parent->field38, 2, 0);
}

void SpinTradeLink_HandleJoinRequest(int sender, int unused, void *data, SpinTradeLink *menu) {
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
                menu->participantMask |= 1 << sender;
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
            SpinTradeGroup_HandleLinkEvent(menu->parent->field38, 8, id);
            return;
        }
        menu->unk38 = src->count;
        SpinTradeGroup_HandleLinkEvent(menu->parent->field38, 7, src->sender);
        return;
    }
    case 1:
        SpinTradeGroup_HandleLinkEvent(menu->parent->field38, 19, src->sender);
        break;
    }
}

void sub_02096ED0(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTradeGroup_HandleMemberEvent(menu->parent->field38, 1, *(u8 *)data);
    if (Comm_GetLocalNetId() == 0) {
        menu->unk34 = 0;
    }
}

void sub_02096EF0(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() != 0) {
        SpinTradeGroup_HandleLinkEvent(menu->parent->field38, 13, 0);
    }
}

void sub_02096F0C(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() == 0) {
        u8 id = sender;
        SpinTradeLink_SendCommand(menu, 0, &id, 1);
    }
}

void SpinTradeLink_HandleStatus(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->receivedStatus |= *(u16 *)data;
}

void SpinTradeLink_HandleJoin(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->memberMask |= 1 << sender;
}

void SpinTradeLink_HandleSlot(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveMemberSlot(menu->parent->field34, data);
}

void SpinTradeLink_HandleCount(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceivePlayerCount(menu->parent->field34, *(u32 *)data);
}

void SpinTradeLink_HandleJump(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveJump(menu->parent->field34, *(u8 *)data);
}

void SpinTradeLink_HandleSpin(int sender, int unused, void *data, SpinTradeLink *menu) {
    if (Comm_GetLocalNetId() != 0) {
        SpinTrade_ReceiveSpinData(menu->parent->field34, data);
    }
}

void SpinTradeLink_HandleTradeSlot(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveTradeSlot(menu->parent->field34, sender, *(u32 *)data);
}

void SpinTradeLink_HandleResult(int sender, int unused, void *data, SpinTradeLink *menu) {
    SpinTrade_ReceiveResult(menu->parent->field34, data);
}

void SpinTradeLink_HandleCancel(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->cancelMask |= 1 << sender;
}

void SpinTradeLink_HandleConfirm(int sender, int unused, void *data, SpinTradeLink *menu) {
    menu->confirmMask |= 1 << sender;
}

u32 SpinTradeLink_GetPacketSize(void) {
    return 24;
}

u32 SpinTradeLink_GetPartySize(void) {
    return 0x590;
}

void *SpinTradeLink_GetPartyRecvBuffer(int sender, SpinTradeLink *menu) {
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
