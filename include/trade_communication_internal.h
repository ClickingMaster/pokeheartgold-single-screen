#ifndef POKEHEARTGOLD_TRADE_COMMUNICATION_INTERNAL_H
#define POKEHEARTGOLD_TRADE_COMMUNICATION_INTERNAL_H
#include "global.h"

#include "save.h"
#include "save_palpad.h"
#include "save_special_ribbons.h"
#include "communication_command.h"
#include "communication_core.h"
#include "comm_state.h"
#include "wireless_trade_select_mon_internal.h"

// The field system as seen by the link command handlers: the selection screen's work is kept in the field
// system's filler_88 while the screen runs (WirelessTrade_SetWork).
typedef struct TradeContext {
    u8 pad0[12];
    SaveData *saveData;
    u8 pad10[0x78];
    WirelessTradeSelectMon *work;
} TradeContext;
typedef void (*WirelessTradeRecvFunc)();

typedef struct WirelessTradeCommand {
    WirelessTradeRecvFunc receive;
    u32 (*getSize)(void);
    void *(*getBuffer)(int sender, TradeContext *context);
} WirelessTradeCommand;

void WirelessTradeSelectMon_SendCommandIfChanged(WirelessTradeSelectMon *, int, int);
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
void WirelessTrade_SetWork(TradeContext *context, WirelessTradeSelectMon *work);
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
void WirelessTrade_SetWork(TradeContext *context, WirelessTradeSelectMon *work);
u32 WirelessTrade_GetPacketSize(void);
u32 WirelessTrade_GetSpecialRibbonsSize(void);
u32 WirelessTrade_GetPalPadSize(void);
u32 WirelessTrade_GetPartyChunkSize(void);
void *WirelessTrade_GetRecvBuffer(int sender, TradeContext *context);

#endif
