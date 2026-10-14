#include "trade_communication_internal.h"

void WirelessTrade_RegisterCommands(TradeContext *context) {
    Comm_RegisterCommandTable(gWirelessTradeCommands, 11, context);
}

u32 WirelessTrade_GetPartnerSlot(u32 slot) {
    if (slot == 12) {
        return 12;
    }
    if (slot < 6) {
        return slot + 6;
    }
    return slot - 6;
}

void WirelessTrade_RecvPartyChunk(int sender, int unused, void *data, TradeContext *context) {
    TradeWork *work = context->work;
    if (sender != Comm_GetLocalNetId()) {
        memcpy(work->receive, data, 0x590);
        work->count++;
        if (work->count * 0x590 >= 0x590) {
            if (Comm_GetLocalNetId() == 1) {
                WirelessTradeSelectMon_SendCommandIfChanged(work, 27, 0);
            } else {
                WirelessTradeSelectMon_SendPartyChunk(Comm_GetLocalNetId(), work->send, work->sequence);
            }
        } else {
            WirelessTradeSelectMon_SendPartyChunk(Comm_GetLocalNetId(), work->send, work->sequence);
        }
        work->sequence++;
    }
}

void WirelessTrade_RecvPartnerSelection(int sender, int unused, u8 *data, TradeContext *context) {
    TradeWork *work = context->work;
    if (sender != Comm_GetLocalNetId()) {
        work->selection = WirelessTrade_GetPartnerSlot(*data);
    }
}

void WirelessTrade_RecvSlot(int sender, int unused, u8 *data, TradeContext *context) {
    context->work->slots[sender] = *data;
}

void WirelessTrade_RecvUnused3(int sender, int unused, void *data, TradeContext *context) {
}

void WirelessTrade_RecvUnused4(int sender, int unused, void *data, TradeContext *context) {
}

void WirelessTrade_RecvSetState2(int sender, int unused, void *data, TradeContext *context) {
    context->work->state = 2;
}

void WirelessTrade_RecvUnused8(int sender, int unused, void *data, TradeContext *context) {
}

void WirelessTrade_RecvRemoteState(int sender, int unused, u8 *data, TradeContext *context) {
    context->work->remoteState = *data;
}

void WirelessTrade_RecvSpecialRibbons(int sender, int unused, u8 *data, TradeContext *context) {
    u8 *ribbons = Save_SpecialRibbons_Get(context->saveData)->ribbons;
    int i;
    if (sender != Comm_GetLocalNetId()) {
        for (i = 0; i < 14; i++) {
            if (data[i] && ribbons[i] != data[i]) {
                ribbons[i] = data[i];
            }
        }
    }
}

void WirelessTrade_RecvPalPad(int sender, int unused, SavePalPad *data, TradeContext *context) {
    TradeWork *work = context->work;
    if (sender != Comm_GetLocalNetId()) {
        SavePalPad_Merge(work->palPad, data, 1, (enum HeapID)26);
        work->state = 3;
    }
}

void WirelessTrade_RecvPacket(int sender, int unused, void *data, TradeContext *context) {
    TradeWork *work = context->work;
    if (sender != Comm_GetLocalNetId()) {
        MIi_CpuCopyFast(data, work->packets[sender].data, 0x3e8);
        work->state = 4;
        Comm_SetWifiContinuousSend(0);
    }
}

void WirelessTrade_SetWork(TradeContext *context, TradeWork *work) {
    context->work = work;
}

u32 WirelessTrade_GetPacketSize(void) {
    return sizeof(TradePacket);
}

u32 WirelessTrade_GetSpecialRibbonsSize(void) {
    return 14;
}

u32 WirelessTrade_GetPalPadSize(void) {
    return sizeof(SavePalPad);
}

u32 WirelessTrade_GetPartyChunkSize(void) {
    return 0x590;
}

void *WirelessTrade_GetRecvBuffer(int sender, TradeContext *context) {
    return context->work->buffers[sender];
}
