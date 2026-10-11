#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_BEACON_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_BEACON_H

#include "global.h"

// Download of a Mystery Gift from the beacons of a distribution station
// (src/application/main_menu/mystery_gift_beacon.c).

#define MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE 104

// one beacon of the station
typedef struct MysteryGiftBeaconPacket {
    u16 id;    // the gift's checksum
    u16 index; // 0xFFFF: the gift's title
    u32 size;  // of the gift
    u8 data[MYSTERY_GIFT_BEACON_PACKET_DATA_SIZE];
} MysteryGiftBeaconPacket;

// states (the names are the strings of the module's debug output, "mywh_SYSSTATE_*")
#define MYSTERY_GIFT_BEACON_STATE_STOP           0
#define MYSTERY_GIFT_BEACON_STATE_IDLE           1
#define MYSTERY_GIFT_BEACON_STATE_SCANNING       2
#define MYSTERY_GIFT_BEACON_STATE_BUSY           3
#define MYSTERY_GIFT_BEACON_STATE_CONNECTED      4
#define MYSTERY_GIFT_BEACON_STATE_DATASHARING    5
#define MYSTERY_GIFT_BEACON_STATE_KEYSHARING     6
#define MYSTERY_GIFT_BEACON_STATE_MEASURECHANNEL 7
#define MYSTERY_GIFT_BEACON_STATE_CONNECT_FAIL   8
#define MYSTERY_GIFT_BEACON_STATE_ERROR          9
#define MYSTERY_GIFT_BEACON_STATE_FATAL          10 // WM could not be started or ended (no name)

// events passed to the callback
#define MYSTERY_GIFT_BEACON_EVENT_SEARCHING 0
#define MYSTERY_GIFT_BEACON_EVENT_FOUND     1
#define MYSTERY_GIFT_BEACON_EVENT_COMPLETE  2
#define MYSTERY_GIFT_BEACON_EVENT_STOPPED   3
#define MYSTERY_GIFT_BEACON_EVENT_ERROR     4
#define MYSTERY_GIFT_BEACON_EVENT_RESTARTED 5 // the station switched to another gift

void MysteryGiftBeacon_SetState(int state);
void MysteryGiftBeacon_SetError(int error);
BOOL MysteryGiftBeacon_StartScan(void);
BOOL MysteryGiftBeacon_ScanChannel(void);
void MysteryGiftBeacon_ScanCallback(void *arg);
BOOL MysteryGiftBeacon_EndScan(void);
void MysteryGiftBeacon_EndScanCallback(void *arg);
void MysteryGiftBeacon_EndCallback(void *arg);
BOOL MysteryGiftBeacon_Start(void);
void MysteryGiftBeacon_IndicationCallback(void *arg);
BOOL MysteryGiftBeacon_Initialize(void);
void MysteryGiftBeacon_InitializeCallback(void *arg);
BOOL MysteryGiftBeacon_End(void);
BOOL MysteryGiftBeacon_Stop(void);
BOOL MysteryGiftBeacon_Reset(void);
void MysteryGiftBeacon_ResetCallback(void *arg);
BOOL MysteryGiftBeacon_Init(void *dest, void (*callback)(int event), void *memory);
void MysteryGiftBeacon_StartReceive(MysteryGiftBeaconPacket *packet);
BOOL MysteryGiftBeacon_ReceivePacket(MysteryGiftBeaconPacket *packet);
u16 MysteryGiftBeacon_Checksum(const u16 *data, u32 size);
BOOL MysteryGiftBeacon_DecryptGift(void);
u32 MysteryGiftBeacon_GetWorkSize(void);
const u16 *MysteryGiftBeacon_GetTitle(void);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_BEACON_H
