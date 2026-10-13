#ifndef POKEHEARTGOLD_TRADE_COMMUNICATION_INTERNAL_H
#define POKEHEARTGOLD_TRADE_COMMUNICATION_INTERNAL_H
#include "global.h"

#include "save.h"
#include "save_palpad.h"
#include "save_special_ribbons.h"
#include "communication_command.h"
#include "communication_core.h"
#include "comm_state.h"
typedef struct TradePacket {
    u32 data[0xFB];
} TradePacket;
typedef struct TradeWork {
    u8 pad0[0x5c];
    int sequence, state, count;
    u32 unk68;
    u32 slots[2];
    u8 pad74[0x24];
    u32 selection;
    u8 pad9c[0x2188];
    void *send, *receive;
    u32 unk222c;
    SavePalPad *palPad;
    u8 pad2234[0x98];
    u32 remoteState;
    u8 pad22d0[0x30];
    u8 buffers[2][0x590];
    TradePacket packets[2];
} TradeWork;
typedef struct TradeContext {
    u8 pad0[12];
    SaveData *saveData;
    u8 pad10[0x78];
    TradeWork *work;
} TradeContext;
extern const u32 gWirelessTradeCommands[];
void WirelessTradeSelectMon_SendCommandIfChanged(TradeWork *, int, int);
void WirelessTradeSelectMon_SendPartyChunk(int, void *, int);

void WirelessTrade_RegisterCommands(TradeContext *context);
u32 WirelessTrade_GetPartnerSlot(u32 slot);
void WirelessTrade_RecvPartyChunk(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvPartnerSelection(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvSlot(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvUnused3(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvUnused4(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvSetState2(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvUnused8(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvRemoteState(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvSpecialRibbons(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvPalPad(int sender, int unused, SavePalPad *data, TradeContext *context);
void WirelessTrade_RecvPacket(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_SetWork(TradeContext *context, TradeWork *work);
u32 WirelessTrade_GetPacketSize(void);
u32 WirelessTrade_GetSpecialRibbonsSize(void);
u32 WirelessTrade_GetPalPadSize(void);
u32 WirelessTrade_GetPartyChunkSize(void);
void *WirelessTrade_GetRecvBuffer(int sender, TradeContext *context);

void WirelessTrade_RegisterCommands(TradeContext *context);
u32 WirelessTrade_GetPartnerSlot(u32 slot);
void WirelessTrade_RecvPartyChunk(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvPartnerSelection(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvSlot(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvUnused3(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvUnused4(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvSetState2(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvUnused8(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_RecvRemoteState(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvSpecialRibbons(int sender, int unused, u8 *data, TradeContext *context);
void WirelessTrade_RecvPalPad(int sender, int unused, SavePalPad *data, TradeContext *context);
void WirelessTrade_RecvPacket(int sender, int unused, void *data, TradeContext *context);
void WirelessTrade_SetWork(TradeContext *context, TradeWork *work);
u32 WirelessTrade_GetPacketSize(void);
u32 WirelessTrade_GetSpecialRibbonsSize(void);
u32 WirelessTrade_GetPalPadSize(void);
u32 WirelessTrade_GetPartyChunkSize(void);
void *WirelessTrade_GetRecvBuffer(int sender, TradeContext *context);

#endif
