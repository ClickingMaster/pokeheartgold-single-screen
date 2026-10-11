#include "trade_communication_internal.h"

void sub_0208F658(TradeContext *context) {
    Comm_RegisterCommandTable(_02105430, 11, context);
}

u32 sub_0208F66C(u32 slot) {
    if (slot == 12) {
        return 12;
    }
    if (slot < 6) {
        return slot + 6;
    }
    return slot - 6;
}

void sub_0208F680(int sender, int unused, void *data, TradeContext *context) {
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

void sub_0208F6F4(int sender, int unused, u8 *data, TradeContext *context) {
    TradeWork *work = context->work;
    if (sender != Comm_GetLocalNetId()) {
        work->selection = sub_0208F66C(*data);
    }
}

void sub_0208F714(int sender, int unused, u8 *data, TradeContext *context) {
    context->work->slots[sender] = *data;
}

void sub_0208F724(int sender, int unused, void *data, TradeContext *context) {
}

void sub_0208F728(int sender, int unused, void *data, TradeContext *context) {
}

void sub_0208F72C(int sender, int unused, void *data, TradeContext *context) {
    context->work->state = 2;
}

void sub_0208F738(int sender, int unused, void *data, TradeContext *context) {
}

void sub_0208F73C(int sender, int unused, u8 *data, TradeContext *context) {
    context->work->remoteState = *data;
}

void sub_0208F74C(int sender, int unused, u8 *data, TradeContext *context) {
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

void sub_0208F77C(int sender, int unused, SavePalPad *data, TradeContext *context) {
    TradeWork *work = context->work;
    if (sender != Comm_GetLocalNetId()) {
        SavePalPad_Merge(work->palPad, data, 1, (enum HeapID)26);
        work->state = 3;
    }
}

void sub_0208F7A8(int sender, int unused, void *data, TradeContext *context) {
    TradeWork *work = context->work;
    if (sender != Comm_GetLocalNetId()) {
        MIi_CpuCopyFast(data, work->packets[sender].data, 0x3e8);
        work->state = 4;
        sub_020378E4(0);
    }
}

void sub_0208F7E0(TradeContext *context, TradeWork *work) {
    context->work = work;
}

u32 sub_0208F7E8(void) {
    return sizeof(TradePacket);
}

u32 sub_0208F7F0(void) {
    return 14;
}

u32 sub_0208F7F4(void) {
    return sizeof(SavePalPad);
}

u32 sub_0208F7F8(void) {
    return 0x590;
}

void *sub_0208F800(int sender, TradeContext *context) {
    return context->work->buffers[sender];
}
